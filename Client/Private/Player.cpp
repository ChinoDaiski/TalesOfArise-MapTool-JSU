#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include"Animation.h"
#include "Enemy.h"
#include "Navigation.h"

CPlayer::CPlayer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
	, m_pPlayer_Manager(CPlayer_Manager::GetInstance())
	, m_pTime_Manager(CTime_Manager::GetInstance())
	, m_pBattle_Manager(CBattle_Manager::GetInstance())

{

	Safe_AddRef(m_pPlayer_Manager);
	Safe_AddRef(m_pTime_Manager);
	Safe_AddRef(m_pBattle_Manager);
}


CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
	, m_pPlayer_Manager(CPlayer_Manager::GetInstance())
	, m_pTime_Manager(CTime_Manager::GetInstance())
	, m_pBattle_Manager(CBattle_Manager::GetInstance())
{

	Safe_AddRef(m_pPlayer_Manager);
	Safe_AddRef(m_pTime_Manager);
	Safe_AddRef(m_pBattle_Manager);
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg, CTransform::TRANSFORMDESC* pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pArg, pTransformDesc)))
	{
		MSG_BOX(L"Failed To CPlayer : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}

	m_pPlayer_Manager->Push_Player(this);

	return S_OK;
}

void CPlayer::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	System_Key_Input();

	if (m_iCurrentAnimationIndex == m_iNextAnimationIndex)
	{
		m_pModelCom->Update(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_bLoop);
		m_pModelFaceCom->Update(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_bLoop);
		m_pModelHairCom->Update(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_bLoop);
		m_pModelSKLCom->Update(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_bLoop);
		_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
		_vector MoveVector = XMVector3TransformCoord(XMLoadFloat3(&m_pModelCom->Get_CurAnimation()->Get_MoveVector()), m_pModelCom->Get_PivotMatrix());
		//MoveVector = XMVector3TransformCoord(MoveVector, WorldMatrix);
		//MoveVector = XMVectorSetY(MoveVector, -XMVectorGetY(MoveVector)); //


		/*	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_vector vAxis = XMVectorSet(0.f, 0.f, 1.f, 0.f);*/




		//////
		_matrix		BoneMatrix = WorldMatrix;

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
		BoneMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		//_float fLength = XMVectorGetX(XMVector3Length(MoveVector));
		MoveVector = XMVector3TransformCoord(MoveVector, BoneMatrix);// *fLength;



		 ////m_pTransformCom->Set_WorldMatrix(WorldMatrix);
		//if (nullptr != m_pNaviCom) {

		//	m_pNaviCom->Move_OnNavigation(XMVectorSetW(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + MoveVector, 1.f))

		//}
		//else{




		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + MoveVector, 1.f));
		
	}

	else
	{

		if (m_bBattle)
		{
			if (m_bSkill)
			{
				if (m_pVecMonsters->size() > 0)
				{
					m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
				}

			}
		}


		m_pModelCom->Update_NextAnim(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
		m_pModelFaceCom->Update_NextAnim(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
		m_pModelHairCom->Update_NextAnim(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
		m_pModelSKLCom->Update_NextAnim(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
		//m_pModelFaceCom->Update_NextAnim(TimeDelta*m_fAttackSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
		//m_pModelHairCom->Update_NextAnim(TimeDelta*m_fAttackSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
	}


}

void CPlayer::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		//m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

}

HRESULT CPlayer::Render()
{

	return S_OK;
}

HRESULT CPlayer::Render_LightDepth()
{
	return S_OK;
}


//HRESULT CPlayer::SetUp_ConstantTable()
//{
//	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
//
//	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
//	{
//		MSG_BOX(L"Failed To CPlayer : SetUp_ConstantTable : m_pTransform->Bind_WorldMatrixOnShader");
//		return E_FAIL;
//	}
//	
//	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
//	{
//		MSG_BOX(L"Failed To CModel_Object : SetUp_ConstantTable : m_pShader->Set_RawValue(g_ViewMatrix)");
//		return E_FAIL;
//	}
//
//	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
//	{
//		MSG_BOX(L"Failed To CModel_Object : SetUp_ConstantTable : m_pShader->Set_RawValue(g_ProjMatrix)");
//		return E_FAIL;
//	}
//	
//	RELEASE_INSTANCE(CGameInstance);
//
//	return S_OK;
//}

CNavigation* CPlayer::SetUp_Navigation(const TCHAR* pTagNaviCompoent)
{
	// 기존에 있는 Navi컴포넌트를 삭제한다.
	if (nullptr != m_pNaviCom)
		Safe_Release(m_pNaviCom);

	Delete_Component(TEXT("Com_Navigation"));

	if (nullptr != m_pNaviCom)
		Safe_Release(m_pNaviCom);

	// 인자로 받은 Navi Component를 복제하여 해당 정보를
	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentIndex = 0;
	NaviDesc.pParent = nullptr;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Navigation"), LEVEL_STATIC, pTagNaviCompoent, (CComponent**)&m_pNaviCom, &NaviDesc)))
		return nullptr;

	return m_pNaviCom;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pModelFaceCom);
	Safe_Release(m_pModelHairCom);
	Safe_Release(m_pModelSKLCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPlayer_Manager);
	Safe_Release(m_pTime_Manager);
	Safe_Release(m_pBattle_Manager);

}

void CPlayer::Set_EquipmentBattle(_bool bBattle)
{
	for (auto Equipment : vecEquipment)
	{
		Equipment->Set_Battle(bBattle);
	}
}

void CPlayer::Set_EquipmentField(_bool bField)
{
	for (auto Equipment : vecEquipment)
	{
		Equipment->Set_Field(bField);
	}
}

void CPlayer::System_Key_Input()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (m_bBattle&&m_bAuto == false)
	{

		if (pGameInstance->Key_Pressing(DIK_LCONTROL) && m_bLControlDown == false)
		{
			m_bLControlDown = true;
			m_bTargetChange = true;
			m_pTime_Manager->Set_AllLayerTime(0.0);
			m_pPlayer_Manager->Get_Camera()->Set_Change(true);

		}

		if (m_bLControlDown)
		{
			if (pGameInstance->Key_Down(DIK_1))
			{
				m_pPlayer_Manager->Set_MainPlayer(0);
			}
			if (pGameInstance->Key_Down(DIK_2))
			{
				m_pPlayer_Manager->Set_MainPlayer(1);
			}

			if (pGameInstance->Key_Down(DIK_LEFTARROW))
			{
				if (m_bAuto == false)
				{
					Change_Targeting(false);
				}
			}
			if (pGameInstance->Key_Down(DIK_RIGHTARROW))
			{
				if (m_bAuto == false)
				{
					Change_Targeting(true);
				}
			}
		}



		if (pGameInstance->Key_Up(DIK_LCONTROL))
		{

			m_bTargetChange = false;
			m_bLControlDown = false;
			m_pTime_Manager->Set_AllLayerTime(1.0);
			m_pPlayer_Manager->Get_Camera()->Set_Change(false);
		}

	}





	//if (pGameInstance->Key_Down(DIK_T))
	//{
	//	if (!m_bAuto)
	//	{
	//		m_pBattle_Manager->Battle_End();
	//		m_bAir = false;
	//		m_bAirAttack = false;
	//		m_bAttack = false;
	//		m_bAttackKeyInput = false;
	//		m_bEvade = false;
	//		m_bSkill = false;
	//		m_bLand = true;
	//		m_bMove = false;
	//		m_bBattle = false;
	//		m_bJump = false;
	//	}
	//}
}


void CPlayer::Change_Targeting(_bool bRight)
{

	if (bRight)
	{

		m_iter++;
		if (m_iter == m_pVecMonsters->end()) //안되면 end()-1로 할것. 민희씨가그럼 내탓아님
			m_iter = m_pVecMonsters->begin();
	}
	else
	{
		//해줘 누군가.
		if (m_iter == m_pVecMonsters->begin())
			m_iter = m_pVecMonsters->end();
		m_iter--;
	}
}

_vector CPlayer::Get_TargetPos()
{
	auto iter = m_iter;

	return ((CTransform*)(*iter)->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

}
