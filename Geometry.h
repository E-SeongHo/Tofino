#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <vector>
#include <string>
#include <DirectXCollision.h>

#include "Util.h"
#include "Mesh.h"

using Microsoft::WRL::ComPtr;
class Geometry : public Mesh
{
public:
    virtual void Init(const float scale = 1.0f, bool isHittable = false) = 0;
    void ReverseIndices();
};

class Triangle : public Geometry
{
public:
    void Init(const float scale = 1.0f, bool isHittable = false) override;
};

class Square : public Geometry
{
public:
    void Init(const float scale = 1.0f, bool isHittable = false) override;
};

class Cube : public Geometry
{
public:
    void Init(const float scale = 1.0f, bool isHittable = false) override;
};

class Sphere : public Geometry
{
public:
    void Init(const float scale = 1.0f, bool isHittable = false) override;
};

class EnvMap 
{
private:
    Geometry* m_mesh = nullptr;

    ComPtr<ID3D11ShaderResourceView> m_envSRV;
    // For IBL
    ComPtr<ID3D11ShaderResourceView> m_irradianceSRV;
    ComPtr<ID3D11ShaderResourceView> m_specularSRV;
    ComPtr<ID3D11ShaderResourceView> m_brdfLookUpSRV;

public:
    void Init(ComPtr<ID3D11Device>& device, const std::wstring filepath);
    void Render(ComPtr<ID3D11DeviceContext>& context);
    ~EnvMap();
};