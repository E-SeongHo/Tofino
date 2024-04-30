#pragma once

#include "AABB.h"

namespace Tofino
{
	// GDC2019 Box2D Dynamic BVH 
	class BVH 
	{
	public:
		struct Node
		{
			AABB box;
			int index;
			int parent;
			int left;
			int right;
			bool isLeaf;
		};

		BVH() = default;

		void Insert(const AABB& box);

	private:
		float CalculateCost(Node* subTree) const;

	private:
		std::unique_ptr<Node> m_root;
		std::vector<Node*> m_nodes;
		int m_count = 0;
	};

}
