#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)

class CTerrain final : public CGameObject {
public:
	typedef struct _tagTerrainDesc {
		const TCHAR* pPrototypeTag_Terrain = nullptr;
	} TerrainDesc;

public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END };

private:
	explicit CTerrain(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*			m_pTextureCom[TYPE_END] = { nullptr };
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	CVIBuffer_Terrain*	m_pVIBufferCom = nullptr;

#ifdef _DEBUG
	// 네비메쉬 관련 변수 및 함수 추가
public:
	HRESULT SetUp_NaviComponent(const TCHAR* pNaviDataFilePath);

private:
	CNavigation*		m_pNaviCom = nullptr;
#endif // _DEBUG

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	HRESULT Create_MaskTexture();
	
public:
	static CTerrain* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END