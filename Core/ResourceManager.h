#pragma once
#include "Resource.h"
#include <string>
#include <unordered_map>

// singleton
class ResourceManager
{
public:
	template <typename T>
	T* Get(std::wstring path)
	{
		// search for existing resource 
		for (auto [key, value] : resources)
		{
			if (key == path)
			{
				return dynamic_cast<T*>(value);					// Return resource
			}			
		}
		// if resource was not loaded
		Resource* resource = dynamic_cast<Resource*>(new T);	// Create new resource
		resource->LoadResource(path);							// Load
		resources[path] = resource;								// Save loaded resource
		return dynamic_cast<T*>(resource);
	}

	void Release(std::wstring path)
	{
		for (auto [key, value] : resources)
		{
			if (key == path)
			{
				value->UnloadResource();				// Free resource memory
				resources.erase(resources.find(key));	// Remove resource from loaded resources
				delete value;							// Free resource object memory
			}
		}
	}
private:
	std::unordered_map<std::wstring, Resource*> resources;
};

