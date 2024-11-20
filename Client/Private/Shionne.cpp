#include "stdafx.h"
#include "..\Public\Shionne.h"
#include "GameInstance.h"

#include "Animation.h"
#include "Channel.h"
#include "Gun.h"

CShionne::CShionne(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CPlayer(pDeviceOut, pDeviceContextOut)
{

}

CShionne::CShionne(const CShionne & rhs)
	: CPlayer(rhs)
{
}

HRESULT CShionne::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShionne::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC      TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_AnimationIndex(m_iCurrentAnimationIndex);

	CGameInstance*pGameInstance = CGameInstance::GetInstance();

	CEquipment::SOCKETDESC         SocketDesc;
	ZeroMemory(&SocketDesc, sizeof(CEquipment::SOCKETDESC));



	SocketDesc.pModelCom = m_pModelSKLCom;
	SocketDesc.iIndex = 0;
	SocketDesc.pBoneName = m_HandBone;



	m_pNormalGun = (CGun*)pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("Layer_Gun"), TEXT("Prototype_GameObject_Gun"), &SocketDesc);
	if (m_pNormalGun == nullptr)
		return E_FAIL;

	m_pNormalGun->Set_Battle(false);
	m_pNormalGun->Set_Field(false);


	vecEquipment.push_back(m_pNormalGun);



	_int m_iMaxCp = 125;
	_int m_iCurrentCp = 125;
	_int m_iMaxHp = 9999;
	_int m_iCurrentHp = 9999;
	_int m_iComboHits = 0;
	_int m_iMaxAg = 7;
	_int m_iCurrentAg = 7;


	m_bAuto = true;
	m_bBattle = false;
	m_bField = false;

	m_eSkillAirFirst = SKILL_AIR_DISPLODE;
	m_eSkillGroundFirst = SKILL_GROUND_GRAVITAS_FIELD;

	m_eSkillAirSecond = SKILL_AIR_SPEAR_SWEEP;
	m_eSkillGroundSecond = SKILL_GROUND_ANNIHILATION;

	m_eSkillAirThird = SKILL_AIR_AQEOUS_IMPACT;
	m_eSkillGroundThird = SKILL_GROUND_LUNAR_BLAST;


	return S_OK;
}

void CShionne::Tick(_double TimeDelta)
{
	if (m_bBattle || m_bField)
	{
		__super::Tick(TimeDelta);

		//UIȮ�ο�

		m_iCurrentHp--;
		m_iCurrentCp--;
		m_iComboHits++;
		m_iCurrentAg--;

		if (m_iComboHits > 999)
			m_iComboHits = 0;
		if (m_iCurrentHp < 0)
			m_iCurrentHp = m_iMaxHp;
		if (m_iCurrentAg < 0)
			m_iCurrentAg = m_iMaxAg;
		if (m_iCurrentCp < 0)
			m_iCurrentCp = m_iMaxCp;


		CGameInstance*      pGameInstance = GET_INSTANCE(CGameInstance);


		//Compute_Look();

		if (!m_bAuto)
		{
			if (!m_bHit)
			{
				Key_Input();

				Compute_Anim(TimeDelta*m_dTimeSpeed);
			}

			else {
				Compute_Hit(TimeDelta*m_dTimeSpeed);

			}
		}









		/*Compute_Move(TimeDelta);
		Skill();
		Jump(TimeDelta);
		*/









		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
		m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());



		/*if (pGameInstance->Key_Down(DIK_T))
		{
			m_bDamage_Small_b = true;
			m_bHit = true;
		}*/

		RELEASE_INSTANCE(CGameInstance);
	}
}

void CShionne::LateTick(_double TimeDelta)
{
	if (m_bBattle || m_bField)
	{
		__super::LateTick(TimeDelta);
		CGameInstance*      pGameInstance = GET_INSTANCE(CGameInstance);

#ifdef _DEBUG
		m_pRendererCom->Add_DebugComponent(m_pSphereCom);
		m_pRendererCom->Add_DebugComponent(m_pSphereCom);
#endif // _DEBUG
		RELEASE_INSTANCE(CGameInstance);
	}
}

HRESULT CShionne::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint      iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;
		//���� ��� �ؽ��İ� ��� ������ ����
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;

	}

	_uint      iNumFaceMeshContainers = m_pModelFaceCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumFaceMeshContainers; ++i)
	{
		if (FAILED(m_pModelFaceCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelFaceCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelFaceCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}


	_uint      iNumHairMeshContainers = m_pModelHairCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumHairMeshContainers; ++i)
	{
		if (FAILED(m_pModelHairCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelHairCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelHairCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}






#ifdef _DEBUG
	m_pOBBCom->Render();
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}


HRESULT CShionne::SetUp_Components()
{
	///* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Body"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_ModelFace"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Face"), (CComponent**)&m_pModelFaceCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_ModelHair"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Hair"), (CComponent**)&m_pModelHairCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_ModelSKL"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_SKL"), (CComponent**)&m_pModelSKLCom)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC         ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	ColliderDesc.vSize = _float3(0.7f, 1.4f, 0.7f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_AttackSPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pAttackSphereCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CShionne::SetUp_ConstantTable()
{
	CGameInstance*      pGameInstance = GET_INSTANCE(CGameInstance);

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

void CShionne::Key_Input()
{
	CGameInstance*      pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	_bool flag = false;

	if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
	{

		flag = true;
		m_bMove = true;

		if (pGameInstance->Key_Down(DIK_LSHIFT))
		{
			m_bEvade = true;
		}
	}
	if (pGameInstance->Key_Down(DIK_LSHIFT))
	{
		flag = true;
		m_bEvadeBack = true;
		m_bEvade = true;
	}

	if (pGameInstance->Key_Down(DIK_SPACE))
	{
		flag = true;
		m_bJump = true;
	}

	if (pGameInstance->Key_Pressing(DIK_E) && m_bSkill == false)
	{
		flag = true;
		m_bSkill = true;
		m_bSkillKeyDown = true;  // ÷�� ��ųŰ ������.
		m_bStartSkill = false;

		if (m_bAir)
		{
			m_eSkillUse = (SIO_SKILL)m_eSkillAirFirst;
		}
		else {
			m_eSkillUse = (SIO_SKILL)m_eSkillGroundFirst;
		}

	}

	if (pGameInstance->Key_Up(DIK_E) && m_bSkill)
	{
		m_bSkillKeyDown = false;
	}

	if (pGameInstance->Key_Pressing(DIK_R) && m_bSkill == false)
	{
		flag = true;
		m_bSkill = true;
		m_bSkillKeyDown = true;  // ÷�� ��ųŰ ������.
		m_bStartSkill = false;
		if (m_bAir)
		{
			m_eSkillUse = (SIO_SKILL)m_eSkillAirSecond;
		}
		else {
			m_eSkillUse = (SIO_SKILL)m_eSkillGroundSecond;
		}
	}

	if (pGameInstance->Key_Up(DIK_R) && m_bSkill)
	{
		m_bSkillKeyDown = false;
	}

	if (pGameInstance->Key_Pressing(DIK_F) && m_bSkill == false)
	{
		flag = true;
		m_bSkill = true;
		m_bSkillKeyDown = true;  // ÷�� ��ųŰ ������.
		m_bStartSkill = false;
		if (m_bAir)
		{
			m_eSkillUse = (SIO_SKILL)m_eSkillAirThird;
		}
		else {
			m_eSkillUse = (SIO_SKILL)m_eSkillGroundThird;
		}
	}

	if (pGameInstance->Key_Up(DIK_F) && m_bSkill)
	{
		m_bSkillKeyDown = false;
	}


	if (pGameInstance->Key_Pressing(DIK_LCONTROL))
	{
		if (pGameInstance->Key_Down(DIK_1))
		{
			m_pPlayer_Manager->Set_MainPlayer(0);
		}
		if (pGameInstance->Key_Down(DIK_2))
		{
			m_pPlayer_Manager->Set_MainPlayer(1);
		}

	}


	if (m_bSkill == false)
	{
		m_bSkillKeyDown = false;
	}


	m_bKeyInput = flag;


	Safe_Release(pGameInstance);


}

void CShionne::Jump(_double TimeDelta)
{
	if (m_bJump)
	{
		m_bLoop = false;
		if (!m_bLand)
		{
			m_bJump = false;
		}
		m_dGravityPower = 0.25;
		if (m_bAttack || m_bAirAttack)
			m_bJump = false;

		if (m_bAttack == false && m_bSkill == false && m_bEvade == false && m_bEvadeBack == false && m_bAirAttack == false && m_bLand)
		{

			if (m_iAbleJumpCount > 0)
			{
				m_bMove = false;
				m_bAir = true;
				m_bLand = false;

				m_dAnimSpeed = 5.5;
				m_iAbleJumpCount--;
				m_iNextAnimationIndex = SIO_ANIM_BTL_JUMP;

			}
			else {
				//return;
			}
			if (m_iNextAnimationIndex != SIO_ANIM_BTL_JUMP)
			{
				m_bJump = false;
			}

		}
	}

	if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_JUMP)
	{
		m_bAir = true;
		_vector  vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float vPosY = XMVectorGetY(vPos);
		m_dJumpSpeed -= TimeDelta*m_dGravityPower;

		vPosY += (_float)m_dJumpSpeed;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, vPosY));
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
		{
			m_dJumpSpeed = 0.14;
			m_iNextAnimationIndex = SIO_ANIM_BTL_FALL;
			m_bJump = false;
		}

	}


	if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_JUMP || m_iCurrentAnimationIndex == SIO_ANIM_BTL_FALL)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_Camera"), TEXT("Com_Transform"), 0);
		_vector vTargetLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);

		_matrix      RotationMatrix;
		_vector vLook[DIR_END];

		for (_uint i = 0; i < DIR_END; ++i)
		{
			RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f*i));
			vLook[i] = XMVectorSetY(XMVector3TransformNormal(vTargetLook, RotationMatrix), 0.f);
		}



		if (pGameInstance->Key_Pressing(DIK_W))
		{
			if (pGameInstance->Key_Pressing(DIK_D))
			{
				m_ePlayerDir = DIR_WD;

			}
			else if (pGameInstance->Key_Pressing(DIK_A))
			{
				m_ePlayerDir = DIR_AW;

			}
			else {
				m_ePlayerDir = DIR_W;

			}

		}

		else if (pGameInstance->Key_Pressing(DIK_D))
		{
			if (pGameInstance->Key_Pressing(DIK_S))
			{
				m_ePlayerDir = DIR_DS;
			}
			else
			{
				m_ePlayerDir = DIR_D;
			}

		}

		else if (pGameInstance->Key_Pressing(DIK_S))
		{
			if (pGameInstance->Key_Pressing(DIK_A))
			{
				m_ePlayerDir = DIR_SA;
			}
			else
			{
				m_ePlayerDir = DIR_S;
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_A))
		{

			m_ePlayerDir = DIR_A;

		}

		if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
		{
			m_pTransformCom->Go_Straight(TimeDelta);

		}

	}






}


void CShionne::Evade()
{

	if (m_bEvade)
	{
		if (m_bSkill)
		{
			m_bEvade = false;
			return;
		}

		m_bLoop = false;
		m_bMove = false;
		m_bAttack = false;
		m_bJump = false;
		if (m_bSkill == false && m_bAttack == false && m_bAirAttack == false)
		{

			if (m_bAir)
			{
				if (m_iAbleAirEvadeCount > 0)
				{
					m_iAbleAirEvadeCount--;
					m_iNextAnimationIndex = SIO_ANIM_BTL_STEP_AIR;
					m_bJump = false;
					m_bAirAttack = false;
					m_dJumpSpeed = 0.14;
					m_dAnimSpeed = 2.5;
				}
				else {
					m_bEvade = false;
				}

			}
			else
			{
				m_dAnimSpeed = 2.5;
				if (m_bEvadeBack)
				{
					m_iNextAnimationIndex = SIO_ANIM_BTL_STEP_LAND_BACK;
				}
				else
				{
					CGameInstance* pGameInstance = CGameInstance::GetInstance();
					CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_Camera"), TEXT("Com_Transform"), 0);
					_vector vTargetLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);

					_matrix      RotationMatrix;
					_vector vLook[DIR_END];

					for (_uint i = 0; i < DIR_END; ++i)
					{
						RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f*i));
						vLook[i] = XMVectorSetY(XMVector3TransformNormal(vTargetLook, RotationMatrix), 0.f);
					}
					m_pTransformCom->TurnAxis(vLook[m_ePlayerDir]);
					m_iNextAnimationIndex = SIO_ANIM_BTL_STEP_LAND;
				}

			}

		}
	}

	if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_STEP_AIR
		|| m_iCurrentAnimationIndex == SIO_ANIM_BTL_STEP_LAND_BACK
		|| m_iCurrentAnimationIndex == SIO_ANIM_BTL_STEP_LAND)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 14)
		{
			m_bMove = false;
			m_bEvade = false;
			m_bEvadeBack = false;
			m_iLinearTime = 0.5;
		}
	}

	if (!m_bEvade)
	{
		m_bEvadeBack = false;
	}








}

void CShionne::Attack()
{

	CGameInstance*      pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Button_Pressing(CInput_Device::DIMB_LBUTTON))
	{
		if (!m_bEvade && !m_bSkill)
		{
			m_bAttackKeyInput = true;

		}
	}
	if (pGameInstance->Button_Up(CInput_Device::DIMB_LBUTTON)) {
		m_bAttackKeyInput = false;
	}

	if (m_bAttack || m_bAirAttack)
	{
		m_bMove = false;
	}

	if (m_bAttackKeyInput)
	{
		if (!m_bAir&&m_bLand)
		{
			m_bLoop = false;
			m_dAnimSpeed = 1.75;
			m_iLinearTime = 0.5;
			switch (m_iCurrentAnimationIndex)
			{

			case SIO_ANIM_BTL_ATTACK_NORMAL_0:
				m_iWaitAnimationIndex = SIO_ANIM_BTL_ATTACK_NORMAL_1;
				break;
			case SIO_ANIM_BTL_ATTACK_NORMAL_1:
				m_iWaitAnimationIndex = SIO_ANIM_BTL_ATTACK_NORMAL_2;
				break;
			case SIO_ANIM_BTL_ATTACK_NORMAL_2:
				m_iWaitAnimationIndex = SIO_ANIM_BTL_ATTACK_NORMAL_3;
				break;
			case SIO_ANIM_BTL_ATTACK_NORMAL_3:
				m_iWaitAnimationIndex = SIO_ANIM_BTL_ATTACK_NORMAL_4;
				break;

			default:
				m_iWaitAnimationIndex = SIO_ANIM_BTL_ATTACK_NORMAL_0;
				break;

			}


			if (m_iWaitAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_0)
			{

				m_iNextAnimationIndex = m_iWaitAnimationIndex;
				m_bAttack = true;
			}

			else if (m_iWaitAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_1&&m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_0)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 32)
				{
					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					m_bAttack = true;


				}
			}

			else if (m_iWaitAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_2&&m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_1)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 29)
				{

					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					m_bAttack = true;

				}
			}

			else if (m_iWaitAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_3&&m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_2)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 33)
				{

					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					m_bAttack = true;

				}
			}


			else if (m_iWaitAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_4&&m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_3)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 27)
				{
					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					m_bAttack = true;
					m_iWaitAnimationIndex = SIO_ANIM_END;
				}
			}

		}

		else if (!m_bLand&&m_bAir)
		{
			m_bJump = false;
			m_bLoop = false;
			m_dAnimSpeed = 1.8;
			m_iLinearTime = 0.5;
			switch (m_iCurrentAnimationIndex)
			{

			case SIO_ANIM_BTL_ATTACK_NORMAL_AIR_0:
				m_iWaitAnimationIndex = SIO_ANIM_BTL_ATTACK_NORMAL_AIR_1;
				break;
			case SIO_ANIM_BTL_ATTACK_NORMAL_AIR_1:
				m_iWaitAnimationIndex = SIO_ANIM_BTL_ATTACK_NORMAL_AIR_2;
				break;
			case SIO_ANIM_BTL_ATTACK_NORMAL_AIR_2:
				break;
			default:
				m_iWaitAnimationIndex = SIO_ANIM_BTL_ATTACK_NORMAL_AIR_0;
				break;

			}
			if (m_iAbleAirAttackCount > 0)
			{
				if (m_iWaitAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_AIR_0)
				{

					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					m_bAirAttack = true;
					m_iAbleAirAttackCount--;
				}
			}
			else {
				m_bAirAttack = false;
			}



			if (m_iWaitAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_AIR_1&&m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_AIR_0)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 30)
				{
					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					m_bAirAttack = true;


				}
			}

			else if (m_iWaitAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_AIR_2&&m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_AIR_1)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 17)
				{

					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					m_bAirAttack = true;
					m_iWaitAnimationIndex = SIO_ANIM_END;

				}
			}

		}


	}

	if (m_bAttack)
	{
		if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_0&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 45 ||
			m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_1&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 55 ||
			m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_2&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 55 ||
			m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_3&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 45 ||
			m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_3&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 45)
		{
			m_bAttack = false;
		}

	}
	if (m_bAirAttack)
	{
		m_bEvade = false;
		if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_AIR_0&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 47 ||
			m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_AIR_1&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 33 ||
			m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NORMAL_AIR_2&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 50)
		{
			m_bAirAttack = false;
		}

	}


	Safe_Release(pGameInstance);
}

void CShionne::Skill(_double TimeDelta)
{
	if (m_bSkill)
	{
		m_bLoop = false;

		switch (m_eSkillUse)
		{
		case Client::CShionne::SKILL_LUNAR_BLAST:
			Lunar_Blast();
			break;
		case Client::CShionne::SKILL_GRAVITAS_FIELD:
			Gravitas_Field();
			break;
		case Client::CShionne::SKILL_ANNIHILATION:
			Annihilation();
			break;
		case Client::CShionne::SKILL_DISPLODE:
			Displode();
			break;
		case Client::CShionne::SKILL_AQEOUS_IMPACT:
			Aqeous_Impact();
			break;
		case Client::CShionne::SKILL_SPEAR_SWEEP:
			Spear_Sweep();
			break;
		default:
			break;
		}


	}




}
//



void CShionne::Compute_Anim(_double TimeDelta)
{
	CGameInstance*      pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	if (m_bMove && !m_bSkill && !m_bEvade && !m_bAttack && !m_bJump &&m_bLand && !m_bAir)
	{
		Compute_Move(TimeDelta);
	}


	Jump(TimeDelta);
	Skill(TimeDelta);
	Evade();
	Attack();











	if (m_bAir)
	{
		m_bMove = false;
		m_bAttack = false;
	}
	else {
		m_bAirAttack = false;
		m_iAbleJumpCount = 1;
		m_iAbleAirEvadeCount = 1;
		m_iAbleAirAttackCount = 1;
		m_dFallSpeed = 0.0;
		m_dJumpSpeed = 0.14;
	}
	if (m_bKeyInput == false)
	{
		m_bMove = false;
	}
	if (
		m_bAttack == false &&
		m_bEvade == false &&
		m_bSkill == false &&
		m_bJump == false &&
		m_bMove == false &&
		m_bAirAttack == false &&
		m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
	{
		if (!m_bAir)
		{
			m_bLoop = true;

			m_iNextAnimationIndex = SIO_ANIM_BTL_MOVE_IDLE;
		}
		else {
			m_bLoop = true;
			m_iNextAnimationIndex = SIO_ANIM_BTL_FALL;
		}
	}


	_vector  vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float vPosY = XMVectorGetY(vPos);

	if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_FALL)
	{
		//m_bMove = false;
		m_dFallSpeed += TimeDelta*m_dGravityPower;
		m_bLand = false;

		vPosY -= (_float)m_dFallSpeed;
		if (vPosY <= 0)
		{
			m_bLoop = false;
			vPosY = 0;
			m_dFallSpeed = 0.0;
			m_iNextAnimationIndex = SIO_ANIM_BTL_LAND;

		}
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, vPosY));
	}

	if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_LAND&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 15)
	{
		m_bLand = true;
	}



	//if (m_bSkill)
	//{
	//   m_bLoop = false;
	//   Skill();
	//}

	if (vPosY <= 0.f)
	{
		m_bAir = false;

	}



	Safe_Release(pGameInstance);



}

void CShionne::Compute_Move(_double TimeDelta)
{


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_Camera"), TEXT("Com_Transform"), 0);
	_vector vTargetLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);

	_matrix      RotationMatrix;
	_vector vLook[DIR_END];

	for (_uint i = 0; i < DIR_END; ++i)
	{
		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f*i));
		vLook[i] = XMVectorSetY(XMVector3TransformNormal(vTargetLook, RotationMatrix), 0.f);
	}







	//

	m_bLoop = true;
	m_iLinearTime = 0.0;

	m_iNextAnimationIndex = SIO_ANIM_BTL_MOVE_RUN;
	m_dAnimSpeed = 1.85;


	if (pGameInstance->Key_Pressing(DIK_W))
	{
		if (pGameInstance->Key_Pressing(DIK_D))
		{

			m_ePlayerDir = DIR_WD;

		}
		else if (pGameInstance->Key_Pressing(DIK_A))
		{
			m_ePlayerDir = DIR_AW;

		}
		else {
			m_ePlayerDir = DIR_W;

		}

	}

	else if (pGameInstance->Key_Pressing(DIK_D))
	{
		if (pGameInstance->Key_Pressing(DIK_S))
		{
			m_ePlayerDir = DIR_DS;
		}
		else
		{
			m_ePlayerDir = DIR_D;
		}

	}

	else if (pGameInstance->Key_Pressing(DIK_S))
	{
		if (pGameInstance->Key_Pressing(DIK_A))
		{
			m_ePlayerDir = DIR_SA;
		}
		else
		{
			m_ePlayerDir = DIR_S;
		}
	}
	else if (pGameInstance->Key_Pressing(DIK_A))
	{

		m_ePlayerDir = DIR_A;

	}
	else
	{
		m_iNextAnimationIndex = SIO_ANIM_BTL_MOVE_BRAKE;
		m_bLoop = false;
	}



	//ī�޶� ���� ����Ű ������ ī�޶� ���ϰ�  �� ���ϰ� 90�� -90? �ɋ����� �� . 
	//ī�޶� ���� �����UŰ ������ 90 




	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vGoalLook = vLook[m_ePlayerDir];
	//_float  fDot = XMVectorGetX(XMVector3Dot(vPlayerLook, vGoalLook)); // ���� ����� Ű�Է� ���� ���� ���� 
	//�������� -10~10�̸� ��ġ, -�� ���� +�� ������ 
	//�������� �����ΰ�..?

	//������ ��ġ�ϴ������� true false�� ��ȯ�ϰԲ�


	//



	if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_MOVE_RUN)
	{

		m_pTransformCom->Check_Right_Left(vGoalLook, TimeDelta*6.5f);
		m_pTransformCom->Go_Straight(TimeDelta);
	}


	if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_MOVE_BRAKE&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 5)
	{
		m_bLoop = false;
		m_bMove = false;

	}





	Safe_Release(pGameInstance);
}
//
//void CShionne::Rising_Palcon()
//{
//
//   if (!m_bStartSkill)
//   {
//      m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_HITENSYOUKU_START;
//      m_bStartSkill = true;
//      m_bLoop = false;
//      m_bJump = false;
//      m_dAnimSpeed = 1.75;
//
//   }
//
//
//   if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_HITENSYOUKU_START
//      &&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
//   {
//      m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_HITENSYOUKU_LOOP;
//   }
//
//   else if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_HITENSYOUKU_LOOP)
//   {
//      if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
//      {
//         m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
//         m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_HITENSYOUKU_END;
//         m_bLand = true;
//         m_bAir = false;
//      }
//      else {
//         m_pModelCom->Get_CurAnimation()->Reset_Animation();
//         m_pModelHairCom->Get_CurAnimation()->Reset_Animation();
//         m_pModelFaceCom->Get_CurAnimation()->Reset_Animation();
//         m_pModelSKLCom->Get_CurAnimation()->Reset_Animation();
//      }
//   }
//   else if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_HITENSYOUKU_END)
//   {
//
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 85)
//      {
//
//         if (m_bSkillKeyDown == true)
//         {
//            m_iNextAnimationIndex = 33;
//         }
//         else {
//            m_bSkill = false;
//            m_bStartSkill = false;
//         }
//      }
//   }
//
//   Infernal_Torrent();
//
//   //else if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_MAOUENGEKIHA_START2&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
//   //{
//   //   m_iNextAnimationIndex = 32;
//   //}
//
//   //else if (m_iCurrentAnimationIndex == 32 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 20)
//   //{
//
//   //   m_iNextAnimationIndex = 31;
//   //}
//
//   //else if (m_iCurrentAnimationIndex == 31
//   //   && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//   //{
//   //   m_bSkill = false;
//   //   m_bStartSkill = false;
//   //}
//
//
//
//   //
//
//
//   //
//
//
//}
//
//void CShionne::Hurricane_Thrust(_double TimeDelta)
//{
//
//
//   if (!m_bStartSkill)
//   {
//      m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_HUZINKEN;
//      m_bStartSkill = true;
//      m_bLoop = false;
//      m_bJump = false;
//      m_dAnimSpeed = 2.1;
//
//   }
//
//
//
//
//   if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_HUZINKEN)
//   {
//
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 90)
//      {
//
//         if (m_bSkillKeyDown == true)
//         {
//            m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_NEPPASENPUUZIN_START;
//         }
//         else {
//            m_bSkill = false;
//            m_bStartSkill = false;
//            m_dAnimSpeed = 1.85;
//         }
//      }
//   }
//
//   SeSIOng_Gale(TimeDelta);
//
//}
//
//void CShionne::Infernal_Torrent()
//{
//   if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_MAOUENGEKIHA_START2)
//   {
//      if (!m_bTorrent)
//      {
//         m_bTorrent = true;
//
//      }
//   }
//
//
//   if (m_iCurrentAnimationIndex == 33
//      && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
//   {
//
//      m_iNextAnimationIndex = 32;
//   }
//
//   if (m_iCurrentAnimationIndex == 32)
//   {
//      if (m_bSkillKeyDown)
//      {
//         m_iCurrentHp--;
//         if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//         {
//
//            m_iNextAnimationIndex = 31;
//         }
//      }
//      else {
//         m_iNextAnimationIndex = 31;
//      }
//
//   }
//
//   if (m_iCurrentAnimationIndex == 31
//      && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 90)
//   {
//
//      m_bSkill = false;
//      m_bStartSkill = false;
//      m_bTorrent = false;
//   }
//}
//
//void CShionne::SeSIOng_Gale(_double TimeDelta)
//{
//   if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_NEPPASENPUUZIN_START)
//   {
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 34)
//      {
//         m_iNextAnimationIndex = 40;
//
//      }
//
//      if (!m_bGale)
//      {
//         m_bGale = true;
//
//      }
//
//   }
//
//
//   if (m_iCurrentAnimationIndex == 40)
//   {
//      if (m_bSkillKeyDown)
//      {
//         m_iCurrentHp--;
//         if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//         {
//
//            m_iNextAnimationIndex = 39;
//         }
//      }
//      else {
//         m_iNextAnimationIndex = 39;
//      }
//
//   }
//
//
//   if (m_iCurrentAnimationIndex == 39)
//   {
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//      {
//
//         m_iNextAnimationIndex = 37;
//         m_dAnimSpeed = 10.0;
//      }
//   }
//
//
//   else if (m_iCurrentAnimationIndex == 37)
//   {
//
//      if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
//      {
//         m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
//         m_iNextAnimationIndex = 36;
//         m_dAnimSpeed = 2.4;
//      }
//
//      else {
//         m_pTransformCom->Go_Down(TimeDelta*2.f);
//         if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
//         {
//            m_dAnimSpeed = 10.0;
//            m_pModelCom->Get_CurAnimation()->Reset_Animation();
//            m_pModelHairCom->Get_CurAnimation()->Reset_Animation();
//            m_pModelFaceCom->Get_CurAnimation()->Reset_Animation();
//            m_pModelSKLCom->Get_CurAnimation()->Reset_Animation();
//         }
//      }
//
//   }
//
//
//   else if (m_iCurrentAnimationIndex == 36)
//   {
//      m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 150)
//      {
//         m_dAnimSpeed = 1.8;
//
//         m_bSkill = false;
//         m_bStartSkill = false;
//         m_bAir = false;
//         m_bJump = false;
//         m_bGale = false;
//         m_bLand = true;
//      }
//
//   }
//}
//
//void CShionne::Risiing_Phoenix()
//{
//
//
//   if (m_iCurrentAnimationIndex == 23)
//   {
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 50)
//      {
//         m_iNextAnimationIndex = 22;
//
//      }
//
//      if (!m_bPhoenix)
//      {
//         m_bPhoenix = true;
//
//      }
//
//   }
//   else if (m_iCurrentAnimationIndex == 22)
//   {
//      m_dAnimSpeed = 9.0;
//      if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
//      {
//         m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
//         m_iNextAnimationIndex = 21;
//         m_dAnimSpeed = 2.1;
//      }
//
//      else {
//         if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//         {
//            m_pModelCom->Get_CurAnimation()->Reset_Animation();
//            m_pModelHairCom->Get_CurAnimation()->Reset_Animation();
//            m_pModelFaceCom->Get_CurAnimation()->Reset_Animation();
//            m_pModelSKLCom->Get_CurAnimation()->Reset_Animation();
//         }
//      }
//
//   }
//
//
//   else if (m_iCurrentAnimationIndex == 21)
//   {
//      m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 59)
//      {
//
//         m_bSkill = false;
//         m_bStartSkill = false;
//         m_bAir = false;
//         m_bJump = false;
//         m_bPhoenix = false;
//         m_bLand = true;
//      }
//
//   }
//
//   //else {
//   //   m_bSkill = false;
//
//   //}
//}
//
//void CShionne::Mirage()
//{
//   if (!m_bStartSkill)
//   {
//      m_iNextAnimationIndex = 29;
//      m_bStartSkill = true;
//      m_bLoop = false;
//
//   }
//
//
//   if (m_iCurrentAnimationIndex == 29
//      && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
//   {
//      m_iNextAnimationIndex = 28;
//   }
//
//   if (m_iCurrentAnimationIndex == 28 && XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
//   {
//      m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
//      m_iNextAnimationIndex = 27;
//   }
//
//   if (m_iCurrentAnimationIndex == 27
//      && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//   {
//      m_bSkill = false;
//      m_bStartSkill = false;
//   }
//}
//
//void CShionne::Rending_Flash()
//{
//   if (!m_bStartSkill)
//   {
//      m_iNextAnimationIndex = 58;
//      m_bStartSkill = true;
//      m_bLoop = false;
//
//   }
//
//
//   if (m_iCurrentAnimationIndex == 58
//      && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
//   {
//      m_iNextAnimationIndex = 57;
//   }
//
//   if (m_iCurrentAnimationIndex == 57 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 20)
//   {
//
//      m_iNextAnimationIndex = 56;
//   }
//
//   if (m_iCurrentAnimationIndex == 56
//      && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//   {
//      m_bSkill = false;
//      m_bStartSkill = false;
//   }
//}
//
//void CShionne::Explosive_Ring()
//{
//   if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_START)
//   {
//      if (!m_bRing) {
//         m_bRing = true;
//
//      }
//   }
//
//
//   if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_START
//      && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
//   {
//
//      m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_LOOP;
//   }
//
//   if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_LOOP)
//   {
//      if (m_bSkillKeyDown)
//      {
//         m_iCurrentHp--;
//         if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//         {
//
//            m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_END;
//         }
//      }
//      else {
//         m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_END;
//      }
//
//   }
//
//   if (m_iCurrentAnimationIndex == SIO_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_END
//      && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 233)
//   {
//
//      m_bSkill = false;
//      m_bStartSkill = false;
//      m_bTorrent = false;
//   }
//}
//
//void CShionne::Lightning_Thrust()
//{
//
//
//   if (m_bEvade == false && m_bJump == false && m_bLand)
//   {
//      m_bMove = false;
//      m_bAttack = false;
//
//
//      if (!m_bStartSkill)
//      {
//         m_iNextAnimationIndex = 65;
//         m_dAnimSpeed = 2.1;
//         m_bStartSkill = true;
//         m_bLoop = false;
//
//      }
//   }
//
//
//
//
//   if (m_iCurrentAnimationIndex == 65)
//   {
//      m_bAir = true;
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 58)
//      {
//         if (m_bSkillKeyDown == true)
//         {
//            m_iNextAnimationIndex = 23;
//            m_dAnimSpeed = 2.1;
//
//         }
//         else {
//            m_iNextAnimationIndex = SIO_ANIM_BTL_FALL;
//            m_dAnimSpeed = 1.85;
//            m_bSkill = false;
//         }
//
//
//      }
//   }
//
//   Risiing_Phoenix();
//   //else if (m_iCurrentAnimationIndex == 23)
//   //{
//   //   if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 50)
//   //   {
//   //      m_iNextAnimationIndex = 22;
//
//   //   }
//
//   //}
//   //else if (m_iCurrentAnimationIndex == 22)
//   //{
//   //   m_dAnimSpeed = 9.0;
//   //   if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
//   //   {
//   //      m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
//   //      m_iNextAnimationIndex = 21;
//   //      m_dAnimSpeed = 1.75;
//   //   }
//
//   //   else {
//   //      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//   //      {
//   //         m_pModelCom->Get_CurAnimation()->Reset_Animation();
//   //         m_pModelHairCom->Get_CurAnimation()->Reset_Animation();
//   //         m_pModelFaceCom->Get_CurAnimation()->Reset_Animation();
//   //         m_pModelSKLCom->Get_CurAnimation()->Reset_Animation();
//   //      }
//   //   }
//
//   //}
//
//
//   //else if (m_iCurrentAnimationIndex == 21)
//   //{
//   //   m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
//   //   if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
//   //   {
//
//   //      m_bSkill = false;
//   //      m_bStartSkill = false;
//   //      m_bAir = false;
//   //      m_bJump = false;
//   //   }
//
//   //}
//
//   //else {
//   //   m_bSkill = false;
//
//   //}
//}
//
//void CShionne::Swallow_Balde()
//{
//
//   if (m_bEvade == false && m_bJump == false)
//   {
//      m_bMove = false;
//      m_bAttack = false;
//
//
//      if (!m_bStartSkill)
//      {
//         m_iNextAnimationIndex = 17;
//
//         m_bStartSkill = true;
//         m_bLoop = false;
//
//      }
//   }
//
//
//
//
//   if (m_iCurrentAnimationIndex == 17)
//   {
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 85)
//      {
//         if (m_bSkillKeyDown == true)
//         {
//            m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_MAOUENGEKIHA_START2;
//
//         }
//         else {
//            m_bSkill = false;
//         }
//
//
//      }
//   }
//
//   Infernal_Torrent();
//}
//
//void CShionne::Dragon_Swarm()
//{
//   if (m_bEvade == false && m_bJump == false)
//   {
//      m_bMove = false;
//      m_bAttack = false;
//
//
//      if (!m_bStartSkill)
//      {
//         m_iNextAnimationIndex = 64;
//
//         m_bStartSkill = true;
//         m_bLoop = false;
//
//      }
//   }
//
//
//
//
//   if (m_iCurrentAnimationIndex == 64)
//   {
//      if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 90)
//      {
//         if (m_bSkillKeyDown == true)
//         {
//            m_iNextAnimationIndex = SIO_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_START;
//
//         }
//         else {
//            m_bSkill = false;
//         }
//
//
//      }
//   }
//
//   Explosive_Ring();
//
//}

void CShionne::Compute_Look()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
}

void CShionne::Compute_Hit(_double TimeDelta)
{

	if (m_bDamage_Small_b)
	{
		m_iNextAnimationIndex = SIO_ANIM_BTL_DAMAGE_SMALL_B;
		m_bLoop = false;
		m_bInvincible = true;
	}


	if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
	{
		m_bHit = false;
		m_bDamage_Small_b = false;


		//��� ���� �ʱ�ȭ?
		m_bInvincible = false;
		m_bAttack = false;
		m_bAirAttack = false;
		m_bSkill = false;
		m_bJump = false;
		m_bEvade = false;
		m_bEvadeBack = false;


		if (m_bAir)
		{
			m_iNextAnimationIndex = SIO_ANIM_BTL_FALL;
		}
		else {
			m_iNextAnimationIndex = SIO_ANIM_BTL_MOVE_IDLE;
			m_bLand = true; //���� �÷��� �Ҽ���?
		}

	}


}

void CShionne::Lunar_Blast()
{
	if (m_bEvade == false && m_bJump == false)
	{
		m_bMove = false;
		m_bAttack = false;


		if (!m_bStartSkill)
		{
			m_iNextAnimationIndex = 7;
			m_dAnimSpeed = 1.8;

			m_bStartSkill = true;
			m_bLoop = false;

		}
	}




	if (m_iCurrentAnimationIndex == 7)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 152)
		{


			m_bSkill = false;
			m_bStartSkill = false;
		}


	}
}


void CShionne::Gravitas_Field()
{
	if (m_bEvade == false && m_bJump == false)
	{
		m_bMove = false;
		m_bAttack = false;


		if (!m_bStartSkill)
		{
			m_iNextAnimationIndex = 12;
			m_dAnimSpeed = 2.0;

			m_bStartSkill = true;
			m_bLoop = false;

		}
	}




	if (m_iCurrentAnimationIndex == 12)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 160)
		{

			m_bSkill = false;
			m_bStartSkill = false;
		}


	}
}

void CShionne::Annihilation()
{
	if (m_bEvade == false && m_bJump == false)
	{
		m_bMove = false;
		m_bAttack = false;


		if (!m_bStartSkill)
		{
			m_iNextAnimationIndex = 34;
			m_dAnimSpeed = 2.1;

			m_bStartSkill = true;
			m_bLoop = false;

		}
	}




	if (m_iCurrentAnimationIndex == 34)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 150)
		{
			m_bSkill = false;
			m_bStartSkill = false;
		}


	}
}

void CShionne::Displode()
{

	if (!m_bStartSkill)
	{
		m_iNextAnimationIndex = 3;
		m_bStartSkill = true;
		m_bLoop = false;
		m_bJump = false;
		m_dAnimSpeed = 2.0;

	}




	if (m_iCurrentAnimationIndex == 3)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 60)
		{

			m_bSkill = false;
			m_bStartSkill = false;

		}
	}


}

void CShionne::Aqeous_Impact()
{



	if (!m_bStartSkill)
	{
		m_iNextAnimationIndex = 37;
		m_bStartSkill = true;
		m_bLoop = false;
		m_bJump = false;
		m_dAnimSpeed = 1.8;

	}


	if (m_iCurrentAnimationIndex == 37
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
	{
		m_iNextAnimationIndex = 36;
		//m_bSkill = false;
		//m_bStartSkill = false;
		m_dAnimSpeed = 2.5;
	}

	else if (m_iCurrentAnimationIndex == 36)
	{
		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
		{
			m_dAnimSpeed = 2.1;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
			m_iNextAnimationIndex = 35;
			m_bLand = true;
			m_bAir = false;
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
		{
			m_dAnimSpeed += 0.2;
			m_pModelCom->Get_CurAnimation()->Reset_Animation();
			m_pModelHairCom->Get_CurAnimation()->Reset_Animation();
			m_pModelFaceCom->Get_CurAnimation()->Reset_Animation();
			m_pModelSKLCom->Get_CurAnimation()->Reset_Animation();
		}
	}
	else if (m_iCurrentAnimationIndex == 35)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 70)
		{
			m_bSkill = false;
			m_bStartSkill = false;
		}
	}
}

void CShionne::Spear_Sweep()
{
	if (!m_bStartSkill)
	{
		m_iNextAnimationIndex = 50;
		m_bStartSkill = true;
		m_bLoop = false;
		m_bJump = false;
		m_dAnimSpeed = 2.2;

	}




	if (m_iCurrentAnimationIndex == 50)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 70)
		{

			m_bSkill = false;
			m_bStartSkill = false;

		}
	}
}



void CShionne::Dragon_Swarm()
{
	if (m_bEvade == false && m_bJump == false)
	{
		m_bMove = false;
		m_bAttack = false;


		if (!m_bStartSkill)
		{
			m_iNextAnimationIndex = 64;

			m_bStartSkill = true;
			m_bLoop = false;

		}
	}


}


CShionne * CShionne::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CShionne*   pInstance = new CShionne(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CShionne"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShionne::Clone(void * pArg)
{
	CShionne*   pInstance = new CShionne(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone CShionne"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShionne::Free()
{
	__super::Free();
}