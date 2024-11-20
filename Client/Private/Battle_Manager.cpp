#include "stdafx.h"
#include "..\Public\Battle_Manager.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Enemy.h"
#include "Player_Manager.h"
#include "Player.h"



IMPLEMENT_SINGLETON(CBattle_Manager)


CBattle_Manager::CBattle_Manager()
{
}



HRESULT CBattle_Manager::NativeConstruct()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	m_pMonsterLayer = pGameInstance->Add_Layer(LEVEL_STATIC, TEXT("Monster_Battle_Layer"));

	Safe_Release(pGameInstance);

	return S_OK;
}


void CBattle_Manager::Battle_Enter(CEnemy* pEnemy)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CPlayer_Manager* pPlayerManger = CPlayer_Manager::GetInstance();
	Safe_AddRef(pGameInstance);
	Safe_AddRef(pPlayerManger);





	CPlayer* pFieldPlayer = pPlayerManger->Get_FieldPlayer();


	_vector vFieldPlayerPos = pFieldPlayer->Get_PlayerPos();

	XMStoreFloat4(&m_vPrePlayerPos, vFieldPlayerPos);

	if (m_pMonsterLayer == nullptr)
		m_pMonsterLayer = pGameInstance->Add_Layer(LEVEL_STATIC, TEXT("Monster_Battle_Layer"));


	if (m_pPlayerLayer == nullptr)
		m_pPlayerLayer = pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Player"));


	_vector vPos[3];
	vPos[0] = XMVectorSet(5.f, 0.f, 10.f, 1.f);
	vPos[1] = XMVectorSet(10.f, 0.f, 10.f, 1.f);
	vPos[2] = XMVectorSet(15.f, 0.f, 10.f, 1.f);


	for (_uint i = 0; i < 3; ++i)
	{
		CEnemy* pEnemyClone = (CEnemy*)pEnemy->Clone(&vPos[i]);
		pEnemyClone->Set_Battle(true);
		m_pMonsterLayer->Add_GameObject(pEnemyClone);

		m_vecMonsters.push_back(pEnemyClone);
	}


	vPos[0] = XMVectorSet(5.f, 0.f, 2.f, 1.f);
	vPos[1] = XMVectorSet(10.f, 0.f, 2.f, 1.f);
	vPos[2] = XMVectorSet(15.f, 0.f, 2.f, 1.f);

	_uint iCnt = 0;

	vector<CEnemy*>::iterator iter = m_vecMonsters.begin();
	for (auto pPlayer : *pPlayerManger->Get_VecPlayers())
	{

		pPlayer->Set_VecMonstersIter(iter);
		pPlayer->Set_Battle(true);
		pPlayer->Set_EquipmentBattle(true);
		pPlayer->Set_PlayerPos(vPos[iCnt++]);
		pPlayer->Set_VecMonsters(&m_vecMonsters);
		++iter;
		if (iter == m_vecMonsters.end())
		{
			iter = m_vecMonsters.begin();
		}
	}

	m_bBattle = true;
	m_pVecPlayers = pPlayerManger->Get_VecPlayers();


	Safe_Release(pGameInstance);
	Safe_Release(pPlayerManger);
}

void CBattle_Manager::Battle_End()
{

	CPlayer_Manager* pPlayerManger = CPlayer_Manager::GetInstance();
	Safe_AddRef(pPlayerManger);
	CPlayer* pFieldPlayer = pPlayerManger->Get_FieldPlayer();

	_uint iCnt = 0;
	for (auto pPlayer : *pPlayerManger->Get_VecPlayers())
	{
		pPlayer->Set_Battle(false);
		pPlayer->Set_EquipmentBattle(false);


	}

	pFieldPlayer->Set_PlayerPos(XMLoadFloat4(&m_vPrePlayerPos));



	Safe_Release(pPlayerManger);
	m_bBattle = false;
}

void CBattle_Manager::Tick(_double TimeDelta)
{
	if (m_bBattle)
	{
		CPlayer_Manager* pPlayerManger = CPlayer_Manager::GetInstance();
		Safe_AddRef(pPlayerManger);

		vector<CEnemy*>::iterator Target_Iter = pPlayerManger->Get_MainPlayer()->Get_Targeting();



		if ((*Target_Iter)->Get_Hp() <= 0)
		{
			Target_Iter++;
			pPlayerManger->Get_MainPlayer()->Set_VecMonstersIter(Target_Iter);
		}



		//몸통 부딪힘
		for (auto Player : *m_pVecPlayers)
		{
			if (Player->Get_Overlap())
				continue;
			CCollider* pPlayerCollider = (CCollider*)Player->Get_Component(TEXT("Com_SPHERE"));
			for (auto Enmey : m_vecMonsters)
			{

				CCollider* pMonsterCollider = (CCollider*)Enmey->Get_Component(TEXT("Com_SPHERE"));

				_float fDistance = 0.f;
				_vector vDir;
				if (pMonsterCollider->Collsion_Sphere(pPlayerCollider, &fDistance, &vDir))
				{
					Player->Set_PlayerPos(Player->Get_PlayerPos() + XMVectorSetY(vDir, 0.f)*fDistance);

				}

			}
		}


		//플레이어 공격  중복처리어떻게하는게 좋을까. 콜라이더 속성을 바꾸면 안되고 나자신이 알아야한다.

		for (_uint i = 0; i<(*m_pVecPlayers).size(); ++i)
		{
			CCollider* pPlayerCollider = (CCollider*)(*m_pVecPlayers)[i]->Get_Component(TEXT("Com_AttackSPHERE"));
			if (!pPlayerCollider->Get_On())
			{
				for (auto Enemy : m_vecMonsters)
				{
					switch ((PLAYER_TYPE)i)
					{
					case Client::CBattle_Manager::PLAYER_ALPHEN:
						Enemy->Set_NewCollisionAlphen(false);
						break;
					case Client::CBattle_Manager::PLAYER_SHIONNE:
						Enemy->Set_NewCollisionShionne(false);
						break;
					case Client::CBattle_Manager::PLAYER_KISARA:
						Enemy->Set_NewCollisionKisara(false);
						break;
					case Client::CBattle_Manager::PLAYER_RINWELL:
						Enemy->Set_NewCollisionRinwell(false);
						break;
					}
				}
				continue;
			}

			for (auto Enemy : m_vecMonsters)
			{
				if (Enemy->Get_Invincible())
				{
					continue;
				}

				CCollider* pMonsterCollider = (CCollider*)Enemy->Get_Component(TEXT("Com_SPHERE"));

				if (pMonsterCollider->Collsion_Sphere(pPlayerCollider))
				{
					//Enemy->Set_Collision(true);


					_uint hit = Enemy->Get_Hits();
					DAMAGEINFO temp = (*m_pVecPlayers)[i]->Get_AttackDamageInfo();



					temp.e_DamageDir = DAMAGE_DIR_FRONT;

					temp.e_PlayerType = (PLAYER_TYPE)i;

					_vector vPos = (*m_pVecPlayers)[i]->Get_PlayerPos();
					_vector vDir = XMVector3Normalize(XMVectorSetY(Enemy->Get_EnemyPos(), 0.f) - XMVectorSetY(vPos, 0.f));
					Enemy->Set_KnockBackDir(vDir);


					switch ((PLAYER_TYPE)i)
					{
					case Client::CBattle_Manager::PLAYER_ALPHEN:
						if (!Enemy->Get_NewCollisionAlphen())
						{
							Enemy->Set_NewCollisionAlphen(true);
							Enemy->Set_PowerUp(temp.fPowerUp);
							Enemy->Set_Collision(true);
							Enemy->Set_Hits();
						}
						break;
					case Client::CBattle_Manager::PLAYER_SHIONNE:
						if (!Enemy->Get_NewCollisionShionne())
						{
							Enemy->Set_NewCollisionShionne(true);
							Enemy->Set_HitDamageInfo(temp);
							Enemy->Set_Collision(true);
						};
						break;
					case Client::CBattle_Manager::PLAYER_KISARA:
						if (!Enemy->Get_NewCollisionKisara())
						{
							Enemy->Set_NewCollisionKisara(true);
							Enemy->Set_HitDamageInfo(temp);
							Enemy->Set_Collision(true);
						}
						break;
					case Client::CBattle_Manager::PLAYER_RINWELL:
						if (!Enemy->Get_NewCollisionRinwell())
						{
							Enemy->Set_NewCollisionRinwell(true);
							Enemy->Set_HitDamageInfo(temp);
							Enemy->Set_Collision(true);
						}
						break;
					}



				}

			}
		}








		//배틀 매니져에서 객체를 담을까 




		//PLAYER1에 담아져있는애들 전부 충돌처리 ->













		////스피어끼리 몸충돌 
		//for (auto Monster : m_pMonsterLayer->Get_ObjectList())
		//{
		//	CCollider* pMonsterCollider = (CCollider*)Monster->Get_Component(TEXT("Com_SPHERE"));
		//	
		//	for (auto Player : m_pPlayerLayer->Get_ObjectList())
		//	{
		//		if (((CPlayer*)Player)->Get_Overlap()) //관통가능하다면 넘김
		//			break;
		//		CCollider* pPlayerCollider = (CCollider*)Player->Get_Component(TEXT("Com_SPHERE"));
		//		
		//		_float fDistance = 0.f;
		//		if (pMonsterCollider->Collsion_Sphere(pPlayerCollider, &fDistance))
		//		{
		//			((CEnemy*)Monster)->Set_CollisionDistance(fDistance);
		//			((CEnemy*)Monster)->Set_BodyCollision(true);

		//		}
		//		else {
		//			((CEnemy*)Monster)->Set_CollisionDistance(0.f);
		//			((CEnemy*)Monster)->Set_BodyCollision(false);
		//		}

		//	}
		//}


		//어택스피어 










		Safe_Release(pPlayerManger);


		auto iter = m_vecMonsters.begin();
		for (iter; iter != m_vecMonsters.end(); ++iter)
		{
			if ((*iter)->Get_Hp() <= 0)
			{
				iter = m_vecMonsters.erase(iter);
			}
		}

		if (m_pMonsterLayer->Get_ObjectList().size() <= 0)
		{
			m_bBattle = false;
			Battle_End();
		}
	}



}

void CBattle_Manager::Free()
{

}