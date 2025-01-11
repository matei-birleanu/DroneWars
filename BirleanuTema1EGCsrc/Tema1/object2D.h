#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float width,
        float height,
        glm::vec3 color,
        bool fill);
    Mesh* CreateTrapezoid(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float widthTop,
        float widthBottom,
        float height,
        glm::vec3 color,
        bool fill);

    Mesh* CreateArc(
        const std::string& name,
        glm::vec3 center,
        float radius,
        float startAngle,
        float endAngle,
        int segments,
        glm::vec3 color,
        bool fill);
    Mesh* CreateInvertedTrapezoid(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float widthTop,
        float widthBottom,
        float height,
        glm::vec3 color,
        bool fill);
    Mesh* CreateTrapezoidWithWideBase(
        const std::string& name, 
        glm::vec3 leftBottomCorner, 
        float widthBottom, 
        float widthTop,
        float height, 
        glm::vec3 color, 
        bool fill);
    Mesh* CreateSemicircle(
        const std::string& name,
        glm::vec3 center,
        float radius,
        int segments,
        glm::vec3 color,
        bool fill);

    Mesh* CreateInvertedSemicircle(const std::string& name, glm::vec3 center, float radius, int segments, glm::vec3 color, bool fill);

    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool filled);

    Mesh* CreateEllipse(const std::string& name, glm::vec3 center, float radiusX, float radiusY, glm::vec3 color, bool fill);


}
