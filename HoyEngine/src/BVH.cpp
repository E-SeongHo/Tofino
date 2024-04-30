
#include <queue>
#include "BVH.h"

namespace Tofino
{
	void BVH::Insert(const AABB& box)
	{
		// Stage 1: find the best sibling for the new leaf (SAH : Surface Area Heuristic)
		// Stage 2: create a new parent(internal node)
		// Stage 3: walk back up the tree refitting AABBs (refitting)
	}

	float BVH::CalculateCost(Node* subTree) const
	{
		float cost = 0.0f;
		std::queue<Node*> queue;

		queue.push(subTree);

		// BFS sub tree
		while(!queue.empty())
		{
			Node* current = queue.front();
			queue.pop();


		}
		return 0.0f;

	}
}
