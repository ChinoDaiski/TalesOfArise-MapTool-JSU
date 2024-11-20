#include"stdafx.h"
#include"UIStatus.h"

CUIStatus::CUIStatus(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUIStatus::CUIStatus(const CUIStatus& rhs)
	: CUI(rhs)
{
}

HRESULT CUIStatus::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUIStatus::NativeConstruct(void * pArg)
{


	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}


	m_iSprite = ((STATUSDESC*)pArg)->kind;


	m_fDistanceToCamera = 89.5;

	m_iShaderPass = 19;

	return S_OK;
}

void CUIStatus::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_fPercentage = cp / 700;

	if (cp <= 10)
		cp = 10;

	if (cp > 680)
		cp = 680;

	if (cp < 300) {
		m_isRed = true;
	}
	else
		m_isRed = false;

	if (GetKeyState('H') & 0x8000)
	{

		cp -= 10;

	}
	if (GetKeyState('J') & 0x8000)
	{

		cp += 10;
	}




}

void CUIStatus::LateTick(_double TimeDelta)
{

	__super::LateTick(TimeDelta);

}

HRESULT CUIStatus::Render()
{

	//battleOn
	if (!m_isBattleOn) {
		m_pShaderCom->Set_RawValue("g_fPercentage", &m_fPercentage, sizeof(_float));
		m_pShaderCom->Set_RawValue("m_isRed", &m_isRed, sizeof(bool));
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CUIStatus::SetUp_Texture()
{
	//dd.png 썼음 조심. 뭐가문제라면 얘가 범인일지도
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_GlowUI", (CComponent**)&m_pTextureCom)))
	{


		MSG_BOX(L"Failed To UIName : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUIStatus * CUIStatus::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUIStatus* pInstance = new CUIStatus(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UINAme : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUIStatus::Clone(void * pArg)
{
	CUIStatus* pInstance = new CUIStatus(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIName : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIStatus::Free()
{
	__super::Free();

}
