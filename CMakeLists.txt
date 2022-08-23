cmake_minimum_required(VERSION 3.19)
project(memory_issue)

set(GLOBAL_PROPERTIES
        CXX_STANDARD 20
        STANDARD_REQUIRED ON
        CXX_EXTENSIONS NO)

list(APPEND
        COMPILE_FLAGS
        -Wall
        -Werror
        -Wextra
        "$<$<CXX_COMPILER_ID:GNU>:-fcoroutines>"
        "$<$<CXX_COMPILER_ID:GNU>:-O0>"
        "$<$<CXX_COMPILER_ID:Clang>:-O0>"
        )

add_compile_options(${COMPILE_FLAGS})

include(FetchContent)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

find_library(LIB_UUID uuid REQUIRED)

find_package( Boost 1.77 COMPONENTS REQUIRED system filesystem thread log log_setup)

IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

set(BOOST_LIBS
        ${Boost_SYSTEM_LIBRARY}
)


add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES ${GLOBAL_PROPERTIES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${BOOST_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
)