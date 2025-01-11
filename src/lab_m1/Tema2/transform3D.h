#pragma once

#include "utils/glm_utils.h"
#include <GL/glew.h>

namespace transform3D
{
    inline glm::mat4 Translate(float tx, float ty, float tz)
    {
        return glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            tx, ty, tz, 1.f
        );
    }

    inline glm::mat4 Scale(float sx, float sy, float sz)
    {
        return glm::mat4(
            sx, 0.f, 0.f, 0.f,
            0.f, sy, 0.f, 0.f,
            0.f, 0.f, sz, 0.f,
            0.f, 0.f, 0.f, 1.f
        );
    }

    inline glm::mat4 RotateOX(float rads)
    {
        float c = cos(rads), s = sin(rads);
        return glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, c, s, 0.f,
            0.f, -s, c, 0.f,
            0.f, 0.f, 0.f, 1.f
        );
    }

    inline glm::mat4 RotateOY(float rads)
    {
        float c = cos(rads), s = sin(rads);
        return glm::mat4(
            c, 0.f, -s, 0.f,
            0.f, 1.f, 0.f, 0.f,
            s, 0.f, c, 0.f,
            0.f, 0.f, 0.f, 1.f
        );
    }

    inline glm::mat4 RotateOZ(float rads)
    {
        float c = cos(rads), s = sin(rads);
        return glm::mat4(
            c, s, 0.f, 0.f,
            -s, c, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        );
    }
}