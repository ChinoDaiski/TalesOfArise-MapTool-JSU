#pragma once
#include "Component.h"





BEGIN(Engine)

class CAnimation;
class CTexture;
class CShader;
class CHierarchyNode;

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix, _bool bMakeDat = true);
	virtual HRESULT NativeConstruct(void* pArg) override;
	void Update(_double TimeDelta, _bool bRoof);
	void Monster_Update(_double TimeDelta, _bool bRoof);
	void Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex);
	void Monster_Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint * pCurrentAnimIndex);
	HRESULT Render(class CShader* pShader, const char* pBoneMatricesName, _uint iMeshContainerIndex, _uint iPassIndex);

public:
	HRESULT Save_Data(const _tchar* FilePath);
	HRESULT Load_Data(const _tchar* FilePath);

public:
	_float4x4 * Get_CombinedTransformationMatrix(const char * pNodeName);
	_uint Get_NumMeshContainer() const;
	CAnimation* Get_Animation(_uint iIndex);
	CAnimation* Get_CurAnimation();

	_matrix CModel::Get_PivotMatrix()
	{
		return XMLoadFloat4x4(&m_PivotMatrix);
	}

	_float4x4 Get_PivotMatrix4x4() {
		return m_PivotMatrix;
	}
	_uint Get_NumAnimation();

public:
	void Set_AnimationIndex(_uint iAnimIndex);

public:
	HRESULT Bind_Material_OnShader(class CShader* pShader, aiTextureType eType, const char* pConstantName, _uint iMeshContainerIndex);

public:
	vector<class CMeshContainer*>& Get_MeshContainer(void) { return m_MeshContainers; }

private:
	HRESULT Ready_MeshContainers();
	HRESULT Clone_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_Materials();
	HRESULT Ready_Animations();
	HRESULT Clone_Animations();
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent = nullptr, _uint iDepth = 0);
	HRESULT Ready_HierarchyNodes();
	HRESULT Set_HierarchyNodes();

private:
	const aiScene* m_pScene = nullptr;
	Assimp::Importer m_Importer;

private:				//2��ȭ��
	_bool	m_isBinary = false;
	MODELDATADESC m_ModelDatadesc;
	vector<HIERARCHYDESC> m_vHierachyData;
	vector<MESHCONTAINERDESC> m_vMeshConData;
	vector<MATERIALSDESC> m_vMatData;
	vector<ANIMATIONDESC> m_vAnimData;
	_int m_iNumChildren = 0;

public:
	TYPE Get_Model_Type(void) { return m_eType; }

private:
	TYPE m_eType = TYPE_END;
	_float4x4 m_PivotMatrix;

private:
	_uint m_iNumMeshContainers = 0;
	vector<class CMeshContainer*> m_MeshContainers;
	typedef vector<class CMeshContainer*> MESHCONTAINERS;

private:
	_uint m_iNumMaterials = 0;
	vector<MODELMATERIAL> m_Materials;
	typedef vector<MODELMATERIAL> MATERIALS;

private:
	_uint m_iNumAnimations = 0, m_iCurrentAnimIndex = 0;
	vector<CAnimation*> m_Animations;
	typedef vector<CAnimation*> ANIMATIONS;

private:
	_int									m_Index = 0;
	_uint m_iNumNodes = 0;
	vector<CHierarchyNode*> m_HierarchyNodes;
	typedef vector<CHierarchyNode*> HIERARCHYNODES;
	CHierarchyNode* m_pRootNode = nullptr;

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity(), _bool bMakeDat = true);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END