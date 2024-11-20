#include "stdafx.h"
#include "..\Public\WayPoint.h"
#include "Player.h"

#include "Level_Balseph.h"
#include "Level_Loading.h"

CWayPoint::CWayPoint(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CWayPoint::CWayPoint(const CWayPoint & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWayPoint::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSG_BOX(L"CWayPoint -> NativeConstruct_Prototype -> NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CWayPoint::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWayPoint::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	Check_TargetPlayer_Collision();

	// 충돌체 업데이트
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CWayPoint::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

#ifdef _DEBUG
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
#endif // _DEBUG
}

HRESULT CWayPoint::Render()
{
#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CWayPoint::SetUp_Components(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	WayPoint_DESC Desc = *((WayPoint_DESC*)pArg);

	// 방향 설정
	m_fCameraLook = Desc.fDir;

	// 위치 설정
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(Desc.fPos.x, Desc.fPos.y, Desc.fPos.z, 1.f));



	// 원형 충돌체 생성
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);		// 상대적 위치 및 반지름 생성
	ColliderDesc.fRadius = 1.f;
	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	


	return S_OK;
}

HRESULT CWayPoint::SetUp_ConstantTable()
{
	return S_OK;
}

HRESULT CWayPoint::Activate_WayPoint(CPlayer * pPlayer)
{
	if (nullptr == m_pDestWayPoint)
		return E_FAIL;


	switch (m_eWayPoint)
	{
	case Client::Tutorial_Town_Start:
		// Tutorial NaviMesh를 플레이어에게 넣어준다.
		// 초기 시작 지점으로 이동한다.
		break;
	case Client::Tutorial_Town_End:
		// Tutorial_Desert_Start으로 이동한다.
		break;
	case Client::Tutorial_Desert_Start:
		break;
	case Client::Tutorial_Desert_End:
		// Balseph_Enterance_Start로 이동한다.
		// Balseph level로 바꾼다.
		// 기존에 있던 오브젝트를 날리고, 새로운 dat 파일을 열어서 오브젝트를 로드한다.
		break;
	case Client::Tutorial_Battle_Start:
		break;
	case Client::Balseph_Enterance_Start:
		break;
	case Client::Balseph_Enterance_End:
		// Balseph_Boss_Start로 이동한다.
		// 기존에 있던 오브젝트를 날리고, 새로운 dat 파일을 열어서 오브젝트를 로드한다.
		break;
	case Client::Balseph_Boss_Start:
		break;
	case Client::Balseph_Boss_End:
		break;
	case Client::Balseph_FireAvatar_Start:
		break;
	case Client::Balseph_FireAvatar_End:
		break;
	case Client::WAYPOINT_END:
		break;
	default:
		break;
	}





	// 플레이어의 위치 변경

	// 도착 지점의 위치를 가져온다.
	_vector vDestPos = dynamic_cast<CTransform*>(m_pDestWayPoint->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	// 플레이어의 위치를 바꾼다.
	dynamic_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, vDestPos);





	// 플레이어가 바라보는 방향 변경

	// 도착지점의 방향을 가져온다.
	_float3 fLook = m_pDestWayPoint->Get_CameraLook();

	// 플레이어가 바라보는 방향을 바꾼다.
	dynamic_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")))->Look(XMVectorSet(fLook.x, fLook.y, fLook.z, 1.f));



	// 카메라의 위치 변경

	// 여기서 카메라의 위치를 조정한다.

	return S_OK;
}

HRESULT CWayPoint::Check_TargetPlayer_Collision(void)
{
	if (nullptr == m_pTargetPlayer) {
		MSG_BOX(L"CWayPoint : No Player Detected!");
		return E_FAIL;
	}

	CCollider* pPlayer_Collider = dynamic_cast<CCollider*>(m_pTargetPlayer->Get_Component(TEXT("Com_SPHERE_Interaction")));

	if (nullptr == pPlayer_Collider) {
		MSG_BOX(L"CWayPoint : No Collider Detected!");
		return E_FAIL;
	}

	// 자신의 충돌체와 플레이어의 상호작용 충돌체가 충돌했을 경우
	if (m_pSphereCom->Collision(pPlayer_Collider)) {

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// 도착지점의 웨이포인트에 따라 따라 다른 위치로 전송한다.
		switch (m_pDestWayPoint->Get_WayPoint())
		{
		case Client::Tutorial_Town_Start:
			break;
		case Client::Tutorial_Town_End:
			break;
		case Client::Tutorial_Desert_Start:
			break;
		case Client::Tutorial_Desert_End:
			break;
		case Client::Tutorial_Battle_Start:
			break;
		case Client::Balseph_Enterance_Start:
			pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_BALSEPH));	// Balseph 보스맵 이동
			break;
		case Client::Balseph_Enterance_End:
			break;
		case Client::Balseph_Boss_Start:
			break;
		case Client::Balseph_Boss_End:
			break;
		case Client::Balseph_FireAvatar_Start:
			break;
		case Client::Balseph_FireAvatar_End:
			break;
		case Client::WAYPOINT_END:
			break;
		default:
			break;
		}

		RELEASE_INSTANCE(CGameInstance);
	}

	return S_OK;
}

CWayPoint * CWayPoint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CWayPoint* pInstance = new CWayPoint(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"CWayPoint -> Create -> pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWayPoint::Clone(void * pArg)
{
	CWayPoint* pInstance = new CWayPoint(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"CWayPoint -> Clone -> pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWayPoint::Free()
{
}