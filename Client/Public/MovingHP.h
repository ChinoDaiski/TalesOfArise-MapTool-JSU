#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class MovingHP final : public CGameObject
{
private:
	explicit MovingHP(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit MovingHP(const MovingHP& rhs);
	virtual ~MovingHP() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	_float4x4 m_ProjMatrix;
	_float m_fWinCX = 0.f, m_fWinCY = 0.f;
private:
	_double				m_Frame = 0.0;
	_float m_fX, m_fY;
public:
	HRESULT SetUp_Components();


public:
	static MovingHP* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END