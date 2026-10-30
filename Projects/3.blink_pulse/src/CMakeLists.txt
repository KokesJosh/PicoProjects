cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

PROJECT(blink_pulse C CXX ASM)

pico_sdk_init()

add_executable(blink_pulse blink_pulse.c)

target_link_libraries(blink_pulse pico_stdlib hardware_pwm pico_cyw43_arch_none)

pico_add_extra_outputs(blink_pulse)