# Build Guide

## How to build

All the description below is based on MacoOS(Apple sillicon M4 Pro)

1. clone the repo to local

```bash
git clone https://github.com/ylongwang2782/stm32f407vet6-micropython.git
```

2. install cmake

```bash
brew install cmake
```

3. install ninja

```bash
brew install ninja
```

4. install toolchain

- download arm-gnu-toolchain

https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

- extract toolchain

```bash
tar -xjf arm-gnu-toolchain-14.3.rel1-darwin-arm64-arm-none-eabi.tar.xz
sudo mv arm-gnu-toolchain-14.3.rel1-darwin-arm64-arm-none-eabi /usr/local/gcc-arm-none-eabi
```

- configure enviroment variable

```bash
nano ~/.zshrc
# add to file
export PATH="/usr/local/gcc-arm-none-eabi/bin:$PATH"
# then exit refresh cache
source ~/.zshrc
```

- build

```bash
cmake --build build/Debug
```

- result

```bash
[13/13] Linking C executable stm32f407vet6-micropython.elf
Memory region         Used Size  Region Size  %age Used
             RAM:         98 KB       128 KB     76.56%
          CCMRAM:           0 B        64 KB      0.00%
           FLASH:      214028 B       512 KB     40.82%
```

## How to download

I use the openocd with DapLink to download to board in vscode/cursor.

1. open vscode/cursor

2. cmd: cmake install

to change the download speed, change Scripts/cmsis-dap.cfg