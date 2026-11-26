cmake_minimum_required(VERSION 4.3)
project(Projeto_Oficina C)

set(CMAKE_C_STANDARD 11)

add_executable(Projeto_Oficina main.c
        ferramenta.c
        ferramenta.h)
