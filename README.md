# <center>RT_Thread</center>

## 项目简介

这是一个基于 RT-Thread 实时操作系统的嵌入式项目集合。目前已上传的项目包括 FOC 磁场定向控制，其他项目正在开发中。

## 包含的项目

1. **FOC**: FOC 磁场定向控制项目 - 实现电机高性能控制算法
2. **rt-thread**: RT-Thread 源码 - 实时操作系统核心代码

## 项目结构

```
├── README.md                    # 项目总README
├── FOC/                         # FOC磁场定向控制项目
└── rt-thread/                   # RT-Thread源码
```

## 环境要求

- RT-Thread Studio 或其他支持的开发环境
- 目标硬件平台：HPM5E3Y 或兼容平台
- 编译工具链：支持 ARM Cortex-M 架构

## 构建和运行

### 编译项目

1. 打开 RT-Thread Studio
2. 导入 FOC 项目文件夹
3. 配置项目参数
4. 编译生成可执行文件

### 下载运行

1. 将编译生成的固件下载到目标板
2. 连接调试串口
3. 观察控制台输出

## 使用示例

- **FOC 示例**：运行 `FOC` 项目，实现电机磁场定向控制。详情请参考 `FOC/README.md`

## 配置说明

- `rtconfig.h`: RT-Thread 系统配置
- `board/`: 板级支持包配置
- `libraries/`: 外部库配置

## 开发指南

1. 熟悉 RT-Thread 开发环境
2. 从 FOC 项目开始学习
3. 根据需求修改和扩展代码
4. 调试和优化性能

## 注意事项

- 确保硬件连接正确，避免损坏芯片
- FOC 项目涉及电机控制，请注意安全

## 许可证

本项目基于 RT-Thread 开源许可证。

## 贡献

欢迎提交 Issue 和 Pull Request 来改进项目。

## 联系方式

如有问题，请通过 RT-Thread 社区或项目维护者联系。