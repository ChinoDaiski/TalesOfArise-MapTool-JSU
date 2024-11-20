#include "stdafx.h"
#include "..\Public\Fire_Deco.h"
#include "GameInstance.h"
#include "PIpeLine.h"

_uint CFire_Deco::m_iFireNumber = 0;

CFire_Deco::CFire_Deco(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CFire_Deco::CFire_Deco(const CFire_Deco& rhs)
	:CGameObject(rhs)
{
}

HRESULT CFire_Deco::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CFire_Deco::NativeConstruct(void* pArg)
{
	if (nullptr == pArg)
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.SpeedPerSec = 1.f;
	TransformDesc.fScalePerSec = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	TransformDesc.RotationPerSec = 1.f;

	m_pTransformCom->Set_TransformDesc(TransformDesc);


	FIREDECODESC tFireDesc;
	memcpy(&tFireDesc, pArg, sizeof(FIREDECODESC));

	if (FAILED(SetUp_Components(tFireDesc.tagTextureCom)))
	{
		return E_FAIL;
	}

	m_iMaxFrame = m_pTextureCom->Get_TextureCnt();
	m_FrameSpeed = tFireDesc.iFrameSpeed;
	m_iShaderPass = tFireDesc.iShaderPass;

	_matrix WorldMatrix = XMMatrixIdentity();

	WorldMatrix.r[2] = XMVector3Normalize(XMLoadFloat3(&tFireDesc.vLook)) * tFireDesc.vScale.z;		// Look
	WorldMatrix.r[0] = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), WorldMatrix.r[2])) * tFireDesc.vScale.x;	// Right
	WorldMatrix.r[1] = XMVector3Normalize(XMVector3Cross(WorldMatrix.r[2], WorldMatrix.r[0])) * tFireDesc.vScale.y;	// Up
	WorldMatrix.r[3] = XMVectorSetW(XMLoadFloat3(&tFireDesc.vPosition), 1.f);

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

	m_fOriginalStrength = tFireDesc.fOriginalStrength;
	m_fGlowStrength = tFireDesc.fGlowStrength;

	// ������ �� ���� ��ȣ�� �ϳ��� ������Ų��.
	m_iCurFireNumber = ++m_iFireNumber;

	// ȸ�� ����, �ش簪��ŭ ȸ���Ѵ�.
	m_fRotation = tFireDesc.fRotation;
	m_pTransformCom->RotationXYZ(XMConvertToRadians(m_fRotation.x), XMConvertToRadians(m_fRotation.y), XMConvertToRadians(m_fRotation.z));

	m_fScale = tFireDesc.vScale;

	return S_OK;
}

void CFire_Deco::Tick(_double TimeDelta)
{
	// Ÿ���� �����Ѵٸ�
	if (nullptr != m_pTarget) {
		CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(TEXT("Com_Transform")));
		_vector vTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_vector vLook = vTargetPos - vPos;
		vLook = XMVector3Normalize(vLook);
		vLook = XMVectorSetY(vLook, 0.f);
		_float3 fScale = m_fScale;

		_matrix WorldMatrix = XMMatrixIdentity();

		WorldMatrix.r[2] = vLook * fScale.z;		// Look
		WorldMatrix.r[0] = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), WorldMatrix.r[2])) * fScale.x;	// Right
		WorldMatrix.r[1] = XMVector3Normalize(XMVector3Cross(WorldMatrix.r[2], WorldMatrix.r[0])) * fScale.y;	// Up
		WorldMatrix.r[3] = XMVectorSetW(vPos, 1.f);

		m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	}


	m_Frame += m_FrameSpeed * TimeDelta;
	if (m_iMaxFrame <= m_Frame)
	{
		m_Frame = 0.0;
	}

	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CFire_Deco::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
	{
		return;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 5.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CFire_Deco::Render()
{
	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iShaderPass);
	m_pVIBufferCom->Render();

//#ifdef _DEBUG
//	m_pSphereCom->Render();
//#endif // _DEBUG

	return S_OK;
}

HRESULT CFire_Deco::SetUp_Components(_tchar* tagTextureCom)
{
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Fire"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_MapTool, tagTextureCom, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 3.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFire_Deco::SetUp_ConstantTable()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float fCamFar = pGameInstance->Get_CamFar();
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCamFar", &fCamFar, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_OriginalStrength", &m_fOriginalStrength, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_GlowStrength", &m_fGlowStrength, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DepthTexture", pGameInstance->Get_RenderTargetSRV(TEXT("Target_Depth")))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_ImageTexture", (_uint)m_Frame)))
		return E_FAIL;

	return S_OK;
}

CFire_Deco* CFire_Deco::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CFire_Deco* pInstance = new CFire_Deco(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CFire_Deco : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CFire_Deco::Clone(void* pArg)
{
	CFire_Deco* pInstance = new CFire_Deco(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CFire_Deco : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFire_Deco::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

	--m_iFireNumber;
}