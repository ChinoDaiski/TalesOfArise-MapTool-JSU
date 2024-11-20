#include "stdafx.h"
#include "..\Public\MapObject.h"
#include "GameInstance.h"
#include "ImGUI_Manager.h"

_uint CMapObject::iFeatureMeshNumber = 0;

CMapObject::CMapObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CMapObject::CMapObject(const CMapObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMapObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	MAPOBJDesc MapObjDesc = *((MAPOBJDesc*)pArg);

	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&MapObjDesc.TransformMatrix));

	m_iFeatureMeshNumber = iFeatureMeshNumber++;

	m_bMap = MapObjDesc.m_bMap;

	if (m_pModelCom->Get_Model_Type() == CModel::TYPE_ANIM)
		m_pModelCom->Set_AnimationIndex(0);

	return S_OK;
}

void CMapObject::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	//m_pModelCom->Update(TimeDelta, false);


	_matrix matWorld = XMMatrixIdentity();
	_float4x4 matWorld4x4;
	XMStoreFloat4x4(&matWorld4x4, matWorld);

	
	XMStoreFloat4((_float4*)&matWorld4x4.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pSphereCom->Update(XMLoadFloat4x4(&matWorld4x4));

	// 해당 모델의 타입이 애님일 경우
	if (m_pModelCom->Get_Model_Type() == CModel::TYPE_ANIM) {
		// 해당 모델의 Update를 돌린다.
		m_pModelCom->Update(TimeDelta, true);
		//_uint iIndex = 0;
		//m_pModelCom->Update_NextAnim(TimeDelta, 0.0, 0, &iIndex);
	}
}

void CMapObject::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom) {
		if (
			!_tcscmp(m_pModelTag, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light")) ||
			!_tcscmp(m_pModelTag, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light2"))
			) {
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
		}
		else {
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

			//if (true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
			//	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CMapObject::Render()
{
	if (
		nullptr == m_pShaderNonAnimCom ||
		nullptr == m_pShaderAnimCom ||
		nullptr == m_pModelCom
		)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	if (m_isTerrainMesh) {
		for (_uint i = 0; i < iNumMeshContainers; ++i) {
			if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->SetUp_ShaderResourceView(m_pShaderNonAnimCom, "g_SourDiffuseTexture", 6)))
				return E_FAIL;
			if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->SetUp_ShaderResourceView(m_pShaderNonAnimCom, "g_DestDiffuseTexture", 17)))
				return E_FAIL;
			if (FAILED(m_pTextureCom[TYPE_MASK]->SetUp_ShaderResourceView(m_pShaderNonAnimCom, "g_MaskTexture", 2)))
				return E_FAIL;
			
			if (FAILED(m_pModelCom->Render(m_pShaderNonAnimCom, nullptr, i, 4)))
				return E_FAIL;
		}
	}
	else {
		if (m_pModelCom->Get_Model_Type() == CModel::TYPE_ANIM) {
			for (_uint i = 0; i < iNumMeshContainers; ++i)
			{
				if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderAnimCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
					return E_FAIL;

				if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderAnimCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
					return E_FAIL;

				if (FAILED(m_pModelCom->Render(m_pShaderAnimCom, "g_BoneMatrices", i, 0)))
					return E_FAIL;
			}
		}
		else if (m_pModelCom->Get_Model_Type() == CModel::TYPE_NONANIM) {
			for (_uint i = 0; i < iNumMeshContainers; ++i) {
				if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderNonAnimCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
					return E_FAIL;

				if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderNonAnimCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
					return E_FAIL;

				if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderNonAnimCom, aiTextureType_EMISSIVE, "g_EmissiveTexture", i)))
					return E_FAIL;


				// IMGUI에서 선택된 오브젝트의 정보가 나의 정보와 같지 않을 경우 
				if (CImGUI_Manager::GetInstance()->Get_SelectedFeatureObject() != this) {
					if (
						!_tcscmp(m_pModelTag, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light")) ||
						!_tcscmp(m_pModelTag, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light2"))
						) {
						// 그냥 그린다.
						if (FAILED(m_pModelCom->Render(m_pShaderNonAnimCom, nullptr, i, 7)))
							return E_FAIL;
					}
					else {
						// 그냥 그린다.
						if (FAILED(m_pModelCom->Render(m_pShaderNonAnimCom, nullptr, i, 0)))
							return E_FAIL;
					}
				}
				// 선택된 오브젝트가 나와 같을 경우 
				else {
					if (FAILED(m_pModelCom->Render(m_pShaderNonAnimCom, nullptr, i, 2)))
						return E_FAIL;
				}
			}
		}
	}

	

#ifdef _DEBUG
	if (CImGUI_Manager::GetInstance()->Get_Level() == CImGUI_Manager::UL_SetObject)
		m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CMapObject::SetUp_Components(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	MAPOBJDesc MapObjDesc = *((MAPOBJDesc*)pArg);

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader_NonAnim"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderNonAnimCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader_Anim"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderAnimCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_MapTool, MapObjDesc.pPrototype_ObjectName, (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	// 모델 Tag 복사
	_tcsncpy_s(m_pModelTag, MapObjDesc.pPrototype_ObjectName, _tcslen(MapObjDesc.pPrototype_ObjectName));

	if 
		(!_tcscmp(TEXT("Prototype_Component_Model_Map_GrandYork_HeightMapMesh"), m_pModelTag) ||
		!_tcscmp(TEXT("Prototype_Component_Model_Map_GrandYork_Battle_02_NaviMap"), m_pModelTag) ||
		!_tcscmp(TEXT("Prototype_Component_Model_Map_GrandYork_Battle_04_NaviMap"), m_pModelTag)
		) {
		m_isTerrainMesh = true;
	}

	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	MapObjDesc.fColliderSize = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vSize = MapObjDesc.fColliderSize;
	ColliderDesc.fRadius = ColliderDesc.vSize.y * 0.5f;
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Sphere"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	if (m_isTerrainMesh) {
		// 텍스쳐 추가
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Diffuse"), LEVEL_MapTool, TEXT("Prototype_Component_Texture_Terrain_Diffuse"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
			return E_FAIL;
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture_Mask"), LEVEL_MapTool, TEXT("Prototype_Component_Texture_Mask"), (CComponent**)&m_pTextureCom[TYPE_MASK])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMapObject::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	//_float fEmissive_Density = 0.25f;
	//if (FAILED(m_pShaderNonAnimCom->Set_RawValue("fEmissive_Density", &fEmissive_Density, sizeof(_float))))
	//	return E_FAIL;

	if (m_isTerrainMesh) {
		if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderNonAnimCom, "g_WorldMatrix")))
			return E_FAIL;
		if (FAILED(m_pShaderNonAnimCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderNonAnimCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;
	}
	else {
		if (m_pModelCom->Get_Model_Type() == CModel::TYPE_ANIM) {
			if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderAnimCom, "g_WorldMatrix")))
				return E_FAIL;
			if (FAILED(m_pShaderAnimCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderAnimCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderAnimCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPositionFloat4(), sizeof(_float4))))
				return E_FAIL;
		}
		else if (m_pModelCom->Get_Model_Type() == CModel::TYPE_NONANIM) {
			if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderNonAnimCom, "g_WorldMatrix")))
				return E_FAIL;
			if (FAILED(m_pShaderNonAnimCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderNonAnimCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderNonAnimCom->Set_RawValue("g_PreviewDiffuse", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4))))
				return E_FAIL;
		}
	}

	_float fAlpha = 1.f;
	if (
		!_tcscmp(m_pModelTag, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light")) ||
		!_tcscmp(m_pModelTag, TEXT("Object_Boss_Lord_Vholran_Igniseri_Light2"))
		) {
		fAlpha = 0.01f;
		if (FAILED(m_pShaderNonAnimCom->Set_RawValue("fAlpha", &fAlpha, sizeof(_float))))
			return E_FAIL;
	}
	else {
		if (FAILED(m_pShaderNonAnimCom->Set_RawValue("fAlpha", &fAlpha, sizeof(_float))))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMapObject * CMapObject::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CMapObject*	pInstance = new CMapObject(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CMapObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMapObject::Clone(void * pArg)
{
	CMapObject*	pInstance = new CMapObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CMapObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapObject::Free()
{
	__super::Free();

	Safe_Release(m_pShaderNonAnimCom);
	Safe_Release(m_pShaderAnimCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphereCom);

	if (m_isTerrainMesh) {
		Safe_Release(m_pTextureCom[TYPE_DIFFUSE]);
		Safe_Release(m_pTextureCom[TYPE_MASK]);
	}
}