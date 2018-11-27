cmake_minimum_required(VERSION 2.8)

## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_NAME "micro-c" )

## Set our project name
project(${PROJECT_NAME})


include_directories(http lxSTR)

link_directories(http lxSTR)


add_subdirectory(analysis)
add_subdirectory(invoicing)
add_subdirectory(reporting)

add_subdirectory(client)

add_subdirectory(http)
add_subdirectory(lxSTR)

