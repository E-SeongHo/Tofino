
#include <vector>

#include "Graphics.h"
#include "Collider.h"
#include "AABB.h"

namespace Tofino
{
    Collider::Collider(const Vector3& lowerBound, const Vector3& upperBound)
        : m_boundingBox(Vector3(-1.0f), Vector3(1.0f)),
          m_constBuffer(ConstantBuffer<ColliderBuffer>(VERTEX_SHADER | PIXEL_SHADER, 0))
	{
		// Even though it only needs position of Vertex, just simply use basicInputLayout
		std::vector<Vertex> vertices;

        std::vector<Vector3> positions;

        Vector3 scale = (upperBound - lowerBound) * 0.5f; 
		// PRIMITIVE_TOPOLOGY_LINELIST
        // topside lines
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);

        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);

        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);

        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);

        // downside lines
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);

        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);

        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);

        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);

        // y-axis lines
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);

        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);

        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);

        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    	

        for(size_t i = 0; i < positions.size(); i++)
        {
            Vertex v;
            v.pos = positions[i];

            vertices.push_back(v);
        }

        m_vertexBuffer = VertexBuffer(vertices);

        m_vertexBuffer.Init(RendererDevice);
        m_constBuffer.Init(RendererDevice);
	}

    void Collider::Translate(const Vector3& translation)
    {
        Matrix m = m_constBuffer.GetData().world.Transpose();
        m.Translation(translation);
        m_constBuffer.GetData().world = m.Transpose();

        m_boundingBox.m_lowerBound += translation;
        m_boundingBox.m_upperBound += translation;
    }

    void Collider::Scale(const Vector3& scale)
    {
        //float maxScale = std::max(std::max(scale.x, scale.y), scale.z);

        Matrix m = m_constBuffer.GetData().world.Transpose();
        m._11 = scale.x;
        m._22 = scale.y;
        m._33 = scale.z;
        m_constBuffer.GetData().world = m.Transpose();

        m_boundingBox.m_lowerBound = Vector3::Transform(Vector3(-1.0f), Matrix::CreateScale(scale));
        m_boundingBox.m_upperBound = Vector3::Transform(Vector3(1.0f), Matrix::CreateScale(scale));
    }

    void Collider::UpdateConstBuffer(ComPtr<ID3D11DeviceContext>& context)
    {
        static constexpr Color red = Color(1.0f, 0.0f, 0.0f, 1.0f);
        static constexpr Color green = Color(0.0f, 1.0f, 0.0f, 1.0f);

    	m_constBuffer.GetData().color = m_isColliding ? red : green;
    	m_constBuffer.Update(context);

        m_isColliding = false;
    }

    bool Collider::CheckCollision(const Collider& other) const
    {
        if (this == &other) return false;

        Vector3 aMin = m_boundingBox.m_lowerBound;
        Vector3 aMax = m_boundingBox.m_upperBound;

        Vector3 bMin = other.m_boundingBox.m_lowerBound;
        Vector3 bMax = other.m_boundingBox.m_upperBound;

        //std::cout << aMin.x << " " << aMin.y << " " << aMin.z << std::endl;
        //std::cout << aMax.x << " " << aMax.y << " " << aMax.z << std::endl;

        //std::cout << bMin.x << " " << bMin.y << " " << bMin.z << std::endl;
        //std::cout << bMax.x << " " << bMax.y << " " << bMax.z << std::endl;

        if (aMax.x < bMin.x || aMin.x > bMax.x) return false;
        if (aMax.y < bMin.y || aMin.y > bMax.y) return false;
        if (aMax.z < bMin.z || aMin.z > bMax.z) return false;
        return true;
    }

    void Collider::OnCollisionDetected()
    {
        m_isColliding = true;

        // m_collisionFunction();
    }
}
