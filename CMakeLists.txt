cmake_minimum_required(VERSION 3.10)
project(AMGLib-Plus C CXX ASM)

include(GNUInstallDirs)
set(CMAKE_BUILD_TYPE Release)

option(BUILD_OSLIB "Build AMGLib-Plus with OSLib support" OFF)
option(BUILD_EXAMPLES "Build examples" ON)

find_package(Bullet REQUIRED)

add_library(lodepng STATIC)
target_include_directories(lodepng PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
)

target_sources(lodepng PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng/lodepng.cpp)

# Set the output library
set(TARGET_LIB AMG)
add_library(${TARGET_LIB} STATIC)

# Include directories
include_directories(
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${PSPDEV}/psp/sdk/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/AMG>
)

# Define the source files
set(SOURCE_DIR src)

file(GLOB_RECURSE LIB_SOURCES ${SOURCE_DIR}/*.cpp)

# Add sources to the target
target_sources(${TARGET_LIB} PRIVATE ${LIB_SOURCES})

# Define preprocessor defines
target_compile_definitions(${TARGET_LIB} PRIVATE _DEBUG PSP ${BULLET_DEFINITIONS})
target_include_directories(${TARGET_LIB} PUBLIC ${BULLET_INCLUDE_DIRS} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng>)
# Linker flags
target_link_libraries(${TARGET_LIB}
	-llodepng
	-lpspaudio
	-lpspmp3
	-lpsputility
	-lpspctrl
	-lpsppower
	-lpsprtc
	-lpspgu
	-lm
	-lstdc++
	-lpspsdk
	${OSL_LIBRARIES}
	Bullet
)

set(OSL_LIBS "-losl -lpng -lz -lpsphprm -lpspmpeg -lpspjpeg -lpspmpegbase -lpspaudiocodec")

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()


# Installation instructions
install(TARGETS lodepng DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS ${TARGET_LIB} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/external/lodepng/lodepng.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(DIRECTORY include/AMG DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h")
