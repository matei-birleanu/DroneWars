#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 corner1 = leftBottomCorner;
    glm::vec3 corner2 = leftBottomCorner + glm::vec3(width, 0, 0);
    glm::vec3 corner3 = leftBottomCorner + glm::vec3(width, height, 0);
    glm::vec3 corner4 = leftBottomCorner + glm::vec3(0, height, 0);

    vertices.push_back(VertexFormat(corner1, color));
    vertices.push_back(VertexFormat(corner2, color));
    vertices.push_back(VertexFormat(corner3, color));
    vertices.push_back(VertexFormat(corner4, color));

    Mesh* rectangle = new Mesh(name);

    if (fill)
    {
        indices = { 0, 1, 2, 0, 2, 3 };

        rectangle->InitFromData(vertices, indices);
    }
    else
    {
        indices = { 0, 1, 2, 3 };

        rectangle->SetDrawMode(GL_LINE_LOOP);
        rectangle->InitFromData(vertices, indices);
    }

    return rectangle;
}
Mesh* object2D::CreateTrapezoid(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float widthTop,
    float widthBottom,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 lb = leftBottomCorner;
    glm::vec3 lt = lb + glm::vec3((widthBottom - widthTop) / 2.0f, height, 0);
    glm::vec3 rt = lt + glm::vec3(widthTop, 0, 0);
    glm::vec3 rb = lb + glm::vec3(widthBottom, 0, 0);

    std::vector<VertexFormat> vertices = {
        VertexFormat(lb, color),
        VertexFormat(lt, color),
        VertexFormat(rt, color),
        VertexFormat(rb, color)
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices;

    if (fill)
    {
        indices = { 0, 1, 2, 0, 2, 3 };
        trapezoid->SetDrawMode(GL_TRIANGLES);
    }
    else
    {
        indices = { 0, 1, 2, 3 };
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}

Mesh* object2D::CreateArc(
    const std::string& name,
    glm::vec3 center,
    float radius,
    float startAngle,
    float endAngle,
    int segments,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float angleStep = (endAngle - startAngle) / segments;

    vertices.emplace_back(center, color);

    for (int i = 0; i <= segments; ++i)
    {
        float angle = startAngle + i * angleStep;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, y, 0), color);

        if (i > 0)
        {
            if (fill)
            {
                indices.push_back(0);
                indices.push_back(i);
                indices.push_back(i + 1);
            }
            else
            {
                indices.push_back(i);
                indices.push_back(i + 1);
            }
        }
    }

    Mesh* arc = new Mesh(name);

    if (fill)
    {
        arc->SetDrawMode(GL_TRIANGLES);
    }
    else
    {
        arc->SetDrawMode(GL_LINE_STRIP);
    }

    arc->InitFromData(vertices, indices);
    return arc;
}
Mesh* object2D::CreateInvertedTrapezoid(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float widthTop,
    float widthBottom,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 lb = leftBottomCorner + glm::vec3((widthTop - widthBottom) / 2.0f, 0, 0);
    glm::vec3 rb = lb + glm::vec3(widthBottom, 0, 0);
    glm::vec3 rt = leftBottomCorner + glm::vec3(widthTop, height, 0);
    glm::vec3 lt = leftBottomCorner + glm::vec3(0, height, 0);

    std::vector<VertexFormat> vertices = {
        VertexFormat(lb, color),
        VertexFormat(rb, color),
        VertexFormat(rt, color),
        VertexFormat(lt, color)
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices;

    if (fill)
    {
        indices = { 0, 1, 2, 0, 2, 3 };
        trapezoid->SetDrawMode(GL_TRIANGLES);
    }
    else
    {
        indices = { 0, 1, 2, 3, 0 };
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}

Mesh* object2D::CreateTrapezoidWithWideBase(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float widthBottom,
    float widthTop,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 lb = leftBottomCorner; 
    glm::vec3 rb = leftBottomCorner + glm::vec3(widthBottom, 0, 0);

    glm::vec3 lt = leftBottomCorner + glm::vec3((widthBottom - widthTop) / 2.0f, height, 0);
    glm::vec3 rt = lt + glm::vec3(widthTop, 0, 0);

    std::vector<VertexFormat> vertices = {
        VertexFormat(lb, color),
        VertexFormat(rb, color), 
        VertexFormat(rt, color), 
        VertexFormat(lt, color)
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices;

    if (fill)
    {
        indices = { 0, 1, 2, 0, 2, 3 };
        trapezoid->SetDrawMode(GL_TRIANGLES);
    }
    else
    {
        indices = { 0, 1, 2, 3, 0 };
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}


Mesh* object2D::CreateSemicircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    int segments,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float angleStep = M_PI / segments;

    vertices.emplace_back(center, color);

    for (int i = 0; i <= segments; ++i)
    {
        float angle = M_PI + i * angleStep;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, y, 0), color);
    }

    if (fill)
    {
        for (int i = 1; i <= segments; ++i)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }
    else
    {
        for (int i = 1; i <= segments; ++i)
        {
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }

    Mesh* semicircle = new Mesh(name);

    if (fill)
    {
        semicircle->SetDrawMode(GL_TRIANGLES);
    }
    else
    {
        semicircle->SetDrawMode(GL_LINE_STRIP);
    }

    semicircle->InitFromData(vertices, indices);
    return semicircle;
}


Mesh* object2D::CreateInvertedSemicircle(const std::string& name, glm::vec3 center, float radius, int segments, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(center, color));

    for (int i = 0; i <= segments; i++) {
        float angle = glm::pi<float>() + (i * glm::pi<float>() / segments);
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
        indices.push_back(i + 1);
    }

    if (fill) {
        for (int i = 1; i < segments; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }
    else {
        indices.push_back(1);
        indices.push_back(segments);
    }
    Mesh* turret = new Mesh(name);
    turret->InitFromData(vertices, indices);
    return turret;
}


Mesh* object2D::CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool filled)
{   
    int segments = 100;
    Mesh* mesh = new Mesh(name);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    if (filled)
    {
        vertices.emplace_back(center, color);

        for (int i = 0; i <= segments; ++i)
        {
            float angle = 2.0f * M_PI * i / segments;
            float x = center.x + radius * cos(angle);
            float y = center.y + radius * sin(angle);
            vertices.emplace_back(glm::vec3(x, y, center.z), color);

            if (i < segments)
            {
                indices.push_back(0);
                indices.push_back(i + 1);
                indices.push_back(i + 2);
            }
        }
    }
    else
    {
        for (int i = 0; i <= segments; ++i)
        {
            float angle = 2.0f * M_PI * i / segments;
            float x = center.x + radius * cos(angle);
            float y = center.y + radius * sin(angle);
            vertices.emplace_back(glm::vec3(x, y, center.z), color);

            if (i < segments)
            {
                indices.push_back(i);
                indices.push_back(i + 1);
            }
        }
    }

    mesh->InitFromData(vertices, indices);
    return mesh;
}
Mesh* object2D::CreateEllipse(
    const std::string& name,
    glm::vec3 center,
    float radiusX,
    float radiusY,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    const int segments = 100;

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = center.x + radiusX * cos(angle);
        float y = center.y + radiusY * sin(angle);
        vertices.emplace_back(glm::vec3(x, y, center.z), color);
        indices.push_back(i);
    }

    indices.push_back(0);

    Mesh* ellipse = new Mesh(name);

    if (fill) {
        std::vector<unsigned int> fillIndices;
        fillIndices.push_back(segments + 1);
        for (int i = 0; i <= segments; ++i) {
            fillIndices.push_back(i);
        }

        vertices.emplace_back(center, color);
        ellipse->InitFromData(vertices, fillIndices);
    }
    else {
        ellipse->SetDrawMode(GL_LINE_LOOP);
        ellipse->InitFromData(vertices, indices);
    }

    return ellipse;
}