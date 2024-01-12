#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <vector>
#include <string>
#include <DirectXCollision.h>

#include "Util.h"
#include "Object.h"

using Microsoft::WRL::ComPtr;
class Geometry : public Object
{
public:
    virtual void LoadGeometry(const float scale = 1.0f) = 0;
};

class Triangle : public Geometry
{
public:
    void LoadGeometry(const float scale = 1.0f) override;
};

class Square : public Geometry
{
public:
    void LoadGeometry(const float scale = 1.0f) override;
};

class Cube : public Geometry
{
public:
    void LoadGeometry(const float scale = 1.0f) override;
};

class Sphere : public Geometry
{
public:
    void LoadGeometry(const float scale = 1.0f) override;
};

class EnvMap 
{
private:
    Mesh* m_mesh;

    ComPtr<ID3D11ShaderResourceView> m_envSRV;
    // For IBL
    ComPtr<ID3D11ShaderResourceView> m_irradianceSRV;
    ComPtr<ID3D11ShaderResourceView> m_specularSRV;
    ComPtr<ID3D11ShaderResourceView> m_brdfLookUpSRV;

public:
    void Init(ComPtr<ID3D11Device>& device, const std::wstring filepath);
    void Render(ComPtr<ID3D11DeviceContext>& context);
    void SetIBLSRVs(ComPtr<ID3D11DeviceContext>& context);
    ~EnvMap();
};