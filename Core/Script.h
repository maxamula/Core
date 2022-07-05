#pragma once
#include "GameObject.h"

class Script;

struct ScriptComponent
{
	Script* pScript;
};

class Script
{
public:
	virtual void Start(GameObject object) = 0;
	virtual void Update(float fElapsedTime, GameObject object) = 0;
};

