# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/Users/alexandruchiriac/src/pico-sdk")

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(InertialMeasurementUnit C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(InertialMeasurementUnit InertialMeasurementUnit.c
        MPU6050/MPU6050.h
        MPU9250/MPU9250.h
        MPU9250/MPU9250.h
        AK8963/AK8963.h
        BMP180/BMP180.h
        BMP180/BMP180.h
        i2c.h
        i2c.c
        units.h)

pico_set_program_name(InertialMeasurementUnit "InertialMeasurementUnit")
pico_set_program_version(InertialMeasurementUnit "0.1")

pico_enable_stdio_uart(InertialMeasurementUnit 0)
pico_enable_stdio_usb(InertialMeasurementUnit 1)

# Add the standard library to the build
target_link_libraries(InertialMeasurementUnit
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(InertialMeasurementUnit PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(InertialMeasurementUnit 
        hardware_i2c
        pico_cyw43_arch_none
        hardware_gpio
        )

pico_add_extra_outputs(InertialMeasurementUnit)

