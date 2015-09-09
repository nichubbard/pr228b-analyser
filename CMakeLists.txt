cmake_minimum_required (VERSION 2.6)
project (analyzer)

set (MIDASSYS $ENV{MIDASSYS})

set (sources adc.c ame.c analyzer.c f-plane.c GammaData.c HagarSort.c
    LEPS.c multiTDC.c Parameters.c qdc.c RawData.c scaler.c
    SiliconData.c W1.c X1.c)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(analyzer ${sources})

SET(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER})

if (NOT DEFINED MIDASSYS)
    message(FATAL_ERROR "Missing definition of MIDAS variables")
endif()

if(${CMAKE_SYSTEM} MATCHES Linux)
    set (OS_DIR linux)
    add_definitions(-DOS_LINUX -Dextname)
elseif (${CMAKE_SYSTEM} MATCHES Apple)
    set (OS_DIR darwin)
    add_definitions(-DOS_LINUX -Dextname)
else()
    message (FATAL_ERROR "Unsupported OS")
endif()

add_definitions(-Wno-cpp -Wno-write-strings -Wno-unused-variable
    -Wno-unused-parameter -Wno-missing-field-initializers
    -Wno-unused-but-set-variable -Wno-maybe-uninitialized)

set (CMAKE_C_FLAGS_DEBUG -O0)
set (CMAKE_C_FLAGS_RELEASE -O3)

find_package("ROOT" REQUIRED COMPONENTS Graf Hist Net Rint RIO Thread Tree)

if (ROOT_FOUND)
    include_directories(${ROOT_INCLUDE_DIRS})
    target_link_libraries(analyzer ${ROOT_LIBRARIES})
    add_definitions(${ROOT_DEFINITIONS} -DHAVE_ROOT -DUSE_ROOT)
endif()


if (MIDASSYS)
    include_directories(${MIDASSYS}/include)
    set (MIDAS_LIB ${MIDASSYS}/${OS_DIR}/lib)
    target_link_libraries(analyzer ${MIDAS_LIB}/rmana.o ${MIDAS_LIB}/libmidas.a)
endif()

find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(analyzer ${ZLIB_LIBRARIES})
endif()

find_package(Threads REQUIRED)
target_link_libraries(analyzer ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(analyzer rt util)
