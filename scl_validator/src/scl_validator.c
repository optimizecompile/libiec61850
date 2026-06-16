/*
 * scl_validator.c - SCL Validator Main Implementation
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#include "scl_validator.h"
#include "scl_elements.h"
#include "scl_parser.h"
#include "scl_checker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Mini-XML header */
#include <mxml.h>

/* Internal helper functions */
static const char* severity_to_string(SclSeverityType severity);
static const char* issue_type_to_string(SclIssueType issue_type);

/* ==================== Validator Creation/Destruction ==================== */

SclValidator* SclValidator_Create(void)
{
    SclValidator* validator = (SclValidator*)calloc(1, sizeof(SclValidator));
    if (validator == NULL) {
        return NULL;
    }

    validator->message_count = 0;
    validator->error_count = 0;
    validator->warning_count = 0;
    validator->has_errors = false;
    validator->has_warnings = false;
    validator->ied_count = 0;
    validator->subnetwork_count = 0;
    validator->dataTypeTemplates = NULL;
    validator->header = NULL;
    validator->xml_doc = NULL;

    return validator;
}

void SclValidator_Destroy(SclValidator* validator)
{
    if (validator == NULL) {
        return;
    }

    /* Free XML document */
    if (validator->xml_doc != NULL) {
        mxmlDelete((mxml_node_t*)validator->xml_doc);
    }

    /* Free DataTypeTemplates */
    if (validator->dataTypeTemplates != NULL) {
        free(validator->dataTypeTemplates);
    }

    /* Free Header */
    if (validator->header != NULL) {
        free(validator->header);
    }

    /* Free IEDs */
    for (int i = 0; i < validator->ied_count; i++) {
        if (validator->ieds[i] != NULL) {
            SclParser_FreeIED(validator->ieds[i]);
        }
    }

    /* Free SubNetworks */
    for (int i = 0; i < validator->subnetwork_count; i++) {
        if (validator->subnetworks[i] != NULL) {
            SclParser_FreeSubNetwork(validator->subnetworks[i]);
        }
    }

    free(validator);
}

/* ==================== Message Handling ==================== */

void SclValidator_AddMessage(SclValidator* validator,
                             SclSeverityType severity,
                             SclIssueType issue_type,
                             int line_number,
                             const char* message,
                             const char* object_ref,
                             const char* issue_code)
{
    if (validator == NULL || validator->message_count >= SCL_MAX_MESSAGES) {
        return;
    }

    SclValidatorMessage* msg = &validator->messages[validator->message_count];
    msg->severity = severity;
    msg->issue_type = issue_type;
    msg->line_number = line_number;
    msg->line_position = 0;

    if (message != NULL) {
        strncpy(msg->message, message, SCL_MAX_DESC_LEN - 1);
        msg->message[SCL_MAX_DESC_LEN - 1] = '\0';
    } else {
        msg->message[0] = '\0';
    }

    if (object_ref != NULL) {
        strncpy(msg->object_ref, object_ref, SCL_MAX_OBJREF_LEN - 1);
        msg->object_ref[SCL_MAX_OBJREF_LEN - 1] = '\0';
    } else {
        msg->object_ref[0] = '\0';
    }

    if (issue_code != NULL) {
        strncpy(msg->issue_code, issue_code, SCL_MAX_NAME_LEN - 1);
        msg->issue_code[SCL_MAX_NAME_LEN - 1] = '\0';
    } else {
        msg->issue_code[0] = '\0';
    }

    validator->message_count++;

    if (severity == SCL_SEVERITY_ERROR) {
        validator->error_count++;
        validator->has_errors = true;
    } else if (severity == SCL_SEVERITY_WARNING) {
        validator->warning_count++;
        validator->has_warnings = true;
    }
}

const SclValidatorMessage* SclValidator_GetMessages(SclValidator* validator, int* count)
{
    if (validator == NULL) {
        if (count != NULL) {
            *count = 0;
        }
        return NULL;
    }

    if (count != NULL) {
        *count = validator->message_count;
    }

    return validator->messages;
}

bool SclValidator_HasErrors(SclValidator* validator)
{
    return (validator != NULL) ? validator->has_errors : false;
}

bool SclValidator_HasWarnings(SclValidator* validator)
{
    return (validator != NULL) ? validator->has_warnings : false;
}

int SclValidator_GetErrorCount(SclValidator* validator)
{
    return (validator != NULL) ? validator->error_count : 0;
}

int SclValidator_GetWarningCount(SclValidator* validator)
{
    return (validator != NULL) ? validator->warning_count : 0;
}

void SclValidator_Reset(SclValidator* validator)
{
    if (validator == NULL) {
        return;
    }

    /* Free XML document */
    if (validator->xml_doc != NULL) {
        mxmlDelete((mxml_node_t*)validator->xml_doc);
        validator->xml_doc = NULL;
    }

    /* Free DataTypeTemplates */
    if (validator->dataTypeTemplates != NULL) {
        free(validator->dataTypeTemplates);
        validator->dataTypeTemplates = NULL;
    }

    /* Free Header */
    if (validator->header != NULL) {
        free(validator->header);
        validator->header = NULL;
    }

    /* Free IEDs */
    for (int i = 0; i < validator->ied_count; i++) {
        if (validator->ieds[i] != NULL) {
            SclParser_FreeIED(validator->ieds[i]);
            validator->ieds[i] = NULL;
        }
    }
    validator->ied_count = 0;

    /* Free SubNetworks */
    for (int i = 0; i < validator->subnetwork_count; i++) {
        if (validator->subnetworks[i] != NULL) {
            SclParser_FreeSubNetwork(validator->subnetworks[i]);
            validator->subnetworks[i] = NULL;
        }
    }
    validator->subnetwork_count = 0;

    validator->message_count = 0;
    validator->error_count = 0;
    validator->warning_count = 0;
    validator->has_errors = false;
    validator->has_warnings = false;
    validator->filename[0] = '\0';
}

/* ==================== Validation Options ==================== */

SclValidationOptions SclValidator_GetDefaultOptions(void)
{
    SclValidationOptions options;
    options.check_schema = true;
    options.check_model_integrity = true;
    options.check_references = true;
    options.check_values = true;
    options.check_communication = true;
    options.strict_mode = false;
    options.verbose = false;
    return options;
}

void SclValidator_SetOptions(SclValidator* validator, const SclValidationOptions* options)
{
    /* Options are stored internally - placeholder for future implementation */
    (void)validator;
    (void)options;
}

/* ==================== File Validation ==================== */

bool SclValidator_ValidateFile(SclValidator* validator, const char* filename)
{
    if (validator == NULL || filename == NULL) {
        return false;
    }

    /* Reset validator state */
    SclValidator_Reset(validator);

    /* Store filename */
    strncpy(validator->filename, filename, SCL_MAX_PATH_LEN - 1);
    validator->filename[SCL_MAX_PATH_LEN - 1] = '\0';

    /* Load XML file */
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_SCHEMA, -1,
                                "Cannot open file", filename, "FileOpenError");
        return false;
    }

    /* Parse XML using mxml */
    validator->xml_doc = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
    fclose(fp);

    if (validator->xml_doc == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_SCHEMA, -1,
                                "Failed to parse XML file", filename, "XmlParseError");
        return false;
    }

    /* Get root element */
    mxml_node_t* root = mxmlFindElement((mxml_node_t*)validator->xml_doc,
                                        (mxml_node_t*)validator->xml_doc,
                                        "SCL", NULL, NULL, MXML_DESCEND_FIRST);

    if (root == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_SCHEMA, -1,
                                "No SCL root element found", NULL, "NoRootElement");
        return false;
    }

    /* Check namespace */
    const char* xmlns = mxmlElementGetAttr(root, "xmlns");
    if (xmlns == NULL || strstr(xmlns, "61850") == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_WARNING, SCL_ISSUE_SCHEMA, -1,
                                "SCL namespace not properly declared", NULL, "NamespaceWarning");
    }

    /* Parse and validate Header */
    SclParser_ParseHeader(validator, root);

    /* Parse DataTypeTemplates */
    SclParser_ParseDataTypeTemplates(validator, root);

    /* Parse IEDs */
    SclParser_ParseIEDs(validator, root);

    /* Parse Communication section */
    SclParser_ParseCommunication(validator, root);

    /* Run validation checks */
    if (validator->dataTypeTemplates != NULL) {
        SclChecker_CheckDataTypeTemplates(validator);
    }

    /* Check IED model integrity */
    for (int i = 0; i < validator->ied_count; i++) {
        SclChecker_CheckIED(validator, validator->ieds[i]);
    }

    /* Check communication section */
    SclChecker_CheckCommunication(validator);

    /* Check references between elements */
    SclChecker_CheckReferences(validator);

    return !validator->has_errors;
}

bool SclValidator_ValidateBuffer(SclValidator* validator, const char* buffer, size_t buffer_size)
{
    if (validator == NULL || buffer == NULL || buffer_size == 0) {
        return false;
    }

    /* Reset validator state */
    SclValidator_Reset(validator);

    /* Parse XML from buffer */
    validator->xml_doc = mxmlLoadString(NULL, buffer, MXML_NO_CALLBACK);

    if (validator->xml_doc == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_SCHEMA, -1,
                                "Failed to parse XML buffer", NULL, "XmlParseError");
        return false;
    }

    /* Get root element */
    mxml_node_t* root = mxmlFindElement((mxml_node_t*)validator->xml_doc,
                                        (mxml_node_t*)validator->xml_doc,
                                        "SCL", NULL, NULL, MXML_DESCEND_FIRST);

    if (root == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_SCHEMA, -1,
                                "No SCL root element found", NULL, "NoRootElement");
        return false;
    }

    /* Parse and validate all sections */
    SclParser_ParseHeader(validator, root);
    SclParser_ParseDataTypeTemplates(validator, root);
    SclParser_ParseIEDs(validator, root);
    SclParser_ParseCommunication(validator, root);

    /* Run validation checks */
    if (validator->dataTypeTemplates != NULL) {
        SclChecker_CheckDataTypeTemplates(validator);
    }

    for (int i = 0; i < validator->ied_count; i++) {
        SclChecker_CheckIED(validator, validator->ieds[i]);
    }

    SclChecker_CheckCommunication(validator);
    SclChecker_CheckReferences(validator);

    return !validator->has_errors;
}

/* ==================== Report Printing ==================== */

static const char* severity_to_string(SclSeverityType severity)
{
    switch (severity) {
        case SCL_SEVERITY_ERROR:   return "ERROR";
        case SCL_SEVERITY_WARNING: return "WARNING";
        case SCL_SEVERITY_INFO:    return "INFO";
        default:                   return "UNKNOWN";
    }
}

static const char* issue_type_to_string(SclIssueType issue_type)
{
    switch (issue_type) {
        case SCL_ISSUE_SCHEMA:           return "Schema";
        case SCL_ISSUE_MODEL_INTEGRITY:  return "Model integrity";
        case SCL_ISSUE_MISSING_ATTRIBUTE: return "Missing attribute";
        case SCL_ISSUE_INVALID_VALUE:    return "Invalid value";
        case SCL_ISSUE_REFERENCE:        return "Reference";
        case SCL_ISSUE_STRUCTURE:        return "Structure";
        default:                         return "Unknown";
    }
}

void SclValidator_PrintReport(SclValidator* validator, FILE* output)
{
    if (validator == NULL) {
        return;
    }

    if (output == NULL) {
        output = stdout;
    }

    fprintf(output, "\n========================================\n");
    fprintf(output, "SCL Validation Report\n");
    fprintf(output, "========================================\n\n");

    if (validator->filename[0] != '\0') {
        fprintf(output, "File: %s\n", validator->filename);
    }

    fprintf(output, "Total messages: %d\n", validator->message_count);
    fprintf(output, "Errors: %d\n", validator->error_count);
    fprintf(output, "Warnings: %d\n", validator->warning_count);
    fprintf(output, "\n");

    if (validator->message_count == 0) {
        fprintf(output, "No issues found. Validation passed.\n");
    } else {
        fprintf(output, "Issues:\n");
        fprintf(output, "----------------------------------------\n");

        for (int i = 0; i < validator->message_count; i++) {
            const SclValidatorMessage* msg = &validator->messages[i];

            fprintf(output, "[%s] [%s] ", severity_to_string(msg->severity),
                                          issue_type_to_string(msg->issue_type));

            if (msg->line_number > 0) {
                fprintf(output, "Line %d: ", msg->line_number);
            }

            fprintf(output, "%s\n", msg->message);

            if (msg->object_ref[0] != '\0') {
                fprintf(output, "  Object: %s\n", msg->object_ref);
            }

            if (msg->issue_code[0] != '\0') {
                fprintf(output, "  Code: %s\n", msg->issue_code);
            }

            fprintf(output, "\n");
        }
    }

    fprintf(output, "========================================\n");

    if (validator->has_errors) {
        fprintf(output, "Validation FAILED\n");
    } else if (validator->has_warnings) {
        fprintf(output, "Validation PASSED with warnings\n");
    } else {
        fprintf(output, "Validation PASSED\n");
    }
    fprintf(output, "========================================\n\n");
}