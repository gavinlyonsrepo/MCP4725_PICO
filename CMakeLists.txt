# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(mcp4725 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# turn on all compiler warnings 
add_compile_options(-Wall -Wextra)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
  examples/isConnected/main.cpp
  #examples/demoTest/main.cpp
  #examples/setVoltage/main.cpp
  #examples/squareWave/main.cpp
  #examples/triangleWave/main.cpp
  #examples/sawToothWave/main.cpp
  #examples/sineWave/main.cpp
  #examples/random/main.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_mcp4725 INTERFACE)

target_sources(pico_mcp4725 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/mcp4725/mcp4725.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/mcp4725/mcp4725_Sinewave_Data.cpp
)

target_include_directories(pico_mcp4725 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c pico_mcp4725)


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

