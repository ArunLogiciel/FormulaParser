cmake_minimum_required(VERSION 3.15)

project(FormulaParser VERSION 0.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # Export all symbols automatically

add_definitions(/bigobj)
# ------------------------

# Force static runtime to make DLL self-contained
if(MSVC)
    # MultiThreaded = /MT, MultiThreadedDebug = /MTd
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
# --- Boost setup ---
if(NOT BOOST_ROOT)
    set(BOOST_ROOT "D:\\OE-Production\\Libraries_64bit_VC17\\boost_1_82_0")
endif()

find_package(Boost REQUIRED)
set(Boost_NAMESPACE ${Boost_NAMESPACE} "libboost")

# --- Source files ---
set(src
    "src/FormulaParser.cpp"
    "src/RawParser.cpp"
    "src/TabularMethod.cpp"
    "src/JsonParser.cpp"
    "src/TieredMethod.cpp"
)

# --- Build shared library (DLL) ---
add_library(${PROJECT_NAME} SHARED ${src}
    "include/FormulaParser.h"
    "include/JsonParser.h"
)

# --- Set the output DLL name ---
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "FeeCalculationHelper")

# --- Include directories ---
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        "${PROJECT_SOURCE_DIR}/include"
        "D:\\FormulaParsar\\FormulaParsar\\include"  # RapidJSON headers if needed
)

# --- Preprocessor definition for exports ---
target_compile_definitions(${PROJECT_NAME} PRIVATE FORMULAPARSER_EXPORTS)

# --- Link Boost headers ---
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::headers)