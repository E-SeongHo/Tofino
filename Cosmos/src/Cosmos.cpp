#include <vector>
#include <string>
#include <imgui.h>

#include "Cosmos.h"
#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Helper.h"
#include "EnvMap.h"
#include "Mesh.h"
#include "Graphics.h"
#include "MeshLoader.h"

using namespace Tofino;
Cosmos::Cosmos()
{
    m_scene = new Scene();
}

Cosmos::~Cosmos()
{
    Application::~Application();
    delete m_scene;
}   

bool Cosmos::Init()
{
    if (!Application::Init()) return false;

    DesignScenes();
    m_scene->Init();

    SetCurrentScene(m_scene);

    Object* obj = m_scene->GetAllSceneObjects()[0];
    return true;
}

void Cosmos::DesignScenes()
{
    Camera* cam = new Camera();
    cam->SetAspect(static_cast<float>(m_width) / m_height);

    m_scene->SetCamera(cam);

    /*{
        for(int k = 0; k < 20; k++)
			for(int i = 0; i < 20; i++)
				for(int j = 0; j < 50; j++)
				{
	                Vector3 Location = { (j-25) * 2.0f, (i-10) * 5.0f, (k-10) * 2.0f };
	                Object* obj = m_scene->CreateEmptyObject("ECS Object" + std::to_string((k * 50 * 20) + (i*50) + j));
	                obj->AddComponent<MeshComponent>(MeshComponent({ MeshLoader::LoadSphere() }));
	                obj->AddComponent<TransformComponent>(TransformComponent());
	                obj->GetComponent<TransformComponent>().Translation = Location;
	                obj->SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.5f);
                }
    }*/

    {
        Object* spaceship = m_scene->CreateEmptyObject("Space station");
        std::string directory = "D:/Workspace/3DModels/sci-fi-space-station/";

    	spaceship->AddComponent<TransformComponent>(TransformComponent());
        spaceship->AddComponent<MeshComponent>(MeshComponent(
            { MeshLoader::LoadModel(directory + "SpaceStation.fbx") })
            );

        // Since Assimp failed to find appropriate textures
        {
            Texture bodyAlbedo2(
                RendererDevice,directory + "SpaceStationParts2_BaseColor.png",
                TextureType::ALBEDO);

        	spaceship->GetComponent<MeshComponent>().Meshes[0].GetMaterial().SetAlbedoMap(bodyAlbedo2);
        }

        spaceship->SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.7f);
        spaceship->GetComponent<MeshComponent>().Meshes[0].SetMaterialFactors(Vector4(0.0f), 0.1, 0.9f);

        spaceship->GetComponent<TransformComponent>().Translation = Vector3(0.0f , 0.0f, 50.0f);
        spaceship->GetComponent<TransformComponent>().Scale = Vector3(0.005f);
    }


    {
        Object* aircraft = m_scene->CreateEmptyObject("E-45 Aircraft");
        std::string directory = "D:/Workspace/3DModels/E-45-Aircraft/";

        aircraft->AddComponent<TransformComponent>(TransformComponent());
        aircraft->AddComponent<MeshComponent>(MeshComponent(
            { MeshLoader::LoadModel(directory + "E 45 Aircraft_obj.obj") })
            );

        // Since Assimp failed to find appropriate textures
        {
            Texture bodyAlbedo(RendererDevice, directory + "E-45 _col.jpg", TextureType::ALBEDO);
            Texture bodyNormal(RendererDevice, directory + "E-45-nor_1.jpg", TextureType::NORMAL);

            Texture glassNormal(RendererDevice, directory + "E-45_glass_nor_.jpg", TextureType::NORMAL);

            // body
            aircraft->GetComponent<MeshComponent>().Meshes[0].GetMaterial().SetAlbedoMap(bodyAlbedo);
            aircraft->GetComponent<MeshComponent>().Meshes[0].GetMaterial().SetNormalMap(bodyNormal);
            aircraft->GetComponent<MeshComponent>().Meshes[0].GetMaterial().GetMaterialStatus().hasHeightMap = 0;

            // glass
            aircraft->GetComponent<MeshComponent>().Meshes[1].GetMaterial().GetMaterialStatus().hasAlbedoMap = 0;
            aircraft->GetComponent<MeshComponent>().Meshes[1].GetMaterial().SetNormalMap(glassNormal);
            aircraft->GetComponent<MeshComponent>().Meshes[1].GetMaterial().GetMaterialStatus().hasHeightMap = 0;
        }

        aircraft->SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.7f);
        aircraft->GetComponent<TransformComponent>().Scale = Vector3(3.0f);
    }

    {
        EnvMap* skybox = new EnvMap("Universe", "./Assets/Cubemap/HDRI/PlanetaryEarth/");
        m_scene->AddSkybox(skybox);
    }

    {
        Light* light = new Light();
        //light->type = LightType::DIRECTIONAL_LIGHT;
        light->pos = Vector3(-0.3f, 0.2f, -2.0f);
        light->strength = 0.5f;
        light->direction = Vector3(0.1f, -0.2f, 1.0f);
        light->coefficient = 2.2f;
        m_scene->AddLight(light);
    }
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
    if (ImGui::CollapsingHeader("Redering Camera"))
    {
        Vector3 origin = m_scene->GetCamera()->GetOrigin();
        Vector3 direction = m_scene->GetCamera()->GetDirection();

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
    ImGuiColorEditFlags flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel
        | ImGuiColorEditFlags_AlphaBar;

    if (ImGui::CollapsingHeader("Scene Objects"))
    {
        for (auto& object : m_scene->GetAllSceneObjects())
        {
            int updateFlag = 0;
            if (ImGui::TreeNode(object->GetName().c_str()))
            {
                auto& transform = object->GetComponent<TransformComponent>();

                updateFlag += ImGui::SliderFloat3("Position", &transform.Translation.x, -100.0f, 100.0f);
                updateFlag += ImGui::SliderFloat3("Rotation", &transform.Rotation.x, -360.0f, 360.0f);
                updateFlag += ImGui::SliderFloat3("Scale", &transform.Scale.x, 0.0f, 10.0f);
                
                updateFlag += ImGui::CheckboxFlags("Albedo Map", &object->GetConstBuffer().GetData().activeAlbedoMap, 1);
                updateFlag += ImGui::CheckboxFlags("Height Map", &object->GetConstBuffer().GetData().activeHeightMap, 1);
                updateFlag += ImGui::CheckboxFlags("Normal Map", &object->GetConstBuffer().GetData().activeNormalMap, 1);

                if (updateFlag) object->SetUpdateFlag(true);

                std::vector<Mesh>& meshes = object->GetComponent<MeshComponent>().Meshes;
                for (int i = 0; i < meshes.size(); i++)
                {
                    int meshUpdateFlag = 0;
                    if (ImGui::TreeNode((std::string("Part ") + std::to_string(i)).c_str()))
                    {
                        auto& materialData = meshes[i].GetMaterial().GetMaterialStatus();
                        if (!object->GetConstBuffer().GetData().activeAlbedoMap)
                        {
                            meshUpdateFlag += ImGui::ColorPicker4("BaseColor", &materialData.baseColor.x, flags, NULL);
                        }
                        meshUpdateFlag += ImGui::SliderFloat("Roughness", &materialData.roughness, 0.0f, 1.0f);
                        meshUpdateFlag += ImGui::SliderFloat("Metaillic", &materialData.metallic, 0.0f, 1.0f);
                        
                        ImGui::TreePop();
                    }

                    if (meshUpdateFlag) meshes[i].SetUpdateFlag(true);
                }
                ImGui::TreePop();
            }
        }
    }

    ImGui::End();
}

Application* CreateApplication()
{
    return new Cosmos();
}
