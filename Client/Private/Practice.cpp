#include "stdafx.h"
#include "..\Public\Practice.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "GameObject.h"


CPractice::CPractice(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CEnemy(pDeviceOut, pDeviceContextOut)
{

}

CPractice::CPractice(const CPractice & rhs)
	: CEnemy(rhs)
{
}

HRESULT CPractice::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPractice::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;



	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_AnimationIndex(MOVE_IDLE);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos = XMVectorSetY(vPos, 0.f);

	m_bBattle = false;
	m_iEnemyInfo.m_iHp = 99;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(20.f, 0.f, 10.f, 1.f));
	return S_OK;
}

void CPractice::Tick(_double TimeDelta)
{

	if (m_iCurrentAnimationIndex == m_iNextAnimationIndex)
	{
		m_pModelCom->Monster_Update(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_bCutAnimation);

		_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
		_vector MoveVector = XMVector3TransformCoord(XMLoadFloat3(&m_pModelCom->Get_CurAnimation()->Get_MoveVector()), m_pModelCom->Get_PivotMatrix());



		_matrix		BoneMatrix = WorldMatrix;

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
		BoneMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);


		MoveVector = XMVector3TransformCoord(MoveVector, BoneMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + MoveVector, 1.f));
	}

	else
	{
		m_pModelCom->Monster_Update_NextAnim(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_iDuration, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
	}

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector pPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_bBattle)
	{
		m_TimeDelta += TimeDelta;

		if (!m_bStartScene && (m_TimeDelta > 1.0))
		{
			StartScene();
		}


		if ((m_TimeDelta > 5.0) && (m_bOnce == false))
		{
			m_bStart = true;

			if (m_iMotion == 6)
				m_iMotion = 0;

			else
				m_iMotion++;
		}


		if (m_pTransformCom->FrontPlayer(pPos))
			m_bFront = true;

		if (!m_bHit)
		{
			switch (m_iMotion)
			{
			case 0:
			{
				if (m_pTransformCom->FrontPlayer(pPos) || m_bFront)
					AttackPattern1(TimeDelta);

				break;
			}
			case 1:
			{
				if (!(m_pTransformCom->LeftPlayer(pPos)))

					AttackPattern2();

				break;
			}
			case 2:
			{
				if (m_pTransformCom->LeftPlayer(pPos))
					AttackPattern3();

				break;
			}

			case 3:
			{
				if (m_pTransformCom->Distance(pPos, 8.f) && m_pTransformCom->FrontPlayer(pPos))
					AttackPattern4();

				break;
			}

			case 4:
				AttackPattern5(TimeDelta);
				break;

			case 5:
				NothingAnim();
				break;

			default:
				return;
			}
		}

		else
		{
			m_bCutAnimation = false;
			if (m_iCurrentAnimationIndex == DAMAGE_LARGE_B)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1)
				{
					m_bHit = false;
					m_iNextAnimationIndex = MOVE_IDLE;
				}
			}
		}
	}

	else  // 배틀 중이 아닐 때 취하는 행동들
	{
		m_TimeDelta += TimeDelta;

		if ((m_TimeDelta > 2.0) && (m_bOnce == false))
		{
			m_bStart = true;

			if (m_iWaitMotion == 3)
				m_iWaitMotion = 0;

			else
				m_iWaitMotion++;
		}


		switch (m_iWaitMotion)
		{
		case 0:
		{
			if (m_pTransformCom->Distance(pPos, 10.f))
				WaitPlayer1();

			else
			{
				m_bCutAnimation = true;
				m_iNextAnimationIndex = MOVE_IDLE;
			}
			break;
		}
		case 1:
		{
			if (m_pTransformCom->Distance(pPos, 20.f))
				WaitPlayer2();

			else
			{
				m_bCutAnimation = true;
				m_iNextAnimationIndex = MOVE_IDLE;
			}

			break;
		}

		case 2:
			WaitPlayer3();
			break;

		default:
			return;
		}

	}


#pragma endregion
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());


	__super::Tick(TimeDelta);


	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bAttackCollision)
	{
		m_bHit = true;
		m_iNextAnimationIndex = DAMAGE_LARGE_B;
		m_bAttackCollision = false;


	}





	///




	RELEASE_INSTANCE(CGameInstance);
}


void CPractice::StartScene()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = ADVENT;
	}

	else if ((m_iCurrentAnimationIndex == ADVENT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bStartScene = true;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::AttackPattern1(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance)

		if (!m_bOnce && m_bStart)
		{
			m_dAnimSpeed = 2.0;
			m_iDuration = 5.0;
			m_bStart = false;
			m_bOnce = true;
			m_bCutAnimation = false;
			m_iNextAnimationIndex = RUSH_START;
		}

		else if ((m_iCurrentAnimationIndex == RUSH_START) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
		{
			m_iNextAnimationIndex = RUSH_LOOP;
		}

		else if ((m_iCurrentAnimationIndex == RUSH_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
		{
			//m_dAnimTime += TimeDelta;

			//if (m_dAnimTime < 1.5)
			//{
			//	m_pModelCom->Get_CurAnimation()->Reset_MonsterAnimation();
			//}

			//else if(m_dAnimTime>1.5 && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
			m_iNextAnimationIndex = RUSH_END;
		}

		else if ((m_iCurrentAnimationIndex == RUSH_END) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
		{
			m_bFront = false;
			m_dAnimSpeed = 2.0;
			m_bCutAnimation = true;
			m_iNextAnimationIndex = MOVE_IDLE;
			m_bOnce = false;
			m_TimeDelta = 0.0;
		}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::AttackPattern2()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = TACKLE_RIGHT;
	}

	else if ((m_iCurrentAnimationIndex == TACKLE_RIGHT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}


void CPractice::AttackPattern3()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = TACKLE_LEFT;
	}

	else if ((m_iCurrentAnimationIndex == TACKLE_LEFT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::AttackPattern4()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = RUSH_SHORT;
	}

	else if ((m_iCurrentAnimationIndex == RUSH_SHORT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::AttackPattern5(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == GIGNAT_RUSH_START) && (0.16f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.15f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == GIGNAT_RUSH_START)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = GIGNAT_RUSH_START;
	}

	else if ((m_iCurrentAnimationIndex == GIGNAT_RUSH_START) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = GIGANT_RUSH_LOOP;
	}

	else if ((m_iCurrentAnimationIndex == GIGANT_RUSH_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iDuration = 1.0;
		m_iNextAnimationIndex = GIGANT_RUSH_END;
	}

	else if ((m_iCurrentAnimationIndex == GIGANT_RUSH_END) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::FallToGround()
{
	if (!m_bOnce)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DOWN_F;
	}

	else if ((m_iCurrentAnimationIndex == DOWN_F) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

}

void CPractice::HitPattern1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector pPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_bSky)		//안에 조건식 다시 짜야한다
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bOnce = true;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = DAMAGE_AIR_LOOP;
	}

	else
	{
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DOWN_F;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	//else if ((m_iCurrentAnimationIndex == DAMAGE_AIR_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	//{

	//	m_bCutAnimation = true;
	//	m_iNextAnimationIndex = DOWN_F;
	//	m_bOnce = false;
	//	m_TimeDelta = 0.0;
	//}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::HitPattern2()
{
	if (!m_bOnce)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DAMAGE_LARGE_R;
	}

	else if ((m_iCurrentAnimationIndex == DAMAGE_LARGE_R) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
	}
}

void CPractice::HitPattern3()
{
	if (!m_bOnce)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DAMAGE_LARGE_L;
	}

	else if ((m_iCurrentAnimationIndex == DAMAGE_LARGE_L) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
	}
}

void CPractice::NothingAnim()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == BRAVE) && (0.16f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.15f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == BRAVE)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = BRAVE;
	}

	else if ((m_iCurrentAnimationIndex == BRAVE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::WaitPlayer1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == DETECT_IDLE) && (0.11f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.1f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == DETECT_IDLE)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 3.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DETECT_IDLE;
	}

	else if ((m_iCurrentAnimationIndex == DETECT_IDLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::WaitPlayer2()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 3.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = SYMBOL_LOOKOUT;
	}

	else if ((m_iCurrentAnimationIndex == SYMBOL_LOOKOUT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPractice::WaitPlayer3()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 3.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = SYMBOL_IDLE;
	}

	else if ((m_iCurrentAnimationIndex == SYMBOL_IDLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPractice::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}



	return S_OK;
}



HRESULT CPractice::SetUp_Components()
{
	///* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Boar"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB */
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	ColliderDesc.vSize = _float3(0.7f, 1.4f, 0.7f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPractice::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPositionFloat4(), sizeof(_float4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPractice * CPractice::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CPractice*	pInstance = new CPractice(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPractice"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPractice::Clone(void * pArg)
{
	CPractice*	pInstance = new CPractice(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone CPractice"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPractice::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
}