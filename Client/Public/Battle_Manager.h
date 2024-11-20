#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CLayer;

END


BEGIN(Client)

class CBattle_Manager final : public CBase
{

public:
	enum PLAYER_TYPE { PLAYER_ALPHEN, PLAYER_SHIONNE, PLAYER_KISARA, PLAYER_RINWELL, PLAYER_END };

	typedef struct tagDamageINFO
	{
		unsigned int iDamage = 0;   //데미지  혹은공격력
		DAMAGE_TYPE e_DamageType = DAMAGE_TYPE_END;  //데미지종류
		DAMAGE_DIR e_DamageDir = DAMAGE_DIR_END;      //데미지방향
		float fPowerUp = 0.f;
		float fPowerBack = 0.f;
		double dCollisionCoolTime = 0.0;
		PLAYER_TYPE e_PlayerType = PLAYER_END;
	}DAMAGEINFO;




	DECLARE_SINGLETON(CBattle_Manager)
private:
	CBattle_Manager();
	virtual ~CBattle_Manager() = default;

public:
	HRESULT NativeConstruct();

public:
	void Battle_Enter(class CEnemy* pEnemy);
	void Battle_End();
	void Tick(_double TimeDelta);

	vector<class CEnemy*>* Get_VecMonster() { return &m_vecMonsters; }




private:
	vector<class CEnemy*> m_vecMonsters;  //플레이어한테 넘겨줘야할 값 
	vector<class CPlayer*>* m_pVecPlayers;

	vector<class CEnemy*> m_vecHitedMonstersByAlphen;
	vector<class CEnemy*> m_vecHitedMonstersByShionne;


	CLayer* m_pMonsterLayer = nullptr;
	CLayer* m_pPlayerLayer = nullptr;
	_float4 m_vPrePlayerPos;

	_bool m_bBattle = false;
	_bool m_bBattleEnd = false;

public:
	virtual void Free() override;
};

END