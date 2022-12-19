cmake_minimum_required(VERSION 3.8)

# Project
project(jade_cocoon_unpacker_repacker)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fmt
add_subdirectory(${PROJECT_SOURCE_DIR}/dep/fmt)

# Exe / Lib
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCES_NO_MAIN
	${SOURCES_DIR}/JCExe.cpp
	${SOURCES_DIR}/JCExe.hpp
	${SOURCES_DIR}/JCTools.cpp
	${SOURCES_DIR}/JCTools.hpp
	${SOURCES_DIR}/Types.hpp)

if(JCUR_LIB)
	add_library(jade_cocoon_unpacker_repacker)
else()
	add_executable(jade_cocoon_unpacker_repacker ${SOURCES_DIR}/Main.cpp)
endif()

target_sources(jade_cocoon_unpacker_repacker PRIVATE ${SOURCES_NO_MAIN})
target_link_libraries(jade_cocoon_unpacker_repacker PRIVATE fmt::fmt)
target_include_directories(jade_cocoon_unpacker_repacker PRIVATE ${PROJECT_SOURCE_DIR}/dep/fmt/include)