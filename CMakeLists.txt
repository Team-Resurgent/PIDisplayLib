# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
   set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(PICO_SDK_PATH ${USERHOME}/.pico-sdk/sdk/1.5.1)
set(PICO_TOOLCHAIN_PATH ${USERHOME}/.pico-sdk/toolchain/13_2_Rel1)
if(WIN32)
    set(pico-sdk-tools_DIR ${USERHOME}/.pico-sdk/tools/1.5.1)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config.cmake)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config-version.cmake)
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(PIDisplayLibExample C CXX ASM)

pico_sdk_init()

add_executable(PIDisplayLibExample 
  main.cpp 
)

target_include_directories(
  PIDisplayLibExample PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

add_subdirectory(PIDisplayLib build)

target_link_libraries(PIDisplayLibExample 
  PIDisplayLib
  pico_stdlib 
  pico_multicore 
  hardware_spi 
  hardware_i2c
)

# create map/bin/hex file etc.
pico_add_extra_outputs(PIDisplayLibExample)

# add url via pico_set_program_url
pico_enable_stdio_usb(PIDisplayLibExample 1)
pico_enable_stdio_uart(PIDisplayLibExample 0)