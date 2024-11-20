#pragma once

namespace Client
{
	const unsigned int	g_iWinCX = 1920;//1280; // 1280;
	const unsigned int	g_iWinCY = 1080;//720; // 720;

	enum LEVEL { 
		LEVEL_STATIC, 
		LEVEL_LOADING,
		LEVEL_LOGO, 
		LEVEL_LOBBY,
		LEVEL_TUTORIAL,
		LEVEL_BALSEPH,
		LEVEL_FIRE_AVATAR,
		LEVEL_BATTLE02, 
		LEVEL_BATTLE04,
		LEVEL_IGNISERI01,
		LEVEL_IGNISERI02,
		LEVEL_WIND_SPIRIT_TOP,
		LEVEL_WIND_SPIRIT_INSIDE,
		LEVEL_BOSS1,
		LEVEL_BOSS2, 
		LEVEL_MapTool,
		LEVEL_END
	};

	/*
		LEVEL_TUTORIAL : 사막(마을, 철도길, 동굴맵(?)), 사막 전투맵
		LEVEL_BALSEPH : 알현실 입구, 알현실, Fire Avatar 보스 맵
		LEVEL_BOSS2 : ?
	*/

	enum DAMAGE_TYPE { DAMAGE_TYPE_DOWN, DAMAGE_TYPE_LARGE, DAMAGE_TYPE_SMALL, DAMAGE_TYPE_END };

	enum DAMAGE_DIR { DAMAGE_DIR_FRONT, DAMAGE_DIR_BACK, DAMAGE_DIR_LEFT, DAMAGE_DIR_RIGHT, DAMAGE_DIR_END };


	enum WAYPOINT {
		Tutorial_Town_Start, Tutorial_Town_End, Tutorial_Desert_Start, Tutorial_Desert_End, Tutorial_Battle_Start,									// Tutorial 레벨
		Balseph_Enterance_Start, Balseph_Enterance_End, Balseph_Boss_Start, Balseph_Boss_End, Balseph_FireAvatar_Start, Balseph_FireAvatar_End,		// Balseph 레벨

		WAYPOINT_END
	};
}

#include <process.h>

#define  LAYER_MAPTOOL_CAMERA					TEXT("Layer_MapTool_Camera")
#define  LAYER_MAPTOOL_TERRAIN					TEXT("Layer_MapTool_Terrain")
#define  LAYER_MAPTOOL_BattleMap				TEXT("Layer_MapTool_BattleMap")
#define  LAYER_MAPTOOL_PlacedObjects			TEXT("Layer_MapTool_PlacedObjects")
#define  LAYER_MAPTOOL_WayPoints				TEXT("Layer_MapTool_WayPoints")
#define  LAYER_MAPTOOL_Picking_Rect				TEXT("Layer_MapTool_PickingB_Rect")
#define  LAYER_MAPTOOL_Dot						TEXT("Layer_MapTool_Dot")
#define  LAYER_MAPTOOL_LightObjects				TEXT("Layer_MapTool_LightObjects")
#define  LAYER_MAPTOOL_Skybox					TEXT("Layer_MapTool_Skybox")

#define PLAYER_PIVOT	


extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
