# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS  ${CMAKE_CURRENT_SOURCE_DIR}/../components/gpio ${CMAKE_CURRENT_SOURCE_DIR}/../components/led ${CMAKE_CURRENT_SOURCE_DIR}/../components/wifi)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(hello-http-server)
