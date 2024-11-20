#include "stdafx.h"
#include "UIBar.h"

//ÀüÅõ
UIBar::UIBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UIBar::UIBar(const UIBar& rhs)
	: CUI(rhs)
{
}

HRESULT UIBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIBar::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	m_fDistanceToCamera = 40;
	m_iShaderPass = 3;
	return S_OK;
}

void UIBar::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);
	if (GetKeyState(VK_F8) & 0x8000)
	{
		Set_Dead(1);
	}

	/*time += TimeDelta;

	if (time > 2)
	{
	m_isRenderOff = true;
	}

	if (m_isRenderOff)
	{
	if (rgb <= 0) rgb = 0;
	rgb -= 0.02;
	}
	else
	{
	if (!m_isFirst)
	rgb += 0.008f;

	if (rgb > 0.8) {
	rgb = 0.8;
	m_isFirst = true;

	}
	}
	*/

	pGameInstnace->Release();
}

void UIBar::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT UIBar::Render()
{
	if (!m_isBattleOn) {
		//if (FAILED(m_pShaderCom->Set_RawValue("exuirgb", &rgb, sizeof(float))))
		//	return E_FAIL;




		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT UIBar::SetUp_Texture()
{
	//Prototype_Component_Texture_HPBar

	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_HPBar", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To UIBar : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIBar * UIBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIBar* pInstance = new UIBar(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UIBar : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIBar::Clone(void * pArg)
{
	UIBar* pInstance = new UIBar(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIBar : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIBar::Free()
{
	__super::Free();

}
