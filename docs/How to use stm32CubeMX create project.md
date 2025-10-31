## 使用 stm32CubeMX 快速创建工程
+ 选择 stm32f407vet6
+ 编辑 project name：stm32f407vet6-micropython
+ Toolchain：CMake

## 编译
+ vscode: cmake build

```bash
[build] [21/21] Linking C executable stm32f407vet6-micropython.elf
[build] Memory region         Used Size  Region Size  %age Used
[build]              RAM:        1584 B       128 KB      1.21%
[build]           CCMRAM:           0 B        64 KB      0.00%
[build]            FLASH:        5664 B       512 KB      1.08%
[driver] Build completed: 00:00:00.524
[build] Build finished with exit code 0
```

+ 添加 clang 设置

```bash
{
    "code-runner.saveFileBeforeRun": true,
    "cmake.generator": "Ninja", //Unix Makefiles
    "cmake.saveBeforeBuild": true,
    "cmake.buildBeforeRun": true,
    "C_Cpp.intelliSenseEngine": "disabled",
    "clangd.arguments": [
        "--clang-tidy",
        "--compile-commands-dir=./build/Debug", // compile_commands.json path
        "--log=verbose",
        "--pretty",
        "--ranking-model=heuristics",
        "--query-driver=${env:ARM_C_PATH}" // compiler path
    ],
    "todo-tree.tree.showBadges": false,
    "files.associations": {
        "main.h": "c",
        "spi.h": "c",
        "cmsis_os.h": "c"
    },
}
```

主要是指定compile_commands.json path，让 clangd 可以正常跳转

## 烧录
+ 使用 OpenOCD 烧录，添加`flash_by_install.cmake`

```bash
install(
  CODE CODE
  "MESSAGE(\"Flash Debug......\")"
  CODE "execute_process(COMMAND openocd -f ${PROJECT_SOURCE_DIR}/Scripts/cmsis-dap.cfg -c \"init; reset
halt; program ${PROJECT_SOURCE_DIR}/build/Debug/stm32f407vet6-micropython.elf reset\" -c
shutdown)")
```

+ 添加`cmsis-dap.cfg`，用于手动调整 speed，speed 过快可能会导致烧录失败

```bash
source [find interface/cmsis-dap.cfg]
transport select swd
source [find target/stm32f4x.cfg]
adapter speed 1000
```

