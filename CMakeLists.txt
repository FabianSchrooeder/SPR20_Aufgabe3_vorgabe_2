cmake_minimum_required(VERSION 3.10)
project(SPR20_Aufgabe3_vorgabe_2 C)

set(CMAKE_C_STANDARD 11)

include_directories(lib)

add_executable(SPR20_Aufgabe3_vorgabe_2
        lib/colors.h
        lib/FCFS.h
        lib/HRRN.h
        lib/LCFS.h
        lib/MLF.h
        lib/PRIOP.h
        lib/process.h
        lib/queue.h
        lib/RR.h
        lib/SRTNP.h
        src/colors.c
        src/FCFS.c
        src/HRRN.c
        src/LCFS.c
        src/MLF.c
        src/PRIOP.c
        src/queue.c
        src/RR.c
        src/SRTNP.c
        main.c)
