cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

PROJECT(multiblink C CXX ASM)

pico_sdk_init()

add_executable(multiblink multiblink.c)

target_link_libraries(multiblink pico_stdlib pico_cyw43_arch_none)


pico_add_extra_outputs(multiblink)

# add_custom_command(COMMAND cp bin/blink.uf2 /e/)
