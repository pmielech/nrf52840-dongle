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

zephyr_library_include_directories(src/remote_service)
zephyr_library_include_directories(src/user_peripherial)