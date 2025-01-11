#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Lab1 : public gfxc::SimpleScene
    {
     public:
        Lab1();
        ~Lab1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // TODO(student): Class variables go here
        GLclampf red = 0;
        GLclampf green = 0;
        GLclampf blue = 0;
        GLclampf alpha = 1;
        std::string changeable{ "change" };
        short int changes{ 0 };
        Mesh* mesh1 = nullptr, * mesh2 = nullptr, * mesh3 = nullptr;

        float x_box = 1.0f, y_box = 0.5f, z_box = 0.0f;

    };
}   // namespace m1
