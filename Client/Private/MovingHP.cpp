#include "stdafx.h"
#include "MovingHP.h"
#include "GameInstance.h"

MovingHP::MovingHP(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

MovingHP::MovingHP(const MovingHP & rhs)
	: CGameObject(rhs)	
	, m_ProjMatrix(rhs.m_ProjMatrix)
	
{
}

HRESULT MovingHP::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;	
	   XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));

	return S_OK;
}

HRESULT MovingHP::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC      TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;








	return S_OK;


}

void MovingHP::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CTransform* vampireTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vampire = vampireTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vampirepos;
	XMStoreFloat3(&vampirepos, vampire);


	_matrix ViewMatrix, ProjMatrix;
	_float4x4 WindowMatrix, WorldMatrix;

	ViewMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);

	XMStoreFloat4x4(&WorldMatrix, vampireTransform->Get_WorldMatrix());


	ProjMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	XMStoreFloat4x4(&WindowMatrix, XMLoadFloat4x4(&WorldMatrix)*ViewMatrix*ProjMatrix);


	_float3 vWindowPos = *(_float3*)&WindowMatrix.m[3][0];

	vWindowPos.x /= vWindowPos.z;
	vWindowPos.y /= vWindowPos.z;

	m_fX = vWindowPos.x*g_iWinCX / 2 + g_iWinCX / 2 ;
	m_fY = vWindowPos.y*-1 * g_iWinCY / 2 + g_iWinCY / 2;



	m_pTransformCom->Scaled(_float3(190, 8, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f, 1.f));



	RELEASE_INSTANCE(CGameInstance);


}

void MovingHP::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);

	


}

HRESULT MovingHP::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;


	_float4x4   ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	_float4x4   ProjMatrixTP;
	XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	//apply 하기전에 값을 던저야하므로 begin전에 숫자를 던지자.
	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT MovingHP::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_GreenHP"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

MovingHP * MovingHP::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	MovingHP*	pInstance = new MovingHP(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created MovingHP"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * MovingHP::Clone(void * pArg)
{
	MovingHP*	pInstance = new MovingHP(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created MovingHP"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void MovingHP::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
