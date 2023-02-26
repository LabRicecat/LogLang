cmake_minimum_required(VERSION 3.9)

project(mathenyatica 
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "A pure functional logic language"
)

set(CMAKE_CXX_STANDARD 20)
set(BINARY mathenyatica)
set(SOURCE 
    main.cpp 
)

add_executable(${BINARY} ${SOURCE})
target_link_options(${BINARY} PUBLIC -lreadline)
install(TARGETS ${BINARY})
