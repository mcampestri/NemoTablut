cmake_minimum_required(VERSION 3.14)
project(FinalTablut C)

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_FLAGS -pthread)
SET(CMAKE_C_FLAGS -O3)
SET(CMAKE_C_FLAGS -Wall)
SET(CMAKE_C_FLAGS -mcmodel=medium)

include_directories(.)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")


add_executable(TablutFinale
        board.c
        board.h
        client.c
        client.h
        entry.h
        evaluation.c
        evaluation.h
        main.c
        minmax.c
        minmax.h
        move_type.h
        move_type.c
        move_sorting.c
        move_sorting.h
        moves.c
        moves.h
        search.c
        search.h
        trasformation.c
        trasformation.h
        trasposition_table.c
        traspostition_table.h
        abdada.c
        abdada.h
        time_control.c
        time_control.h
        search_threads.c
        search_threads.h
        draws_control.c
        draws_control.h
        parameters.h)
