# SCL Validator for IEC 61850

## 简介

这是一个用 C 语言实现的 IEC 61850 SCL/CCD/CID/ICD/SCD 文件合法性验证工具，专为嵌入式 ARM Linux 设备设计。

## 功能特性

### 1. XML 结构验证
- SCL 根元素检查
- Header 元素解析与验证
- 命名空间声明检查

### 2. DataTypeTemplates 验证
- LNodeType 引用完整性检查
- DOType 引用完整性检查
- DAType 引用完整性检查
- EnumType 引用完整性检查
- 未使用类型检测（警告）

### 3. IED 模型验证
- IED 名称合法性检查（字符限制）
- AccessPoint 结构检查
- Server/LDevice/LN 层级检查
- LN0 必须存在检查
- lnType/lnClass/inst 属性检查

### 4. DOI/DAI/SDI 验证
- DAI 值类型检查：
  - BOOLEAN: true/false/1/0
  - INT8/16/32/64: 范围检查
  - INT8U/16U/24U/32U: 范围检查
  - FLOAT32/64: 可解析性检查
  - ENUMERATED: 枚举值存在性检查
  - OCTET_STRING_64: Base64 格式检查

### 5. 控制块验证
- ReportControl: 名称、DataSet 引用检查
- GSEControl: 名称、DataSet 引用检查
- SMVControl: 名称检查
- LogControl: 名称检查
- SettingGroup: 必须在 LLN0 中

### 6. 通信段验证
- SubNetwork 名称检查
- ConnectedAP IED/AP 引用检查
- P 地址值格式检查：
  - IP/SUBNET/GATEWAY: 4 字节 Hex
  - MAC-Address: 6 字节 Hex
  - VLAN-ID: 3 字符 Hex
  - VLAN-PRIORITY: 0-7
  - APPID: 2 字节 Hex
  - OSI selectors: Hex 格式

### 7. 跨引用验证
- IED 与 Communication 段关联
- DataSet 与控制块关联
- FCDA 引用路径验证

## 目录结构

```
scl_validator/
├── include/
│   ├── scl_types.h        # 类型定义
│   ├── scl_elements.h     # SCL 元素结构
│   ├── scl_validator.h    # 验证器接口
│   ├── scl_parser.h       # XML 解析器接口
│   └── scl_checker.h      # 检查器接口
├── src/
│   ├── scl_validator.c    # 验证器实现
│   ├── scl_parser.c       # XML 解析实现
│   ├── scl_checker.c      # 检查实现
│   ├── scl_validate.c     # 命令行工具
├── test/
│   ├── sample.icd         # 正确示例文件
│   └ sample_errors.icd    # 错误示例文件
├── lib/                   # 编译输出库
├── bin/                   # 编译输出可执行文件
├── mxml/                  # Mini-XML 库（自动下载）
└── Makefile               # 构建脚本
```

## 构建方法

### 本地编译 (x86 Linux)
```bash
make all
```

### ARM 交叉编译
```bash
make arm
```

### 安装
```bash
make install DESTDIR=/opt/scl_validator
```

## 使用方法

### 基本用法
```bash
./bin/scl_validate device.icd
```

### 输出到文件
```bash
./bin/scl_validate project.scd -o report.txt
```

### 详细模式
```bash
./bin/scl_validate device.cid -v
```

### 严格模式（警告视为错误）
```bash
./bin/scl_validate device.icd --strict
```

### 禁用特定检查
```bash
./bin/scl_validate device.icd --no-schema
./bin/scl_validate device.icd --no-values
```

## API 使用示例

```c
#include "scl_validator.h"

int main(int argc, char* argv[])
{
    /* 创建验证器 */
    SclValidator* validator = SclValidator_Create();
    
    /* 设置选项 */
    SclValidationOptions options = SclValidator_GetDefaultOptions();
    options.strict_mode = true;
    SclValidator_SetOptions(validator, &options);
    
    /* 验证文件 */
    bool success = SclValidator_ValidateFile(validator, "device.icd");
    
    /* 打印报告 */
    SclValidator_PrintReport(validator, stdout);
    
    /* 检查结果 */
    if (SclValidator_HasErrors(validator)) {
        printf("Validation failed with %d errors\n", 
               SclValidator_GetErrorCount(validator));
    }
    
    /* 清理 */
    SclValidator_Destroy(validator);
    
    return success ? 0 : 1;
}
```

## 错误类型

| 类型 | 说明 |
|------|------|
| Schema | XML 结构/格式错误 |
| Model integrity | 模型完整性错误 |
| Missing attribute | 必需属性缺失 |
| Invalid value | 属性值无效 |
| Reference | 引用未定义元素 |
| Structure | 结构层级错误 |

## 返回值

- 0: 验证通过
- 1: 参数错误
- 2: 验证失败（发现错误）

## 依赖

- Mini-XML (mxml) - 轻量级 XML 解析库
- C 标准库

## 许可证

Apache License 2.0