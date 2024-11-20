#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* �ε����� �����ش�. */
/* . �δ��� �ε��Ҽ��ֵ���. �δ���ü���� */
/* �δ����� ������ �� ����. */
BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	explicit CLevel_Loading(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual void Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

private:
	LEVEL			m_eNextLevel = LEVEL_END;
	class CLoader*	m_pLoader = nullptr;
	int num = 0;
	_float numtime = 0;
public:
	static CLevel_Loading* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eNextLevel);
	virtual void Free() override;

private:
	_tchar LoadingMsg[MAX_PATH] = TEXT("");
	//string LoadingMsg = "";


};

END