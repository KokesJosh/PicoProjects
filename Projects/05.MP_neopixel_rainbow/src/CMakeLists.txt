cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

PROJECT(${PROJECT_NAME} C CXX ASM)

pico_sdk_init()

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.c)


target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_pio hardware_clocks pico_cyw43_arch_none)

pico_generate_pio_header(neopixel_rainbow
    ${CMAKE_CURRENT_LIST_DIR}/neopixel_rainbow.pio
)

#Create map/bin/hex
pico_add_extra_outputs(${PROJECT_NAME})

# add_custom_command(COMMAND cp bin/${PROJECT_NAME}.uf2 /e/)
