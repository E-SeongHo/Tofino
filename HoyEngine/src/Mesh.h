#pragma once

#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Tofino
{
    using Microsoft::WRL::ComPtr;
    class Mesh
    {
    public:
        Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

        void Init(ComPtr<ID3D11Device>& device);
        void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
        void Bind(ComPtr<ID3D11DeviceContext>& context) const;

        bool IsUpdateFlagSet() const    { return m_updateFlag; }
        void SetUpdateFlag(bool flag)   { m_updateFlag = flag; }

        uint32_t GetIndexCount() const  { return m_indexBuffer.GetData().size(); }

        VertexBuffer& GetVertexBuffer()             { return m_vertexBuffer;}
        const IndexBuffer& GetIndexBuffer() const   { return m_indexBuffer; }
        Material& GetMaterial()                     { return m_material;    }

        void SetMaterialFactors(Vector4 baseColor, const float roughness, const float metallic);

    private:
        VertexBuffer m_vertexBuffer;
        IndexBuffer m_indexBuffer;

        Material m_material;
        bool m_updateFlag = false;
    };
}