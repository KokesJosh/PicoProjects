cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

PROJECT(${PROJECT_NAME} C CXX ASM)

pico_sdk_init()

add_executable(${EXE_NAME} ${EXE_NAME}.c)


target_link_libraries(${EXE_NAME} pico_stdlib pico_cyw43_arch_none)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)

#Create map/bin/hex
pico_add_extra_outputs(${EXE_NAME})

# add_custom_command(COMMAND cp bin/${EXE_NAME}.uf2 /e/)
