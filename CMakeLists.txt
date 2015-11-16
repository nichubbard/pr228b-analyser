cmake_minimum_required (VERSION 2.6)
include (CMakeDependentOption)
project (analyzer)

set (MIDASSYS $ENV{MIDASSYS})

set (sources adc.c ame.c analyzer.c f-plane.c
    multiTDC.c Parameters.c qdc.c scaler.c)

option (ENABLE_GAMMA
    "Support sorting gamma data into GammaData, reconfigure to enable specific detectors" OFF)
cmake_dependent_option(ENABLE_GAMMA_LEPS
    "Support sorting LEPS data into GammaData" OFF
    "ENABLE_GAMMA" OFF)
option (ENABLE_RAW
    "Store raw ADC/TDC/QDC into RawData for testing, slows down sort a lot" OFF)
option (ENABLE_SILICON
    "Support sorting silicon data into SiliconData, reconfigure to enable specific detectors" OFF)
cmake_dependent_option (ENABLE_SILICON_W1
    "Support sorting W1 data into SiliconData" OFF
    "ENABLE_SILICON" OFF)
cmake_dependent_option (ENABLE_SILICON_X1
    "Support sorting X1 data into SiliconData" OFF
    "ENABLE_SILICON" OFF)
cmake_dependent_option (ENABLE_SILICON_MMM
    "Support sorting MMM data into SiliconData" OFF
    "ENABLE_SILICON" OFF)

if (ENABLE_GAMMA)
  set (sources ${sources} GammaData.c)
endif()
if (ENABLE_GAMMA_LEPS)
  set (sources ${sources} LEPS.c)
endif()
if (ENABLE_RAW)
  set (sources ${sources} RawData.c)
endif()
if (ENABLE_SILICON)
  set (sources ${sources} SiliconData.c)
endif()
if (ENABLE_SILICON_MMM)
  set (sources ${sources} MMM.c)
endif()
if (ENABLE_SILICON_W1)
  set (sources ${sources} W1.c)
endif()
if (ENABLE_SILICON_X1)
  set (sources ${sources} X1.c)
endif()

SET(full_source "")
FOREACH(f ${sources})
  LIST(APPEND full_source "src/${f}")
ENDFOREACH(f)
SET(sources ${full_source})

configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

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

find_package("ROOT" REQUIRED COMPONENTS Graf Hist Net Rint RIO Thread Tree)
if (ROOT_FOUND)
    include_directories(${ROOT_INCLUDE_DIRS})
    target_link_libraries(analyzer ${ROOT_LIBRARIES})
    add_definitions(${ROOT_DEFINITIONS} -DHAVE_ROOT -DUSE_ROOT)
endif()

