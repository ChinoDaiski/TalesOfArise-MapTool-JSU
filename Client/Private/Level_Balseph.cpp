#include "stdafx.h"
#include "..\Public\Level_Balseph.h"

#include "Sword.h"
#include "GameInstance.h"
#include "Camera_Default.h"
#include"..\Public\UIBackground.h"
#include"..\Public\UIEmblem.h"
#include"..\Public\UIExplain.h"
#include"..\Public\UI_Smash.h"
#include"..\Public\UI_SmashEffect.h"
#include"..\Public\UILine.h"
#include"..\Public\UINumber.h"
#include"..\Public\SmashNum.h"
#include"..\Public\UIName.h"
#include"..\Public\UIHpBar.h"
#include"..\Public\UIBar.h"

#include"UI_Manager.h"
#include"LineMsgUI.h"
#include"UIHpBarRed.h"
#include"UI_Manager.h"
#include"TutorialMsg.h"
#include"MiniMap.h"
#include"TutoLineMsg.h"
#include"LineSprite.h"
#include "Layer.h"	//�̰� ���ν��Ͻ��� �־�����.
#include"MiniMapView.h"
#include"UIGlow.h"
#include"UIBillBoard.h"
#include "UIStatus.h"
#include "MapObject.h"
#include "WayPoint.h"




CLevel_Balseph::CLevel_Balseph(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
	, m_pBattle_Manager(CBattle_Manager::GetInstance())
{
	Safe_AddRef(m_pBattle_Manager);
}

HRESULT CLevel_Balseph::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;	

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Balseph(TEXT("Layer_Balseph"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_FireAvatar(TEXT("Layer_FireAvatar"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Mantis(TEXT("Layer_Mantis"))))
	//	return E_FAIL;

	/*if (FAILED(Ready_Layer_Punisher(TEXT("Layer_Punisher"))))
		return E_FAIL;*/

	//if (FAILED(Ready_Layer_Boar(TEXT("Layer_Boar"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Practice(TEXT("Layer_Practice"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	if (FAILED(Ready_Fonts()))
		return E_FAIL;

	// �� ����
	if (FAILED(Ready_Map("../Bin/Resources/Dat/Stage03/", "MeshList.dat")))
		return E_FAIL;

	return S_OK;
}

void CLevel_Balseph::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (nullptr == pGameInstance)
		return;

	m_pBattle_Manager->Tick(TimeDelta);


	//LineMsg���õ� �ڵ�.
	if (LineMsgCreate)
	{
		LineMsgUI::MSGLINEUIDESC LineMsgDESC;
		LineMsgDESC.uidesc.fX = g_iWinCX / 2;
		LineMsgDESC.uidesc.fY = g_iWinCX / 2 - 700;
		LineMsgDESC.uidesc.fCX = 700;
		LineMsgDESC.uidesc.fCY = 250;
		LineMsgDESC.sprite = LineMsgSprite;  //1���� ���� ������ �ִ�.

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_LineMsgUI"), &LineMsgDESC)))
			return;

		LineMsgCreate = false;
	}
	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD7))
	{

		//���Ű�. �ؼ� �̷��� �� �ٲ���ƾ���. �ش� ���Ͷ� �΋H������ ���� �ٸ��� ������Ű�� �ɵ�.
		LineMsgCreate = true;


	}

	//��Ƽ������ ������ �غ��ϼ��� �ڵ�.
	if (pGameInstance->Get_DIKeyState(DIK_F10))
	{

		//Line
		UILine::LINEDESC linedesc;
		linedesc.uidesc.fX = 0;
		linedesc.uidesc.fY = g_iWinCX / 2 - 700;
		linedesc.uidesc.fCX = 1920;
		linedesc.uidesc.fCY = 80;
		linedesc.kind = 1;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_Line"), &linedesc)))
			return;


		//���� �ִ� TutoLine���� ���� ���� �޽��� �̸��׸� ��Ƽ������ ������ �غ��ϼ���.
		//5 - worldŽ�� 6- �߼����� 7 - ��Ƽ������ ���� 
		//�׳� ��Ŭ���� ���弼��
		//TutoLineMsg
		TutoLineMsg::MSGLINEUIDESC TutoLineMsgDesc;
		TutoLineMsgDesc.uidesc.fX = g_iWinCX / 2;
		TutoLineMsgDesc.uidesc.fY = g_iWinCX / 2 - 750;
		TutoLineMsgDesc.uidesc.fCX = 500;
		TutoLineMsgDesc.uidesc.fCY = 70;
		TutoLineMsgDesc.sprite = 1;

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_TutoLineMsg"), &TutoLineMsgDesc)))
			return;


		////LineSprite
		LineSprite::MINIMAPDESC LineSpritedesc;
		LineSpritedesc.uidesc.fX = g_iWinCX / 2;
		LineSpritedesc.uidesc.fY = g_iWinCX / 2 - 750;
		LineSpritedesc.uidesc.fCX = 400;
		LineSpritedesc.uidesc.fCY = 400;
		LineSpritedesc.kind = 0;

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
			return;

	}

	//Tutorial ���� �� �������� �Լ�.
	if (pGameInstance->Get_DIKeyState(DIK_F11))
	{
		m_isBattle = false;
		//Tutorial ���� �� ��������
		CLayer* TutorialLayer = pGameInstance->Find_Layer(LEVEL_TUTORIAL, TEXT("Layer_TutorialUI"));
		list<CGameObject*>& TutorialList = TutorialLayer->Get_ObjectList();

		for (auto& iter : TutorialList)
		{
			//����
			dynamic_cast<CUI*>(iter)->RenderChange(false);
		}
	}



	//NUMPAD5 ��Ʋ UI�� �����ع����� �Լ�
	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD5))
	{

		if (!m_isBattleOn) {
			if (FAILED(Ready_Layer_Battle_UI()))//Tutorial Layer
				return;
			m_isBattleOn = true;

		}
	}

	//F12������, TutorialLayer �� �ѹ����� �Լ�
	if (pGameInstance->Get_DIKeyState(DIK_F12))
	{

		//tutolevel �� �ѹ�����
		CLayer* TutorialLayer = pGameInstance->Find_Layer(LEVEL_TUTORIAL, TEXT("Layer_TutorialUI"));
		list<CGameObject*>& TutorialList = TutorialLayer->Get_ObjectList();
		m_isBattle = false;
		for (auto& iter : TutorialList)
		{
			//����
			dynamic_cast<CUI*>(iter)->RenderChange(true);
		}

	}


	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD3))
	{

		//SmashUI alphen
		CUI_SmashEffect::UIINFO uidesc;

		uidesc.fX = 190;
		uidesc.fY = 712;
		uidesc.fCX = 10;
		uidesc.fCY = 10;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
			return;

	}
	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD4))
	{

		CUI_SmashEffect::UIINFO uidesc;

		ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
		uidesc.fX = 130;
		uidesc.fY = 777;
		uidesc.fCX = 140;
		uidesc.fCY = 150;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
			return;


	}

	Safe_Release(pGameInstance);

}

HRESULT CLevel_Balseph::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("Ʃ�丮�� �����Դϴ�."));

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (m_isBattle) {

		wsprintf(HitMsg, TEXT("HITS"));
		if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan40"), HitMsg, _float2(1787, 217/*g_iWinCX / 2 + 700, g_iWinCY / 2 - 200*/), XMVectorSet(0.9f, 1.f, 0.7f, 1.f))))
			return E_FAIL;



		wsprintf(DamageMsg, TEXT("DAMAGES"));
		if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan40"), DamageMsg, _float2(1747, 267/*g_iWinCX / 2 + 650, g_iWinCY / 2 - 150*/), XMVectorSet(0.9f, 1.f, 0.7f, 1.f))))
			return E_FAIL;

	}
	
	return S_OK;

}


HRESULT CLevel_Balseph::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 0.f);
	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float4(10.f, 4.f, 10.f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.0f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);	
	//LightDesc.vSpecular = _float4(1.f, 0.0f, 0.0f, 1.f);
	//if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;	

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float4(15.f, 4.f, 10.f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.0f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	//LightDesc.vSpecular = _float4(0.f, 1.f, 0.0f, 1.f);
	//if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.eType = CCamera::CAMERA;
	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc))
		return E_FAIL;

	//for.lightdepth
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.eType = CCamera::LIGHT;
	CameraDesc.vEye = _float3(0.f, 20.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 500.f;
	CameraDesc.fFovy = XMConvertToRadians(120.f);
	CameraDesc.fAspect = (_float)g_iLightCX / g_iLightCY;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_LightDepth"), &CameraDesc))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Alphen")))
		return E_FAIL;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Shionne")))
		return E_FAIL;



	//if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Sword"), &SocketDesc))
	//	return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Balseph(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Balseph")))
		return E_FAIL;

	CSword::SOCKETDESC			SocketDesc;
	ZeroMemory(&SocketDesc, sizeof(CSword::SOCKETDESC));

	SocketDesc.pModelCom = (CModel*)pGameInstance->Get_Component(LEVEL_TUTORIAL, pLayerTag, TEXT("Com_ModelSKL"));
	SocketDesc.pBoneName = "KK_R";

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BalsephAxe"), &SocketDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_Balseph::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);



	CUIBackground::UIBACKDESC uidesc;
	uidesc.kind = 3;
	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BackGroundUI"), &uidesc))
		return E_FAIL;



	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Terrain")))
		return E_FAIL;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Sky")))
		return E_FAIL;




	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Rect_Effect")))
		return E_FAIL;


	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Point_Effect")))
		return E_FAIL;

	for (_uint i = 0; i < 20; ++i)
	{
		if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Effect")))
			return E_FAIL;
	}


	Safe_Release(pGameInstance);

	return S_OK;


	
}

HRESULT CLevel_Balseph::Ready_Layer_UI()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	m_isBattle = false;
	if (FAILED(Ready_Layer_Tutorial_UI())) //Tutorial Layer
		return E_FAIL;


	//if (FAILED(Ready_Layer_BarUI(L"Layer_BarUI"))) //�Ͼ���
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_HP(L"Layer_HpUI"))) //�ʷ���
	//	return E_FAIL;
	////������ SEt_dead �̽� �������� Ǯ� ���� �ɵ�


	//if (FAILED(Ready_Layer_HPRed(L"Layer_RedBar")))
	//	return E_FAIL;



	/*if (FAILED(Ready_Layer_Balseph(TEXT("Layer_Balseph"))))
	return E_FAIL;*/

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	//	return E_FAIL;

	///////////falied to texture


	////////////�׳� �̹����� �ִ°�
	//if (FAILED(Ready_Layer_SmashNum(L"Layer_SmashNum")))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_AlphaTexture(L"Layer_chTalk")))
	//	return E_FAIL;




	Safe_Release(pGameInstance);


	return S_OK;
}



HRESULT CLevel_Balseph::Ready_Layer_EmblemUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	UIINFO uidesc;
	uidesc.fX = g_iWinCX / 2;
	uidesc.fY = 400;
	uidesc.fCX = 400;
	uidesc.fCY = 400;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_EmblemUI"), &uidesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_ExUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//�ڿ� ��� ���� UI
	UIExplain::UIEXDESC uidesc;
	uidesc.kind = 1;
	uidesc.tUIInfo.fX = 150;
	uidesc.tUIInfo.fY = 319;
	uidesc.tUIInfo.fCX = 0.7f;	//428
	uidesc.tUIInfo.fCY = 200.f;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_ExplainUI"), &uidesc)))
		return E_FAIL;

	//BATTLE TEX
	ZeroMemory(&uidesc, sizeof(UIExplain::UIEXDESC));
	uidesc.kind = 0;
	uidesc.tUIInfo.fX = 150;
	uidesc.tUIInfo.fY = 310;
	uidesc.tUIInfo.fCX = 300;
	uidesc.tUIInfo.fCY = 200;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_ExplainUI"), &uidesc)))
		return E_FAIL;

	//RESULT
	/*UIExplain::UIEXDESC uidesc;
	ZeroMemory(&uidesc, sizeof(UIExplain::UIEXDESC));
	uidesc.kind = 2;
	uidesc.tUIInfo.fX =100;
	uidesc.tUIInfo.fY = 600;
	uidesc.tUIInfo.fCX =500;
	uidesc.tUIInfo.fCY = 600;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_ExplainUI"), &uidesc)))
	return E_FAIL;*/

	////���� ���� UI
	//ZeroMemory(&uidesc, sizeof(UIExplain::UIEXDESC));
	//

	//uidesc.kind = 0;
	//uidesc.tUIInfo.fX = 175;
	//uidesc.tUIInfo.fY = 200;
	//uidesc.tUIInfo.fCX = 350;
	//uidesc.tUIInfo.fCY = 200;



	//if (nullptr == (m_pGameInstance->Add_GameObjectToLayer(LEVEL_GAME1, pLayerTag, TEXT("Prototype_GameObject_ExplainUI"), &uidesc)))
	//	return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_SmashUI()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Smash::UISMDESC uidesc;
	uidesc.tUIInfo.fX = 190;
	uidesc.tUIInfo.fY = 712;//470
	uidesc.tUIInfo.fCX = 110;
	uidesc.tUIInfo.fCY = 110;
	uidesc.kind = 0;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
		return E_FAIL;

	//ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	//uidesc.tUIInfo.fX = 190;
	//uidesc.tUIInfo.fY = 762;
	//uidesc.tUIInfo.fCX = 80;
	//uidesc.tUIInfo.fCY = 80;
	//uidesc.kind = 1;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
	//	return E_FAIL;



	ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	uidesc.tUIInfo.fX = 130;
	uidesc.tUIInfo.fY = 777;
	uidesc.tUIInfo.fCX = 110;
	uidesc.tUIInfo.fCY = 110;
	uidesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
		return E_FAIL;


	//�Ǹ�����
	//ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	//uidesc.tUIInfo.fX = 150;
	//uidesc.tUIInfo.fY = 812;
	//uidesc.tUIInfo.fCX = 80;
	//uidesc.tUIInfo.fCY = 80;
	//uidesc.kind = 3;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
	//	return E_FAIL;


	//���� �߰� ��ì��
	//ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	//uidesc.tUIInfo.fX = 70;
	//uidesc.tUIInfo.fY = 802;
	//uidesc.tUIInfo.fCX = 40;
	//uidesc.tUIInfo.fCY = 40;
	//uidesc.kind = 2;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
	//	return E_FAIL;

	//ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	//uidesc.tUIInfo.fX = 55;
	//uidesc.tUIInfo.fY = 824;
	//uidesc.tUIInfo.fCX = 40;
	//uidesc.tUIInfo.fCY = 40;
	//uidesc.kind = 3;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
	//	return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}



HRESULT CLevel_Balseph::Ready_Layer_SmashEffect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CUI_SmashEffect::UIINFO uidesc;

	uidesc.fX = 190;
	uidesc.fY = 520;
	uidesc.fCX = 10;
	uidesc.fCY = 10;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
		return E_FAIL;


	ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
	uidesc.fX = 150;
	uidesc.fY = 470;
	uidesc.fCX = 140;
	uidesc.fCY = 150;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
		return E_FAIL;

	ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
	uidesc.fX = 110;
	uidesc.fY = 520;
	uidesc.fCX = 140;
	uidesc.fCY = 150;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
		return E_FAIL;

	ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
	uidesc.fX = 150;
	uidesc.fY = 570;
	uidesc.fCX = 140;
	uidesc.fCY = 150;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Line(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	UILine::LINEDESC uidesc;
	uidesc.uidesc.fX = 1788;//1748g_iWinCX / 2 + 700;
	uidesc.uidesc.fY = 247;//g_iWinCY / 2 - 250;
	uidesc.uidesc.fCX = 300;
	uidesc.uidesc.fCY = 100;
	uidesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Line"), &uidesc)))
		return E_FAIL;


	/*ZeroMemory(&uidesc, sizeof(UILine::UIINFO));


	uidesc.fX = 1598;
	uidesc.fY = 807;
	uidesc.fCX = 100;
	uidesc.fCY = 40;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Line"), &uidesc)))
	return E_FAIL;



	ZeroMemory(&uidesc, sizeof(UILine::UIINFO));
	uidesc.fX = 1098;
	uidesc.fY = 827;
	uidesc.fCX = 100;
	uidesc.fCY = 40;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Line"), &uidesc)))
	return E_FAIL;



	ZeroMemory(&uidesc, sizeof(UILine::UIINFO));
	uidesc.fX = 1598;
	uidesc.fY = 847;
	uidesc.fCX = 100;
	uidesc.fCY = 40;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Line"), &uidesc)))
	return E_FAIL;
	*/



	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_Balseph::Ready_Layer_NumberFont(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	//Hit
	CUINumber::UINUMDESC uidesc;
	uidesc.tUIInfo.fX = 1757; // g_iWinCX / 2 + 470;
	uidesc.tUIInfo.fY = 214;//g_iWinCY / 2 - 285; //235 50�� ��������.
	uidesc.tUIInfo.fCX = 300;
	uidesc.tUIInfo.fCY = 100;
	uidesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;




	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_DamageFont(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//Hit
	CUINumber::UINUMDESC uidesc;
	uidesc.tUIInfo.fX = 1730;//g_iWinCX / 2 + 440;
	uidesc.tUIInfo.fY = 278;//g_iWinCY / 2 - 220;
	uidesc.tUIInfo.fCX = 70;
	uidesc.tUIInfo.fCY = 50;
	uidesc.kind = 1;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;





	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_HPFont(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//Hit
	CUINumber::UINUMDESC uidesc;
	uidesc.tUIInfo.fX = 1870;
	uidesc.tUIInfo.fY = 600;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;



	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));


	uidesc.tUIInfo.fX = 1870;
	uidesc.tUIInfo.fY = 800;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;




	//ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));


	//uidesc.tUIInfo.fX = g_iWinCX / 2 + 590;
	//uidesc.tUIInfo.fY = g_iWinCY / 2 + 80;
	//uidesc.tUIInfo.fCX = 20;
	//uidesc.tUIInfo.fCY = 20;
	//uidesc.kind = 3;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
	//	return E_FAIL;



	//ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));


	//uidesc.tUIInfo.fX = g_iWinCX / 2 + 590;
	//uidesc.tUIInfo.fY = g_iWinCY / 2 + 140;
	//uidesc.tUIInfo.fCX = 20;
	//uidesc.tUIInfo.fCY = 20;
	//uidesc.kind = 3;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_StatusFont(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//Hit
	CUINumber::UINUMDESC uidesc;
	//�ؿ� ���ִ� ��.. ������ ���� ��ٱ�..
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 840;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;

	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 830;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;


	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 820;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_StatusFont2(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//Hit
	CUINumber::UINUMDESC uidesc;


	//�տ� �ִ� �� �� ū �۾� �� ������ ���� ��ٱ� 
	//�ؿ� ���ִ� ��.. ������ ���� ��ٱ�..
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 795;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 35;
	uidesc.tUIInfo.fCY = 35;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;




	//�ؿ� ���ִ� ��..

	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 780;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 35;
	uidesc.tUIInfo.fCY = 35;
	uidesc.kind = 3;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;



	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 765;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 35;
	uidesc.tUIInfo.fCY = 35;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;




	// �� �۴��
	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 814;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 30;
	uidesc.tUIInfo.fCY = 30;
	uidesc.kind = 4;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_NameUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CUIName::UINAMEDESC uidesc;

	CUI_Manager* pUIInstance = CUI_Manager::GetInstance();
	CGameObject* test = pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_ALPHEN);

	//��¿�� ���� ������ �׳� �̷��� ����.

	uidesc.tUIInfo.fX = 1670;
	uidesc.tUIInfo.fY = 541;
	uidesc.tUIInfo.fCX = 50;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 0;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
		return E_FAIL;



	ZeroMemory(&uidesc, sizeof(CUIName::UINAMEDESC));
	uidesc.tUIInfo.fX = 1670;
	uidesc.tUIInfo.fY = 613;
	uidesc.tUIInfo.fCX = 50;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 1;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
		return E_FAIL;

	if (nullptr != test)
	{
		uidesc.tUIInfo.fX = 1670;
		uidesc.tUIInfo.fY = 541;
		uidesc.tUIInfo.fCX = 50;
		uidesc.tUIInfo.fCY = 20;
		uidesc.kind = 0;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
			return E_FAIL;
	}
	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_SION))
	{
		ZeroMemory(&uidesc, sizeof(CUIName::UINAMEDESC));
		uidesc.tUIInfo.fX = 1610;
		uidesc.tUIInfo.fY = 613;
		uidesc.tUIInfo.fCX = 50;
		uidesc.tUIInfo.fCY = 20;
		uidesc.kind = 1;

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
			return E_FAIL;
	}
	/*if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_RINWELL))
	{

	ZeroMemory(&uidesc, sizeof(CUIName::UINAMEDESC));
	uidesc.tUIInfo.fX = 1752;
	uidesc.tUIInfo.fY = 674;
	uidesc.tUIInfo.fCX = 50;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 2;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
	return E_FAIL;

	}	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYRE_KAKURA))
	{

	ZeroMemory(&uidesc, sizeof(CUIName::UINAMEDESC));
	uidesc.tUIInfo.fX = 1752;
	uidesc.tUIInfo.fY = 737;
	uidesc.tUIInfo.fCX = 50;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
	return E_FAIL;

	}*/

	//������ �ֵ�.





	Safe_Release(pGameInstance);
	return S_OK;
}
HRESULT CLevel_Balseph::Ready_Layer_StatusBallUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_StatusBall"))))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}
HRESULT CLevel_Balseph::Ready_Layer_StatusUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	
	CUIStatus::STATUSUIDESC uidesc;


	//�տ� �ִ� �� �� ū �۾� �� ������ ���� ��ٱ� 
	//�ؿ� ���ִ� ��.. ������ ���� ��ٱ�..
	uidesc.uidesc.fX = 1855;
	uidesc.uidesc.fY = g_iWinCY / 2 - 88;
	uidesc.uidesc.fCX = 104;
	uidesc.uidesc.fCY = 102;
	uidesc.kind = 0;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Status"), &uidesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_SmashNum(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	SmashNum::UISMNUMDESC uidesc;

	uidesc.tUIInfo.fX = 183;
	uidesc.tUIInfo.fY = 760;//502;
	uidesc.tUIInfo.fCX = 40;
	uidesc.tUIInfo.fCY = 40;
	uidesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;


	//2
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));

	uidesc.tUIInfo.fX = 170;
	uidesc.tUIInfo.fY = 772;
	uidesc.tUIInfo.fCX = 40;
	uidesc.tUIInfo.fCY = 40;
	uidesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;

	//3
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));

	uidesc.tUIInfo.fX = 206;
	uidesc.tUIInfo.fY = 772;
	uidesc.tUIInfo.fCX = 40;
	uidesc.tUIInfo.fCY = 40;
	uidesc.kind = 2;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;
	//4  
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 191;
	uidesc.tUIInfo.fY = 786;
	uidesc.tUIInfo.fCX = 40;
	uidesc.tUIInfo.fCY = 40;

	uidesc.kind = 3;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;

	//CP
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1806;
	uidesc.tUIInfo.fY = 446;
	uidesc.tUIInfo.fCX = 50;
	uidesc.tUIInfo.fCY = 50;

	uidesc.kind = 4;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;
	//R
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1756;
	uidesc.tUIInfo.fY = 866;
	uidesc.tUIInfo.fCX = 70;
	uidesc.tUIInfo.fCY = 70;

	uidesc.kind = 5;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;


	//E
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1724;
	uidesc.tUIInfo.fY = 893;
	uidesc.tUIInfo.fCX = 70;
	uidesc.tUIInfo.fCY = 70;

	uidesc.kind = 6;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;


	//F
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1756;
	uidesc.tUIInfo.fY = 921;
	uidesc.tUIInfo.fCX = 70;
	uidesc.tUIInfo.fCY = 70;

	uidesc.kind = 7;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;


	//�� �� ȭ��ǥ
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1111;
	uidesc.tUIInfo.fY = 578;
	uidesc.tUIInfo.fCX = 150;
	uidesc.tUIInfo.fCY = 150;

	uidesc.kind = 8;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_AlphaTexture(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	SmashNum::UISMNUMDESC uidesc;
	uidesc.tUIInfo.fX = 0;
	uidesc.tUIInfo.fY = 488;//338;
	uidesc.tUIInfo.fCX = 200;
	uidesc.tUIInfo.fCY = 70;
	uidesc.kind = 9;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"),&uidesc)))
		return E_FAIL;


	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));

	uidesc.tUIInfo.fX = 0;
	uidesc.tUIInfo.fY = 540;//390;
	uidesc.tUIInfo.fCX = 200;
	uidesc.tUIInfo.fCY = 60;
	uidesc.kind = 10;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"),&uidesc)))
		return E_FAIL;
	Safe_Release(pGameInstance);




	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_AutoTargetUI(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CLevel_Balseph::Ready_Layer_AllAboutHp(const _tchar * pLayerTag)
{
	//if (FAILED(Ready_Layer_BarUI(L"Layer_BattleUI")))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_HPRed(L"Layer_BattleUI")))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_HP(L"Layer_BattleUI")))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_MonsterHp(const _tchar * pLayerTag)
{

	//���߿� �������� �����. 

	//if (FAILED(Ready_Layer_BarUI(L"Layer_BattleUI")))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_HPRed(L"Layer_BattleUI")))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_HP(L"Layer_BattleUI")))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_HP(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//�ʷ���

	UIHpBar::UIHPBARDESC uidesc;

	uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
	uidesc.tUIInfo.fY = 570;
	uidesc.tUIInfo.fCX = 325;
	uidesc.tUIInfo.fCY = 17;
	uidesc.kind = 0;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpUI"), &uidesc)))
		return E_FAIL;

	ZeroMemory(&uidesc, sizeof(UIHpBar::UIHPBARDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
	uidesc.tUIInfo.fY = 630;
	uidesc.tUIInfo.fCX = 325;
	uidesc.tUIInfo.fCY = 17;
	uidesc.kind = 0;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpUI"), &uidesc)))
		return E_FAIL;


	CUI_Manager* pUIInstance = CUI_Manager::GetInstance();

	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_ALPHEN))
	{
		uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
		uidesc.tUIInfo.fY = 570;
		uidesc.tUIInfo.fCX = 325;
		uidesc.tUIInfo.fCY = 17;
		uidesc.kind = 0;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpUI"), &uidesc)))
			return E_FAIL;
	}
	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_SION)) {

		ZeroMemory(&uidesc, sizeof(UIHpBar::UIHPBARDESC));
		uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
		uidesc.tUIInfo.fY = 630;
		uidesc.tUIInfo.fCX = 325;
		uidesc.tUIInfo.fCY = 17;
		uidesc.kind = 0;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpUI"), &uidesc)))
			return E_FAIL;
	}

	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_RINWELL)) {
		ZeroMemory(&uidesc, sizeof(UIHpBar::UIHPBARDESC));
		uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
		uidesc.tUIInfo.fY = 690;
		uidesc.tUIInfo.fCX = 325;
		uidesc.tUIInfo.fCY = 17;
		uidesc.kind = 0;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpUI"), &uidesc)))
			return E_FAIL;

	}
	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYRE_KAKURA)) {
		ZeroMemory(&uidesc, sizeof(UIHpBar::UIHPBARDESC));
		uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
		uidesc.tUIInfo.fY = 750;
		uidesc.tUIInfo.fCX = 325;
		uidesc.tUIInfo.fCY = 17;
		uidesc.kind = 0;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpUI"), &uidesc)))
			return E_FAIL;

	}
	return S_OK;

	Safe_Release(pGameInstance);



}

HRESULT CLevel_Balseph::Ready_Layer_HPRed(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//������

	UIHpBarRed::UIHPBARDESC uidesc;
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
	uidesc.tUIInfo.fY = 570;
	uidesc.tUIInfo.fCX = 368;
	uidesc.tUIInfo.fCY = 14;
	uidesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpREDUI"), &uidesc)))
		return E_FAIL;

	ZeroMemory(&uidesc, sizeof(UIHpBarRed::UIHPBARDESC));

	uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
	uidesc.tUIInfo.fY = 630;
	uidesc.tUIInfo.fCX = 368;
	uidesc.tUIInfo.fCY = 14;
	uidesc.kind = 1;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpREDUI"), &uidesc)))
		return E_FAIL;

	/*uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
	uidesc.tUIInfo.fY = 630;
	uidesc.tUIInfo.fCX = 368;
	uidesc.tUIInfo.fCY = 14;
	uidesc.kind = 1;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpREDUI"), &uidesc)))
	return E_FAIL;

	ZeroMemory(&uidesc, sizeof(UIHpBarRed::UIHPBARDESC));

	uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
	uidesc.tUIInfo.fY = 690;
	uidesc.tUIInfo.fCX = 368;
	uidesc.tUIInfo.fCY = 14;
	uidesc.kind = 1;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpREDUI"), &uidesc)))
	return E_FAIL;

	ZeroMemory(&uidesc, sizeof(UIHpBarRed::UIHPBARDESC));


	uidesc.tUIInfo.fX = g_iWinCX / 2 + 663;
	uidesc.tUIInfo.fY = 750;
	uidesc.tUIInfo.fCX = 368;
	uidesc.tUIInfo.fCY = 14;
	uidesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_HpREDUI"), &uidesc)))
	return E_FAIL;

	*/
	return S_OK;

	Safe_Release(pGameInstance);
}

HRESULT CLevel_Balseph::Ready_Layer_BarUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIInstance = CUI_Manager::GetInstance();

	UIINFO uidesc;
	uidesc.fX = g_iWinCX / 2 + 800;
	uidesc.fY = 570;
	uidesc.fCX = 280;
	uidesc.fCY = 18;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
		return E_FAIL;
	ZeroMemory(&uidesc, sizeof(UIINFO));
	uidesc.fX = g_iWinCX / 2 + 800;
	uidesc.fY = 630;
	uidesc.fCX = 280;
	uidesc.fCY = 18;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
		return E_FAIL;


	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_ALPHEN))
	{

		uidesc.fX = g_iWinCX / 2 + 800;
		uidesc.fY = 570;
		uidesc.fCX = 280;
		uidesc.fCY = 18;

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
			return E_FAIL;

	}
	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_SION)) {
		ZeroMemory(&uidesc, sizeof(UIINFO));

		uidesc.fX = g_iWinCX / 2 + 800;
		uidesc.fY = 630;
		uidesc.fCX = 280;
		uidesc.fCY = 18;


		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
			return E_FAIL;

	}
	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_RINWELL)) {
		ZeroMemory(&uidesc, sizeof(UIINFO));
		uidesc.fX = g_iWinCX / 2 + 800;
		uidesc.fY = 690;
		uidesc.fCX = 280;
		uidesc.fCY = 18;

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
			return E_FAIL;

	}
	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYRE_KAKURA))
	{
		ZeroMemory(&uidesc, sizeof(UIINFO));
		uidesc.fX = g_iWinCX / 2 + 800;
		uidesc.fY = 750;
		uidesc.fCX = 280;
		uidesc.fCY = 18;


		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
			return E_FAIL;


	}


	return S_OK;

	Safe_Release(pGameInstance);

}

HRESULT CLevel_Balseph::Ready_Layer_InfoFadeUI(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	UIINFO uidesc;
	uidesc.fX = g_iWinCX / 2;
	uidesc.fY = g_iWinCX / 2 - 400;
	uidesc.fCX = 400;
	uidesc.fCY = 300;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_InfoFadeUI"), &uidesc)))
		return E_FAIL;


	return S_OK;

	Safe_Release(pGameInstance);

}

HRESULT CLevel_Balseph::Ready_Layer_LineMsgUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LineMsgUI::MSGLINEUIDESC uidesc;
	uidesc.uidesc.fX = g_iWinCX / 2;
	uidesc.uidesc.fY = g_iWinCX / 2 - 700;
	uidesc.uidesc.fCX = 700;
	uidesc.uidesc.fCY = 250;
	uidesc.sprite = 1;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_LineMsgUI"), &uidesc)))
		return E_FAIL;


	return S_OK;

	Safe_Release(pGameInstance);
}

CLevel_Balseph * CLevel_Balseph::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_Balseph*	pInstance = new CLevel_Balseph(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Tutorial"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CLevel_Balseph::Ready_Layer_Boar(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Boar")))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Practice(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_vector vPos = XMVectorSet(30.f, 0.f, 30.f,1.f);
	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Practice"),&vPos))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}



HRESULT CLevel_Balseph::Ready_Layer_Punisher(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Punisher")))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}


HRESULT CLevel_Balseph::Ready_Layer_FireAvatar(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_FireAvatar")))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Mantis(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Mantis")))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Tutorial_UI()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//���� ���̴� �н� ���� �̻���.
	//if (FAILED(Ready_Layer_EmblemUI(L"Layer_TutorialUI")))
	//	return E_FAIL;

	//�̸��׸� ä��.. ���̷���
	if (FAILED(Ready_Layer_InfoFadeUI(L"Layer_TutorialUI")))
		return E_FAIL;

	//Tutorial Msg �߰�.
	TutorialMsg::TMSGDESC uidesc;
	uidesc.uidesc.fX = g_iWinCX / 2;
	uidesc.uidesc.fY = g_iWinCX / 2;
	uidesc.uidesc.fCX = 200;
	uidesc.uidesc.fCY = 50;
	uidesc.kind = 0;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_TutorialMsg"), &uidesc)))
		return E_FAIL;

	//Line
	UILine::LINEDESC linedesc;
	linedesc.uidesc.fX = 0;
	linedesc.uidesc.fY = g_iWinCX / 2 - 700;
	linedesc.uidesc.fCX = 1920;
	linedesc.uidesc.fCY = 80;
	linedesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_Line"), &linedesc)))
		return E_FAIL;


	//���� �ִ� TutoLine���� ���� ���� �޽��� �̸��׸� ��Ƽ������ ������ �غ��ϼ���.
	//5 - worldŽ�� 6- �߼����� 7 - ��Ƽ������ ���� 
	//�׳� ��Ŭ���� ���弼��
	//TutoLineMsg
	TutoLineMsg::MSGLINEUIDESC TutoLineMsgDesc;
	TutoLineMsgDesc.uidesc.fX = g_iWinCX / 2;
	TutoLineMsgDesc.uidesc.fY = g_iWinCX / 2 - 750;
	TutoLineMsgDesc.uidesc.fCX = 500;
	TutoLineMsgDesc.uidesc.fCY = 70;
	TutoLineMsgDesc.sprite = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_TutoLineMsg"), &TutoLineMsgDesc)))
		return E_FAIL;


	////LineSprite
	LineSprite::MINIMAPDESC LineSpritedesc;
	LineSpritedesc.uidesc.fX = g_iWinCX / 2;
	LineSpritedesc.uidesc.fY = g_iWinCX / 2 - 750;
	LineSpritedesc.uidesc.fCX = 400;
	LineSpritedesc.uidesc.fCY = 400;
	LineSpritedesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
		return E_FAIL;

	//

	//�̴ϸ�
	MiniMap::MINIMAPDESC minidesc;
	minidesc.uidesc.fX = g_iWinCX - 300;
	minidesc.uidesc.fY = 300;
	minidesc.uidesc.fCX = 400;
	minidesc.uidesc.fCY = 400;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_MiniMap"), &minidesc)))
		return E_FAIL;


	//�̴ϸ� veow
	MIniMapView::MINIMAPDESC MIniMapViewDesc;
	MIniMapViewDesc.uidesc.fX = g_iWinCX - 300;
	MIniMapViewDesc.uidesc.fY = 300;
	MIniMapViewDesc.uidesc.fCX = 50;
	MIniMapViewDesc.uidesc.fCY = 50;
	MIniMapViewDesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_MiniMapView"), &MIniMapViewDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_Balseph::Ready_Layer_Battle_UI()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_isBattle = true;

	if (FAILED(Ready_Layer_SmashUI()))
		return E_FAIL;

	//���� ���� 
	if (FAILED(Ready_Layer_ExUI(L"Layer_BattleUI")))
		return E_FAIL;

	if (FAILED(Ready_Layer_AllAboutHp(L"Layer_BattleUI")))
		return E_FAIL;

	//Line
	if (FAILED(Ready_Layer_Line(L"Layer_BattleUI")))
		return E_FAIL;


	//if (FAILED(Ready_Layer_NumberFont(L"Layer_BattleUI")))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_DamageFont(L"Layer_BattleUI")))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_StatusFont(L"Layer_BattleUI")))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_StatusFont2(L"Layer_BattleUI")))
	//	return E_FAIL;


	if (FAILED(Ready_Layer_HPFont(L"Layer_BattleUI")))
		return E_FAIL;


	if (FAILED(Ready_Layer_NameUI(L"Layer_BattleUI")))
		return E_FAIL;

	if (FAILED(Ready_Layer_StatusBallUI(L"Layer_BattleUI")))
		return E_FAIL;

	if (FAILED(Ready_Layer_StatusUI(L"Layer_BattleUI")))
		return E_FAIL;

	//Line
	UILine::LINEDESC linedesc;
	linedesc.uidesc.fX = 0;
	linedesc.uidesc.fY = g_iWinCX / 2 - 700;
	linedesc.uidesc.fCX = 1920;
	linedesc.uidesc.fCY = 80;
	linedesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Line"), &linedesc)))
		return E_FAIL;


	//���� �ִ� TutoLine���� ���� ���� �޽��� �̸��׸� ��Ƽ������ ������ �غ��ϼ���.
	//5 - worldŽ�� 6- �߼����� 7 - ��Ƽ������ ���� 
	//�׳� ��Ŭ���� ���弼��

	//TutoLineMsg
	TutoLineMsg::MSGLINEUIDESC TutoLineMsgDesc;
	TutoLineMsgDesc.uidesc.fX = g_iWinCX / 2;
	TutoLineMsgDesc.uidesc.fY = g_iWinCX / 2 - 750;
	TutoLineMsgDesc.uidesc.fCX = 600;
	TutoLineMsgDesc.uidesc.fCY = 70;
	TutoLineMsgDesc.sprite = 1;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_TutoLineMsg"), &TutoLineMsgDesc)))
		return E_FAIL;


	////LineSprite
	LineSprite::MINIMAPDESC LineSpritedesc;
	LineSpritedesc.uidesc.fX = g_iWinCX / 2;
	LineSpritedesc.uidesc.fY = g_iWinCX / 2 - 750;
	LineSpritedesc.uidesc.fCX = 500;
	LineSpritedesc.uidesc.fCY = 400;
	LineSpritedesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
		return E_FAIL;


	////Line
	//UILine::LINEDESC linedesc;
	//linedesc.uidesc.fX = 0;
	//linedesc.uidesc.fY = g_iWinCX / 2 - 500;
	//linedesc.uidesc.fCX = 1920;
	//linedesc.uidesc.fCY = 80;
	//linedesc.kind = 1;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Line"), &linedesc)))
	//	return E_FAIL;

	////�̰� �װ���.. ���������� ��Ƽ�� �����϶�� �޽���
	////���� �ִ� TutoLine���� ���� ���� �޽��� �̸��׸� ��Ƽ������ ������ �غ��ϼ���.
	////5 - worldŽ�� 6- �߼����� 7 - ��Ƽ������ ���� 
	////�׳� ��Ŭ���� ���弼��
	////TutoLineMsg
	//TutoLineMsg::MSGLINEUIDESC TutoLineMsgDesc;
	//TutoLineMsgDesc.uidesc.fX = g_iWinCX / 2;
	//TutoLineMsgDesc.uidesc.fY = g_iWinCX / 2 - 550;
	//TutoLineMsgDesc.uidesc.fCX = 400;
	//TutoLineMsgDesc.uidesc.fCY = 70;
	//TutoLineMsgDesc.sprite = 1;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_TutoLineMsg"), &TutoLineMsgDesc)))
	//	return E_FAIL;


	//////LineSprite
	//LineSprite::MINIMAPDESC LineSpritedesc;
	//LineSpritedesc.uidesc.fX = g_iWinCX / 2;
	//LineSpritedesc.uidesc.fY = g_iWinCX / 2 - 550;
	//LineSpritedesc.uidesc.fCX = 400;
	//LineSpritedesc.uidesc.fCY = 400;
	//LineSpritedesc.kind = 0;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
	//	return E_FAIL;


	//�̰� ��¥ num��,..
	if (FAILED(Ready_Layer_SmashNum(L"Layer_BattleUI")))
		return E_FAIL;

	////�̰ǾƸ� ���Ű� �ؽ��� �ϴ��� Tick���� ����.
	//if (FAILED(Ready_Layer_AlphaTexture(L"Layer_BattleUI")))
	//	return E_FAIL;


	//name..
	if (FAILED(Ready_Layer_NameUI(L"Layer_BattleUI")))
		return E_FAIL;



	////hp���� �����
	if (FAILED(Ready_Layer_AllAboutHp(L"Layer_BattleUI")))
		return E_FAIL;



	////glowUI ����
	//UIGlow::UIINFO uidesc;
	//uidesc.fX = g_iWinCX / 2;
	//uidesc.fY = g_iWinCX / 2 - 750;
	//uidesc.fCX = 500;
	//uidesc.fCY = 400;


	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_UIGlow"), &LineSpritedesc)))
	//	return E_FAIL;


	////Line

	//


	//

	//BillBoardUI
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_UIBillBoard"))))
	//	return E_FAIL;


	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_MovingHP")))
		return E_FAIL;




	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Layer_Inventory_UI()
{
	return E_NOTIMPL;
}

HRESULT CLevel_Balseph::Ready_Layer_Inventory_Itme_UI()
{
	return E_NOTIMPL;
}


void CLevel_Balseph::Free()
{
	__super::Free();

	Safe_Release(m_pBattle_Manager);
}

HRESULT CLevel_Balseph::Ready_Fonts()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (nullptr == pGameInstance)
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Fonts(m_pDevice, m_pDeviceContext, TEXT("Font_Javan40"), TEXT("../Bin/Resources/Fonts/Javanese20.spritefont"))))
	{


		MSG_BOX(TEXT("Failed to Load Font"));
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Fonts(m_pDevice, m_pDeviceContext, TEXT("Font_Javan10"), TEXT("../Bin/Resources/Fonts/Javanese10.spritefont"))))
	{


		MSG_BOX(TEXT("Failed to Load Font"));
		return E_FAIL;
	}


	pGameInstance->Release();
	return S_OK;
}

HRESULT CLevel_Balseph::Ready_Map(const char * pModelFilePath, const char * pModelFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// ������ ����.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	for (auto& Obj : m_vMapObject)
		Safe_Release(Obj);
	m_vMapObject.clear();

	CMapObject::MAPOBJDesc Desc;

	while (true)
	{
		// ���ڿ� �޸��� ũ�⸦ ����
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		// �о�� �޸��� ũ�⸸ŭ ���ڿ��� �Ҵ��ϰ�, �ش� ���ڿ��� �о��
		TCHAR*	pName = new TCHAR[dwStrByte];
		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

		// �߸����� ��� �ش� �޸� ���� �� ���߱�
		if (0 == dwByte)
		{
			delete[]pName;
			break;
		}

		// Desc�� ���� �ֱ�
		Desc.pPrototype_ObjectName = pName;

		// Collider ���� ����
		Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

		// ��� ������ ����
		ReadFile(hFile, &Desc.TransformMatrix, sizeof(_float4x4), &dwByte, nullptr);

		// �� ���� Ȯ��
		ReadFile(hFile, &Desc.m_bMap, sizeof(_bool), &dwByte, nullptr);



		// �ش� ������Ʈ�� �̸��� WarpGate �� ���
		if (!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_WarpGate"))) {
			static _uint iWayPointNumber = 0;

			// WayPoint Desc���� ä���.
			CWayPoint::WayPoint_DESC WayPointDesc;
			WayPointDesc.iPointNumber = iWayPointNumber++;

			// ��ġ ����
			memcpy(&WayPointDesc.fPos, &Desc.TransformMatrix.m[CTransform::STATE_POSITION][0], sizeof(_float3));

			// ���� ����
			memcpy(&WayPointDesc.fDir, &Desc.TransformMatrix.m[CTransform::STATE_LOOK][0], sizeof(_float3));

			CWayPoint* pWayPoint = dynamic_cast<CWayPoint*>(pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, LAYER_MAPTOOL_WayPoints, TEXT("Prototype_GameObject_WayPoint"), &WayPointDesc));
			m_vWayPoints.push_back(pWayPoint);
		}
		else {
			// �� ������Ʈ ����
			CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
			m_vMapObject.push_back(pMapObject);
		}
	}

	// ������ �ݴ´�.
	CloseHandle(hFile);





	// ���⼭ m_vWayPoints�� �ִ� �ε�� ���� ����Ʈ�� ���� �������ش�.

	// �ۼ�






	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


