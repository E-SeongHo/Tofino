#include <vector>
#include <string>
#include <imgui.h>

#include <Camera.h>
#include <Object.h>
#include <Helper.h>
#include <Mesh.h>
#include <MeshLoader.h>

#include "Cosmos.h"
#include "BattleScene.h"
#include "Graphics.h"
#include "Meteor.h"

using namespace Tofino;
Cosmos::Cosmos()
{   // Must Use Engine API only After Applcation::Init() Called!!! 
    assert(m_scenes.empty());
}

Cosmos::~Cosmos()
{
    Application::~Application();
}   

bool Cosmos::Init()
{
    if (!Application::Init()) return false;

    DesignScenes();

    for(const auto& scene : m_scenes)
    {
        scene->Init();
    }

    SetCurrentScene(m_scenes[0].get());

    return true;
}

void Cosmos::DesignScenes()
{
    m_scenes.push_back(std::make_unique<BattleScene>());
}

void Cosmos::Update(float deltaTime)
{
    Application::Update(deltaTime);
    
}

void Cosmos::RenderGUI()
{
    ImGui::NewFrame();
    ImGui::Begin("Scene Control");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Rendering Camera"))
    {
        Vector3 origin = m_editorCamera->GetOrigin();
        Vector3 direction = m_editorCamera->GetDirection();

        ImGui::SeparatorText("Origin");
        ImGui::Text("X: (%d) Y: (%d), Z: (%d)", (int)origin.x, (int)origin.y, (int)origin.z);
        ImGui::SeparatorText("Direction");
        ImGui::Text("X: (%.2f) Y: (%.2f), Z: (%.2f)", direction.x, direction.y, direction.z);
        ImGui::Text(" ");
    }

    /*if (ImGui::CollapsingHeader("Rendering Options"))
    {
        ImGui::Checkbox("Show WireFrame", &m_wireRendering);
    }*/

    /*if (ImGui::CollapsingHeader("Light"))
    {
        ImGui::SliderFloat3("Origin", &m_globalConstBufferCPU.light.pos.x, -200.0f, 200.0f);
        ImGui::SliderFloat("Strength", &m_globalConstBufferCPU.light.strength, 0.0f, 10.0f);
        ImGui::SliderFloat3("Direction", &m_globalConstBufferCPU.light.direction.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Coefficient", &m_globalConstBufferCPU.light.coefficient, 0.0f, 10.0f);
    }*/

    // Setting Objects
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::CollapsingHeader("Scene Objects"))
    {
        for (auto& object : m_currentScene->GetAllSceneObjects())
        {
            object->RenderGUI();
        }
    }

    ImGui::End();
}

Application* CreateApplication()
{
    return new Cosmos();
}
