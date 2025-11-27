cmake_minimum_required(VERSION 3.10.0)
project(lab6 VERSION 0.1.0 LANGUAGES C CXX)

if (MSVC)
    add_compile_options(/W3)
    add_compile_options(/std:c++23preview)
    add_compile_options(/EHsc)
    add_compile_options(/utf-8)
    add_compile_definitions(MSVC)
else()
    add_compile_options(-Wall)
    add_compile_options(-std=c++23)
    add_compile_options(-fexceptions)
endif()


add_executable(lab6
    main.cpp 
)
