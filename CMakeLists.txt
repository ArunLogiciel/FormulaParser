cmake_minimum_required (VERSION 3.8)

project(FormulaParser VERSION 0.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # Export all symbols if not marked manually

add_definitions(/bigobj)

# Boost setup
if(NOT BOOST_ROOT)
    set(BOOST_ROOT "D:\\OE-Production\\Libraries_64bit_VC17\\boost_1_82_0")
endif()

find_package(Boost REQUIRED)

set(Boost_NAMESPACE ${Boost_NAMESPACE} "libboost")

# Source files
set(src
    "src/RawParser.cpp"
    "src/TabularMethod.cpp"
    "src/TieredMethod.cpp"
    "src/FormulaParser.cpp"
)

# Build as DLL (shared library)
add_library(${PROJECT_NAME} SHARED ${src} "include/FormulaParser.h" "include/JsonParser.h" "src/JsonParser.cpp")

# Tell the compiler to define FORMULAPARSER_EXPORTS when building this target
target_compile_definitions(${PROJECT_NAME} PRIVATE FORMULAPARSER_EXPORTS)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        "D:\\FormulaParsar\\FormulaParsar\\include"  # RapidJSON headers
)

# Boost headers
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::headers)
