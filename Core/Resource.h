#pragma once
#include <string>
#include <unordered_map>
	
class Resource
{
	friend class ResourceManager;
protected:
	virtual void LoadResource(std::wstring filepath) = 0;	 // will allocate memory and fill it with resource
	virtual void UnloadResource() = 0;	// free allocated memory
};

