/*
 * scl_validate.c - SCL Validator Command Line Tool
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#include "scl_validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static void print_usage(const char* program_name)
{
    printf("SCL Validator for IEC 61850 - CCD/CID/ICD/SCD File Validation Tool\n\n");
    printf("Usage: %s <scl_file> [options]\n\n", program_name);
    printf("Options:\n");
    printf("  -h, --help              Show this help message\n");
    printf("  -o, --output <file>     Write report to file (default: stdout)\n");
    printf("  -q, --quiet             Quiet mode - only show errors\n");
    printf("  -v, --verbose           Verbose mode - show all messages\n");
    printf("  -s, --strict            Strict mode - treat warnings as errors\n");
    printf("  -c, --check <type>      Enable specific check type:\n");
    printf("                          all, schema, integrity, references, values, communication\n");
    printf("  --no-schema             Skip XSD schema validation\n");
    printf("  --no-integrity          Skip model integrity checks\n");
    printf("  --no-references         Skip reference validation\n");
    printf("  --no-values             Skip value range/format checks\n");
    printf("  --no-communication      Skip communication section checks\n");
    printf("\nExamples:\n");
    printf("  %s device.icd\n", program_name);
    printf("  %s project.scd -o report.txt\n", program_name);
    printf("  %s device.cid --strict -v\n", program_name);
    printf("\n");
}

static void print_version(void)
{
    printf("SCL Validator version 1.0.0\n");
    printf("Copyright 2025 - IEC 61850 SCL Validation Tool\n");
    printf("Using Mini-XML (mxml) for XML parsing\n");
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    /* Default options */
    SclValidationOptions options = SclValidator_GetDefaultOptions();
    bool quiet_mode = false;
    bool verbose_mode = false;
    char* output_file = NULL;

    /* Parse command line options */
    static struct option long_options[] = {
        {"help",           no_argument,       NULL, 'h'},
        {"version",        no_argument,       NULL, 'V'},
        {"output",         required_argument, NULL, 'o'},
        {"quiet",          no_argument,       NULL, 'q'},
        {"verbose",        no_argument,       NULL, 'v'},
        {"strict",         no_argument,       NULL, 's'},
        {"check",          required_argument, NULL, 'c'},
        {"no-schema",      no_argument,       NULL,  1 },
        {"no-integrity",   no_argument,       NULL,  2 },
        {"no-references",  no_argument,       NULL,  3 },
        {"no-values",      no_argument,       NULL,  4 },
        {"no-communication", no_argument,     NULL,  5 },
        {NULL,             0,                 NULL,  0 }
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "hVo:qvs", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                print_usage(argv[0]);
                return 0;

            case 'V':
                print_version();
                return 0;

            case 'o':
                output_file = optarg;
                break;

            case 'q':
                quiet_mode = true;
                break;

            case 'v':
                verbose_mode = true;
                options.verbose = true;
                break;

            case 's':
                options.strict_mode = true;
                break;

            case 'c':
                /* Enable specific check type */
                if (strcmp(optarg, "all") == 0) {
                    options.check_schema = true;
                    options.check_model_integrity = true;
                    options.check_references = true;
                    options.check_values = true;
                    options.check_communication = true;
                } else if (strcmp(optarg, "schema") == 0) {
                    options.check_schema = true;
                } else if (strcmp(optarg, "integrity") == 0) {
                    options.check_model_integrity = true;
                } else if (strcmp(optarg, "references") == 0) {
                    options.check_references = true;
                } else if (strcmp(optarg, "values") == 0) {
                    options.check_values = true;
                } else if (strcmp(optarg, "communication") == 0) {
                    options.check_communication = true;
                } else {
                    fprintf(stderr, "Unknown check type: %s\n", optarg);
                    return 1;
                }
                break;

            case 1:  /* --no-schema */
                options.check_schema = false;
                break;

            case 2:  /* --no-integrity */
                options.check_model_integrity = false;
                break;

            case 3:  /* --no-references */
                options.check_references = false;
                break;

            case 4:  /* --no-values */
                options.check_values = false;
                break;

            case 5:  /* --no-communication */
                options.check_communication = false;
                break;

            case '?':
                return 1;

            default:
                break;
        }
    }

    /* Check if file argument is provided */
    if (optind >= argc) {
        fprintf(stderr, "Error: No SCL file specified\n");
        print_usage(argv[0]);
        return 1;
    }

    const char* scl_file = argv[optind];

    /* Create validator */
    SclValidator* validator = SclValidator_Create();
    if (validator == NULL) {
        fprintf(stderr, "Error: Failed to create validator\n");
        return 1;
    }

    /* Set options */
    SclValidator_SetOptions(validator, &options);

    /* Print header */
    if (!quiet_mode) {
        printf("\n");
        printf("========================================\n");
        printf("SCL Validator for IEC 61850\n");
        printf("========================================\n");
        printf("Input file: %s\n", scl_file);
        printf("\n");
    }

    /* Validate file */
    bool success = SclValidator_ValidateFile(validator, scl_file);

    /* Print report */
    FILE* output = stdout;
    if (output_file != NULL) {
        output = fopen(output_file, "w");
        if (output == NULL) {
            fprintf(stderr, "Error: Cannot open output file %s\n", output_file);
            SclValidator_Destroy(validator);
            return 1;
        }
    }

    if (quiet_mode) {
        /* Only print summary */
        if (SclValidator_HasErrors(validator)) {
            fprintf(output, "FAILED: %d errors found\n", SclValidator_GetErrorCount(validator));
        } else if (SclValidator_HasWarnings(validator)) {
            fprintf(output, "PASSED: %d warnings found\n", SclValidator_GetWarningCount(validator));
        } else {
            fprintf(output, "PASSED: No issues found\n");
        }
    } else {
        SclValidator_PrintReport(validator, output);
    }

    if (output_file != NULL) {
        fclose(output);
        if (!quiet_mode) {
            printf("Report written to: %s\n", output_file);
        }
    }

    /* Cleanup */
    SclValidator_Destroy(validator);

    /* Return status */
    if (success) {
        return 0;  /* Validation passed */
    } else {
        return 2;  /* Validation failed */
    }
}