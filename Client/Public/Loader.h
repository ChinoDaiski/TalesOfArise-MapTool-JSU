#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 레벨별로 로딩해야할 리소스 생성한다.(스레드로)  */

BEGIN(Engine)

class CComponent;

END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	explicit CLoader(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CriticalSection() {
		return m_CriticalSection;
	}

	LEVEL Get_LevelIndex() {
		return m_eLevel;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoading;
	}

public:
	HRESULT NativeConstruct(LEVEL eLevel);


	// ======================== LobbyLevel ========================
public:
	HRESULT Loading_ForLobbyLevel();
private:



	// ======================== GamePlay ========================
public:
	HRESULT Loading_ForGamePlayLevel();
private:
	HRESULT Loading_GamePlayLevel_Texture();
	HRESULT Loading_GamePlayLevel_Object();
	HRESULT Loading_GamePlayLevel_Shader();
	HRESULT Loading_GamePlayLevel_Model();
	HRESULT Loading_GamePlayLevel_Component();
	HRESULT Loading_GamePlayLevel_Collider();


	// ======================== Boss1 ========================
public:
	HRESULT Loading_ForBoss1();
private:



	// ======================== Boss2 ========================
public:
	HRESULT Loading_ForBoss2();
private:



	// ======================== MapTool ========================
public:
	HRESULT Loading_ForMapToolLevel();
private:
	HRESULT Loading_MapToolLevel_Texture(LEVEL eLevel);
	HRESULT Loading_MapToolLevel_Object();
	HRESULT Loading_MapToolLevel_Shader();
	HRESULT Loading_MapToolLevel_Model(LEVEL eLevel);
	HRESULT Loading_MapToolLevel_Component();

private:
	HRESULT Loading_LEVEL_MapTool_Component_HeightMap();
	HRESULT Loading_LEVEL_MapTool_Component_Collider();

private:
	HRESULT Loading_LEVEL_MapTool_Model_BattleMap(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_GAR_Cliff(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_GAR_Stair(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Maps(LEVEL eLevel);

private:
	// 1번째 맵에서 사용할 리소스
	HRESULT Loading_LEVEL_MapTool_Model_BezCastle(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Elevator(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_RailPilar(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Railway(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Rock(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Ivy(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_DRTHouse(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_TruckRoad(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Ors(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_OilPump(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_IronWall(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_BrokenHouse(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_GateScrap(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_GRN_Boss_Stair(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Cave(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_FireGate(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Balseph_Enterance(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Balseph(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_WarpGate(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Grass(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Tree(LEVEL eLevel);

	HRESULT Loading_LEVEL_MapTool_Model_TorchLight(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Furniture(LEVEL eLevel);

	HRESULT Loading_LEVEL_MapTool_Model_Anim_Objects(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Owls(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Anim_OilPump(LEVEL eLevel);

	HRESULT ASDF();


	HRESULT Loading_LEVEL_MapTool_Model_Map_GrandYork(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Map_Boss_Balseph(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Map_Boss_FireAvatar(LEVEL eLevel);

	HRESULT Loading_LEVEL_MapTool_Model_Map_Cave_01(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Map_Cave_05(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Map_Cave_07(LEVEL eLevel);

	HRESULT Loading_LEVEL_MapTool_Model_Map_GrandYork_Battle_02(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Map_GrandYork_Battle_04(LEVEL eLevel);

	HRESULT Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Phase01(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Phase02(LEVEL eLevel);

	HRESULT Loading_LEVEL_MapTool_Model_Map_Tower_of_the_Wind_Spirit_Inside(LEVEL eLevel);
	HRESULT Loading_LEVEL_MapTool_Model_Map_Tower_of_the_Wind_Spirit_Top(LEVEL eLevel);

	HRESULT Loading_LEVEL_MapTool_Model_Map_Boss_Lord_Vholran_Igniseri_Object(LEVEL eLevel);



	HRESULT Loading_LEVEL_MapTool_Model_Standard_Player(LEVEL eLevel);


private:
	HRESULT Add_Prototype_Model(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype);






	// ======================== MapTool To Tutorial Level ========================
private:
	HRESULT Loading_MapTool_To_TutorialLevel_Object();






private:
	LEVEL			m_eLevel = LEVEL_END;
	_bool			m_isFinished = false;
	_tchar			m_szLoading[MAX_PATH] = TEXT("");

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	HANDLE				m_hThread = 0;
	CRITICAL_SECTION	m_CriticalSection;

public:
	static CLoader* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eLevel);
	virtual void Free() override;
};

END