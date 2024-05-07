//

//

#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"
#include <map>
#include <vector>
#include "Coord.h"


class Camera {

    std::map<int, std::string> *debug_string_add_map_;

public:
    std::tuple<Coord, Coord> storedStates[5];
    Coord pos;
    Coord tgt;
    Coord up;
    Coord ang;
    int whichPlace;

    // constructor:

    Camera(float position[3], float target[3], float orientation[3], float transSpeed, float rot_sens);

    Camera(Coord pos, Coord target, Coord orientation, float transSpeed, float rot_sens);

    Camera(Coord Pos, Coord Target, Coord orientation);

    Coord normDirVec();

    int saveToFile(const std::string &filename);

    int loadFromFile(const std::string &filename);

    void storeState(int index) {
        storedStates[index] = std::make_tuple(pos, tgt);
    }

    void restoreState(int index) {

        pos = std::get<0>(storedStates[index]);
        tgt = std::get<1>(storedStates[index]);
        lookAt();
    }

    void setPitchYaw();


    Camera() = default;

    int whichSpace(Coord posi);

    int collisionYZ(Coord pos);

    int collisionYX(Coord pos);

    int collisionXZ(Coord pos);

    void getRelTrans(Coord deltaTranslation);

    void moveCamWithColl(Coord deltaTranslation);

    // calculating movement
    void relTrans(Coord deltaTranslation);

    void relRot(Coord deltaAngle);

    void updateCardInfo();

    void lookAt(DebugLevel dbg = NONE);

    std::vector<std::string> toString(int prec) const;

    // std::vector<std::string> toString() const;

    // void relRot(float pitch, float yaw);

    void setDebugStringAdd(std::map<int, std::string> *mapPtr) {
        this->debug_string_add_map_ = mapPtr;
    }

    void useDebugStringAdd(int lineInt, std::string message);

    Coord vec() const {
        return Coord(cos(ang.Y) * cos(ang.X), sin(ang.X), sin(ang.Y) * cos(ang.X));
    };
};

Coord calcPitchYaw(Coord position, Coord target);

Coord calcTarget(Coord position, Coord pitchYaw);

std::string trunc(float fl, int prec = 3);

#endif //CAMERA_H
