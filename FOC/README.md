# 简介

本示例演示了基于 RT-Thread 的 FOC (Field-Oriented Control) 磁场定向控制算法，用于高性能电机控制。该项目实现了完整的 FOC 控制流程，包括 Clarke 变换、Park 变换、PID 控制和 SVPWM 调制。

## 示例展示的功能

- Clarke 变换：三相到两相静止坐标系变换
- Park 变换：两相静止到两相旋转坐标系变换
- PID 控制：电流环和速度环控制
- SVPWM 调制：空间矢量脉宽调制
- 电机控制：无刷直流电机或永磁同步电机控制

## 准备工作

- 连接电机驱动板和电机
- 配置 PWM 输出引脚用于电机驱动
- 配置 ADC 引脚用于电流和位置反馈
- 确保电源供应稳定
- 注意：
  - 电机参数需根据实际电机调整
  - 电流传感器范围需匹配电机额定电流
  - PWM 频率需根据电机特性设置

## 运行示例

编译程序并下载，若程序被正确执行，则调试串口会输出如下信息：

```console
 \ | /
- RT -     Thread Operating System
 / | \     5.0.1 build Jul 28 2023 17:17:20
 2006 - 2022 Copyright by RT-Thread team
msh >
```

按 `TAB` 键会输出如下命令：

### RT-Thread 命令

```console
RT-Thread shell commands:
clear            - clear the terminal screen
version          - show RT-Thread version information
list             - list objects
help             - RT-Thread shell help.
ps               - List threads in the system.
free             - Show the memory usage in the system.
date             - get date and time or set (local timezone) [year month day hour min sec]
foc              - foc [option]
motor            - motor control [option]
reset            - reset the board
foc_sample       - FOC control sample
```

测试时请使用 `foc_sample` 命令。当命令被正确执行后，例程会循环演示：

- FOC 控制初始化
- 电机启动和速度控制
- 电流环和速度环 PID 调节
- SVPWM 波形生成

## 配置说明

- `hpm_motor.h` / `hpm_motor.c`: 电机参数配置和控制函数
- `hpm_pid.h` / `hpm_pid.c`: PID 控制器实现
- `hpm_clarke.h` / `hpm_clarke.c`: Clarke 变换
- `hpm_park.h` / `hpm_park.c`: Park 变换
- `hpm_svpwm.h` / `hpm_svpwm.c`: SVPWM 调制
- `hpm_foc.h` / `hpm_foc.c`: FOC 主控制算法

## 注意事项

- 电机控制涉及高电压和高电流，请确保安全措施
- 首次运行时请在低速下测试
- 根据电机参数调整 PID 参数
- 监控电机温度和电流，避免过载
