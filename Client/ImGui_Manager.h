#pragma once


// ���⼭ define�ϸ� ��
#define ����
//#define ����



#include "Client_Defines.h"
#include "Base.h"

#ifdef ����
#include "Camera.h"
#endif

#ifdef ����

#include "Picking_Rect.h"

// �� ������������ �ε��� �� �ִ� �ִ� �޽� ����
#define MAX_CNT	500

// �ε��� �� �ִ� ������ �Ǵ� ������Ʈ�� �ִ� ����
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


#ifdef ����
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

	void ImGUI_SelectUI(void);					// ����Ʈ��, ������Ʈ ��ġ, �׺� �޽� ���� �� 1�� �����Ͽ� ���� ���� UI

	UI_LEVEL m_eUI = UL_END;					// ���� ���õ� UI ����

	void ImGUI_SelectStage(void);				// �������� ���� UI
	void ImGUI_SettingCamera(void);				// ī�޶� ���� UI
	void ImGUI_SettingPickingTerrain(void);		// WireFrame ���� UI

	void ImGUI_SettingFire(void);
	void ImGUI_SettingObject(void);
	void ImGUI_NeviCell(void);

	void ImGUI_Lights(void);					// �� ���� ��

	void ImGUI_Sky(void);						// Skybox ���� ��

	void ImGUI_PostProcessing(void);			// PostProcessing ���� ��

	void ImGUI_Show_Fire_Standard_Object(_double TimeDelta);
	void ImGUI_Show_Fire_Object(_double TimeDelta);


private:
	void ImGUI_Setting_Emissive(void);
	_float m_fEmissive = 0.5f;

private:
	void Control_PostProcessing(void);	// PostProcessing ���� ���� ����
	void ImGUI_Setting_PostPorcessing_SaveLoad(void);
	HRESULT Save_PostProcessing_Info(const char* pModelFilePath, const char* pModelFileName);
	HRESULT Load_PostProcessing_Info(const char* pModelFilePath, const char* pModelFileName);

private:
	void Control_Skybox(void);			// ��ī�� �ڽ��� ���� ����
	void ImGUI_Setting_Skybox_SaveLoad(void);
	HRESULT Save_Skybox_Info(const char* pModelFilePath, const char* pModelFileName);
	HRESULT Load_Skybox_Info(const char* pModelFilePath, const char* pModelFileName);

	// Skybox ����
	CSky* m_pSkybox = nullptr;
	_uint m_iMaxSkyTextureCnt = 1;
	_uint m_iCurSkyboxIndex = 0;


private:
	// SettingLight ����
	void Create_Lights(void);					// ������ ����
	void Delete_Lights(void);					// ������ ����
	void Control_Select_PointLight(void);		// ������ ������ ���� ����
	void Update_Lights(void);					// vector�� ���� ������ ������ �ִ� ������ ���� ����
	void Show_CamPosition(void);				// ī�޶��� ��ġ ���� ���
	void Show_DirectionLight(void);				// ���Ɽ�� ���� ���
	void Control_DirectionLight(void);			// ���Ɽ�� ���� ����
	void Show_PointLight(void);					// �������� ���� ���
	void CreateDelete_PointLight(void);			// �������� ���� ����
	void Create_PointLight_LightObject(void);	// ������ ��� �������� �پ� �浹ü�� �׷��ִ� ������Ʈ�� �����ϴ� �Լ�
	void Delete_PointLight_LightObject(void);	// �浹ü�� �׷��ִ� ������Ʈ�� �����ϴ� �Լ�
	HRESULT Save_Lights(const char* pModelFilePath, const char* pModelFileName);	// ����(���Ɽ �� ������) ����
	HRESULT Load_Lights(const char* pModelFilePath, const char* pModelFileName);	// ����(���Ɽ �� ������) ����
	void ImGUI_Setting_Light_SaveLoad(void);	// Light ���� UI - SaveLoad
	void Create_LightObject(CLight* pLight);	// �������� �پ� �浹ü�� �׷��ִ� ������Ʈ ����

	_uint Get_LightIndex(CLight* pLight);	// ����Ʈ �Ŵ������� ���ڷ� ���� ���� ������ �ִ� �ε��� ������ ��ȯ�ϴ� �Լ�

	vector<CLight*>	m_vDirectionalLight;	// ���Ɽ ����
	vector<CLight*>	m_vPointLight;			// ������ ����
	vector<CLightObject*> m_vLightObject;	// ������ �浹ü ����

	_uint m_iLightIndex = 0;

	CLight* m_pSelected_PointLight = nullptr;	// ������ �������� ����
	_int m_iCurrent_PointLight_Index = 0;		// ���� ������ ����Ʈ�� �ε��� ����
	_int m_iSelect_PointLight_Index = -1;		// ������ ������ �ε��� ����


private:
	// SettingFire ����
	void Create_StandardObject(void);		// ���� ������Ʈ ����
	void Show_StandardObject(void);			// ���� ������Ʈ �����ֱ�
	void Control_StandardObject(void);		// ���� ������Ʈ ����, �ش��ϴ� ������Ʈ�� �����ֱ�
	void Update_StandardObject(void);		// ���� ������Ʈ ������Ʈ

	void Create_FireObject(void);			// �� ����
	void Show_FireObjectList(void);			// ������ �� ����Ʈ ����
	void Control_Select_FireObject(void);	// ���õ� �� ���� ����
	void Update_Selected_FireObject(void);	// ������ ������Ʈ�� Position, Scale���� ������Ʈ�ϴ� �Լ�
	
	void ImGUI_SettingMesh_Fire_Transform(void);		// Fire ���� UI - Transform
	void ImGUI_SettingMesh_Fire_Transform_Position(void);
	void ImGUI_SettingMesh_Fire_Transform_Rotation(void);
	void ImGUI_SettingMesh_Fire_Transform_Scale(void);

	void ImGUI_SettingMesh_Fire_GlowStrength(void);
	void ImGUI_SettingMesh_Fire_OriginalStrength(void);
	void ImGUI_SettingMesh_Fire_FrameSpeed(void);
	void ImGUI_SettingMesh_Fire_SaveLoad(void);		// Fire ���� UI - SaveLoad

	HRESULT LoadFireList(const char* pModelFilePath, const char* pModelFileName);	// ���ڷ� ���� ��ο� �����ϴ� �޽� ����Ʈ�� ������ �ҷ��ͼ� strMeshList�� �����ϴ� �Լ�
	HRESULT SaveFireList(const char* pModelFilePath, const char* pModelFileName);	// ���ڷ� ���� ��ο� strMeshList�� ������ �����ϴ� �Լ�
	HRESULT LoadMapFireList(const char* pModelFilePath, const char* pModelFileName);	// ���ڷ� ���� ��ο� �����ϴ� �޽� ����Ʈ�� ������ �ҷ��ͼ� strMeshList�� �����ϴ� �Լ�

private:
	void Update_Standard_MapObject_List(void);	// ��ġ�� ���� ������Ʈ ������ ������Ʈ �ϴ� �Լ�
	
private:
	CMapObject* m_pSelected_Fire_Standard_MapObject = nullptr;
	vector<CMapObject*> m_vFire_Standard_MapObjects;
	char	m_szStandard_Object_List[MAX_STANDARD_OBJECT_CNT][MAX_PATH];	// �ε� �� �޽� ����Ʈ(ver.char)
	_int m_iCurrent_Standard_Object_Index = 0;	// ���� ������ ���� ������Ʈ ����Ʈ�� �ε��� ����

	CFire_Deco* m_pSelected_FireObject = nullptr;				// ������ ���� ����
	vector<CFire_Deco*> m_vFires[MAX_STANDARD_OBJECT_CNT];		// ���� ������ �ִ� ���� ����(������ ������Ʈ�� �ε����� vector�� �ִ� ���� ������ render�Ѵ�.)
	_int m_iCurrentFire_Index = 0;			// ���� �� ����Ʈ�� �ε��� ����
	_int m_iSelectFireObjectIndex = -1;		// ������ ������Ʈ �ε��� ����

private:
	// ���� ��ġ �̵�
	string m_str_Fire_PositionX;
	string m_str_Fire_PositionY;
	string m_str_Fire_PositionZ;

private:
	_float3 m_vFire_Position = _float3(0.f, 0.f, 0.f);
	_float3 m_vFire_Rotation = _float3(0.f, 0.f, 0.f);
	_float3 m_vFire_Scale = _float3(1.f, 1.f, 1.f);		// ���� ������ ��

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
	vector<CDot*> m_vDot;		// ���� �� ����
	CDot* m_pDot = nullptr;		// ���� ��



private:
	void Render_Cells(void);

private:	// �׺� ������ �׸��� ���� �ʿ��� ������
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

public:	// ����Ʈ�� ���� ���� �� �Լ�
	void Push_HeightMapTag(const TCHAR* pHeightMapTag) { m_HeightMapList.push_back(wstring(pHeightMapTag)); }
	void UpdateHeightMapList(void);

private:
	vector<wstring>	m_HeightMapList;				// �ε� �� ����Ʈ�� ����Ʈ(ver.wstring)
	char	m_szHeightMapList[MAX_CNT][MAX_PATH];	// �ε� �� ����Ʈ�� ����Ʈ(ver.char)
	wstring m_strSelectedHeightMap;					// ���õ� ����Ʈ�� �±� ���ڿ�

private:
	void ImGUI_SettingMesh_Mesh(void);			// Mesh ���� UI - Mesh
	void ImGUI_SettingMesh_Feature(void);		// Mesh ���� UI - Feature
	void ImGUI_SettingMesh_Transform(void);		// Mesh ���� UI - Transform
	void ImGUI_SettingMesh_SaveLoad(void);		// Mesh ���� UI - SaveLoad
	void ImGUI_SettingMesh_PickingMesh(void);	// Mesh ���� UI - MeshPicking

private:
	void Update_Transform_Position(void);

private:
	void ImGUI_SettingMesh_Transform_Position(void);
	void ImGUI_SettingMesh_Transform_Rotation(void);
	void ImGUI_SettingMesh_Transform_Scale(void);

private:
	void Settings(void);
	void Colors(void);			// Ư�� ĭ�̳� ��ư�� ������ �ٲٴ� �Լ�
	void SetSize(void);			// ImGUI ��ü�� ����� �����ϴ� �Լ�

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
	_int Get_SelectFeatureObjectIndex(void) { return m_iSelectFeatureObjectIndex; }				// ���õ� ������Ʈ �ε��� ������ ��ȯ�ϴ� �Լ�
	void Set_SelectFeatureObjectIndex(_int iIndex) { m_iSelectFeatureObjectIndex = iIndex; }	// ���õ� ������Ʈ �ε��� ������ ��ȯ�ϴ� �Լ�
	CMapObject* Get_SelectedFeatureObject(void) { return m_pSelectedFeatureObject; }			// ���õ� ������Ʈ�� ������ ��ȯ�ϴ� �Լ�
	void Update_SelectedFeature(void);		// ������ ������Ʈ�� Position, Scale���� ������Ʈ�ϴ� �Լ�

private:
	_int m_iSelectFeatureObjectIndex = -1;
	_int m_iCurrent_Index = 0;
	CMapObject* m_pSelectedFeatureObject = nullptr;

public:
	void PushMeshList(const TCHAR* pPrototypeTag);	// �±׸� ���ڷ� �޾Ƽ� �ش� �޽��� ������ vector�� ����ִ� �Լ�
	void UpdateMeshList(void);					// �޽� ����Ʈ�� ������Ʈ �ϴ� �Լ�
	void UpdateFeatureList(void);				// ��ġ�� �޽� ����Ʈ�� ������Ʈ �ϴ� �Լ�
	void LoadMeshList(const char* pModelFilePath, const char* pModelFileName);	// ���ڷ� ���� ��ο� �����ϴ� �޽� ����Ʈ�� ������ �ҷ��ͼ� strMeshList�� �����ϴ� �Լ�
	void SaveMeshList(const char* pModelFilePath, const char* pModelFileName);	// ���ڷ� ���� ��ο� strMeshList�� ������ �����ϴ� �Լ�

private:
	vector<wstring>	m_strMeshList;				// �ε� �� �޽� ����Ʈ(ver.wstring)
	char	m_szMeshList[MAX_CNT][MAX_PATH];	// �ε� �� �޽� ����Ʈ(ver.char)

	vector<CMapObject*>	m_PlacedMapObjectList;	// ��ġ�� �޽� ����Ʈ
	char	m_szFeatureList[MAX_CNT][MAX_PATH];	// ��ġ�� �޽� ����Ʈ(ver.char)

private:
	CMapObject* m_pPickingMeshObject = nullptr;

public:
	Stage_LEVEL Get_CurStageLevel(void) { return m_eCurStageLevel; }	// ���� �������� ���� ��ȯ

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
	// �̵�
	string m_str_PositionX;
	string m_str_PositionY;
	string m_str_PositionZ;

	// ȸ��
	string m_str_RotationX;
	string m_str_RotationY;
	string m_str_RotationZ;

	// ũ��
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
	vector<CCell*> m_vCell;		// ���� ������ ����
	CCell* m_pCell = nullptr;	// ���� ��

private:
	void SaveNaviMesh(const char* pModelFilePath, const char* pModelFileName);


public:
	void Set_Client_Player(CPlayer* pPlayer);

private:
	void Render_PlayerPos(void);

private:
	CPlayer* m_pClient_Player = nullptr;
	



#endif // !����


#ifdef ����

private:
	HRESULT ImGUI_MinHee(void);
	HRESULT Tick_MinHee(void);

private:
	void Shutdown(void);

public:
	void Set_Camera(CCamera* pCamera);
	void Setting_Camera();

private:
	//UI Mouse��ǥ��
	float mx, my;
private:
	CCamera* m_pCamera = nullptr;

#endif // !����
};

END