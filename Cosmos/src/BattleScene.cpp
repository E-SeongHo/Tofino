
#include <EnvMap.h>

#include "BattleScene.h"
#include "Meteor.h"
#include "Aircraft.h"
#include "MeshLoader.h"
#include "SpaceStation.h"

using namespace Tofino;

BattleScene::BattleScene()
{
    //Scene();
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

    //Meteor& obj = m_scene->CreateObject<Meteor>("Lava Rock");

    // Meteors
    m_meteorGenerator.GenerateMeteors(this);

    SpaceStation& spaceStation = CreateObject<SpaceStation>("Space Station");

    /*Object& obj = CreateObject<Object>("test cube");
    obj.AddComponent<MeshComponent>(MeshComponent{ MeshLoader::LoadCube() });*/

    Aircraft& aircraft = CreateObject<Aircraft>("E-45 Aircraft");
    SetMainObject(aircraft);

    EnvMap* skybox = new EnvMap("Universe", "./Assets/Cubemap/HDRI/PlanetaryEarth/");
    AddSkybox(skybox);

    {
        Light* light = new Light();
        //light->type = LightType::DIRECTIONAL_LIGHT;
        light->pos = Vector3(-0.3f, 0.2f, -2.0f);
        light->strength = 0.5f;
        light->direction = Vector3(0.1f, -0.2f, 1.0f);
        light->coefficient = 2.2f;
        AddLight(light);
    }
}

void BattleScene::Update(float deltaTime)
{
	Scene::Update(deltaTime);

    static float timeElapsed = 0.0f;

    if (m_play)
    {
        timeElapsed += deltaTime;

        if (timeElapsed > 20.0f)
        {
            m_meteorGenerator.ShootMeteors();
            timeElapsed = 0.0f;
        }
    }

}
