#include "stdafx.h"
#include "..\Public\Enemy.h"
#include "GameInstance.h"
#include "Player.h"

CEnemy::CEnemy(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
	, m_pBattle_Manager(CBattle_Manager::GetInstance())
	, m_pPlayer_Manager(CPlayer_Manager::GetInstance())

{

}

CEnemy::CEnemy(const CEnemy & rhs)
	: CGameObject(rhs)
	, m_pBattle_Manager(CBattle_Manager::GetInstance())
	, m_pPlayer_Manager(CPlayer_Manager::GetInstance())
{
}

HRESULT CEnemy::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CEnemy::NativeConstruct(void * pArg, CTransform::TRANSFORMDESC* pTransformDesc)
{

	if (FAILED(__super::NativeConstruct(pArg, pTransformDesc)))
	{
		MSG_BOX(L"Failed To CEnemy : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}
	if (nullptr != pArg)
	{

		_vector vPos = *(_vector*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	}



	return S_OK;
}

void CEnemy::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (!m_bBattle)
	{
		if (m_pSphereCom->Collsion_Sphere((CCollider*)m_pPlayer_Manager->Get_FieldPlayer()->Get_Component(TEXT("Com_SPHERE"))))
		{
			m_pBattle_Manager->Battle_Enter(this);
			m_bDead = true;

		}

	}

	//else {

	//	Compute_Gravity(TimeDelta);
	//}

}

void CEnemy::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

#ifdef _DEBUG
	m_pRendererCom->Add_DebugComponent(m_pOBBCom);
	m_pRendererCom->Add_DebugComponent(m_pSphereCom);
#endif // _DEBUG

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

}

void CEnemy::Compute_Collider()
{
	m_pAttackSphereCom->Set_On(m_bOnAttackCollider);
}

HRESULT CEnemy::Render()
{

#ifdef _DEBUG
	m_pOBBCom->Render();
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CEnemy::Compute_Gravity(_double TimeDelta)
{

	if (m_bBattle)
	{

		//m_fPowerUp = m_HitDamageInfo.fPowerUp*TimeDelta;

		_float Gravity = 0.1f;


		m_fPowerUp -= TimeDelta*Gravity;



		//m_fPowerUp -= TimeDelta;

		_vector  vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float vPosY = XMVectorGetY(vPos);




		vPosY += m_fPowerUp;
		XMVectorSet(0.f, 1.f, 0.f, 1.f)*m_HitDamageInfo.fPowerUp;
		if (vPosY <= 0)
		{

			vPosY = 0;

		}
		vPos = XMVectorSetY(vPos, vPosY);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, vPosY));











		if (m_HitDamageInfo.fPowerBack > 0.f)
		{
			m_HitDamageInfo.fPowerBack -= TimeDelta*Gravity;
			Set_EnemyPos(Get_EnemyPos() + XMLoadFloat3(&m_vKnockBackDir)*m_HitDamageInfo.fPowerBack);



		}

	}












}

void CEnemy::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);



}
