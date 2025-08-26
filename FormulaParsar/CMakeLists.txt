# CMakeList.txt : Top-level CMake project file, do global configuration
cmake_minimum_required (VERSION 3.25)

project(FormulaParser VERSION 0.0.0.0)

set(CMAKE_CXX_STANDARD 17)

add_definitions(/bigobj)

if(NOT BOOST_ROOT)
	set(BOOST_ROOT "D:\\OE-Production\\Libraries_64bit_VC17\\boost_1_82_0")
endif()

find_package(Boost REQUIRED)

set (Boost_NAMESPACE ${Boost_NAMESPACE} "libboost")

set (src
	"src/main.cpp"
	"src/RawParser.cpp"
	"src/TabularMethod.cpp"
)

add_executable(${PROJECT_NAME} ${src})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::headers)