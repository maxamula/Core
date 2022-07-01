#pragma once
#include "GameObject.h"

class Script;

struct ScriptComponent
{
	Script* script;
};

class Script
{
public:
	Script(GameObject* pObj) : This(pObj)
	{}
	virtual void Start() = 0;
	virtual void Update(float fDeltaTime) = 0;
protected:
	GameObject* This = 0;
};

