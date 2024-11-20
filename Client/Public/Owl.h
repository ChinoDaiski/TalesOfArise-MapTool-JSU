#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CCollider;
class CRenderer;
class CModel;
class CNavigation;

END

BEGIN(Client)

class CPlayer;

class COwl final : public CGameObject
{
public:
	typedef struct _tagOwlDesc {
		_float3 fPos;		// �ξ����� ��ġ
		const TCHAR* pTagItemName = nullptr;	// ������ �̸�
	} OwlDesc;

private:
	COwl(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	COwl(const COwl& rhs);
	virtual ~COwl() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static COwl* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_uint m_iCurrentAnimationIndex = 0;
	_uint m_iNextAnimationIndex = 0;



public:
	void Check_TargetDistance(void);	// Ÿ�ٰ��� �Ÿ��� �����Ͽ� ���� �Ÿ� ���� ������ �ൿ�� �����Ѵ�.
	



public:
	void Set_TargetPlayer(CPlayer* pTargetPlayer) { m_pTargetPlayer = pTargetPlayer; }	// ���ڷ� ���� �÷��̾� ������ Ÿ������ �Ѵ�.
	
	_float Get_Distance(void) { return m_fDistance; }
	void Set_Distance(_float fDistance) { m_fDistance = fDistance; }


private:
	class CPlayer* m_pTargetPlayer = nullptr;	// Ÿ�ٰ� ���� �Ÿ� ���� ������ ���, �÷��̾ �ٶ󺸰�, �÷��̾ ��ȣ�ۿ��� �ϸ�, Ư�� �ִϸ��̼� �� UI�� ����Ѵ�.
	_float m_fDistance;		// Ÿ�ٰ��� �Ÿ� ����, Ÿ�ٰ� �Ÿ��� �ش� ������ �������, �������� �����Ѵ�.
	_bool m_bIn;			// true�� ��� ������Ʈ ��ȣ�ۿ�
	

private:
	const TCHAR* m_TagItemp = nullptr;		// ������ ����, ���߿� ������ class�� �����, ���⿡ ������ ������ �־��ش�. �켱 �̸��� �༭ UI ��ȣ�ۿ뿡 ���.




};

END