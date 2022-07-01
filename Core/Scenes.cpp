#include "Scenes.h"

Scenes::Scenes(Graphics* pGfx) : m_pGfx(pGfx)
{}

Scene* Scenes::CreateScene(std::wstring szLevelName)
{
	Scene* pNewScene = new Scene(m_pGfx);
	m_sceneList[szLevelName] = pNewScene;
	return pNewScene;
}

Scene* Scenes::GetScene(std::wstring szLevelName)
{
	return m_sceneList[szLevelName];
}

void Scenes::DeleteScene(std::wstring szLevelName)
{
	auto pScene = m_sceneList[szLevelName];
	if (pScene == NULL)
		return;
	if (m_pCurrentScene == pScene)
		m_pCurrentScene = NULL;
	delete pScene;
	m_sceneList.erase(m_sceneList.find(szLevelName));
}

void Scenes::SetCurrentLevel(std::wstring szLevelName)
{
	auto pScene = m_sceneList[szLevelName];
	if (pScene != NULL)
		m_pCurrentScene = pScene;
}
