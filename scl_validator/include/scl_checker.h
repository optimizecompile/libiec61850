/*
 * scl_checker.h - SCL Validation Checker Interface
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#ifndef SCL_CHECKER_H_
#define SCL_CHECKER_H_

#include "scl_validator.h"
#include "scl_elements.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Check DataTypeTemplates section
 * @param validator Validator instance
 */
void SclChecker_CheckDataTypeTemplates(SclValidator* validator);

/**
 * @brief Check IED structure and integrity
 * @param validator Validator instance
 * @param ied IED to check
 */
void SclChecker_CheckIED(SclValidator* validator, SclIED* ied);

/**
 * @brief Check Communication section
 * @param validator Validator instance
 */
void SclChecker_CheckCommunication(SclValidator* validator);

/**
 * @brief Check cross-references between elements
 * @param validator Validator instance
 */
void SclChecker_CheckReferences(SclValidator* validator);

/**
 * @brief Check DAI value validity
 * @param validator Validator instance
 * @param dai DAI to check
 * @param ln LogicalNode containing the DAI
 * @param doi DOI containing the DAI
 */
void SclChecker_CheckDAIValue(SclValidator* validator, SclDAI* dai, SclLN* ln, SclDOI* doi);

/**
 * @brief Check P address value validity
 * @param validator Validator instance
 * @param p P element to check
 */
void SclChecker_CheckPValue(SclValidator* validator, SclP* p);

/**
 * @brief Check if string is valid Base64
 * @param value String to check
 * @return true if valid Base64
 */
bool SclChecker_IsBase64String(const char* value);

/**
 * @brief Check if string is valid hex
 * @param value String to check
 * @return true if valid hex string
 */
bool SclChecker_IsHexString(const char* value);

/**
 * @brief Check if value is within integer type range
 * @param value String value
 * @param btype Basic type
 * @return true if valid
 */
bool SclChecker_CheckIntegerValue(const char* value, SclBTypeType btype);

/**
 * @brief Check if enum value exists in enum type
 * @param validator Validator instance
 * @param value Enum value string
 * @param enum_type_id Enum type ID
 * @return true if value exists in enum type
 */
bool SclChecker_CheckEnumValue(SclValidator* validator, const char* value, const char* enum_type_id);

/**
 * @brief Find DOType by ID
 * @param validator Validator instance
 * @param id DOType ID
 * @return Pointer to DOType or NULL
 */
SclDOType* SclChecker_FindDOType(SclValidator* validator, const char* id);

/**
 * @brief Find DAType by ID
 * @param validator Validator instance
 * @param id DAType ID
 * @return Pointer to DAType or NULL
 */
SclDAType* SclChecker_FindDAType(SclValidator* validator, const char* id);

/**
 * @brief Find LNodeType by ID
 * @param validator Validator instance
 * @param id LNodeType ID
 * @return Pointer to LNodeType or NULL
 */
SclLNodeType* SclChecker_FindLNodeType(SclValidator* validator, const char* id);

/**
 * @brief Find EnumType by ID
 * @param validator Validator instance
 * @param id EnumType ID
 * @return Pointer to EnumType or NULL
 */
SclEnumType* SclChecker_FindEnumType(SclValidator* validator, const char* id);

/**
 * @brief Find IED by name
 * @param validator Validator instance
 * @param name IED name
 * @return Pointer to IED or NULL
 */
SclIED* SclChecker_FindIED(SclValidator* validator, const char* name);

#ifdef __cplusplus
}
#endif

#endif /* SCL_CHECKER_H_ */