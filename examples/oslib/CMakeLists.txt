cmake_minimum_required(VERSION 3.10)
project(oslib C CXX ASM)

include(GNUInstallDirs)
set(CMAKE_BUILD_TYPE Release)

add_executable(${PROJECT_NAME}.elf ${PROJECT_NAME}.cpp)
target_compile_definitions(${PROJECT_NAME}.elf PUBLIC AMG_COMPAT_OSLIB=1)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC -L${PSPSDK}/lib -L${PSPDEV}/lib -lAMG -losl -lpng -lz -lpspjpeg -llodepng -lpspaudio -lpspmp3 -lbulletpsp -lpsputility -lpsppower -lpsprtc -lpspgu -lm -lstdc++ -lpspsdk -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpsphprm -lpspnet -lpspnet_apctl)

create_pbp_file(TARGET ${PROJECT_NAME}.elf
	TITLE ${PROJECT_NAME}
	ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../assets/icon0.png"
)
file(COPY assets/ DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets")

