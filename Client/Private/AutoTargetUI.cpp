#include"stdafx.h"
#include"AutoTargetUI.h"

AutoTargetUI::AutoTargetUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

AutoTargetUI::AutoTargetUI(const AutoTargetUI& rhs)
	:CUI(rhs)
{
}

HRESULT AutoTargetUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	

	return S_OK;
}

HRESULT AutoTargetUI::NativeConstruct(void * pArg)
{


	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}


	m_tUIInfo.fX = g_iWinCX / 2 + 580;
	m_tUIInfo.fY = g_iWinCY / 2 - 130;
	m_tUIInfo.fCX = 45;
	m_tUIInfo.fCY = 45;


	m_fDistanceToCamera = 89.3;
	m_iShaderPass = 0;

	return S_OK;
}

void AutoTargetUI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	
	

	
}

void AutoTargetUI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT AutoTargetUI::Render()
{
	if (!m_isBattleOn) {
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT AutoTargetUI::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_SmashNumTexture", (CComponent**)&m_pTexture)))
	{

		
		MSG_BOX(L"Failed To UIName : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

AutoTargetUI * AutoTargetUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	AutoTargetUI* pInstance = new AutoTargetUI(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UINAme : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}	
	return pInstance;
}

CGameObject * AutoTargetUI::Clone(void * pArg)
{
	AutoTargetUI* pInstance = new AutoTargetUI(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIName : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void AutoTargetUI::Free()
{
}
