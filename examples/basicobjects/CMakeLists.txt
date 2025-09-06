cmake_minimum_required(VERSION 3.10)
project(basicobjects C CXX ASM)

include(GNUInstallDirs)
set(CMAKE_BUILD_TYPE Release)

find_package(Bullet REQUIRED)

add_executable(${PROJECT_NAME}.elf ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC -L${PSPSDK}/lib -L${PSPDEV}/lib ${BULLET_DEFINITIONS} ${BULLET_LIBRARY_DIRS} -L${CMAKE_BINARY_DIR}
	-lLinearMath -lBulletSoftBody -lBullet3Collision -lBullet3Common -lBullet3Dynamics -lBullet3Geometry -lBulletCollision -lBulletDynamics -lBulletInverseDynamics -lBulletSoftBody -lAMG
	-lpspjpeg -llodepng -lpspaudio -lpspmp3 -lpsputility -lpsppower -lpsprtc -lpspgu -lm -lstdc++ -lpspsdk -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspnet -lpspnet_apctl)

create_pbp_file(TARGET ${PROJECT_NAME}.elf
	TITLE ${PROJECT_NAME}
	ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../assets/icon0.png"
)
file(COPY assets/ DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets")

