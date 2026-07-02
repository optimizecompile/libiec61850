# libIEC61850 项目 Wiki

> 本 Wiki 基于 libIEC61850 开源仓库（GPLv3），由 MZ Automation GmbH 维护，是 IEC 61850 协议的 C 语言实现。

---

## 目录

- [1. 项目概览](#1-项目概览)
- [2. 仓库结构](#2-仓库结构)
- [3. 整体架构](#3-整体架构)
- [4. 核心模块详解](#4-核心模块详解)
  - [4.1 IEC 61850 应用层](#41-iec-61850-应用层)
  - [4.2 MMS 协议层](#42-mms-协议层)
  - [4.3 GOOSE / SV 实时通信](#43-goose--sv-实时通信)
  - [4.4 硬件抽象层 (HAL)](#44-硬件抽象层-hal)
  - [4.5 通用工具层](#45-通用工具层)
- [5. 数据模型](#5-数据模型)
- [6. 服务器端 API](#6-服务器端-api)
- [7. 客户端 API](#7-客户端-api)
- [8. 配置文件解析器](#8-配置文件解析器)
- [9. SCL 文件校验器](#9-scl-文件校验器)
- [10. 工具与绑定](#10-工具与绑定)
- [11. 构建系统](#11-构建系统)
- [12. 示例索引](#12-示例索引)
- [13. 配置选项](#13-配置选项)
- [14. 版本历史](#14-版本历史)

---

## 1. 项目概览

| 属性 | 说明 |
|------|------|
| 项目名称 | libIEC61850 |
| 语言 | C99 (核心库), Java (工具), C#/.NET (绑定), Python (绑定) |
| 许可证 | GPLv3 |
| 维护方 | MZ Automation GmbH |
| 当前版本 | 1.6.1 |
| 支持平台 | Linux, Windows, macOS, BSD, 嵌入式 ARM |
| 协议支持 | MMS (客户端/服务器), GOOSE, Sampled Values (SV), R-GOOSE/R-SV, SNTP |

### 核心特性

- **完全自包含**：内置完整 MMS 协议栈（ISO/OSI 上层 + ASN.1 BER 编解码），无外部 MMS 库依赖
- **跨平台**：通过 HAL 抽象层支持多种 OS 和嵌入式平台
- **三种数据模型构建方式**：静态 C 代码、动态 API、配置文件解析
- **TLS 支持**：集成 mbedtls（2.28 和 3.x 两个版本）
- **Edition 1/2/2.1**：支持 IEC 61850 多版本
- **丰富示例**：40+ 示例覆盖所有协议功能

---

## 2. 仓库结构

```
libiec61850/
├── src/                          # 核心库源代码
│   ├── common/                   # 通用工具（链表、缓冲区、字符串等）
│   ├── iec61850/                 # IEC 61850 核心实现
│   │   ├── client/               #   客户端实现
│   │   ├── server/               #   服务器端实现
│   │   │   ├── impl/             #     IedServer 主实现
│   │   │   ├── mms_mapping/      #     IEC61850↔MMS 映射层
│   │   │   └── model/            #     数据模型与配置文件解析
│   │   ├── inc/                  #   公共 API 头文件
│   │   └── inc_private/          #   私有内部头文件
│   ├── mms/                      # MMS 协议实现
│   │   ├── asn1/                 #   BER 编解码原语
│   │   ├── iso_acse/             #   ACSE（认证）
│   │   ├── iso_client/           #   ISO 客户端连接
│   │   ├── iso_common/           #   ISO 公共参数
│   │   ├── iso_cotp/             #   COTP 传输协议
│   │   ├── iso_mms/              #   MMS 核心
│   │   │   ├── asn1c/            #     asn1c 自动生成代码
│   │   │   ├── client/           #     MMS 客户端服务
│   │   │   ├── common/           #     MMS 公共（MmsValue 等）
│   │   │   └── server/           #     MMS 服务器端服务
│   │   ├── iso_presentation/     #   表示层
│   │   ├── iso_server/           #   ISO 服务器
│   │   └── iso_session/          #   会话层
│   ├── goose/                    # GOOSE 发布/订阅
│   ├── sampled_values/           # SV 采样值发布/订阅
│   ├── r_session/                # R-Session（R-GOOSE/R-SV）
│   ├── sntp/                     # SNTP 时间同步客户端
│   ├── logging/                  # 日志存储服务
│   └── doxygen/                  # Doxygen 文档配置
├── hal/                          # 硬件抽象层
│   ├── inc/                      # HAL 头文件
│   ├── ethernet/                 # 以太网（Linux/BSD/Win32）
│   ├── socket/                   # Socket（Linux/BSD/Win32）
│   ├── thread/                   # 线程/信号量
│   ├── filesystem/               # 文件系统
│   ├── time/                     # 时间
│   ├── serial/                   # 串口
│   ├── memory/                   # 内存管理
│   └── tls/                      # TLS（mbedtls/mbedtls3）
├── examples/                     # C 示例程序（40+）
├── tools/                        # Java 模型生成工具
│   └── model_generator/          #   genmodel.jar, genconfig.jar 等
├── dotnet/                       # C#/.NET 绑定与示例
├── pyiec61850/                   # Python 绑定（SWIG）
├── scl_validator/                # SCL 文件校验器（C 实现）
├── config/                       # 栈配置
│   └── stack_config.h            #   编译期配置开关
├── third_party/                  # 第三方依赖（mbedtls/sqlite/winpcap）
├── fuzz/                         # 模糊测试
├── demos/                        # BeagleBone 演示
├── CMakeLists.txt                # 主 CMake 构建脚本
├── Makefile                      # 主 Makefile
├── README.md                     # 项目说明
├── CHANGELOG                     # 变更日志
└── COPYING                       # GPLv3 许可证
```

---

## 3. 整体架构

libIEC61850 采用四层架构设计：

```
┌─────────────────────────────────────────────────────────┐
│                    应用代码 (用户程序)                     │
├─────────────────────────────────────────────────────────┤
│           IEC 61850 应用 API 层                           │
│  ┌─────────────────┐    ┌──────────────────┐           │
│  │  IedServer API  │    │ IedConnection API │           │
│  │  (服务器端)      │    │ (客户端)          │           │
│  └────────┬────────┘    └────────┬─────────┘           │
├───────────┼──────────────────────┼─────────────────────┤
│           │  协议映射与 MMS 层     │                     │
│  ┌────────▼────────┐    ┌────────▼─────────┐           │
│  │   MmsMapping    │    │  MmsConnection   │           │
│  │ (IEC61850↔MMS)  │    │                  │           │
│  └────────┬────────┘    └────────┬─────────┘           │
│           │                       │                     │
│  ┌────────▼───────────────────────▼─────────┐          │
│  │     MMS 协议栈 (ISO/OSI 上层)              │          │
│  │  ACSE → Presentation → Session → COTP     │          │
│  │  + ASN.1 BER 编解码 (asn1c 生成)           │          │
│  └───────────────────┬──────────────────────┘          │
├──────────────────────┼──────────────────────────────────┤
│   实时发布订阅层      │         TCP/IP 通路               │
│  ┌──────────┐  ┌───┐ │  ┌──────────┐                    │
│  │  GOOSE   │  │SV │ │  │   TLS    │                    │
│  │Publisher │  │Pub│ │  │ (mbedtls)│                    │
│  │Subscriber│  │Sub│ │  └────┬─────┘                    │
│  └────┬─────┘  └─┬─┘ │       │                          │
│       │  R-Session │       │                          │
│       │  (R-GOOSE/ │       │                          │
│       │   R-SV)    │       │                          │
├───────┼────────────┼───────┼──────────────────────────┤
│       │   HAL 平台抽象层  │                          │
│  ┌────▼────┐  ┌────▼──┐  ┌▼──────────┐               │
│  │Ethernet │  │Socket │  │ Thread/   │               │
│  │(二层)   │  │(TCP) │  │ File/Time │               │
│  └────┬────┘  └───┬───┘  └───────────┘               │
├───────┼───────────┼──────────────────────────────────┤
│       │     操作系统 (Linux/Windows/BSD/macOS)        │
└───────┴───────────┴──────────────────────────────────┘
```

**关键设计原则**：
1. MMS 通信走 TCP（ISO/OSI 上层协议栈），GOOSE/SV 走二层以太网直发
2. R-Session 在 GOOSE/SV 之上插入 IP 多播和安全层
3. HAL 统一抽象所有 OS 资源，上层代码完全平台无关
4. 所有 MMS PDU 编解码由 asn1c 自动生成的代码完成

---

## 4. 核心模块详解

### 4.1 IEC 61850 应用层

**位置**: `src/iec61850/`

这是与用户最直接交互的层，提供面向对象风格的 C API。

#### 公共 API 头文件

| 头文件 | 说明 |
|--------|------|
| [iec61850_server.h](file:///workspace/src/iec61850/inc/iec61850_server.h) | 服务器端 API（~2100行）：IedServer 生命周期、数据更新、控制回调、报告、GOOSE 发布、设置组、访问控制 |
| [iec61850_client.h](file:///workspace/src/iec61850/inc/iec61850_client.h) | 客户端 API（~3100行）：连接管理、数据读写、报告订阅、控制操作、模型发现、文件传输 |
| [iec61850_model.h](file:///workspace/src/iec61850/inc/iec61850_model.h) | 数据模型核心定义：IedModel, LogicalDevice, LogicalNode, DataObject, DataAttribute, 各类 ControlBlock |
| [iec61850_common.h](file:///workspace/src/iec61850/inc/iec61850_common.h) | 公共类型：FunctionalConstraint, ControlModel, Quality, Dbpos, Timestamp, PhyComAddress 等 |
| [iec61850_dynamic_model.h](file:///workspace/src/iec61850/inc/iec61850_dynamic_model.h) | 动态模型创建 API：运行时构建 IedModel |
| [iec61850_cdc.h](file:///workspace/src/iec61850/inc/iec61850_cdc.h) | CDC 辅助函数：SPS, DPS, MV, SPC, DPC, INC, ASG 等标准化创建 |
| [iec61850_config_file_parser.h](file:///workspace/src/iec61850/inc/iec61850_config_file_parser.h) | 配置文件解析器：从文本配置文件加载模型 |

#### 服务器端核心结构

```c
// 内部结构 (ied_server_private.h)
struct sIedServer {
    IedModel* model;              // 数据模型
    MmsDevice* mmsDevice;         // MMS 设备模型
    MmsServer mmsServer;          // MMS 服务器实例
    MmsMapping* mmsMapping;       // IEC61850↔MMS 映射
    LinkedList clientConnections; // 客户端连接列表
    Semaphore dataModelLock;      // 数据模型锁
    uint8_t writeAccessPolicies;  // 写访问策略位
    Thread serverThread;          // 服务器线程
    uint8_t edition;              // IEC 61850 版本
    bool running;
};
```

#### 客户端连接状态机

```
CLOSED → CONNECTING → CONNECTED → CLOSING → CLOSED
                        ↓
                   (连接丢失)
                        ↓
                      CLOSED
```

---

### 4.2 MMS 协议层

**位置**: `src/mms/`

libIEC61850 实现了完整的自包含 MMS 协议栈，无需外部 MMS 库。

#### ISO/OSI 上层协议栈

```
应用层:  MMS (Manufacturing Message Specification)
表示层:  ISO 8823 (Presentation)
会话层:  ISO 8327 (Session)
传输层:  ISO 8073 (COTP - Connection-Oriented Transport)
关联层:  ISO/IEC 8650 (ACSE - Association Control)
承载层:  TCP/IP
```

#### MMS 服务实现

| 服务 | 服务器端文件 | 客户端文件 | 说明 |
|------|-------------|-----------|------|
| Read | [mms_read_service.c](file:///workspace/src/mms/iso_mms/server/mms_read_service.c) | [mms_client_read.c](file:///workspace/src/mms/iso_mms/client/mms_client_read.c) | 读取变量值 |
| Write | [mms_write_service.c](file:///workspace/src/mms/iso_mms/server/mms_write_service.c) | [mms_client_write.c](file:///workspace/src/mms/iso_mms/client/mms_client_write.c) | 写入变量值 |
| GetNameList | [mms_get_namelist_service.c](file:///workspace/src/mms/iso_mms/server/mms_get_namelist_service.c) | [mms_client_get_namelist.c](file:///workspace/src/mms/iso_mms/client/mms_client_get_namelist.c) | 目录列表 |
| Identify | [mms_identify_service.c](file:///workspace/src/mms/iso_mms/server/mms_identify_service.c) | [mms_client_identify.c](file:///workspace/src/mms/iso_mms/client/mms_client_identify.c) | 设备标识 |
| InformationReport | [mms_information_report.c](file:///workspace/src/mms/iso_mms/server/mms_information_report.c) | - | 报告推送 |
| NamedVariableList | [mms_named_variable_list_service.c](file:///workspace/src/mms/iso_mms/server/mms_named_variable_list_service.c) | [mms_client_named_variable_list.c](file:///workspace/src/mms/iso_mms/client/mms_client_named_variable_list.c) | 数据集 CRUD |
| Journal | [mms_journal_service.c](file:///workspace/src/mms/iso_mms/server/mms_journal_service.c) | [mms_client_journals.c](file:///workspace/src/mms/iso_mms/client/mms_client_journals.c) | 日志查询 |
| File | [mms_file_service.c](file:///workspace/src/mms/iso_mms/server/mms_file_service.c) | [mms_client_files.c](file:///workspace/src/mms/iso_mms/client/mms_client_files.c) | 文件传输 |
| Status | [mms_status_service.c](file:///workspace/src/mms/iso_mms/server/mms_status_service.c) | [mms_client_status.c](file:///workspace/src/mms/iso_mms/client/mms_client_status.c) | VMD 状态 |
| Initiate | - | [mms_client_initiate.c](file:///workspace/src/mms/iso_mms/client/mms_client_initiate.c) | 连接初始化 |

#### ASN.1 BER 编解码

- 基础原语: [src/mms/asn1/](file:///workspace/src/mms/asn1/) — ber_decode.c, ber_encoder.c, ber_integer.c
- 自动生成: [src/mms/iso_mms/asn1c/](file:///workspace/src/mms/iso_mms/asn1c/) — 由 asn1c 工具基于 MMS ASN.1 定义生成，覆盖所有 PDU 类型

#### MmsValue 类型系统

```c
typedef enum {
    MMS_ARRAY, MMS_STRUCTURE, MMS_BOOLEAN, MMS_BIT_STRING,
    MMS_INTEGER, MMS_UNSIGNED, MMS_FLOAT, MMS_OCTET_STRING,
    MMS_VISIBLE_STRING, MMS_GENERALIZED_TIME, MMS_BINARY_TIME,
    MMS_BCD, MMS_OBJ_ID, MMS_STRING, MMS_UTC_TIME,
    MMS_DATA_ACCESS_ERROR
} MmsType;
```

#### IEC 61850 ↔ MMS 映射层

**位置**: [src/iec61850/server/mms_mapping/](file:///workspace/src/iec61850/server/mms_mapping/)

| 文件 | 映射内容 |
|------|---------|
| [mms_mapping.c](file:///workspace/src/iec61850/server/mms_mapping/mms_mapping.c) | 核心映射：IedModel→MmsDevice, 数据模型转换 |
| [control.c](file:///workspace/src/iec61850/server/mms_mapping/control.c) | 控制对象：Oper/Select/Cancel/SBOw |
| [reporting.c](file:///workspace/src/iec61850/server/mms_mapping/reporting.c) | 报告控制块：BRCB/URCB |
| [logging.c](file:///workspace/src/iec61850/server/mms_mapping/logging.c) | 日志控制块：LCB |
| [mms_goose.c](file:///workspace/src/iec61850/server/mms_mapping/mms_goose.c) | GOOSE 控制块映射 |
| [mms_sv.c](file:///workspace/src/iec61850/server/mms_mapping/mms_sv.c) | SV 控制块映射 |

---

### 4.3 GOOSE / SV 实时通信

#### GOOSE (IEC 61850-8-1)

**位置**: [src/goose/](file:///workspace/src/goose/)

GOOSE 走二层以太网直发，不经过 MMS/TCP 栈。

| 文件 | 说明 |
|------|------|
| [goose_publisher.c](file:///workspace/src/goose/goose_publisher.c) | GOOSE 发布器 |
| [goose_subscriber.c](file:///workspace/src/goose/goose_subscriber.c) | GOOSE 订阅器 |
| [goose_receiver.c](file:///workspace/src/goose/goose_receiver.c) | GOOSE 接收器（管理多个订阅者） |

**GOOSE 发布流程**:
```
用户设置 CommParameters (VLAN/AppID/MAC)
  → GoosePublisher_create()
  → 设置 GoID/DataSetRef/ConfRev 等
  → GoosePublisher_publish(dataSetValues)
  → ASN.1 BER 编码 → Ethernet 二层发送
```

**GOOSE 订阅流程**:
```
GooseSubscriber_create(goCbRef, dataSetValues)
  → 设置监听回调
  → GooseReceiver_create()
  → GooseReceiver_addSubscriber()
  → GooseReceiver_start()  (内部线程接收)
  → 收到报文 → ASN.1 解码 → 触发回调
```

#### Sampled Values (IEC 61850-9-2)

**位置**: [src/sampled_values/](file:///workspace/src/sampled_values/)

| 文件 | 说明 |
|------|------|
| [sv_publisher.c](file:///workspace/src/sampled_values/sv_publisher.c) | SV 发布器 |
| [sv_subscriber.c](file:///workspace/src/sampled_values/sv_subscriber.c) | SV 订阅器 |

**SV 发布关键概念**:
- ASDU (Application Service Data Unit): 每个采样点
- SmpCnt: 采样计数器
- SmpRate: 采样率（如 4000 Hz）
- SmpMod: 采样模式

#### R-Session (R-GOOSE / R-SV, IEC 61850 90-5)

**位置**: [src/r_session/](file:///workspace/src/r_session/)

R-Session 提供 IP 多播传输的 GOOSE/SV，支持 AES-GCM 加密和 HMAC 签名。

```c
RSession session = RSession_create();
RSession_setSecurity(session, R_SECURITY_ALG_AES_128_GCM, R_SIGNATURE_ALG_HMAC_SHA256_128);
RSession_addKey(session, keyId, key, keyLen, ...);
RSession_start(session);

// 创建 R-GOOSE 发布器
GoosePublisher publisher = GoosePublisher_createRemote(session, appId);
```

---

### 4.4 硬件抽象层 (HAL)

**位置**: [hal/](file:///workspace/hal/)

HAL 统一抽象所有 OS 资源，使协议栈完全平台无关。

| 模块 | 头文件 | 平台实现 | 用途 |
|------|--------|---------|------|
| Socket | [hal_socket.h](file:///workspace/hal/inc/hal_socket.h) | linux/bsd/win32 | TCP/UDP 通信（MMS） |
| Ethernet | [hal_ethernet.h](file:///workspace/hal/inc/hal_ethernet.h) | linux/bsd/win32 | 二层以太网（GOOSE/SV） |
| Thread | [hal_thread.h](file:///workspace/hal/inc/hal_thread.h) | linux/bsd/macos/win32 | 线程与信号量 |
| Filesystem | [hal_filesystem.h](file:///workspace/hal/inc/hal_filesystem.h) | linux/win32 | 文件操作 |
| Time | [hal_time.h](file:///workspace/hal/inc/hal_time.h) | unix/win32 | 高精度时间 |
| Serial | [hal_serial.h](file:///workspace/hal/inc/hal_serial.h) | linux/win32 | 串口通信 |
| TLS | [tls_socket.h](file:///workspace/hal/inc/tls_socket.h) | mbedtls/mbedtls3 | TLS 加密 |
| Memory | [lib_memory.h](file:///workspace/hal/inc/lib_memory.h) | - | 内存管理 |

**移植到新平台**：只需实现 HAL 头文件中定义的函数集即可。

---

### 4.5 通用工具层

**位置**: [src/common/](file:///workspace/src/common/)

| 文件 | 说明 |
|------|------|
| [linked_list.c](file:///workspace/src/common/linked_list.c) | 单向链表 |
| [mem_alloc_linked_list.c](file:///workspace/src/common/mem_alloc_linked_list.c) | 自管理内存链表 |
| [map.c](file:///workspace/src/common/map.c) | 哈希映射 |
| [string_map.c](file:///workspace/src/common/string_map.c) | 字符串键映射 |
| [buffer_chain.c](file:///workspace/src/common/buffer_chain.c) | 缓冲区链 |
| [byte_buffer.c](file:///workspace/src/common/byte_buffer.c) | 字节缓冲区 |
| [conversions.c](file:///workspace/src/common/conversions.c) | 类型转换 |
| [string_utilities.c](file:///workspace/src/common/string_utilities.c) | 字符串工具 |
| [simple_allocator.c](file:///workspace/src/common/simple_allocator.c) | 简单内存池 |

---

## 5. 数据模型

### 模型层次结构

```
IedModel (IED 顶级容器)
├── firstChild → LogicalDevice (LD)
│                ├── firstChild → LogicalNode (LN)
│                │                └── firstChild → DataObject (DO)
│                │                                 └── firstChild → DataAttribute (DA) 或子 DO
│                ├── sibling → 下一个 LN
│                └── ...
├── dataSets → DataSet → DataSetEntry
├── rcbs → ReportControlBlock (RCB)
├── gseCBs → GSEControlBlock (GoCB)
├── svCBs → SVControlBlock (SVCB)
├── sgcbs → SettingGroupControlBlock (SGCB)
├── lcbs → LogControlBlock (LCB)
└── logs → Log
```

### 核心结构体

所有模型节点共享 `ModelNode` 基类：
```c
struct sModelNode {
    ModelNodeType modelType;   // 节点类型
    char* name;                // 名称
    ModelNode* parent;         // 父节点
    ModelNode* sibling;        // 兄弟节点
    ModelNode* firstChild;     // 第一个子节点
};
```

### FunctionalConstraint (FC)

| FC | 全称 | 用途 |
|----|------|------|
| ST | Status | 状态数据 |
| MX | Measurement | 测量值 |
| CO | Control | 控制操作 |
| SP | Setpoint | 设定值 |
| SV | Substituted Value | 替代值 |
| CF | Configuration | 配置 |
| DC | Description | 描述 |
| SG | Setting Group | 设置组 |
| SE | Setting Group Edit | 设置组编辑 |
| SR | Service Response | 服务响应 |
| OR | Object Reference | 对象引用 |
| BL | Blocking | 闭锁 |
| EX | Extended | 扩展定义 |
| US | Unicast SV | 单播 SV |
| MS | Multicast SV | 多播 SV |
| RP | Unbuffered Report | 非缓冲报告 |
| BR | Buffered Report | 缓冲报告 |
| LG | Log | 日志 |
| GO | GOOSE | GOOSE 控制 |

### 三种模型构建方式

| 方式 | 工具/方法 | 优点 | 缺点 |
|------|----------|------|------|
| **静态模型** | `genmodel.jar` 从 SCL 生成 C 代码 | 编译期确定，最高效 | 修改需重新编译 |
| **动态模型** | `iec61850_dynamic_model.h` API | 运行时灵活构建 | 代码量较大 |
| **配置文件** | `genconfig.jar` 生成配置 → `ConfigFileParser` 运行时加载 | 易维护，无需重编译 | 需额外解析步骤 |

---

## 6. 服务器端 API

### 生命周期

```c
// 创建
IedServer server = IedServer_create(model);
// 或带 TLS
IedServer server = IedServer_createWithTlsSupport(model, tlsConfig);
// 或带配置
IedServer server = IedServer_createWithConfig(model, tlsConfig, serverConfig);

// 启动
IedServer_start(server, 102);  // TCP 端口 102 (MMS 标准)

// 运行中...

// 停止与销毁
IedServer_stop(server);
IedServer_destroy(server);
```

### 数据更新

```c
// 按类型更新
IedServer_updateBooleanAttributeValue(server, da, true);
IedServer_updateInt32AttributeValue(server, da, 100);
IedServer_updateFloatAttributeValue(server, da, 3.14f);
IedServer_updateQuality(server, da, quality);
IedServer_updateTimestampAttributeValue(server, da, &timestamp);
```

### 控制回调

```c
// 设置控制处理器
IedServer_setControlHandler(server, ctlObject, controlHandler, param);
// 设置检查处理器
IedServer_setPerformCheckHandler(server, ctlObject, checkHandler, param);
// 设置等待执行处理器
IedServer_setWaitForExecutionHandler(server, ctlObject, waitHandler, param);
```

### 非线程模式

适用于没有线程支持的平台或需要集成到已有事件循环的场景：

```c
IedServer_startThreadless(server, 102);
while (running) {
    IedServer_waitReady(server, 100);      // 等待数据
    IedServer_processIncomingData(server);  // 处理输入
    IedServer_performPeriodicTasks(server); // 周期任务（报告/GOOSE）
}
IedServer_stopThreadless(server);
```

### 访问控制

```c
// 按功能约束设置写策略
IedServer_setWriteAccessPolicy(server, IEC61850_FC_CF, ACCESS_POLICY_ALLOW);
// 单个属性写访问回调
IedServer_handleWriteAccess(server, da, writeHandler, param);
// 读访问回调
IedServer_setReadAccessHandler(server, readHandler, param);
```

---

## 7. 客户端 API

### 连接管理

```c
IedConnection con = IedConnection_create();
IedConnectionState state = IedConnection_connect(con, &error, "192.168.1.1", 102);
if (state == IED_STATE_CONNECTED) {
    // 执行操作...
    IedConnection_close(con);
}
IedConnection_destroy(con);
```

### 数据读写

```c
// 同步读
MmsValue* value = IedConnection_readObject(con, &error, "ied1LD0/LLN0.Health.stVal", IEC61850_FC_ST);

// 类型化读
float voltage = IedConnection_readFloatValue(con, &error, "ied1LD0/MMXU1.PhV.phsA.cVal.mag.f", IEC61850_FC_MX);

// 同步写
IedConnection_writeBooleanValue(con, &error, "ied1LD0/CSWI1.Pos.stVal", IEC61850_FC_CO, true);
```

### 异步操作

```c
// 异步读
IedConnection_readObjectAsync(con, &error, ref, fc, readHandler, param);
// 在事件循环中
IedConnection_tick(con);  // 驱动异步操作
```

### 报告订阅

```c
// 获取 RCB 值
ClientReportControlBlock rcb = IedConnection_getRCBValues(con, &error, rcbRef, NULL);
// 设置 RCB 参数
ClientReportControlBlock_setRptEna(rcb, true);
ClientReportControlBlock_setResv(rcb, true);
// 写回 RCB
IedConnection_setRCBValues(con, &error, rcb, RCB_ELEMENT_RPT_ENA | RCB_ELEMENT_RESV);
// 安装报告处理器
IedConnection_installReportHandler(con, rcbRef, rptId, reportHandler, param);
```

### 控制操作

```c
ControlObjectClient ctl = ControlObjectClient_create("ied1LD0/CSWI1.Pos", con);
// 直接操作
ControlObjectClient_operate(ctl, ctlVal, 0);
// 选择后操作（SBO）
ControlObjectClient_select(ctl);
ControlObjectClient_operate(ctl, ctlVal, 0);
// 带值选择
ControlObjectClient_selectWithValue(ctl, ctlVal);
ControlObjectClient_operate(ctl, ctlVal, 0);
```

### 模型发现

```c
// 获取逻辑设备列表
LinkedList ldList = IedConnection_getLogicalDeviceList(con, &error);
// 获取 LN 目录
LinkedList lnList = IedConnection_getLogicalDeviceDirectory(con, &error, ldName);
// 获取数据目录
LinkedList dataDir = IedConnection_getDataDirectory(con, &error, dataRef);
```

---

## 8. 配置文件解析器

**位置**: [src/iec61850/server/model/config_file_parser.c](file:///workspace/src/iec61850/server/model/config_file_parser.c)

这是一个基于文本行的简单解析器，从 `genconfig.jar` 生成的配置文件加载 IedModel。

### 工作流程

```
genconfig.jar (SCL → .cfg 配置文件)
                        ↓
ConfigFileParser_createModelFromConfigFileEx("model.cfg")
                        ↓
    逐行解析 → 调用 dynamic_model API 构建 IedModel
                        ↓
                    返回 IedModel*
```

### 支持的指令

| 指令 | 功能 |
|------|------|
| `IEDNAME:` | 设置 IED 名称 |
| `LD:` | 创建 LogicalDevice |
| `LN:` | 创建 LogicalNode |
| `DO:` | 创建 DataObject |
| `DA:` | 创建 DataAttribute |
| `DATASET:` | 创建 DataSet |
| `DATASETENTRY:` | 创建 DataSetEntry |
| `RCB:` | 创建 ReportControlBlock |
| `GoCB:` | 创建 GSEControlBlock |
| `SVCB:` | 创建 SVControlBlock |
| `SGCB:` | 创建 SettingGroupControlBlock |
| `LCB:` | 创建 LogControlBlock |
| `SETVALUE:` | 设置初始值 |

---

## 9. SCL 文件校验器

**位置**: [scl_validator/](file:///workspace/scl_validator/)

这是基于本仓库 .NET SCLParser 逻辑用 C 语言重新实现的 SCL/CCD/CID 文件校验工具，专为嵌入式设备设计。

### 校验能力

| 校验类别 | 检查项 |
|---------|--------|
| XML 结构 | SCL 根元素、命名空间、Header |
| DataTypeTemplates | LNodeType/DOType/DAType/EnumType 引用完整性、未使用类型检测 |
| IED 模型 | IED 名称合法性、AccessPoint/Server/LDevice/LN 层级、LN0 必须存在 |
| DAI 值 | BOOLEAN/INT/FLOAT/ENUM/Base64 格式与范围检查 |
| 控制块 | ReportControl/GSEControl DataSet 引用、SettingGroup 位置 |
| 通信段 | P 地址格式（IP/MAC/VLAN/APPID）、ConnectedAP 引用 |
| 跨引用 | IED↔Communication、DataSet↔控制块、FCDA 路径 |

### 使用方式

```bash
# 命令行工具
./scl_validate device.icd -v

# API 调用
SclValidator* validator = SclValidator_Create();
bool ok = SclValidator_ValidateFile(validator, "device.icd");
SclValidator_PrintReport(validator, stdout);
SclValidator_Destroy(validator);
```

---

## 10. 工具与绑定

### Java 模型生成工具

**位置**: [tools/model_generator/](file:///workspace/tools/model_generator/)

| 工具 | 用途 |
|------|------|
| `genmodel.jar` | 从 SCL 文件生成静态 C 模型代码（static_model.c/.h） |
| `genconfig.jar` | 从 SCL 文件生成文本配置文件（.cfg） |
| `gendyncode.jar` | 生成动态模型构建 C 代码 |
| `modelviewer.jar` | SCL 模型查看器（GUI） |

### C#/.NET 绑定

**位置**: [dotnet/](file:///workspace/dotnet/)

- 完整的 C# API 包装（IEC61850.NET.csproj）
- 20+ C# 示例项目
- 支持 .NET Core 2.0

### Python 绑定

**位置**: [pyiec61850/](file:///workspace/pyiec61850/)

- 基于 SWIG 的 Python 绑定（实验性）
- 事件处理器封装
- 示例脚本

---

## 11. 构建系统

### CMake（推荐）

```bash
mkdir build && cd build
cmake ..
make
```

### Makefile

```bash
make            # 构建库
make examples   # 构建示例
make install    # 安装
make WITH_MBEDTLS=1  # 启用 TLS
```

### ARM 交叉编译

```bash
# 使用 CMake 工具链
cmake -DCMAKE_TOOLCHAIN_FILE=mingw-w64-x86_64.cmake ..
# 或设置交叉编译器
export CC=arm-linux-gnueabihf-gcc
cmake -DCMAKE_C_COMPILER=$CC ..
```

### 栈配置

配置文件: [config/stack_config.h](file:///workspace/config/stack_config.h)

关键编译开关：
- `CONFIG_MMS_SUPPORT` — MMS 协议支持
- `CONFIG_GOOSE_SUPPORT` — GOOSE 支持
- `CONFIG_SAMPLED_VALUES_SUPPORT` — SV 支持
- `CONFIG_INCLUDE_PLATFORM_SPECIFIC_HEADERS` — 平台特定头文件
- `CONFIG_MAXIMUM_TCP_CONNECTIONS` — 最大 TCP 连接数

---

## 12. 示例索引

### 服务器示例

| 示例 | 说明 | 关键文件 |
|------|------|---------|
| server_example_simple | 最简单的 IED 服务器 | [server_example_simple/](file:///workspace/examples/server_example_simple/) |
| server_example_basic_io | 基本 IO 服务器 | [server_example_basic_io/](file:///workspace/examples/server_example_basic_io/) |
| server_example_control | 控制对象示例 | [server_example_control/](file:///workspace/examples/server_example_control/) |
| server_example_goose | GOOSE 发布服务器 | [server_example_goose/](file:///workspace/examples/server_example_goose/) |
| server_example_dynamic | 动态模型构建 | [server_example_dynamic/](file:///workspace/examples/server_example_dynamic/) |
| server_example_config_file | 配置文件加载 | [server_example_config_file/](file:///workspace/examples/server_example_config_file/) |
| server_example_setting_groups | 设置组 | [server_example_setting_groups/](file:///workspace/examples/server_example_setting_groups/) |
| server_example_logging | 日志服务 | [server_example_logging/](file:///workspace/examples/server_example_logging/) |
| server_example_files | 文件传输服务 | [server_example_files/](file:///workspace/examples/server_example_files/) |
| server_example_password_auth | 密码认证 | [server_example_password_auth/](file:///workspace/examples/server_example_password_auth/) |
| server_example_access_control | 访问控制 | [server_example_access_control/](file:///workspace/examples/server_example_access_control/) |
| server_example_threadless | 无线程模式 | [server_example_threadless/](file:///workspace/examples/server_example_threadless/) |
| server_example_deadband | 死区控制 | [server_example_deadband/](file:///workspace/examples/server_example_deadband/) |
| server_example_substitution | 值替代 | [server_example_substitution/](file:///workspace/examples/server_example_substitution/) |
| server_example_complex_array | 复杂数组 | [server_example_complex_array/](file:///workspace/examples/server_example_complex_array/) |
| server_example_service_tracking | 服务跟踪 | [server_example_service_tracking/](file:///workspace/examples/server_example_service_tracking/) |
| server_example_61400_25 | 风电模型 | [server_example_61400_25/](file:///workspace/examples/server_example_61400_25/) |

### 客户端示例

| 示例 | 说明 |
|------|------|
| iec61850_client_example1 | 基本连接与数据读取 |
| iec61850_client_example2 | 数据写入 |
| iec61850_client_example4 | 报告订阅 |
| iec61850_client_example5 | 控制操作 |
| iec61850_client_example_control | 控制对象客户端 |
| iec61850_client_example_reporting | 报告处理 |
| iec61850_client_example_async | 异步操作 |
| iec61850_client_example_no_thread | 无线程客户端 |
| iec61850_client_example_files | 文件传输 |
| iec61850_client_example_log | 日志查询 |
| iec61850_client_example_array | 数组处理 |
| iec61850_client_example_ClientGooseControl | GOOSE 控制 |
| iec61850_sv_client_example | SV 客户端 |

### GOOSE / SV / R-GOOSE 示例

| 示例 | 说明 |
|------|------|
| goose_publisher | GOOSE 发布 |
| goose_subscriber | GOOSE 订阅 |
| goose_observer | GOOSE 观察器 |
| sv_publisher | SV 发布 |
| sv_subscriber | SV 订阅 |
| iec61850_9_2_LE_example | IEC 61850-9-2 LE |
| r_goose_publisher_example | R-GOOSE 发布 |
| r_goose_receiver_example | R-GOOSE 接收 |
| rsv_publisher_example | R-SV 发布 |
| rsv_subscriber_example | R-SV 订阅 |

### TLS 示例

| 示例 | 说明 |
|------|------|
| tls_client_example | TLS 客户端 |
| tls_server_example | TLS 服务器 |

---

## 13. 配置选项

配置文件: [config/stack_config.h](file:///workspace/config/stack_config.h)

| 配置项 | 默认值 | 说明 |
|--------|--------|------|
| `CONFIG_MMS_SUPPORT` | 1 | 启用 MMS 协议 |
| `CONFIG_GOOSE_SUPPORT` | 1 | 启用 GOOSE |
| `CONFIG_SAMPLED_VALUES_SUPPORT` | 1 | 启用 SV |
| `CONFIG_RGOOSE_SUPPORT` | 0 | 启用 R-GOOSE（BETA） |
| `CONFIG_MMS_SERVER_CONFIG` | 1 | MMS 服务器配置 |
| `CONFIG_MAXIMUM_TCP_CONNECTIONS` | 5 | 最大 TCP 连接数 |
| `CONFIG_MMS_MAX_NUMBER_OF_DATA_SET_MEMBERS` | 256 | 数据集最大成员数 |
| `CONFIG_REPORTING_DEFAULT_REPORT_SIZE` | 64 | 默认报告缓冲区大小 |
| `CONFIG_GOOSE_DEFAULT_RETRANSMIT_INTERVAL` | 1000 | GOOSE 默认重传间隔(ms) |
| `CONFIG_DEFAULT_MMS_AP_TITLE` | "1,1,9999,1" | 默认 AP Title |
| `CONFIG_DEFAULT_MMS_AE_QUALIFIER` | 12 | 默认 AE Qualifier |
| `CONFIG_INCLUDE_PLATFORM_SPECIFIC_HEADERS` | 0 | 平台特定头文件 |

---

## 14. 版本历史

当前版本: **1.6.1**

### 1.6.1 主要变更
- 修复 MMS 客户端连接问题
- 改进 GOOSE 发布器稳定性
- 修复 SV 订阅器边界问题
- 增强控制对象处理

详细变更见 [CHANGELOG](file:///workspace/CHANGELOG)。

---

## 相关文档

- [学习计划](file:///workspace/docs/LEARNING_PLAN.md)
- [学习参考资料](file:///workspace/docs/LEARNING_RESOURCES.md)
- [SCL 校验器文档](file:///workspace/scl_validator/README.md)
- [项目 README](file:///workspace/README.md)
- [变更日志](file:///workspace/CHANGELOG)