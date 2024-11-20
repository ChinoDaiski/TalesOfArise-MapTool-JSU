#pragma once
#include "Client_Defines.h"
#include "Base.h"

#include"GameObject.h"


BEGIN(Client)

class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

	enum LEVELUI {LEVEL_TUTORIAL, LEVEL_INVEN_LOBBY, LEVEL_INVEN_ITEM, LEVEL_INVEN_ARMY, LEVEL_BOSS1, LEVEL_BOSS2, LEVEL_END};
	enum UIPLAYER {PLAYER_ALPHEN, PLAYER_SION, PLAYER_RINWELL, PLAYRE_KAKURA, PLAYER_END};
	enum UIKIND {ACQUIRE, KIND_END};
private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	typedef struct tagPlayerInfo {
		_int iMaxCp = 0;
		_int iCurrentCp = 0;
		_int iMaxHp = 0;
		_int iCurrentHp = 0;
		_int iComboHits = 0;
		_int iMaxAg = 0;
		_int iCurrentAg = 0;
	}PLAYERINFO;

	PLAYERINFO PlayerInfo;




public:
	void pushUI(LEVELUI lv, UIKIND UIKind, void* pArg =NULL); // 레벨별로, 종류별로, 옵션값별로 다른 UI를 Push할 예정.
	PLAYERINFO& GetPlayerInfo(UIPLAYER player);
	void SetPlayerInfo(UIPLAYER player, PLAYERINFO& playerinfo);
	void SetPlayer(UIPLAYER player, CGameObject* gameObject);
	CGameObject* GetPlayer(UIPLAYER player);
	int GetAlphenInfo();
	void SetAlphenCombo(_uint combohit);
private:
	
	CGameObject* PlayerAlphen = nullptr;
	CGameObject* PlayerSion = nullptr;
	CGameObject* PlayerKakura = nullptr;
	CGameObject* PlayerRinwell = nullptr;
private:
	map<LEVELUI, UIKIND> UIMap;
	map<UIPLAYER, PLAYERINFO*> InfoMap;

public:
	virtual void Free() override;
};

END