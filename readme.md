# stm32f407vet6-micropython

A stm32f407vet6 micropython demo.

## Project Enviroment

1. IDE：vscode/cursor
2. Toolchain: cmake + arm-gcc-none-eabi
3. Debbuger: openocd + daplink

## How to use

- Build

[How to Build](docs/build_guide.md)

- Setup and Test

1. Build and download to stm32f407vet6 board.
2. Connect usart1(PA9_PA10) via ttl to USB.
3. Open serail port with 115200 baudrate on a serial terminal
4. Type `help()` to test and get result.
