#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Camera_Default.h"
#include "LightDepth.h"
#include "Terrain.h"
#include "Player.h"
#include "Alphen.h"
#include "Shionne.h"
#include "Sky.h"
#include "Sword.h"
#include "Gun.h"
#include "Rect_Effect.h"
#include "Point_Effect.h"
#include "Effect.h"
#include "Fire_Avatar.h"
#include "Lord_Balseph.h"
#include "BalsephAxe.h"
#include "Boar.h"
#include "Mantis.h"
#include "Punisher.h"
#include "FireAvatar_Rock.h"
#include "Practice.h"


#include "Camera_MapTool.h"
#include "MapObject.h"
#include "ImGUI_Manager.h"
#include "Picking_Rect.h"
#include "Dot.h"
#include "WayPoint.h"

#include "Fire_Deco.h"

#include "Navigation.h"

#include "LightObject.h"

CLoader::CLoader(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: m_pDevice(pDeviceOut), m_pDeviceContext(pDeviceContextOut)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY ThreadEntryFunc(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_LevelIndex())
	{

	case LEVEL_LOBBY:
		pLoader->Loading_ForLobbyLevel();
		break;

	case LEVEL_TUTORIAL:
		pLoader->Loading_ForGamePlayLevel();
		break;

	case LEVEL_MapTool:
		pLoader->Loading_ForMapToolLevel();
		break;

	/*case LEVEL_BOSS1:
		pLoader->Loading_ForBossLevel1();
		break;*/

	/*case LEVEL_BOSS2:
		pLoader->Loading_ForBossLevel2();
		break;*/
	}


	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLobbyLevel()
{
	m_isFinished = false;

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));


	lstrcpy(m_szLoading, TEXT("셰이더를 로드중입니다. "));
	

	lstrcpy(m_szLoading, TEXT("게임오브젝트를 로드중입니다. "));


	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	lstrcpy(m_szLoading, TEXT("TutorialLevel 텍스쳐를 로드중입니다. "));
	if (FAILED(Loading_GamePlayLevel_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("TutorialLevel 모델을 로드중입니다. "));
	if (FAILED(Loading_GamePlayLevel_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("TutorialLevel 셰이더를 로드중입니다. "));
	if (FAILED(Loading_GamePlayLevel_Shader()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("TutorialLevel 게임오브젝트를 로드중입니다. "));
	if (FAILED(Loading_GamePlayLevel_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("TutorialLevel 컴포넌트를 로드중입니다. "));
	if (FAILED(Loading_GamePlayLevel_Component()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("콜라이더 생성중입니다. "));
	if (FAILED(Loading_GamePlayLevel_Collider()))
		return E_FAIL;




	lstrcpy(m_szLoading, TEXT("MapTool_To_TutorialLevel 텍스쳐를 로드중입니다. "));
	//if (FAILED(Loading_MapToolLevel_Texture(LEVEL_TUTORIAL)))
	//	return E_FAIL;

	//lstrcpy(m_szLoading, TEXT("MapToolLevel 모델을 로드중입니다. "));
	if (FAILED(Loading_MapToolLevel_Model(LEVEL_MapTool)))	// 오브젝트를 LEVEL_MapTool에 몰아넣고, 해당 clone만 원하는 레벨에서 한다.
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("MapTool_To_TutorialLevel 게임오브젝트를 로드중입니다. "));
	if (FAILED(Loading_MapTool_To_TutorialLevel_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("MapTool_To_Tutorial 레벨의 로딩이 완료되었습니다.. "));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_GamePlayLevel_Texture()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	/* For. Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_TerrainMask */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Snow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_SkyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/Skybox%d.dds"), 15))))
		return E_FAIL;

	///* For. Prototype_Component_Texture_Sky */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Sky"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	//	return E_FAIL;

	/* For. Prototype_Component_Texture_Explosion */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Explosion"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
		return E_FAIL;




	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_GamePlayLevel_Object()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Default */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Default"),
		CCamera_Default::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LightDepth */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LightDepth"),
		CLightDepth::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Alphen */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Alphen"),
		CAlphen::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shionne"),
		CShionne::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FireAvatar */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FireAvatar"),
		CFire_Avatar::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Balseph */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Balseph"),
		CLord_Balseph::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BalsephAxe */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BalsephAxe"),
		CBalsephAxe::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boar"),
		CBoar::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Practice"),
		CPractice::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mantis"),
		CMantis::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Punisher"),
		CPunisher::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FireAvatar_Rock"),
		CFireAvatar_Rock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sword */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sword"),
		CSword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sword */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gun"),
		CGun::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rect_Effect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rect_Effect"),
		CRect_Effect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Point_Effect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Point_Effect"),
		CPoint_Effect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		CEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	/* For.Prototype_GameObject_WayPoint */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WayPoint"),
		CWayPoint::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_GamePlayLevel_Shader()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For. Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements, CShader::FX))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxAnim*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnim.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements, CShader::FX))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxInstance*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance.hlsl"), VTXINSTANCE_DECLARATION::Elements, VTXINSTANCE_DECLARATION::iNumElements, CShader::FX))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxCube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE_DECLARATION::Elements, VTXCUBE_DECLARATION::iNumElements, CShader::FX))))
		return E_FAIL;


	/* For. Prototype_Component_Shader_VtxNonAnim*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNonAnim.hlsl"), VTXNONANIMMODEL_DECLARATION::Elements, VTXNONANIMMODEL_DECLARATION::iNumElements, CShader::FX))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlayLevel_Model()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix, PivotAxeMatrix;
	
	//1회차: PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	PivotMatrix = XMMatrixScaling(0.003f, 0.003f, 0.003f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	
	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Body"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ari/Body/", "Ari_Body.fbx", PivotMatrix))))
		return E_FAIL;

	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Face"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ari/Face/", "Ari_Face.fbx", PivotMatrix))))
		return E_FAIL;


	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Hair"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ari/Hair/", "Ari_Hair.fbx", PivotMatrix))))
		return E_FAIL;

	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_SKL"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ari/SKL/", "Ari_SKL.fbx", PivotMatrix))))
		return E_FAIL;

	/*For Player*/	
	//1회차: PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f))* XMMatrixRotationX(XMConvertToRadians(180.f));
	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f))* XMMatrixRotationX(XMConvertToRadians(180.f));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Sword_26"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ari/Sword/Ari_Sword_26/", "Ari_Sword_26.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Sword_28"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ari/Sword/Ari_Sword_28/", "Ari_Sword_28.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Sword_100"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ari/Sword/Ari_Sword_100/", "Ari_Sword_100.fbx", PivotMatrix))))
		return E_FAIL;


	////////////////////////////////////////////////////////////////////////SIO////////////////////////////////////////////////////////////
	//SIO

	PivotMatrix = XMMatrixScaling(0.003f, 0.003f, 0.003f) * XMMatrixRotationY(XMConvertToRadians(180.f));

	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Body"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sio/Body/", "Sio_Body.fbx", PivotMatrix))))
		return E_FAIL;

	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Face"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sio/Face/", "Sio_Face.fbx", PivotMatrix))))
		return E_FAIL;

	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Hair"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sio/Hair/", "Sio_Hair.fbx", PivotMatrix))))
		return E_FAIL;

	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_SKL"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sio/SKL/", "Sio_SKL.fbx", PivotMatrix))))
		return E_FAIL;


	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f))* XMMatrixRotationX(XMConvertToRadians(180.f));
	/*For Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Gun_0"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Sio/Gun/Sio_Gun_0/", "Sio_Gun_0.fbx", PivotMatrix))))
		return E_FAIL;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	// 나중에 LEVEL_BOSS1으로 옮겨야함
	////Balseph
	//PivotMatrix = XMMatrixScaling(0.003f, 0.003f, 0.003f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Balseph"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Baselph/", "wow.fbx", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BalsephFace"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Baselph/", "balsephFace.fbx", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BalsephSKL"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Baselph/", "balsephSKL.fbx", PivotMatrix))))
	//	return E_FAIL;

	//PivotAxeMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BalsephAXE"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Baselph/", "balsephAxe.fbx", PivotAxeMatrix))))
	//	return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.003f, 0.003f, 0.003f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_FireAvatar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/FireAvatar/", "fire_Avatar.fbx", PivotMatrix))))
		return E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.003f, 0.003f, 0.003f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Boar"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Boar/", "boar.fbx", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Mantis"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Mantis/", "mantis1.fbx", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Punisher"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/punisher/", "punisher.fbx", PivotMatrix))))
	//	return E_FAIL;

	//PivotMatrix = XMMatrixScaling(1.5f, 1.5f, 1.5f) * XMMatrixRotationX(XMConvertToRadians(180.f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_EFRRock"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/FireAvatar/", "efrrock.fbx", PivotMatrix))))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlayLevel_Component()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For. Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/* For. Prototype_Component_VIBuffer_RectInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 10))))
		return E_FAIL;

	/* For. Prototype_Component_VIBuffer_PointInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 50))))
		return E_FAIL;

	/* For. Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	/* For. Prototype_Component_Navigation_Mesh */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_Tutorial"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage01/CellList.dat")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_Balseph"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage02/CellList.dat")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_BalsephEnterance"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage03/CellList.dat")))))
		return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_FireAvatar"),
	//	CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage04/CellList.dat")))))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlayLevel_Collider()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_Collider_AABB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForBoss1()
{
	return S_OK;
}

HRESULT CLoader::Loading_ForBoss2()
{
	return S_OK;
}

HRESULT CLoader::Loading_ForMapToolLevel()
{
	lstrcpy(m_szLoading, TEXT("MapToolLevel 텍스쳐를 로드중입니다. "));
	if (FAILED(Loading_MapToolLevel_Texture(LEVEL_MapTool)))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("MapToolLevel 모델을 로드중입니다. "));
	if (FAILED(Loading_MapToolLevel_Model(LEVEL_MapTool)))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("MapToolLevel 셰이더를 로드중입니다. "));
	if (FAILED(Loading_MapToolLevel_Shader()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("MapToolLevel 게임오브젝트를 로드중입니다. "));
	if (FAILED(Loading_MapToolLevel_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("MapToolLevel 컴포넌트를 로드중입니다. "));
	if (FAILED(Loading_MapToolLevel_Component()))
		return E_FAIL;

	ASDF();

	lstrcpy(m_szLoading, TEXT("맵툴 레벨의 로딩이 완료되었습니다.. "));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_MapToolLevel_Texture(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// Terrain 관련
	lstrcpy(m_szLoading, TEXT("Terrain를 로드중입니다."));

	if (FAILED(pGameInstance->Add_Prototype(eLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/Tile/Tile%d.dds"), 2))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_TerrainMask */
	if (FAILED(pGameInstance->Add_Prototype(eLevel, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/Filter/Filter.bmp"), 1))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(eLevel, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/Brush/Brush.png"), 1))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_SkyBox */
	if (FAILED(pGameInstance->Add_Prototype(eLevel, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/Skybox%d.dds"), 15))))
		return E_FAIL;



	/* For. Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(eLevel, TEXT("Prototype_Component_Texture_Terrain_Diffuse"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/TerrainTextrure%d.dds"), 18))))
		return E_FAIL;
	/* For. Prototype_Component_Texture_Mask */
	if (FAILED(pGameInstance->Add_Prototype(eLevel, TEXT("Prototype_Component_Texture_Mask"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Mask%d.dds"), 3))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_MapToolLevel_Object()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// Terrain 관련
	lstrcpy(m_szLoading, TEXT("Terrain를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	// Camera_Default 관련
	lstrcpy(m_szLoading, TEXT("Camera_MapTool를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_MapTool"),
		CCamera_MapTool::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	// MapObject 관련
	lstrcpy(m_szLoading, TEXT("MapObject를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	// Picking_Rect 관련
	lstrcpy(m_szLoading, TEXT("Picking_Rect를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Picking_Rect"),
		CPicking_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	// Dot 관련
	lstrcpy(m_szLoading, TEXT("Dot을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Dot"),
		CDot::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	// LightObject 관련
	lstrcpy(m_szLoading, TEXT("LightObject를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LightObject"),
		CLightObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_MapToolLevel_Shader()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For. Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNonAnim.hlsl"), VTXNONANIMMODEL_DECLARATION::Elements, VTXNONANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxAnim*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnim.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_Fire*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Fire"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Fire.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements, CShader::FX))))
		return E_FAIL;	
	
	/* For. Prototype_Component_Shader_VtxCube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE_DECLARATION::Elements, VTXCUBE_DECLARATION::iNumElements, CShader::FX))))
		return E_FAIL;

	/* For. Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_MapToolLevel_Model(LEVEL eLevel)
{
	/*lstrcpy(m_szLoading, TEXT("BattleMap을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_BattleMap(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("GAR_Cliff를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_GAR_Cliff(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("GAR_Stair을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_GAR_Stair(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Maps를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Maps(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("BezCastle을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_BezCastle(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Elevator를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Elevator(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("RailPilar를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_RailPilar(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Railway을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Railway(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Rock을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Rock(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Tree를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Tree(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Grass를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Grass(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("TruckRoad을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_TruckRoad(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Ors을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Ors(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("OilPump을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_OilPump(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("DRTHouse를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_DRTHouse(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("BrokenHouse을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_BrokenHouse(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("GateScrap을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_GateScrap(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Cave을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Cave(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("FireGate를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_FireGate(eLevel)))
	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("IronWall를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_IronWall(eLevel)))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("GRN_Boss_Stair을 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_GRN_Boss_Stair(eLevel)))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Balseph_Enterance를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Balseph_Enterance(eLevel)))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Balseph를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Balseph(eLevel)))
		return E_FAIL;*/

		//lstrcpy(m_szLoading, TEXT("Map_Cave_01을 로드중입니다."));
		//if (FAILED(Loading_LEVEL_MapTool_Model_Map_Cave_01(eLevel)))
		//	return E_FAIL;

		//lstrcpy(m_szLoading, TEXT("Map_Cave_05을 로드중입니다."));
		//if (FAILED(Loading_LEVEL_MapTool_Model_Map_Cave_05(eLevel)))
		//	return E_FAIL;

		//lstrcpy(m_szLoading, TEXT("Map_Cave_07을 로드중입니다."));
		//if (FAILED(Loading_LEVEL_MapTool_Model_Map_Cave_07(eLevel)))
		//	return E_FAIL;



	LEVEL CurLevel = LEVEL_IGNISERI01;
	switch (CurLevel)
	{
	case Client::LEVEL_STATIC:
		break;
	case Client::LEVEL_LOADING:
		break;
	case Client::LEVEL_LOGO:
		break;
	case Client::LEVEL_LOBBY:
		break;
	case Client::LEVEL_TUTORIAL:
		lstrcpy(m_szLoading, TEXT("Map_GrandYork를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_GrandYork(eLevel)))
			return E_FAIL;

		// 집 모델들
		lstrcpy(m_szLoading, TEXT("DRTHouse를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_DRTHouse(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("BrokenHouse을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_BrokenHouse(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("WarpGate를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_WarpGate(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Grass를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Grass(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Tree를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Tree(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Ivy를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Ivy(eLevel)))
			return E_FAIL;

		// 올빼미
		lstrcpy(m_szLoading, TEXT("Owl을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Owls(eLevel)))
			return E_FAIL;

		// 올빼미
		lstrcpy(m_szLoading, TEXT("OilPump02를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Anim_OilPump(eLevel)))
			return E_FAIL;

		break;
	case Client::LEVEL_BALSEPH:
		lstrcpy(m_szLoading, TEXT("Map_Boss_Balseph를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_Boss_Balseph(eLevel)))
			return E_FAIL;

		// 토치라이트
		lstrcpy(m_szLoading, TEXT("Torchlight를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_TorchLight(eLevel)))
			return E_FAIL;

		// 가구
		lstrcpy(m_szLoading, TEXT("Furniture를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Furniture(eLevel)))
			return E_FAIL;

		// 애니메이션 오브젝트
		lstrcpy(m_szLoading, TEXT("Anim Object들을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Anim_Objects(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("WarpGate를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_WarpGate(eLevel)))
			return E_FAIL;

		break;
	case Client::LEVEL_FIRE_AVATAR:
		lstrcpy(m_szLoading, TEXT("Map_Boss_FireAvatar를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_Boss_FireAvatar(eLevel)))
			return E_FAIL;


		break;

	case Client::LEVEL_BATTLE02:
		lstrcpy(m_szLoading, TEXT("Map_GrandYork_Battle_02을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_GrandYork_Battle_02(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Grass를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Grass(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Tree를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Tree(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Ivy를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Ivy(eLevel)))
			return E_FAIL;

		// 올빼미
		lstrcpy(m_szLoading, TEXT("Owl을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Owls(eLevel)))
			return E_FAIL;

		// 올빼미
		lstrcpy(m_szLoading, TEXT("OilPump02를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Anim_OilPump(eLevel)))
			return E_FAIL;

		break;


	case Client::LEVEL_BATTLE04:
		lstrcpy(m_szLoading, TEXT("Map_GrandYork_Battle_04을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_GrandYork_Battle_04(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Grass를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Grass(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Tree를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Tree(eLevel)))
			return E_FAIL;

		lstrcpy(m_szLoading, TEXT("Ivy를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Ivy(eLevel)))
			return E_FAIL;

		// 올빼미
		lstrcpy(m_szLoading, TEXT("Owl을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Owls(eLevel)))
			return E_FAIL;

		// 올빼미
		lstrcpy(m_szLoading, TEXT("OilPump02를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Anim_OilPump(eLevel)))
			return E_FAIL;
		break;

	case Client::LEVEL_IGNISERI01:
		lstrcpy(m_szLoading, TEXT("Map_Boss_Lord_Vholran_Igniseri_Phase01을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Phase01(eLevel)))
			return E_FAIL;

		if (FAILED(Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Object(eLevel)))
			return E_FAIL;
		
		break;
	case Client::LEVEL_IGNISERI02:
		lstrcpy(m_szLoading, TEXT("Map_Boss_Lord_Vholran_Igniseri_Phase02를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Phase02(eLevel)))
			return E_FAIL;

		if (FAILED(Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Object(eLevel)))
			return E_FAIL;
		break;

	case Client::LEVEL_WIND_SPIRIT_INSIDE:
		lstrcpy(m_szLoading, TEXT("Map_Tower_of_the_Wind_Spirit_Inside를 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_Tower_of_the_Wind_Spirit_Inside(eLevel)))
			return E_FAIL;
		break;

	case Client::LEVEL_WIND_SPIRIT_TOP:
		lstrcpy(m_szLoading, TEXT("Map_Tower_of_the_Wind_Spirit_Top을 로드중입니다."));
		if (FAILED(Loading_LEVEL_MapTool_Model_Map_Tower_of_the_Wind_Spirit_Top(eLevel)))
			return E_FAIL;
		break;

	case Client::LEVEL_BOSS1:
		break;
	case Client::LEVEL_BOSS2:
		break;
	case Client::LEVEL_MapTool:
		break;
	case Client::LEVEL_END:
		break;
	default:
		break;
	}



	// 기준이 되는 플레이어
	lstrcpy(m_szLoading, TEXT("Standard_Player를 로드중입니다."));
	if (FAILED(Loading_LEVEL_MapTool_Model_Standard_Player(eLevel)))
		return E_FAIL;



	
	return S_OK;
}

HRESULT CLoader::Loading_MapToolLevel_Component()
{
	//// 하이트맵 추가
	//if (FAILED(Loading_LEVEL_MapTool_Component_HeightMap()))
	//	return E_FAIL;

	// 콜라이더 추가
	if (FAILED(Loading_LEVEL_MapTool_Component_Collider()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Component_HeightMap()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);

	pImgui->Push_HeightMapTag(TEXT("BattleBase_32"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain_BattleBase_32"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/HeightMap/Bmp/HeightMap_BattleBase_32.bmp")))))
		return E_FAIL;

	pImgui->Push_HeightMapTag(TEXT("BattleBase_64"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain_BattleBase_64"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/HeightMap/Bmp/HeightMap_BattleBase_64.bmp")))))
		return E_FAIL;

	pImgui->Push_HeightMapTag(TEXT("BattleBase_128"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain_BattleBase_128"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/HeightMap/Bmp/HeightMap_BattleBase_128.bmp")))))
		return E_FAIL;

	pImgui->Push_HeightMapTag(TEXT("BattleBase_256"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain_BattleBase_256"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/HeightMap/Bmp/HeightMap_BattleBase_256.bmp")))))
		return E_FAIL;

	pImgui->Push_HeightMapTag(TEXT("BattleBase_512"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain_BattleBase_512"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/HeightMap/Bmp/HeightMap_BattleBase_512.bmp")))))
		return E_FAIL;

	pImgui->Push_HeightMapTag(TEXT("YOK_BG_Mountain3"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain_YOK_BG_Mountain3"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/HeightMap/Bmp/HeightMap_YOK_BG_Mountain3.bmp")))))
		return E_FAIL;

	pImgui->Push_HeightMapTag(TEXT("YOK_BG_Mountain4"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain_YOK_BG_Mountain4"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Map/HeightMap/Bmp/HeightMap_YOK_BG_Mountain4.bmp")))))
		return E_FAIL;

	pImgui->UpdateHeightMapList();

	RELEASE_INSTANCE(CImGUI_Manager);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Component_Collider()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_BattleMap(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//_matrix			PivotMatrix;

	//PivotMatrix = XMMatrixScaling(1.f, 0.01f, 1.f);

	///* For. Prototype_Component_VIBuffer_Terrain*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_BattleMap_01"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
	//		"../Bin/Resources/Model/Stage01/Battle/",
	//		"BattleBackground.fbx", PivotMatrix))))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_GAR_Cliff(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	//PivotMatrix = XMMatrixScaling(0.00001f, 0.00001f, 0.00001f);
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff_01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff01.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff_02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff02.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff_03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff03.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff_04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff04.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff_05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff05.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff_06"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff06.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff_07"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff07.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff2_01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff2_01.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff2_02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff2_02.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(
		eLevel, TEXT("Prototype_Component_Model_GAR_Cliff2_03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Model/Map/GAR/Cliff/", "Cliff2_03.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_GAR_Stair(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	//if (FAILED(Add_Prototype_Model(LEVEL_TOOL_MAP, TEXT("Prototype_Component_Model_GAR_Cliff2_03"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
	//		"../Bin/Resource/Model/Map/GAR/Bld_Stair/",
	//		"Bld_Stair03_Sum.fbx", PivotMatrix))))
	//	return E_FAIL;






	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Maps(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("SnowMap를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_SnowMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/SnowMap/",
			"SnowMap.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("FireManMap를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_FireManMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Map_FireMan_Boss.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("StartingTown를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_StartingTown"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/", "Map_StartingTown.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_BezCastle(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BezCastle01_002"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BezCastle/",
			"BezCastle01_002.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BezCastle01_004"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BezCastle/",
			"BezCastle01_004.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BezCastle01_006"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BezCastle/",
			"BezCastle01_006.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BezCastle01_007"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BezCastle/",
			"BezCastle01_007.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BezCastleAll02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BezCastle/",
			"BezCastleAll02.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Elevator(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Elevator"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Elevator/",
			"Elevator.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Elevator_Stair"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Stair/",
			"ElevatorStair.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_RailPilar(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_PillarTrain_01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/RailPillar/",
			"PillarTrain_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_PillarTrain_02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/RailPillar/",
			"PillarTrain_02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_PillarTrain_03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/RailPillar/",
			"PillarTrain_03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_PillarTrain_04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/RailPillar/",
			"PillarTrain_04.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_PillarTrain_05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/RailPillar/",
			"PillarTrain_05.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_PillarTrain_06"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/RailPillar/",
			"PillarTrain_06.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Railway(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_RailwayBridge"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Railway/",
			"RailwayBridge.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BezCastleBridge"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Railway/",
			"BezCastleBridge.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Rock(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_CliffRock01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"CliffRock01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_CliffRock01_Snow"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"CliffRock01_Snow.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_CliffRock02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"CliffRock02.fbx", PivotMatrix))))
		return E_FAIL;






	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertCliffRock01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertCliffRock01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertCliffRock02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertCliffRock02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertCliffRock03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertCliffRock03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertCliffRock04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertCliffRock04.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertCliffRock05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertCliffRock05.fbx", PivotMatrix))))
		return E_FAIL;





	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertRock01_a"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertRock01_a.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertRock01_b"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertRock01_b.fbx", PivotMatrix))))
		return E_FAIL;





	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertRock03_a"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertRock03_a.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertRock03_b"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertRock03_b.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertRock03_c"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertRock03_c.fbx", PivotMatrix))))
		return E_FAIL;





	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertRock04_a"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertRock04_a.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertRock04_b"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertRock04_b.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DesertRock04_c"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DesertRock04_c.fbx", PivotMatrix))))
		return E_FAIL;




	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DistantRock01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DistantRock01.fbx", PivotMatrix))))
		return E_FAIL;




	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DistantRock01_a"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DistantRock01_a.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DistantRock01_b"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DistantRock01_b.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DistantRock01_c"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DistantRock01_c.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DistantRock01_d"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"DistantRock01_d.fbx", PivotMatrix))))
		return E_FAIL;




	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_FireGate_01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"FireGate_01.fbx", PivotMatrix))))
		return E_FAIL;




	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_MiddleCliff01_a"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"MiddleCliff01_a.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_MiddleCliff01_b"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"MiddleCliff01_b.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_MiddleCliff01_c"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"MiddleCliff01_c.fbx", PivotMatrix))))
		return E_FAIL;





	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_StrangeRock02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"StrangeRock02.fbx", PivotMatrix))))
		return E_FAIL;





	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_YOKRocks01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"YOKRocks01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_YOKRocks02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Rock/",
			"YOKRocks02.fbx", PivotMatrix))))
		return E_FAIL;




	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Tree(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Dead_Tree01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"Dead_Tree01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Dead_Tree02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"Dead_Tree02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Dead_Tree03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"Dead_Tree03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Juniper_Tree05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"Juniper_Tree05.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Juniper_Tree06"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"Juniper_Tree06.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_MP_Tree03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"MP_Tree03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_MP_Tree04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"MP_Tree04.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_MP_Tree05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"MP_Tree05.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_MP_Tree06"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Tree/",
			"MP_Tree06.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Grass(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Grass01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Grass/",
			"Grass01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Grass02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Grass/",
			"Grass02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Grass03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Grass/",
			"Grass03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Grass05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Grass/",
			"Grass05.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Grass08"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Grass/",
			"Grass08.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Ivy(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Ivy01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Ivy01.fbx", PivotMatrix))))
		return E_FAIL;
	
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Ivy02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Ivy02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Ivy03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Ivy03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Red01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Red01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Red02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Red02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Red03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Red03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Yellow01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Yellow01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Yellow02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Yellow02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ivy_Yellow03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ivy/",
			"Ivy_Yellow03.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_DRTHouse(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BrokenHouse02_B"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"BrokenHouse02_B.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BrokenHouse02_C"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"BrokenHouse02_C.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse05.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse06"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse06.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse07"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse07.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse08"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse08.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse09"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse09.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse10.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse11.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_DRTHouse12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/DRTHouse/",
			"DRTHouse12.fbx", PivotMatrix))))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_TruckRoad(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_TruckRoad_Curve0"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/TruckRoad/",
			"TruckRoad_Curve0.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_TruckRoad_Curve10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/TruckRoad/",
			"TruckRoad_Curve10.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_TruckRoad_Curve15"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/TruckRoad/",
			"TruckRoad_Curve15.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_TruckRoad_Curve20"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/TruckRoad/",
			"TruckRoad_Curve20.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Ors(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Ors"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Ors/",
			"Ors.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_OilPump(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_OilPump"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/OilPump/",
			"OilPump.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_IronWall(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_IronWall"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/IronWall/",
			"IronWall.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_IronWall_01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/IronWall/",
			"IronWall_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_IronWall_02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/IronWall/",
			"IronWall_02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_IronWall_03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/IronWall/",
			"IronWall_03.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_BrokenHouse(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BrokenHouse01_A"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BrokenHouse/",
			"BrokenHouse01_A.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BrokenHouse01_B"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BrokenHouse/",
			"BrokenHouse01_B.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BrokenHouse01_C"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BrokenHouse/",
			"BrokenHouse01_C.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_BrokenHouse02_A"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/BrokenHouse/",
			"BrokenHouse02_A.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_GateScrap(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01_gate_a"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01_gate_a.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01_gate_b"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01_gate_b.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01_wall_a"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01_wall_a.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01_wall_c"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01_wall_c.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01_wall_d"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01_wall_d.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01_wall_e"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01_wall_e.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01_wall01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01_wall01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01_wall02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01_wall02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap01b_MRG"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap01b_MRG.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GateScrap04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/GateScrap/",
			"GateScrap04.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_GRN_Boss_Stair(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GRN_Boss_Stair"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Map/GRN_Boss_Stair/",
			"GRN_Stair03.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Cave(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GRN_Prop_Rock01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Cave/",
			"GRN_Prop_Rock01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GRN_Prop_Torch"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Cave/",
			"GRN_Prop_Torch.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_FireGate(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_FireGate"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/FireGate/",
			"FireGate.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Balseph_Enterance(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	// 0.001f * 1.7f = 0.0017f;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Balseph_Enterance"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Map_Balseph_Enterance.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Balseph(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	// 보스 전투 맵
	// 0.05f * 0.045f = 0.00225f
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Balseph"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Map_Balseph.fbx", PivotMatrix))))
		return E_FAIL;

	// 가구
	lstrcpy(m_szLoading, TEXT("Balseph맵의 가구를 로드중입니다."));
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture02.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture03.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture04.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture05.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture07"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture07.fbx", PivotMatrix))))
		return E_FAIL;

	// 0.003f * 0.65f = 0.00195f;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture08_Picture"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture08_Picture.fbx", PivotMatrix))))
		return E_FAIL;


	// 0.003f * 0.4f = 0.0012f;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture09"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture09.fbx", PivotMatrix))))
		return E_FAIL;

	// 횟불
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Torchlight_Balseph"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Torchlight_Balseph.fbx", PivotMatrix))))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_WarpGate(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_WarpGate"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/WarpGate/",
			"WarpGate.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_TorchLight(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	// 횟불
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Torchlight_Balseph"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Torchlight_Balseph.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GRN_Prop_Torch"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/Cave/",
			"GRN_Prop_Torch.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_GRN_Chandelier"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"GRN_Chandelier.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Furniture(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	// 가구
	lstrcpy(m_szLoading, TEXT("Balseph맵의 가구를 로드중입니다."));
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture02.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture03"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture03.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture04.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture05.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture07"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture07.fbx", PivotMatrix))))
		return E_FAIL;

	// 0.003f * 0.65f = 0.00195f;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture08_Picture"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture08_Picture.fbx", PivotMatrix))))
		return E_FAIL;


	// 0.003f * 0.4f = 0.0012f;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Furniture09"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Furniture09.fbx", PivotMatrix))))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Anim_Objects(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	// 가구
	lstrcpy(m_szLoading, TEXT("Balseph맵의 Stair을 로드중입니다."));
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Stair_Object"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Map/D03_GRN/",
			"Map_Balseph_Stair.fbx", PivotMatrix, false))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Owls(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	// 올빼미
	lstrcpy(m_szLoading, TEXT("Owl을 로드중입니다."));
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PivotMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_001"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_001.fbx", PivotMatrix, false))))
		return E_FAIL;

	/*if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_002"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_002.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_003"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_003.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_004"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_004.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_005"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_005.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_006"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_006.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_007"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_007.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_008"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_008.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_009"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_009.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_010"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_010.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_011"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_011.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_012"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_012.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_013"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_013.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_014"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_014.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_015"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_015.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Owl_016"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/Owl/",
			"Owl_016.fbx", PivotMatrix, false))))
		return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Anim_OilPump(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	// 가구
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_OilPump02_Anim"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/MapObejct/OilPump/",
			"OilPump02_Anim.fbx", PivotMatrix, false))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_OilPump02_NonAnim"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/MapObejct/OilPump/",
			"OilPump02_NonAnim.fbx", PivotMatrix, false))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::ASDF()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MapTool, TEXT("Prototype_Component_Texture_Fire0"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Sprite/Fire/Fire0(%d).png"), 64))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fire"),
		CFire_Deco::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_GrandYork(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	//if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_GrandYork"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
	//		"../Bin/Resources/UMap/Map_GrandYork/",
	//		"Map_GrandYork.fbx", PivotMatrix))))
	//	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_GrandYork_HeightMapMesh를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_GrandYork_HeightMapMesh"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_GrandYork_HeightMapMesh.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_GrandYork_MapMesh를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_GrandYork_MapMesh"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_GrandYork_MapMesh.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_GrandYork_NaviMap를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_GrandYork_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_GrandYork_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Boss_Balseph(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Boss_Balseph를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_Balseph"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Boss_Balseph.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Boss_Balseph_NaviMap를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_Balseph_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Boss_Balseph_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Boss_FireAvatar(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Boss_FireAvatar를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_FireAvatar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Boss_FireAvatar.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Boss_FireAvatar_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_FireAvatar_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Boss_FireAvatar_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Cave_01(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Cave_01를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Cave_01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Cave_01.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Cave_01_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Cave_01_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Cave_01_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Cave_05(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Cave_05를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Cave_05"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Cave_05.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Cave_05_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Cave_05_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Cave_05_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Cave_07(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Cave_07를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Cave_07"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Cave_07.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Cave_07_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Cave_07_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_Cave_07_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_GrandYork_Battle_02(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_GrandYork_Battle_02를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_GrandYork_Battle02.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_GrandYork_Battle_02_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_02_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_GrandYork_Battle02_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_GrandYork_Battle_04(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_GrandYork_Battle_04를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_GrandYork_Battle04.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_GrandYork_Battle_04_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_04_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"Map_GrandYork_Battle04_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Phase01(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Boss_Lord_Vholran_Igniseri_Phase01를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_Lord_Vholran_Igniseri_Phase01"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/Boss/",
			"Boss_Lord_Vholran_Igniseri_Phase01.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Boss_Lord_Vholran_Igniseri_Phase01_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_Lord_Vholran_Igniseri_Phase01_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/Boss/",
			"Boss_Lord_Vholran_Igniseri_Phase01_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;



	lstrcpy(m_szLoading, TEXT("Map_Boss_Lord_Vholran_Igniseri_Enterance를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_Lord_Vholran_Igniseri_Enterance"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/Enterance/",
			"Boss_Lord_Vholran_Igniseri_Enterance.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Boss_Lord_Vholran_Igniseri_Enterance_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_Lord_Vholran_Igniseri_Enterance_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/Enterance/",
			"Boss_Lord_Vholran_Igniseri_Enterance_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;





	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Phase02(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Boss_Lord_Vholran_Igniseri_Phase02를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_Lord_Vholran_Igniseri_Phase02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/Boss/",
			"Boss_Lord_Vholran_Igniseri_Phase02.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Boss_Lord_Vholran_Igniseri_Phase02_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Boss_Lord_Vholran_Igniseri_Phase02_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/Boss/",
			"Boss_Lord_Vholran_Igniseri_Phase02_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Tower_of_the_Wind_Spirit_Inside(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Tower_of_the_Wind_Spirit_Inside를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Tower_of_the_Wind_Spirit_Inside_Etc"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/GAR/",
			"Tower_ExceptTop_Etc.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Tower_of_the_Wind_Spirit_Inside_Outside"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/GAR/",
			"Tower_ExceptTop_Outside.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Tower_of_the_Wind_Spirit_Inside_Inside"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/GAR/",
			"Tower_ExceptTop_Inside.fbx", PivotMatrix))))
		return E_FAIL;
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Tower_of_the_Wind_Spirit_Inside_Scenery"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/GAR/",
			"Tower_ExceptTop_Scenery.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Tower_of_the_Wind_Spirit_Inside_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Tower_of_the_Wind_Spirit_Inside_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/GAR/",
			"Tower_ExceptTop_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;






	lstrcpy(m_szLoading, TEXT("Model_Wind_Tower_Candle을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Wind_Tower_Candle"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/GAR/Add_Object/",
			"Wind_Tower_Candle.fbx", PivotMatrix))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Tower_of_the_Wind_Spirit_Top(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Map_Tower_of_the_Wind_Spirit_Inside를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Tower_of_the_Wind_Spirit_Top"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/GAR/",
			"Tower_Top.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Map_Tower_of_the_Wind_Spirit_Inside_NaviMap을 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Map_Tower_of_the_Wind_Spirit_Top_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/GAR/",
			"Tower_Top_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Object(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/AddObject/Light/",
			"DEL_Light.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/AddObject/Light/",
			"DEL_Light2.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light_Tower를 로드중입니다."));
	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light_Tower"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/UMap/DEL/AddObject/Light/",
			"DEL_Light_Tower.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_LEVEL_MapTool_Model_Standard_Player(LEVEL eLevel)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_StandardPlayer"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Player/",
			"Standard_Player.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(Add_Prototype_Model(eLevel, TEXT("Prototype_Component_Model_Blender"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/UMap/Map_GrandYork/",
			"untitled.fbx", PivotMatrix, false))))
		return E_FAIL;




	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Add_Prototype_Model(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// ImGUI에 pPrototypeTag 추가
	CImGUI_Manager* pImGUI_Manager = CImGUI_Manager::GetInstance();
	pImGUI_Manager->PushMeshList(pPrototypeTag);

	// GameInstance에 프로토타입 추가
	if (FAILED(pGameInstance->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_MapTool_To_TutorialLevel_Object()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// Camera_Default 관련
	lstrcpy(m_szLoading, TEXT("Camera_MapTool를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_MapTool"),
		CCamera_MapTool::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	// MapObject 관련
	lstrcpy(m_szLoading, TEXT("MapObject를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	// Picking_Rect 관련
	lstrcpy(m_szLoading, TEXT("Picking_Rect를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Picking_Rect"),
		CPicking_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	// Dot 관련
	lstrcpy(m_szLoading, TEXT("Dot을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Dot"),
		CDot::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	
	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
