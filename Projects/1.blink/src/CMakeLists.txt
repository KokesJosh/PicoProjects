cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

PROJECT(blink C CXX ASM)

pico_sdk_init()

add_executable(blink main.c)


target_link_libraries(blink pico_stdlib pico_cyw43_arch_none)

#Create map/bin/hex
pico_add_extra_outputs(blink)

# add_custom_command(COMMAND cp bin/blink.uf2 /e/)
