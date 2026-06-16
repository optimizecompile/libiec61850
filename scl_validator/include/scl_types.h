/*
 * scl_types.h - SCL Validator Type Definitions
 *
 * Copyright 2025
 *
 * This file is part of SCL Validator for IEC 61850.
 *
 * Licensed under the Apache License, Version 2.0.
 */

#ifndef SCL_TYPES_H_
#define SCL_TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum string lengths */
#define SCL_MAX_NAME_LEN        65
#define SCL_MAX_ID_LEN          130
#define SCL_MAX_DESC_LEN        256
#define SCL_MAX_PATH_LEN        512
#define SCL_MAX_VALUE_LEN       512
#define SCL_MAX_OBJREF_LEN      256

/* Maximum array sizes */
#define SCL_MAX_IEDS            32
#define SCL_MAX_ACCESS_POINTS   16
#define SCL_MAX_LDEVICES        64
#define SCL_MAX_LNODES          256
#define SCL_MAX_DATA_OBJECTS    128
#define SCL_MAX_DATA_ATTRIBUTES 256
#define SCL_MAX_DATASETS        64
#define SCL_MAX_FCDAS           128
#define SCL_MAX_REPORT_CBS      32
#define SCL_MAX_GSE_CBS         32
#define SCL_MAX_SMV_CBS         16
#define SCL_MAX_LOG_CBS         16
#define SCL_MAX_SUBNETWORKS     16
#define SCL_MAX_CONNECTED_APS   64

/* DataTypeTemplates limits */
#define SCL_MAX_LNODE_TYPES     256
#define SCL_MAX_DO_TYPES        256
#define SCL_MAX_DA_TYPES        256
#define SCL_MAX_ENUM_TYPES      128
#define SCL_MAX_ENUM_VALUES     64

/* Error message limits */
#define SCL_MAX_MESSAGES        512

/* Severity levels */
typedef enum {
    SCL_SEVERITY_ERROR = 0,
    SCL_SEVERITY_WARNING = 1,
    SCL_SEVERITY_INFO = 2
} SclSeverityType;

/* Issue types */
typedef enum {
    SCL_ISSUE_SCHEMA = 0,           /* XML Schema validation error */
    SCL_ISSUE_MODEL_INTEGRITY = 1,   /* Model integrity error */
    SCL_ISSUE_MISSING_ATTRIBUTE = 2, /* Required attribute missing */
    SCL_ISSUE_INVALID_VALUE = 3,     /* Invalid attribute value */
    SCL_ISSUE_REFERENCE = 4,         /* Reference to undefined element */
    SCL_ISSUE_STRUCTURE = 5          /* Structural error */
} SclIssueType;

/* Functional Constraints (FC) */
typedef enum {
    SCL_FC_ST = 0,   /* Status */
    SCL_FC_MX = 1,   /* Measured values */
    SCL_FC_CF = 2,   /* Configuration */
    SCL_FC_DC = 3,   /* Description */
    SCL_FC_SP = 4,   /* Setpoint */
    SCL_FC_SG = 5,   /* Setting group */
    SCL_FC_SE = 6,   /* Setting group editable */
    SCL_FC_SR = 7,   /* Service request */
    SCL_FC_OR = 8,   /* Operate received */
    SCL_FC_BL = 9,   /* Blocking */
    SCL_FC_EX = 10,  /* Extended definition */
    SCL_FC_CO = 11,  /* Control */
    SCL_FC_US = 12,  /* User defined */
    SCL_FC_MS = 13,  /* Mirror status */
    SCL_FC_EN = 14,  /* Engineering */
    SCL_FC_UNKNOWN = 15
} SclFCType;

/* Data Attribute Types (bType) */
typedef enum {
    SCL_BTYPE_BOOLEAN = 0,
    SCL_BTYPE_INT8 = 1,
    SCL_BTYPE_INT16 = 2,
    SCL_BTYPE_INT24 = 3,
    SCL_BTYPE_INT32 = 4,
    SCL_BTYPE_INT64 = 5,
    SCL_BTYPE_INT8U = 6,
    SCL_BTYPE_INT16U = 7,
    SCL_BTYPE_INT24U = 8,
    SCL_BTYPE_INT32U = 9,
    SCL_BTYPE_FLOAT32 = 10,
    SCL_BTYPE_FLOAT64 = 11,
    SCL_BTYPE_ENUMERATED = 12,
    SCL_BTYPE_VISIBLE_STRING_32 = 13,
    SCL_BTYPE_VISIBLE_STRING_64 = 14,
    SCL_BTYPE_VISIBLE_STRING_65 = 15,
    SCL_BTYPE_VISIBLE_STRING_129 = 16,
    SCL_BTYPE_VISIBLE_STRING_255 = 17,
    SCL_BTYPE_UNICODE_STRING_255 = 18,
    SCL_BTYPE_OCTET_STRING_64 = 19,
    SCL_BTYPE_TIMESTAMP = 20,
    SCL_BTYPE_QUALITY = 21,
    SCL_BTYPE_CHECK = 22,
    SCL_BTYPE_CODED_ENUM = 23,
    SCL_BTYPE_OBJECT_REFERENCE = 24,
    SCL_BTYPE_ENTRY_TIME = 25,
    SCL_BTYPE_PHYCOMADDR = 26,
    SCL_BTYPE_CURRENCY = 27,
    SCL_BTYPE_STRUCT = 28,
    SCL_BTYPE_UNKNOWN = 29
} SclBTypeType;

/* Trigger Options (trgOps) bit flags */
typedef enum {
    SCL_TRGOPS_DATA_CHANGE = 0x01,
    SCL_TRGOPS_QUALITY_CHANGE = 0x02,
    SCL_TRGOPS_DATA_UPDATE = 0x04,
    SCL_TRGOPS_GENERAL_INTERROGATION = 0x08,
    SCL_TRGOPS_INTEGRITY = 0x10
} SclTrgOpsFlags;

/* Common Data Classes (CDC) - subset */
typedef enum {
    SCL_CDC_SPS = 0,    /* Single Point Status */
    SCL_CDC_DPS = 1,    /* Double Point Status */
    SCL_CDC_INS = 2,    /* Integer Status */
    SCL_CDC_ENS = 3,    /* Enumerated Status */
    SCL_CDC_MVS = 4,    /* Measured Value Status */
    SCL_CDC_MV = 5,     /* Measured Value */
    SCL_CDC_CMV = 6,    /* Complex Measured Value */
    SCL_CDC_SPC = 7,    /* Single Point Controllable */
    SCL_CDC_DPC = 8,    /* Double Point Controllable */
    SCL_CDC_INC = 9,    /* Integer Controllable */
    SCL_CDC_ENC = 10,   /* Enumerated Controllable */
    SCL_CDC_APC = 11,   /* Analog Process Controllable */
    SCL_CDC_BSC = 12,   /* Binary Security Controllable */
    SCL_CDC_ISC = 13,   /* Integer Security Controllable */
    SCL_CDC_LPL = 14,   /* Logical Node Name Plate */
    SCL_CDC_DPL = 15,   /* Device Name Plate */
    SCL_CDC_SG = 16,    /* Setting Group */
    SCL_CDC_ARR = 17,   /* Array */
    SCL_CDC_UNKNOWN = 18
} SclCDCType;

/* P address types in Communication section */
typedef enum {
    SCL_P_TYPE_IP = 0,
    SCL_P_TYPE_SUBNET = 1,
    SCL_P_TYPE_GATEWAY = 2,
    SCL_P_TYPE_VLAN_ID = 3,
    SCL_P_TYPE_VLAN_PRIORITY = 4,
    SCL_P_TYPE_APPID = 5,
    SCL_P_TYPE_MAC_ADDRESS = 6,
    SCL_P_TYPE_OSITSEL = 7,
    SCL_P_TYPE_OSIPSSEL = 8,
    SCL_P_TYPE_OSITSSEL = 9,
    SCL_P_TYPE_SCLURI = 10,
    SCL_P_TYPE_UNKNOWN = 11
} SclPTypeType;

/* Forward declarations for structures */
typedef struct SclValidatorMessage SclValidatorMessage;
typedef struct SclValidator SclValidator;
typedef struct SclHeader SclHeader;
typedef struct SclIED SclIED;
typedef struct SclAccessPoint SclAccessPoint;
typedef struct SclServer SclServer;
typedef struct SclLDevice SclLDevice;
typedef struct SclLN SclLN;
typedef struct SclDOI SclDOI;
typedef struct SclDAI SclDAI;
typedef struct SclSDI SclSDI;
typedef struct SclDataSet SclDataSet;
typedef struct SclFCDA SclFCDA;
typedef struct SclReportControl SclReportControl;
typedef struct SclGSEControl SclGSEControl;
typedef struct SclSMVControl SclSMVControl;
typedef struct SclLogControl SclLogControl;
typedef struct SclLog SclLog;
typedef struct SclSettingControl SclSettingControl;
typedef struct SclDataTypeTemplates SclDataTypeTemplates;
typedef struct SclLNodeType SclLNodeType;
typedef struct SclDOType SclDOType;
typedef struct SclDAType SclDAType;
typedef struct SclEnumType SclEnumType;
typedef struct SclSubNetwork SclSubNetwork;
typedef struct SclConnectedAP SclConnectedAP;
typedef struct SclGSE SclGSE;
typedef struct SclSMV SclSMV;
typedef struct SclP SclP;

/* DO/DA Definition structures */
typedef struct SclDataObjectDefinition SclDataObjectDefinition;
typedef struct SclDataAttributeDefinition SclDataAttributeDefinition;
typedef struct SclEnumValue SclEnumValue;

#ifdef __cplusplus
}
#endif

#endif /* SCL_TYPES_H_ */