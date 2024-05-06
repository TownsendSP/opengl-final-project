cmake_minimum_required(VERSION 3.22)
project(tgsouthaFinalProject)

set(CMAKE_CXX_STANDARD 17)

#if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_PREFIX_PATH "C:/Program/NVIDIA Corporation/Cg")
#endif()

set(MAIN_BINARY_NAME "tgsouthaFinalProject")

find_package(OpenGL REQUIRED)
#find_package(GL REQUIRED)
find_package(GLUT REQUIRED)


#set(GLUT_INCLUDE_DIR "C:/Program/NVIDIA Corporation/Cg/include")
#set(GLUT_glut_LIBRARY "C:/Program/NVIDIA Corporation/Cg/lib/glut32.lib")
#find_package(GLUT REQUIRED)


include_directories(${OPENGL_INCLUDE_DIR}  ${GLUT_INCLUDE_DIRS})
#include_directories(${OPENGL_INCLUDE_DIR}   ${GLUT_INCLUDE_DIRS})

message("OpenGL include directory: ${OPENGL_INCLUDE_DIRS}")
message("GLUT include directory: ${GLUT_INCLUDE_DIRS}")
message("GLUT Library: ${GLUT_glut_LIBRARY}")
message("OpenGL Library: ${OPENGL_LIBRARIES}")

#target_link_libraries (${MAIN_BINARY_NAME} ${GLUT_LIBRARIES})
add_executable(${MAIN_BINARY_NAME} main.cpp
        res/models/crystal.h
        res/models/hat.h
        res/models/hatuv.h
        res/models/lightsaber.h
        res/models/obiwan.h
        src/Camera.cpp
        src/Camera.h
        src/ColorData.cpp
        src/ColorData.h
        src/Coord.cpp
        src/Coord.h
        src/LeftVP.cpp
        src/LeftVP.h
        src/Scenedraw.cpp
        src/CampFire.cpp
        src/CampFire.h
        src/fSceneObjs.cpp
        src/fSceneObjs.h
        src/globals.cpp
        src/globals.h
        src/lighting.cpp
        src/lighting.h
        src/modelLoader.cpp
        src/modelLoader.h
        src/testingFunctions.cpp
        src/testingFunctions.h
        src/textureLoader.cpp
        src/textureLoader.h
        src/things.cpp
        src/things.h

)
target_link_libraries (${MAIN_BINARY_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
