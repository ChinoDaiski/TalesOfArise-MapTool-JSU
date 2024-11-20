#include "stdafx.h"
#include "..\Public\\UI_Manager.h"
#include "GameObject.h"
#include "Layer.h"
#include "GameInstance.h"
#include"Alphen.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

void CUI_Manager::pushUI(LEVELUI lv, UIKIND UIKind, void * pArg)
{
	//UI레벨과 종류에 맞게 넣어주면. 
	UIMap.insert(make_pair<>(lv, UIKind));
	int a = 10;
}

CUI_Manager::PLAYERINFO& CUI_Manager::GetPlayerInfo(UIPLAYER player)
{
	if (player == CUI_Manager::UIPLAYER::PLAYER_ALPHEN) {

		PlayerInfo.iMaxHp = dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iMaxHp;
		PlayerInfo.iComboHits = dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iComboHits;
		PlayerInfo.iCurrentAg = dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iCurrentAg;
		PlayerInfo.iCurrentCp = dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iCurrentCp;
		PlayerInfo.iCurrentHp = dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iCurrentHp;
		PlayerInfo.iMaxAg = dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iMaxAg;
		PlayerInfo.iMaxCp = dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iMaxCp;
		PlayerInfo.iMaxHp = dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iMaxHp;

	}

	return PlayerInfo;
	
}

void CUI_Manager::SetPlayerInfo( UIPLAYER player, PLAYERINFO& playerinfo)
{
	InfoMap.insert(make_pair<>(player, &playerinfo));	//받아온 플레이어의 주소를 던지자

}

void CUI_Manager::SetPlayer(UIPLAYER player, CGameObject* gameObject)
{

	if (player == PLAYER_ALPHEN)
		PlayerAlphen = gameObject;
	if (player == PLAYER_SION)
		PlayerSion = gameObject;



}

CGameObject* CUI_Manager::GetPlayer(UIPLAYER player)
{


	if (player == UIPLAYER::PLAYER_ALPHEN)
		return PlayerAlphen;
	else if (player == UIPLAYER::PLAYER_SION)
		return PlayerSion;
	else if (player == UIPLAYER::PLAYER_RINWELL)
		return PlayerRinwell;
	else if (player == UIPLAYER::PLAYRE_KAKURA)
		return PlayerKakura;
	else
		return nullptr;
}

int CUI_Manager::GetAlphenInfo()
{ 
	return dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iComboHits; 
}

void CUI_Manager::SetAlphenCombo(_uint combohit)
{

	dynamic_cast<CAlphen*>(PlayerAlphen)->PlayerInfo.iComboHits = combohit;
}



void CUI_Manager::Free()
{
	//for (_uint i = 0; i < m_iNumLevels; ++i)
	//{
	//	for (auto& Pair : m_pLayers[i])
	//		Safe_Release(Pair.second);
	//	m_pLayers[i].clear();
	//}
	//Safe_Delete_Array(m_pLayers);

	//for (auto& Pair : m_Prototypes)
	//	Safe_Release(Pair.second);

	//m_Prototypes.clear();
}
