## Features

1. _**HDR Rendering Pipeline**_
2. _**Entity-Component-System**_
3. _**Collision Detection**_
4. _**GPU Instancing**_
5. _**Image Based Lighting**_
6. _**Import 3D models using assimp**_
7. _**Edit Mode & Traversal Mode(First Person View)**_
8. _**GUI connected**_


## Get Started

### 1. Create Application 

1. Create a project, and define the `CreateApplication()` function

Entry point is as follows (main.cpp)

``` cpp

// Cosmos.h

class Cosmos : public Tofino::Application
{
public:
	Cosmos();
	~Cosmos() override;

	bool Init() override;
	void Update(float deltaTime) override;
	void RenderGUI() override;

private:
	void DesignScenes();

private:
	std::vector<std::unique_ptr<Tofino::Scene>> m_scenes;
};


// Cosmos.cpp

Application* CreateApplication()
{
    return new Cosmos();
}

```

**Note:** you must use the Engine API only after `Application::Init()` has been called


2. Set Scene to render

```cpp

class BattleScene : public Tofino::Scene
{
	BattleScene();
	void Update(float deltaTime) override;
}

// Cosmos.cpp

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

```

### 2. Application Modes

Tofino offers 3 application modes

1. Edit Mode ( `E` key )
2. First Person Traversal Mode ( `F` key )
3. Play Mode ( `P` key )

### 3. Create Object

1. Define Tofino::Object base class in your project  

**Note:** Initialize parent class (Object) appropriately

``` cpp

// SpaceStatin.h

#include <Object.h>

class SpaceStation : public Tofino::Object
{
public:
	SpaceStation(Tofino::Scene* scene, const std::string name = "Untitled", const bool isHittable = true);
	~SpaceStation() override;

	// Called once when scene init
	void Init(ComPtr<ID3D11Device>& device) override;

	// Called every frame
	void Update(float deltaTime) override;

	void OnCollisionDetected(Tofino::Collision& collision) override;
};

```

2. Create Object in two ways 

``` cpp
// Scene.h

T_OBJECT& CreateObject(const std::string& name, bool instancing = false);

InstanceGroup& CreateObjectInstances(MeshComponent sharedMeshComponent, std::string groupName, int numInstances);


// 1. Create a single Object
scene->CreateObject<SpaceStation>("name");


// 2. Create a instance group
scene->CreateObjectInstancs<SpaceStation>(mesh, "station instanced", 1000);

```










