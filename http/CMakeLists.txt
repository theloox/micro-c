cmake_minimum_required(VERSION 2.8)

## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_NAME "xhttp" )

## Set our project name
project(${PROJECT_NAME})

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB SRCS "*.cpp" "*.c")

## Define the executable
add_library(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} lxSTR)
