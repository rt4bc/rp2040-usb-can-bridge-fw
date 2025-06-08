cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(rp2040_usb_can_bridge_fw)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(rp2040_usb_can_bridge
    src/main.c
    src/usb_descriptors.c
    src/can.c
    src/slcan.c
    src/error.c
    src/led.c
)
option(DEBUG_PRINT "Enable debug output in C code" OFF)
if(DEBUG_PRINT)
    #target_compile_definitions(rp2040_usb_can_bridge PUBLIC DEBUG_PRINT=${DEBUG_PRINT})
    add_compile_definitions(DEBUG_PRINT)
endif()

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(rp2040_usb_can_bridge pico_stdlib hardware_spi pico_unique_id tinyusb_device tinyusb_board )

target_include_directories(rp2040_usb_can_bridge PUBLIC inc)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(rp2040_usb_can_bridge)