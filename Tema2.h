#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/camera1.h"
#include <components/text_renderer.h>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    public:
        void FrameStart() override;

        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1), const int hp = 0, const int type = -1);

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        implemented::Camera1* camera1;
        implemented::Camera1* camera2;

        glm::mat4 projectionMatrix;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        int nr_hit;

        std::vector<Mesh*> tank; // 0 = corp, 1 = turela, 2 = tun, 3 = senile, 4 = proiectil
        glm::vec3 standard_pos[6];
        float rotate_turela;
        int shoot = 0; // shoot = 1 = trag prima data
                       // shoot = 2 = sunt in curs de tragere

        float tank_rotate;
        glm::vec3 cameraPos, cameraFront, cameraUp;

        int ok = 0;

        float update_bullet = 0;    // miscare proiectil fata de positia initiala de pornire
        float old_rotate_tank;
        float old_rotate_turela;
        glm::vec3 my_bullet_pos;
        float my_time;

        int random;
        std::vector<glm::vec3> enemy_positions;
        std::vector<glm::vec3> enemy_colors;
        std::vector<int> enemy_nr_hits;
        std::vector<int> enemy_is_shooting;
        std::vector<float> enemy_tank_rotate;
        std::vector<float> enemy_turela_rotate;
        std::vector<float> enemy_update_bullet;
        std::vector<glm::vec3> enemy_start_shoot;
        std::vector<float> enemy_old_rotate_tank;
        std::vector<float> enemy_old_rotate_turela;

        std::vector<float> enemy_time;

        std::vector<glm::vec3> enemy_bullet_pos;
        std::vector<int> choice;

        std::vector<float> time_to_choose;

        int points = 0;

        float global_time = 0;
        int finish = 0;

        int random2;
        std::vector<glm::vec3> buildings_position;
        std::vector<glm::vec3> buildings_size;
    };
}   // namespace m1
