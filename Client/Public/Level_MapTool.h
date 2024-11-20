#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_MapTool final : public CLevel
{
private:
	CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_MapTool() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual void Tick(_double TimeDelta);
	virtual HRESULT Render();

public:
	static CLevel_MapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Skybox();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BattleMap(const _tchar* pLayerTag);
	HRESULT Ready_Picking_Rect();
};

END