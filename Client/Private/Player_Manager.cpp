#include "stdafx.h"
#include "..\Public\Player_Manager.h"
#include "GameInstance.h"
#include "Player.h"
#include"Camera_Default.h"
IMPLEMENT_SINGLETON(CPlayer_Manager)


CPlayer_Manager::CPlayer_Manager()
{
}



void CPlayer_Manager::Set_MainPlayer(_uint iIndex)
{
	if (iIndex >= m_vecPlayers.size())
		return;

	for (_uint i = 0; i < m_vecPlayers.size(); ++i)
	{
		if (i == iIndex)
		{
			m_vecPlayers[i]->Set_Auto(false);
			continue;
		}
		m_vecPlayers[i]->Set_Auto(true);
	}
	m_iMainPlayerIndex = iIndex;
	m_pMainPlayer = m_vecPlayers[iIndex];
	m_pCamera->Set_PlayerIndex(m_iMainPlayerIndex);


}

void CPlayer_Manager::Set_FieldPlayer(_uint iIndex)
{


	if (iIndex >= m_vecPlayers.size())
		return;

	m_pFieldPlayer = m_vecPlayers[iIndex];


}


void CPlayer_Manager::Free()
{

}