/*
 * scl_checker.c - SCL Validation Checker Implementation
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#include "scl_checker.h"
#include "scl_validator.h"
#include "scl_elements.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

/* ==================== Helper Functions ==================== */

bool SclChecker_IsHexString(const char* value)
{
    if (value == NULL) {
        return false;
    }

    for (const char* p = value; *p != '\0'; p++) {
        if (!isxdigit(*p)) {
            return false;
        }
    }

    return true;
}

bool SclChecker_IsBase64String(const char* value)
{
    if (value == NULL || strlen(value) == 0 || strlen(value) % 4 != 0) {
        return false;
    }

    /* Check for whitespace */
    if (strchr(value, ' ') != NULL || strchr(value, '\t') != NULL ||
        strchr(value, '\r') != NULL || strchr(value, '\n') != NULL) {
        return false;
    }

    /* Check characters */
    int index = strlen(value) - 1;

    /* Skip padding */
    if (value[index] == '=') index--;
    if (value[index] == '=') index--;

    for (int i = 0; i <= index; i++) {
        char c = value[i];
        int iv = (int)c;

        /* Valid Base64 characters: A-Z, a-z, 0-9, +, / */
        if (!((iv >= 48 && iv <= 57) ||   /* 0-9 */
              (iv >= 65 && iv <= 90) ||   /* A-Z */
              (iv >= 97 && iv <= 122) ||  /* a-z */
              (iv == 43) || (iv == 47))) {
            return false;
        }
    }

    return true;
}

bool SclChecker_CheckIntegerValue(const char* value, SclBTypeType btype)
{
    if (value == NULL) {
        return false;
    }

    char* endptr;
    long long val = strtoll(value, &endptr, 10);

    if (*endptr != '\0') {
        return false;  /* Not a valid integer */
    }

    /* Check range based on type */
    switch (btype) {
        case SCL_BTYPE_INT8:
            return (val >= -128 && val <= 127);
        case SCL_BTYPE_INT16:
            return (val >= -32768 && val <= 32767);
        case SCL_BTYPE_INT32:
            return (val >= -2147483648LL && val <= 2147483647LL);
        case SCL_BTYPE_INT64:
            return true;  /* Any valid long long is OK */
        case SCL_BTYPE_INT8U:
            return (val >= 0 && val <= 255);
        case SCL_BTYPE_INT16U:
            return (val >= 0 && val <= 65535);
        case SCL_BTYPE_INT24U:
            return (val >= 0 && val <= 8388607);
        case SCL_BTYPE_INT32U:
            return (val >= 0 && val <= 4294967295ULL);
        default:
            return false;
    }
}

bool SclChecker_CheckEnumValue(SclValidator* validator, const char* value, const char* enum_type_id)
{
    if (validator == NULL || validator->dataTypeTemplates == NULL ||
        value == NULL || enum_type_id == NULL) {
        return false;
    }

    SclEnumType* enum_type = SclChecker_FindEnumType(validator, enum_type_id);
    if (enum_type == NULL) {
        return false;
    }

    /* Check if value exists in enum values */
    for (int i = 0; i < enum_type->value_count; i++) {
        if (strcmp(enum_type->values[i].symbolic_name, value) == 0) {
            return true;
        }
    }

    return false;
}

/* ==================== Type Lookup Functions ==================== */

SclDOType* SclChecker_FindDOType(SclValidator* validator, const char* id)
{
    if (validator == NULL || validator->dataTypeTemplates == NULL || id == NULL) {
        return NULL;
    }

    for (int i = 0; i < validator->dataTypeTemplates->do_type_count; i++) {
        if (strcmp(validator->dataTypeTemplates->do_types[i].id, id) == 0) {
            return &validator->dataTypeTemplates->do_types[i];
        }
    }

    return NULL;
}

SclDAType* SclChecker_FindDAType(SclValidator* validator, const char* id)
{
    if (validator == NULL || validator->dataTypeTemplates == NULL || id == NULL) {
        return NULL;
    }

    for (int i = 0; i < validator->dataTypeTemplates->da_type_count; i++) {
        if (strcmp(validator->dataTypeTemplates->da_types[i].id, id) == 0) {
            return &validator->dataTypeTemplates->da_types[i];
        }
    }

    return NULL;
}

SclLNodeType* SclChecker_FindLNodeType(SclValidator* validator, const char* id)
{
    if (validator == NULL || validator->dataTypeTemplates == NULL || id == NULL) {
        return NULL;
    }

    for (int i = 0; i < validator->dataTypeTemplates->lnode_type_count; i++) {
        if (strcmp(validator->dataTypeTemplates->lnode_types[i].id, id) == 0) {
            return &validator->dataTypeTemplates->lnode_types[i];
        }
    }

    return NULL;
}

SclEnumType* SclChecker_FindEnumType(SclValidator* validator, const char* id)
{
    if (validator == NULL || validator->dataTypeTemplates == NULL || id == NULL) {
        return NULL;
    }

    for (int i = 0; i < validator->dataTypeTemplates->enum_type_count; i++) {
        if (strcmp(validator->dataTypeTemplates->enum_types[i].id, id) == 0) {
            return &validator->dataTypeTemplates->enum_types[i];
        }
    }

    return NULL;
}

SclIED* SclChecker_FindIED(SclValidator* validator, const char* name)
{
    if (validator == NULL || name == NULL) {
        return NULL;
    }

    for (int i = 0; i < validator->ied_count; i++) {
        if (strcmp(validator->ieds[i]->name, name) == 0) {
            return validator->ieds[i];
        }
    }

    return NULL;
}

/* ==================== DataTypeTemplates Checking ==================== */

static void check_lnode_type_references(SclValidator* validator, SclLNodeType* lnode_type)
{
    for (int i = 0; i < lnode_type->data_object_count; i++) {
        SclDataObjectDefinition* do_def = &lnode_type->data_objects[i];

        SclDOType* do_type = SclChecker_FindDOType(validator, do_def->type);
        if (do_type == NULL) {
            char msg[SCL_MAX_DESC_LEN];
            snprintf(msg, SCL_MAX_DESC_LEN,
                     "DOType \"%s\" of DO \"%s\" in LNodeType \"%s\" is not defined",
                     do_def->type, do_def->name, lnode_type->id);
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                    msg, lnode_type->id, "DOTypeNotFound");
        } else {
            do_type->is_used = true;
        }
    }
}

static void check_do_type_references(SclValidator* validator, SclDOType* do_type)
{
    /* Check SDO references */
    for (int i = 0; i < do_type->data_object_count; i++) {
        SclDataObjectDefinition* sdo_def = &do_type->data_objects[i];

        SclDOType* sdo_type = SclChecker_FindDOType(validator, sdo_def->type);
        if (sdo_type == NULL) {
            char msg[SCL_MAX_DESC_LEN];
            snprintf(msg, SCL_MAX_DESC_LEN,
                     "DOType \"%s\" of SDO \"%s\" in DOType \"%s\" is not defined",
                     sdo_def->type, sdo_def->name, do_type->id);
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                    msg, do_type->id, "DOTypeNotFound");
        } else {
            sdo_type->is_used = true;
        }
    }

    /* Check DA references */
    for (int i = 0; i < do_type->data_attribute_count; i++) {
        SclDataAttributeDefinition* da_def = &do_type->data_attributes[i];

        if (da_def->btype == SCL_BTYPE_STRUCT) {
            SclDAType* da_type = SclChecker_FindDAType(validator, da_def->type);
            if (da_type == NULL) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "DAType \"%s\" of DA \"%s\" in DOType \"%s\" is not defined",
                         da_def->type, da_def->name, do_type->id);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                        msg, do_type->id, "DATypeNotFound");
            } else {
                da_type->is_used = true;
            }
        } else if (da_def->btype == SCL_BTYPE_ENUMERATED) {
            SclEnumType* enum_type = SclChecker_FindEnumType(validator, da_def->type);
            if (enum_type == NULL) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "EnumType \"%s\" of DA \"%s\" in DOType \"%s\" is not defined",
                         da_def->type, da_def->name, do_type->id);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                        msg, do_type->id, "EnumTypeNotFound");
            } else {
                enum_type->is_used = true;
            }
        }

        /* Check FC validity */
        if (da_def->fc == SCL_FC_UNKNOWN) {
            char msg[SCL_MAX_DESC_LEN];
            snprintf(msg, SCL_MAX_DESC_LEN,
                     "Invalid FC for DA \"%s\" in DOType \"%s\"",
                     da_def->name, do_type->id);
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                    msg, do_type->id, "InvalidFC");
        }
    }
}

static void check_da_type_references(SclValidator* validator, SclDAType* da_type)
{
    for (int i = 0; i < da_type->attribute_count; i++) {
        SclDataAttributeDefinition* bda_def = &da_type->attributes[i];

        if (bda_def->btype == SCL_BTYPE_STRUCT) {
            SclDAType* sub_da_type = SclChecker_FindDAType(validator, bda_def->type);
            if (sub_da_type == NULL) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "DAType \"%s\" of BDA \"%s\" in DAType \"%s\" is not defined",
                         bda_def->type, bda_def->name, da_type->id);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                        msg, da_type->id, "DATypeNotFound");
            } else {
                sub_da_type->is_used = true;
            }
        } else if (bda_def->btype == SCL_BTYPE_ENUMERATED) {
            SclEnumType* enum_type = SclChecker_FindEnumType(validator, bda_def->type);
            if (enum_type == NULL) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "EnumType \"%s\" of BDA \"%s\" in DAType \"%s\" is not defined",
                         bda_def->type, bda_def->name, da_type->id);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                        msg, da_type->id, "EnumTypeNotFound");
            } else {
                enum_type->is_used = true;
            }
        }
    }
}

static void check_unused_types(SclValidator* validator)
{
    /* Check for unused LNodeType definitions */
    for (int i = 0; i < validator->dataTypeTemplates->lnode_type_count; i++) {
        if (!validator->dataTypeTemplates->lnode_types[i].is_used) {
            char msg[SCL_MAX_DESC_LEN];
            snprintf(msg, SCL_MAX_DESC_LEN,
                     "LNodeType \"%s\" is defined but never referenced",
                     validator->dataTypeTemplates->lnode_types[i].id);
            SclValidator_AddMessage(validator, SCL_SEVERITY_WARNING, SCL_ISSUE_MODEL_INTEGRITY, -1,
                                    msg, validator->dataTypeTemplates->lnode_types[i].id, "UnusedLNodeType");
        }
    }

    /* Check for unused DOType definitions */
    for (int i = 0; i < validator->dataTypeTemplates->do_type_count; i++) {
        if (!validator->dataTypeTemplates->do_types[i].is_used) {
            char msg[SCL_MAX_DESC_LEN];
            snprintf(msg, SCL_MAX_DESC_LEN,
                     "DOType \"%s\" is defined but never referenced",
                     validator->dataTypeTemplates->do_types[i].id);
            SclValidator_AddMessage(validator, SCL_SEVERITY_WARNING, SCL_ISSUE_MODEL_INTEGRITY, -1,
                                    msg, validator->dataTypeTemplates->do_types[i].id, "UnusedDOType");
        }
    }

    /* Check for unused DAType definitions */
    for (int i = 0; i < validator->dataTypeTemplates->da_type_count; i++) {
        if (!validator->dataTypeTemplates->da_types[i].is_used) {
            char msg[SCL_MAX_DESC_LEN];
            snprintf(msg, SCL_MAX_DESC_LEN,
                     "DAType \"%s\" is defined but never referenced",
                     validator->dataTypeTemplates->da_types[i].id);
            SclValidator_AddMessage(validator, SCL_SEVERITY_WARNING, SCL_ISSUE_MODEL_INTEGRITY, -1,
                                    msg, validator->dataTypeTemplates->da_types[i].id, "UnusedDAType");
        }
    }

    /* Check for unused EnumType definitions */
    for (int i = 0; i < validator->dataTypeTemplates->enum_type_count; i++) {
        if (!validator->dataTypeTemplates->enum_types[i].is_used) {
            char msg[SCL_MAX_DESC_LEN];
            snprintf(msg, SCL_MAX_DESC_LEN,
                     "EnumType \"%s\" is defined but never referenced",
                     validator->dataTypeTemplates->enum_types[i].id);
            SclValidator_AddMessage(validator, SCL_SEVERITY_WARNING, SCL_ISSUE_MODEL_INTEGRITY, -1,
                                    msg, validator->dataTypeTemplates->enum_types[i].id, "UnusedEnumType");
        }
    }
}

void SclChecker_CheckDataTypeTemplates(SclValidator* validator)
{
    if (validator == NULL || validator->dataTypeTemplates == NULL) {
        return;
    }

    /* Check LNodeType references */
    for (int i = 0; i < validator->dataTypeTemplates->lnode_type_count; i++) {
        check_lnode_type_references(validator, &validator->dataTypeTemplates->lnode_types[i]);
    }

    /* Check DOType references */
    for (int i = 0; i < validator->dataTypeTemplates->do_type_count; i++) {
        check_do_type_references(validator, &validator->dataTypeTemplates->do_types[i]);
    }

    /* Check DAType references */
    for (int i = 0; i < validator->dataTypeTemplates->da_type_count; i++) {
        check_da_type_references(validator, &validator->dataTypeTemplates->da_types[i]);
    }

    /* Check for unused types */
    check_unused_types(validator);
}

/* ==================== IED Checking ==================== */

static void check_ln(SclValidator* validator, SclLN* ln, const char* ied_name, const char* ld_inst)
{
    char obj_ref[SCL_MAX_OBJREF_LEN];
    snprintf(obj_ref, SCL_MAX_OBJREF_LEN, "%s%s/%s%s%s",
             ied_name, ld_inst, ln->prefix, ln->ln_class, ln->inst);

    /* Check lnType reference */
    if (ln->ln_type[0] == '\0') {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "LN has no lnType attribute", obj_ref, "MissingLnType");
    } else {
        SclLNodeType* lnode_type = SclChecker_FindLNodeType(validator, ln->ln_type);
        if (lnode_type == NULL) {
            char msg[SCL_MAX_DESC_LEN];
            snprintf(msg, SCL_MAX_DESC_LEN,
                     "LNodeType \"%s\" referenced by LN is not defined", ln->ln_type);
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                    msg, obj_ref, "LNodeTypeNotFound");
        } else {
            lnode_type->is_used = true;
        }
    }

    /* Check lnClass */
    if (ln->ln_class[0] == '\0') {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "LN has no lnClass attribute", obj_ref, "MissingLnClass");
    }

    /* Check inst for non-LLN0 */
    if (strcmp(ln->ln_class, "LLN0") != 0 && ln->inst[0] == '\0') {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "LN has no inst attribute", obj_ref, "MissingInst");
    }

    /* Check DOI/DAI values */
    for (int i = 0; i < ln->doi_count; i++) {
        for (int j = 0; j < ln->dois[i].dai_count; j++) {
            SclChecker_CheckDAIValue(validator, &ln->dois[i].dais[j], ln, &ln->dois[i]);
        }
    }

    /* Check DataSet references */
    for (int i = 0; i < ln->dataset_count; i++) {
        SclDataSet* dataset = &ln->datasets[i];

        /* Check FCDA references */
        for (int j = 0; j < dataset->fcda_count; j++) {
            SclFCDA* fcda = &dataset->fcdas[j];

            /* Check if referenced LD exists */
            /* TODO: Implement full FCDA reference checking */
        }
    }

    /* Check ReportControl */
    for (int i = 0; i < ln->report_control_count; i++) {
        SclReportControl* rc = &ln->report_controls[i];

        if (rc->name[0] == '\0') {
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                    "ReportControl has no name attribute", obj_ref, "MissingName");
        }

        if (rc->dat_set[0] != '\0') {
            /* Check if DataSet exists */
            bool found = false;
            for (int j = 0; j < ln->dataset_count; j++) {
                if (strcmp(ln->datasets[j].name, rc->dat_set) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "ReportControl \"%s\" references undefined DataSet \"%s\"",
                         rc->name, rc->dat_set);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                        msg, obj_ref, "DataSetNotFound");
            }
        }
    }

    /* Check GSEControl */
    for (int i = 0; i < ln->gse_control_count; i++) {
        SclGSEControl* gc = &ln->gse_controls[i];

        if (gc->name[0] == '\0') {
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                    "GSEControl has no name attribute", obj_ref, "MissingName");
        }

        if (gc->dat_set[0] != '\0') {
            bool found = false;
            for (int j = 0; j < ln->dataset_count; j++) {
                if (strcmp(ln->datasets[j].name, gc->dat_set) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "GSEControl \"%s\" references undefined DataSet \"%s\"",
                         gc->name, gc->dat_set);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                        msg, obj_ref, "DataSetNotFound");
            }
        }
    }

    /* Check SMVControl */
    for (int i = 0; i < ln->smv_control_count; i++) {
        SclSMVControl* smvc = &ln->smv_controls[i];

        if (smvc->name[0] == '\0') {
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                    "SMVControl has no name attribute", obj_ref, "MissingName");
        }
    }

    /* Check SettingGroup must be in LLN0 */
    if (ln->has_setting_control && strcmp(ln->ln_class, "LLN0") != 0) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_STRUCTURE, -1,
                                "SettingGroup control is not defined in LLN0", obj_ref, "SettingGroupNotInLLN0");
    }
}

static void check_ldevice(SclValidator* validator, SclLDevice* ldevice, const char* ied_name)
{
    char obj_ref[SCL_MAX_OBJREF_LEN];
    snprintf(obj_ref, SCL_MAX_OBJREF_LEN, "%s%s", ied_name, ldevice->inst);

    /* Check inst attribute */
    if (ldevice->inst[0] == '\0') {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "LDevice has no inst attribute", obj_ref, "MissingInst");
    }

    /* Check LN0 exists */
    if (ldevice->ln0.ln_class[0] == '\0') {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_STRUCTURE, -1,
                                "LDevice has no LN0", obj_ref, "MissingLN0");
    } else {
        check_ln(validator, &ldevice->ln0, ied_name, ldevice->inst);
    }

    /* Check all LN elements */
    for (int i = 0; i < ldevice->logical_node_count; i++) {
        check_ln(validator, &ldevice->logical_nodes[i], ied_name, ldevice->inst);
    }
}

static void check_access_point(SclValidator* validator, SclAccessPoint* ap, const char* ied_name)
{
    char obj_ref[SCL_MAX_OBJREF_LEN];
    snprintf(obj_ref, SCL_MAX_OBJREF_LEN, "%s/%s", ied_name, ap->name);

    /* Check name attribute */
    if (ap->name[0] == '\0') {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "AccessPoint has no name attribute", ied_name, "MissingName");
    }

    /* Check Server or ServerAt exists */
    if (!ap->has_server && ap->server_at[0] == '\0') {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_STRUCTURE, -1,
                                "AccessPoint has no Server or ServerAt", obj_ref, "MissingServer");
    }

    /* Check Server structure */
    if (ap->has_server && ap->server != NULL) {
        for (int i = 0; i < ap->server->ldevice_count; i++) {
            check_ldevice(validator, &ap->server->ldevices[i], ied_name);
        }
    }
}

void SclChecker_CheckIED(SclValidator* validator, SclIED* ied)
{
    if (validator == NULL || ied == NULL) {
        return;
    }

    /* Check name attribute */
    if (ied->name[0] == '\0') {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "IED has no name attribute", NULL, "MissingName");
    }

    /* Check AccessPoint count */
    if (ied->access_point_count == 0) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_STRUCTURE, -1,
                                "IED has no AccessPoint", ied->name, "MissingAccessPoint");
    }

    /* Check all AccessPoints */
    for (int i = 0; i < ied->access_point_count; i++) {
        check_access_point(validator, &ied->access_points[i], ied->name);
    }
}

/* ==================== DAI Value Checking ==================== */

static SclDataAttributeDefinition* find_da_definition(SclValidator* validator, SclLN* ln, const char* doi_name, const char* dai_name)
{
    if (validator == NULL || ln == NULL || validator->dataTypeTemplates == NULL) {
        return NULL;
    }

    /* Find LNodeType */
    SclLNodeType* lnode_type = SclChecker_FindLNodeType(validator, ln->ln_type);
    if (lnode_type == NULL) {
        return NULL;
    }

    /* Find DO definition in LNodeType */
    SclDataObjectDefinition* do_def = NULL;
    for (int i = 0; i < lnode_type->data_object_count; i++) {
        if (strcmp(lnode_type->data_objects[i].name, doi_name) == 0) {
            do_def = &lnode_type->data_objects[i];
            break;
        }
    }

    if (do_def == NULL) {
        return NULL;
    }

    /* Find DOType */
    SclDOType* do_type = SclChecker_FindDOType(validator, do_def->type);
    if (do_type == NULL) {
        return NULL;
    }

    /* Find DA definition in DOType */
    for (int i = 0; i < do_type->data_attribute_count; i++) {
        if (strcmp(do_type->data_attributes[i].name, dai_name) == 0) {
            return &do_type->data_attributes[i];
        }
    }

    return NULL;
}

void SclChecker_CheckDAIValue(SclValidator* validator, SclDAI* dai, SclLN* ln, SclDOI* doi)
{
    if (validator == NULL || dai == NULL || ln == NULL || doi == NULL) {
        return;
    }

    char obj_ref[SCL_MAX_OBJREF_LEN];
    snprintf(obj_ref, SCL_MAX_OBJREF_LEN, "%s%s%s/%s.%s",
             ln->prefix, ln->ln_class, ln->inst, doi->name, dai->name);

    /* Find DA definition */
    SclDataAttributeDefinition* da_def = find_da_definition(validator, ln, doi->name, dai->name);

    if (da_def == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                "DA definition not found for DAI", obj_ref, "DADefNotFound");
        return;
    }

    /* Check each value */
    for (int i = 0; i < dai->value_count; i++) {
        const char* value = dai->values[i].value;

        if (value == NULL || strlen(value) == 0) {
            continue;  /* Empty value is OK */
        }

        /* Check based on bType */
        switch (da_def->btype) {
            case SCL_BTYPE_BOOLEAN:
                if (strcmp(value, "true") != 0 && strcmp(value, "false") != 0 &&
                    strcmp(value, "1") != 0 && strcmp(value, "0") != 0) {
                    char msg[SCL_MAX_DESC_LEN];
                    snprintf(msg, SCL_MAX_DESC_LEN,
                             "Invalid boolean value \"%s\" in DAI", value);
                    SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                            msg, obj_ref, "InvalidBoolean");
                }
                break;

            case SCL_BTYPE_INT8:
            case SCL_BTYPE_INT16:
            case SCL_BTYPE_INT32:
            case SCL_BTYPE_INT64:
            case SCL_BTYPE_INT8U:
            case SCL_BTYPE_INT16U:
            case SCL_BTYPE_INT24U:
            case SCL_BTYPE_INT32U:
                if (!SclChecker_CheckIntegerValue(value, da_def->btype)) {
                    char msg[SCL_MAX_DESC_LEN];
                    snprintf(msg, SCL_MAX_DESC_LEN,
                             "Invalid integer value \"%s\" for type %d in DAI",
                             value, da_def->btype);
                    SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                            msg, obj_ref, "InvalidInteger");
                }
                break;

            case SCL_BTYPE_FLOAT32:
            case SCL_BTYPE_FLOAT64:
                /* Check if it's a valid float */
                char* endptr;
                strtod(value, &endptr);
                if (*endptr != '\0') {
                    char msg[SCL_MAX_DESC_LEN];
                    snprintf(msg, SCL_MAX_DESC_LEN,
                             "Invalid float value \"%s\" in DAI", value);
                    SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                            msg, obj_ref, "InvalidFloat");
                }
                break;

            case SCL_BTYPE_ENUMERATED:
                if (!SclChecker_CheckEnumValue(validator, value, da_def->type)) {
                    char msg[SCL_MAX_DESC_LEN];
                    snprintf(msg, SCL_MAX_DESC_LEN,
                             "Value \"%s\" does not exist in enumerated type \"%s\"",
                             value, da_def->type);
                    SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                            msg, obj_ref, "InvalidEnumValue");
                }
                break;

            case SCL_BTYPE_OCTET_STRING_64:
                if (!SclChecker_IsBase64String(value)) {
                    char msg[SCL_MAX_DESC_LEN];
                    snprintf(msg, SCL_MAX_DESC_LEN,
                             "Invalid Base64 value \"%s\" in DAI", value);
                    SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                            msg, obj_ref, "InvalidBase64");
                }
                break;

            default:
                /* Other types - no specific validation */
                break;
        }
    }
}

/* ==================== Communication Checking ==================== */

void SclChecker_CheckPValue(SclValidator* validator, SclP* p)
{
    if (validator == NULL || p == NULL || p->text[0] == '\0') {
        return;
    }

    switch (p->p_type) {
        case SCL_P_TYPE_IP:
        case SCL_P_TYPE_SUBNET:
        case SCL_P_TYPE_GATEWAY:
            /* IP address: 4 hex bytes (8 characters) */
            if (strlen(p->text) != 8 || !SclChecker_IsHexString(p->text)) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "P address of type IP/SUBNET/GATEWAY size is not 4. Actual size: %zu. Value: %s",
                         strlen(p->text), p->text);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                        msg, NULL, "InvalidIP");
            }
            break;

        case SCL_P_TYPE_MAC_ADDRESS:
            /* MAC address: 6 hex bytes (12 characters) */
            if (strlen(p->text) != 12 || !SclChecker_IsHexString(p->text)) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "P address of type MAC-Address size is not 6. Actual size: %zu. Value: %s",
                         strlen(p->text), p->text);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                        msg, NULL, "InvalidMAC");
            }
            break;

        case SCL_P_TYPE_VLAN_ID:
            /* VLAN-ID: 3 hex characters */
            if (strlen(p->text) != 3 || !SclChecker_IsHexString(p->text)) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "P address of type VLAN-ID size is not 3. Actual size: %zu. Value: %s",
                         strlen(p->text), p->text);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                        msg, NULL, "InvalidVlanId");
            }
            break;

        case SCL_P_TYPE_VLAN_PRIORITY:
            /* VLAN-PRIORITY: 0-7 */
            int prio = atoi(p->text);
            if (prio < 0 || prio > 7) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "P address of type VLAN-PRIORITY does not have a 0-7 value. Value: %s",
                         p->text);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                        msg, NULL, "InvalidVlanPrio");
            }
            break;

        case SCL_P_TYPE_APPID:
            /* APPID: 2 hex bytes (4 characters) */
            if (strlen(p->text) != 4 || !SclChecker_IsHexString(p->text)) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "P address of type APPID size is not 2. Actual size: %zu. Value: %s",
                         strlen(p->text), p->text);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                        msg, NULL, "InvalidAppId");
            }
            break;

        case SCL_P_TYPE_OSITSEL:
        case SCL_P_TYPE_OSIPSSEL:
        case SCL_P_TYPE_OSITSSEL:
            /* OSI selectors: hex values */
            if (!SclChecker_IsHexString(p->text)) {
                char msg[SCL_MAX_DESC_LEN];
                snprintf(msg, SCL_MAX_DESC_LEN,
                         "P address of type OSI selector does not have a Hex value. Value: %s",
                         p->text);
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                        msg, NULL, "InvalidOSI");
            }
            break;

        default:
            /* Unknown or SCL-URI type - no specific validation */
            break;
    }
}

void SclChecker_CheckCommunication(SclValidator* validator)
{
    if (validator == NULL || validator->subnetwork_count == 0) {
        return;
    }

    for (int i = 0; i < validator->subnetwork_count; i++) {
        SclSubNetwork* sn = validator->subnetworks[i];

        /* Check SubNetwork name */
        if (sn->name[0] == '\0') {
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                    "SubNetwork has no name attribute", NULL, "MissingName");
        }

        /* Check ConnectedAPs */
        for (int j = 0; j < sn->connected_ap_count; j++) {
            SclConnectedAP* cap = &sn->connected_aps[j];

            /* Check IED reference */
            if (cap->ied_name[0] != '\0') {
                SclIED* ied = SclChecker_FindIED(validator, cap->ied_name);
                if (ied == NULL) {
                    char msg[SCL_MAX_DESC_LEN];
                    snprintf(msg, SCL_MAX_DESC_LEN,
                             "ConnectedAP references undefined IED \"%s\"",
                             cap->ied_name);
                    SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                            msg, cap->ied_name, "IEDNotFound");
                }
            }

            /* Check P values */
            for (int k = 0; k < cap->p_count; k++) {
                SclChecker_CheckPValue(validator, &cap->ps[k]);
            }

            /* Check GSE P values */
            for (int k = 0; k < cap->gse_count; k++) {
                for (int l = 0; l < cap->gses[k].p_count; l++) {
                    SclChecker_CheckPValue(validator, &cap->gses[k].ps[l]);
                }
            }

            /* Check SMV P values */
            for (int k = 0; k < cap->smv_count; k++) {
                for (int l = 0; l < cap->smvs[k].p_count; l++) {
                    SclChecker_CheckPValue(validator, &cap->smvs[k].ps[l]);
                }
            }
        }
    }
}

/* ==================== Reference Checking ==================== */

void SclChecker_CheckReferences(SclValidator* validator)
{
    if (validator == NULL) {
        return;
    }

    /* Check that all IEDs referenced in Communication exist */
    for (int i = 0; i < validator->subnetwork_count; i++) {
        SclSubNetwork* sn = validator->subnetworks[i];

        for (int j = 0; j < sn->connected_ap_count; j++) {
            SclConnectedAP* cap = &sn->connected_aps[j];

            /* Check if IED exists */
            if (cap->ied_name[0] != '\0') {
                SclIED* ied = SclChecker_FindIED(validator, cap->ied_name);
                if (ied == NULL) {
                    /* Already reported in CheckCommunication */
                } else {
                    /* Check if AccessPoint exists in IED */
                    bool ap_found = false;
                    for (int k = 0; k < ied->access_point_count; k++) {
                        if (strcmp(ied->access_points[k].name, cap->ap_name) == 0) {
                            ap_found = true;
                            break;
                        }
                    }

                    if (!ap_found) {
                        char msg[SCL_MAX_DESC_LEN];
                        snprintf(msg, SCL_MAX_DESC_LEN,
                                 "ConnectedAP references undefined AccessPoint \"%s\" in IED \"%s\"",
                                 cap->ap_name, cap->ied_name);
                        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_REFERENCE, -1,
                                                msg, cap->ied_name, "APNotFound");
                    }
                }
            }
        }
    }

    /* Check that all lnType references are valid */
    for (int i = 0; i < validator->ied_count; i++) {
        SclIED* ied = validator->ieds[i];

        for (int j = 0; j < ied->access_point_count; j++) {
            SclAccessPoint* ap = &ied->access_points[j];

            if (ap->has_server && ap->server != NULL) {
                for (int k = 0; k < ap->server->ldevice_count; k++) {
                    SclLDevice* ld = &ap->server->ldevices[k];

                    /* Check LN0 */
                    if (ld->ln0.ln_type[0] != '\0') {
                        SclLNodeType* lnt = SclChecker_FindLNodeType(validator, ld->ln0.ln_type);
                        if (lnt == NULL) {
                            /* Already reported in check_ln */
                        }
                    }

                    /* Check other LN elements */
                    for (int l = 0; l < ld->logical_node_count; l++) {
                        SclLN* ln = &ld->logical_nodes[l];

                        if (ln->ln_type[0] != '\0') {
                            SclLNodeType* lnt = SclChecker_FindLNodeType(validator, ln->ln_type);
                            if (lnt == NULL) {
                                /* Already reported in check_ln */
                            }
                        }
                    }
                }
            }
        }
    }
}