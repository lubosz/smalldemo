cmake_minimum_required(VERSION 2.8)
project(smalldemo)

#grset(CMAKE_BUILD_TYPE RELEASE)

#set(CMAKE_CXX_FLAGS "-O3 -g0") #Optimized

#set(CMAKE_CXX_FLAGS "-Os -Wl,--gc-sections -ffunction-sections -fdata-sections")

#find_package(Qt4 REQUIRED)

#build examples
#file(GLOB HEADERS src/*.h)
#file(GLOB SOURCES src/*.c)

#add_executable(smalldemo ${SOURCES} ${HEADERS})
#add_executable(smalldemo src/nurbs.c)
#add_executable(small src/small.c)
#add_executable(smallxcb-only src/xcb-only.c)
add_executable(smallglx src/glx.c)
#target_link_libraries(small glut)
#add_executable(smalldemo ${SOURCES})
#target_link_libraries(${EXAMPLE_NAME} lub xcb rt GL)
target_link_libraries(smallglx GL X11)
