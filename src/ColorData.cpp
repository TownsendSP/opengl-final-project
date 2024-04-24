//
// Created by tgsp on 4/4/2024.
//

#include "ColorData.h"




bool isColorCloseEnough(ColorData color1, ColorData color2, float closeness) {
    if (abs(color1.R - color2.R) < closeness &&
        abs(color1.G - color2.G) < closeness &&
        abs(color1.B - color2.B) < closeness) {
        return true;
        }
    return false;
}


