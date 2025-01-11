#include "object3D.h"

#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"


namespace object3D
{
    // ------------------------------------------------------------------------
    // 1) Exemplu existent: creează un pătrat 2D (pentru referință)
    // ------------------------------------------------------------------------
    Mesh* CreateSquare(const std::string& name,
        glm::vec3 leftBottomCorner,
        float length,
        glm::vec3 color,
        bool fill)
    {
        glm::vec3 corner = leftBottomCorner;

        std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner,                                         color),
            VertexFormat(corner + glm::vec3(length, 0, 0),               color),
            VertexFormat(corner + glm::vec3(length, length, 0),          color),
            VertexFormat(corner + glm::vec3(0, length, 0),               color)
        };

        Mesh* square = new Mesh(name);
        std::vector<unsigned int> indices = { 0, 1, 2, 3 };

        if (!fill) {
            square->SetDrawMode(GL_LINE_LOOP);
        }
        else {
            // Draw 2 triangles. Add the remaining 2 indices
            indices.push_back(0);
            indices.push_back(2);
        }

        square->InitFromData(vertices, indices);
        return square;
    }

    // ------------------------------------------------------------------------
    // 2) Creează un cub 1x1x1 cu (0,0,0) - colț stânga-jos dedesubt
    //    Folosește un parametru size dacă vrem să-l facem ușor "scalabil" din start
    // ------------------------------------------------------------------------
    Mesh* CreateBox(const std::string& name,
        float size,
        glm::vec3 color)
    {
        // Vom defini cubul cu colțul la (0,0,0) și diagonala pe (size,size,size)
        // 8 vertecși (fiecare cu aceeași culoare).
        glm::vec3 corner = glm::vec3(0, 0, 0);

        std::vector<VertexFormat> vertices =
        {
            // bottom
            VertexFormat(corner + glm::vec3(0,       0,       0), color),
            VertexFormat(corner + glm::vec3(size,    0,       0), color),
            VertexFormat(corner + glm::vec3(size,    0,       size), color),
            VertexFormat(corner + glm::vec3(0,       0,       size), color),

            // top
            VertexFormat(corner + glm::vec3(0,       size,    0), color),
            VertexFormat(corner + glm::vec3(size,    size,    0), color),
            VertexFormat(corner + glm::vec3(size,    size,    size), color),
            VertexFormat(corner + glm::vec3(0,       size,    size), color),
        };

        // Cele 12 triughiuri (36 indici) care formează cubul
        std::vector<unsigned int> indices =
        {
            0, 1, 2,    0, 2, 3,      // bottom
            4, 5, 6,    4, 6, 7,      // top
            0, 1, 5,    0, 5, 4,      // face "front"
            1, 2, 6,    1, 6, 5,      // face "right"
            2, 3, 7,    2, 7, 6,      // face "back"
            3, 0, 4,    3, 4, 7       // face "left"
        };

        Mesh* cube = new Mesh(name);
        cube->InitFromData(vertices, indices);
        return cube;
    }

} // namespace object3D
