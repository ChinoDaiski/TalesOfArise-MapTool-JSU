#include "stdafx.h"
#include "..\Public\LightObject.h"
#include "GameInstance.h"
#include "ImGUI_Manager.h"

CLightObject::CLightObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CLightObject::CLightObject(const CLightObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLightObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLightObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLightObject::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (nullptr != m_pTargetLight) {
		LIGHTDESC LightDesc = *m_pTargetLight->Get_LightDesc();
		
		_matrix matWorld = XMMatrixIdentity();
		_float4x4 matWorld4x4;
		XMStoreFloat4x4(&matWorld4x4, matWorld);

		memcpy(&matWorld4x4.m[CTransform::STATE_POSITION][0], &LightDesc.vPosition, sizeof(_float4));

		m_pSphereCom->Update(XMLoadFloat4x4(&matWorld4x4));
	}
}

void CLightObject::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom) {
		m_pRendererCom->Add_DebugComponent(m_pSphereCom);
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CLightObject::Render()
{
#ifdef _DEBUG
	if (CImGUI_Manager::GetInstance()->Get_Level() == CImGUI_Manager::UL_Lights)
		m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CLightObject::SetUp_Components(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	LIGHTOBJDesc LightObjDesc = *((LIGHTOBJDesc*)pArg);

	m_pTargetLight = LightObjDesc.pTargetLight;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	LightObjDesc.fColliderSize = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vSize = LightObjDesc.fColliderSize;
	ColliderDesc.fRadius = ColliderDesc.vSize.y * 0.5f;
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Sphere"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CLightObject * CLightObject::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLightObject*	pInstance = new CLightObject(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CLightObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLightObject::Clone(void * pArg)
{
	CLightObject*	pInstance = new CLightObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CLightObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLightObject::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphereCom);
}