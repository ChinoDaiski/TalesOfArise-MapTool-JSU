#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CLight;
END

BEGIN(Client)

class CLightObject : public CGameObject {

public:
	typedef struct _tagLightObjectDesc {
		CLight* pTargetLight;
		_float3 fColliderSize;
	}LIGHTOBJDesc;

protected:
	explicit CLightObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CLightObject(const CLightObject& rhs);
	virtual ~CLightObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CRenderer*			m_pRendererCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;

protected:
	HRESULT SetUp_Components(void* pArg);

public:
	CLight* Get_TargetLight(void) { return m_pTargetLight; }

private:
	CLight* m_pTargetLight = nullptr;

public:
	static CLightObject* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END