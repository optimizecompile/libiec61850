/*
 * scl_parser.c - SCL XML Parser Implementation
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#include "scl_parser.h"
#include "scl_validator.h"
#include "scl_elements.h"
#include <mxml.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ==================== Helper Functions ==================== */

const char* SclParser_GetAttr(void* node, const char* name)
{
    if (node == NULL || name == NULL) {
        return NULL;
    }
    return mxmlElementGetAttr((mxml_node_t*)node, name);
}

int SclParser_GetAttrInt(void* node, const char* name, int default_value)
{
    const char* attr = SclParser_GetAttr(node, name);
    if (attr == NULL) {
        return default_value;
    }
    return atoi(attr);
}

uint32_t SclParser_GetAttrUInt(void* node, const char* name, uint32_t default_value)
{
    const char* attr = SclParser_GetAttr(node, name);
    if (attr == NULL) {
        return default_value;
    }
    return (uint32_t)strtoul(attr, NULL, 10);
}

bool SclParser_GetAttrBool(void* node, const char* name, bool default_value)
{
    const char* attr = SclParser_GetAttr(node, name);
    if (attr == NULL) {
        return default_value;
    }
    /* Check for "true" or "1" */
    if (strcmp(attr, "true") == 0 || strcmp(attr, "1") == 0) {
        return true;
    }
    return false;
}

SclFCType SclParser_ParseFC(const char* fc_str)
{
    if (fc_str == NULL) {
        return SCL_FC_UNKNOWN;
    }

    if (strcmp(fc_str, "ST") == 0) return SCL_FC_ST;
    if (strcmp(fc_str, "MX") == 0) return SCL_FC_MX;
    if (strcmp(fc_str, "CF") == 0) return SCL_FC_CF;
    if (strcmp(fc_str, "DC") == 0) return SCL_FC_DC;
    if (strcmp(fc_str, "SP") == 0) return SCL_FC_SP;
    if (strcmp(fc_str, "SG") == 0) return SCL_FC_SG;
    if (strcmp(fc_str, "SE") == 0) return SCL_FC_SE;
    if (strcmp(fc_str, "SR") == 0) return SCL_FC_SR;
    if (strcmp(fc_str, "OR") == 0) return SCL_FC_OR;
    if (strcmp(fc_str, "BL") == 0) return SCL_FC_BL;
    if (strcmp(fc_str, "EX") == 0) return SCL_FC_EX;
    if (strcmp(fc_str, "CO") == 0) return SCL_FC_CO;
    if (strcmp(fc_str, "US") == 0) return SCL_FC_US;
    if (strcmp(fc_str, "MS") == 0) return SCL_FC_MS;
    if (strcmp(fc_str, "EN") == 0) return SCL_FC_EN;

    return SCL_FC_UNKNOWN;
}

SclBTypeType SclParser_ParseBType(const char* btype_str)
{
    if (btype_str == NULL) {
        return SCL_BTYPE_UNKNOWN;
    }

    if (strcmp(btype_str, "BOOLEAN") == 0) return SCL_BTYPE_BOOLEAN;
    if (strcmp(btype_str, "INT8") == 0) return SCL_BTYPE_INT8;
    if (strcmp(btype_str, "INT16") == 0) return SCL_BTYPE_INT16;
    if (strcmp(btype_str, "INT24") == 0) return SCL_BTYPE_INT24;
    if (strcmp(btype_str, "INT32") == 0) return SCL_BTYPE_INT32;
    if (strcmp(btype_str, "INT64") == 0) return SCL_BTYPE_INT64;
    if (strcmp(btype_str, "INT8U") == 0) return SCL_BTYPE_INT8U;
    if (strcmp(btype_str, "INT16U") == 0) return SCL_BTYPE_INT16U;
    if (strcmp(btype_str, "INT24U") == 0) return SCL_BTYPE_INT24U;
    if (strcmp(btype_str, "INT32U") == 0) return SCL_BTYPE_INT32U;
    if (strcmp(btype_str, "FLOAT32") == 0) return SCL_BTYPE_FLOAT32;
    if (strcmp(btype_str, "FLOAT64") == 0) return SCL_BTYPE_FLOAT64;
    if (strcmp(btype_str, "Enum") == 0 || strcmp(btype_str, "ENUMERATED") == 0) return SCL_BTYPE_ENUMERATED;
    if (strcmp(btype_str, "VisString32") == 0) return SCL_BTYPE_VISIBLE_STRING_32;
    if (strcmp(btype_str, "VisString64") == 0) return SCL_BTYPE_VISIBLE_STRING_64;
    if (strcmp(btype_str, "VisString65") == 0) return SCL_BTYPE_VISIBLE_STRING_65;
    if (strcmp(btype_str, "VisString129") == 0) return SCL_BTYPE_VISIBLE_STRING_129;
    if (strcmp(btype_str, "VisString255") == 0) return SCL_BTYPE_VISIBLE_STRING_255;
    if (strcmp(btype_str, "Unicode255") == 0) return SCL_BTYPE_UNICODE_STRING_255;
    if (strcmp(btype_str, "Octet64") == 0) return SCL_BTYPE_OCTET_STRING_64;
    if (strcmp(btype_str, "Timestamp") == 0) return SCL_BTYPE_TIMESTAMP;
    if (strcmp(btype_str, "Quality") == 0) return SCL_BTYPE_QUALITY;
    if (strcmp(btype_str, "Check") == 0) return SCL_BTYPE_CHECK;
    if (strcmp(btype_str, "ObjRef") == 0) return SCL_BTYPE_OBJECT_REFERENCE;
    if (strcmp(btype_str, "EntryTime") == 0) return SCL_BTYPE_ENTRY_TIME;
    if (strcmp(btype_str, "PhyComAddr") == 0) return SCL_BTYPE_PHYCOMADDR;
    if (strcmp(btype_str, "Currency") == 0) return SCL_BTYPE_CURRENCY;
    if (strcmp(btype_str, "Struct") == 0) return SCL_BTYPE_STRUCT;

    return SCL_BTYPE_UNKNOWN;
}

SclPTypeType SclParser_ParsePType(const char* ptype_str)
{
    if (ptype_str == NULL) {
        return SCL_P_TYPE_UNKNOWN;
    }

    if (strcmp(ptype_str, "IP") == 0) return SCL_P_TYPE_IP;
    if (strcmp(ptype_str, "SUBNET") == 0) return SCL_P_TYPE_SUBNET;
    if (strcmp(ptype_str, "GATEWAY") == 0) return SCL_P_TYPE_GATEWAY;
    if (strcmp(ptype_str, "VLAN-ID") == 0) return SCL_P_TYPE_VLAN_ID;
    if (strcmp(ptype_str, "VLAN-PRIORITY") == 0) return SCL_P_TYPE_VLAN_PRIORITY;
    if (strcmp(ptype_str, "APPID") == 0) return SCL_P_TYPE_APPID;
    if (strcmp(ptype_str, "MAC-Address") == 0) return SCL_P_TYPE_MAC_ADDRESS;
    if (strcmp(ptype_str, "OSI-TSEL") == 0) return SCL_P_TYPE_OSITSEL;
    if (strcmp(ptype_str, "OSI-PSSEL") == 0) return SCL_P_TYPE_OSIPSSEL;
    if (strcmp(ptype_str, "OSI-SSSEL") == 0) return SCL_P_TYPE_OSITSSEL;
    if (strcmp(ptype_str, "SCL-URI") == 0) return SCL_P_TYPE_SCLURI;

    return SCL_P_TYPE_UNKNOWN;
}

int SclParser_GetLineNumber(void* node)
{
    /* mxml doesn't provide line numbers directly - return -1 */
    (void)node;
    return -1;
}

/* ==================== Header Parsing ==================== */

void SclParser_ParseHeader(SclValidator* validator, void* parent)
{
    mxml_node_t* header_node = mxmlFindElement((mxml_node_t*)parent,
                                                (mxml_node_t*)validator->xml_doc,
                                                "Header", NULL, NULL, MXML_DESCEND_FIRST);

    if (header_node == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "Header element is missing", NULL, "MissingHeader");
        return;
    }

    validator->header = (SclHeader*)calloc(1, sizeof(SclHeader));
    if (validator->header == NULL) {
        return;
    }

    const char* id = SclParser_GetAttr(header_node, "id");
    if (id != NULL) {
        strncpy(validator->header->id, id, SCL_MAX_ID_LEN - 1);
    }

    const char* version = SclParser_GetAttr(header_node, "version");
    if (version != NULL) {
        strncpy(validator->header->version, version, SCL_MAX_NAME_LEN - 1);
    }

    const char* revision = SclParser_GetAttr(header_node, "revision");
    if (revision != NULL) {
        strncpy(validator->header->revision, revision, SCL_MAX_NAME_LEN - 1);
    }

    const char* tool_id = SclParser_GetAttr(header_node, "toolID");
    if (tool_id != NULL) {
        strncpy(validator->header->tool_id, tool_id, SCL_MAX_NAME_LEN - 1);
    }

    const char* name_structure = SclParser_GetAttr(header_node, "nameStructure");
    if (name_structure != NULL) {
        strncpy(validator->header->name_structure, name_structure, SCL_MAX_NAME_LEN - 1);
    }
}

/* ==================== DataTypeTemplates Parsing ==================== */

static void parse_enum_type(SclValidator* validator, mxml_node_t* enum_node, SclEnumType* enum_type)
{
    const char* id = SclParser_GetAttr(enum_node, "id");
    if (id != NULL) {
        strncpy(enum_type->id, id, SCL_MAX_ID_LEN - 1);
    }

    enum_type->value_count = 0;

    /* Parse EnumVal elements */
    mxml_node_t* enumval_node;
    for (enumval_node = mxmlFindElement(enum_node, enum_node, "EnumVal", NULL, NULL, MXML_DESCEND_FIRST);
         enumval_node != NULL && enum_type->value_count < SCL_MAX_ENUM_VALUES;
         enumval_node = mxmlFindElement(enumval_node, enum_node, "EnumVal", NULL, NULL, MXML_NO_DESCEND)) {

        SclEnumValue* val = &enum_type->values[enum_type->value_count];
        val->ord = SclParser_GetAttrInt(enumval_node, "ord", 0);

        const char* name = SclParser_GetAttr(enumval_node, "valName");
        if (name != NULL) {
            strncpy(val->symbolic_name, name, SCL_MAX_NAME_LEN - 1);
        }

        enum_type->value_count++;
    }
}

static void parse_da_type(SclValidator* validator, mxml_node_t* da_node, SclDAType* da_type)
{
    const char* id = SclParser_GetAttr(da_node, "id");
    if (id != NULL) {
        strncpy(da_type->id, id, SCL_MAX_ID_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(da_node, "desc");
    if (desc != NULL) {
        strncpy(da_type->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    da_type->attribute_count = 0;

    /* Parse BDA elements */
    mxml_node_t* bda_node;
    for (bda_node = mxmlFindElement(da_node, da_node, "BDA", NULL, NULL, MXML_DESCEND_FIRST);
         bda_node != NULL && da_type->attribute_count < SCL_MAX_DATA_ATTRIBUTES;
         bda_node = mxmlFindElement(bda_node, da_node, "BDA", NULL, NULL, MXML_NO_DESCEND)) {

        SclDataAttributeDefinition* attr = &da_type->attributes[da_type->attribute_count];

        const char* name = SclParser_GetAttr(bda_node, "name");
        if (name != NULL) {
            strncpy(attr->name, name, SCL_MAX_NAME_LEN - 1);
        }

        const char* btype = SclParser_GetAttr(bda_node, "bType");
        attr->btype = SclParser_ParseBType(btype);

        const char* type = SclParser_GetAttr(bda_node, "type");
        if (type != NULL) {
            strncpy(attr->type, type, SCL_MAX_ID_LEN - 1);
        }

        const char* fc = SclParser_GetAttr(bda_node, "fc");
        attr->fc = SclParser_ParseFC(fc);

        attr->saddr = SclParser_GetAttrInt(bda_node, "sAddr", 0);
        attr->count = SclParser_GetAttrInt(bda_node, "count", 0);

        /* Parse Val element */
        mxml_node_t* val_node = mxmlFindElement(bda_node, bda_node, "Val", NULL, NULL, MXML_DESCEND_FIRST);
        if (val_node != NULL) {
            const char* val_text = mxmlGetText(val_node, NULL);
            if (val_text != NULL) {
                strncpy(attr->val, val_text, SCL_MAX_VALUE_LEN - 1);
            }
        }

        da_type->attribute_count++;
    }
}

static void parse_do_type(SclValidator* validator, mxml_node_t* do_node, SclDOType* do_type)
{
    const char* id = SclParser_GetAttr(do_node, "id");
    if (id != NULL) {
        strncpy(do_type->id, id, SCL_MAX_ID_LEN - 1);
    }

    const char* cdc = SclParser_GetAttr(do_node, "cdc");
    if (cdc != NULL) {
        strncpy(do_type->cdc, cdc, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(do_node, "desc");
    if (desc != NULL) {
        strncpy(do_type->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    do_type->data_object_count = 0;
    do_type->data_attribute_count = 0;

    /* Parse SDO elements */
    mxml_node_t* sdo_node;
    for (sdo_node = mxmlFindElement(do_node, do_node, "SDO", NULL, NULL, MXML_DESCEND_FIRST);
         sdo_node != NULL && do_type->data_object_count < SCL_MAX_DATA_OBJECTS;
         sdo_node = mxmlFindElement(sdo_node, do_node, "SDO", NULL, NULL, MXML_NO_DESCEND)) {

        SclDataObjectDefinition* do_def = &do_type->data_objects[do_type->data_object_count];

        const char* name = SclParser_GetAttr(sdo_node, "name");
        if (name != NULL) {
            strncpy(do_def->name, name, SCL_MAX_NAME_LEN - 1);
        }

        const char* type = SclParser_GetAttr(sdo_node, "type");
        if (type != NULL) {
            strncpy(do_def->type, type, SCL_MAX_ID_LEN - 1);
        }

        do_type->data_object_count++;
    }

    /* Parse DA elements */
    mxml_node_t* da_node;
    for (da_node = mxmlFindElement(do_node, do_node, "DA", NULL, NULL, MXML_DESCEND_FIRST);
         da_node != NULL && do_type->data_attribute_count < SCL_MAX_DATA_ATTRIBUTES;
         da_node = mxmlFindElement(da_node, do_node, "DA", NULL, NULL, MXML_NO_DESCEND)) {

        SclDataAttributeDefinition* attr = &do_type->data_attributes[do_type->data_attribute_count];

        const char* name = SclParser_GetAttr(da_node, "name");
        if (name != NULL) {
            strncpy(attr->name, name, SCL_MAX_NAME_LEN - 1);
        }

        const char* btype = SclParser_GetAttr(da_node, "bType");
        attr->btype = SclParser_ParseBType(btype);

        const char* type = SclParser_GetAttr(da_node, "type");
        if (type != NULL) {
            strncpy(attr->type, type, SCL_MAX_ID_LEN - 1);
        }

        const char* fc = SclParser_GetAttr(da_node, "fc");
        attr->fc = SclParser_ParseFC(fc);

        attr->dchg = SclParser_GetAttrInt(da_node, "dchg", 0);
        attr->qchg = SclParser_GetAttrInt(da_node, "qchg", 0);
        attr->dupd = SclParser_GetAttrInt(da_node, "dupd", 0);
        attr->saddr = SclParser_GetAttrInt(da_node, "sAddr", 0);
        attr->count = SclParser_GetAttrInt(da_node, "count", 0);

        /* Parse Val element */
        mxml_node_t* val_node = mxmlFindElement(da_node, da_node, "Val", NULL, NULL, MXML_DESCEND_FIRST);
        if (val_node != NULL) {
            const char* val_text = mxmlGetText(val_node, NULL);
            if (val_text != NULL) {
                strncpy(attr->val, val_text, SCL_MAX_VALUE_LEN - 1);
            }
        }

        do_type->data_attribute_count++;
    }
}

static void parse_lnode_type(SclValidator* validator, mxml_node_t* ln_node, SclLNodeType* lnode_type)
{
    const char* id = SclParser_GetAttr(ln_node, "id");
    if (id != NULL) {
        strncpy(lnode_type->id, id, SCL_MAX_ID_LEN - 1);
    }

    const char* ln_class = SclParser_GetAttr(ln_node, "lnClass");
    if (ln_class != NULL) {
        strncpy(lnode_type->ln_class, ln_class, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(ln_node, "desc");
    if (desc != NULL) {
        strncpy(lnode_type->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    lnode_type->data_object_count = 0;

    /* Parse DO elements */
    mxml_node_t* do_node;
    for (do_node = mxmlFindElement(ln_node, ln_node, "DO", NULL, NULL, MXML_DESCEND_FIRST);
         do_node != NULL && lnode_type->data_object_count < SCL_MAX_DATA_OBJECTS;
         do_node = mxmlFindElement(do_node, ln_node, "DO", NULL, NULL, MXML_NO_DESCEND)) {

        SclDataObjectDefinition* do_def = &lnode_type->data_objects[lnode_type->data_object_count];

        const char* name = SclParser_GetAttr(do_node, "name");
        if (name != NULL) {
            strncpy(do_def->name, name, SCL_MAX_NAME_LEN - 1);
        }

        const char* type = SclParser_GetAttr(do_node, "type");
        if (type != NULL) {
            strncpy(do_def->type, type, SCL_MAX_ID_LEN - 1);
        }

        lnode_type->data_object_count++;
    }
}

void SclParser_ParseDataTypeTemplates(SclValidator* validator, void* parent)
{
    mxml_node_t* dtt_node = mxmlFindElement((mxml_node_t*)parent,
                                            (mxml_node_t*)validator->xml_doc,
                                            "DataTypeTemplates", NULL, NULL, MXML_DESCEND_FIRST);

    if (dtt_node == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_WARNING, SCL_ISSUE_STRUCTURE, -1,
                                "DataTypeTemplates section is missing", NULL, "MissingDataTypeTemplates");
        return;
    }

    validator->dataTypeTemplates = (SclDataTypeTemplates*)calloc(1, sizeof(SclDataTypeTemplates));
    if (validator->dataTypeTemplates == NULL) {
        return;
    }

    /* Parse EnumType elements */
    mxml_node_t* enum_node;
    for (enum_node = mxmlFindElement(dtt_node, dtt_node, "EnumType", NULL, NULL, MXML_DESCEND_FIRST);
         enum_node != NULL && validator->dataTypeTemplates->enum_type_count < SCL_MAX_ENUM_TYPES;
         enum_node = mxmlFindElement(enum_node, dtt_node, "EnumType", NULL, NULL, MXML_NO_DESCEND)) {

        parse_enum_type(validator, enum_node,
                        &validator->dataTypeTemplates->enum_types[validator->dataTypeTemplates->enum_type_count++]);
    }

    /* Parse DAType elements */
    mxml_node_t* da_node;
    for (da_node = mxmlFindElement(dtt_node, dtt_node, "DAType", NULL, NULL, MXML_DESCEND_FIRST);
         da_node != NULL && validator->dataTypeTemplates->da_type_count < SCL_MAX_DA_TYPES;
         da_node = mxmlFindElement(da_node, dtt_node, "DAType", NULL, NULL, MXML_NO_DESCEND)) {

        parse_da_type(validator, da_node,
                      &validator->dataTypeTemplates->da_types[validator->dataTypeTemplates->da_type_count++]);
    }

    /* Parse DOType elements */
    mxml_node_t* do_node;
    for (do_node = mxmlFindElement(dtt_node, dtt_node, "DOType", NULL, NULL, MXML_DESCEND_FIRST);
         do_node != NULL && validator->dataTypeTemplates->do_type_count < SCL_MAX_DO_TYPES;
         do_node = mxmlFindElement(do_node, dtt_node, "DOType", NULL, NULL, MXML_NO_DESCEND)) {

        parse_do_type(validator, do_node,
                      &validator->dataTypeTemplates->do_types[validator->dataTypeTemplates->do_type_count++]);
    }

    /* Parse LNodeType elements */
    mxml_node_t* ln_node;
    for (ln_node = mxmlFindElement(dtt_node, dtt_node, "LNodeType", NULL, NULL, MXML_DESCEND_FIRST);
         ln_node != NULL && validator->dataTypeTemplates->lnode_type_count < SCL_MAX_LNODE_TYPES;
         ln_node = mxmlFindElement(ln_node, dtt_node, "LNodeType", NULL, NULL, MXML_NO_DESCEND)) {

        parse_lnode_type(validator, ln_node,
                         &validator->dataTypeTemplates->lnode_types[validator->dataTypeTemplates->lnode_type_count++]);
    }
}

/* ==================== IED Parsing ==================== */

static void parse_doi(SclValidator* validator, mxml_node_t* doi_node, SclDOI* doi);
static void parse_dai(SclValidator* validator, mxml_node_t* dai_node, SclDAI* dai);
static void parse_sdi(SclValidator* validator, mxml_node_t* sdi_node, SclSDI* sdi);
static void parse_dataset(SclValidator* validator, mxml_node_t* ds_node, SclDataSet* dataset);
static void parse_fcda(SclValidator* validator, mxml_node_t* fcda_node, SclFCDA* fcda);
static void parse_report_control(SclValidator* validator, mxml_node_t* rc_node, SclReportControl* rc);
static void parse_gse_control(SclValidator* validator, mxml_node_t* gc_node, SclGSEControl* gc);
static void parse_smv_control(SclValidator* validator, mxml_node_t* smvc_node, SclSMVControl* smvc);
static void parse_log_control(SclValidator* validator, mxml_node_t* lc_node, SclLogControl* lc);
static void parse_log(SclValidator* validator, mxml_node_t* log_node, SclLog* log);
static void parse_setting_control(SclValidator* validator, mxml_node_t* sg_node, SclSettingControl* sg);
static void parse_ln(SclValidator* validator, mxml_node_t* ln_node, SclLN* ln);
static void parse_ldevice(SclValidator* validator, mxml_node_t* ld_node, SclLDevice* ldevice);
static void parse_server(SclValidator* validator, mxml_node_t* server_node, SclServer* server);
static void parse_access_point(SclValidator* validator, mxml_node_t* ap_node, SclAccessPoint* ap);

static void parse_dai(SclValidator* validator, mxml_node_t* dai_node, SclDAI* dai)
{
    const char* name = SclParser_GetAttr(dai_node, "name");
    if (name != NULL) {
        strncpy(dai->name, name, SCL_MAX_NAME_LEN - 1);
    }

    dai->ix = SclParser_GetAttrInt(dai_node, "ix", -1);

    const char* val_kind = SclParser_GetAttr(dai_node, "valKind");
    if (val_kind != NULL) {
        strncpy(dai->val_kind, val_kind, SCL_MAX_NAME_LEN - 1);
    }

    dai->value_count = 0;

    /* Parse Val elements */
    mxml_node_t* val_node;
    for (val_node = mxmlFindElement(dai_node, dai_node, "Val", NULL, NULL, MXML_DESCEND_FIRST);
         val_node != NULL && dai->value_count < 16;
         val_node = mxmlFindElement(val_node, dai_node, "Val", NULL, NULL, MXML_NO_DESCEND)) {

        const char* val_text = mxmlGetText(val_node, NULL);
        if (val_text != NULL) {
            strncpy(dai->values[dai->value_count].value, val_text, SCL_MAX_VALUE_LEN - 1);
        }

        const char* sgroup = SclParser_GetAttr(val_node, "sGroup");
        if (sgroup != NULL) {
            strncpy(dai->values[dai->value_count].sGroup, sgroup, SCL_MAX_NAME_LEN - 1);
        }

        dai->value_count++;
    }
}

static void parse_sdi(SclValidator* validator, mxml_node_t* sdi_node, SclSDI* sdi)
{
    const char* name = SclParser_GetAttr(sdi_node, "name");
    if (name != NULL) {
        strncpy(sdi->name, name, SCL_MAX_NAME_LEN - 1);
    }

    sdi->ix = SclParser_GetAttrInt(sdi_node, "ix", -1);
    sdi->dai_count = 0;
    sdi->sub_sdi_count = 0;

    /* Parse DAI elements */
    mxml_node_t* dai_node;
    for (dai_node = mxmlFindElement(sdi_node, sdi_node, "DAI", NULL, NULL, MXML_DESCEND_FIRST);
         dai_node != NULL && sdi->dai_count < SCL_MAX_DATA_ATTRIBUTES;
         dai_node = mxmlFindElement(dai_node, sdi_node, "DAI", NULL, NULL, MXML_NO_DESCEND)) {

        parse_dai(validator, dai_node, &sdi->dais[sdi->dai_count++]);
    }

    /* Parse nested SDI elements */
    mxml_node_t* nested_sdi_node;
    for (nested_sdi_node = mxmlFindElement(sdi_node, sdi_node, "SDI", NULL, NULL, MXML_DESCEND_FIRST);
         nested_sdi_node != NULL && sdi->sub_sdi_count < SCL_MAX_DATA_OBJECTS;
         nested_sdi_node = mxmlFindElement(nested_sdi_node, sdi_node, "SDI", NULL, NULL, MXML_NO_DESCEND)) {

        sdi->sub_sdis[sdi->sub_sdi_count] = (SclSDI*)calloc(1, sizeof(SclSDI));
        if (sdi->sub_sdis[sdi->sub_sdi_count] != NULL) {
            parse_sdi(validator, nested_sdi_node, sdi->sub_sdis[sdi->sub_sdi_count++]);
        }
    }
}

static void parse_doi(SclValidator* validator, mxml_node_t* doi_node, SclDOI* doi)
{
    const char* name = SclParser_GetAttr(doi_node, "name");
    if (name != NULL) {
        strncpy(doi->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(doi_node, "desc");
    if (desc != NULL) {
        strncpy(doi->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    doi->ix = SclParser_GetAttrInt(doi_node, "ix", -1);
    doi->dai_count = 0;
    doi->sdi_count = 0;

    /* Parse DAI elements */
    mxml_node_t* dai_node;
    for (dai_node = mxmlFindElement(doi_node, doi_node, "DAI", NULL, NULL, MXML_DESCEND_FIRST);
         dai_node != NULL && doi->dai_count < SCL_MAX_DATA_ATTRIBUTES;
         dai_node = mxmlFindElement(dai_node, doi_node, "DAI", NULL, NULL, MXML_NO_DESCEND)) {

        parse_dai(validator, dai_node, &doi->dais[doi->dai_count++]);
    }

    /* Parse SDI elements */
    mxml_node_t* sdi_node;
    for (sdi_node = mxmlFindElement(doi_node, doi_node, "SDI", NULL, NULL, MXML_DESCEND_FIRST);
         sdi_node != NULL && doi->sdi_count < SCL_MAX_DATA_OBJECTS;
         sdi_node = mxmlFindElement(sdi_node, doi_node, "SDI", NULL, NULL, MXML_NO_DESCEND)) {

        doi->sdis[doi->sdi_count] = (SclSDI*)calloc(1, sizeof(SclSDI));
        if (doi->sdis[doi->sdi_count] != NULL) {
            parse_sdi(validator, sdi_node, doi->sdis[doi->sdi_count++]);
        }
    }
}

static void parse_fcda(SclValidator* validator, mxml_node_t* fcda_node, SclFCDA* fcda)
{
    const char* ld_inst = SclParser_GetAttr(fcda_node, "ldInst");
    if (ld_inst != NULL) {
        strncpy(fcda->ld_inst, ld_inst, SCL_MAX_NAME_LEN - 1);
    }

    const char* prefix = SclParser_GetAttr(fcda_node, "prefix");
    if (prefix != NULL) {
        strncpy(fcda->prefix, prefix, SCL_MAX_NAME_LEN - 1);
    }

    const char* ln_class = SclParser_GetAttr(fcda_node, "lnClass");
    if (ln_class != NULL) {
        strncpy(fcda->ln_class, ln_class, SCL_MAX_NAME_LEN - 1);
    }

    const char* ln_inst = SclParser_GetAttr(fcda_node, "lnInst");
    if (ln_inst != NULL) {
        strncpy(fcda->ln_inst, ln_inst, SCL_MAX_NAME_LEN - 1);
    }

    const char* do_name = SclParser_GetAttr(fcda_node, "doName");
    if (do_name != NULL) {
        strncpy(fcda->do_name, do_name, SCL_MAX_NAME_LEN - 1);
    }

    const char* da_name = SclParser_GetAttr(fcda_node, "daName");
    if (da_name != NULL) {
        strncpy(fcda->da_name, da_name, SCL_MAX_NAME_LEN - 1);
    }

    const char* fc = SclParser_GetAttr(fcda_node, "fc");
    fcda->fc = SclParser_ParseFC(fc);

    fcda->ix = SclParser_GetAttrInt(fcda_node, "ix", -1);
}

static void parse_dataset(SclValidator* validator, mxml_node_t* ds_node, SclDataSet* dataset)
{
    const char* name = SclParser_GetAttr(ds_node, "name");
    if (name != NULL) {
        strncpy(dataset->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(ds_node, "desc");
    if (desc != NULL) {
        strncpy(dataset->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    dataset->fcda_count = 0;

    /* Parse FCDA elements */
    mxml_node_t* fcda_node;
    for (fcda_node = mxmlFindElement(ds_node, ds_node, "FCDA", NULL, NULL, MXML_DESCEND_FIRST);
         fcda_node != NULL && dataset->fcda_count < SCL_MAX_FCDAS;
         fcda_node = mxmlFindElement(fcda_node, ds_node, "FCDA", NULL, NULL, MXML_NO_DESCEND)) {

        parse_fcda(validator, fcda_node, &dataset->fcdas[dataset->fcda_count++]);
    }
}

static void parse_report_control(SclValidator* validator, mxml_node_t* rc_node, SclReportControl* rc)
{
    const char* name = SclParser_GetAttr(rc_node, "name");
    if (name != NULL) {
        strncpy(rc->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(rc_node, "desc");
    if (desc != NULL) {
        strncpy(rc->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    const char* rpt_id = SclParser_GetAttr(rc_node, "rptID");
    if (rpt_id != NULL && strcmp(rpt_id, "") != 0) {
        strncpy(rc->rpt_id, rpt_id, SCL_MAX_ID_LEN - 1);
    }

    rc->buffered = SclParser_GetAttrBool(rc_node, "buffered", false);

    const char* dat_set = SclParser_GetAttr(rc_node, "datSet");
    if (dat_set != NULL && strcmp(dat_set, "") != 0) {
        strncpy(rc->dat_set, dat_set, SCL_MAX_NAME_LEN - 1);
    }

    rc->conf_rev = SclParser_GetAttrUInt(rc_node, "confRev", 1);
    rc->trg_ops = SclParser_GetAttrInt(rc_node, "trgOps", 0);
    rc->opt_fields = SclParser_GetAttrInt(rc_node, "optFields", 0);
    rc->buf_time = SclParser_GetAttrUInt(rc_node, "bufTime", 0);
    rc->intg_pd = SclParser_GetAttrUInt(rc_node, "intgPd", 0);
    rc->indexed = SclParser_GetAttrBool(rc_node, "indexed", false);
}

static void parse_gse_control(SclValidator* validator, mxml_node_t* gc_node, SclGSEControl* gc)
{
    const char* name = SclParser_GetAttr(gc_node, "name");
    if (name != NULL) {
        strncpy(gc->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(gc_node, "desc");
    if (desc != NULL) {
        strncpy(gc->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    const char* dat_set = SclParser_GetAttr(gc_node, "datSet");
    if (dat_set != NULL && strcmp(dat_set, "") != 0) {
        strncpy(gc->dat_set, dat_set, SCL_MAX_NAME_LEN - 1);
    }

    gc->conf_rev = SclParser_GetAttrUInt(gc_node, "confRev", 1);
    gc->fixed_offs = SclParser_GetAttrBool(gc_node, "fixedOffs", false);
    gc->min_time = SclParser_GetAttrInt(gc_node, "minTime", -1);
    gc->max_time = SclParser_GetAttrInt(gc_node, "maxTime", -1);
    gc->indexed = SclParser_GetAttrBool(gc_node, "indexed", false);
}

static void parse_smv_control(SclValidator* validator, mxml_node_t* smvc_node, SclSMVControl* smvc)
{
    const char* name = SclParser_GetAttr(smvc_node, "name");
    if (name != NULL) {
        strncpy(smvc->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(smvc_node, "desc");
    if (desc != NULL) {
        strncpy(smvc->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    const char* dat_set = SclParser_GetAttr(smvc_node, "datSet");
    if (dat_set != NULL && strcmp(dat_set, "") != 0) {
        strncpy(smvc->dat_set, dat_set, SCL_MAX_NAME_LEN - 1);
    }

    smvc->conf_rev = SclParser_GetAttrUInt(smvc_node, "confRev", 1);
    smvc->smp_mod = SclParser_GetAttrInt(smvc_node, "smpMod", 0);
    smvc->smp_rate = SclParser_GetAttrInt(smvc_node, "smpRate", 0);
    smvc->opt_fields = SclParser_GetAttrInt(smvc_node, "optFields", 0);
    smvc->is_unicast = SclParser_GetAttrBool(smvc_node, "multicast", true) ? false : true;
    smvc->indexed = SclParser_GetAttrBool(smvc_node, "indexed", false);
}

static void parse_log_control(SclValidator* validator, mxml_node_t* lc_node, SclLogControl* lc)
{
    const char* name = SclParser_GetAttr(lc_node, "name");
    if (name != NULL) {
        strncpy(lc->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(lc_node, "desc");
    if (desc != NULL) {
        strncpy(lc->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    const char* dat_set = SclParser_GetAttr(lc_node, "datSet");
    if (dat_set != NULL && strcmp(dat_set, "") != 0) {
        strncpy(lc->dat_set, dat_set, SCL_MAX_NAME_LEN - 1);
    }

    const char* log_ref = SclParser_GetAttr(lc_node, "logRef");
    if (log_ref != NULL) {
        strncpy(lc->log_ref, log_ref, SCL_MAX_OBJREF_LEN - 1);
    }

    lc->trg_ops = SclParser_GetAttrUInt(lc_node, "trgOps", 0);
    lc->intg_pd = SclParser_GetAttrUInt(lc_node, "intgPd", 0);
    lc->log_ena = SclParser_GetAttrBool(lc_node, "logEna", true);
    lc->with_reason_code = SclParser_GetAttrBool(lc_node, "withReasonCode", false);
}

static void parse_log(SclValidator* validator, mxml_node_t* log_node, SclLog* log)
{
    const char* name = SclParser_GetAttr(log_node, "name");
    if (name != NULL) {
        strncpy(log->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(log_node, "desc");
    if (desc != NULL) {
        strncpy(log->desc, desc, SCL_MAX_DESC_LEN - 1);
    }
}

static void parse_setting_control(SclValidator* validator, mxml_node_t* sg_node, SclSettingControl* sg)
{
    sg->act_sg = SclParser_GetAttrInt(sg_node, "actSG", 1);
    sg->num_of_sgs = SclParser_GetAttrInt(sg_node, "numOfSGs", 1);

    const char* desc = SclParser_GetAttr(sg_node, "desc");
    if (desc != NULL) {
        strncpy(sg->desc, desc, SCL_MAX_DESC_LEN - 1);
    }
}

static void parse_ln(SclValidator* validator, mxml_node_t* ln_node, SclLN* ln)
{
    const char* prefix = SclParser_GetAttr(ln_node, "prefix");
    if (prefix != NULL) {
        strncpy(ln->prefix, prefix, SCL_MAX_NAME_LEN - 1);
    }

    const char* ln_class = SclParser_GetAttr(ln_node, "lnClass");
    if (ln_class != NULL) {
        strncpy(ln->ln_class, ln_class, SCL_MAX_NAME_LEN - 1);
    }

    const char* ln_type = SclParser_GetAttr(ln_node, "lnType");
    if (ln_type != NULL) {
        strncpy(ln->ln_type, ln_type, SCL_MAX_ID_LEN - 1);
    }

    const char* inst = SclParser_GetAttr(ln_node, "inst");
    if (inst != NULL) {
        strncpy(ln->inst, inst, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(ln_node, "desc");
    if (desc != NULL) {
        strncpy(ln->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    ln->doi_count = 0;
    ln->dataset_count = 0;
    ln->report_control_count = 0;
    ln->gse_control_count = 0;
    ln->smv_control_count = 0;
    ln->log_control_count = 0;
    ln->log_count = 0;
    ln->has_setting_control = false;

    /* Parse DOI elements */
    mxml_node_t* doi_node;
    for (doi_node = mxmlFindElement(ln_node, ln_node, "DOI", NULL, NULL, MXML_DESCEND_FIRST);
         doi_node != NULL && ln->doi_count < SCL_MAX_DATA_OBJECTS;
         doi_node = mxmlFindElement(doi_node, ln_node, "DOI", NULL, NULL, MXML_NO_DESCEND)) {

        parse_doi(validator, doi_node, &ln->dois[ln->doi_count++]);
    }

    /* Parse DataSet elements */
    mxml_node_t* ds_node;
    for (ds_node = mxmlFindElement(ln_node, ln_node, "DataSet", NULL, NULL, MXML_DESCEND_FIRST);
         ds_node != NULL && ln->dataset_count < SCL_MAX_DATASETS;
         ds_node = mxmlFindElement(ds_node, ln_node, "DataSet", NULL, NULL, MXML_NO_DESCEND)) {

        parse_dataset(validator, ds_node, &ln->datasets[ln->dataset_count++]);
    }

    /* Parse ReportControl elements */
    mxml_node_t* rc_node;
    for (rc_node = mxmlFindElement(ln_node, ln_node, "ReportControl", NULL, NULL, MXML_DESCEND_FIRST);
         rc_node != NULL && ln->report_control_count < SCL_MAX_REPORT_CBS;
         rc_node = mxmlFindElement(rc_node, ln_node, "ReportControl", NULL, NULL, MXML_NO_DESCEND)) {

        parse_report_control(validator, rc_node, &ln->report_controls[ln->report_control_count++]);
    }

    /* Parse GSEControl elements */
    mxml_node_t* gc_node;
    for (gc_node = mxmlFindElement(ln_node, ln_node, "GSEControl", NULL, NULL, MXML_DESCEND_FIRST);
         gc_node != NULL && ln->gse_control_count < SCL_MAX_GSE_CBS;
         gc_node = mxmlFindElement(gc_node, ln_node, "GSEControl", NULL, NULL, MXML_NO_DESCEND)) {

        parse_gse_control(validator, gc_node, &ln->gse_controls[ln->gse_control_count++]);
    }

    /* Parse SMVControl elements */
    mxml_node_t* smvc_node;
    for (smvc_node = mxmlFindElement(ln_node, ln_node, "SMVControl", NULL, NULL, MXML_DESCEND_FIRST);
         smvc_node != NULL && ln->smv_control_count < SCL_MAX_SMV_CBS;
         smvc_node = mxmlFindElement(smvc_node, ln_node, "SMVControl", NULL, NULL, MXML_NO_DESCEND)) {

        parse_smv_control(validator, smvc_node, &ln->smv_controls[ln->smv_control_count++]);
    }

    /* Parse LogControl elements */
    mxml_node_t* lc_node;
    for (lc_node = mxmlFindElement(ln_node, ln_node, "LogControl", NULL, NULL, MXML_DESCEND_FIRST);
         lc_node != NULL && ln->log_control_count < SCL_MAX_LOG_CBS;
         lc_node = mxmlFindElement(lc_node, ln_node, "LogControl", NULL, NULL, MXML_NO_DESCEND)) {

        parse_log_control(validator, lc_node, &ln->log_controls[ln->log_control_count++]);
    }

    /* Parse Log elements */
    mxml_node_t* log_node;
    for (log_node = mxmlFindElement(ln_node, ln_node, "Log", NULL, NULL, MXML_DESCEND_FIRST);
         log_node != NULL && ln->log_count < SCL_MAX_LOG_CBS;
         log_node = mxmlFindElement(log_node, ln_node, "Log", NULL, NULL, MXML_NO_DESCEND)) {

        parse_log(validator, log_node, &ln->logs[ln->log_count++]);
    }

    /* Parse SettingGroup element */
    mxml_node_t* sg_node = mxmlFindElement(ln_node, ln_node, "SettingGroup", NULL, NULL, MXML_DESCEND_FIRST);
    if (sg_node != NULL) {
        parse_setting_control(validator, sg_node, &ln->setting_control);
        ln->has_setting_control = true;
    }
}

static void parse_ldevice(SclValidator* validator, mxml_node_t* ld_node, SclLDevice* ldevice)
{
    const char* inst = SclParser_GetAttr(ld_node, "inst");
    if (inst != NULL) {
        strncpy(ldevice->inst, inst, SCL_MAX_NAME_LEN - 1);
    }

    const char* ld_name = SclParser_GetAttr(ld_node, "ldName");
    if (ld_name != NULL) {
        strncpy(ldevice->ld_name, ld_name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(ld_node, "desc");
    if (desc != NULL) {
        strncpy(ldevice->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    ldevice->logical_node_count = 0;

    /* Parse LN0 */
    mxml_node_t* ln0_node = mxmlFindElement(ld_node, ld_node, "LN0", NULL, NULL, MXML_DESCEND_FIRST);
    if (ln0_node != NULL) {
        parse_ln(validator, ln0_node, &ldevice->ln0);

        const char* ln_type = SclParser_GetAttr(ln0_node, "lnType");
        if (ln_type != NULL) {
            strncpy(ldevice->ln_type, ln_type, SCL_MAX_ID_LEN - 1);
        }
    } else {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_STRUCTURE, -1,
                                "LDevice missing LN0", ldevice->inst, "MissingLN0");
    }

    /* Parse LN elements */
    mxml_node_t* ln_node;
    for (ln_node = mxmlFindElement(ld_node, ld_node, "LN", NULL, NULL, MXML_DESCEND_FIRST);
         ln_node != NULL && ldevice->logical_node_count < SCL_MAX_LNODES;
         ln_node = mxmlFindElement(ln_node, ld_node, "LN", NULL, NULL, MXML_NO_DESCEND)) {

        parse_ln(validator, ln_node, &ldevice->logical_nodes[ldevice->logical_node_count++]);
    }
}

static void parse_server(SclValidator* validator, mxml_node_t* server_node, SclServer* server)
{
    const char* desc = SclParser_GetAttr(server_node, "desc");
    if (desc != NULL) {
        strncpy(server->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    const char* authentication = SclParser_GetAttr(server_node, "authentication");
    if (authentication != NULL) {
        strncpy(server->authentication, authentication, SCL_MAX_NAME_LEN - 1);
    }

    server->timeout = SclParser_GetAttrInt(server_node, "timeout", 0);
    server->ldevice_count = 0;

    /* Parse LDevice elements */
    mxml_node_t* ld_node;
    for (ld_node = mxmlFindElement(server_node, server_node, "LDevice", NULL, NULL, MXML_DESCEND_FIRST);
         ld_node != NULL && server->ldevice_count < SCL_MAX_LDEVICES;
         ld_node = mxmlFindElement(ld_node, server_node, "LDevice", NULL, NULL, MXML_NO_DESCEND)) {

        parse_ldevice(validator, ld_node, &server->ldevices[server->ldevice_count++]);
    }

    if (server->ldevice_count == 0) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_STRUCTURE, -1,
                                "Server has no LDevice", NULL, "MissingLDevice");
    }
}

static void parse_access_point(SclValidator* validator, mxml_node_t* ap_node, SclAccessPoint* ap)
{
    const char* name = SclParser_GetAttr(ap_node, "name");
    if (name == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "AccessPoint has no name attribute", NULL, "MissingName");
    } else {
        strncpy(ap->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(ap_node, "desc");
    if (desc != NULL) {
        strncpy(ap->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    ap->has_server = false;
    ap->server = NULL;

    /* Parse Server element */
    mxml_node_t* server_node = mxmlFindElement(ap_node, ap_node, "Server", NULL, NULL, MXML_DESCEND_FIRST);
    if (server_node != NULL) {
        ap->server = (SclServer*)calloc(1, sizeof(SclServer));
        if (ap->server != NULL) {
            parse_server(validator, server_node, ap->server);
            ap->has_server = true;
        }
    }

    /* Parse ServerAt element */
    mxml_node_t* server_at_node = mxmlFindElement(ap_node, ap_node, "ServerAt", NULL, NULL, MXML_DESCEND_FIRST);
    if (server_at_node != NULL) {
        const char* ap_name = SclParser_GetAttr(server_at_node, "apName");
        if (ap_name != NULL) {
            strncpy(ap->server_at, ap_name, SCL_MAX_NAME_LEN - 1);
        } else {
            SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                    "ServerAt has no apName attribute", ap->name, "MissingApName");
        }
    }
}

static void parse_ied(SclValidator* validator, mxml_node_t* ied_node, SclIED* ied)
{
    const char* name = SclParser_GetAttr(ied_node, "name");
    if (name == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "IED has no name attribute", NULL, "MissingName");
    } else {
        strncpy(ied->name, name, SCL_MAX_NAME_LEN - 1);

        /* Validate IED name format */
        for (const char* p = name; *p != '\0'; p++) {
            if (!isalnum(*p) && *p != '_' && *p != '-' && *p != '.') {
                SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_INVALID_VALUE, -1,
                                        "Invalid character in IED name", name, "InvalidName");
                break;
            }
        }
    }

    const char* desc = SclParser_GetAttr(ied_node, "desc");
    if (desc != NULL) {
        strncpy(ied->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    const char* manufacturer = SclParser_GetAttr(ied_node, "manufacturer");
    if (manufacturer != NULL) {
        strncpy(ied->manufacturer, manufacturer, SCL_MAX_NAME_LEN - 1);
    }

    const char* type = SclParser_GetAttr(ied_node, "type");
    if (type != NULL) {
        strncpy(ied->type, type, SCL_MAX_NAME_LEN - 1);
    }

    const char* config_rev = SclParser_GetAttr(ied_node, "configRevision");
    if (config_rev != NULL) {
        strncpy(ied->config_revision, config_rev, SCL_MAX_NAME_LEN - 1);
    }

    ied->access_point_count = 0;

    /* Parse AccessPoint elements */
    mxml_node_t* ap_node;
    for (ap_node = mxmlFindElement(ied_node, ied_node, "AccessPoint", NULL, NULL, MXML_DESCEND_FIRST);
         ap_node != NULL && ied->access_point_count < SCL_MAX_ACCESS_POINTS;
         ap_node = mxmlFindElement(ap_node, ied_node, "AccessPoint", NULL, NULL, MXML_NO_DESCEND)) {

        parse_access_point(validator, ap_node, &ied->access_points[ied->access_point_count++]);
    }

    if (ied->access_point_count == 0) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_STRUCTURE, -1,
                                "IED has no AccessPoint", ied->name, "MissingAccessPoint");
    }
}

void SclParser_ParseIEDs(SclValidator* validator, void* parent)
{
    validator->ied_count = 0;

    mxml_node_t* ied_node;
    for (ied_node = mxmlFindElement((mxml_node_t*)parent, (mxml_node_t*)validator->xml_doc,
                                    "IED", NULL, NULL, MXML_DESCEND_FIRST);
         ied_node != NULL && validator->ied_count < SCL_MAX_IEDS;
         ied_node = mxmlFindElement(ied_node, (mxml_node_t*)validator->xml_doc,
                                    "IED", NULL, NULL, MXML_NO_DESCEND)) {

        validator->ieds[validator->ied_count] = (SclIED*)calloc(1, sizeof(SclIED));
        if (validator->ieds[validator->ied_count] != NULL) {
            parse_ied(validator, ied_node, validator->ieds[validator->ied_count]);
            validator->ied_count++;
        }
    }

    if (validator->ied_count == 0) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_WARNING, SCL_ISSUE_STRUCTURE, -1,
                                "No IED elements found in SCL file", NULL, "NoIED");
    }
}

/* ==================== Communication Parsing ==================== */

static void parse_p_element(SclValidator* validator, mxml_node_t* p_node, SclP* p)
{
    const char* type = SclParser_GetAttr(p_node, "type");
    p->p_type = SclParser_ParsePType(type);

    const char* text = mxmlGetText(p_node, NULL);
    if (text != NULL) {
        strncpy(p->text, text, SCL_MAX_VALUE_LEN - 1);
    }
}

static void parse_gse(SclValidator* validator, mxml_node_t* gse_node, SclGSE* gse)
{
    const char* cb_name = SclParser_GetAttr(gse_node, "cbName");
    if (cb_name != NULL) {
        strncpy(gse->cb_name, cb_name, SCL_MAX_NAME_LEN - 1);
    } else {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "GSE has no cbName attribute", NULL, "MissingCbName");
    }

    const char* ld_inst = SclParser_GetAttr(gse_node, "ldInst");
    if (ld_inst != NULL) {
        strncpy(gse->ld_inst, ld_inst, SCL_MAX_NAME_LEN - 1);
    }

    gse->min_time = SclParser_GetAttrInt(gse_node, "minTime", -1);
    gse->max_time = SclParser_GetAttrInt(gse_node, "maxTime", -1);
    gse->p_count = 0;

    /* Parse P elements */
    mxml_node_t* p_node;
    for (p_node = mxmlFindElement(gse_node, gse_node, "P", NULL, NULL, MXML_DESCEND_FIRST);
         p_node != NULL && gse->p_count < 16;
         p_node = mxmlFindElement(p_node, gse_node, "P", NULL, NULL, MXML_NO_DESCEND)) {

        parse_p_element(validator, p_node, &gse->ps[gse->p_count++]);
    }
}

static void parse_smv(SclValidator* validator, mxml_node_t* smv_node, SclSMV* smv)
{
    const char* cb_name = SclParser_GetAttr(smv_node, "cbName");
    if (cb_name != NULL) {
        strncpy(smv->cb_name, cb_name, SCL_MAX_NAME_LEN - 1);
    } else {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "SMV has no cbName attribute", NULL, "MissingCbName");
    }

    const char* ld_inst = SclParser_GetAttr(smv_node, "ldInst");
    if (ld_inst != NULL) {
        strncpy(smv->ld_inst, ld_inst, SCL_MAX_NAME_LEN - 1);
    }

    smv->p_count = 0;

    /* Parse P elements */
    mxml_node_t* p_node;
    for (p_node = mxmlFindElement(smv_node, smv_node, "P", NULL, NULL, MXML_DESCEND_FIRST);
         p_node != NULL && smv->p_count < 16;
         p_node = mxmlFindElement(p_node, smv_node, "P", NULL, NULL, MXML_NO_DESCEND)) {

        parse_p_element(validator, p_node, &smv->ps[smv->p_count++]);
    }
}

static void parse_connected_ap(SclValidator* validator, mxml_node_t* cap_node, SclConnectedAP* cap)
{
    const char* ied_name = SclParser_GetAttr(cap_node, "iedName");
    if (ied_name == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "ConnectedAP is missing iedName attribute", NULL, "MissingIedName");
    } else {
        strncpy(cap->ied_name, ied_name, SCL_MAX_NAME_LEN - 1);
    }

    const char* ap_name = SclParser_GetAttr(cap_node, "apName");
    if (ap_name == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "ConnectedAP is missing apName attribute", NULL, "MissingApName");
    } else {
        strncpy(cap->ap_name, ap_name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(cap_node, "desc");
    if (desc != NULL) {
        strncpy(cap->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    cap->gse_count = 0;
    cap->smv_count = 0;
    cap->p_count = 0;

    /* Parse P elements */
    mxml_node_t* p_node;
    for (p_node = mxmlFindElement(cap_node, cap_node, "P", NULL, NULL, MXML_DESCEND_FIRST);
         p_node != NULL && cap->p_count < 16;
         p_node = mxmlFindElement(p_node, cap_node, "P", NULL, NULL, MXML_NO_DESCEND)) {

        parse_p_element(validator, p_node, &cap->ps[cap->p_count++]);
    }

    /* Parse GSE elements */
    mxml_node_t* gse_node;
    for (gse_node = mxmlFindElement(cap_node, cap_node, "GSE", NULL, NULL, MXML_DESCEND_FIRST);
         gse_node != NULL && cap->gse_count < SCL_MAX_GSE_CBS;
         gse_node = mxmlFindElement(gse_node, cap_node, "GSE", NULL, NULL, MXML_NO_DESCEND)) {

        parse_gse(validator, gse_node, &cap->gses[cap->gse_count++]);
    }

    /* Parse SMV elements */
    mxml_node_t* smv_node;
    for (smv_node = mxmlFindElement(cap_node, cap_node, "SMV", NULL, NULL, MXML_DESCEND_FIRST);
         smv_node != NULL && cap->smv_count < SCL_MAX_SMV_CBS;
         smv_node = mxmlFindElement(smv_node, cap_node, "SMV", NULL, NULL, MXML_NO_DESCEND)) {

        parse_smv(validator, smv_node, &cap->smvs[cap->smv_count++]);
    }
}

static void parse_subnetwork(SclValidator* validator, mxml_node_t* sn_node, SclSubNetwork* subnetwork)
{
    const char* name = SclParser_GetAttr(sn_node, "name");
    if (name == NULL) {
        SclValidator_AddMessage(validator, SCL_SEVERITY_ERROR, SCL_ISSUE_MISSING_ATTRIBUTE, -1,
                                "SubNetwork has no name attribute", NULL, "MissingName");
    } else {
        strncpy(subnetwork->name, name, SCL_MAX_NAME_LEN - 1);
    }

    const char* desc = SclParser_GetAttr(sn_node, "desc");
    if (desc != NULL) {
        strncpy(subnetwork->desc, desc, SCL_MAX_DESC_LEN - 1);
    }

    const char* type = SclParser_GetAttr(sn_node, "type");
    if (type != NULL) {
        strncpy(subnetwork->type, type, SCL_MAX_NAME_LEN - 1);
    }

    subnetwork->connected_ap_count = 0;

    /* Parse ConnectedAP elements */
    mxml_node_t* cap_node;
    for (cap_node = mxmlFindElement(sn_node, sn_node, "ConnectedAP", NULL, NULL, MXML_DESCEND_FIRST);
         cap_node != NULL && subnetwork->connected_ap_count < SCL_MAX_CONNECTED_APS;
         cap_node = mxmlFindElement(cap_node, sn_node, "ConnectedAP", NULL, NULL, MXML_NO_DESCEND)) {

        parse_connected_ap(validator, cap_node, &subnetwork->connected_aps[subnetwork->connected_ap_count++]);
    }
}

void SclParser_ParseCommunication(SclValidator* validator, void* parent)
{
    validator->subnetwork_count = 0;

    mxml_node_t* comm_node = mxmlFindElement((mxml_node_t*)parent, (mxml_node_t*)validator->xml_doc,
                                             "Communication", NULL, NULL, MXML_DESCEND_FIRST);

    if (comm_node == NULL) {
        /* Communication section is optional */
        return;
    }

    /* Parse SubNetwork elements */
    mxml_node_t* sn_node;
    for (sn_node = mxmlFindElement(comm_node, comm_node, "SubNetwork", NULL, NULL, MXML_DESCEND_FIRST);
         sn_node != NULL && validator->subnetwork_count < SCL_MAX_SUBNETWORKS;
         sn_node = mxmlFindElement(sn_node, comm_node, "SubNetwork", NULL, NULL, MXML_NO_DESCEND)) {

        validator->subnetworks[validator->subnetwork_count] = (SclSubNetwork*)calloc(1, sizeof(SclSubNetwork));
        if (validator->subnetworks[validator->subnetwork_count] != NULL) {
            parse_subnetwork(validator, sn_node, validator->subnetworks[validator->subnetwork_count]);
            validator->subnetwork_count++;
        }
    }
}

/* ==================== Memory Cleanup ==================== */

static void free_sdi(SclSDI* sdi)
{
    if (sdi == NULL) {
        return;
    }

    for (int i = 0; i < sdi->sub_sdi_count; i++) {
        free_sdi(sdi->sub_sdis[i]);
    }
}

static void free_doi(SclDOI* doi)
{
    if (doi == NULL) {
        return;
    }

    for (int i = 0; i < doi->sdi_count; i++) {
        free_sdi(doi->sdis[i]);
    }
}

static void free_ln(SclLN* ln)
{
    if (ln == NULL) {
        return;
    }

    for (int i = 0; i < ln->doi_count; i++) {
        free_doi(&ln->dois[i]);
    }
}

static void free_ldevice(SclLDevice* ldevice)
{
    if (ldevice == NULL) {
        return;
    }

    free_ln(&ldevice->ln0);

    for (int i = 0; i < ldevice->logical_node_count; i++) {
        free_ln(&ldevice->logical_nodes[i]);
    }
}

static void free_server(SclServer* server)
{
    if (server == NULL) {
        return;
    }

    for (int i = 0; i < server->ldevice_count; i++) {
        free_ldevice(&server->ldevices[i]);
    }
}

static void free_access_point(SclAccessPoint* ap)
{
    if (ap == NULL) {
        return;
    }

    free_server(ap->server);
}

void SclParser_FreeIED(SclIED* ied)
{
    if (ied == NULL) {
        return;
    }

    for (int i = 0; i < ied->access_point_count; i++) {
        free_access_point(&ied->access_points[i]);
    }

    free(ied);
}

void SclParser_FreeSubNetwork(SclSubNetwork* subnetwork)
{
    if (subnetwork == NULL) {
        return;
    }

    free(subnetwork);
}