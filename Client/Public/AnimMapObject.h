#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CTexture;
END

BEGIN(Client)

class CAnimMapObject : public CGameObject {
public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK, TYPE_END };

public:
	typedef struct _tagMapObjectDesc {
		const _tchar*	pPrototype_ObjectName = nullptr;	// ������Ÿ�� ������Ʈ �±�
		_float4x4	TransformMatrix;	// �̵� ���
		_float3		fColliderSize;		// �浹ü�� ������
		_bool		m_bMap = false;
	}MAPOBJDesc;

protected:
	explicit CAnimMapObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CAnimMapObject(const CAnimMapObject& rhs);
	virtual ~CAnimMapObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	CCollider* Get_Collider(void) { return m_pSphereCom; }

protected:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderNonAnimCom = nullptr;
	CShader*			m_pShaderAnimCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;

	CTexture*			m_pTextureCom[TYPE_END] = { nullptr };

protected:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_ConstantTable();

public:
	const _float3 Get_Rotation(void) { return m_fRotation; }
	void Set_Rotation(_float3 fRotation) { m_fRotation = fRotation; }
	const TCHAR* Get_ModelTag(void) { return m_pModelTag; }
	_bool isMap(void) { return m_bMap; }


private:
	TCHAR m_pModelTag[MAX_PATH];
	_float3 m_fRotation = _float3{ 0.f, 0.f, 0.f };
	_bool m_bMap = false;

public:
	static CAnimMapObject* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	_uint Get_FeatureNumber(void) { return m_iFeatureMeshNumber; }

private:
	_uint m_iFeatureMeshNumber = 0;

private:
	static _uint iFeatureMeshNumber;	// ��ġ�� �޽��� �� ����


private:
	_bool m_isTerrainMesh = false;
};

END