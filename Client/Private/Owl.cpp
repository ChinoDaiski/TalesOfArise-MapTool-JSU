#include "stdafx.h"
#include "..\Public\Owl.h"

#include "GameInstance.h"

COwl::COwl(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

COwl::COwl(const COwl & rhs)
	: CGameObject(rhs)
{
}

HRESULT COwl::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT COwl::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_AnimationIndex(m_iCurrentAnimationIndex);

	CGameInstance*pGameInstance = CGameInstance::GetInstance();


	return S_OK;
}

void COwl::Tick(_double TimeDelta)
{
}

void COwl::LateTick(_double TimeDelta)
{
}

HRESULT COwl::Render()
{
	return S_OK;
}

HRESULT COwl::SetUp_Components()
{
	return S_OK;
}

HRESULT COwl::SetUp_ConstantTable()
{
	return S_OK;
}

COwl * COwl::Create(ID3D11Device * pDeviceOut, ID3D11DeviceContext * pDeviceContextOut)
{
	return nullptr;
}

CGameObject * COwl::Clone(void * pArg)
{
	return nullptr;
}

void COwl::Free()
{
}

void COwl::Check_TargetDistance(void)
{
}
