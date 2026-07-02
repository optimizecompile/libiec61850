# libIEC61850 学习参考资料

> 本文档汇总了学习 IEC 61850 标准和 libIEC61850 开源库所需的各类参考资料。

---

## 目录

- [1. IEC 61850 国际标准](#1-iec-61850-国际标准)
- [2. 核心技术文档](#2-核心技术文档)
- [3. 书籍推荐](#3-书籍推荐)
- [4. 在线课程与教程](#4-在线课程与教程)
- [5. 开源项目与工具](#5-开源项目与工具)
- [6. 电力系统通信背景知识](#6-电力系统通信背景知识)
- [7. 网络协议背景知识](#7-网络协议背景知识)
- [8. C 语言与嵌入式开发](#8-c-语言与嵌入式开发)
- [9. 实用工具](#9-实用工具)
- [10. 社区与论坛](#10-社区与论坛)

---

## 1. IEC 61850 国际标准

### 核心标准（必读）

| 标准编号 | 标题 | 重要性 | 说明 |
|---------|------|--------|------|
| IEC 61850-1 | Introduction and overview | ★★★★★ | 标准概述与基本概念 |
| IEC 61850-2 | Glossary | ★★★☆☆ | 术语表 |
| IEC 61850-3 | General requirements | ★★★☆☆ | 一般要求（可靠性、环境条件） |
| IEC 61850-4 | System and project management | ★★☆☆☆ | 系统与项目管理 |
| IEC 61850-5 | Communication requirements for functions and device models | ★★★★☆ | 功能与设备模型的通信需求 |
| IEC 61850-6 | Configuration language for communication (SCL) | ★★★★★ | SCL 配置语言定义 |
| IEC 61850-7-1 | Basic information and communication structure | ★★★★★ | 基本信息与通信结构 |
| IEC 61850-7-2 | Abstract communication service interface (ACSI) | ★★★★★ | 抽象通信服务接口 |
| IEC 61850-7-3 | Common data classes (CDC) | ★★★★☆ | 公共数据类定义 |
| IEC 61850-7-4 | Compatible logical node classes and data classes | ★★★★☆ | 兼容逻辑节点类与数据类 |
| IEC 61850-8-1 | Specific communication service mapping - MMS | ★★★★★ | ACSI 到 MMS 的映射 |
| IEC 61850-9-2 | Specific communication service mapping - SV | ★★★★☆ | 采样值传输映射 |
| IEC 61850-9-2 LE | Lite Edition of IEC 61850-9-2 | ★★★☆☆ | 9-2 简化版（实际工程常用） |
| IEC 61850-10 | Conformance testing | ★★☆☆☆ | 一致性测试 |

### 扩展标准

| 标准编号 | 标题 | 说明 |
|---------|------|------|
| IEC 61850-7-410 | Hydroelectric power plants | 水力发电厂模型 |
| IEC 61850-7-420 | Distributed energy resources (DER) | 分布式能源 |
| IEC 61850-7-500 | Usage information | 使用信息 |
| IEC 61850-7-510 | Hydroelectric guidelines | 水电指导 |
| IEC 61850-90-1 | Using GOOSE for station-to-station | 站间 GOOSE 通信 |
| IEC 61850-90-5 | Using IEC 61850 for synchrophasors | R-GOOSE/R-SV 传输 |
| IEC 61850-90-6 | Using IEC 61850 for substation automation | 变电站自动化应用 |
| IEC 61850-80-1 | Mapping to IEC 60870-5-103 | 与传统协议映射 |
| IEC 61400-25 | Wind power communication | 风电通信（基于 IEC 61850） |

### 标准获取方式
- IEC 官方网站购买：https://webstore.iec.ch/
- 部分标准草案可通过技术论坛获取预览版

---

## 2. 核心技术文档

### libIEC61850 官方文档

| 文档 | 位置 | 说明 |
|------|------|------|
| 项目 README | [README.md](file:///workspace/README.md) | 项目概览、构建说明 |
| 项目 Wiki | [WIKI.md](file:///workspace/docs/WIKI.md) | 完整架构文档 |
| 学习计划 | [LEARNING_PLAN.md](file:///workspace/docs/LEARNING_PLAN.md) | 分阶段学习路线 |
| 变更日志 | [CHANGELOG](file:///workspace/CHANGELOG) | 版本变更记录 |
| Doxygen 文档 | [src/doxygen/](file:///workspace/src/doxygen/) | API 参考文档配置 |
| SCL 校验器文档 | [scl_validator/README.md](file:///workspace/scl_validator/README.md) | SCL 校验工具说明 |
| Python 绑定文档 | [pyiec61850/README.md](file:///workspace/pyiec61850/README.md) | Python 绑定说明 |

### libIEC61850 官方网站
- 主页：https://libiec61850.com/
- GitHub：https://github.com/mz-automation/libiec61850
- 文档：https://libiec61850.com/documentation/

### 关键 API 头文件（按学习顺序）

| 头文件 | 说明 | 行数 |
|--------|------|------|
| [iec61850_common.h](file:///workspace/src/iec61850/inc/iec61850_common.h) | 公共类型定义 | ~500 |
| [iec61850_model.h](file:///workspace/src/iec61850/inc/iec61850_model.h) | 数据模型结构 | ~600 |
| [iec61850_server.h](file:///workspace/src/iec61850/inc/iec61850_server.h) | 服务器端 API | ~2100 |
| [iec61850_client.h](file:///workspace/src/iec61850/inc/iec61850_client.h) | 客户端 API | ~3100 |
| [iec61850_dynamic_model.h](file:///workspace/src/iec61850/inc/iec61850_dynamic_model.h) | 动态模型 API | ~400 |
| [iec61850_cdc.h](file:///workspace/src/iec61850/inc/iec61850_cdc.h) | CDC 辅助函数 | ~300 |
| [iec61850_config_file_parser.h](file:///workspace/src/iec61850/inc/iec61850_config_file_parser.h) | 配置文件解析 | ~70 |
| [mms_value.h](file:///workspace/src/mms/inc/mms_value.h) | MmsValue 类型系统 | ~600 |
| [mms_client_connection.h](file:///workspace/src/mms/inc/mms_client_connection.h) | MMS 客户端 API | ~500 |
| [mms_server.h](file:///workspace/src/mms/inc/mms_server.h) | MMS 服务器 API | ~400 |
| [iso_connection_parameters.h](file:///workspace/src/mms/inc/iso_connection_parameters.h) | ISO 连接参数 | ~300 |
| [goose_publisher.h](file:///workspace/src/goose/goose_publisher.h) | GOOSE 发布器 | ~200 |
| [goose_subscriber.h](file:///workspace/src/goose/goose_subscriber.h) | GOOSE 订阅器 | ~200 |
| [sv_publisher.h](file:///workspace/src/sampled_values/sv_publisher.h) | SV 发布器 | ~200 |
| [sv_subscriber.h](file:///workspace/src/sampled_values/sv_subscriber.h) | SV 订阅器 | ~200 |

---

## 3. 书籍推荐

### IEC 61850 专题

| 书名 | 作者 | 说明 | 推荐度 |
|------|------|------|--------|
| **IEC 61850 Demystified** | Herb Falk | IEC 61850 入门经典，通俗易懂 | ★★★★★ |
| **IEC 61850: Principles and Applications** | Sunil Kumar | 系统讲解原理与应用 | ★★★★☆ |
| **Understanding IEC 61850** | Pascal Adant | 深入理解标准细节 | ★★★★☆ |
| **Substation Automation Systems** | Evelio Padilla | 变电站自动化系统设计 | ★★★☆☆ |
| **Smart Grid Communications** | Stephen F. Bush | 智能电网通信（含 IEC 61850 章节） | ★★★☆☆ |
| **Power System SCADA and Smart Grids** | John D. McDonald | 电力系统 SCADA 与智能电网 | ★★★☆☆ |

### 电力系统通信

| 书名 | 作者 | 说明 |
|------|------|------|
| Communication Systems for Electrical Grids | L. Peretto | 电网通信系统 |
| Industrial Communication Systems | G. K. Saha | 工业通信系统（含 IEC 61850） |
| Power Line Communications | H. Ferreira | 电力线通信 |

### 网络协议

| 书名 | 作者 | 说明 |
|------|------|------|
| TCP/IP Illustrated, Volume 1 | W. Richard Stevens | TCP/IP 协议详解 |
| ASN.1 - Communication Between Heterogeneous Systems | Olivier Dubuisson | ASN.1 编解码详解 |
| ASN.1 Complete | John Larmouth | ASN.1 完整指南 |

### 嵌入式 C 编程

| 书名 | 作者 | 说明 |
|------|------|------|
| Effective C | Robert C. Seacord | 现代 C 编程最佳实践 |
| Modern C | Jens Gustedt | C99/C11 语言指南 |
| Expert C Programming: Deep C Secrets | Peter van der Linden | C 语言深入理解 |
| Making Embedded Systems | Elecia White | 嵌入式系统设计 |

---

## 4. 在线课程与教程

### IEC 61850 在线资源

| 资源 | 说明 | 链接 |
|------|------|------|
| libIEC61850 官方文档 | API 参考与教程 | https://libiec61850.com/documentation/ |
| MZ Automation 技术博客 | libIEC61850 使用技巧 | https://libiec61850.com/blog/ |
| IEC 61850 Wiki | 社区维护的知识库 | https://en.wikipedia.org/wiki/IEC_61850 |
| OMICRON IEC 61850 培训 | 在线培训课程 | https://www.omicronenergy.com/ |
| NettedAutomation 教程 | IEC 61850 技术教程 | http://www.nettedautomation.com/ |
| SISCO 培训资源 | MMS/IEC 61850 培训 | https://www.sisconet.com/ |

### 协议分析教程

| 资源 | 说明 | 链接 |
|------|------|------|
| Wireshark IEC 61850 指南 | 使用 Wireshark 分析 IEC 61850 | https://wiki.wireshark.org/IEC61850 |
| Wireshark MMS 指南 | 使用 Wireshark 分析 MMS | https://wiki.wireshark.org/MMS |
| GOOSE 报文分析 | GOOSE 报文结构与抓包分析 | 各技术博客 |

### C 语言与嵌入式开发

| 资源 | 说明 | 链接 |
|------|------|------|
| learn-c.org | 交互式 C 语言教程 | https://www.learn-c.org/ |
| C Programming | cprogramming.com 教程 | https://www.cprogramming.com/ |
| GCC 文档 | GCC 编译器使用 | https://gcc.gnu.org/onlinedocs/ |
| CMake 教程 | CMake 构建系统 | https://cmake.org/cmake/help/latest/guide/tutorial/ |
| Embedded C 编程标准 | MISRA C 指南 | https://www.misra.org.uk/ |

---

## 5. 开源项目与工具

### IEC 61850 相关开源项目

| 项目 | 说明 | 链接 |
|------|------|------|
| **libIEC61850** | 本项目 — C 语言 IEC 61850 实现 | https://github.com/mz-automation/libiec61850 |
| **libIEC61850 .NET** | C#/.NET 绑定 | 包含在本项目中 |
| **OpenIEC61850** | 另一个开源 IEC 61850 实现（C++） | https://github.com/automarks/openIEC61850 |
| **IEC61850 Open Library** | Python IEC 61850 库 | https://github.com/ricardobastos/IEC61850 |
| **J61850** | Java IEC 61850 库 | https://github.com/IEC61850/J61850 |
| **OpenMUC** | Java 能源管理系统（含 IEC 61850） | https://www.openmuc.org/ |

### 协议工具

| 工具 | 说明 | 链接 |
|------|------|------|
| **Wireshark** | 网络协议分析器（支持 IEC 61850/MMS/GOOSE/SV） | https://www.wireshark.org/ |
| **IEDScout** | IEC 61850 客户端测试工具（OMICRON） | https://www.omicronenergy.com/ |
| **Scalibur** | SCL 文件管理工具 | 商业软件 |
| **StationScout** | SCL 配置验证工具 | https://www.omicronenergy.com/ |
| **mz-automation modelviewer** | 本项目附带的 SCL 查看器 | [tools/model_generator/](file:///workspace/tools/model_generator/) |

### XML 工具

| 工具 | 说明 | 链接 |
|------|------|------|
| **Mini-XML (mxml)** | 轻量级 XML 解析库（SCL 校验器使用） | https://github.com/michaelrsweet/mxml |
| **libxml2** | 功能完整的 XML 库 | http://xmlsoft.org/ |
| **xmllint** | XML 验证命令行工具 | http://xmlsoft.org/xmllint.html |

### 构建与测试工具

| 工具 | 说明 | 链接 |
|------|------|------|
| **CMake** | 跨平台构建系统 | https://cmake.org/ |
| **GCC / Clang** | C 编译器 | https://gcc.gnu.org/ |
| **Valgrind** | 内存检测工具 | https://valgrind.org/ |
| **AddressSanitizer** | 内存错误检测（GCC/Clang 内置） | https://clang.llvm.org/docs/AddressSanitizer.html |
| **OSS-Fuzz** | 模糊测试框架（本项目已集成） | https://google.github.io/oss-fuzz/ |

### TLS 库

| 库 | 说明 | 链接 |
|---|------|------|
| **mbedtls** | 轻量级 TLS 库（本项目使用） | https://www.trustedfirmware.org/projects/mbed-tls/ |
| **OpenSSL** | 主流 TLS 库 | https://www.openssl.org/ |

---

## 6. 电力系统通信背景知识

### 传统电力通信协议（对比理解 IEC 61850）

| 协议 | 说明 | 与 IEC 61850 的关系 |
|------|------|-------------------|
| IEC 60870-5-101/103/104 | 传统远动协议 | IEC 61850 是替代者 |
| DNP3 | 北美电力通信协议 | 功能类似的竞品 |
| Modbus | 工业通信协议 | 更简单，IEC 61850 功能更强 |
| IEC 60870-6 (ICCP/TASE.2) | 控制中心间通信 | 与 IEC 61850 互补 |

### 电力系统自动化层级

```
控制中心 (Control Center)
    ↕ ICCP / IEC 61850 (站间)
变电站 (Substation)
    ↕ IEC 61850 (站内 MMS + GOOSE)
间隔层 (Bay Level)
    ↕ IEC 61850 SV / 传统模拟量
过程层 (Process Level)
```

### 关键概念

- **变电站自动化** (Substation Automation)：利用计算机和通信技术实现变电站监控与保护
- **SCADA** (Supervisory Control and Data Acquisition)：监控与数据采集系统
- **RTU** (Remote Terminal Unit)：远程终端装置
- **IED** (Intelligent Electronic Device)：智能电子设备
- **GOOSE** 用于站内高速横向通信（如保护跳闸信号）
- **SV** 用于电流/电压采样值传输（数字化变电站）
- **MMS** 用于站控层与间隔层之间的纵向通信

### 推荐阅读
- IEEE PES Substations Committee 教育资源
- CIGRE 技术报告（关于 IEC 61850 工程实践）
- 中国电力行业标准 DL/T 860（等同采用 IEC 61850）

---

## 7. 网络协议背景知识

### ISO/OSI 七层模型与 IEC 61850 的关系

```
OSI 层     | IEC 61850 映射
----------|---------------------------
应用层     | MMS (IEC 61850-8-1)
表示层     | ISO 8823 (Presentation)
会话层     | ISO 8327 (Session)
传输层     | ISO 8073 (COTP/RFC1006)
网络层     | IP
数据链路层 | Ethernet / GOOSE / SV
物理层     | Ethernet PHY
```

### MMS (Manufacturing Message Specification)

- 标准：ISO/IEC 9506
- 用于制造业和电力系统的报文规范
- libIEC61850 包含完整的 MMS 协议栈实现
- MMS PDU 使用 ASN.1 BER 编码

### ASN.1 BER (Basic Encoding Rules)

- 标准：ISO/IEC 8825-1
- 用于 MMS PDU 和 GOOSE 报文的编解码
- libIEC61850 使用 asn1c 工具自动生成编解码代码
- asn1c 项目：https://github.com/vlm/asn1c

### RFC 1006 (ISO Transport over TCP)

- 标准：RFC 1006
- 将 ISO 8073 (COTP) 承载在 TCP 之上
- libIEC61850 的 MMS 通信基于此

### GOOSE 以太网帧结构

```
| 目标 MAC (6B) | 源 MAC (6B) | VLAN Tag (4B) | EtherType 0x88B8 (2B) |
| GOOSE PDU (ASN.1 BER 编码) |
```

### SV 以太网帧结构

```
| 目标 MAC (6B) | 源 MAC (6B) | VLAN Tag (4B) | EtherType 0x88BA (2B) |
| SV PDU (ASN.1 BER 编码) |
```

---

## 8. C 语言与嵌入式开发

### C 语言学习资源

| 资源 | 说明 | 链接 |
|------|------|------|
| C Reference | cppreference.com C 语言参考 | https://en.cppreference.com/w/c |
| GCC Manual | GCC 编译器手册 | https://gcc.gnu.org/onlinedocs/gcc/ |
| C FAQ | C 语言常见问题 | http://c-faq.com/ |
| ISO C Standard | ISO/IEC 9899:1999 (C99) | https://www.iso.org/standard/29237.html |

### 嵌入式 Linux 开发

| 资源 | 说明 | 链接 |
|------|------|------|
| Embedded Linux Wiki | 嵌入式 Linux 知识库 | https://elinux.org/ |
| Yocto Project | 嵌入式 Linux 构建框架 | https://www.yoctoproject.org/ |
| Buildroot | 嵌入式 Linux 构建系统 | https://buildroot.org/ |
| ARM 交叉编译指南 | GCC ARM 工具链使用 | https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain |

### 交叉编译工具链

| 工具链 | 说明 | 链接 |
|--------|------|------|
| GCC ARM | Linaro ARM GCC | https://www.linaro.org/ |
| arm-linux-gnueabihf-gcc | Debian/Ubuntu ARM 交叉编译器 | apt install gcc-arm-linux-gnueabihf |
| musl-cross-make | 静态链接交叉编译 | https://github.com/richfelker/musl-cross-make |

### 多线程编程

| 资源 | 说明 |
|------|------|
| POSIX Threads Programming | POSIX 线程编程指南 |
| pthreads 入门 | https://computing.llnl.gov/tutorials/pthreads/ |
| 信号量与互斥锁 | POSIX 信号量使用 |

### 内存管理

| 资源 | 说明 |
|------|------|
| C 动态内存分配 | malloc/free 最佳实践 |
| 内存池设计 | 嵌入式系统内存池实现 |
| Valgrind 使用 | 内存泄漏检测 |

---

## 9. 实用工具

### 开发工具

| 工具 | 用途 | 推荐度 |
|------|------|--------|
| VS Code | 代码编辑器（C/C++ 插件） | ★★★★★ |
| CLion | JetBrains C/C++ IDE | ★★★★☆ |
| Eclipse CDT | C/C++ IDE（模型生成工具使用） | ★★★☆☆ |
| GDB | 调试器 | ★★★★★ |
| Valgrind | 内存检测 | ★★★★★ |
| strace/ltrace | 系统调用追踪 | ★★★★☆ |
| perf | 性能分析 | ★★★★☆ |

### 网络分析工具

| 工具 | 用途 | 说明 |
|------|------|------|
| Wireshark | 协议分析 | 支持 IEC 61850/MMS/GOOSE/SV 解析 |
| tcpdump | 命令行抓包 | 无 GUI 环境使用 |
| tcpreplay | 报文重放 | 回放抓包数据 |
| netem | 网络模拟 | 模拟延迟、丢包 |

### 版本控制

| 工具 | 说明 |
|------|------|
| Git | 版本控制 |
| GitHub | 代码托管 |
| GitKraken / Sourcetree | Git GUI 客户端 |

### 文档工具

| 工具 | 用途 |
|------|------|
| Doxygen | C 代码 API 文档生成 |
| Markdown | 文档编写 |
| PlantUML / draw.io | 架构图绘制 |
| Graphviz | 结构图生成 |

---

## 10. 社区与论坛

### 技术社区

| 社区 | 说明 | 链接 |
|------|------|------|
| libIEC61850 GitHub Issues | 问题反馈与讨论 | https://github.com/mz-automation/libiec61850/issues |
| Stack Overflow | 技术问答（搜索 IEC 61850） | https://stackoverflow.com/questions/tagged/iec61850 |
| Reddit /r/PowerSystems | 电力系统讨论 | https://www.reddit.com/r/PowerSystems/ |
| IEEE PES | 电力与能源协会 | https://www.ieee-pes.org/ |
| IEC TC 57 | IEC 61850 技术委员会 | https://www.iec.ch/dyn/www/f?p=103:7:0::::FSP_ORG_ID:1273 |

### 中国技术社区

| 社区 | 说明 |
|------|------|
| CSDN / 博客园 | 搜索 IEC 61850 相关博客 |
| 知乎 | 电力系统通信话题 |
| 电力行业论坛 | 中国电力企业联合会 |

### 邮件列表 / 讨论组

| 资源 | 说明 |
|------|------|
| libIEC61850 邮件列表 | 项目官方讨论 |
| IEC TC 57 工作组 | 标准制定讨论（需会员） |

---

## 附录：快速参考卡

### IEC 61850 对象引用格式

```
IED_NAME + LD_INST / LN_PREFIX + LN_CLASS + LN_INST . DO_NAME . DA_NAME
                    \_________________________/
                              LN Reference
\________________________________________________/
                    Object Reference
+ FC (Functional Constraint)
```

示例：`IED1LD0/LLN0.Health.stVal` + `ST`

### 常见逻辑节点类

| LN 类 | 名称 | 用途 |
|--------|------|------|
| LLN0 | Logical Node Zero | LD 级管理 |
| LPHD | Physical Device | 物理设备信息 |
| MMXU | Measurement | 测量值 |
| CSWI | Switch Control | 开关控制 |
| PTOC | Protection TOC | 过流保护 |
| PTTR | Protection TTR | 过热保护 |
| PDIF | Protection DIFF | 差动保护 |
| XCBR | Circuit Breaker | 断路器 |
| XSWI | Switch | 隔离开关 |
| MMTR | Metering | 计量 |
| GGIO | Generic I/O | 通用 IO |

### 常见 CDC (Common Data Class)

| CDC | 名称 | 典型用途 |
|-----|------|---------|
| SPS | Single Point Status | 单点状态 |
| DPS | Double Point Status | 双点状态（中间/合/分/坏） |
| INS | Integer Status | 整数状态 |
| ENS | Enumerated Status | 枚举状态 |
| MV | Measured Value | 测量值 |
| CMV | Complex Measured Value | 复数测量值 |
| SPC | Single Point Controllable | 单点可控 |
| DPC | Double Point Controllable | 双点可控 |
| INC | Integer Controllable | 整数可控 |
| APC | Analog Process Controllable | 模拟量可控 |
| SAV | Sampled Value | 采样值 |
| LPL | Name Plate | 名称板 |

### 常见 FC (Functional Constraint)

| FC | 用途 | 典型 DA |
|----|------|---------|
| ST | 状态 | stVal, q, t |
| MX | 测量 | mag, ang, q, t |
| CO | 控制 | Oper, SBOw, Cancel |
| CF | 配置 | ctlModel, sboTimeout |
| DC | 描述 | vendor, swRev, d |
| SP | 设定 | setMag |
| SG | 设置组 | 当前设置组值 |
| SE | 设置组编辑 | 编辑设置组值 |

### GOOSE EtherType 与 AppID

| 协议 | EtherType | 说明 |
|------|-----------|------|
| GOOSE | 0x88B8 | IEC 61850 GOOSE |
| GSE (Management) | 0x88B9 | IEC 61850 GSE 管理 |
| SV | 0x88BA | IEC 61850 采样值 |

### MMS 标准端口

| 端口 | 用途 |
|------|------|
| 102 | MMS over TCP (RFC 1006) 标准端口 |

---

## 相关文档

- [项目 Wiki](file:///workspace/docs/WIKI.md)
- [学习计划](file:///workspace/docs/LEARNING_PLAN.md)
- [项目 README](file:///workspace/README.md)