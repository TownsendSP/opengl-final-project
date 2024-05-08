cmake_minimum_required(VERSION 3.22)
project(tgsouthaFinalProject)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_PREFIX_PATH "C:/Program/NVIDIA Corporation/Cg")
endif()

set(MAIN_BINARY_NAME "tgsouthaFinalProject")
set(TESTING_BINARY_NAME "testing")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


include_directories(${OPENGL_INCLUDE_DIR}   ${GLUT_INCLUDE_DIRS})

add_executable(${MAIN_BINARY_NAME} main.cpp
        res/models/crystal.h
        res/models/hatuv.h
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

target_link_libraries (${MAIN_BINARY_NAME}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(testing
        mainForTesting.cpp
        res/models/batuv.h
        res/models/crystal.h
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
target_link_libraries (${TESTING_BINARY_NAME}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})



add_executable(objtoh
        objtoh/objtoh.cpp
        objtoh/tiny_obj_loader.h
)

