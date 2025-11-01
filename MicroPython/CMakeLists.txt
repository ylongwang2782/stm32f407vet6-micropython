# MicroPython 库的 CMakeLists.txt

# 设置 MicroPython 核心目录和目标
set(MICROPY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MICROPY_TARGET micropython)

# 定义目录变量
set(MICROPY_PY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/py)
set(MICROPY_SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)
set(MICROPY_PORT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/micropython_port)

# ==================== qstr 定义 ====================
# 定义 port 特定的 qstr 文件
set(MICROPY_QSTRDEFS_PORT
    ${MICROPY_PORT_DIR}/qstrdefsport.h
)

# 定义需要扫描 qstr 的源文件列表
set(MICROPY_SOURCE_QSTR
    ${MICROPY_PY_DIR}/argcheck.c
    ${MICROPY_PY_DIR}/asmarm.c
    ${MICROPY_PY_DIR}/asmbase.c
    ${MICROPY_PY_DIR}/asmrv32.c
    ${MICROPY_PY_DIR}/asmthumb.c
    ${MICROPY_PY_DIR}/asmx64.c
    ${MICROPY_PY_DIR}/asmx86.c
    ${MICROPY_PY_DIR}/asmxtensa.c
    ${MICROPY_PY_DIR}/bc.c
    ${MICROPY_PY_DIR}/binary.c
    ${MICROPY_PY_DIR}/builtinevex.c
    ${MICROPY_PY_DIR}/builtinhelp.c
    ${MICROPY_PY_DIR}/builtinimport.c
    ${MICROPY_PY_DIR}/compile.c
    ${MICROPY_PY_DIR}/cstack.c
    ${MICROPY_PY_DIR}/emitbc.c
    ${MICROPY_PY_DIR}/emitcommon.c
    ${MICROPY_PY_DIR}/emitglue.c
    ${MICROPY_PY_DIR}/emitinlinerv32.c
    ${MICROPY_PY_DIR}/emitinlinethumb.c
    ${MICROPY_PY_DIR}/emitinlinextensa.c
    ${MICROPY_PY_DIR}/emitnarm.c
    ${MICROPY_PY_DIR}/emitndebug.c
    ${MICROPY_PY_DIR}/emitnrv32.c
    ${MICROPY_PY_DIR}/emitnthumb.c
    ${MICROPY_PY_DIR}/emitnx64.c
    ${MICROPY_PY_DIR}/emitnx86.c
    ${MICROPY_PY_DIR}/emitnxtensa.c
    ${MICROPY_PY_DIR}/emitnxtensawin.c
    ${MICROPY_PY_DIR}/formatfloat.c
    ${MICROPY_PY_DIR}/frozenmod.c
    ${MICROPY_PY_DIR}/gc.c
    ${MICROPY_PY_DIR}/lexer.c
    ${MICROPY_PY_DIR}/malloc.c
    ${MICROPY_PY_DIR}/map.c
    ${MICROPY_PY_DIR}/modarray.c
    ${MICROPY_PY_DIR}/modbuiltins.c
    ${MICROPY_PY_DIR}/modcmath.c
    ${MICROPY_PY_DIR}/modcollections.c
    ${MICROPY_PY_DIR}/modgc.c
    ${MICROPY_PY_DIR}/modio.c
    ${MICROPY_PY_DIR}/modmath.c
    ${MICROPY_PY_DIR}/modmicropython.c
    ${MICROPY_PY_DIR}/modstruct.c
    ${MICROPY_PY_DIR}/modsys.c
    ${MICROPY_PY_DIR}/modthread.c
    ${MICROPY_PY_DIR}/moderrno.c
    ${MICROPY_PY_DIR}/mpprint.c
    ${MICROPY_PY_DIR}/mpstate.c
    ${MICROPY_PY_DIR}/mpz.c
    ${MICROPY_PY_DIR}/nativeglue.c
    ${MICROPY_PY_DIR}/obj.c
    ${MICROPY_PY_DIR}/objarray.c
    ${MICROPY_PY_DIR}/objattrtuple.c
    ${MICROPY_PY_DIR}/objbool.c
    ${MICROPY_PY_DIR}/objboundmeth.c
    ${MICROPY_PY_DIR}/objcode.c
    ${MICROPY_PY_DIR}/objcell.c
    ${MICROPY_PY_DIR}/objclosure.c
    ${MICROPY_PY_DIR}/objcomplex.c
    ${MICROPY_PY_DIR}/objdeque.c
    ${MICROPY_PY_DIR}/objdict.c
    ${MICROPY_PY_DIR}/objenumerate.c
    ${MICROPY_PY_DIR}/objexcept.c
    ${MICROPY_PY_DIR}/objfilter.c
    ${MICROPY_PY_DIR}/objfloat.c
    ${MICROPY_PY_DIR}/objfun.c
    ${MICROPY_PY_DIR}/objgenerator.c
    ${MICROPY_PY_DIR}/objgetitemiter.c
    ${MICROPY_PY_DIR}/objint.c
    ${MICROPY_PY_DIR}/objint_longlong.c
    ${MICROPY_PY_DIR}/objint_mpz.c
    ${MICROPY_PY_DIR}/objlist.c
    ${MICROPY_PY_DIR}/objmap.c
    ${MICROPY_PY_DIR}/objmodule.c
    ${MICROPY_PY_DIR}/objnamedtuple.c
    ${MICROPY_PY_DIR}/objnone.c
    ${MICROPY_PY_DIR}/objobject.c
    ${MICROPY_PY_DIR}/objpolyiter.c
    ${MICROPY_PY_DIR}/objproperty.c
    ${MICROPY_PY_DIR}/objrange.c
    ${MICROPY_PY_DIR}/objreversed.c
    ${MICROPY_PY_DIR}/objringio.c
    ${MICROPY_PY_DIR}/objset.c
    ${MICROPY_PY_DIR}/objsingleton.c
    ${MICROPY_PY_DIR}/objslice.c
    ${MICROPY_PY_DIR}/objstr.c
    ${MICROPY_PY_DIR}/objstringio.c
    ${MICROPY_PY_DIR}/objstrunicode.c
    ${MICROPY_PY_DIR}/objtuple.c
    ${MICROPY_PY_DIR}/objtype.c
    ${MICROPY_PY_DIR}/objzip.c
    ${MICROPY_PY_DIR}/opmethods.c
    ${MICROPY_PY_DIR}/pairheap.c
    ${MICROPY_PY_DIR}/parse.c
    ${MICROPY_PY_DIR}/parsenum.c
    ${MICROPY_PY_DIR}/parsenumbase.c
    ${MICROPY_PY_DIR}/persistentcode.c
    ${MICROPY_PY_DIR}/profile.c
    ${MICROPY_PY_DIR}/pystack.c
    ${MICROPY_PY_DIR}/qstr.c
    ${MICROPY_PY_DIR}/reader.c
    ${MICROPY_PY_DIR}/repl.c
    ${MICROPY_PY_DIR}/ringbuf.c
    ${MICROPY_PY_DIR}/runtime.c
    ${MICROPY_PY_DIR}/runtime_utils.c
    ${MICROPY_PY_DIR}/scheduler.c
    ${MICROPY_PY_DIR}/scope.c
    ${MICROPY_PY_DIR}/sequence.c
    ${MICROPY_PY_DIR}/showbc.c
    ${MICROPY_PY_DIR}/smallint.c
    ${MICROPY_PY_DIR}/stackctrl.c
    ${MICROPY_PY_DIR}/stream.c
    ${MICROPY_PY_DIR}/unicode.c
    ${MICROPY_PY_DIR}/vm.c
    ${MICROPY_PY_DIR}/vstr.c
    ${MICROPY_PY_DIR}/warning.c
    # shared/ files that contain root pointers
    ${MICROPY_SHARED_DIR}/readline/readline.c
    ${MICROPY_SHARED_DIR}/runtime/pyexec.c
)

# ==================== 包含目录 ====================
set(MICROPY_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MICROPY_PY_DIR}
    ${MICROPY_PORT_DIR}
    PARENT_SCOPE  # 让父项目也能访问
)

# ==================== py/ 核心源文件 ====================
file(GLOB MICROPY_PY_SRC
    "${MICROPY_PY_DIR}/*.c"
)

# ==================== shared/ 源文件 ====================
set(MICROPY_SHARED_SRC
    # libc
    ${MICROPY_SHARED_DIR}/libc/printf.c
    ${MICROPY_SHARED_DIR}/libc/string0.c
    
    # readline
    ${MICROPY_SHARED_DIR}/readline/readline.c
    
    # runtime
    ${MICROPY_SHARED_DIR}/runtime/pyexec.c
    ${MICROPY_SHARED_DIR}/runtime/stdout_helpers.c
    ${MICROPY_SHARED_DIR}/runtime/sys_stdio_mphal.c
    ${MICROPY_SHARED_DIR}/runtime/interrupt_char.c
    ${MICROPY_SHARED_DIR}/runtime/gchelper_thumb2.s  # ARM Cortex-M
)

# ==================== port 源文件 ====================
set(MICROPY_PORT_SRC
    ${MICROPY_PORT_DIR}/mpconfigport.c
    ${MICROPY_PORT_DIR}/mphalport.c
    # 添加其他你的移植文件
)

# ==================== 创建 MicroPython 静态库 ====================
add_library(micropython STATIC
    ${MICROPY_PY_SRC}
    ${MICROPY_SHARED_SRC}
    ${MICROPY_PORT_SRC}
)

# ==================== 设置包含目录 ====================
target_include_directories(micropython PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MICROPY_PY_DIR}
    ${MICROPY_PORT_DIR}
    ${CMAKE_BINARY_DIR}     # 用于包含 genhdr/xxx.h (相对路径)
    ${CMAKE_SOURCE_DIR}/Core/Inc      # 访问 main.h 等 HAL 头文件
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

# ==================== 编译选项 ====================
target_compile_definitions(micropython PUBLIC
    # 不使用 frozen modules，所以不需要 MICROPY_QSTR_EXTRA_POOL
    MICROPY_MODULE_FROZEN_MPY=0
)

# 针对 MicroPython 的特殊编译选项
target_compile_options(micropython PRIVATE
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-sign-compare
)

# ==================== 包含 MicroPython 的 qstr 生成系统 ====================
# 这会自动生成 qstrdefs.generated.h, mpversion.h, moduledefs.h, root_pointers.h
include(${MICROPY_PY_DIR}/mkrules.cmake)

# ==================== 汇编文件特殊处理 ====================
set_source_files_properties(
    ${MICROPY_SHARED_DIR}/runtime/gchelper_thumb2.s
    PROPERTIES 
    COMPILE_FLAGS "-x assembler-with-cpp"
    LANGUAGE ASM
)