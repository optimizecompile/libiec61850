# libIEC61850 学习计划

> 本学习计划基于 libIEC61850 仓库的实际代码结构，分为 6 个阶段，从入门到精通。

---

## 学习路线图

```
阶段1: 基础准备          阶段2: 运行第一个程序      阶段3: 深入服务器开发
(IEC 61850 基础)    →   (编译与示例)         →   (数据模型与控制)
                                                    ↓
阶段6: 高级主题          阶段5: GOOSE/SV 实时通信   阶段4: 客户端开发
(安全/移植/扩展)    ←   (二层通信)           ←   (连接/读写/报告)
```

---

## 阶段 1: 基础准备

### 目标
理解 IEC 61850 标准的核心概念，熟悉 libIEC61850 仓库结构。

### 学习内容

#### 1.1 IEC 61850 标准概述
- 了解 IEC 61850 标准的定位：电力系统自动化通信标准
- 理解核心概念：
  - **IED** (Intelligent Electronic Device) — 智能电子设备
  - **LD** (Logical Device) — 逻辑设备
  - **LN** (Logical Node) — 逻辑节点（如 LLN0, LPHD, MMXU, CSWI, PTOC）
  - **DO** (Data Object) — 数据对象（如 Pos, Health, TotW）
  - **DA** (Data Attribute) — 数据属性（如 stVal, q, t, ctlVal）
  - **FC** (Functional Constraint) — 功能约束（ST, MX, CO, CF, SP 等）
  - **CDC** (Common Data Class) — 公共数据类（SPS, DPS, SPC, DPC, MV, CMV 等）
  - **DataSet** — 数据集
  - **RCB** (Report Control Block) — 报告控制块（BRCB/URCB）
  - **GoCB** (GOOSE Control Block) — GOOSE 控制块
  - **SVCB** (SV Control Block) — SV 控制块
  - **SGCB** (Setting Group Control Block) — 设置组控制块

#### 1.2 SCL 文件格式
- 了解 SCL (Substation Configuration Description Language) 的 XML 结构
- 理解 ICD/CID/SCD/IID 文件类型的区别
- 阅读一个示例 SCL 文件：[sampleModel_with_dataset.cid](file:///workspace/examples/server_example_simple/sampleModel_with_dataset.cid)

#### 1.3 仓库结构熟悉
- 浏览 [项目 Wiki](file:///workspace/docs/WIKI.md) 的"仓库结构"章节
- 重点了解以下目录：
  - `src/iec61850/inc/` — 公共 API 头文件
  - `examples/` — 示例程序
  - `tools/model_generator/` — 模型生成工具
  - `config/stack_config.h` — 配置选项

### 实践任务
- [ ] 阅读项目 [README.md](file:///workspace/README.md)
- [ ] 浏览 [Wiki 文档](file:///workspace/docs/WIKI.md)
- [ ] 打开一个 ICD 文件，理解其 XML 结构
- [ ] 安装 Java 运行时（用于模型生成工具）

### 推荐资料
- IEC 61850-1: 基本概念与原则
- IEC 61850-6: 配置语言（SCL）
- IEC 61850-7-1: 基本信息模型
- IEC 61850-7-2: 抽象通信服务接口（ACSI）
- IEC 61850-8-1: 特定通信服务映射（SCSM）— MMS 映射

---

## 阶段 2: 运行第一个程序

### 目标
成功编译 libIEC61850 库，运行服务器和客户端示例。

### 学习内容

#### 2.1 编译库
```bash
cd /workspace
make            # 或 mkdir build && cd build && cmake ..
```

阅读构建文件：
- [CMakeLists.txt](file:///workspace/CMakeLists.txt)
- [Makefile](file:///workspace/Makefile)
- [config/stack_config.h](file:///workspace/config/stack_config.h)

#### 2.2 运行服务器示例
```bash
# 最简单的服务器
cd examples/server_example_simple
make
./server_example_simple
```

阅读代码：[server_example_simple.c](file:///workspace/examples/server_example_simple/server_example_simple.c)

理解关键步骤：
1. 使用静态模型（`static_model.h` 由 `genmodel.jar` 生成）
2. `IedServer_create()` 创建服务器
3. `IedServer_start()` 启动服务器
4. 主循环中更新数据
5. `IedServer_stop()` / `IedServer_destroy()` 清理

#### 2.3 运行客户端示例
```bash
cd examples/iec61850_client_example1
make
./client_example1 <server_ip>
```

阅读代码：[client_example1.c](file:///workspace/examples/iec61850_client_example1/client_example1.c)

#### 2.4 使用模型生成工具
```bash
# 从 SCL 生成静态 C 模型
java -jar tools/model_generator/genmodel.jar examples/server_example_simple/sampleModel_with_dataset.cid

# 查看生成的文件
cat static_model.h
cat static_model.c
```

### 实践任务
- [ ] 成功编译库和至少 3 个示例
- [ ] 同时运行服务器和客户端，完成数据读取
- [ ] 使用 genmodel.jar 从 ICD 文件生成 C 模型代码
- [ ] 修改服务器示例中的数据值，观察客户端读取结果变化
- [ ] 尝试使用 genconfig.jar 生成配置文件，并运行 server_example_config_file

### 关键代码阅读
| 文件 | 重点 |
|------|------|
| [ied_server.c](file:///workspace/src/iec61850/server/impl/ied_server.c) | 服务器生命周期管理 |
| [iec61850_server.h](file:///workspace/src/iec61850/inc/iec61850_server.h) | 服务器 API 定义 |
| [iec61850_model.h](file:///workspace/src/iec61850/inc/iec61850_model.h) | 数据模型结构体 |
| [iec61850_common.h](file:///workspace/src/iec61850/inc/iec61850_common.h) | 公共类型定义 |

---

## 阶段 3: 深入服务器开发

### 目标
掌握 IEC 61850 服务器端开发的核心技能：数据模型、控制对象、报告、设置组、访问控制。

### 学习内容

#### 3.1 数据模型深入

**三种模型构建方式对比**：

| 方式 | 示例 | 适用场景 |
|------|------|---------|
| 静态模型 | [server_example_simple](file:///workspace/examples/server_example_simple/) | 型号固定的设备 |
| 动态模型 | [server_example_dynamic](file:///workspace/examples/server_example_dynamic/) | 运行时可变的模型 |
| 配置文件 | [server_example_config_file](file:///workspace/examples/server_example_config_file/) | 需要灵活配置的设备 |

**学习动态模型 API**：
- 阅读 [iec61850_dynamic_model.h](file:///workspace/src/iec61850/inc/iec61850_dynamic_model.h)
- 阅读 [dynamic_model.c](file:///workspace/src/iec61850/server/model/dynamic_model.c)
- 运行 [server_example_dynamic](file:///workspace/examples/server_example_dynamic/)

**CDC 辅助函数**：
- 阅读 [iec61850_cdc.h](file:///workspace/src/iec61850/inc/iec61850_cdc.h)
- 理解 CDC_SPS_create, CDC_DPS_create, CDC_SPC_create, CDC_MV_create 等
- 阅读 [cdc.c](file:///workspace/src/iec61850/server/model/cdc.c)

#### 3.2 控制对象

- 阅读 [server_example_control](file:///workspace/examples/server_example_control/)
- 理解控制模型：direct/sbo, normal/enhanced security
- 学习控制回调：ControlHandler, PerformCheckHandler, WaitForExecutionHandler
- 理解 ControlAction API

关键代码：
- [control.c](file:///workspace/src/iec61850/server/mms_mapping/control.c) — 控制对象映射实现
- [iec61850_server.h](file:///workspace/src/iec61850/inc/iec61850_server.h) — ControlHandler 等回调定义

#### 3.3 报告服务

- 阅读 [server_example_simple](file:///workspace/examples/server_example_simple/) 中的报告配置
- 理解 BRCB (Buffered RCB) vs URCB (Unbuffered RCB)
- 理解触发选项：数据变化、质量变化、数据更新、完整性周期
- 理解报告选项字段

关键代码：
- [reporting.c](file:///workspace/src/iec61850/server/mms_mapping/reporting.c) — 报告映射实现

#### 3.4 设置组

- 阅读 [server_example_setting_groups](file:///workspace/examples/server_example_setting_groups/)
- 理解 SGCB (Setting Group Control Block)
- 学习设置组切换回调

#### 3.5 访问控制

- 阅读 [server_example_access_control](file:///workspace/examples/server_example_access_control/)
- 理解 WriteAccessHandler 回调
- 理解访问策略设置
- 阅读 [server_example_password_auth](file:///workspace/examples/server_example_password_auth/) — 认证

#### 3.6 无线程模式

- 阅读 [server_example_threadless](file:///workspace/examples/server_example_threadless/)
- 理解 startThreadless / waitReady / processIncomingData / performPeriodicTasks
- 适用场景：实时系统、已有事件循环的系统

#### 3.7 日志服务

- 阅读 [server_example_logging](file:///workspace/examples/server_example_logging/)
- 理解 LCB (Log Control Block)
- 了解日志存储驱动（sqlite 可选）

#### 3.8 文件传输服务

- 阅读 [server_example_files](file:///workspace/examples/server_example_files/)
- 理解 MMS 文件服务
- 理解文件存储基路径配置

### 实践任务
- [ ] 使用动态 API 构建一个包含 3 个 LN 的 IedModel
- [ ] 实现一个支持控制操作的服务器（SPC 类型的开关控制）
- [ ] 配置 BRCB 报告，客户端订阅后能收到数据变化通知
- [ ] 实现设置组切换功能
- [ ] 实现 WriteAccessHandler，拦截并验证写操作
- [ ] 将服务器改为无线程模式运行

---

## 阶段 4: 客户端开发

### 目标
掌握 IEC 61850 客户端开发：连接、数据读写、报告订阅、控制操作、模型发现。

### 学习内容

#### 4.1 连接管理
- 阅读 [client_example1.c](file:///workspace/examples/iec61850_client_example1/client_example1.c)
- 理解连接状态机：CLOSED → CONNECTING → CONNECTED → CLOSING
- 理解连接超时、异步连接

#### 4.2 数据读写
- 阅读 [client_example2.c](file:///workspace/examples/iec61850_client_example2/client_example2.c)
- 学习同步/异步读写 API
- 理解对象引用格式：`iedLD/LN.DO.DA` + FC

#### 4.3 报告订阅
- 阅读 [client_example_reporting](file:///workspace/examples/iec61850_client_example_reporting/)
- 理解 RCB 获取与设置流程
- 学习报告处理回调
- 理解 ReasonForInclusion

#### 4.4 控制操作
- 阅读 [client_example_control](file:///workspace/examples/iec61850_client_example_control/)
- 理解 ControlObjectClient API
- 学习直接操作、SBO 操作、SBOw 操作

#### 4.5 异步客户端
- 阅读 [client_example_async](file:///workspace/examples/iec61850_client_example_async/)
- 理解异步 API 与 tick 机制
- 学习异步回调模式

#### 4.6 模型发现
- 理解 ACSI 类（ACSIClass 枚举）
- 学习动态发现服务器模型
- 阅读 [client_example5.c](file:///workspace/examples/iec61850_client_example5/client_example5.c)

#### 4.7 数据集操作
- 理解动态数据集创建、删除、读取
- 理解 FCDA 引用格式

#### 4.8 文件传输
- 阅读 [iec61850_client_example_files](file:///workspace/examples/iec61850_client_example_files/)
- 学习文件上传、下载、删除

### 实践任务
- [ ] 编写客户端程序连接服务器，读取所有 LN 的数据
- [ ] 实现报告订阅，打印收到的报告内容
- [ ] 实现控制操作（选择→操作→确认流程）
- [ ] 使用模型发现 API 动态获取服务器完整模型
- [ ] 创建动态数据集并订阅
- [ ] 实现文件上传和下载
- [ ] 将客户端改为异步模式

---

## 阶段 5: GOOSE / SV 实时通信

### 目标
掌握 GOOSE 和 Sampled Values 的发布与订阅。

### 学习内容

#### 5.1 GOOSE 发布
- 阅读 [goose_publisher_example.c](file:///workspace/examples/goose_publisher/goose_publisher_example.c)
- 理解 CommParameters（VLAN/AppID/MAC 地址）
- 理解 GOOSE 报文结构：stNum/sqNum/timeAllowedToLive
- 理解重传机制

#### 5.2 GOOSE 订阅
- 阅读 [goose_subscriber_example.c](file:///workspace/examples/goose_subscriber/goose_subscriber_example.c)
- 理解 GooseReceiver / GooseSubscriber 关系
- 理解 GOOSE 订阅验证（confRev, ndsCom, 时间有效性）
- 理解无线程模式接收

#### 5.3 服务器集成 GOOSE
- 阅读 [server_example_goose](file:///workspace/examples/server_example_goose/)
- 理解 IedServer_enableGoosePublishing
- 理解 GoCB 通过 MMS 配置与二层发送的关系

#### 5.4 SV 发布
- 阅读 [sv_publisher_example.c](file:///workspace/examples/sv_publisher/sv_publisher_example.c)
- 理解 ASDU 结构
- 理解采样计数器（SmpCnt）和采样率
- 理解 IEC 61850-9-2 LE

#### 5.5 SV 订阅
- 阅读 [sv_subscriber_example.c](file:///workspace/examples/sv_subscriber/sv_subscriber_example.c)
- 理解 SVReceiver / SVSubscriber
- 理解 SV 数据类型读取（INT8/16/32, FLOAT 等）

#### 5.6 IEC 61850-9-2 LE 示例
- 阅读 [iec61850_9_2_LE_example/](file:///workspace/examples/iec61850_9_2_LE_example/)
- 理解 9-2 LE 规范的采样率配置

#### 5.7 R-GOOSE / R-SV（可选）
- 阅读 [r_goose_publisher_example.c](file:///workspace/examples/r_goose_publisher_example/r_goose_publisher_example.c)
- 理解 R-Session 概念
- 理解 IP 多播传输
- 理解安全机制（AES-GCM 加密、HMAC 签名）

### 实践任务
- [ ] 运行 GOOSE 发布器和订阅器，验证数据接收
- [ ] 在服务器示例中集成 GOOSE 发布
- [ ] 运行 SV 发布器和订阅器
- [ ] 使用 SV 发布器发送模拟采样数据
- [ ] 尝试 R-GOOSE 发布与接收（如环境允许）
- [ ] 使用 Wireshark 抓包分析 GOOSE/SV 报文结构

### 关键代码阅读
| 文件 | 重点 |
|------|------|
| [goose_publisher.c](file:///workspace/src/goose/goose_publisher.c) | GOOSE 编码与发送 |
| [goose_subscriber.c](file:///workspace/src/goose/goose_subscriber.c) | GOOSE 解码与验证 |
| [goose_receiver.c](file:///workspace/src/goose/goose_receiver.c) | 以太网接收管理 |
| [sv_publisher.c](file:///workspace/src/sampled_values/sv_publisher.c) | SV 编码与发送 |
| [sv_subscriber.c](file:///workspace/src/sampled_values/sv_subscriber.c) | SV 解码 |
| [r_session.c](file:///workspace/src/r_session/r_session.c) | R-Session 实现 |

---

## 阶段 6: 高级主题

### 目标
掌握 TLS 安全通信、平台移植、性能优化、协议扩展。

### 学习内容

#### 6.1 TLS 安全通信
- 阅读 [tls_server_example](file:///workspace/examples/tls_server_example/) 和 [tls_client_example](file:///workspace/examples/tls_client_example/)
- 理解 TLSConfiguration 配置
- 理解证书管理
- 了解 mbedtls 集成

关键代码：
- [hal/tls/](file:///workspace/hal/tls/) — TLS 实现
- [third_party/mbedtls/](file:///workspace/third_party/mbedtls/) — mbedtls 依赖

#### 6.2 平台移植
- 阅读 [hal/inc/](file:///workspace/hal/inc/) 下所有头文件
- 理解 HAL 接口设计
- 学习移植到新平台的方法：
  1. 实现 `hal_socket.h` — Socket 通信
  2. 实现 `hal_thread.h` — 线程与信号量
  3. 实现 `hal_ethernet.h` — 以太网（GOOSE/SV）
  4. 实现 `hal_filesystem.h` — 文件系统
  5. 实现 `hal_time.h` — 时间
  6. 实现 `hal_memory.h` — 内存管理
- 参考 Linux 实现：[hal/socket/linux/](file:///workspace/hal/socket/linux/), [hal/thread/linux/](file:///workspace/hal/thread/linux/)

#### 6.3 配置与裁剪
- 阅读 [config/stack_config.h](file:///workspace/config/stack_config.h)
- 理解编译开关的作用
- 学习根据资源限制裁剪功能

#### 6.4 SNTP 时间同步
- 阅读 [sntp_example](file:///workspace/examples/sntp_example/)
- 理解时间同步在 GOOSE/SV 中的重要性

#### 6.5 MMS 协议栈深入
- 阅读 [src/mms/](file:///workspace/src/mms/) 目录结构
- 理解 ISO/OSI 上层协议栈（ACSE/COTP/Session/Presentation）
- 理解 ASN.1 BER 编解码
- 阅读 [mms_value.c](file:///workspace/src/mms/iso_mms/common/mms_value.c) — MmsValue 类型系统

#### 6.6 SCL 文件校验
- 阅读 [scl_validator/](file:///workspace/scl_validator/) 目录
- 理解 SCL 文件合法性检查的实现
- 学习如何扩展校验规则

#### 6.7 性能优化
- 理解数据模型锁（IedServer_lockDataModel）
- 理解报告缓冲区大小配置
- 理解最大连接数配置
- 理解 GOOSE 重传间隔优化

#### 6.8 Python / C# 绑定
- 阅读 [pyiec61850/](file:///workspace/pyiec61850/) — Python 绑定
- 阅读 [dotnet/](file:///workspace/dotnet/) — C# 绑定
- 理解 SWIG 接口文件

### 实践任务
- [ ] 配置 TLS 服务器和客户端，使用证书进行安全通信
- [ ] 将 HAL 移植到一个新的目标平台（如 FreeRTOS）
- [ ] 裁剪栈配置，构建一个仅支持 GOOSE 的最小库
- [ ] 使用 SNTP 客户端同步时间
- [ ] 使用 Wireshark 分析完整的 MMS 通信流程
- [ ] 使用 SCL 校验器验证一个真实的 CID 文件
- [ ] 编写 Python 脚本使用 pyiec61850 连接服务器
- [ ] 分析 MmsValue 的内存布局，优化内存使用

---

## 学习时间安排建议

| 阶段 | 内容 | 建议周期 | 前置条件 |
|------|------|---------|---------|
| 1 | 基础准备 | 1-2 周 | C 语言基础、电力系统基础 |
| 2 | 运行第一个程序 | 1 周 | 阶段 1 完成 |
| 3 | 深入服务器开发 | 3-4 周 | 阶段 2 完成 |
| 4 | 客户端开发 | 2-3 周 | 阶段 3 完成 |
| 5 | GOOSE/SV 实时通信 | 2-3 周 | 阶段 3 完成 |
| 6 | 高级主题 | 3-4 周 | 阶段 3-5 完成 |

> 注：以上周期为全职学习建议。实际时间取决于个人背景和投入程度。

---

## 关键代码阅读顺序

### 入门级
1. [iec61850_common.h](file:///workspace/src/iec61850/inc/iec61850_common.h) — 理解基本类型
2. [iec61850_model.h](file:///workspace/src/iec61850/inc/iec61850_model.h) — 理解数据模型
3. [server_example_simple.c](file:///workspace/examples/server_example_simple/server_example_simple.c) — 第一个服务器
4. [client_example1.c](file:///workspace/examples/iec61850_client_example1/client_example1.c) — 第一个客户端

### 进阶级
5. [iec61850_server.h](file:///workspace/src/iec61850/inc/iec61850_server.h) — 完整服务器 API
6. [iec61850_client.h](file:///workspace/src/iec61850/inc/iec61850_client.h) — 完整客户端 API
7. [ied_server.c](file:///workspace/src/iec61850/server/impl/ied_server.c) — 服务器实现
8. [mms_mapping.c](file:///workspace/src/iec61850/server/mms_mapping/mms_mapping.c) — 映射层
9. [control.c](file:///workspace/src/iec61850/server/mms_mapping/control.c) — 控制实现
10. [reporting.c](file:///workspace/src/iec61850/server/mms_mapping/reporting.c) — 报告实现

### 高级
11. [goose_publisher.c](file:///workspace/src/goose/goose_publisher.c) — GOOSE 发布
12. [goose_subscriber.c](file:///workspace/src/goose/goose_subscriber.c) — GOOSE 订阅
13. [sv_publisher.c](file:///workspace/src/sampled_values/sv_publisher.c) — SV 发布
14. [mms_server.c](file:///workspace/src/mms/iso_mms/server/mms_server.c) — MMS 服务器
15. [mms_value.c](file:///workspace/src/mms/iso_mms/common/mms_value.c) — MmsValue 类型系统
16. [hal_socket.h](file:///workspace/hal/inc/hal_socket.h) + Linux 实现 — HAL 层
17. [r_session.c](file:///workspace/src/r_session/r_session.c) — R-Session
18. [config_file_parser.c](file:///workspace/src/iec61850/server/model/config_file_parser.c) — 配置文件解析

---

## 常见问题

### Q: 如何选择静态模型 vs 动态模型 vs 配置文件？
- **静态模型**：设备型号固定，ROM/Flash 受限的嵌入式设备
- **动态模型**：运行时需要灵活调整模型的场景
- **配置文件**：需要在不重新编译固件的情况下更新模型

### Q: GOOSE 和 MMS 有什么区别？
- **MMS** 走 TCP/IP，用于客户端-服务器模式的数据访问、报告、控制
- **GOOSE** 走二层以太网，用于设备间的高速状态传输（<4ms），无需 TCP 连接

### Q: 什么是 R-GOOSE？
- R-GOOSE (IEC 61850 90-5) 是 GOOSE 的 IP 多播扩展，通过 UDP 传输，支持加密和签名

### Q: 如何在嵌入式平台上减少内存占用？
- 通过 `config/stack_config.h` 裁剪不需要的功能
- 减少最大连接数、数据集大小、报告缓冲区
- 使用静态模型代替动态模型
- 使用无线程模式减少线程栈开销

### Q: 如何调试 MMS 通信问题？
- 使用 Wireshark 的 IEC 61850/MMS dissector 抓包分析
- 设置 `MmsConnection_setRawMessageHandler` 回调查看原始 PDU
- 检查 ISO 连接参数（AP Title, AE Qualifier, Selectors）

---

## 相关文档

- [项目 Wiki](file:///workspace/docs/WIKI.md)
- [学习参考资料](file:///workspace/docs/LEARNING_RESOURCES.md)
- [项目 README](file:///workspace/README.md)
- [SCL 校验器文档](file:///workspace/scl_validator/README.md)