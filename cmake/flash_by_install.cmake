install(
  CODE CODE
  "MESSAGE(\"Flash Debug......\")"
  CODE "execute_process(COMMAND openocd -f ${PROJECT_SOURCE_DIR}/Scripts/cmsis-dap.cfg -c \"init; reset
halt; program ${PROJECT_SOURCE_DIR}/build/Debug/stm32f407vet6-micropython.elf reset\" -c
shutdown)")