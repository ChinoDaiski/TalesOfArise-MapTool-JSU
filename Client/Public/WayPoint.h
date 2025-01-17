#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CPlayer;

// 캐릭터가 웨이포인트 이용시, 해당 객체의 월드 위치로 이동하고, 캐릭터의 카메라는 m_vCameraLook방향을 바라보게 한다.

class CWayPoint final : public CGameObject
{
public:
	typedef struct _tagWayPoint_DESC {
		_float3		fPos;			// 웨이 포인트의 위치
		_float3		fDir;			// 카메라의 방향
		_uint		iPointNumber;	// 웨이 포인트의 번호, 해당 번호로 웨이 포인트를 찾는다.

		const TCHAR* pTagNavigation = nullptr;	// 이동될 맵의 navigation 컴포넌트 값

		CPlayer* pTargetPlayer = nullptr;	// 충돌 처리를 할 플레이어 오브젝트 정보
	}WayPoint_DESC;

private:
	CWayPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CWayPoint(const CWayPoint& rhs);
	virtual ~CWayPoint() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_ConstantTable();

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;

public:
	HRESULT Activate_WayPoint(CPlayer* pPlayer);

	HRESULT Check_TargetPlayer_Collision(void);

public:
	void Set_Destination(CWayPoint* pDestWayPoint) { m_pDestWayPoint = pDestWayPoint; }
	CWayPoint* Get_Destination(void) { return m_pDestWayPoint; }
	_float3 Get_CameraLook(void) { return m_fCameraLook; }
	WAYPOINT	Get_WayPoint(void) { return m_eWayPoint; }
	void Set_TargetPlayer(CPlayer* pPlayer) { m_pTargetPlayer = pPlayer; }
	CPlayer* Get_TargetPlayer(void) { return m_pTargetPlayer; }

private:
	CWayPoint*	m_pDestWayPoint = nullptr;		// 이동될 웨이 포인트 정보
	_float3		m_fCameraLook;					// 이동시 카메라의 방향
	WAYPOINT	m_eWayPoint = WAYPOINT_END;
	CPlayer*	m_pTargetPlayer = nullptr;

public:
	static CWayPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END