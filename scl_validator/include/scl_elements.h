/*
 * scl_elements.h - SCL Element Structures
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#ifndef SCL_ELEMENTS_H_
#define SCL_ELEMENTS_H_

#include "scl_types.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== Header ==================== */
struct SclHeader {
    char id[SCL_MAX_ID_LEN];
    char version[SCL_MAX_NAME_LEN];      /* e.g., "2007" */
    char revision[SCL_MAX_NAME_LEN];     /* e.g., "B" or "B4" */
    char tool_id[SCL_MAX_NAME_LEN];
    char name_structure[SCL_MAX_NAME_LEN]; /* IEDNameStructure */
};

/* ==================== DataTypeTemplates ==================== */
struct SclEnumValue {
    int ord;                              /* Ordinal value */
    char symbolic_name[SCL_MAX_NAME_LEN]; /* Symbolic name */
};

struct SclEnumType {
    char id[SCL_MAX_ID_LEN];
    SclEnumValue values[SCL_MAX_ENUM_VALUES];
    int value_count;
    bool is_used;
};

struct SclDataAttributeDefinition {
    char name[SCL_MAX_NAME_LEN];
    SclBTypeType btype;
    char type[SCL_MAX_ID_LEN];           /* For ENUMERATED/STRUCT types */
    SclFCType fc;
    int trg_ops;                          /* Trigger options bit flags */
    int dchg;                             /* Data change trigger */
    int qchg;                             /* Quality change trigger */
    int dupd;                             /* Data update trigger */
    char val[SCL_MAX_VALUE_LEN];          /* Default value */
    int saddr;                            /* Short address */
    int count;                            /* Array count */
    bool is_used;
};

struct SclDAType {
    char id[SCL_MAX_ID_LEN];
    char desc[SCL_MAX_DESC_LEN];
    SclDataAttributeDefinition attributes[SCL_MAX_DATA_ATTRIBUTES];
    int attribute_count;
    bool is_used;
};

struct SclDataObjectDefinition {
    char name[SCL_MAX_NAME_LEN];
    char type[SCL_MAX_ID_LEN];           /* Reference to DOType */
    bool is_used;
};

struct SclDOType {
    char id[SCL_MAX_ID_LEN];
    char cdc[SCL_MAX_NAME_LEN];          /* Common Data Class */
    char desc[SCL_MAX_DESC_LEN];
    SclDataObjectDefinition data_objects[SCL_MAX_DATA_OBJECTS];
    int data_object_count;
    SclDataAttributeDefinition data_attributes[SCL_MAX_DATA_ATTRIBUTES];
    int data_attribute_count;
    bool is_used;
};

struct SclLNodeType {
    char id[SCL_MAX_ID_LEN];
    char ln_class[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    SclDataObjectDefinition data_objects[SCL_MAX_DATA_OBJECTS];
    int data_object_count;
    bool is_used;
};

struct SclDataTypeTemplates {
    SclLNodeType lnode_types[SCL_MAX_LNODE_TYPES];
    int lnode_type_count;
    SclDOType do_types[SCL_MAX_DO_TYPES];
    int do_type_count;
    SclDAType da_types[SCL_MAX_DA_TYPES];
    int da_type_count;
    SclEnumType enum_types[SCL_MAX_ENUM_TYPES];
    int enum_type_count;
};

/* ==================== DOI/DAI/SDI ==================== */
struct SclVal {
    char value[SCL_MAX_VALUE_LEN];
    char sGroup[SCL_MAX_NAME_LEN];       /* Setting group number */
};

struct SclDAI {
    char name[SCL_MAX_NAME_LEN];
    SclVal values[16];                    /* Multiple values possible */
    int value_count;
    char val_kind[SCL_MAX_NAME_LEN];     /* Conf, RO, etc. */
    int ix;                               /* Array index */
};

struct SclSDI {
    char name[SCL_MAX_NAME_LEN];
    int ix;                               /* Array index */
    SclDAI dais[SCL_MAX_DATA_ATTRIBUTES];
    int dai_count;
    SclSDI* sub_sdis[SCL_MAX_DATA_ATTRIBUTES]; /* Nested SDIs */
    int sub_sdi_count;
};

struct SclDOI {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    int ix;                               /* Array index */
    SclDAI dais[SCL_MAX_DATA_ATTRIBUTES];
    int dai_count;
    SclSDI* sdis[SCL_MAX_DATA_OBJECTS];
    int sdi_count;
};

/* ==================== LN ==================== */
struct SclLN {
    char prefix[SCL_MAX_NAME_LEN];
    char ln_class[SCL_MAX_NAME_LEN];
    char ln_type[SCL_MAX_ID_LEN];        /* Reference to LNodeType */
    char inst[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    
    SclDOI dois[SCL_MAX_DATA_OBJECTS];
    int doi_count;
    
    SclDataSet datasets[SCL_MAX_DATASETS];
    int dataset_count;
    
    SclReportControl report_controls[SCL_MAX_REPORT_CBS];
    int report_control_count;
    
    SclGSEControl gse_controls[SCL_MAX_GSE_CBS];
    int gse_control_count;
    
    SclSMVControl smv_controls[SCL_MAX_SMV_CBS];
    int smv_control_count;
    
    SclLogControl log_controls[SCL_MAX_LOG_CBS];
    int log_control_count;
    
    SclLog logs[SCL_MAX_LOG_CBS];
    int log_count;
    
    SclSettingControl setting_control;
    bool has_setting_control;
};

/* ==================== LDevice ==================== */
struct SclLDevice {
    char inst[SCL_MAX_NAME_LEN];
    char ld_name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    char ln_type[SCL_MAX_ID_LEN];        /* For LN0 */
    
    SclLN ln0;                            /* LN0 (required) */
    SclLN logical_nodes[SCL_MAX_LNODES];
    int logical_node_count;
};

/* ==================== Server ==================== */
struct SclServer {
    char desc[SCL_MAX_DESC_LEN];
    char authentication[SCL_MAX_NAME_LEN]; /* None, Password, Strong, etc. */
    int timeout;                          /* Connection timeout */
    
    SclLDevice ldevices[SCL_MAX_LDEVICES];
    int ldevice_count;
};

/* ==================== AccessPoint ==================== */
struct SclAccessPoint {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    SclServer* server;
    bool has_server;
    char server_at[SCL_MAX_NAME_LEN];    /* Reference to another AP */
};

/* ==================== IED ==================== */
struct SclIED {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    char manufacturer[SCL_MAX_NAME_LEN];
    char type[SCL_MAX_NAME_LEN];
    char config_revision[SCL_MAX_NAME_LEN];
    char original_scl_revision[SCL_MAX_NAME_LEN];
    char original_scl_version[SCL_MAX_NAME_LEN];
    char engineering_revision[SCL_MAX_NAME_LEN];
    
    SclAccessPoint access_points[SCL_MAX_ACCESS_POINTS];
    int access_point_count;
};

/* ==================== DataSet & FCDA ==================== */
struct SclFCDA {
    char ld_inst[SCL_MAX_NAME_LEN];
    char prefix[SCL_MAX_NAME_LEN];
    char ln_class[SCL_MAX_NAME_LEN];
    char ln_inst[SCL_MAX_NAME_LEN];
    char do_name[SCL_MAX_NAME_LEN];
    char da_name[SCL_MAX_NAME_LEN];
    SclFCType fc;
    int ix;                               /* Array index */
};

struct SclDataSet {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    SclFCDA fcdas[SCL_MAX_FCDAS];
    int fcda_count;
};

/* ==================== Control Blocks ==================== */
struct SclReportControl {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    char rpt_id[SCL_MAX_ID_LEN];
    bool buffered;
    char dat_set[SCL_MAX_NAME_LEN];      /* DataSet reference */
    uint32_t conf_rev;
    int trg_ops;
    int opt_fields;
    uint32_t buf_time;
    uint32_t intg_pd;
    bool indexed;
};

struct SclGSEControl {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    char dat_set[SCL_MAX_NAME_LEN];
    uint32_t conf_rev;
    bool fixed_offs;
    int min_time;
    int max_time;
    bool indexed;
};

struct SclSMVControl {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    char dat_set[SCL_MAX_NAME_LEN];
    uint32_t conf_rev;
    int smp_mod;
    int smp_rate;
    int opt_fields;
    bool is_unicast;
    bool indexed;
};

struct SclLogControl {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    char dat_set[SCL_MAX_NAME_LEN];
    char log_ref[SCL_MAX_OBJREF_LEN];
    uint32_t trg_ops;
    uint32_t intg_pd;
    bool log_ena;
    bool with_reason_code;
};

struct SclLog {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
};

struct SclSettingControl {
    int act_sg;                           /* Active setting group */
    int num_of_sgs;                       /* Number of setting groups */
    char desc[SCL_MAX_DESC_LEN];
};

/* ==================== Communication Section ==================== */
struct SclP {
    SclPTypeType p_type;
    char text[SCL_MAX_VALUE_LEN];        /* Value of P element */
};

struct SclGSE {
    char cb_name[SCL_MAX_NAME_LEN];
    char ld_inst[SCL_MAX_NAME_LEN];
    SclP ps[16];
    int p_count;
    int min_time;
    int max_time;
};

struct SclSMV {
    char cb_name[SCL_MAX_NAME_LEN];
    char ld_inst[SCL_MAX_NAME_LEN];
    SclP ps[16];
    int p_count;
};

struct SclConnectedAP {
    char ied_name[SCL_MAX_NAME_LEN];
    char ap_name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    SclGSE gses[SCL_MAX_GSE_CBS];
    int gse_count;
    SclSMV smvs[SCL_MAX_SMV_CBS];
    int smv_count;
    SclP ps[16];
    int p_count;
};

struct SclSubNetwork {
    char name[SCL_MAX_NAME_LEN];
    char desc[SCL_MAX_DESC_LEN];
    char type[SCL_MAX_NAME_LEN];         /* 8-MMS, IP, etc. */
    SclConnectedAP connected_aps[SCL_MAX_CONNECTED_APS];
    int connected_ap_count;
};

#ifdef __cplusplus
}
#endif

#endif /* SCL_ELEMENTS_H_ */