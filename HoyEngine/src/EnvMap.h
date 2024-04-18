#pragma once

#include <d3d11.h>
#include <wrl.h> 
#include <string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

namespace Tofino
{
    class Geometry;

    using Microsoft::WRL::ComPtr;

    class EnvMap
    {
    public:
        EnvMap(const std::string name, const std::string filepath);
        ~EnvMap() = default;

        void Init(ComPtr<ID3D11Device>& device);
        void Bind(ComPtr<ID3D11DeviceContext>& context) const;
        void BindIBLSRVs(ComPtr<ID3D11DeviceContext>& context) const;
        uint32_t GetIndexCount() const { return m_indexBuffer.GetData().size(); }

    private:
        std::string m_name;
        std::string m_filepath;

        VertexBuffer m_vertexBuffer;
        IndexBuffer m_indexBuffer;

        Texture m_envMap;
        // IBL
        Texture m_irradianceMap;
        Texture m_specularMap;
        Texture m_brdfLUT;
    };
}