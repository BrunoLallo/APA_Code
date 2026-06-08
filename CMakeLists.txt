cmake_minimum_required(VERSION 3.12)
project(FiltragemAPA CXX)
file(GLOB FONTES_SRC CONFIGURE_DEPENDS "src/*.cpp")
add_executable(app main.cpp ${FONTES_SRC})

target_compile_definitions(app PRIVATE "PASTA_IMAGENS=\"${CMAKE_CURRENT_SOURCE_DIR}/images/\"")
target_compile_definitions(app PRIVATE "PASTA_RELATORIOS=\"${CMAKE_CURRENT_SOURCE_DIR}/relatorios/\"")

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/headers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)