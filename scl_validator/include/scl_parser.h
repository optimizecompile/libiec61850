/*
 * scl_parser.h - SCL XML Parser Interface
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#ifndef SCL_PARSER_H_
#define SCL_PARSER_H_

#include "scl_validator.h"
#include "scl_elements.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse Header element
 * @param validator Validator instance
 * @param parent Parent XML node (SCL root)
 */
void SclParser_ParseHeader(SclValidator* validator, void* parent);

/**
 * @brief Parse DataTypeTemplates section
 * @param validator Validator instance
 * @param parent Parent XML node (SCL root)
 */
void SclParser_ParseDataTypeTemplates(SclValidator* validator, void* parent);

/**
 * @brief Parse all IED elements
 * @param validator Validator instance
 * @param parent Parent XML node (SCL root)
 */
void SclParser_ParseIEDs(SclValidator* validator, void* parent);

/**
 * @brief Parse Communication section
 * @param validator Validator instance
 * @param parent Parent XML node (SCL root)
 */
void SclParser_ParseCommunication(SclValidator* validator, void* parent);

/**
 * @brief Free IED structure
 * @param ied IED to free
 */
void SclParser_FreeIED(SclIED* ied);

/**
 * @brief Free SubNetwork structure
 * @param subnetwork SubNetwork to free
 */
void SclParser_FreeSubNetwork(SclSubNetwork* subnetwork);

/* Helper functions for attribute parsing */
const char* SclParser_GetAttr(void* node, const char* name);
int SclParser_GetAttrInt(void* node, const char* name, int default_value);
uint32_t SclParser_GetAttrUInt(void* node, const char* name, uint32_t default_value);
bool SclParser_GetAttrBool(void* node, const char* name, bool default_value);
SclFCType SclParser_ParseFC(const char* fc_str);
SclBTypeType SclParser_ParseBType(const char* btype_str);
SclPTypeType SclParser_ParsePType(const char* ptype_str);
int SclParser_GetLineNumber(void* node);

#ifdef __cplusplus
}
#endif

#endif /* SCL_PARSER_H_ */