/*
 * scl_validator.h - SCL Validator Main Interface
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#ifndef SCL_VALIDATOR_H_
#define SCL_VALIDATOR_H_

#include "scl_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Validation message structure
 */
struct SclValidatorMessage {
    SclSeverityType severity;       /* ERROR, WARNING, INFO */
    SclIssueType issue_type;        /* Issue category */
    int line_number;                /* XML line number */
    int line_position;              /* XML line position */
    char message[SCL_MAX_DESC_LEN]; /* Error description */
    char object_ref[SCL_MAX_OBJREF_LEN]; /* Object reference */
    char issue_code[SCL_MAX_NAME_LEN];   /* Issue code for categorization */
};

/**
 * @brief Main validator structure
 */
struct SclValidator {
    /* Validation messages */
    SclValidatorMessage messages[SCL_MAX_MESSAGES];
    int message_count;
    int error_count;
    int warning_count;

    /* File information */
    char filename[SCL_MAX_PATH_LEN];
    int current_line;

    /* SCL Document structure */
    SclHeader *header;
    SclIED *ieds[SCL_MAX_IEDS];
    int ied_count;

    SclSubNetwork *subnetworks[SCL_MAX_SUBNETWORKS];
    int subnetwork_count;

    SclDataTypeTemplates *dataTypeTemplates;

    /* Internal state */
    bool has_errors;
    bool has_warnings;
    void *xml_doc;              /* mxml document pointer */
};

/**
 * @brief Create a new validator instance
 * @return Pointer to new validator, NULL on failure
 */
SclValidator* SclValidator_Create(void);

/**
 * @brief Destroy validator and free all resources
 * @param validator Validator instance to destroy
 */
void SclValidator_Destroy(SclValidator* validator);

/**
 * @brief Validate an SCL file
 * @param validator Validator instance
 * @param filename Path to SCL file (ICD/CID/SCD)
 * @return true if validation passed (no errors), false otherwise
 */
bool SclValidator_ValidateFile(SclValidator* validator, const char* filename);

/**
 * @brief Validate SCL from memory buffer
 * @param validator Validator instance
 * @param buffer Buffer containing SCL XML content
 * @param buffer_size Size of buffer
 * @return true if validation passed, false otherwise
 */
bool SclValidator_ValidateBuffer(SclValidator* validator, const char* buffer, size_t buffer_size);

/**
 * @brief Add a validation message
 * @param validator Validator instance
 * @param severity Message severity
 * @param issue_type Issue type
 * @param line_number XML line number (-1 if unknown)
 * @param message Error message
 * @param object_ref Object reference (optional)
 * @param issue_code Issue code (optional)
 */
void SclValidator_AddMessage(SclValidator* validator,
                             SclSeverityType severity,
                             SclIssueType issue_type,
                             int line_number,
                             const char* message,
                             const char* object_ref,
                             const char* issue_code);

/**
 * @brief Get all validation messages
 * @param validator Validator instance
 * @param count Output: number of messages
 * @return Array of messages (owned by validator, do not free)
 */
const SclValidatorMessage* SclValidator_GetMessages(SclValidator* validator, int* count);

/**
 * @brief Print validation report to file
 * @param validator Validator instance
 * @param output Output file (stdout if NULL)
 */
void SclValidator_PrintReport(SclValidator* validator, FILE* output);

/**
 * @brief Check if validation has errors
 * @param validator Validator instance
 * @return true if there are errors
 */
bool SclValidator_HasErrors(SclValidator* validator);

/**
 * @brief Check if validation has warnings
 * @param validator Validator instance
 * @return true if there are warnings
 */
bool SclValidator_HasWarnings(SclValidator* validator);

/**
 * @brief Get error count
 * @param validator Validator instance
 * @return Number of errors
 */
int SclValidator_GetErrorCount(SclValidator* validator);

/**
 * @brief Get warning count
 * @param validator Validator instance
 * @return Number of warnings
 */
int SclValidator_GetWarningCount(SclValidator* validator);

/**
 * @brief Clear all messages and reset validator
 * @param validator Validator instance
 */
void SclValidator_Reset(SclValidator* validator);

/* Validation control options */
typedef struct {
    bool check_schema;          /* Enable XSD schema validation */
    bool check_model_integrity; /* Enable model integrity checks */
    bool check_references;      /* Enable reference validation */
    bool check_values;          /* Enable value range/format checks */
    bool check_communication;   /* Enable communication section checks */
    bool strict_mode;           /* Treat warnings as errors */
    bool verbose;               /* Enable verbose output */
} SclValidationOptions;

/**
 * @brief Set validation options
 * @param validator Validator instance
 * @param options Validation options
 */
void SclValidator_SetOptions(SclValidator* validator, const SclValidationOptions* options);

/**
 * @brief Get default validation options
 * @return Default options structure
 */
SclValidationOptions SclValidator_GetDefaultOptions(void);

#ifdef __cplusplus
}
#endif

#endif /* SCL_VALIDATOR_H_ */