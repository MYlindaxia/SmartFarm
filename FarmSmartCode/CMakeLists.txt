cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME FarmSmartCode)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        Core/ASP/oled/oled.h
        Core/ASP/oled/oled.c
        Core/ASP/oled/font.h
        Core/ASP/oled/font.c
        Core/ASP/aht20/aht20.h
        Core/ASP/aht20/aht20.c
        Core/configuration/environment.h
        Core/configuration/environment.c
        Core/configuration/screen.h
        Core/configuration/screen.c
        Core/utils/FormatValue.h
        Core/utils/FormatValue.c
        Core/ASP/buzzer/buzzer.h
        Core/ASP/buzzer/buzzer.c
        Core/ASP/encoder/encoder.h
        Core/ASP/encoder/encoder.c
        Core/ASP/ble/ble.h
        Core/ASP/ble/ble.c
        Core/ASP/key/key.h
        Core/ASP/key/key.c
        Core/ASP/light/light.h
        Core/ASP/light/light.c
        Core/utils/soft_I2C.h
        Core/utils/soft_I2C.c
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
        Core/utils
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
