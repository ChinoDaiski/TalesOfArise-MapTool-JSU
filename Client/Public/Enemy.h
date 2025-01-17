#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Battle_Manager.h"
#include "Player_Manager.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CEnemy abstract : public CGameObject
{
protected:
	CEnemy(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	CEnemy(const CEnemy& rhs);
	virtual ~CEnemy() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg, CTransform::TRANSFORMDESC* pTransformDesc);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	_int Get_Hp() { return m_iEnemyInfo.m_iHp; }
	void Set_Battle(_bool bBattle) { m_bBattle = bBattle; }
	_vector Get_EnemyPos() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	void	Set_EnemyPos(_fvector vPos) { m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos); }




	void Set_CollisionDistance(_float fDistance) { m_fCollisionDistance = fDistance; }

	void Set_Collision(_bool bCollision) { m_bAttackCollision = bCollision; }
	void Set_KnockBackDir(_fvector vDir) { XMStoreFloat3(&m_vKnockBackDir, vDir); }

	//디버그용 나중에 지울것 
	void Set_Hits() { m_iHits++; }
	_uint Get_Hits() { return m_iHits; }


	void Set_NewCollisionAlphen(_bool bNewCollision) { m_bNewCollisionAlphen = bNewCollision; }
	void Set_NewCollisionShionne(_bool bNewCollision) { m_bNewCollisionShionne = bNewCollision; }
	void Set_NewCollisionKisara(_bool bNewCollision) { m_bNewCollisionKisara = bNewCollision; }
	void Set_NewCollisionRinwell(_bool bNewCollision) { m_bNewCollisionRinwell = bNewCollision; }

	_bool Get_NewCollisionAlphen() { return m_bNewCollisionAlphen; }
	_bool Get_NewCollisionShionne() { return  m_bNewCollisionShionne; }
	_bool Get_NewCollisionKisara() { return m_bNewCollisionKisara; }
	_bool Get_NewCollisionRinwell() { return  m_bNewCollisionRinwell; }


	void Set_PowerUp(_float fPower) { m_fPowerUp = fPower; }
	void Set_PowerKnocKBack(_float fPower) { m_fPowerKnockBack = fPower; }








	_bool Get_Invincible() { return m_bInvincible; }

	void Set_HitDamageInfo(CBattle_Manager::DAMAGEINFO DamageInfo) { m_HitDamageInfo = DamageInfo; }
	CBattle_Manager::DAMAGEINFO Get_AttackDamageInfo() { return m_AttackDamageInfo; }


protected:
	void Compute_Gravity(_double TimeDelta);
	void Compute_Collider();


protected:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pAttackSphereCom = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;




protected:
	CBattle_Manager* m_pBattle_Manager = nullptr;
	CPlayer_Manager* m_pPlayer_Manager = nullptr;

	CBattle_Manager::DAMAGEINFO m_HitDamageInfo;
	CBattle_Manager::DAMAGEINFO m_AttackDamageInfo;


	//디버그용 멤버변수
	_uint m_iHits = 0;


	//공중제어변수
	_float m_fPowerUp = 0.f;
	_float m_fPowerKnockBack = 0.f;





	//충돌변수
	_bool m_bInvincible = false;
	_bool m_bNewCollisionAlphen = false;
	_bool m_bNewCollisionShionne = false;
	_bool m_bNewCollisionKisara = false;
	_bool m_bNewCollisionRinwell = false;

	_bool m_bOnAttackCollider = false;
	_bool m_bBattle = true;			// 배틀중이 아닌 몬스터들이 맵에서 플레이어를 기다리고 있다면 false, 플레이어와 전투중이라면 true
	_bool m_bCutAnimation = true;		// true이면 계속 움직이고 false이면 멈춘다
	_bool m_bHit = false;				// 플레이어에게 공격당했을 경우
	_uint m_iHitCount = 0;				// 플레이어에게 공격당한 횟수가 특정 횟수가 되면 플레이어가 공격하더라도 몬스터도 break상태가 아닌 공격을 한다
	_double m_dAnimSpeed = 2.0;
	_double m_iDuration = 5.0;		// 선형보간 Term식
	_uint m_iCurrentAnimationIndex = 0;
	_uint m_iNextAnimationIndex = 0;
	_float m_fCollisionDistance = 0.f;
	_bool m_bAttackCollision = false;

	_float3 m_vKnockBackDir;


	typedef struct tagEnemyInfo
	{
		_uint m_iHp = 0;					// 몬스터 체력
		_uint m_iBreakCount = 0;			// 플레이어에게 맞은 횟수가 일정시간 동안 20번 이상이라면 m_bHit은 true로 변환된다
	}ENEMYINFO;

	ENEMYINFO m_iEnemyInfo;
};

END