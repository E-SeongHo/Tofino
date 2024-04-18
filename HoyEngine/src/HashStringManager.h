#pragma once

#include <string>
#include <unordered_map>
#include <cassert>

// Generate object's ID using hash function

namespace Tofino
{
	using ObjectID = uint32_t;

	class HashStringManager
	{
	public:
		static HashStringManager& GetInstance()
		{
			static HashStringManager s;
			return s;
		}
		~HashStringManager() = default;
		HashStringManager(const HashStringManager& other) = delete;
		HashStringManager& operator=(const HashStringManager& other) = delete;

		// x65599 hash function
		static ObjectID GenerateHash(const std::string& str)
		{
			assert(GetInstance().m_idTable.find(str) == GetInstance().m_idTable.end());

			uint32_t hash = 0;
			for (auto c : str)
			{
				hash = 65599 * hash + c;
			}

			hash = hash ^ (hash >> 16); 
			GetInstance().m_idTable[str] = hash;

			return hash;
		}

		static ObjectID GetID(const std::string& str)
		{
			assert(GetInstance().m_idTable.find(str) != GetInstance().m_idTable.end());

			return GetInstance().m_idTable[str];
		}

		static bool IsAvailableName(const std::string& str)
		{
			return GetInstance().m_idTable.find(str) == GetInstance().m_idTable.end();
		}

	private:
		HashStringManager() = default;

	private:
		std::unordered_map<std::string, ObjectID> m_idTable;
	};
}