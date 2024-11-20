#pragma once


// 여기서 define하면 됨
#define 성욱
//#define 민희



#include "Client_Defines.h"
#include "Base.h"

#ifdef 민희
#include "Camera.h"
#endif

#ifdef 성욱

#include "Picking_Rect.h"

// 한 스테이지에서 로드할 수 있는 최대 메쉬 갯수
#define MAX_CNT	500

// 로드할 수 있는 기준이 되는 오브젝트의 최대 갯수
#define MAX_STANDARD_OBJECT_CNT	10

BEGIN(Engine)

class CCell;
class CVIBuffer_Triangle;
class CShader;
class CModel;
class CMeshContainer;

END


BEGIN(Client)

class CCamera_MapTool;
class CMapObject;
class CPreviewObject;
class CTerrain;
class CDot;
class CPlayer;
class CFire_Deco;
class CLightObject;
class CSky;

END

#endif

BEGIN(Client)

class CImGUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGUI_Manager)
private:
	CImGUI_Manager();
	virtual ~CImGUI_Manager() = default;
public:
	HRESULT Init_ImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT StartFrame(void);
	HRESULT Set_Contents(void);

public:
	HRESULT Tick(_double TimeDelta);
	HRESULT Render(void);

public:
	void InitWin32(HWND hWnd);
	LRESULT WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	void OnOffImgui(void) { m_bImguiEnable = !m_bImguiEnable; }
	bool isImguiEnable() { return m_bImguiEnable; }

private:
	bool m_bImguiEnable = false;

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

public:
	virtual void Free() override;


#ifdef 성욱
public:
	enum Stage_LEVEL { Stage_01, Stage_02, Stage_03, Stage_04, Stage_05, Stage_06, Stage_07, Stage_08, Stage_09, Stage_10, Stage_END };
	enum UI_LEVEL { UL_CreateFire, UL_SetObject, UL_Navigation, UL_Lights, UL_Sky, UL_PostProcessing, UL_END };

private:
	void Shutdown(void);

private:
	HRESULT Tick_SeongUk(_double TimeDelta);


public:
	UI_LEVEL Get_Level(void) { return m_eUI; }

private:
	HRESULT ImGUI_SeongUk(void);

	void ImGUI_SelectUI(void);					// 하이트맵, 오브젝트 배치, 네비 메쉬 생성 중 1을 선택하여 툴을 띄우는 UI

	UI_LEVEL m_eUI = UL_END;					// 현재 선택된 UI 정보

	void ImGUI_SelectStage(void);				// 스테이지 선택 UI
	void ImGUI_SettingCamera(void);				// 카메라 관련 UI
	void ImGUI_SettingPickingTerrain(void);		// WireFrame 관련 UI

	void ImGUI_SettingFire(void);
	void ImGUI_SettingObject(void);
	void ImGUI_NeviCell(void);

	void ImGUI_Lights(void);					// 빛 관련 툴

	void ImGUI_Sky(void);						// Skybox 관련 툴

	void ImGUI_PostProcessing(void);			// PostProcessing 관련 툴

	void ImGUI_Show_Fire_Standard_Object(_double TimeDelta);
	void ImGUI_Show_Fire_Object(_double TimeDelta);


private:
	void ImGUI_Setting_Emissive(void);
	_float m_fEmissive = 0.5f;

private:
	void Control_PostProcessing(void);	// PostProcessing 관련 정보 수정
	void ImGUI_Setting_PostPorcessing_SaveLoad(void);
	HRESULT Save_PostProcessing_Info(const char* pModelFilePath, const char* pModelFileName);
	HRESULT Load_PostProcessing_Info(const char* pModelFilePath, const char* pModelFileName);

private:
	void Control_Skybox(void);			// 스카이 박스의 정보 수정
	void ImGUI_Setting_Skybox_SaveLoad(void);
	HRESULT Save_Skybox_Info(const char* pModelFilePath, const char* pModelFileName);
	HRESULT Load_Skybox_Info(const char* pModelFilePath, const char* pModelFileName);

	// Skybox 관련
	CSky* m_pSkybox = nullptr;
	_uint m_iMaxSkyTextureCnt = 1;
	_uint m_iCurSkyboxIndex = 0;


private:
	// SettingLight 관련
	void Create_Lights(void);					// 점광원 생성
	void Delete_Lights(void);					// 점광원 삭제
	void Control_Select_PointLight(void);		// 선택한 점광원 정보 수정
	void Update_Lights(void);					// vector형 광원 정보가 가지고 있는 광원의 정보 수정
	void Show_CamPosition(void);				// 카메라의 위치 정보 출력
	void Show_DirectionLight(void);				// 방향광의 정보 출력
	void Control_DirectionLight(void);			// 방향광의 정보 수정
	void Show_PointLight(void);					// 점광원의 정보 출력
	void CreateDelete_PointLight(void);			// 점광원의 정보 수정
	void Create_PointLight_LightObject(void);	// 생성된 모든 점광원에 붙어 충돌체를 그려주는 오브젝트를 생성하는 함수
	void Delete_PointLight_LightObject(void);	// 충돌체를 그려주는 오브젝트를 삭제하는 함수
	HRESULT Save_Lights(const char* pModelFilePath, const char* pModelFileName);	// 광원(방향광 및 점광원) 생성
	HRESULT Load_Lights(const char* pModelFilePath, const char* pModelFileName);	// 광원(방향광 및 점광원) 삭제
	void ImGUI_Setting_Light_SaveLoad(void);	// Light 관련 UI - SaveLoad
	void Create_LightObject(CLight* pLight);	// 점광원에 붙어 충돌체를 그려주는 오브젝트 생성

	_uint Get_LightIndex(CLight* pLight);	// 라이트 매니저에서 인자로 받은 빛이 가지고 있는 인덱스 정보를 반환하는 함수

	vector<CLight*>	m_vDirectionalLight;	// 방향광 정보
	vector<CLight*>	m_vPointLight;			// 점광원 정보
	vector<CLightObject*> m_vLightObject;	// 점광원 충돌체 정보

	_uint m_iLightIndex = 0;

	CLight* m_pSelected_PointLight = nullptr;	// 선택한 점광원의 정보
	_int m_iCurrent_PointLight_Index = 0;		// 현재 점광원 리스트의 인덱스 정보
	_int m_iSelect_PointLight_Index = -1;		// 선택한 점광원 인덱스 정보


private:
	// SettingFire 관련
	void Create_StandardObject(void);		// 기준 오브젝트 생성
	void Show_StandardObject(void);			// 기준 오브젝트 보여주기
	void Control_StandardObject(void);		// 기준 오브젝트 선택, 해당하는 오브젝트만 보여주기
	void Update_StandardObject(void);		// 기준 오브젝트 업데이트

	void Create_FireObject(void);			// 불 생성
	void Show_FireObjectList(void);			// 생성된 불 리스트 정보
	void Control_Select_FireObject(void);	// 선택된 불 정보 조정
	void Update_Selected_FireObject(void);	// 선택한 오브젝트의 Position, Scale값을 업데이트하는 함수
	
	void ImGUI_SettingMesh_Fire_Transform(void);		// Fire 관련 UI - Transform
	void ImGUI_SettingMesh_Fire_Transform_Position(void);
	void ImGUI_SettingMesh_Fire_Transform_Rotation(void);
	void ImGUI_SettingMesh_Fire_Transform_Scale(void);

	void ImGUI_SettingMesh_Fire_GlowStrength(void);
	void ImGUI_SettingMesh_Fire_OriginalStrength(void);
	void ImGUI_SettingMesh_Fire_FrameSpeed(void);
	void ImGUI_SettingMesh_Fire_SaveLoad(void);		// Fire 관련 UI - SaveLoad

	HRESULT LoadFireList(const char* pModelFilePath, const char* pModelFileName);	// 인자로 받은 경로에 존재하는 메쉬 리스트의 정보를 불러와서 strMeshList에 저장하는 함수
	HRESULT SaveFireList(const char* pModelFilePath, const char* pModelFileName);	// 인자로 받은 경로에 strMeshList의 정보를 저장하는 함수
	HRESULT LoadMapFireList(const char* pModelFilePath, const char* pModelFileName);	// 인자로 받은 경로에 존재하는 메쉬 리스트의 정보를 불러와서 strMeshList에 저장하는 함수

private:
	void Update_Standard_MapObject_List(void);	// 배치된 기준 오브젝트 정보를 업데이트 하는 함수
	
private:
	CMapObject* m_pSelected_Fire_Standard_MapObject = nullptr;
	vector<CMapObject*> m_vFire_Standard_MapObjects;
	char	m_szStandard_Object_List[MAX_STANDARD_OBJECT_CNT][MAX_PATH];	// 로드 된 메쉬 리스트(ver.char)
	_int m_iCurrent_Standard_Object_Index = 0;	// 현재 선택한 기준 오브젝트 리스트의 인덱스 정보

	CFire_Deco* m_pSelected_FireObject = nullptr;				// 선택한 불의 정보
	vector<CFire_Deco*> m_vFires[MAX_STANDARD_OBJECT_CNT];		// 툴이 가지고 있는 불의 정보(선택한 오브젝트의 인덱스의 vector에 있는 불의 정보를 render한다.)
	_int m_iCurrentFire_Index = 0;			// 현재 불 리스트의 인덱스 정보
	_int m_iSelectFireObjectIndex = -1;		// 선택한 오브젝트 인덱스 정보

private:
	// 불의 위치 이동
	string m_str_Fire_PositionX;
	string m_str_Fire_PositionY;
	string m_str_Fire_PositionZ;

private:
	_float3 m_vFire_Position = _float3(0.f, 0.f, 0.f);
	_float3 m_vFire_Rotation = _float3(0.f, 0.f, 0.f);
	_float3 m_vFire_Scale = _float3(1.f, 1.f, 1.f);		// 불의 스케일 값

public:
	class CPicking_Rect* Get_PickingRect(void) { return m_pPicking_Rect; }
	void Set_PickingRect(class CPicking_Rect* pRect) {
		if (nullptr != m_pPicking_Rect)
			Safe_Release(m_pPicking_Rect);
		m_pPicking_Rect = pRect;
	}
private:
	class CPicking_Rect* m_pPicking_Rect = nullptr;


public:
	void Add_SelectDot(CDot* pDot) {
		if (m_SelectDotslist.size() >= 3) {
			m_SelectDotslist.pop_front();
		}
		m_SelectDotslist.push_back(pDot);
	}
private:
	list<CDot*> m_SelectDotslist;



public:
	void Set_Dot(class CDot* pDot) {
		if (nullptr != m_pDot)
			m_pDot = pDot;
	}
	HRESULT Save_DotInfo(const char * pModelFilePath, const char * pModelFileName);
	HRESULT Load_DotInfo(const char * pModelFilePath, const char * pModelFileName);
private:
	vector<CDot*> m_vDot;		// 찍힌 점 정보
	CDot* m_pDot = nullptr;		// 현재 점



private:
	void Render_Cells(void);

private:	// 네비 셀들을 그리기 위해 필요한 변수들
	class CVIBuffer_Triangle*		m_pVIBuffer = nullptr;
	class CShader*					m_pShader = nullptr;


public:
	_float Get_DotColliderSize(void) { return m_fDot_ColliderSize; }
	_float Get_ObjectColliderSize(void) { return m_fObject_ColliderSize; }
	_float Get_MeshSpeed(void) { return m_fMeshSpeed; }

private:
	_float m_fDot_ColliderSize = 3.f;
	_float m_fObject_ColliderSize = 3.f;
	_float m_fMeshSpeed = 0.f;

public:	// 하이트맵 관련 변수 및 함수
	void Push_HeightMapTag(const TCHAR* pHeightMapTag) { m_HeightMapList.push_back(wstring(pHeightMapTag)); }
	void UpdateHeightMapList(void);

private:
	vector<wstring>	m_HeightMapList;				// 로드 된 하이트맵 리스트(ver.wstring)
	char	m_szHeightMapList[MAX_CNT][MAX_PATH];	// 로드 된 하이트맵 리스트(ver.char)
	wstring m_strSelectedHeightMap;					// 선택된 하이트맵 태그 문자열

private:
	void ImGUI_SettingMesh_Mesh(void);			// Mesh 관련 UI - Mesh
	void ImGUI_SettingMesh_Feature(void);		// Mesh 관련 UI - Feature
	void ImGUI_SettingMesh_Transform(void);		// Mesh 관련 UI - Transform
	void ImGUI_SettingMesh_SaveLoad(void);		// Mesh 관련 UI - SaveLoad
	void ImGUI_SettingMesh_PickingMesh(void);	// Mesh 관련 UI - MeshPicking

private:
	void Update_Transform_Position(void);

private:
	void ImGUI_SettingMesh_Transform_Position(void);
	void ImGUI_SettingMesh_Transform_Rotation(void);
	void ImGUI_SettingMesh_Transform_Scale(void);

private:
	void Settings(void);
	void Colors(void);			// 특정 칸이나 버튼의 색상을 바꾸는 함수
	void SetSize(void);			// ImGUI 자체의 사이즈를 조정하는 함수

private:
	HRESULT Save_NaviCell(const TCHAR* pNaviDataFilePath, const vector<CCell*>& vCells);

public:
	void Set_Camera(CCamera_MapTool* pCamera) { m_pCurrCamera = pCamera; }
	void Set_Terrain(CTerrain* pTerrain) { m_pCurrTerrain = pTerrain; }

private:
	_float2		m_vCameraMax;
	_float2		m_vWireFrameMax;

private:
	CCamera_MapTool*	m_pCurrCamera = nullptr;
	CTerrain*	m_pCurrTerrain = nullptr;

	const TCHAR* m_pPrototypeTag_SelectedMesh = TEXT("");

public:
	_int Get_SelectFeatureObjectIndex(void) { return m_iSelectFeatureObjectIndex; }				// 선택된 오브젝트 인덱스 정보를 반환하는 함수
	void Set_SelectFeatureObjectIndex(_int iIndex) { m_iSelectFeatureObjectIndex = iIndex; }	// 선택된 오브젝트 인덱스 정보를 반환하는 함수
	CMapObject* Get_SelectedFeatureObject(void) { return m_pSelectedFeatureObject; }			// 선택된 오브젝트의 정보를 반환하는 함수
	void Update_SelectedFeature(void);		// 선택한 오브젝트의 Position, Scale값을 업데이트하는 함수

private:
	_int m_iSelectFeatureObjectIndex = -1;
	_int m_iCurrent_Index = 0;
	CMapObject* m_pSelectedFeatureObject = nullptr;

public:
	void PushMeshList(const TCHAR* pPrototypeTag);	// 태그를 인자로 받아서 해당 메쉬의 정보를 vector에 집어넣는 함수
	void UpdateMeshList(void);					// 메쉬 리스트를 업데이트 하는 함수
	void UpdateFeatureList(void);				// 배치된 메쉬 리스트를 업데이트 하는 함수
	void LoadMeshList(const char* pModelFilePath, const char* pModelFileName);	// 인자로 받은 경로에 존재하는 메쉬 리스트의 정보를 불러와서 strMeshList에 저장하는 함수
	void SaveMeshList(const char* pModelFilePath, const char* pModelFileName);	// 인자로 받은 경로에 strMeshList의 정보를 저장하는 함수

private:
	vector<wstring>	m_strMeshList;				// 로드 된 메쉬 리스트(ver.wstring)
	char	m_szMeshList[MAX_CNT][MAX_PATH];	// 로드 된 메쉬 리스트(ver.char)

	vector<CMapObject*>	m_PlacedMapObjectList;	// 배치된 메쉬 리스트
	char	m_szFeatureList[MAX_CNT][MAX_PATH];	// 배치된 메쉬 리스트(ver.char)

private:
	CMapObject* m_pPickingMeshObject = nullptr;

public:
	Stage_LEVEL Get_CurStageLevel(void) { return m_eCurStageLevel; }	// 현재 스테이지 레벨 반환

private:
	_bool m_bSelect_Stage = false;
	_uint m_iCurrStage = 0;
	Stage_LEVEL m_eCurStageLevel = Stage_END;

public:
	_bool Get_AssignMesh(void) { return m_bAssignMesh; }

private:
	_bool m_bAssignMesh = false;
	_bool m_bSelectMesh = false;

private:
	_float3 m_vPosition = _float3(0.f, 0.f, 0.f);
	_float3 m_vRotation = _float3(0.f, 0.f, 0.f);
	_float3 m_vScale = _float3(1.f, 1.f, 1.f);

private:
	// 이동
	string m_str_PositionX;
	string m_str_PositionY;
	string m_str_PositionZ;

	// 회전
	string m_str_RotationX;
	string m_str_RotationY;
	string m_str_RotationZ;

	// 크기
	string m_str_ScaleX;
	string m_str_ScaleY;
	string m_str_ScaleZ;

private:
	_float m_fCameraSpeed = 100.f;

private:
	void LoadCellList(const char* pModelFilePath, const char* pModelFileName);
	void SaveCellList(const char* pModelFilePath, const char* pModelFileName);

	_bool CheckCellCW(_float3* fPos);
	_bool CheckSameDot(_float3* fPos);

private:
	vector<CCell*> m_vCell;		// 찍힌 셀들의 정보
	CCell* m_pCell = nullptr;	// 현재 셀

private:
	void SaveNaviMesh(const char* pModelFilePath, const char* pModelFileName);


public:
	void Set_Client_Player(CPlayer* pPlayer);

private:
	void Render_PlayerPos(void);

private:
	CPlayer* m_pClient_Player = nullptr;
	



#endif // !성욱


#ifdef 민희

private:
	HRESULT ImGUI_MinHee(void);
	HRESULT Tick_MinHee(void);

private:
	void Shutdown(void);

public:
	void Set_Camera(CCamera* pCamera);
	void Setting_Camera();

private:
	//UI Mouse좌표용
	float mx, my;
private:
	CCamera* m_pCamera = nullptr;

#endif // !민희
};

END