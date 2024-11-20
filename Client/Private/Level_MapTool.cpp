#include "stdafx.h"
#include "..\Public\Level_MapTool.h"
#include "ImGUI_Manager.h"

#include "GameInstance.h"
#include "Camera_MapTool.h"
#include "MapObject.h"
#include "Terrain.h"
#include "Picking_Rect.h"

#include "Fire_Deco.h"

CLevel_MapTool::CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_MapTool::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_MAPTOOL_CAMERA)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Skybox()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_BattleMap(LAYER_MAPTOOL_BattleMap)))
	//	return E_FAIL;

	if (FAILED(Ready_Picking_Rect()))
		return E_FAIL;
	

	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);
	pImgui->OnOffImgui();
	RELEASE_INSTANCE(CImGUI_Manager);

	return S_OK;
}

void CLevel_MapTool::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);
	pImgui->Tick(TimeDelta);
	RELEASE_INSTANCE(CImGUI_Manager);
#endif //_DEBUG
}

HRESULT CLevel_MapTool::Render()
{
	SetWindowText(g_hWnd, L"MapTool");

#ifdef _DEBUG
	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);
	pImgui->Render();
	RELEASE_INSTANCE(CImGUI_Manager);
#endif //_DEBUG

	return S_OK;
}

CLevel_MapTool* CLevel_MapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_MapTool* pInstance = new CLevel_MapTool(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"Failed To CLevel_MapTool : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_MapTool::Free()
{
	__super::Free();
}

HRESULT CLevel_MapTool::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(5.f, -1.f, 5.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 0.f);
	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	//LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	//LightDesc.vDirection = _float4(-5.f, -1.f, -5.f, 0.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 0.f);
	//if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Skybox()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_Skybox, TEXT("Prototype_GameObject_Sky")))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 100.f, 0.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 50.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 10000.f;
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	CameraDesc.eType = CCamera::CAMERA;

	CCamera_MapTool* pCamera = dynamic_cast<CCamera_MapTool*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, pLayerTag, TEXT("Prototype_GameObject_Camera_MapTool"), &CameraDesc));

	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.SpeedPerSec = 100.f;
	transformDesc.RotationPerSec = 90.f;

	dynamic_cast<CTransform*>(pCamera->Get_Component(TEXT("Com_Transform")))->Set_TransformDesc(transformDesc);

#ifdef _DEBUG
	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);
	pImgui->Set_Camera(pCamera);
	RELEASE_INSTANCE(CImGUI_Manager);
#endif //_DEBUG

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_BattleMap(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//CMapObject::MAPOBJDesc Desc;
	//Desc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_BattleMap_01");
	//XMStoreFloat4x4(&Desc.TransformMatrix, XMMatrixIdentity());

	//pGameInstance->Add_GameObjectToLayer(LEVEL_TEST, pLayerTag, TEXT("Prototype_GameObject_MapObject"), &Desc);

	CFire_Deco::FIREDECODESC Desc;
	Desc.fGlowStrength = 1.f;
	Desc.fOriginalStrength = 1.f;
	Desc.iFrameSpeed = 16;
	Desc.iShaderPass = 2;
	Desc.tagTextureCom = TEXT("Prototype_Component_Texture_Fire0");
	Desc.vLook = _float3(1.f, 0.f, 0.f);
	Desc.vPosition = _float3(0.f, 10.f, 0.f);
	Desc.vScale = _float3(5.f, 5.f, 1.f);
	pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, pLayerTag, TEXT("Prototype_GameObject_Fire"), &Desc);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Picking_Rect()
{	
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPicking_Rect::Picking_RectDESC Desc;
	Desc.fSize = _float2(5000, 5000.f);
	Desc.vPos = _float3(0.f, 0.f, 0.f);

	CPicking_Rect* pPicking_Rect = dynamic_cast<CPicking_Rect*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_Picking_Rect, TEXT("Prototype_GameObject_Picking_Rect"), &Desc));

	if (nullptr == pPicking_Rect)
		return E_FAIL;

	CImGUI_Manager::GetInstance()->Set_PickingRect(pPicking_Rect);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
