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
		_float3 fPos;		// 부엉이의 위치
		const TCHAR* pTagItemName = nullptr;	// 아이템 이름
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
	void Check_TargetDistance(void);	// 타겟과의 거리를 측정하여 일정 거리 내에 들어오면 행동을 시작한다.
	



public:
	void Set_TargetPlayer(CPlayer* pTargetPlayer) { m_pTargetPlayer = pTargetPlayer; }	// 인자로 받은 플레이어 정보를 타겟으로 한다.
	
	_float Get_Distance(void) { return m_fDistance; }
	void Set_Distance(_float fDistance) { m_fDistance = fDistance; }


private:
	class CPlayer* m_pTargetPlayer = nullptr;	// 타겟과 일정 거리 내에 들어왔을 경우, 플레이어를 바라보고, 플레이어가 상호작용을 하면, 특정 애니메이션 및 UI를 출력한다.
	_float m_fDistance;		// 타겟과의 거리 정보, 타겟과 거리가 해당 값보다 적을경우, 움직임을 시작한다.
	_bool m_bIn;			// true일 경우 오브젝트 상호작용
	

private:
	const TCHAR* m_TagItemp = nullptr;		// 아이템 정보, 나중에 아이템 class를 만들고, 여기에 아이템 정보를 넣어준다. 우선 이름만 줘서 UI 상호작용에 사용.




};

END