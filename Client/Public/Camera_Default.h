#pragma once

#include "Client_Defines.h"
#include "Camera.h"
#include "Player_Manager.h"
BEGIN(Client)

class CCamera_Default final : public CCamera
{
private:
	explicit CCamera_Default(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CCamera_Default(const CCamera_Default& rhs);
	virtual ~CCamera_Default() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();


public:
	/*	������ǥ��	*/
	void SphericalCoordinates(_fvector vCamerapos, _fvector vTargetPos);
	void SphericalCoordinatesRotate(_float newAzimuth, _float newElevation);
	void SphericalCoordinatesTranslateRadius(_float newRadius);
	_vector toCartesian();
	void CartesianToLocal(_fvector vTargetPos);


public:
	//�÷��̾� ����
	void Set_PlayerIndex(_uint iIndex) { m_iPlayerIndex = iIndex; }
	void TagetChange();
	void Set_Change(_bool bChange) { m_bChange = bChange; }

public:
	static CCamera_Default* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float m_fRadius = 0.f;  //������ǥ�� ������
	_float m_fAzimuth = 0.f; //������ x
	_float m_fElevation = 0.f; //�Ӱ� y
	_float m_fT = 0.f;			//���
	_float m_minRadius = 0.f;
	_float m_maxRadius = 0.f;
	_float m_minAzimuth = 0.f;
	_float m_RadMinAzimuth = 0.f;
	_float m_maxAzimuth = 0.f;
	_float m_RadMaxAzimuth = 0.f;
	_float m_minElevation = 0.f;
	_float m_RadminElevation = 0.f;
	_float m_maxElevation = 0.f;
	_float m_RadmaxElevation = 0.f;
	_bool m_bSpherical = true;		//������ǥ�� ��������
	_bool m_bSphericalStart = false; //������ǥ�� ��� �ѹ� ���ؾ��ؼ� ó���� ȣ��
	_float m_fCameraLookatPlayerHeight = 0.f;
	_float m_fMouseSensitivity = 0.f; //���콺 ��������



	_bool m_bChange = false;



	CPlayer_Manager* m_pPlayer_Manager = nullptr;
	_uint m_iPlayerIndex = 0;




};

END