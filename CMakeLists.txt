cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nrf52840-dongle)

target_sources(app PRIVATE 
    src/main.c
)

target_sources(app PRIVATE 
    src/remote_service/remote.c
)

target_sources(app PRIVATE 
    src/user_peripherial/adc.c
)

add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/hex-edit.sh
    COMMENT "Running hex edit shell file..."
)

zephyr_library_include_directories(src/remote_service)
zephyr_library_include_directories(src/user_peripherial)
