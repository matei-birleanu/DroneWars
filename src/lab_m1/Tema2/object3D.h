#pragma once

#include <string>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "core/gpu/vertex_format.h"

namespace object3D
{
    // Creare pătrat 2D (doar dacă ai nevoie)
    Mesh* CreateSquare(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float length,
        glm::vec3 color,
        bool fill = false);

    // Creare paralelipiped 3D cu dimensiuni (sizeX, sizeY, sizeZ) și culoare uniformă
    Mesh* CreateParallelepiped(
        const std::string& name,
        float sizeX,
        float sizeY,
        float sizeZ,
        glm::vec3 color);
}
