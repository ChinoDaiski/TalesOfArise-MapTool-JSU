#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "..\Public\ImGUI_Manager.h"

#ifdef 민희

#endif

#ifdef 성욱

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_MapTool.h"
#include "MapObject.h"
#include "Terrain.h"
#include "Cell.h"
#include "Navigation.h"
#include "Collider.h"
#include "Picking_Rect.h"
#include "Dot.h"
#include "VIBuffer_Triangle.h"
#include "Shader.h"
#include "PIpeLine.h"
#include "Model.h"
#include "MeshContainer.h"
#include "Player.h"

#include "Owl.h"
#include "Fire_Deco.h"

#include "Light.h"
#include "LightObject.h"
#include "Sky.h"

#endif

IMPLEMENT_SINGLETON(CImGUI_Manager)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CImGUI_Manager::CImGUI_Manager()
{
}

HRESULT CImGUI_Manager::Init_ImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	ImGui_ImplDX11_Init(*ppDeviceOut, *ppDeviceContextOut);

	m_pDevice = *ppDeviceOut;
	m_pDeviceContext = *ppDeviceContextOut;

	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVIBuffer)
	{
		MSG_BOX(L"Failed To CNavigation : NativeConstruct_Prototype : nullptr == m_pVIBuffer");
		return E_FAIL;
	}
	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, L"../Bin/ShaderFiles/Shader_Navigation.hlsl", VTXCOL_DECLARATION::Elements, VTXCOL_DECLARATION::iNumElements, CShader::FX);
	if (nullptr == m_pShader)
	{
		MSG_BOX(L"Failed To CNavigation : NativeConstruct_Prototype : nullptr == m_pShader");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CImGUI_Manager::StartFrame(void)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return S_OK;
}

HRESULT CImGUI_Manager::Set_Contents(void)
{
#ifdef 성욱
	ImGUI_SeongUk();
#endif // !성욱

#ifdef 민희
	ImGUI_MinHee();
#endif // !민희

	return S_OK;
}

HRESULT CImGUI_Manager::Tick(_double TimeDelta)
{
#ifdef 성욱
	if (FAILED(Tick_SeongUk(TimeDelta)))
		return E_FAIL;
#endif // !성욱

#ifdef 민희
	if (FAILED(Tick_MinHee()))
		return E_FAIL;
#endif // !민희

	return S_OK;
}

HRESULT CImGUI_Manager::Render(void)
{
	// Imgui render옵션이 켜져 있다면
	if (m_bImguiEnable)
	{
		StartFrame();
		Set_Contents();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	return S_OK;
}

void CImGUI_Manager::InitWin32(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
}

LRESULT CImGUI_Manager::WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
}
	
void CImGUI_Manager::Free()
{
	Shutdown();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	// Safe_Release(m_pDevice), Safe_Release(m_pDeviceContext) 처리를 ImGui_ImplWin32_Shutdown에서 해준다.

	ImGui::DestroyContext();
}

#ifdef 성욱

void CImGUI_Manager::Shutdown(void)
{
}

HRESULT CImGUI_Manager::Tick_SeongUk(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 플레이어의 정보가 존재한다면
	if (nullptr != m_pClient_Player)
		// 해당 플레이어의 위치 정보를 좌상단에 출력한다.
		Render_PlayerPos();



	// 선택한 툴이 Sky 관련 툴일 경우
	if (m_eUI == UI_LEVEL::UL_Sky) {

	}


	// 선택한 툴이 Light 관련 툴일 경우
	if (m_eUI == UI_LEVEL::UL_Lights) {

		// Q버튼을 누르고 있을 경우
		if (pGameInstance->Key_Pressing(DIK_Q)) {	
			CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_MapTool, LAYER_MAPTOOL_LightObjects);

			if (nullptr != pLayer) {
				auto& Objlist = pLayer->Get_ObjectList();

				CPicking_Manager::RAYDESC RayDesc = pGameInstance->Get_vRayDesc();

				for (auto& Obj : Objlist) {
					CCollider* pSphereCom = dynamic_cast<CCollider*>(Obj->Get_Component(TEXT("Com_Sphere")));

					_float fDist;
					if (pSphereCom->Collision_Lay(XMLoadFloat3(&RayDesc.vRayPos), XMLoadFloat3(&RayDesc.vRay), fDist)) {
						m_pSelected_PointLight = dynamic_cast<CLightObject*>(Obj)->Get_TargetLight();
					}
				}
			}
		}

		if (nullptr != m_pSelected_PointLight) {
			LIGHTDESC LightDesc = *m_pSelected_PointLight->Get_LightDesc();
			_float3 fPos = _float3(LightDesc.vPosition.x, LightDesc.vPosition.y, LightDesc.vPosition.z);
			_vector vPos = XMLoadFloat3(&fPos);

			// -x
			if (pGameInstance->Key_Pressing(DIK_NUMPAD4)) {
				vPos -= dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_RIGHT) * TimeDelta * m_fMeshSpeed;
			}
			// +x
			if (pGameInstance->Key_Pressing(DIK_NUMPAD6)) {
				vPos += dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_RIGHT) * TimeDelta * m_fMeshSpeed;
			}

			// +z
			if (pGameInstance->Key_Pressing(DIK_NUMPAD8)) {
				_float3 fObjPos;
				XMStoreFloat3(&fObjPos, vPos);


				vPos += dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_LOOK) * TimeDelta * m_fMeshSpeed;
				fObjPos.x = XMVectorGetX(vPos);
				fObjPos.z = XMVectorGetZ(vPos);

				vPos = XMLoadFloat3(&fObjPos);
			}
			// -z
			if (pGameInstance->Key_Pressing(DIK_NUMPAD2)) {
				_float3 fObjPos;
				XMStoreFloat3(&fObjPos, vPos);


				vPos -= dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_LOOK) * TimeDelta * m_fMeshSpeed;
				fObjPos.x = XMVectorGetX(vPos);
				fObjPos.z = XMVectorGetZ(vPos);

				vPos = XMLoadFloat3(&fObjPos);
			}

			// +y
			if (pGameInstance->Key_Pressing(DIK_NUMPAD7)) {
				vPos += dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_UP) * TimeDelta * m_fMeshSpeed;
			}
			// -y
			if (pGameInstance->Key_Pressing(DIK_NUMPAD9)) {
				vPos -= dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_UP) * TimeDelta * m_fMeshSpeed;
			}

			XMStoreFloat3(&fPos, vPos);

			LightDesc.vPosition = _float4(fPos.x, fPos.y, fPos.z, 1.f);
			m_pSelected_PointLight->Set_LightDesc(LightDesc);
		}

	}
	



	if (m_eUI == UI_LEVEL::UL_CreateFire) {
		
		// 선택한 기준 오브젝트 관련 불 정보 그리기
		ImGUI_Show_Fire_Standard_Object(TimeDelta);
		ImGUI_Show_Fire_Object(TimeDelta);

		if (nullptr != m_pSelected_FireObject) {

			// Q키를 누르고 있을 경우
			if (pGameInstance->Key_Pressing(DIK_Q)) {

				// m_pSelected_FireObject의 위치 컴포넌트를 가져온다.
				CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelected_FireObject->Get_Component(TEXT("Com_Transform")));

				// 4번 키를 누를 경우(좌로 이동)
				if (pGameInstance->Key_Pressing(DIK_NUMPAD4)) {
					pTransform->Go_Left(TimeDelta * m_fMeshSpeed);
				}
				// 6번 키를 누를 경우(우로 이동)
				if (pGameInstance->Key_Pressing(DIK_NUMPAD6)) {
					pTransform->Go_Right(TimeDelta * m_fMeshSpeed);
				}
				// 2번 키를 누를 경우(앞으로 이동)
				if (pGameInstance->Key_Pressing(DIK_NUMPAD8)) {
					pTransform->Go_ForWard(TimeDelta * m_fMeshSpeed);
				}
				// 8번 키를 누를 경우(뒤로 이동)
				if (pGameInstance->Key_Pressing(DIK_NUMPAD2)) {
					pTransform->Go_BackWard(TimeDelta * m_fMeshSpeed);
				}
				// 7번 키를 누를 경우(위로 이동)
				if (pGameInstance->Key_Pressing(DIK_NUMPAD7)) {
					pTransform->Go_Up(TimeDelta * m_fMeshSpeed);
				}
				// 9번 키를 누를 경우(아래로 이동)
				if (pGameInstance->Key_Pressing(DIK_NUMPAD9)) {
					pTransform->Go_Down(TimeDelta * m_fMeshSpeed);
				}
			}
		}
	}

	if (m_eUI == UI_LEVEL::UL_SetObject) {
		// Q키를 누르고 있을 경우
		if (pGameInstance->Key_Pressing(DIK_Q)) {

			// 마우스 왼쪽 버튼을 눌렀을 경우
			if (pGameInstance->Button_Down(CInput_Device::DIMB_LBUTTON)) {

				if (nullptr != m_pPickingMeshObject) {
					vector<class CMeshContainer*>& vMeshContainer = dynamic_cast<CModel*>(m_pPickingMeshObject->Get_Component(TEXT("Com_Model")))->Get_MeshContainer();

					_vector vDist;
					_vector vPos;
					_bool isPicking = false;
					map<_float, _vector> Dists;
					Dists.clear();

					for (auto& Container : vMeshContainer) {
						VTXNONANIMMODEL* pVertices = Container->Get_NonAnimVertices();
						FACELISTINDICES32* pIndicies = Container->Get_Indices();
						_uint iNumPrimitive = Container->Get_PrimitiveCnt();
						_matrix matWorld = XMMatrixIdentity();

						for (int i = 0; i < iNumPrimitive; ++i) {
							if (pGameInstance->Picking(
								XMLoadFloat3(&pVertices[pIndicies[i]._0].vPosition),
								XMLoadFloat3(&pVertices[pIndicies[i]._1].vPosition),
								XMLoadFloat3(&pVertices[pIndicies[i]._2].vPosition),
								matWorld,
								&vPos,		// 위치
								&vDist		// 거리
							)) {
								_float fDist;
								XMStoreFloat(&fDist, vDist);

								Dists.emplace(fDist, vPos);
							}
						}
					}

					if (!Dists.empty()) {
						isPicking = true;
					}

					if (isPicking) {
						CMapObject::MAPOBJDesc Desc;
						Desc.pPrototype_ObjectName = m_pPrototypeTag_SelectedMesh;

						// Desc.TransformMatrix값을 항등행렬로 변환
						XMStoreFloat4x4(&Desc.TransformMatrix, XMMatrixIdentity());

						// 피킹된 위치로 행렬의 위치값 변경
						vDist = XMVectorSetW(Dists.begin()->second, 1.f);

						XMStoreFloat4((_float4*)&Desc.TransformMatrix.m[CTransform::STATE_POSITION][0], vDist);

						// 충돌체 생성
						Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

						// 해당 오브젝트가 맵인지 아닌지 확인
						// _tcscmp함수는 같은 문자열일 경우 0을 반환
						if (!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_SnowMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_FireManMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_StartingTown")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Balseph_Enterance")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Balseph")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Boss_Balseph_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Boss_FireAvatar_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_01_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_05_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_07_NaviMap"))
							)
						{
							Desc.m_bMap = true;
						}

						// 맵 오브젝트가 생성된다면
						if (Desc.m_bMap == true) {
							// 생성된적이 없다면 생성한다.
							if (nullptr == m_pPickingMeshObject) {
								m_pPickingMeshObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
								m_PlacedMapObjectList.push_back(m_pPickingMeshObject);
							}
						}
						else {
							// 맵 오브젝트 생성
							CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
							m_PlacedMapObjectList.push_back(pMapObject);

							// 생성한 오브젝트 선택
							m_pSelectedFeatureObject = pMapObject;
							//Update_Transform_Position();
						}

						// 배치된 오브젝트 리스트 업데이트
						UpdateFeatureList();
					}

				}

				else {
					// 정점 4개
					_float3* RectPos = m_pPicking_Rect->Get_RectPos();

					// 피킹한다.
					_float3 TriPos[3];
					TriPos[0] = RectPos[0];
					TriPos[1] = RectPos[1];
					TriPos[2] = RectPos[2];

					_vector vDist;
					_matrix WorldMatrix = XMMatrixIdentity();

					if (pGameInstance->Picking(TriPos, WorldMatrix, &vDist)) {
						CMapObject::MAPOBJDesc Desc;
						Desc.pPrototype_ObjectName = m_pPrototypeTag_SelectedMesh;

						// Desc.TransformMatrix값을 항등행렬로 변환
						XMStoreFloat4x4(&Desc.TransformMatrix, XMMatrixIdentity());

						// 피킹된 위치로 행렬의 위치값 변경
						XMStoreFloat4((_float4*)&Desc.TransformMatrix.m[CTransform::STATE_POSITION][0], vDist);

						// 충돌체 생성
						Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

						// 해당 오브젝트가 맵인지 아닌지 확인
						// _tcscmp함수는 같은 문자열일 경우 0을 반환
						if (!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_SnowMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_FireManMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_StartingTown")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Balseph_Enterance")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Balseph")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Boss_Balseph_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Boss_FireAvatar_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_01_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_05_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_07_NaviMap"))
							)
						{
							Desc.m_bMap = true;
						}

						// 맵 오브젝트가 생성된다면
						if (Desc.m_bMap == true) {
							// 생성된적이 없다면 생성한다.
							if (nullptr == m_pPickingMeshObject) {
								m_pPickingMeshObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
								m_PlacedMapObjectList.push_back(m_pPickingMeshObject);
							}
						}
						else {
							// 맵 오브젝트 생성
							CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
							m_PlacedMapObjectList.push_back(pMapObject);

							// 생성한 오브젝트 선택
							m_pSelectedFeatureObject = pMapObject;
						}

						// 배치된 오브젝트 리스트 업데이트
						UpdateFeatureList();
					}

					TriPos[0] = RectPos[0];
					TriPos[1] = RectPos[2];
					TriPos[2] = RectPos[3];

					if (pGameInstance->Picking(TriPos, WorldMatrix, &vDist)) {
						CMapObject::MAPOBJDesc Desc;
						Desc.pPrototype_ObjectName = m_pPrototypeTag_SelectedMesh;

						// Desc.TransformMatrix값을 항등행렬로 변환
						XMStoreFloat4x4(&Desc.TransformMatrix, XMMatrixIdentity());

						// 피킹된 위치로 행렬의 위치값 변경
						XMStoreFloat4((_float4*)&Desc.TransformMatrix.m[CTransform::STATE_POSITION][0], vDist);

						// 충돌체 생성
						Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

						// 해당 오브젝트가 맵인지 아닌지 확인
						// _tcscmp함수는 같은 문자열일 경우 0을 반환
						if (!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_SnowMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_FireManMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_StartingTown")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Balseph_Enterance")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Balseph")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Boss_Balseph_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Boss_FireAvatar_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_01_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_05_NaviMap")) ||
							!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_Cave_07_NaviMap"))
							)
						{
							Desc.m_bMap = true;
						}

						// 맵 오브젝트가 생성된다면
						if (Desc.m_bMap == true) {
							// 생성된적이 없다면 생성한다.
							if (nullptr == m_pPickingMeshObject) {
								m_pPickingMeshObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
								m_PlacedMapObjectList.push_back(m_pPickingMeshObject);
							}
						}
						else {
							// 맵 오브젝트 생성
							CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
							m_PlacedMapObjectList.push_back(pMapObject);

							// 생성한 오브젝트 선택
							m_pSelectedFeatureObject = pMapObject;
						}

						// 배치된 오브젝트 리스트 업데이트
						UpdateFeatureList();
					}
				}
			}

			// 마우스 오른쪽 버튼을 눌렀을 경우
			if (pGameInstance->Button_Down(CInput_Device::DIMB_RBUTTON)) {
				// 마우스에서 가장 가까운 위치에 있는 오브젝트가 선택된다.

				// 메쉬를 선택하는 것을 true로 한다.
				m_bSelectMesh = true;

				CPicking_Manager::RAYDESC Desc = pGameInstance->Get_vRayDesc();

				// 배치된 오브젝트들을 가져와 마우스의 위치에서 가장 가까운 오브젝트를 선택한다. 
				_float fDist = 0.f;
				map<_float, CMapObject*> CollisionObjects;

				// 모든 충돌체를 돌아다니며 충돌된 오브젝트들을 CollisionObjects에 넣는다.
				// 이때, map을 사용하여 key 값을 거리를 줌으로서 자동으로 가장 가까운 값이 앞에 오도록 한다.
				for (auto& Object : m_PlacedMapObjectList) {
					if (Object->Get_Collider()->Collision_Lay(XMLoadFloat3(&Desc.vRayPos), XMLoadFloat3(&Desc.vRay), fDist)) {
						CollisionObjects.emplace(fDist, Object);
					}
				}

				// 충돌된 오브젝트가 존재한다면
				if (!CollisionObjects.empty()) {
					// 선택중인 오브젝트에 가장 가까운 오브젝트를 넣는다.
					m_pSelectedFeatureObject = CollisionObjects.begin()->second;
					auto& iter = find(m_PlacedMapObjectList.begin(), m_PlacedMapObjectList.end(), m_pSelectedFeatureObject);
					m_iCurrent_Index = (int)distance(m_PlacedMapObjectList.begin(), iter);

					// 선택된 오브젝트의 Transform 값을 가지고 위치 및 크기 값을 채운다.
					Update_SelectedFeature();
				}
			}


		}





		if (nullptr != m_pSelectedFeatureObject) {
			_vector vPos = dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
			_float3 fPos;
			XMStoreFloat3(&fPos, vPos);


			// -x
			if (pGameInstance->Key_Pressing(DIK_NUMPAD4)) {
				vPos -= dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_RIGHT) * TimeDelta * m_fMeshSpeed;
				//fPos.x -= TimeDelta * fSpeed;
				Update_Transform_Position();
			}
			// +x
			if (pGameInstance->Key_Pressing(DIK_NUMPAD6)) {
				vPos += dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_RIGHT) * TimeDelta * m_fMeshSpeed;
				//fPos.x += TimeDelta * fSpeed;
				Update_Transform_Position();
			}
			// +z
			if (pGameInstance->Key_Pressing(DIK_NUMPAD8)) {
				_vector vObjPos = dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
				_float3 fObjPos;
				XMStoreFloat3(&fObjPos, vObjPos);


				vPos += dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_LOOK) * TimeDelta * m_fMeshSpeed;
				fObjPos.x = XMVectorGetX(vPos);
				fObjPos.z = XMVectorGetZ(vPos);

				vPos = XMLoadFloat3(&fObjPos);
				//fPos.z += TimeDelta * fSpeed;
				Update_Transform_Position();
			}
			// -z
			if (pGameInstance->Key_Pressing(DIK_NUMPAD2)) {
				_vector vObjPos = dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
				_float3 fObjPos;
				XMStoreFloat3(&fObjPos, vObjPos);


				vPos -= dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_LOOK) * TimeDelta * m_fMeshSpeed;
				fObjPos.x = XMVectorGetX(vPos);
				fObjPos.z = XMVectorGetZ(vPos);

				vPos = XMLoadFloat3(&fObjPos);
				//fPos.z -= TimeDelta * fSpeed;
				Update_Transform_Position();
			}
			// +y
			if (pGameInstance->Key_Pressing(DIK_NUMPAD7)) {
				vPos += dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_UP) * TimeDelta * m_fMeshSpeed;
				//fPos.y += TimeDelta * fSpeed;
				Update_Transform_Position();
			}
			// -y
			if (pGameInstance->Key_Pressing(DIK_NUMPAD9)) {
				vPos -= dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_UP) * TimeDelta * m_fMeshSpeed;
				//fPos.y -= TimeDelta * fSpeed;
				Update_Transform_Position();
			}

			dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vPos, 1.f));
		}

	}


	if (m_eUI == UI_LEVEL::UL_Navigation) {

		// 키보드 e버튼을 누르고 있다면
		if (pGameInstance->Key_Pressing(DIK_E)) {
			// 왼쪽 마우스 버튼을 눌렀을 경우
			if (pGameInstance->Button_Down(CInput_Device::DIMB_LBUTTON)) {
				if (nullptr != m_pPickingMeshObject) {
					vector<class CMeshContainer*>& vMeshContainer = dynamic_cast<CModel*>(m_pPickingMeshObject->Get_Component(TEXT("Com_Model")))->Get_MeshContainer();

					_vector vDist;
					_vector vPos;
					_bool isPicking = false;
					map<_float, _vector> Dists;
					Dists.clear();

					for (auto& Container : vMeshContainer) {
						VTXNONANIMMODEL* pVertices = Container->Get_NonAnimVertices();
						FACELISTINDICES32* pIndicies = Container->Get_Indices();
						_uint iNumPrimitive = Container->Get_PrimitiveCnt();
						_matrix matWorld = XMMatrixIdentity();

						for (int i = 0; i < iNumPrimitive; ++i) {
							if (pGameInstance->Picking(
								XMLoadFloat3(&pVertices[pIndicies[i]._0].vPosition),
								XMLoadFloat3(&pVertices[pIndicies[i]._1].vPosition),
								XMLoadFloat3(&pVertices[pIndicies[i]._2].vPosition),
								matWorld,
								&vPos,		// 위치
								&vDist		// 거리
							)) {
								_float fDist;
								XMStoreFloat(&fDist, vDist);

								Dists.emplace(fDist, vPos);
							}
						}
					}

					if (!Dists.empty()) {
						isPicking = true;
					}

					// 피킹이 되었다면
					if (isPicking) {
						// 해당 위치에 dot을 생성한다.
						CDot::DotDesc Desc;

						// 피킹된 위치로 행렬의 위치값 변경
						vDist = XMVectorSetW(Dists.begin()->second, 1.f);
						XMStoreFloat3(&Desc.fPos, vDist);

						m_pDot = dynamic_cast<CDot*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_Dot, TEXT("Prototype_GameObject_Dot"), &Desc));

						m_vDot.push_back(m_pDot);
					}

				}
			}

			// 마우스 오른쪽 버튼을 눌렀을 경우
			if (pGameInstance->Button_Down(CInput_Device::DIMB_RBUTTON)) {

				CPicking_Manager::RAYDESC Desc = pGameInstance->Get_vRayDesc();
				_float fDist;

				auto& iter = find_if(m_vDot.begin(), m_vDot.end(), [&](CDot* pDot) {
					if (dynamic_cast<CCollider*>(pDot->Get_Component(TEXT("Com_Sphere")))->Collision_Lay(XMLoadFloat3(&Desc.vRayPos), XMLoadFloat3(&Desc.vRay), fDist))
						return true;

					return false;
				});


				if (iter != m_vDot.end())
					m_pDot = *iter;

				if (nullptr != m_pDot) {
					auto& iter = find_if(m_vDot.begin(), m_vDot.end(), [&](CDot* pDot) {
						if (m_pDot == pDot)
							return true;

						return false;
					});

					if (iter != m_vDot.end()) {
						(*iter)->Set_Dead(true);
						m_vDot.erase(iter);
					}

					m_pDot = nullptr;

					m_SelectDotslist.clear();
				}
			}
		}

		if (pGameInstance->Key_Pressing(DIK_R)) {

			if (pGameInstance->Key_Pressing(DIK_NUMPADPLUS)) {
				if (nullptr != m_pDot) {
					_vector vPos = dynamic_cast<CTransform*>(m_pDot->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
					_float3 fPos;

					XMStoreFloat3(&fPos, vPos);
					fPos.y += TimeDelta * 100.f;

					vPos = XMLoadFloat3(&fPos);

					dynamic_cast<CTransform*>(m_pDot->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, vPos);
				}
			}
			if (pGameInstance->Key_Pressing(DIK_NUMPADMINUS)) {
				if (nullptr != m_pDot) {
					_vector vPos = dynamic_cast<CTransform*>(m_pDot->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
					_float3 fPos;

					XMStoreFloat3(&fPos, vPos);
					fPos.y -= TimeDelta * 100.f;

					vPos = XMLoadFloat3(&fPos);

					dynamic_cast<CTransform*>(m_pDot->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, vPos);
				}
			}




			if (pGameInstance->Key_Down(DIK_DELETE)) {
				if (nullptr != m_pDot) {
					auto& iter = find_if(m_vDot.begin(), m_vDot.end(), [&](CDot* pDot) {
						if (m_pDot == pDot)
							return true;

						return false;
					});

					if (iter != m_vDot.end()) {
						(*iter)->Set_Dead(true);
						m_vDot.erase(iter);
					}

					m_pDot = nullptr;
				}
			}

		}


		// T를 누르고 있을 경우
		if (pGameInstance->Key_Pressing(DIK_T)) {
			if (pGameInstance->Button_Down(CInput_Device::DIMB_LBUTTON)) {
				CPicking_Manager::RAYDESC Desc = pGameInstance->Get_vRayDesc();
				_float fDist;

				auto& iter = find_if(m_vDot.begin(), m_vDot.end(), [&](CDot* pDot) {
					if (dynamic_cast<CCollider*>(pDot->Get_Component(TEXT("Com_Sphere")))->Collision_Lay(XMLoadFloat3(&Desc.vRayPos), XMLoadFloat3(&Desc.vRay), fDist))
						return true;

					return false;
				});

				if (iter != m_vDot.end()) {
					Add_SelectDot(*iter);
				}
			}
			else if (pGameInstance->Button_Down(CInput_Device::DIMB_RBUTTON)) {
				CPicking_Manager::RAYDESC Desc = pGameInstance->Get_vRayDesc();
				_float fDist;

				auto& iter = find_if(m_vCell.begin(), m_vCell.end(), [&](CCell* pCell) {
					_vector vDist;
					if (pGameInstance->Picking(pCell->Get_Points(), XMMatrixIdentity(), &vDist))
						return true;

					return false;
				});

				if (iter != m_vCell.end()) {
					m_vCell.erase(iter);
				}
			}
		}

	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CImGUI_Manager::ImGUI_SeongUk(void)
{
	if (m_bImguiEnable) {
		ImGUI_SettingCamera();

		// 스테이지가 눌려져 있다면 출력
		if (m_eCurStageLevel != Stage_END)
			ImGUI_SelectUI();
		else {
			ImGUI_SelectStage();
		}

		switch (m_eUI)
		{
		case Client::CImGUI_Manager::UL_CreateFire:
			ImGUI_SettingFire();
			break;

		case Client::CImGUI_Manager::UL_SetObject:
			ImGUI_SettingObject();
			break;

		case Client::CImGUI_Manager::UL_Navigation:
			ImGUI_NeviCell();
			Render_Cells();
			break;

		case Client::CImGUI_Manager::UL_Lights:
			ImGUI_Lights();
			break;

		case Client::CImGUI_Manager::UL_Sky:
			ImGUI_Sky();
			break;

		case Client::CImGUI_Manager::UL_PostProcessing:
			ImGUI_PostProcessing();
			break;

		case Client::CImGUI_Manager::UL_END:
			break;

		default:
			break;
		}
	}

	return S_OK;
}

void CImGUI_Manager::ImGUI_SelectUI(void)
{
	// 창의 사이즈및 위치 조정
	ImGui::SetNextWindowSize(ImVec2(620.f, 60.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Select Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 0));

	// 버튼의 가로 길이를 설정
	// 하이트 맵 UI로 들어가는 버튼
	if (ImGui::Button("Fire", ImVec2(80.f, 20.f))) {
		m_eUI = UI_LEVEL::UL_CreateFire;
	}

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();

	if (ImGui::Button("Object", ImVec2(80.f, 20.f))) {
		m_eUI = UI_LEVEL::UL_SetObject;
	}


	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();


	if (ImGui::Button("Navigation", ImVec2(80.f, 20.f))) {
		m_eUI = UI_LEVEL::UL_Navigation;
	}

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();

	if (ImGui::Button("Lights", ImVec2(80.f, 20.f))) {
		m_eUI = UI_LEVEL::UL_Lights;
	}

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();

	if (ImGui::Button("Sky", ImVec2(80.f, 20.f))) {
		m_eUI = UI_LEVEL::UL_Sky;
	}

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();

	if (ImGui::Button("PostProcessing", ImVec2(80.f, 20.f))) {
		m_eUI = UI_LEVEL::UL_PostProcessing;
	}
	
	ImGui::End();
}

void CImGUI_Manager::ImGUI_SelectStage(void)
{
	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(500.f, 500.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Stage Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 좌측으로 고정
	ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 0));

	// ListBox 선택 : Stage 01 ~ 05
	static const char* Stage[]{ 
		"Stage 01. GrandYork Town", 
		"Stage 02. Tower_of_the_Wind_Spirit_Top", 
		"Stage 03. Boss Balseph", 
		"Stage 04. Fire Avatar",
		"Stage 05. Boss_Lord_Vholran_Igniseri_Phase01",
		"Stage 06. Boss_Lord_Vholran_Igniseri_Phase02",
		"Stage 07. Boss_Lord_Vholran_Igniseri_Enterance",
		"Stage 08. GrandYork_Battle_02",
		"Stage 09. GrandYork_Battle_04",
		"Stage 10. Tower_of_the_Wind_Spirit_Inside"
	};
	static int iSelected_Stage = 0;

	ImGui::PushItemWidth(450);
	ImGui::ListBox("Select StageInfo", &iSelected_Stage, Stage, IM_ARRAYSIZE(Stage), 20);

	// 버튼을 누르면
	if (ImGui::Button("Select")) {
		// 선택된 스테이지의 정보가 로드된다.
		m_bSelect_Stage = true;

		// 현재 스테이지 정보 설정
		switch (iSelected_Stage)
		{
		case 0:
			m_eCurStageLevel = Stage_01;
			break;
		case 1:
			m_eCurStageLevel = Stage_02;
			break;
		case 2:
			m_eCurStageLevel = Stage_03;
			break;
		case 3:
			m_eCurStageLevel = Stage_04;
			break;
		case 4:
			m_eCurStageLevel = Stage_05;
			break;
		case 5:
			m_eCurStageLevel = Stage_06;
			break;
		case 6:
			m_eCurStageLevel = Stage_07;
			break;
		case 7:
			m_eCurStageLevel = Stage_08;
			break;
		case 8:
			m_eCurStageLevel = Stage_09;
			break;
		case 9:
			m_eCurStageLevel = Stage_10;
			break;
		default:
			break;
		}
	}

	ImGui::End();
}

void CImGUI_Manager::ImGUI_SettingCamera(void)
{
	if (nullptr == m_pCurrCamera)
		return;

	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(300.f, 250.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Camera Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 좌측으로 고정
	//ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 0));
	ImGui::SetWindowPos(ImVec2(0.f, 0.f));

	// 카메라의 높이, XZ평면에서의 좌표 출력
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")));

	_vector vCameraPos = pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 fCameraPos;
	XMStoreFloat3(&fCameraPos, vCameraPos);


	// 카메라의 현재 위치 출력
	ImGui::Text("x : %.3f, y : %.3f, z : %.3f", fCameraPos.x, fCameraPos.y, fCameraPos.z);

	// 칸의 가로 길이를 70으로 조정
	ImGui::PushItemWidth(50.f);
	// =================== 카메라 X ===================
	// 입력된 문자열을 임시 문자열에 저장
	static std::string str_CameraX;
	char szCameraX[20]{};
	strncpy_s(szCameraX, str_CameraX.c_str(), sizeof(szCameraX) - 1);
	ImGui::InputText("x", szCameraX, sizeof(szCameraX));
	str_CameraX = szCameraX;

	// =================== 카메라 Height(y) ===================
	ImGui::SameLine(75.f);
	// 입력된 문자열을 임시 문자열에 저장
	static std::string str_CameraHeight;
	char szCameraHeight[20]{};
	strncpy_s(szCameraHeight, str_CameraHeight.c_str(), sizeof(szCameraHeight) - 1);
	ImGui::InputText("y", szCameraHeight, sizeof(szCameraHeight));
	str_CameraHeight = szCameraHeight;

	// =================== 카메라 Z ===================
	ImGui::SameLine(145.f);
	// 입력된 문자열을 임시 문자열에 저장
	static std::string str_CameraZ;
	char szCameraZ[20]{};
	strncpy_s(szCameraZ, str_CameraZ.c_str(), sizeof(szCameraZ) - 1);
	ImGui::InputText("z", szCameraZ, sizeof(szCameraZ));
	str_CameraZ = szCameraZ;

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();

	if (ImGui::Button("Update")) {
		// 저장된 문자열을 숫자로 변환
		_double dX = 0.0;
		if (!str_CameraX.empty())
			dX = stod(str_CameraX);

		// 저장된 문자열을 숫자로 변환
		_double dZ = 0.0;
		if (!str_CameraZ.empty())
			dZ = stod(str_CameraZ);

		// 저장된 문자열을 숫자로 변환
		_double dHeight = 0.0;
		if (!str_CameraHeight.empty())
			dHeight = stod(str_CameraHeight);

		pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet((_float)dX, (_float)dHeight, (_float)dZ, 1.f));
	}

	// 다음줄로 이동
	ImGui::NextColumn();
	ImGui::Text(" ");

	// 카메라의 이동 스피드 세팅 및 출력
	ImGui::PushItemWidth(200.f);
	_float f[1] = { m_fCameraSpeed };
	ImGui::SliderFloat("Speed", f, 0.f, 2000.f);

	m_fCameraSpeed = f[0];


	pTransform->Set_Speed(m_fCameraSpeed);


	// 다음줄로 이동
	ImGui::NextColumn();
	ImGui::Text(" ");

	// 카메라 회전각도 출력
	static _float fRotation = 0.f;

	ImGui::PushItemWidth(200.f);
	ImGui::SliderFloat("Rotation", &fRotation, 0.f, 359.999f);
	//m_pCurrCamera->Rotation(fRotation);


	// 다음줄로 이동
	ImGui::NextColumn();
	ImGui::Text(" ");

	// 카메라의 각도 출력
	//ImGui::Text("Curr Degree : %.3f", m_pCurrCamera->Get_Degree());
	static _float fDegree = 60.f;

	ImGui::PushItemWidth(200.f);
	ImGui::SliderFloat("Degree", &fDegree, 0.f, 89.999f);
	//m_pCurrCamera->Adjust_Angle(fDegree);



	// 메쉬 이동속도
	ImGui::NextColumn();
	ImGui::SliderFloat("MeshSpeed", &m_fMeshSpeed, 0.f, 1000.f);





	// 해당 박스의 최대 사이즈 저장
	m_vCameraMax.x = ImGui::GetItemRectMax().x;
	m_vCameraMax.y = ImGui::GetItemRectMax().y;

	ImGui::End();
}

void CImGUI_Manager::ImGUI_SettingPickingTerrain(void)
{
	if (nullptr == m_pCurrTerrain)
		return;

	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(300.f, 75.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Terrain Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 좌측으로 고정
	//ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, m_vCameraMax.y + 20));
	ImGui::SetWindowPos(ImVec2(0.f, m_vCameraMax.y + 20));

	// 현재 Terrain Height 출력
	//ImGui::Text("Curr Height : %.3f", m_pCurrTerrain->Get_Height());

	// 칸의 가로 길이를 70으로 조정
	ImGui::PushItemWidth(70.f);
	// =================== 지형 높이 ===================
	// 입력된 문자열을 임시 문자열에 저장
	static std::string str_TerrainHeight;
	char sz_TerrainHeight[20]{};
	strncpy_s(sz_TerrainHeight, str_TerrainHeight.c_str(), sizeof(sz_TerrainHeight) - 1);
	ImGui::InputText("Height", sz_TerrainHeight, sizeof(sz_TerrainHeight));
	str_TerrainHeight = sz_TerrainHeight;

	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();
	if (ImGui::Button("Update")) {
		// 저장된 문자열을 숫자로 변환
		_float fHeight = 0.0;
		if (!str_TerrainHeight.empty())
			fHeight = stof(str_TerrainHeight);

		//m_pCurrTerrain->Change_Height(fHeight);
	}

	// 해당 박스의 최대 사이즈 저장
	m_vWireFrameMax.x = ImGui::GetItemRectMax().x;
	m_vWireFrameMax.y = ImGui::GetItemRectMax().y;

	ImGui::End();
}

void CImGUI_Manager::ImGUI_SettingFire(void)
{
	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(400.f, 900.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Fire Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 우측으로 고정
	ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 60.f));

	// 기준이 되는 오브젝트 1개만 생성
	Create_StandardObject();

	// 불 오브젝트 생성
	Create_FireObject();

	// 생성된 불 오브젝트 정보 리스트로 출력
	Show_FireObjectList();

	// 선택한 불 정보 컨트롤
	Control_Select_FireObject();

	// 불 정보 세이브 로드
	ImGUI_SettingMesh_Fire_SaveLoad();

	ImGui::End();
}

void CImGUI_Manager::ImGUI_SettingObject(void)
{
	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(550.f, 900.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Mesh Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 우측으로 고정
	ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 60.f));

	// Mesh 관련 툴 출력
	ImGUI_SettingMesh_Mesh();

	// 다음줄로 이동
	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");

	// Feature(배치된 메쉬) 관련 툴 출력
	ImGUI_SettingMesh_Feature();

	// 다음줄로 이동
	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");

	// Transform(Select Feature 움직임) 관련 툴 출력
	ImGUI_SettingMesh_Transform();

	// 다음줄로 이동
	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");

	// Save / Load 관련 툴 출력
	ImGUI_SettingMesh_SaveLoad();

	// 다음줄로 이동
	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");

	// PickingMesh Object
	ImGUI_SettingMesh_PickingMesh();

	ImGui::End();
}

void CImGUI_Manager::ImGUI_NeviCell(void)
{
	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(350.f, 500.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Navi Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 좌측으로 고정
	ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 60.f));

	if (nullptr != m_pPicking_Rect) {
		_vector vPos = dynamic_cast<CTransform*>(m_pPicking_Rect->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

		static _float fPos;
		fPos = XMVectorGetY(vPos);

		ImGui::Text("Picking Rect Height : %f", fPos);

		ImGui::SliderFloat("Rect Height", &fPos, 0.f, 200.f);

		m_pPicking_Rect->Change_Height(fPos);
	}

	ImGui::NewLine();
	ImGui::Text("Dot list - loaded [ %d ] Dots", m_vDot.size());
	if (ImGui::Button("Save Dot")) {
		switch (m_eCurStageLevel)
		{
		case Client::CImGUI_Manager::Stage_01:
			Save_DotInfo("../Bin/Resources/Dat/Stage01/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_02:
			Save_DotInfo("../Bin/Resources/Dat/Stage02/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_03:
			Save_DotInfo("../Bin/Resources/Dat/Stage03/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_04:
			Save_DotInfo("../Bin/Resources/Dat/Stage04/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_05:
			Save_DotInfo("../Bin/Resources/Dat/Stage05/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_06:
			Save_DotInfo("../Bin/Resources/Dat/Stage06/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_07:
			Save_DotInfo("../Bin/Resources/Dat/Stage07/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_08:
			Save_DotInfo("../Bin/Resources/Dat/Stage08/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_09:
			Save_DotInfo("../Bin/Resources/Dat/Stage09/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_10:
			Save_DotInfo("../Bin/Resources/Dat/Stage10/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_END:
			break;
		default:
			break;
		}
	}


	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();

	if (ImGui::Button("Load Dot")) {
		switch (m_eCurStageLevel)
		{
		case Client::CImGUI_Manager::Stage_01:
			Load_DotInfo("../Bin/Resources/Dat/Stage01/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_02:
			Load_DotInfo("../Bin/Resources/Dat/Stage02/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_03:
			Load_DotInfo("../Bin/Resources/Dat/Stage03/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_04:
			Load_DotInfo("../Bin/Resources/Dat/Stage04/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_05:
			Load_DotInfo("../Bin/Resources/Dat/Stage05/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_06:
			Load_DotInfo("../Bin/Resources/Dat/Stage06/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_07:
			Load_DotInfo("../Bin/Resources/Dat/Stage07/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_08:
			Load_DotInfo("../Bin/Resources/Dat/Stage08/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_09:
			Load_DotInfo("../Bin/Resources/Dat/Stage09/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_10:
			Load_DotInfo("../Bin/Resources/Dat/Stage10/", "DotList.dat");
			break;
		case Client::CImGUI_Manager::Stage_END:
			break;
		default:
			break;
		}
	}

	ImGui::Text("Picked Dot Cnt : %d", m_SelectDotslist.size());
	int i = 1;
	for (auto& Dot : m_SelectDotslist) {
		_vector vPos = dynamic_cast<CTransform*>(Dot->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
		_float3 fPos;
		XMStoreFloat3(&fPos, vPos);

		ImGui::Text("%d.(%.3f, %.3f, %.3f)", i, fPos.x, fPos.y, fPos.z);
		++i;
	}


	if (ImGui::Button("Create Terrain Cells")) {

		if (m_SelectDotslist.size() == 3) {
			_vector pPoints[3];

			int i = 0;
			for (auto& Dot : m_SelectDotslist) {
				pPoints[i++] = dynamic_cast<CTransform*>(Dot->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
			}

			// 셀 생성
			m_pCell = CCell::Create(m_pDevice, m_pDeviceContext, pPoints[0], pPoints[1], pPoints[2], 0);
			m_vCell.push_back(m_pCell);

			m_SelectDotslist.clear();
		}
	}

	ImGui::Text("Generated Cell Cnt : %d", m_vCell.size());

	// 선택된 셀 정보
	ImGui::NewLine();
	ImGui::Text("Select Cell");


	ImGui::NewLine();


	// 네비 메쉬 저장
	if (ImGui::Button("Delete Last Cell")) {
		m_vCell.pop_back();
	}



	ImGui::NewLine();
	ImGui::NewLine();

	// 셀 저장
	if (ImGui::Button("Save Cells")) {

		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			SaveCellList("../Bin/Resources/Dat/Stage01/", "CellList.dat");
			break;
		case 1:
			SaveCellList("../Bin/Resources/Dat/Stage02/", "CellList.dat");
			break;
		case 2:
			SaveCellList("../Bin/Resources/Dat/Stage03/", "CellList.dat");
			break;
		case 3:
			SaveCellList("../Bin/Resources/Dat/Stage04/", "CellList.dat");
			break;
		case 4:
			SaveCellList("../Bin/Resources/Dat/Stage05/", "CellList.dat");
			break;
		case 5:
			SaveCellList("../Bin/Resources/Dat/Stage06/", "CellList.dat");
			break;
		case 6:
			SaveCellList("../Bin/Resources/Dat/Stage07/", "CellList.dat");
			break;
		case 7:
			SaveCellList("../Bin/Resources/Dat/Stage08/", "CellList.dat");
			break;
		case 8:
			SaveCellList("../Bin/Resources/Dat/Stage09/", "CellList.dat");
			break;
		case 9:
			SaveCellList("../Bin/Resources/Dat/Stage10/", "CellList.dat");
			break;
		default:
			break;
		}
	}

	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	// 셀 불러오기
	if (ImGui::Button("Load Cells")) {
		// 현재 스테이지에 따라 다른 경로로 불러오기
		switch (m_eCurStageLevel)
		{
		case 0:
			LoadCellList("../Bin/Resources/Dat/Stage01/", "CellList.dat");
			break;
		case 1:
			LoadCellList("../Bin/Resources/Dat/Stage02/", "CellList.dat");
			break;
		case 2:
			LoadCellList("../Bin/Resources/Dat/Stage03/", "CellList.dat");
			break;
		case 3:
			LoadCellList("../Bin/Resources/Dat/Stage04/", "CellList.dat");
			break;
		case 4:
			LoadCellList("../Bin/Resources/Dat/Stage05/", "CellList.dat");
			break;
		case 5:
			LoadCellList("../Bin/Resources/Dat/Stage06/", "CellList.dat");
			break;
		case 6:
			LoadCellList("../Bin/Resources/Dat/Stage07/", "CellList.dat");
			break;
		case 7:
			LoadCellList("../Bin/Resources/Dat/Stage08/", "CellList.dat");
			break;
		case 8:
			LoadCellList("../Bin/Resources/Dat/Stage09/", "CellList.dat");
			break;
		case 9:
			LoadCellList("../Bin/Resources/Dat/Stage10/", "CellList.dat");
			break;
		default:
			break;
		}
	}

	// 네비 메쉬 저장
	if (ImGui::Button("Save NaviMesh")) {
		// 현재 스테이지에 따라 다른 경로로 불러오기
		switch (m_eCurStageLevel)
		{
		case 0:
			SaveNaviMesh("../Bin/Resources/Dat/Stage01/", "Navi_Mesh.dat");
			break;
		case 1:
			SaveNaviMesh("../Bin/Resources/Dat/Stage02/", "Navi_Mesh.dat");
			break;
		case 2:
			SaveNaviMesh("../Bin/Resources/Dat/Stage03/", "Navi_Mesh.dat");
			break;
		case 3:
			SaveNaviMesh("../Bin/Resources/Dat/Stage04/", "Navi_Mesh.dat");
			break;
		case 4:
			SaveNaviMesh("../Bin/Resources/Dat/Stage05/", "Navi_Mesh.dat");
			break;
		case 5:
			SaveNaviMesh("../Bin/Resources/Dat/Stage06/", "Navi_Mesh.dat");
			break;
		case 6:
			SaveNaviMesh("../Bin/Resources/Dat/Stage07/", "Navi_Mesh.dat");
			break;
		case 7:
			SaveNaviMesh("../Bin/Resources/Dat/Stage08/", "Navi_Mesh.dat");
			break;
		case 8:
			SaveNaviMesh("../Bin/Resources/Dat/Stage09/", "Navi_Mesh.dat");
			break;
		case 9:
			SaveNaviMesh("../Bin/Resources/Dat/Stage10/", "Navi_Mesh.dat");
			break;
		default:
			break;
		}
	}




	// 네비 메쉬 저장
	if (ImGui::Button("Load Picking Object Cells")) {
		// 현재 피킹 오브젝트로 선택한 오브젝트의 이름이 특정 태그라면 해당 오브젝트의 모든 정점정보를 dot으로 바꾼다.
		if (nullptr != m_pPickingMeshObject) {

			CTransform* pTransform = dynamic_cast<CTransform*>(m_pPickingMeshObject->Get_Component(TEXT("Com_Transform")));
			

			vector<class CMeshContainer*>& vMeshContainer = dynamic_cast<CModel*>(m_pPickingMeshObject->Get_Component(TEXT("Com_Model")))->Get_MeshContainer();

			_vector vDist;
			_vector vPos;
			_bool isPicking = false;
			map<_float, _vector> Dists;
			Dists.clear();

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			CLayer* pLayer_Dot = pGameInstance->Find_Layer(LEVEL_MapTool, LAYER_MAPTOOL_Dot);
			list<CGameObject*> DotList;

			if (nullptr != pLayer_Dot)
				DotList = pLayer_Dot->Get_ObjectList();


			for (auto& Container : vMeshContainer) {
				VTXNONANIMMODEL* pVertices = Container->Get_NonAnimVertices();
				_uint iVerticesCnt = Container->Get_VerticesCnt();

				// 오브젝트의 정점 정보를 가져온다.
				CDot::DotDesc Desc;
				for (_uint i = 0; i < iVerticesCnt; ++i) {
					Desc.fPos = pVertices[i].vPosition;

					// 항등행렬 설정
					_matrix matDot = XMMatrixIdentity();
					_float4x4 matDotFloat4x4;
					XMStoreFloat4x4(&matDotFloat4x4, matDot);

					// 위치값 넣기
					memcpy(matDotFloat4x4.m[3], &Desc.fPos, sizeof(_float3));

					// _matrix로 변환
					matDot = XMLoadFloat4x4(&matDotFloat4x4);

					_matrix matWorld = pTransform->Get_WorldMatrix();

					matWorld = XMMatrixMultiply(matDot, matWorld);

					XMStoreFloat4x4(&matDotFloat4x4, matWorld);

					
					memcpy(&Desc.fPos, matDotFloat4x4.m[3], sizeof(_float3));

					m_pDot = dynamic_cast<CDot*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_Dot, TEXT("Prototype_GameObject_Dot"), &Desc));
					m_vDot.push_back(m_pDot);
				}
			}


			// cell 생성
			_uint iIndexCnt = 0;
			_uint iOverlap = 0;
			for (auto& Container : vMeshContainer) {
				VTXNONANIMMODEL* pVertices = Container->Get_NonAnimVertices();
				FACELISTINDICES32* pIndicies = Container->Get_Indices();
				_uint iVerticesCnt = Container->Get_VerticesCnt();
				_uint iIndiciesCnt = Container->Get_PrimitiveCnt();
			
				_float3 fPos[3];
				for (_uint i = 0; i < iIndiciesCnt; ++i) {
					fPos[0] = pVertices[pIndicies[i]._0].vPosition;
					fPos[1] = pVertices[pIndicies[i]._1].vPosition;
					fPos[2] = pVertices[pIndicies[i]._2].vPosition;

					if (!CheckCellCW(fPos)) {
						// [1], [2]번 인덱스의 값을 서로 바꾼다.
						_float3 fTempPos = fPos[1];
						fPos[1] = fPos[2];
						fPos[2] = fTempPos;
					}


					if (Check_SameCell(fPos)) {
						m_vCell.push_back(CCell::Create(m_pDevice, m_pDeviceContext, fPos, iIndexCnt++));
					}
					else
						++iOverlap;
				}
			}




			RELEASE_INSTANCE(CGameInstance);
		}
	}

	ImGui::End();
}

void CImGUI_Manager::ImGUI_Lights(void)
{
	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(500.f, 1000.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Light Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 좌측으로 고정
	ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 60.f));

	// 광원 정보 업데이트
	Update_Lights();

	// 현재 카메라의 좌표 출력
	Show_CamPosition();

	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();

	// 방향광 정보 출력
	Show_DirectionLight();

	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();

	// 점광원 정보 출력
	Show_PointLight();

	ImGui::NextColumn();
	ImGui::Text(" ");

	// 점광원 생성/삭제
	CreateDelete_PointLight();

	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");

	// 선택한 점광원 정보 조정
	Control_Select_PointLight();

	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");

	// 광원정보 저장/불러오기
	ImGUI_Setting_Light_SaveLoad();

	// 점광원의 충돌체 오브젝트 생성 -> 충돌체를 그려 점광원의 위치를 알 수 있게한다.
	Create_PointLight_LightObject();
	Delete_PointLight_LightObject();

	// 광원 정보 업데이트
	Update_Lights();

	ImGui::End();
}

void CImGUI_Manager::ImGUI_Sky(void)
{
	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(400.f, 300.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("Skybox Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 좌측으로 고정
	ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 60.f));

	// 스카이박스 선택
	Control_Skybox();

	// 스카이박스 정보 저장/불러오기
	ImGUI_Setting_Skybox_SaveLoad();

	ImGui::End();
}

void CImGUI_Manager::ImGUI_PostProcessing(void)
{
	// 창의 사이즈를 조정
	ImGui::SetNextWindowSize(ImVec2(400.f, 200.f));

	// 조정된 사이즈의 창 출력 시작
	ImGui::Begin("PostProcessing Tool", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// ImGUI창의 시작 좌표를 좌측으로 고정
	ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 60.f));

	// White, MiddleGray 값 수정
	Control_PostProcessing();

	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");

	// 광원정보 저장/불러오기
	ImGUI_Setting_PostPorcessing_SaveLoad();

	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();
	ImGui::Text(" ");

	ImGUI_Setting_Emissive();

	ImGui::End();
}

void CImGUI_Manager::ImGUI_Show_Fire_Standard_Object(_double TimeDelta)
{
	if (!m_vFire_Standard_MapObjects.empty()) {
		if (m_vFire_Standard_MapObjects.size() > m_iCurrent_Standard_Object_Index) {
			m_vFire_Standard_MapObjects.at(m_iCurrent_Standard_Object_Index)->Tick(TimeDelta);
			m_vFire_Standard_MapObjects.at(m_iCurrent_Standard_Object_Index)->LateTick(TimeDelta);
		}
	}
}

void CImGUI_Manager::ImGUI_Show_Fire_Object(_double TimeDelta)
{
	if (!m_vFire_Standard_MapObjects.empty()) {
		if (m_vFire_Standard_MapObjects.size() > m_iCurrent_Standard_Object_Index) {
			{
				for (auto& Fire : m_vFires[m_iCurrent_Standard_Object_Index]) {
					Fire->Tick(TimeDelta);
					Fire->LateTick(TimeDelta);
				}
			}
		}
	}
}

void CImGUI_Manager::ImGUI_Setting_Emissive(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CShader* pShader = dynamic_cast<CShader*>(pGameInstance->Get_Component(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Com_Shader_NonAnim")));

	ImGui::SliderFloat("Emissive", &m_fEmissive, 0.f, 1.f);

	if (nullptr != pShader) {
		if (FAILED(pShader->Set_RawValue("fEmissive_Density", &m_fEmissive, sizeof(_float))))
			MSG_BOX(TEXT("CImGUI_Manager::ImGUI_Setting_Emissive - No Emissive Value in NonAnimShader!"));
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::Control_PostProcessing(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float fMiddleGray = pGameInstance->Get_MiddleGray();
	_float fWhite = pGameInstance->Get_White();

	ImGui::Text("MiddleGray : %.2f", fMiddleGray);
	ImGui::SliderFloat("MiddleGray", &fMiddleGray, 0.f, 5.f);

	ImGui::Text("White : %.2f", fWhite);
	ImGui::SliderFloat("White", &fWhite, 0.f, 2.f);

	pGameInstance->Set_MiddleGray(fMiddleGray);
	pGameInstance->Set_White(fWhite);

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::ImGUI_Setting_PostPorcessing_SaveLoad(void)
{
	// Save 버튼
	if (ImGui::Button("Save PostProcessing")) {

		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage01/", "PostProcessingInfo.dat");
			break;
		case 1:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage02/", "PostProcessingInfo.dat");
			break;
		case 2:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage03/", "PostProcessingInfo.dat");
			break;
		case 3:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage04/", "PostProcessingInfo.dat");
			break;
		case 4:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage05/", "PostProcessingInfo.dat");
			break;
		case 5:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage06/", "PostProcessingInfo.dat");
			break;
		case 6:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage07/", "PostProcessingInfo.dat");
			break;
		case 7:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage08/", "PostProcessingInfo.dat");
			break;
		case 8:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage09/", "PostProcessingInfo.dat");
			break;
		case 9:
			Save_PostProcessing_Info("../Bin/Resources/Dat/Stage10/", "PostProcessingInfo.dat");
			break;
		default:
			break;
		}
	}

	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	if (ImGui::Button("Load PostProcessing")) {
		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage01/", "PostProcessingInfo.dat");
			break;
		case 1:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage02/", "PostProcessingInfo.dat");
			break;
		case 2:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage03/", "PostProcessingInfo.dat");
			break;
		case 3:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage04/", "PostProcessingInfo.dat");
			break;
		case 4:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage05/", "PostProcessingInfo.dat");
			break;
		case 5:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage06/", "PostProcessingInfo.dat");
			break;
		case 6:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage07/", "PostProcessingInfo.dat");
			break;
		case 7:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage08/", "PostProcessingInfo.dat");
			break;
		case 8:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage09/", "PostProcessingInfo.dat");
			break;
		case 9:
			Load_PostProcessing_Info("../Bin/Resources/Dat/Stage10/", "PostProcessingInfo.dat");
			break;
		default:
			break;
		}
	}
}

HRESULT CImGUI_Manager::Save_PostProcessing_Info(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	_ulong		dwByte = 0;
	DWORD		dwStrByte = 0;

	HANDLE		hFile = CreateFile(szCurPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 가지고 있는 PostProcessing 정보 저장 - 톤 매핑시 사용

	// MiddleGray 저장
	_float fMiddleGray = pGameInstance->Get_MiddleGray();
	WriteFile(hFile, &fMiddleGray, sizeof(_float), &dwByte, nullptr);

	// White 저장
	_float fWhite = pGameInstance->Get_White();
	WriteFile(hFile, &fWhite, sizeof(_float), &dwByte, nullptr);

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CImGUI_Manager::Load_PostProcessing_Info(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// MiddleGray 저장
	_float fMiddleGray = 0.f;
	ReadFile(hFile, &fMiddleGray, sizeof(_float), &dwByte, nullptr); 
	pGameInstance->Set_MiddleGray(fMiddleGray);

	// White 저장
	_float fWhite = 0.f;
	ReadFile(hFile, &fWhite, sizeof(_float), &dwByte, nullptr);
	pGameInstance->Set_White(fWhite);

	RELEASE_INSTANCE(CGameInstance);

	// 파일을 닫는다.
	CloseHandle(hFile);

	return S_OK;
}

void CImGUI_Manager::Control_Skybox(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr == m_pSkybox) {
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_MapTool, LAYER_MAPTOOL_Skybox);

		if (nullptr != pLayer) {
			m_pSkybox = dynamic_cast<CSky*>(*pLayer->Get_ObjectList().begin());

			m_iMaxSkyTextureCnt = m_pSkybox->Get_TextureCnt();
		}
	}

	if (nullptr != m_pSkybox) {
		ImGui::Text("Curr Text Numger : %d", m_iCurSkyboxIndex);

		if (ImGui::Button("Index Up")) {
			++m_iCurSkyboxIndex;
		}
		if (ImGui::Button("Index Down")) {
			--m_iCurSkyboxIndex;

			if (m_iCurSkyboxIndex < 0)
				m_iCurSkyboxIndex = 0;
		}

		if (nullptr != m_pSkybox) {
			m_pSkybox->Set_CurTextureNum(m_iCurSkyboxIndex);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::ImGUI_Setting_Skybox_SaveLoad(void)
{
	// Save 버튼
	if (ImGui::Button("Save Skybox")) {

		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage01/", "SkyboxInfo.dat");
			break;
		case 1:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage02/", "SkyboxInfo.dat");
			break;
		case 2:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage03/", "SkyboxInfo.dat");
			break;
		case 3:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage04/", "SkyboxInfo.dat");
			break;
		case 4:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage05/", "SkyboxInfo.dat");
			break;
		case 5:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage06/", "SkyboxInfo.dat");
			break;
		case 6:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage07/", "SkyboxInfo.dat");
			break;
		case 7:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage08/", "SkyboxInfo.dat");
			break;
		case 8:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage09/", "SkyboxInfo.dat");
			break;
		case 9:
			Save_Skybox_Info("../Bin/Resources/Dat/Stage10/", "SkyboxInfo.dat");
			break;
		default:
			break;
		}
	}

	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	if (ImGui::Button("Load Skybox")) {
		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage01/", "SkyboxInfo.dat");
			break;
		case 1:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage02/", "SkyboxInfo.dat");
			break;
		case 2:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage03/", "SkyboxInfo.dat");
			break;
		case 3:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage04/", "SkyboxInfo.dat");
			break;
		case 4:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage05/", "SkyboxInfo.dat");
			break;
		case 5:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage06/", "SkyboxInfo.dat");
			break;
		case 6:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage07/", "SkyboxInfo.dat");
			break;
		case 7:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage08/", "SkyboxInfo.dat");
			break;
		case 8:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage09/", "SkyboxInfo.dat");
			break;
		case 9:
			Load_Skybox_Info("../Bin/Resources/Dat/Stage10/", "SkyboxInfo.dat");
			break;
		default:
			break;
		}
	}
}

HRESULT CImGUI_Manager::Save_Skybox_Info(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	_ulong		dwByte = 0;
	DWORD		dwStrByte = 0;

	HANDLE		hFile = CreateFile(szCurPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;

	// 가지고 있는 스카이 박스의 텍스쳐 번호 저장
	_uint iCurTextureNum = m_pSkybox->Get_CurTextureNum();
	WriteFile(hFile, &iCurTextureNum, sizeof(_uint), &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CImGUI_Manager::Load_Skybox_Info(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// MiddleGray 저장
	_uint iCurTextureNum = 0;
	ReadFile(hFile, &iCurTextureNum, sizeof(_uint), &dwByte, nullptr);
	m_iCurSkyboxIndex = iCurTextureNum;
	m_pSkybox->Set_CurTextureNum(iCurTextureNum);

	// 파일을 닫는다.
	CloseHandle(hFile);

	return S_OK;
}

void CImGUI_Manager::Create_Lights(void)
{
	static _float fCreateStartPos[3];

	ImGui::InputFloat3("Startring Pos", fCreateStartPos, "%.2f");

	if (ImGui::Button("Create Point Light")) {
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// 점광원 생성
		LIGHTDESC LightDesc;
		ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

		LightDesc.eType = tagLightDesc::TYPE_POINT;
		LightDesc.vPosition = _float4(fCreateStartPos[0], fCreateStartPos[1], fCreateStartPos[2], 1.f);
		LightDesc.fRange = 10.f;
		LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.0f, 1.f);
		LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
		LightDesc.vSpecular = _float4(1.f, 0.0f, 0.0f, 1.f);
		LightDesc.fPower = 10.f;

		if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
			return;

		RELEASE_INSTANCE(CGameInstance);
	}

	if (ImGui::Button("Create Camera Point Light")) {
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// 점광원 생성
		LIGHTDESC LightDesc;
		ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

		_vector vPos = dynamic_cast<CTransform*>(m_pCurrCamera->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
		_float3 fPos;
		XMStoreFloat3(&fPos, vPos);

		LightDesc.eType = tagLightDesc::TYPE_POINT;
		LightDesc.vPosition = _float4(fPos.x, fPos.y, fPos.z, 1.f);
		LightDesc.fRange = 10.f;
		LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.0f, 1.f);
		LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
		LightDesc.vSpecular = _float4(1.f, 0.0f, 0.0f, 1.f);
		LightDesc.fPower = 10.f;

		if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
			return;

		RELEASE_INSTANCE(CGameInstance);
	}

}

void CImGUI_Manager::Delete_Lights(void)
{
	// 현재 선택한 점광원의 정보를 삭제한다.
	if (ImGui::Button("Delete Select Point Light")) {
		if (nullptr != m_pSelected_PointLight) {
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			// 삭제한 점광원의 정보는 Light_Manager에도 적용된다.
			if (FAILED(pGameInstance->Delete_Lights(m_pSelected_PointLight))) {
				MSG_BOX(TEXT("Failed to Delete Select PointLight"));
			}

			m_pSelected_PointLight = nullptr;

			RELEASE_INSTANCE(CGameInstance);
		}
	}
}

void CImGUI_Manager::Control_Select_PointLight(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 선택한 점광원의 정보가 존재할 경우
	if (nullptr != m_pSelected_PointLight) {
		// 해당 점광원의 정보를 출력 및 수정한다.

		// 선택한 점광원의 정보를 가져온다.
		LIGHTDESC tLightDesc = *m_pSelected_PointLight->Get_LightDesc();


		// 점광원의 정보 출력
		ImGui::Text("Point Light Info");


		// 점광원의 위치
		ImGui::Text("Point - x : %.2f   y : %.2f   z : %.2f", tLightDesc.vPosition.x, tLightDesc.vPosition.y, tLightDesc.vPosition.z);

		_float fPosition[3] = { tLightDesc.vPosition.x, tLightDesc.vPosition.y, tLightDesc.vPosition.z };
		if (ImGui::InputFloat3("Point Light - Position", fPosition, "%.2f")) {
			tLightDesc.vPosition.x = fPosition[0];
			tLightDesc.vPosition.y = fPosition[1];
			tLightDesc.vPosition.z = fPosition[2];
		}



		ImGui::NextColumn();
		ImGui::Text(" ");



		// 점광원의 범위
		ImGui::Text("Range - %.2f", tLightDesc.fRange);

		_float fRange = { tLightDesc.fRange };
		if (ImGui::InputFloat("Point Light - Range", &fRange, 0.f, 100.f, "%.2f")) {
			tLightDesc.fRange = fRange;
		}



		ImGui::NextColumn();
		ImGui::Text(" ");





		// 점광원의 방향
		ImGui::Text("Direction - x : %.2f   y : %.2f   z : %.2f", tLightDesc.vDirection.x, tLightDesc.vDirection.y, tLightDesc.vDirection.z);

		_float fDir[3] = { tLightDesc.vDirection.x, tLightDesc.vDirection.y, tLightDesc.vDirection.z };
		if (ImGui::InputFloat3("Point Light - Direction", fDir, "%.2f")) {
			tLightDesc.vDirection.x = fDir[0];
			tLightDesc.vDirection.y = fDir[1];
			tLightDesc.vDirection.z = fDir[2];
		}



		ImGui::NextColumn();
		ImGui::Text(" ");



		// 방향광의 색상
		ImGui::Text("Diffuse - x : %.2f   y : %.2f   z : %.2f   w : %.2f", tLightDesc.vDiffuse.x, tLightDesc.vDiffuse.y, tLightDesc.vDiffuse.z, tLightDesc.vDiffuse.w);

		_float vColor[4];
		vColor[0] = tLightDesc.vDiffuse.x;
		vColor[1] = tLightDesc.vDiffuse.y;
		vColor[2] = tLightDesc.vDiffuse.z;
		vColor[3] = tLightDesc.vDiffuse.w;

		ImGui::PushItemWidth(200);
		if (ImGui::ColorEdit4("Point Light - Diffuse", vColor)) {
			tLightDesc.vDiffuse.x = vColor[0];
			tLightDesc.vDiffuse.y = vColor[1];
			tLightDesc.vDiffuse.z = vColor[2];
			tLightDesc.vDiffuse.w = vColor[3];
		}




		ImGui::NextColumn();
		ImGui::Text(" ");




		// 방향광의 주변광(Ambient)
		_float vAmbient[4];
		vAmbient[0] = tLightDesc.vAmbient.x;
		vAmbient[1] = tLightDesc.vAmbient.y;
		vAmbient[2] = tLightDesc.vAmbient.z;
		vAmbient[3] = tLightDesc.vAmbient.w;

		ImGui::Text("Ambient - x : %.2f   y : %.2f   z : %.2f   w : %.2f", tLightDesc.vDiffuse.x, tLightDesc.vDiffuse.y, tLightDesc.vDiffuse.z, tLightDesc.vAmbient.w);

		if (ImGui::InputFloat3("Point Light - Ambient", vAmbient, "%.2f")) {
			tLightDesc.vAmbient.x = vAmbient[0];
			tLightDesc.vAmbient.y = vAmbient[1];
			tLightDesc.vAmbient.z = vAmbient[2];
			tLightDesc.vAmbient.w = 1.f;
		}


		// 반사광은 존재하지 않는다. 


		ImGui::NextColumn();
		ImGui::Text(" ");



		// 점광원의 파워
		ImGui::Text("Power - %.2f", tLightDesc.fPower);

		_float fPower = { tLightDesc.fPower };
		if (ImGui::InputFloat("Point Light - Power", &fPower, 0.f, 100.f, "%.2f")) {
			tLightDesc.fPower = fPower;
		}




		// 전체 빛의 정보에서 해당 인덱스를 검색
		m_iLightIndex = Get_LightIndex(m_pSelected_PointLight);

		// 수정한 빛의 정보를 반영
		pGameInstance->Set_LightDesc(m_iLightIndex, tLightDesc);

	}



	/*list<CLight*>* pLights = pGameInstance->Get_Lights();
	if (nullptr == pLights) {
		return;
	}

	CLight* pLight = nullptr;
	_uint iIndex = 0;

	if (ImGui::Button("IndexUp"))
	{
		++m_iLightIndex;
		if (m_iLightIndex >= pLights->size())
		{
			m_iLightIndex = (_uint)pLights->size() - 1;
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("IndexDown"))
	{
		--m_iLightIndex;
		if (m_iLightIndex <= 0)
		{
			m_iLightIndex = 0;
		}
	}

	for (auto& iter = pLights->begin(); iter != pLights->end(); ++iter)
	{
		if (m_iLightIndex == iIndex)
		{
			pLight = *iter;
			break;
		}
		++iIndex;
	}

	if (nullptr == pLight)
	{
		return;
	}

	LIGHTDESC tLightDesc = *pLight->Get_LightDesc();

	char szText[MAX_PATH] = "";
	switch (tLightDesc.eType)
	{
	case LIGHTDESC::TYPE_DIRECTIONAL:
		sprintf_s(szText, "Directional Light%d", m_iLightIndex);
		break;
	case LIGHTDESC::TYPE_POINT:
		sprintf_s(szText, "Point Light%d", m_iLightIndex);
		break;
	}

	ImGui::Text(szText);

	ImGui::Text("x : %.2f", tLightDesc.vPosition.x);
	ImGui::SameLine();
	ImGui::Text("y : %.2f", tLightDesc.vPosition.y);
	ImGui::SameLine();
	ImGui::Text("z : %.2f", tLightDesc.vPosition.z);

	_float fPos[3] = { tLightDesc.vPosition.x, tLightDesc.vPosition.y, tLightDesc.vPosition.z };

	if (ImGui::InputFloat3("Position", fPos, "%.2f"))
	{
		tLightDesc.vPosition.x = fPos[0];
		tLightDesc.vPosition.y = fPos[1];
		tLightDesc.vPosition.z = fPos[2];
	}

	if (ImGui::Button("x up"))
	{
		tLightDesc.vPosition.x += 0.1f;
	}
	ImGui::SameLine();
	if (ImGui::Button("x down"))
	{
		tLightDesc.vPosition.x -= 0.1f;
	}
	ImGui::SameLine();
	if (ImGui::Button("y up"))
	{
		tLightDesc.vPosition.y += 0.1f;
	}
	ImGui::SameLine();
	if (ImGui::Button("y down"))
	{
		tLightDesc.vPosition.y -= 0.1f;
	}
	ImGui::SameLine();
	if (ImGui::Button("z up"))
	{
		tLightDesc.vPosition.z += 0.1f;
	}
	ImGui::SameLine();
	if (ImGui::Button("z down"))
	{
		tLightDesc.vPosition.z -= 0.1f;
	}

	_float fLightPower = tLightDesc.fPower;
	if (ImGui::InputFloat("LightPower", &fLightPower))
	{
		tLightDesc.fPower = fLightPower;
	}

	if (ImGui::Button("LightPower up"))
	{
		tLightDesc.fPower += 1.f;
	}
	ImGui::SameLine();
	if (ImGui::Button("LightPower down"))
	{
		tLightDesc.fPower -= 1.f;
	}

	_float fLightRange = tLightDesc.fRange;
	if (ImGui::InputFloat("LightRange", &fLightRange))
	{
		tLightDesc.fRange = fLightRange;
	}

	if (ImGui::Button("LightRange up"))
	{
		tLightDesc.fRange += 1.f;
	}
	ImGui::SameLine();
	if (ImGui::Button("LightRange down"))
	{
		tLightDesc.fRange -= 1.f;
	}

	_float fLightAmbient[4] = { tLightDesc.vAmbient.x, tLightDesc.vAmbient.y, tLightDesc.vAmbient.z, tLightDesc.vAmbient.w };
	if (ImGui::SliderFloat4("LightAmbient", fLightAmbient, 0.f, 1.f))
	{
		tLightDesc.vAmbient.x = fLightAmbient[0];
		tLightDesc.vAmbient.y = fLightAmbient[1];
		tLightDesc.vAmbient.z = fLightAmbient[2];
		tLightDesc.vAmbient.w = fLightAmbient[3];
	}

	pGameInstance->Set_LightDesc(m_iLightIndex, tLightDesc);*/

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::Update_Lights(void)
{
	// 방향광과 점광원의 정보를 담고 있는 vector를 초기화 시킨다.
	m_vDirectionalLight.clear();
	m_vPointLight.clear();

	// 빛의 정보를 가져온다.
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	list<CLight*>* pLights = pGameInstance->Get_Lights();

	if (nullptr == pLights)
		return;

	// 가져온 정보를 돌여 해당하는 정보에 따라 빛의 정보를 vector에 담는다.
	CLight* pLight = nullptr;
	for (auto& iter = pLights->begin(); iter != pLights->end(); ++iter)
	{
		pLight = *iter;

		LIGHTDESC tLightDesc = *pLight->Get_LightDesc();
		switch (tLightDesc.eType)
		{
		case LIGHTDESC::TYPE_DIRECTIONAL:
			m_vDirectionalLight.push_back(pLight);
			break;
		case LIGHTDESC::TYPE_POINT:
			m_vPointLight.push_back(pLight);
			break;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::Show_CamPosition(void)
{
	CGameInstance* pGameIstance = GET_INSTANCE(CGameInstance);

	_float4 vCameraPos = pGameIstance->Get_CamPositionFloat4();
	ImGui::Text("Cam Position   x : %.2f  y : %.2f  z : %.2f", vCameraPos.x, vCameraPos.y, vCameraPos.z);

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::Show_DirectionLight(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_vDirectionalLight.empty())
		return;

	ImGui::Text("Directional Light Info");

	LIGHTDESC tLightDesc = *((*m_vDirectionalLight.begin())->Get_LightDesc());

	// 방향광의 방향
	// 방향광의 색상
	// 방향광의 반사광 -> 제외(디퍼드 연산시 포함되지 않음)
	// 방향광의 주변광

	// 방향광의 방향
	ImGui::Text("Direction - x : %.2f   y : %.2f   z : %.2f", tLightDesc.vDirection.x, tLightDesc.vDirection.y, tLightDesc.vDirection.z);

	_float fDir[3] = { tLightDesc.vDirection.x, tLightDesc.vDirection.y, tLightDesc.vDirection.z };
	if (ImGui::InputFloat3("Directional Light - Direction", fDir, "%.2f")) {
		tLightDesc.vDirection.x = fDir[0];
		tLightDesc.vDirection.y = fDir[1];
		tLightDesc.vDirection.z = fDir[2];
	}



	ImGui::NextColumn();
	ImGui::Text(" ");



	// 방향광의 색상
	ImGui::Text("Ambient - x : %.2f   y : %.2f   z : %.2f   w : %.2f", tLightDesc.vAmbient.x, tLightDesc.vAmbient.y, tLightDesc.vAmbient.z, tLightDesc.vAmbient.w);

	_float vColor[4]; 
	vColor[0] = tLightDesc.vAmbient.x;
	vColor[1] = tLightDesc.vAmbient.y;
	vColor[2] = tLightDesc.vAmbient.z;
	vColor[3] = tLightDesc.vAmbient.w;

	ImGui::PushItemWidth(200);
	if (ImGui::ColorEdit4("Directional Light - Diffuse", vColor)) {
		tLightDesc.vAmbient.x = vColor[0];
		tLightDesc.vAmbient.y = vColor[1];
		tLightDesc.vAmbient.z = vColor[2];
		tLightDesc.vAmbient.w = vColor[3];
	}




	ImGui::NextColumn();
	ImGui::Text(" ");




	//// 방향광의 주변광(Ambient) -> 방향광의 색상이 주변광으로 들어간다.
	//_float vAmbient[4];
	//vAmbient[0] = tLightDesc.vAmbient.x;
	//vAmbient[1] = tLightDesc.vAmbient.y;
	//vAmbient[2] = tLightDesc.vAmbient.z;
	//vAmbient[3] = tLightDesc.vAmbient.w;

	//ImGui::Text("Ambient - x : %.2f   y : %.2f   z : %.2f   w : %.2f", tLightDesc.vDiffuse.x, tLightDesc.vDiffuse.y, tLightDesc.vDiffuse.z, tLightDesc.vAmbient.w);

	//if (ImGui::InputFloat3("Directional Light - Ambient", vAmbient, "%.2f")) {
	//	tLightDesc.vAmbient.x = vAmbient[0];
	//	tLightDesc.vAmbient.y = vAmbient[1];
	//	tLightDesc.vAmbient.z = vAmbient[2];
	//	tLightDesc.vAmbient.w = 1.f;
	//}


	// 반사광은 존재하지 않는다. 


	// 전체 빛의 정보에서 해당 인덱스를 검색
	m_iLightIndex = Get_LightIndex(*m_vDirectionalLight.begin());
	
	// 수정한 빛의 정보를 반영
	pGameInstance->Set_LightDesc(m_iLightIndex, tLightDesc);

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::Show_PointLight(void)
{
	// 점광원의 리스트 박스 출력
	ImGui::Text("Point light ListBox");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 불의 최대 갯수를 20개로 한다.
	const char* Point_Lights[]{
		"000","001","002","003","004","005","006","007","008","009",
		"010","011","012","013","014","015","016","017","018","019",
		"020","021","022","023","024","025","026","027","028","029",
		"030","031","032","033","034","035","036","037","038","039",
		"040","041","042","043","044","045","046","047","048","049",
		"050","051","052","053","054","055","056","057","058","059",
		"060","061","062","063","064","065","066","067","068","069",
		"070","071","072","073","074","075","076","077","078","079",
		"080","081","082","083","084","085","086","087","088","089",
		"090","091","092","093","094","095","096","097","098","099"
	};

	// 가지고 있는 불의 정보를 선택하여 현재 선택한 불의 정보를 수정할 수 있다.

	// 리스트 박스를 구현한다.
	// 칸의 가로 길이를 400으로 조정
	ImGui::PushItemWidth(400.f);
	
	// 선택한 Index를 가진 불의 정보를 선택한다.
	ImGui::ListBox("3", &m_iSelect_PointLight_Index, Point_Lights, (int)m_vPointLight.size(), 5);

	// 선택한 오브젝트 인덱스가 0번쨰이고, 현재 인덱스도 0번쨰 일 경우, 그러니깐 처음 생성한 경우
	if (m_iSelect_PointLight_Index == 0 && m_iCurrent_PointLight_Index == 0) {

		// 빛의 정보가 존재할 경우
		if (!m_vPointLight.empty()) {
			// 아직 빛의 정보를 선택하지 않았을 경우
			if (nullptr == m_pSelected_PointLight) {
				// 처음 생성한 빛의 정보를 선택한다.
				m_pSelected_PointLight = m_vPointLight.at(m_iSelect_PointLight_Index);
			}
		}
	}

	// 선택한 오브젝트 인덱스가 현재 인덱스와 다른 경우
	if (m_iSelect_PointLight_Index != m_iCurrent_PointLight_Index) {
		m_iCurrent_PointLight_Index = m_iSelect_PointLight_Index;

		if (!m_vPointLight.empty()) {
			if (m_iSelect_PointLight_Index < 0)
				m_iSelect_PointLight_Index = 0;

			m_pSelected_PointLight = m_vPointLight.at(m_iSelect_PointLight_Index);
		}
	}


	/*
	typedef struct tagLightDesc
	{
	enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };
	TYPE eType;

	XMFLOAT4 vPosition;
	float fRange;

	XMFLOAT4 vDirection;

	XMFLOAT4 vDiffuse;
	XMFLOAT4 vAmbient;
	XMFLOAT4 vSpecular;

	float fPower;
	}LIGHTDESC;
	*/


	// 점광원의 위치
	// 점광원의 범위
	// 점광원의 방향
	// 점광원의 색상
	// 점광원의 방향광
	// 점광원의 주변광


}

void CImGUI_Manager::CreateDelete_PointLight(void)
{
	// 점광원 생성 버튼
	Create_Lights();

	// 점광원 삭제 버튼
	Delete_Lights();
}

void CImGUI_Manager::Create_PointLight_LightObject(void)
{
	// 점광원의 충돌체 오브젝트 생성
	if (ImGui::Button("Create LightObject")) {
		// 존재하는 모든 점광원에 충돌체 오브젝트를 생성한다.
		for (auto& Light : m_vPointLight) {
			Create_LightObject(Light);
		}
	}
}

void CImGUI_Manager::Delete_PointLight_LightObject(void)
{
	// 점광원의 충돌체 오브젝트 삭제
	if (ImGui::Button("Delete LightObject")) {
		// 존재하는 모든 점광원에 충돌체 오브젝트를 삭제
		
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_MapTool, LAYER_MAPTOOL_LightObjects);

		// Layer가 존재할 경우
		if (nullptr != pLayer) {
			// 해당 Layer에 있는 오브젝트를 전부 삭제한다.
			auto& ObjList = pLayer->Get_ObjectList();

			for (auto& Obj : ObjList) {
				Obj->Set_Dead(true);
			}
		}

		RELEASE_INSTANCE(CGameInstance);


	}
}

HRESULT CImGUI_Manager::Save_Lights(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	_ulong		dwByte = 0;
	DWORD		dwStrByte = 0;

	HANDLE		hFile = CreateFile(szCurPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;


	// 가지고 있는 방향광의 정보 저장

	// 방향광의 갯수 저장
	_uint iDirectionalLightCnt = m_vDirectionalLight.size();
	WriteFile(hFile, &iDirectionalLightCnt, sizeof(_uint), &dwByte, nullptr);

	for (int i = 0; i < iDirectionalLightCnt; ++i) {
		// 방향광의 빛 정보 추출
		LIGHTDESC LightDesc = *m_vDirectionalLight.at(i)->Get_LightDesc();

		// 방향광의 빛 정보 저장
		WriteFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);
	}



	// 가지고 있는 점광원의 정보 저장

	// 점광원의 갯수 저장
	_uint iPointLightCnt = m_vPointLight.size();
	WriteFile(hFile, &iPointLightCnt, sizeof(_uint), &dwByte, nullptr);

	for (int i = 0; i < iPointLightCnt; ++i) {
		// 점광원의 빛 정보 추출
		LIGHTDESC LightDesc = *m_vPointLight.at(i)->Get_LightDesc();

		// 점광원의 빛 정보 저장
		WriteFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);
	}
	
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CImGUI_Manager::Load_Lights(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	
	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 기존에 존재하는 빛의 정보를 삭제한다.
	pGameInstance->Clear_Lights();

	while (true)
	{
		// 방향광의 갯수 읽기
		_uint iDirectionalLightCnt;
		ReadFile(hFile, &iDirectionalLightCnt, sizeof(_uint), &dwByte, nullptr);

		LIGHTDESC LightDesc;
		ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

		for (int i = 0; i < iDirectionalLightCnt; ++i) {
			// 방향광의 빛 정보 로드 
			ReadFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);

			// 방향광 생성
			pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc);
		}


		// 점광원의 갯수 읽기
		_uint iPointLightCnt;
		ReadFile(hFile, &iPointLightCnt, sizeof(_uint), &dwByte, nullptr);

		// 점광원의 정보 로드
		for (int i = 0; i < iPointLightCnt; ++i) {
			// 점광원의 빛 정보 로드 
			ReadFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);

			// 점광원 생성
			pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc);
		}

		break;
	}

	RELEASE_INSTANCE(CGameInstance);

	// 파일을 닫는다.
	CloseHandle(hFile);

	m_pSelected_PointLight = nullptr;

	return S_OK;
}

void CImGUI_Manager::ImGUI_Setting_Light_SaveLoad(void)
{
	// Save 버튼
	if (ImGui::Button("Save Light")) {

		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			Save_Lights("../Bin/Resources/Dat/Stage01/", "LightInfo.dat");
			break;
		case 1:
			Save_Lights("../Bin/Resources/Dat/Stage02/", "LightInfo.dat");
			break;
		case 2:
			Save_Lights("../Bin/Resources/Dat/Stage03/", "LightInfo.dat");
			break;
		case 3:
			Save_Lights("../Bin/Resources/Dat/Stage04/", "LightInfo.dat");
			break;
		case 4:
			Save_Lights("../Bin/Resources/Dat/Stage05/", "LightInfo.dat");
			break;
		case 5:
			Save_Lights("../Bin/Resources/Dat/Stage06/", "LightInfo.dat");
			break;
		case 6:
			Save_Lights("../Bin/Resources/Dat/Stage07/", "LightInfo.dat");
			break;
		case 7:
			Save_Lights("../Bin/Resources/Dat/Stage08/", "LightInfo.dat");
			break;
		case 8:
			Save_Lights("../Bin/Resources/Dat/Stage09/", "LightInfo.dat");
			break;
		case 9:
			Save_Lights("../Bin/Resources/Dat/Stage10/", "LightInfo.dat");
			break;
		default:
			break;
		}
	}

	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	if (ImGui::Button("Load Light")) {
		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			Load_Lights("../Bin/Resources/Dat/Stage01/", "LightInfo.dat");
			break;
		case 1:
			Load_Lights("../Bin/Resources/Dat/Stage02/", "LightInfo.dat");
			break;
		case 2:
			Load_Lights("../Bin/Resources/Dat/Stage03/", "LightInfo.dat");
			break;
		case 3:
			Load_Lights("../Bin/Resources/Dat/Stage04/", "LightInfo.dat");
			break;
		case 4:
			Load_Lights("../Bin/Resources/Dat/Stage05/", "LightInfo.dat");
			break;
		case 5:
			Load_Lights("../Bin/Resources/Dat/Stage06/", "LightInfo.dat");
			break;
		case 6:
			Load_Lights("../Bin/Resources/Dat/Stage07/", "LightInfo.dat");
			break;
		case 7:
			Load_Lights("../Bin/Resources/Dat/Stage08/", "LightInfo.dat");
			break;
		case 8:
			Load_Lights("../Bin/Resources/Dat/Stage09/", "LightInfo.dat");
			break;
		case 9:
			Load_Lights("../Bin/Resources/Dat/Stage10/", "LightInfo.dat");
			break;
		default:
			break;
		}
	}
}

void CImGUI_Manager::Create_LightObject(CLight * pLight)
{
	if (nullptr == pLight) {
		MSG_BOX(TEXT("CImGUI_Manager - Create_Lights : Wrong Parameter Detected"));
		return;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 해당 점광원을 따라 다니는 LightObject를 하나 만든다.
	CLightObject::LIGHTOBJDesc LightObjectDesc;
	LightObjectDesc.fColliderSize = _float3(3.f, 3.f, 3.f);
	LightObjectDesc.pTargetLight = pLight;

	CLightObject* pLigtObject = dynamic_cast<CLightObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_LightObjects, TEXT("Prototype_GameObject_LightObject"), &LightObjectDesc));

	if (nullptr == pLigtObject) {
		MSG_BOX(TEXT("CImGUI_Manager - Create_Lights : Failed to Create LightObject"));
		return;
	}

	// 점광원 충돌체 그려주는 오브젝트 vector에 따로 저장
	m_vLightObject.push_back(pLigtObject);

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::Control_DirectionLight(void)
{

}

_uint CImGUI_Manager::Get_LightIndex(CLight * pLight)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	list<CLight*> Lights = *pGameInstance->Get_Lights();

	_uint iIndex = 0;
	for (auto& Light : Lights) {
		if (Light == pLight)
			break;

		++iIndex;
	}

	RELEASE_INSTANCE(CGameInstance);

	return iIndex;
}

void CImGUI_Manager::Create_StandardObject(void)
{
	// 현재 list에 있는 mesh 갯수 출력
	ImGui::Text("Mesh list - loaded [ %d ] Meshes", m_strMeshList.size());

	// 칸의 가로 길이를 150으로 조정
	ImGui::PushItemWidth(150.f);

	ImVec2 ListBoxSize = ImVec2(380.f, 250);
	static _int iCurrIndex = -1;
	if (ImGui::BeginListBox(" ", ListBoxSize)) {

		_uint iCnt = 0;
		static const char* Selected_IndexTag = " ";
		for (auto& strName : m_strMeshList) {
			// wstring 형 문자열을 string 형으로 변환
			string str(strName.begin(), strName.end());

			// 선택된 문자열과 현재 인덱스의 문자열이 같다면 true 아니면 false 대입
			_bool is_selected = (Selected_IndexTag == str);

			// 인덱스 확인
			if (ImGui::Selectable(m_szMeshList[iCnt], is_selected)) {
				Selected_IndexTag = m_szMeshList[iCnt];
				static basic_string<TCHAR> tStr;
				tStr = basic_string<TCHAR>(strName.begin(), strName.end());

				m_pPrototypeTag_SelectedMesh = tStr.c_str();
				iCurrIndex = iCnt;

				m_bSelectMesh = false;
				m_pSelectedFeatureObject = nullptr;
			}

			++iCnt;
		}

		ImGui::EndListBox();
	}

	static string CurrIndexTag;

	if (iCurrIndex >= 0) {
		string s(m_strMeshList.at(iCurrIndex).begin(), m_strMeshList.at(iCurrIndex).end());
		CurrIndexTag = s;
	}
	else
		CurrIndexTag = "None";

	char buff[MAX_PATH] = " ";
	sprintf_s(buff, sizeof(buff), "%s", CurrIndexTag.c_str());
	ImGui::Text("Select Mesh");
	ImGui::Text("%s", buff);


	// 생성 버튼, 버튼을 누르면
	if (ImGui::Button("Create TargetObject")) {
		wstring wstr(m_pPrototypeTag_SelectedMesh);
		if (!wstr.empty()) {

			// 현재 선택한 오브젝트가 이미 존재하는 오브젝트인지 검사하여 해당 오브젝트가 존재하면 생성하지 않는다.
			_bool bAlreadyExist = false;
			for (auto& MapObject : m_vFire_Standard_MapObjects) {
				if (!_tcscmp(MapObject->Get_ModelTag(), m_pPrototypeTag_SelectedMesh)) {
					bAlreadyExist = true;
					break;
				}
			}

			if (!bAlreadyExist) {
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

				// 선택한 오브젝트가 화면 중앙에 행렬정보를 XMMatrixIdentity()로 설정한 상태로 생성된다.

				// 선택한 오브젝트의 모델 tag는 CurrIndexTag값이다.
				CMapObject::MAPOBJDesc Desc;
				Desc.pPrototype_ObjectName = m_pPrototypeTag_SelectedMesh;

				XMStoreFloat4x4(&Desc.TransformMatrix, XMMatrixIdentity());

				// 충돌체 생성
				Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

				// 맵 오브젝트 생성
				CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Clone_Prototype(TEXT("Prototype_GameObject_MapObject"), &Desc));
				m_vFire_Standard_MapObjects.push_back(pMapObject);		// 맵 오브젝트 정보 푸쉬

				// 현재 선택한 기준이 되는 오브젝트 정보를 가장 최근에 생성한 오브젝트로 한다.
				m_pSelected_Fire_Standard_MapObject = pMapObject;

				// 선택한 불의 정보를 nullptr로 만든다.
				m_pSelected_FireObject = nullptr;

				Update_Standard_MapObject_List();

				RELEASE_INSTANCE(CGameInstance);
			}
		}
	}


	// 현재 list에 있는 mesh 갯수 출력
	ImGui::Text("Mesh list - loaded [ %d ] Meshes", m_vFire_Standard_MapObjects.size());

	// 칸의 가로 길이를 150으로 조정
	ImGui::PushItemWidth(150.f);

	// 여기서 원하는 오브젝트 선택
	ListBoxSize = ImVec2(380.f, 250);
	static _int iCurrIndex_Standard_Object = -1;
	if (ImGui::BeginListBox("   ", ListBoxSize)) {

		_uint iCnt = 0;
		static const char* Selected_IndexTag = " ";
		for (auto& MapObject : m_vFire_Standard_MapObjects) {
			// wstring 형 문자열을 string 형으로 변환
			wstring wstr(MapObject->Get_ModelTag());
			string str(wstr.begin(), wstr.end());

			// 선택된 문자열과 현재 인덱스의 문자열이 같다면 true 아니면 false 대입
			_bool is_selected = (Selected_IndexTag == str);

			// 인덱스 확인
			if (ImGui::Selectable(m_szStandard_Object_List[iCnt], is_selected)) {
				Selected_IndexTag = m_szStandard_Object_List[iCnt];

				m_pPrototypeTag_SelectedMesh = MapObject->Get_ModelTag();
				iCurrIndex_Standard_Object = iCnt;

				// 현재 선택한 불 인덱스 정보 저장
				m_iCurrent_Standard_Object_Index = iCurrIndex_Standard_Object;

				m_pSelected_FireObject= nullptr;
			}

			++iCnt;
		}

		ImGui::EndListBox();
	}

	if (iCurrIndex_Standard_Object >= 0) {
		if (m_vFire_Standard_MapObjects.size() > iCurrIndex_Standard_Object) {
			wstring wstr(m_vFire_Standard_MapObjects.at(iCurrIndex_Standard_Object)->Get_ModelTag());
			string s(wstr.begin(), wstr.end());
			CurrIndexTag = s;
		}
	}
	else
		CurrIndexTag = "None";

	ZeroMemory(buff, sizeof(char) * MAX_PATH);
	sprintf_s(buff, sizeof(buff), "%s", CurrIndexTag.c_str());
	ImGui::Text("Select Mesh");
	ImGui::Text("%s", buff);

	ImGui::NextColumn();
	ImGui::Text(" ");
	ImGui::NextColumn();







	// 생성 버튼, 버튼을 누르면
	// 선택한 오브젝트 정보를 삭제하고, 관련 불 또한 삭제한다.
	if (ImGui::Button("Delete Select Object")) {
		// 기준 오브젝트 삭제
		m_vFire_Standard_MapObjects.erase(m_vFire_Standard_MapObjects.begin() + m_iCurrent_Standard_Object_Index);

		// 관련 불 오브젝트 삭제
		for (auto Fire : m_vFires[m_iCurrent_Standard_Object_Index]) {
			Safe_Release(Fire);
		}
		m_vFires[m_iCurrent_Standard_Object_Index].clear();

		Update_Standard_MapObject_List();

		if (m_vFire_Standard_MapObjects.empty())
			iCurrIndex_Standard_Object = -1;
	}

}

void CImGUI_Manager::Create_FireObject(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	static _float fFire_GlowStrength = 0.f;
	ImGui::SliderFloat("Glow Strength", &fFire_GlowStrength, 0.f, 1.f);

	static _float fFire_OriginalStrength = 0.f;
	ImGui::SliderFloat("Original Strength", &fFire_OriginalStrength, 0.f, 1.f);

	static _int iFrameSpeed = 0.f;
	ImGui::SliderInt("Frame Speed", &iFrameSpeed, 0.f, 20.f);

	static _float fLook[3];
	ImGui::SliderFloat3("Look", fLook, -1.f, 1.f);

	static _float fPos[3];
	ImGui::SliderFloat3("Position", fPos, -1000.f, 1000.f);

	static _float fScale[3];
	ImGui::SliderFloat3("Scale", fScale, 0.0000001f, 100.f);

	if (ImGui::Button("Create Fire")) {

		if (m_iCurrent_Standard_Object_Index >= 0) {
			// 불을 생성하기 위한 정보 설정
			CFire_Deco::FIREDECODESC FireDesc;
			FireDesc.fGlowStrength = fFire_GlowStrength;
			FireDesc.fOriginalStrength = fFire_OriginalStrength;
			FireDesc.iFrameSpeed = iFrameSpeed;
			FireDesc.iShaderPass = 2;
			FireDesc.tagTextureCom = TEXT("Prototype_Component_Texture_Fire0");

			memcpy(&FireDesc.vLook, fLook, sizeof(_float) * 3);
			memcpy(&FireDesc.vPosition, fPos, sizeof(_float) * 3);
			memcpy(&FireDesc.vScale, fScale, sizeof(_float) * 3);

			FireDesc.fRotation = _float3(0.f, 0.f, 0.f);

			// 불을 생성하여 해당 정보를 m_vFires에 저장한다.
			CFire_Deco* pFire = dynamic_cast<CFire_Deco*>(pGameInstance->Clone_Prototype(TEXT("Prototype_GameObject_Fire"), &FireDesc));
			pFire->Set_Target(m_pCurrCamera);
			m_vFires[m_iCurrent_Standard_Object_Index].push_back(pFire);
		}
	}



	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::Show_FireObjectList(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 불의 최대 갯수를 20개로 한다.
	const char* FeatureMesh[] {
		"000","001","002","003","004","005","006","007","008","009",
		"010","011","012","013","014","015","016","017","018","019",
		"020","021","022","023","024","025","026","027","028","029",
		"030","031","032","033","034","035","036","037","038","039",
		"040","041","042","043","044","045","046","047","048","049",
		"050","051","052","053","054","055","056","057","058","059",
		"060","061","062","063","064","065","066","067","068","069",
		"070","071","072","073","074","075","076","077","078","079",
		"080","081","082","083","084","085","086","087","088","089",
		"090","091","092","093","094","095","096","097","098","099"
	};

	// 가지고 있는 불의 정보를 선택하여 현재 선택한 불의 정보를 수정할 수 있다.

	// 리스트 박스를 구현한다.
	// 칸의 가로 길이를 400으로 조정
	ImGui::PushItemWidth(400.f);

	// 선택한 Index를 가진 불의 정보를 선택한다.
	ImGui::ListBox("2", &m_iCurrentFire_Index, FeatureMesh, (int)m_vFires[m_iCurrent_Standard_Object_Index].size(), 5);
	
	// 선택한 오브젝트 인덱스가 0번쨰이고, 현재 인덱스도 0번쨰 일 경우, 그러니깐 처음 생성한 경우
	if (m_iSelectFireObjectIndex == 0 && m_iCurrentFire_Index == 0) {

		if (!m_vFires[m_iCurrent_Standard_Object_Index].empty()) {
			if (nullptr == m_pSelected_FireObject) {
				// 처음 생성한 불의 정보를 선택한다.
				m_pSelected_FireObject = m_vFires[m_iCurrent_Standard_Object_Index].at(m_iSelectFireObjectIndex);
				Update_Selected_FireObject();
			}
		}
	}

	// 선택한 오브젝트 인덱스가 현재 인덱스와 다른 경우
	if (m_iSelectFireObjectIndex != m_iCurrentFire_Index) {
		m_iSelectFireObjectIndex = m_iCurrentFire_Index;

		if (!m_vFires[m_iCurrent_Standard_Object_Index].empty()) {
			m_pSelected_FireObject = m_vFires[m_iCurrent_Standard_Object_Index].at(m_iSelectFireObjectIndex);
			Update_Selected_FireObject();
		}
	}

	static string CurrIndexTag;

	if (nullptr != m_pSelected_FireObject) {
		CurrIndexTag.clear();
		m_pSelected_FireObject->Get_FireNumber();

		char szNumber[10];
		_itoa_s(m_pSelected_FireObject->Get_FireNumber(), szNumber, 10);
		CurrIndexTag.append(szNumber);

		CurrIndexTag.append(". ");

		wstring wstr{ TEXT("Prototype_GameObject_Fire") };
		string str{ wstr.begin(),wstr.end() };
		CurrIndexTag.append(str);
	}
	else
		CurrIndexTag = "None";

	char buff[MAX_PATH] = " ";
	sprintf_s(buff, sizeof(buff), "%s", CurrIndexTag.c_str());
	ImGui::Text("Select Feature");
	ImGui::Text("%s", buff);


	ImGui::NextColumn();

	// 불을 삭제할 경우
	if (ImGui::Button("Delete_Fire")) {
		if (!m_vFires[m_iCurrent_Standard_Object_Index].empty()) {
			m_vFires[m_iCurrent_Standard_Object_Index].at(m_iSelectFireObjectIndex)->Set_Dead(true);
			m_vFires[m_iCurrent_Standard_Object_Index].erase(m_vFires[m_iCurrent_Standard_Object_Index].begin() + m_iSelectFireObjectIndex);
			m_pSelected_FireObject = nullptr;
		}
	}



	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::Control_Select_FireObject(void)
{
	// 선택한 오브젝트가 없을 경우 nullptr
	if (nullptr == m_pSelected_FireObject)
		return;

	ImGUI_SettingMesh_Fire_Transform();
}

void CImGUI_Manager::Update_Selected_FireObject(void)
{
	// Transform 컴포넌트 얻어오기
	CTransform* pTransformCom = dynamic_cast<CTransform*>(m_pSelected_FireObject->Get_Component(TEXT("Com_Transform")));

	// Position 값 세팅
	XMStoreFloat3(&m_vPosition, pTransformCom->Get_State(CTransform::STATE_POSITION));

	// Scale 값 세팅
	m_vFire_Scale = pTransformCom->Get_Scale();

	m_str_Fire_PositionX = to_string(m_vPosition.x);
	m_str_Fire_PositionY = to_string(m_vPosition.y);
	m_str_Fire_PositionZ = to_string(m_vPosition.z);

	//m_str_RotationX = to_string(0);
	//m_str_RotationY = to_string(0);
	//m_str_RotationZ = to_string(0);

	//m_str_ScaleX = to_string(m_vScale.x);
	//m_str_ScaleY = to_string(m_vScale.y);
	//m_str_ScaleZ = to_string(m_vScale.z);
}

void CImGUI_Manager::ImGUI_SettingMesh_Fire_Transform(void)
{
	ImGui::NextColumn();

	ImGui::Text("Setting Select Fire Transform");
	ImGui::Text("Position");

	// 칸의 가로 길이를 65으로 조정
	ImGui::PushItemWidth(70.f);

	ImGUI_SettingMesh_Fire_Transform_Position();
	ImGUI_SettingMesh_Fire_Transform_Rotation();
	ImGUI_SettingMesh_Fire_Transform_Scale();

	ImGUI_SettingMesh_Fire_GlowStrength();
	ImGUI_SettingMesh_Fire_OriginalStrength();
	ImGUI_SettingMesh_Fire_FrameSpeed();
}

void CImGUI_Manager::ImGUI_SettingMesh_Fire_Transform_Position(void)
{
	// =================== Position X ===================
	char szPositionX[20]{};
	if (nullptr != m_pSelected_FireObject)
		strncpy_s(szPositionX, m_str_Fire_PositionX.c_str(), sizeof(szPositionX) - 1);
	ImGui::InputText("PosX", szPositionX, sizeof(szPositionX));
	m_str_Fire_PositionX = szPositionX;

	ImGui::SameLine();
	// =================== Position Y ===================
	char szPositionY[20]{};
	if (nullptr != m_pSelected_FireObject)
		strncpy_s(szPositionY, m_str_Fire_PositionY.c_str(), sizeof(szPositionY) - 1);
	ImGui::InputText("PosY", szPositionY, sizeof(szPositionY));
	m_str_Fire_PositionY = szPositionY;

	ImGui::SameLine();
	// =================== Position Z ===================
	char szPositionZ[20]{};
	if (nullptr != m_pSelected_FireObject)
		strncpy_s(szPositionZ, m_str_Fire_PositionZ.c_str(), sizeof(szPositionZ) - 1);
	ImGui::InputText("PosZ", szPositionZ, sizeof(szPositionZ));
	m_str_Fire_PositionZ = szPositionZ;

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();
	// =================== Position Button ===================
	if (ImGui::Button("T")) {
		// T버튼을 누른다면
		m_vFire_Position = _float3(stof(m_str_Fire_PositionX), stof(m_str_Fire_PositionY), stof(m_str_Fire_PositionZ));

		// 선택한 오브젝트가 존재한다면
		if (nullptr != m_pSelected_FireObject) {
			dynamic_cast<CTransform*>(m_pSelected_FireObject->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vFire_Position), 1.f));
		}
	}
}

void CImGUI_Manager::ImGUI_SettingMesh_Fire_Transform_Rotation(void)
{
	static _float RotX = 0;
	static _float RotY = 0;
	static _float RotZ = 0;


	if (nullptr != m_pSelected_FireObject) {
		_float3 Rot = m_pSelected_FireObject->Get_Rotation();
		RotX = Rot.x;
		RotY = Rot.y;
		RotZ = Rot.z;
	}

	// =================== Rotation X ===================
	ImGui::SliderFloat("RotX", &RotX, 0.f, 360.f);

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();
	// =================== Rotation Y ===================
	ImGui::SliderFloat("RotY", &RotY, 0.f, 360.f);

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();
	// =================== Rotation Z ===================
	ImGui::SliderFloat("RotZ", &RotZ, 0.f, 360.f);
	// =================== Rotation Button ===================

	// 선택한 오브젝트가 존재한다면
	if (nullptr != m_pSelected_FireObject) {
		// 회전 각도를 표시하지말고, 특정 버튼을 눌러 회전을 표현
		CTransform* pTransformCom = dynamic_cast<CTransform*>(m_pSelected_FireObject->Get_Component(TEXT("Com_Transform")));
		pTransformCom->RotationXYZ(XMConvertToRadians(RotX), XMConvertToRadians(RotY), XMConvertToRadians(RotZ));
		_float3 Rot{ RotX, RotY, RotZ };
		m_pSelected_FireObject->Set_Rotation(Rot);
	}
}

void CImGUI_Manager::ImGUI_SettingMesh_Fire_Transform_Scale(void)
{
	static _float Scale[3];


	if (nullptr != m_pSelected_FireObject) {
		_float3 fScale = m_pSelected_FireObject->Get_Scale();

		Scale[0] = fScale.x;
		Scale[1] = fScale.y;
		Scale[2] = fScale.z;

		//CTransform* pTransformCom = dynamic_cast<CTransform*>(m_pSelected_FireObject->Get_Component(TEXT("Com_Transform")));
		//Scale[0] = pTransformCom->Get_Scale().x;
		//Scale[1] = pTransformCom->Get_Scale().y;
		//Scale[2] = pTransformCom->Get_Scale().z;
	}


	ImGui::PushItemWidth(400.f);
	ImGui::SliderFloat3("Scale", Scale, 0.f, 10.f);


	if (Scale[0] == 0.f) {
		Scale[0] = 0.0001f;
	}
	if (Scale[1] == 0.f) {
		Scale[1] = 0.0001f;
	}
	if (Scale[2] == 0.f) {
		Scale[2] = 0.001f;
	}



	// 선택한 오브젝트가 존재한다면0.000001f
	if (nullptr != m_pSelected_FireObject) {
		// 회전 각도를 표시하지말고, 특정 버튼을 눌러 회전을 표현
		m_pSelected_FireObject->Set_Scale(_float3(Scale[0], Scale[1], Scale[2]));
	}
}

void CImGUI_Manager::ImGUI_SettingMesh_Fire_GlowStrength(void)
{
	if (nullptr == m_pSelected_FireObject)
		return;

	static _float fGlowStrength = m_pSelected_FireObject->Get_GlowStrength();
	ImGui::SliderFloat("Glow_Strength", &fGlowStrength, 0.f, 1.f);

	m_pSelected_FireObject->Set_GlowStrength(fGlowStrength);


}

void CImGUI_Manager::ImGUI_SettingMesh_Fire_OriginalStrength(void)
{
	if (nullptr == m_pSelected_FireObject)
		return;

	static _float fOriginalStrength = m_pSelected_FireObject->Get_OriginalStrength();
	ImGui::SliderFloat("Original_Strength", &fOriginalStrength, 0.f, 1.f);

	m_pSelected_FireObject->Set_OriginalStrength(fOriginalStrength);
}

void CImGUI_Manager::ImGUI_SettingMesh_Fire_FrameSpeed(void)
{
	if (nullptr == m_pSelected_FireObject)
		return;

	static _float fFrameSpeed = m_pSelected_FireObject->Get_FrameSpeed();
	ImGui::SliderFloat("FrameSpeed", &fFrameSpeed, 0.f, 100.f);

	m_pSelected_FireObject->Set_FrameSpeed(fFrameSpeed);
}

void CImGUI_Manager::ImGUI_SettingMesh_Fire_SaveLoad(void)
{
	// Save 버튼
	if (ImGui::Button("Save")) {
		
		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			SaveFireList("../Bin/Resources/Dat/Stage01/", "FireInfo.dat");
			break;
		case 1:
			SaveFireList("../Bin/Resources/Dat/Stage02/", "FireInfo.dat");
			break;
		case 2:
			SaveFireList("../Bin/Resources/Dat/Stage03/", "FireInfo.dat");
			break;
		case 3:
			SaveFireList("../Bin/Resources/Dat/Stage04/", "FireInfo.dat");
			break;
		case 4:
			SaveFireList("../Bin/Resources/Dat/Stage05/", "FireInfo.dat");
			break;
		case 5:
			SaveFireList("../Bin/Resources/Dat/Stage06/", "FireInfo.dat");
			break;
		case 6:
			SaveFireList("../Bin/Resources/Dat/Stage07/", "FireInfo.dat");
			break;
		case 7:
			SaveFireList("../Bin/Resources/Dat/Stage08/", "FireInfo.dat");
			break;
		case 8:
			SaveFireList("../Bin/Resources/Dat/Stage09/", "FireInfo.dat");
			break;
		case 9:
			SaveFireList("../Bin/Resources/Dat/Stage10/", "FireInfo.dat");
			break;
		default:
			break;
		}
	}

	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	if (ImGui::Button("Load")) {
		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			LoadFireList("../Bin/Resources/Dat/Stage01/", "FireInfo.dat");
			break;
		case 1:
			LoadFireList("../Bin/Resources/Dat/Stage02/", "FireInfo.dat");
			break;
		case 2:
			LoadFireList("../Bin/Resources/Dat/Stage03/", "FireInfo.dat");
			break;
		case 3:
			LoadFireList("../Bin/Resources/Dat/Stage04/", "FireInfo.dat");
			break;
		case 4:
			LoadFireList("../Bin/Resources/Dat/Stage05/", "FireInfo.dat");
			break;
		case 5:
			LoadFireList("../Bin/Resources/Dat/Stage06/", "FireInfo.dat");
			break;
		case 6:
			LoadFireList("../Bin/Resources/Dat/Stage07/", "FireInfo.dat");
			break;
		case 7:
			LoadFireList("../Bin/Resources/Dat/Stage08/", "FireInfo.dat");
			break;
		case 8:
			LoadFireList("../Bin/Resources/Dat/Stage09/", "FireInfo.dat");
			break;
		case 9:
			LoadFireList("../Bin/Resources/Dat/Stage10/", "FireInfo.dat");
			break;
		default:
			break;
		}

		// m_szMeshList 업데이트
		UpdateMeshList();
	}
}

HRESULT CImGUI_Manager::LoadFireList(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	while (true)
	{
		_uint iStandard_Object_Cnt = 0;
		ReadFile(hFile, &iStandard_Object_Cnt, sizeof(_uint), &dwByte, nullptr);

		for (_uint i = 0; i < iStandard_Object_Cnt; ++i) {
			// 문자열 메모리의 크기를 읽음
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			// 읽어온 메모리의 크기만큼 문자열을 할당하고, 해당 문자열을 읽어옴
			TCHAR*	pName = new TCHAR[dwStrByte];
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// 잘못됬을 경우 해당 메모리 삭제 및 멈추기
			if (0 == dwByte)
			{
				delete[] pName;
				break;
			}

			// 읽어온 오브젝트 생성
			CMapObject::MAPOBJDesc Desc;
			Desc.pPrototype_ObjectName = pName;

			XMStoreFloat4x4(&Desc.TransformMatrix, XMMatrixIdentity());

			// 충돌체 생성
			Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

			// 맵 오브젝트 생성
			CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Clone_Prototype(TEXT("Prototype_GameObject_MapObject"), &Desc));
			m_vFire_Standard_MapObjects.push_back(pMapObject);		// 맵 오브젝트 정보 푸쉬


			// 불 갯수 읽기
			_uint iFireCnt = 0;
			ReadFile(hFile, &iFireCnt, sizeof(_uint), &dwByte, nullptr);

			// 불의 갯수만큼 반복하여 불의 정보를 읽어오기
			CFire_Deco::FIREDECODESC FireDesc;
			for (_uint j = 0; j < iFireCnt; ++j) {
				ReadFile(hFile, &FireDesc, sizeof(CFire_Deco::FIREDECODESC), &dwByte, nullptr);

				// 문자열 메모리의 크기를 읽음
				ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

				// 읽어온 메모리의 크기만큼 문자열을 할당하고, 해당 문자열을 읽어옴
				TCHAR*	pName = new TCHAR[dwStrByte];
				ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

				// 잘못됬을 경우 해당 메모리 삭제 및 멈추기
				if (0 == dwByte)
				{
					delete[] pName;
					break;
				}

				FireDesc.tagTextureCom = pName;

				// 읽은 정보를 사용하여 불을 생성
				CFire_Deco* pFire = dynamic_cast<CFire_Deco*>(pGameInstance->Clone_Prototype(TEXT("Prototype_GameObject_Fire"), &FireDesc));
				m_vFires[i].push_back(pFire);
			}
		}

		break;
	}

	Update_Standard_MapObject_List();

	RELEASE_INSTANCE(CGameInstance);

	// 파일을 닫는다.
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CImGUI_Manager::SaveFireList(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	_ulong		dwByte = 0;
	DWORD		dwStrByte = 0;

	HANDLE		hFile = CreateFile(szCurPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;


	// 가지고 있는 m_vFire_Standard_MapObjects의 갯수 저장
	_uint iStandard_MapObjectCnt = m_vFire_Standard_MapObjects.size();
	WriteFile(hFile, &iStandard_MapObjectCnt, sizeof(_uint), &dwByte, nullptr);


	// m_vFire_Standard_MapObjects 를 돌며 맵 오브젝트의 tag를 저장하고, m_vFires의 특정 인덱스를 돌며 해당 불의 갯수 및 내용을 저장한다.
	_uint iCnt = 0;
	for (auto& Standard_MapObject : m_vFire_Standard_MapObjects) {

		const TCHAR* pModelTag = Standard_MapObject->Get_ModelTag();

		wstring wstr{ pModelTag };
		string str{ wstr.begin(), wstr.end() };

		// 문자열의 사이즈 구하기
		dwStrByte = sizeof(TCHAR) * ((int)wstr.size() + 1);

		// 해당 문자열의 사이즈 저장
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		// 문자열의 메모리를 구한 문자열의 사이즈만큼 저장
		WriteFile(hFile, wstr.c_str(), dwStrByte, &dwByte, nullptr);


		// 가지고 있는 불의 갯수 저장
		_uint iFireCnt = m_vFires[iCnt].size();
		WriteFile(hFile, &iFireCnt, sizeof(_uint), &dwByte, nullptr);

		for (auto& Fire : m_vFires[iCnt]) {
			CFire_Deco::FIREDECODESC FireDesc;
			FireDesc.fGlowStrength = Fire->Get_GlowStrength();
			FireDesc.fOriginalStrength = Fire->Get_OriginalStrength();
			FireDesc.fRotation = Fire->Get_Rotation();
			FireDesc.iFrameSpeed = Fire->Get_FrameSpeed();
			FireDesc.iShaderPass = 2;

			CTransform* pFireTransform = dynamic_cast<CTransform*>(Fire->Get_Component(TEXT("Com_Transform")));

			XMStoreFloat3(&FireDesc.vPosition, pFireTransform->Get_State(CTransform::STATE_POSITION));
			FireDesc.vScale = pFireTransform->Get_Scale();
			FireDesc.tagTextureCom = TEXT("Prototype_Component_Texture_Fire0");
			FireDesc.vLook = _float3(0.f, 0.f, 1.f);

			// 불을 생성하는데 필요한 정보 저장
			WriteFile(hFile, &FireDesc, sizeof(CFire_Deco::FIREDECODESC), &dwByte, nullptr);

			wstring wstr{ FireDesc.tagTextureCom };
			string str{ wstr.begin(), wstr.end() };

			// 문자열의 사이즈 구하기
			dwStrByte = sizeof(TCHAR) * ((int)wstr.size() + 1);

			// 해당 문자열의 사이즈 저장
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			// 문자열의 메모리를 구한 문자열의 사이즈만큼 저장
			WriteFile(hFile, wstr.c_str(), dwStrByte, &dwByte, nullptr);
		}

		++iCnt;
	}


	//
	


	// 오브젝트 및 불 상대 위치정보 저장
	//_float3		vPoint[3];
	//for (const auto& Dot : m_vDot) {
	//	_vector vPos = dynamic_cast<CTransform*>(Dot->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
	//	_float3 fPos;
	//	XMStoreFloat3(&fPos, vPos);

	//	WriteFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
	//}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CImGUI_Manager::LoadMapFireList(const char * pModelFilePath, const char * pModelFileName)
{
	// 파일을 읽어 오브젝트와 불의 정보를 가져온다.
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	vector<CMapObject*> vMapObjects;
	vector<CFire_Deco*> FireInfo[MAX_STANDARD_OBJECT_CNT];

	while (true)
	{
		_uint iStandard_Object_Cnt = 0;
		ReadFile(hFile, &iStandard_Object_Cnt, sizeof(_uint), &dwByte, nullptr);

		for (_uint i = 0; i < iStandard_Object_Cnt; ++i) {
			// 문자열 메모리의 크기를 읽음
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			// 읽어온 메모리의 크기만큼 문자열을 할당하고, 해당 문자열을 읽어옴
			TCHAR*	pName = new TCHAR[dwStrByte];
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// 잘못됬을 경우 해당 메모리 삭제 및 멈추기
			if (0 == dwByte)
			{
				delete[] pName;
				break;
			}

			// 읽어온 오브젝트 생성
			CMapObject::MAPOBJDesc Desc;
			Desc.pPrototype_ObjectName = pName;

			XMStoreFloat4x4(&Desc.TransformMatrix, XMMatrixIdentity());

			// 충돌체 생성
			Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

			// 맵 오브젝트 생성
			CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Clone_Prototype(TEXT("Prototype_GameObject_MapObject"), &Desc));
			vMapObjects.push_back(pMapObject);		// 맵 오브젝트 정보 푸쉬

			_uint iFireCnt = 0;		// 불 갯수 읽기
			ReadFile(hFile, &iFireCnt, sizeof(_uint), &dwByte, nullptr);

			// 불의 갯수만큼 반복하여 불의 정보를 읽어오기
			CFire_Deco::FIREDECODESC FireDesc;
			for (_uint j = 0; j < iFireCnt; ++j) {
				ReadFile(hFile, &FireDesc, sizeof(CFire_Deco::FIREDECODESC), &dwByte, nullptr);

				// 문자열 메모리의 크기를 읽음
				ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

				// 읽어온 메모리의 크기만큼 문자열을 할당하고, 해당 문자열을 읽어옴
				TCHAR*	pName = new TCHAR[dwStrByte];
				ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

				// 잘못됬을 경우 해당 메모리 삭제 및 멈추기
				if (0 == dwByte)
				{
					delete[] pName;
					break;
				}

				FireDesc.tagTextureCom = pName;

				// 읽은 정보를 사용하여 불을 생성
				CFire_Deco* pFire = dynamic_cast<CFire_Deco*>(pGameInstance->Clone_Prototype(TEXT("Prototype_GameObject_Fire"), &FireDesc));
				FireInfo[i].push_back(pFire);
			}
		}

		break;
	}

	Update_Standard_MapObject_List();

	RELEASE_INSTANCE(CGameInstance);

	// 파일을 닫는다.
	CloseHandle(hFile);
	




	// 읽은 불 정보를 맵에 배치되어 있는 오브젝트를 돌며 관련 오브젝트가 존재할 경우 해당 오브젝트에 불을 생성한다.

	pGameInstance = GET_INSTANCE(CGameInstance);

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects);

	if (nullptr == pLayer)
		return E_FAIL;

	auto& Objlist = pLayer->Get_ObjectList();

	_uint iMapObjectCnt = 0;
	for (auto& MapObject : vMapObjects) {
		// 파일에서 가져온 모델 태그 정보
		const TCHAR* pMapObjectModelTag = dynamic_cast<CMapObject*>(MapObject)->Get_ModelTag();

		// 맵에 설치된 오브젝트 리스트 정보
		for (auto& Obj : Objlist) {
			// 설치된 오브젝트의 모델 Tag 정보를 가져온다.
			const TCHAR* pObjModelTag = dynamic_cast<CMapObject*>(Obj)->Get_ModelTag();

			// 가져온 모델의 이름과 파일에서 가져온 모델 태그 정보를 비교한다.
			if (!_tcscmp(pMapObjectModelTag, pObjModelTag)) {
				// 만약 같은 정보가 존재할 경우
				// 해당 오브젝트가 가지고 있는 불 정보를 가져와 맵에 설치된 오브젝트의 위치에 소환한다.

				// 맵에 설치되어 있는 오브젝트의 행렬 정보를 가져온다.
				CTransform* pTransform = dynamic_cast<CTransform*>(Obj->Get_Component(TEXT("Com_Transform")));
				_matrix matMapObjectWorld = pTransform->Get_WorldMatrix();

				for (auto& Fire : FireInfo[iMapObjectCnt]) {
					CFire_Deco::FIREDECODESC FireDesc;
					FireDesc.fGlowStrength = Fire->Get_GlowStrength();
					FireDesc.fOriginalStrength = Fire->Get_OriginalStrength();
					FireDesc.fRotation = Fire->Get_Rotation();
					FireDesc.iFrameSpeed = Fire->Get_FrameSpeed();
					FireDesc.iShaderPass = 2;

					CTransform* pFireTransform = dynamic_cast<CTransform*>(Fire->Get_Component(TEXT("Com_Transform")));

					XMStoreFloat3(&FireDesc.vPosition, pFireTransform->Get_State(CTransform::STATE_POSITION));
					FireDesc.vScale = pFireTransform->Get_Scale();
					FireDesc.tagTextureCom = TEXT("Prototype_Component_Texture_Fire0");
					FireDesc.vLook = _float3(0.f, 0.f, 1.f);

					// 불을 생성하는데 필요한 정보 저장
					WriteFile(hFile, &FireDesc, sizeof(CFire_Deco::FIREDECODESC), &dwByte, nullptr);

					CFire_Deco* pFire = dynamic_cast<CFire_Deco*>(pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_Fire"), &FireDesc));
					pFire->Set_Target(m_pCurrCamera);
					CTransform* pFire_Transform = dynamic_cast<CTransform*>(pFire->Get_Component(TEXT("Com_Transform")));
					_matrix matFireWorld = pFire_Transform->Get_WorldMatrix();

					matFireWorld = XMMatrixMultiply(matFireWorld, matMapObjectWorld);
					pFire_Transform->Set_WorldMatrix(matFireWorld);

					_float4x4 matFireWorld4X4;
					XMStoreFloat4x4(&matFireWorld4X4, matFireWorld);


					//// 불이 생성되는 위치에 점광원 생성
					//LIGHTDESC LightDesc;
					//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

					//LightDesc.eType = LIGHTDESC::TYPE_POINT;
					//LightDesc.vPosition = _float4(matFireWorld4X4._41, matFireWorld4X4._42, matFireWorld4X4._43, 1.f);
					//LightDesc.fRange = 4.f;
					//LightDesc.vDiffuse = _float4(1.f, 0.8314f, 0.0f, 1.f);
					//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
					//LightDesc.vSpecular = _float4(0.f, 0.0f, 0.0f, 1.f);
					//LightDesc.fPower = 10.f;

					//pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc);
				}
			}
			

		}
		++iMapObjectCnt;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CImGUI_Manager::Update_Standard_MapObject_List(void)
{
	_uint iCnt = 0;
	for (auto& Standard_Object : m_vFire_Standard_MapObjects) {
		// 문자열 추출
		wstring wStr(Standard_Object->Get_ModelTag());

		// 문자열의 사이즈 추출
		size_t size = (wcslen(wStr.c_str()) + 1) * sizeof(TCHAR);

		// m_szMeshList에 문자열을 추출된 문자열의 사이즈만큼 할당 
		size_t convertedSize;
		wcstombs_s(&convertedSize, m_szStandard_Object_List[iCnt++], size, wStr.c_str(), size);
	}
}

HRESULT CImGUI_Manager::Save_DotInfo(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(szCurPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;

	_float3		vPoint[3];
	for (const auto& Dot : m_vDot) {
		_vector vPos = dynamic_cast<CTransform*>(Dot->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
		_float3 fPos;
		XMStoreFloat3(&fPos, vPos);

		WriteFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CImGUI_Manager::Load_DotInfo(const char * pModelFilePath, const char * pModelFileName)
{
	for (auto& Dot : m_vDot) {
		Safe_Release(Dot);
	}
	m_vDot.clear();

	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;


	CDot::DotDesc Desc;
	while (true)
	{
		ReadFile(hFile, &Desc.fPos, sizeof(_float3), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// 점 오브젝트 생성
		CDot* pDot = dynamic_cast<CDot*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_Dot, TEXT("Prototype_GameObject_Dot"), &Desc));
		m_vDot.push_back(pDot);

		RELEASE_INSTANCE(CGameInstance);
	}

	// 파일을 닫는다.
	CloseHandle(hFile);

	return S_OK;
}

void CImGUI_Manager::Render_Cells(void)
{
	if (nullptr == m_pVIBuffer)
	{
		MSG_BOX(L"Failed To CNavigation : Render : nullptr == m_pVIBuffer");
		return;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	_uint iIndex = 0;

	for (auto& pCell : m_vCell)
	{
		if (nullptr != pCell)
		{
			;
#ifdef _DEBUG
			pCell->Render(m_pVIBuffer, m_pShader, pCell == m_pCell ? _float4(1.f, 0.f, 0.f, 1.f) : _float4(1.f, 1.f, 1.f, 1.f));
#endif // _DEBUG
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CImGUI_Manager::UpdateHeightMapList(void)
{
	_uint iCnt = 0;
	for (auto& MapObject : m_HeightMapList) {

		// 문자열의 사이즈 추출
		size_t size = (wcslen(MapObject.c_str()) + 1) * sizeof(TCHAR);

		// m_szMeshList에 문자열을 추출된 문자열의 사이즈만큼 할당 
		size_t convertedSize;
		wcstombs_s(&convertedSize, m_szHeightMapList[iCnt++], size, MapObject.c_str(), size);
	}
}

void CImGUI_Manager::ImGUI_SettingMesh_Mesh(void)
{
	// 현재 list에 있는 mesh 갯수 출력
	ImGui::Text("Mesh list - loaded [ %d ] Meshes", m_strMeshList.size());

	// 칸의 가로 길이를 150으로 조정
	ImGui::PushItemWidth(150.f);

	ImVec2 ListBoxSize = ImVec2(480.f, 250);
	static _int iCurrIndex = -1;
	if (ImGui::BeginListBox(" ", ListBoxSize)) {

		_uint iCnt = 0;
		static const char* Selected_IndexTag = " ";
		for (auto& strName : m_strMeshList) {
			// wstring 형 문자열을 string 형으로 변환
			string str(strName.begin(), strName.end());

			// 선택된 문자열과 현재 인덱스의 문자열이 같다면 true 아니면 false 대입
			_bool is_selected = (Selected_IndexTag == str);

			// 인덱스 확인
			if (ImGui::Selectable(m_szMeshList[iCnt], is_selected)) {
				Selected_IndexTag = m_szMeshList[iCnt];
				static basic_string<TCHAR> tStr;
				tStr = basic_string<TCHAR>(strName.begin(), strName.end());

				m_pPrototypeTag_SelectedMesh = tStr.c_str();
				iCurrIndex = iCnt;

				m_bSelectMesh = false;
				m_pSelectedFeatureObject = nullptr;
			}

			++iCnt;
		}

		ImGui::EndListBox();
	}

	static string CurrIndexTag;

	if (iCurrIndex >= 0) {
		string s(m_strMeshList.at(iCurrIndex).begin(), m_strMeshList.at(iCurrIndex).end());
		CurrIndexTag = s;
	}
	else
		CurrIndexTag = "None";

	char buff[MAX_PATH] = " ";
	sprintf_s(buff, sizeof(buff), "%s", CurrIndexTag.c_str());
	ImGui::Text("Select Mesh");
	ImGui::Text("%s", buff);
}

void CImGUI_Manager::ImGUI_SettingMesh_Feature(void)
{
	// 현재 list에 있는 mesh 갯수 출력
	ImGui::Text("Feature list - placed [ %d ] Meshes", m_PlacedMapObjectList.size());

	const char* FeatureMesh[]{
		"000","001","002","003","004","005","006","007","008","009",
		"010","011","012","013","014","015","016","017","018","019",
		"020","021","022","023","024","025","026","027","028","029",
		"030","031","032","033","034","035","036","037","038","039",
		"040","041","042","043","044","045","046","047","048","049",
		"050","051","052","053","054","055","056","057","058","059",
		"060","061","062","063","064","065","066","067","068","069",
		"070","071","072","073","074","075","076","077","078","079",
		"080","081","082","083","084","085","086","087","088","089",
		"090","091","092","093","094","095","096","097","098","099",

		"100","101","102","103","104","105","106","107","108","109",
		"110","111","112","113","114","115","116","117","118","119",
		"120","121","122","123","124","125","126","127","128","129",
		"130","131","132","133","134","135","136","137","138","139",
		"140","141","142","143","144","145","146","147","148","149",
		"150","151","152","153","154","155","156","157","158","159",
		"160","161","162","163","164","165","166","167","168","169",
		"170","171","172","173","174","175","176","177","178","179",
		"180","181","182","183","184","185","186","187","188","189",
		"190","191","192","193","194","195","196","197","198","199",

		"200","201","202","203","204","205","206","207","208","209",
		"210","211","212","213","214","215","216","217","218","219",
		"220","221","222","223","224","225","226","227","228","229",
		"230","231","232","233","234","235","236","237","238","239",
		"240","241","242","243","244","245","246","247","248","249",
		"250","251","252","253","254","255","256","257","258","259",
		"260","261","262","263","264","265","266","267","268","269",
		"270","271","272","273","274","275","276","277","278","279",
		"280","281","282","283","284","285","286","287","288","289",
		"290","291","292","293","294","295","296","297","298","299",

		"300","301","302","303","304","305","306","307","308","309",
		"310","311","312","313","314","315","316","317","318","319",
		"320","321","322","323","324","325","326","327","328","329",
		"330","331","332","333","334","335","336","337","338","339",
		"340","341","342","343","344","345","346","347","348","349",
		"350","351","352","353","354","355","356","357","358","359",
		"360","361","362","363","364","365","366","367","368","369",
		"370","371","372","373","374","375","376","377","378","379",
		"380","381","382","383","384","385","386","387","388","389",
		"390","391","392","393","394","395","396","397","398","399",

		"400","401","402","403","404","405","406","407","408","409",
		"410","411","412","413","414","415","416","417","418","419",
		"420","421","422","423","424","425","426","427","428","429",
		"430","431","432","433","434","435","436","437","438","439",
		"440","441","442","443","444","445","446","447","448","449",
		"450","451","452","453","454","455","456","457","458","459",
		"460","461","462","463","464","465","466","467","468","469",
		"470","471","472","473","474","475","476","477","478","479",
		"480","481","482","483","484","485","486","487","488","489",
		"490","491","492","493","494","495","496","497","498","499"
	};

	// 칸의 가로 길이를 300으로 조정
	ImGui::PushItemWidth(400.f);
	ImGui::ListBox("1", &m_iCurrent_Index, FeatureMesh, (int)m_PlacedMapObjectList.size(), 5);
	if (m_iSelectFeatureObjectIndex == 0 && m_iCurrent_Index == 0) {
		m_iSelectFeatureObjectIndex = m_iCurrent_Index;
		m_bSelectMesh = true;

		if (!m_PlacedMapObjectList.empty()) {
			if (nullptr == m_pSelectedFeatureObject) {
				m_pSelectedFeatureObject = m_PlacedMapObjectList.at(m_iSelectFeatureObjectIndex);
				Update_SelectedFeature();
			}
		}
	}
	if (m_iSelectFeatureObjectIndex != m_iCurrent_Index) {
		m_iSelectFeatureObjectIndex = m_iCurrent_Index;
		m_bSelectMesh = true;

		if (!m_PlacedMapObjectList.empty()) {
			m_pSelectedFeatureObject = m_PlacedMapObjectList.at(m_iSelectFeatureObjectIndex);

			Update_SelectedFeature();
		}
	}

	static string CurrIndexTag;

	if (nullptr != m_pSelectedFeatureObject) {
		CurrIndexTag.clear();
		m_pSelectedFeatureObject->Get_FeatureNumber();

		char szNumber[10];
		_itoa_s(m_pSelectedFeatureObject->Get_FeatureNumber(), szNumber, 10);
		CurrIndexTag.append(szNumber);

		CurrIndexTag.append(". ");

		wstring wstr{ m_pSelectedFeatureObject->Get_ModelTag() };
		string str{ wstr.begin(),wstr.end() };
		CurrIndexTag.append(str);
	}
	else
		CurrIndexTag = "None";

	char buff[MAX_PATH] = " ";
	sprintf_s(buff, sizeof(buff), "%s", CurrIndexTag.c_str());
	ImGui::Text("Select Feature");
	ImGui::Text("%s", buff);

	// 삭제 버튼
	if (ImGui::Button("Delete")) {
		// 선택된 오브젝트가 존재할 경우
		if (nullptr != m_pSelectedFeatureObject) {
			// 해당 오브젝트를 m_PlacedMapObjectList에서 검색한다.
			auto& iter = find_if(m_PlacedMapObjectList.begin(), m_PlacedMapObjectList.end(), [&](const CMapObject* pObject) {
				return m_pSelectedFeatureObject == pObject;
			});

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			auto& ObjList = pGameInstance->Find_Layer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects)->Get_ObjectList();
			auto& Objlist_iter = find_if(ObjList.begin(), ObjList.end(), [&](CGameObject* pObject) {
				return m_pSelectedFeatureObject == dynamic_cast<CMapObject*>(pObject);
			});

			// 찾는 오브젝트가 존재할 경우
			if (Objlist_iter != ObjList.end()) {
				ObjList.erase(Objlist_iter);
			}

			RELEASE_INSTANCE(CGameInstance);

			// 검색된 오브젝트 삭제
			m_PlacedMapObjectList.erase(iter);

			UpdateFeatureList();

			m_pSelectedFeatureObject = nullptr;
		}
	}
}

void CImGUI_Manager::ImGUI_SettingMesh_Transform(void)
{
	ImGui::Text("Setting Select Mesh Transform");
	ImGui::Text("Position");

	// 칸의 가로 길이를 65으로 조정
	ImGui::PushItemWidth(70.f);

	ImGUI_SettingMesh_Transform_Position();
	ImGUI_SettingMesh_Transform_Rotation();
	ImGUI_SettingMesh_Transform_Scale();
}

void CImGUI_Manager::ImGUI_SettingMesh_SaveLoad(void)
{
	// Save 버튼
	if (ImGui::Button("Save")) {


		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			SaveMeshList("../Bin/Resources/Dat/Stage01/", "MeshList.dat");
			break;
		case 1:
			SaveMeshList("../Bin/Resources/Dat/Stage02/", "MeshList.dat");
			break;
		case 2:
			SaveMeshList("../Bin/Resources/Dat/Stage03/", "MeshList.dat");
			break;
		case 3:
			SaveMeshList("../Bin/Resources/Dat/Stage04/", "MeshList.dat");
			break;
		case 4:
			SaveMeshList("../Bin/Resources/Dat/Stage05/", "MeshList.dat");
			break;
		case 5:
			SaveMeshList("../Bin/Resources/Dat/Stage06/", "MeshList.dat");
			break;
		case 6:
			SaveMeshList("../Bin/Resources/Dat/Stage07/", "MeshList.dat");
			break;
		case 7:
			SaveMeshList("../Bin/Resources/Dat/Stage08/", "MeshList.dat");
			break;
		case 8:
			SaveMeshList("../Bin/Resources/Dat/Stage09/", "MeshList.dat");
			break;
		case 9:
			SaveMeshList("../Bin/Resources/Dat/Stage10/", "MeshList.dat");
			break;
		default:
			break;
		}
	}

	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	if (ImGui::Button("Load")) {
		// 현재 스테이지에 따라 다른 경로로 저장
		switch (m_eCurStageLevel)
		{
		case 0:
			LoadMeshList("../Bin/Resources/Dat/Stage01/", "MeshList.dat");
			
			break;
		case 1:
			LoadMeshList("../Bin/Resources/Dat/Stage02/", "MeshList.dat");
			break;
		case 2:
			LoadMeshList("../Bin/Resources/Dat/Stage03/", "MeshList.dat");
			LoadMapFireList("../Bin/Resources/Dat/Stage03/", "FireInfo.dat");
			break;
		case 3:
			LoadMeshList("../Bin/Resources/Dat/Stage04/", "MeshList.dat");
			break;
		case 4:
			LoadMeshList("../Bin/Resources/Dat/Stage05/", "MeshList.dat");
			break;
		case 5:
			LoadMeshList("../Bin/Resources/Dat/Stage06/", "MeshList.dat");
			break;
		case 6:
			LoadMeshList("../Bin/Resources/Dat/Stage07/", "MeshList.dat");
			break;
		case 7:
			LoadMeshList("../Bin/Resources/Dat/Stage08/", "MeshList.dat");
			break;
		case 8:
			LoadMeshList("../Bin/Resources/Dat/Stage09/", "MeshList.dat");
			break;
		case 9:
			LoadMeshList("../Bin/Resources/Dat/Stage10/", "MeshList.dat");
			break;
		default:
			break;
		}

		// m_szMeshList 업데이트
		UpdateMeshList();
	}
}

void CImGUI_Manager::ImGUI_SettingMesh_PickingMesh(void)
{	
	// Save 버튼
	if (ImGui::Button("Picking Mesh")) {
		m_pPickingMeshObject = m_pSelectedFeatureObject;
	}

	static string CurrIndexTag;

	if (nullptr != m_pPickingMeshObject) {
		wstring wstr{ m_pPickingMeshObject->Get_ModelTag() };
		CurrIndexTag = string(wstr.begin(), wstr.end());
	}
	else
		CurrIndexTag = "None";

	char buff[MAX_PATH] = " ";
	sprintf_s(buff, sizeof(buff), "%s", CurrIndexTag.c_str());
	ImGui::Text("Picking Object Name");
	ImGui::Text("%s", buff);
	
}

void CImGUI_Manager::Update_Transform_Position(void)
{
	// 선택된 오브젝트가 존재할 경우, 해당 오브젝트를 선택하거나 움직일 때만 갱신하도록 한다.
	if (nullptr != m_pSelectedFeatureObject) {
		_vector vPos = dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
		_float3 fPos;
		XMStoreFloat3(&fPos, vPos);

		m_str_PositionX = to_string(fPos.x);
		m_str_PositionY = to_string(fPos.y);
		m_str_PositionZ = to_string(fPos.z);
	};
}

void CImGUI_Manager::ImGUI_SettingMesh_Transform_Position(void)
{
	// =================== Position X ===================
	char szPositionX[20]{};
	if (nullptr != m_pSelectedFeatureObject)
		strncpy_s(szPositionX, m_str_PositionX.c_str(), sizeof(szPositionX) - 1);
	ImGui::InputText("PosX", szPositionX, sizeof(szPositionX));
	m_str_PositionX = szPositionX;

	ImGui::SameLine();
	// =================== Position Y ===================
	char szPositionY[20]{};
	if (nullptr != m_pSelectedFeatureObject)
		strncpy_s(szPositionY, m_str_PositionY.c_str(), sizeof(szPositionY) - 1);
	ImGui::InputText("PosY", szPositionY, sizeof(szPositionY));
	m_str_PositionY = szPositionY;

	ImGui::SameLine();
	// =================== Position Z ===================
	char szPositionZ[20]{};
	if (nullptr != m_pSelectedFeatureObject)
		strncpy_s(szPositionZ, m_str_PositionZ.c_str(), sizeof(szPositionZ) - 1);
	ImGui::InputText("PosZ", szPositionZ, sizeof(szPositionZ));
	m_str_PositionZ = szPositionZ;

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();
	// =================== Position Button ===================
	if (ImGui::Button("T")) {
		// T버튼을 누른다면
		m_vPosition = _float3(stof(m_str_PositionX), stof(m_str_PositionY), stof(m_str_PositionZ));

		// 선택한 오브젝트가 존재한다면
		if (nullptr != m_pSelectedFeatureObject) {
			dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
		}
	}
}

void CImGUI_Manager::ImGUI_SettingMesh_Transform_Rotation(void)
{
	static _float RotX = 0;
	static _float RotY = 0;
	static _float RotZ = 0;


	if (nullptr != m_pSelectedFeatureObject) {
		_float3 Rot = m_pSelectedFeatureObject->Get_Rotation();
		RotX = Rot.x;
		RotY = Rot.y;
		RotZ = Rot.z;
	}

	// =================== Rotation X ===================
	ImGui::SliderFloat("RotX", &RotX, 0.f, 360.f);

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();
	// =================== Rotation Y ===================
	ImGui::SliderFloat("RotY", &RotY, 0.f, 360.f);

	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();
	// =================== Rotation Z ===================
	ImGui::SliderFloat("RotZ", &RotZ, 0.f, 360.f);
	// =================== Rotation Button ===================

	// 선택한 오브젝트가 존재한다면
	if (nullptr != m_pSelectedFeatureObject) {
		// 회전 각도를 표시하지말고, 특정 버튼을 눌러 회전을 표현
		CTransform* pTransformCom = dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")));
		pTransformCom->RotationXYZ(XMConvertToRadians(RotX), XMConvertToRadians(RotY), XMConvertToRadians(RotZ));
		_float3 Rot{ RotX, RotY, RotZ };
		m_pSelectedFeatureObject->Set_Rotation(Rot);
	}
}

void CImGUI_Manager::ImGUI_SettingMesh_Transform_Scale(void)
{
	static _float Scale = 0;


	if (nullptr != m_pSelectedFeatureObject) {
		CTransform* pTransformCom = dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")));
		Scale = pTransformCom->Get_Scale().x;
	}


	ImGui::SliderFloat("Scale", &Scale, 0.f, 3.f);


	if (Scale == 0.f) {
		Scale = 0.00000000001f;
	}



	// 선택한 오브젝트가 존재한다면
	if (nullptr != m_pSelectedFeatureObject) {
		// 회전 각도를 표시하지말고, 특정 버튼을 눌러 회전을 표현
		CTransform* pTransformCom = dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")));
		pTransformCom->Scaled(_float3(Scale, Scale, Scale));
	}
}

void CImGUI_Manager::Settings(void)
{
	// 사이즈 설정
	SetSize();

	// 색상 설정
	Colors();
}

void CImGUI_Manager::Colors(void)
{
	ImGuiStyle& Style = ImGui::GetStyle();

	// 배경화면의 색상을 바꾼다.
	Style.Colors[ImGuiCol_WindowBg] = ImColor(0.f, 0.f, 0.f);
	Style.Colors[ImGuiCol_ChildBg] = ImColor(0.f, 0.f, 0.f);

	Style.Colors[ImGuiCol_Text] = ImColor(1.f, 1.f, 1.f);
	Style.Colors[ImGuiCol_CheckMark] = ImColor(1.f, 1.f, 1.f);
}

void CImGUI_Manager::SetSize(void)
{
	ImGuiStyle& Style = ImGui::GetStyle();
	//Style.WindowMinSize = ImVec2(350.f, 500.f);
}

HRESULT CImGUI_Manager::Save_NaviCell(const TCHAR * pNaviDataFilePath, const vector<CCell*>& vCells)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(pNaviDataFilePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;

	_float3		vPoint[3];
	for (const auto& Cell : vCells) {
		memcpy(vPoint, Cell->Get_Points(), sizeof(_float3) * 3);

		WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);
	}

	CloseHandle(hFile);
	return S_OK;
}

_bool CImGUI_Manager::Check_SameCell(_float3 * pPoints)
{
	_bool bCreate = true;
	for (auto& Cell : m_vCell) {
		if (Check_Cell_Point(pPoints[0], Cell) && Check_Cell_Point(pPoints[1], Cell) && Check_Cell_Point(pPoints[2], Cell)) {
			bCreate = false;
			break;
		}
	}

	return bCreate;
}

_bool CImGUI_Manager::Check_Cell_Point(_float3 & pPoint, CCell * pCell)
{
	_bool bPoint = false;

	_float3* pPoints = pCell->Get_Points();

	for (int i = 0; i < 3; ++i) {
		if (
			pPoints[i].x == pPoint.x &&
			pPoints[i].y == pPoint.y &&
			pPoints[i].z == pPoint.z
			) {
			bPoint = true;
			break;
		}
	}

	return bPoint;
}

void CImGUI_Manager::Update_SelectedFeature(void)
{
	// Transform 컴포넌트 얻어오기
	CTransform* pTransformCom = dynamic_cast<CTransform*>(m_pSelectedFeatureObject->Get_Component(TEXT("Com_Transform")));

	// Position 값 세팅
	XMStoreFloat3(&m_vPosition, pTransformCom->Get_State(CTransform::STATE_POSITION));

	// Scale 값 세팅
	m_vScale = pTransformCom->Get_Scale();

	m_str_PositionX = to_string(m_vPosition.x);
	m_str_PositionY = to_string(m_vPosition.y);
	m_str_PositionZ = to_string(m_vPosition.z);

	//m_str_RotationX = to_string(0);
	//m_str_RotationY = to_string(0);
	//m_str_RotationZ = to_string(0);

	//m_str_ScaleX = to_string(m_vScale.x);
	//m_str_ScaleY = to_string(m_vScale.y);
	//m_str_ScaleZ = to_string(m_vScale.z);
}

void CImGUI_Manager::PushMeshList(const TCHAR * pPrototypeTag)
{
	m_strMeshList.push_back(pPrototypeTag);

	UpdateMeshList();
}

void CImGUI_Manager::UpdateMeshList(void)
{
	_uint iCnt = 0;
	for (auto& strName : m_strMeshList) {
		// 문자열의 사이즈 추출
		size_t size = (wcslen(strName.c_str()) + 1) * sizeof(TCHAR);

		// m_szMeshList에 문자열을 추출된 문자열의 사이즈만큼 할당 
		size_t convertedSize;
		wcstombs_s(&convertedSize, m_szMeshList[iCnt++], size, strName.c_str(), size);
	}
}

void CImGUI_Manager::UpdateFeatureList(void)
{
	_uint iCnt = 0;
	for (auto& MapObject : m_PlacedMapObjectList) {
		if (nullptr != MapObject) {
			wstring wstr{ MapObject->Get_ModelTag() };

			// 문자열의 사이즈 추출
			size_t size = (wcslen(wstr.c_str()) + 1) * sizeof(TCHAR);

			// m_szMeshList에 문자열을 추출된 문자열의 사이즈만큼 할당 
			size_t convertedSize;
			wcstombs_s(&convertedSize, m_szFeatureList[iCnt++], size, wstr.c_str(), size);
		}
	}
}

void CImGUI_Manager::LoadMeshList(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	m_PlacedMapObjectList.clear();

	static _uint iCnt = 0;

	while (true)
	{
		// 문자열 메모리의 크기를 읽음
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		// 읽어온 메모리의 크기만큼 문자열을 할당하고, 해당 문자열을 읽어옴
		TCHAR*	pName = new TCHAR[dwStrByte];
		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

		// 잘못됬을 경우 해당 메모리 삭제 및 멈추기
		if (0 == dwByte)
		{
			delete[] pName;
			break;
		}

		// 만약 설치될 오브젝트가 부엉이라면
		if (
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_001"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_002"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_003"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_004"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_005"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_006"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_007"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_008"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_009"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_010"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_011"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_012"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_013"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_014"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_015"), pName) ||
			!_tcscmp(TEXT("Prototype_Component_Model_Owl_016"), pName)
			) {

			CMapObject::MAPOBJDesc Desc;

			// Desc에 정보 넣기
			Desc.pPrototype_ObjectName = pName;

			// Collider 정보 설정
			Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

			// 행렬 정보를 설정
			ReadFile(hFile, &Desc.TransformMatrix, sizeof(_float4x4), &dwByte, nullptr);

			// 맵 여부 확인
			ReadFile(hFile, &Desc.m_bMap, sizeof(_bool), &dwByte, nullptr);


			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			// 맵 오브젝트 생성
			CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
			m_PlacedMapObjectList.push_back(pMapObject);

			if (Desc.m_bMap) {
				if (nullptr == m_pPickingMeshObject)
					m_pPickingMeshObject = pMapObject;
			}

		}
		else {
			CMapObject::MAPOBJDesc Desc;

			// Desc에 정보 넣기
			Desc.pPrototype_ObjectName = pName;

			// Collider 정보 설정
			Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

			// 행렬 정보를 설정
			ReadFile(hFile, &Desc.TransformMatrix, sizeof(_float4x4), &dwByte, nullptr);

			// 맵 여부 확인
			ReadFile(hFile, &Desc.m_bMap, sizeof(_bool), &dwByte, nullptr);


			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			if (iCnt == 60) {
				int a = 10;
			}

			// 맵 오브젝트 생성
			CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_MapTool, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));

			if (nullptr == pMapObject)
				break;

			m_PlacedMapObjectList.push_back(pMapObject);

			if (Desc.m_bMap) {
				if (nullptr == m_pPickingMeshObject)
					m_pPickingMeshObject = pMapObject;
			}

			++iCnt;






			//// 광원 생성
			//if (
			//	!_tcscmp(TEXT("Prototype_Component_Model_BrokenHouse02_B"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_BrokenHouse02_C"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse02"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse03"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse05"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse06"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse07"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse08"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse09"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse10"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse11"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_DRTHouse12"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_BrokenHouse01_A"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_BrokenHouse01_B"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_BrokenHouse01_C"), pName) ||
			//	!_tcscmp(TEXT("Prototype_Component_Model_BrokenHouse02_A"), pName)
			//	) {
			//	// 집이라면 해당 집 오브젝트 위에 광원을 생성한다.
			//	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			//	// 점광원 생성
			//	LIGHTDESC LightDesc;
			//	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

			//	LightDesc.eType = tagLightDesc::TYPE_POINT;
			//	LightDesc.vPosition = _float4(Desc.TransformMatrix._41, Desc.TransformMatrix._42 + 4.5f, Desc.TransformMatrix._43, 1.f);
			//	LightDesc.fRange = 12.f;
			//	LightDesc.vDiffuse = _float4(0.59f, 0.29f, 0.f, 1.f);
			//	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
			//	LightDesc.vSpecular = _float4(0.f, 0.0f, 0.0f, 1.f);
			//	LightDesc.fPower = 10.f;

			//	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
			//		return;

			//	RELEASE_INSTANCE(CGameInstance);
			//}


		}

		UpdateFeatureList();

		RELEASE_INSTANCE(CGameInstance);
	}

	// 파일을 닫는다.
	CloseHandle(hFile);

	// 배치된 오브젝트 리스트 업데이트
	UpdateFeatureList();
}

void CImGUI_Manager::SaveMeshList(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	for (auto& MapObject : m_PlacedMapObjectList) {
		const TCHAR* pModelTag = MapObject->Get_ModelTag();

		wstring wstr{ pModelTag };
		string str{ wstr.begin(), wstr.end() };

		// 문자열의 사이즈 구하기
		dwStrByte = sizeof(TCHAR) * ((int)wstr.size() + 1);

		// 해당 문자열의 사이즈 저장
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		// 문자열의 메모리를 구한 문자열의 사이즈만큼 저장
		WriteFile(hFile, wstr.c_str(), dwStrByte, &dwByte, nullptr);

		// 오브젝트의 위치, 회전 값, 크기 값 저장

		// 월드 행렬 구하기
		_float4x4& MatWorld = dynamic_cast<CTransform*>(MapObject->Get_Component(TEXT("Com_Transform")))->Get_WorldFloat4x4();

		// 문자열의 메모리를 구한 문자열의 사이즈만큼 저장
		WriteFile(hFile, &MatWorld, sizeof(_float4x4), &dwByte, nullptr);

		// 맵 여부 저장
		_bool isMap = MapObject->isMap();
		WriteFile(hFile, &isMap, sizeof(_bool), &dwByte, nullptr);
	}

	// 파일을 닫는다.
	CloseHandle(hFile);
}

void CImGUI_Manager::LoadCellList(const char * pModelFilePath, const char * pModelFileName)
{
	for (auto& Cell : m_vCell) {
		Safe_Release(Cell);
	}
	m_vCell.clear();


	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	_float3 fPos[3];
	ZeroMemory(fPos, sizeof(_float3) * 3);

	while (true)
	{
		for (int i = 0; i < 3; ++i) {
			ReadFile(hFile, &fPos[i], sizeof(_float3), &dwByte, nullptr);
			if (0 == dwByte)
				break;
		}

		if (0 == dwByte)
			break;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// 셀 생성
		m_pCell = CCell::Create(m_pDevice, m_pDeviceContext, fPos);
		m_vCell.push_back(m_pCell);

		RELEASE_INSTANCE(CGameInstance);
	}

	// 파일을 닫는다.
	CloseHandle(hFile);
}

void CImGUI_Manager::SaveCellList(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	int iCnt = 0;
	for (auto& Cell : m_vCell) {

		_float3* fPos = Cell->Get_Points();

		// 만약 3점의 정보중 중복된 정보가 있을 경우
		if (CheckSameDot(fPos))
			// 넘긴다.
			continue;


		// 점 세개의 정보가 순서대로 시계방향인지 검사
		// 만약 반시계방향이라면
		if (!CheckCellCW(fPos)) {
			// [1], [2]번 인덱스의 값을 서로 바꾼다.
			_float3 fTempPos = fPos[1];
			fPos[1] = fPos[2];
			fPos[2] = fTempPos;
		}

		// 점 3개의 정보를 저장
		WriteFile(hFile, fPos, sizeof(_float3) * 3, &dwByte, nullptr);

		++iCnt;
	}

	// 파일을 닫는다.
	CloseHandle(hFile);
}

_bool CImGUI_Manager::CheckCellCW(_float3 * fPos)
{
	if (nullptr == fPos)
		return false;

	_double dTemp = fPos[0].x * fPos[1].z + fPos[1].x * fPos[2].z + fPos[2].x * fPos[0].z;
	dTemp -= (fPos[0].z * fPos[1].x + fPos[1].z * fPos[2].x + fPos[2].z * fPos[0].x);

	// 반시계방향
	if (dTemp > 0)
		return false;
	// 시계방향
	else if (dTemp < 0)
		return true;
	// 일직선
	else
		return false;
}

_bool CImGUI_Manager::CheckSameDot(_float3 * fPos)
{
	// 1~3번까지의 점이 있을 때, 1번 점과 2번점이 같거나, 1번점과 3번점이 같거나, 2번점과 3번점이 같을 경우
	if (
		((fPos[0].x == fPos[1].x) && (fPos[0].y == fPos[1].y) && (fPos[0].z == fPos[1].z)) ||
		((fPos[0].x == fPos[2].x) && (fPos[0].y == fPos[2].y) && (fPos[0].z == fPos[2].z)) ||
		((fPos[1].x == fPos[2].x) && (fPos[1].y == fPos[2].y) && (fPos[1].z == fPos[2].z))
		)
	{
		// 참을 반환한다.
		return true;
	}

	// 거짓을 반환한다.
	return false;
}

void CImGUI_Manager::SaveNaviMesh(const char * pModelFilePath, const char * pModelFileName)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	for (auto& Cell : m_vCell) {

		_float3* fPos = Cell->Get_Points();

		// 점 3개의 정보를 저장
		WriteFile(hFile, fPos, sizeof(_float3) * 3, &dwByte, nullptr);
	}

	// 파일을 닫는다.
	CloseHandle(hFile);
}

void CImGUI_Manager::Set_Client_Player(CPlayer * pPlayer)
{
	if (nullptr == pPlayer)
		return;

	m_pClient_Player = pPlayer;
}

void CImGUI_Manager::Render_PlayerPos(void)
{
	//// 창의 사이즈를 조정
	//ImGui::SetNextWindowSize(ImVec2(300.f, 150.f));

	//// 조정된 사이즈의 창 출력 시작
	//ImGui::Begin("Player Pos", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	//// ImGUI창의 시작 좌표를 좌측으로 고정
	////ImGui::SetWindowPos(ImVec2(g_iWinCX - ImGui::GetWindowSize().x, 60.f));
	////ImGui::SetWindowPos(ImVec2(0.f, 0.f));

	//if (nullptr == m_pClient_Player)
	//	ImGui::Text("Player : nullptr");
	//else {
	//	CTransform* pTransform = dynamic_cast<CTransform*>(m_pClient_Player->Get_Component(TEXT("Com_Transform")));

	//	_vector vPos = pTransform->Get_State(CTransform::STATE_POSITION);

	//	_float3 fPos;
	//	XMStoreFloat3(&fPos, vPos);

	//	ImGui::Text("Player : %.2f, %.2f, %.2f", fPos.x, fPos.y, fPos.z);
	//}

	//ImGui::End();
}

#endif

#ifdef 민희

HRESULT CImGUI_Manager::ImGUI_MinHee(void)
{
	static bool show_demo_window = true;
	static bool show_another_window = false;

	ImGui::SetNextWindowSize(ImVec2(600.f, 300.f));

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (ImGui::Begin("Tool"))
	{

	}

	if (ImGui::Button("Save"))
	{

	}

	POINT pt = Get_Mouse(g_hWnd);
	mx = (_float)pt.x;
	my = (_float)pt.y;
	ImGui::Text("Mousepos { %f, %f}", mx, my);


	ImGui::Text("Playerpos : {%f, %f, %f}", 0.f, 0.f, 0.f);
	ImGui::End();

	return S_OK;
}

HRESULT CImGUI_Manager::Tick_MinHee(void)
{
	return E_NOTIMPL;
}

void CImGUI_Manager::Shutdown(void)
{
	Safe_Release(m_pCamera);
}

void CImGUI_Manager::Set_Camera(CCamera* pCamera)
{
	m_pCamera = pCamera;
	Safe_AddRef(m_pCamera);
}

void CImGUI_Manager::Setting_Camera()
{
	if (nullptr == m_pCamera)
	{
		return;
	}
	CTransform* pTransform = (CTransform*)m_pCamera->Get_Component(L"Com_Transform");
	_float3 vCameraPosition;
	XMStoreFloat3(&vCameraPosition, pTransform->Get_State(CTransform::STATE_POSITION));
	ImGui::Text("Camera Pos : %.2f, %.2f, %.2f", vCameraPosition.x, vCameraPosition.y, vCameraPosition.z);
}

#endif