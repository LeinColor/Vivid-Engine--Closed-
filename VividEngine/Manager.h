#pragma once

#include <vector>
#include <unordered_map>

template<typename T>
class Manager {
private:
	std::vector<T*> manager;
	std::unordered_map<uint32_t, size_t> lookup;

public:
	T* At(size_t index)
	{
		return manager[index];
	}
	void Add(T* item)
	{
		manager.push_back(item);
		//lookup.insert(std::make_pair(item->ID, manager.size() - 1));
	}
	const size_t GetSize() const
	{
		return manager.size();
	}
};