#include "stdafx.h"
#include "..\Public\Probs.h"
#include "GameInstance.h"
#include "Particle_Manager.h"
#include "Quest_Manager.h"

CProbs::CProbs(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CStaticObject(pDevice, pContext, strObjectTag, eType)
{
}

CProbs::CProbs(const CProbs& rhs)
	: CStaticObject(rhs)
{
}

HRESULT CProbs::Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (FAILED(__super::Initialize_Prototype(eType, strFilePath, strFileName)))
		return E_FAIL;


	return S_OK;
}

HRESULT CProbs::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (m_strMapObjName == TEXT("Evermore_DecoBuild_01"))
	{
		m_iMoveTick = 101;
		m_iRandomCase = GI->RandomInt(0, 1);
	}

	PrepareRainbowColor();

	return S_OK;
}

void CProbs::Tick(_float fTimeDelta)
{
	if (nullptr == m_pParticle && TEXT("Common_PropA_00") == m_strObjectTag)
	{
		CParticle_Manager::GetInstance()->Generate_Particle_To_Position(TEXT("CampFire"), m_pTransformCom->Get_WorldMatrix(),
			Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), this, &m_pParticle, false);
	}

	if (nullptr == m_pParticle && TEXT("Winter_CampFire") == m_strObjectTag)
		CParticle_Manager::GetInstance()->Generate_Particle_To_Position(TEXT("Winter_CampFire"), m_pTransformCom->Get_WorldMatrix(), Vec3(-0.3f, 1.5f, 0.0f), Vec3(0.0f,0.0f,0.0f), Vec3(0.0f, 0.0f, 0.0f), this, &m_pParticle, false);


	if (TEXT("Winter_Plants_02") == m_strObjectTag || TEXT("Winter_Plants_01") == m_strObjectTag || TEXT("WitchForest_Plant_01") == m_strObjectTag)
	{
		m_fTime += fTimeDelta;

		m_fAngle = 0.2 * cos(m_fTime);
	}

	__super::Tick(fTimeDelta);

	MoveProps(fTimeDelta);
}

void CProbs::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::GetInstance()->QUESTEVENT_BOSS_KILL)
	{
		Compute_CamZ(m_pTransformCom->Get_Position());

		if (m_fCamDistance <= 150.0f && true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 30.0f))
			m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
	}
	else if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 30.0f))
	{
		// Shadow 필요하면 ShadowRender 추가?
		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDERGROUP::RENDER_SHADOW, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
		m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
	}


}

HRESULT CProbs::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CProbs::Render_ShadowDepth()
{
	if (true == m_bEnable)
		return S_OK;

	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

HRESULT CProbs::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (true == m_bEnable)
		return S_OK;

	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;


	if (TEXT("Winter_Plants_02") == m_strObjectTag || TEXT("Winter_Plants_01") == m_strObjectTag)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(0), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
		if (FAILED(pInstancingShader->Bind_RawValue("fGrassAngle", &m_fAngle, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, 0, pInstancingBuffer, WorldMatrices, 6)))
			return E_FAIL;
	}
	else if (TEXT("WitchForest_Plant_01") == m_strObjectTag)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(0), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
		if (FAILED(pInstancingShader->Bind_RawValue("fGrassAngle", &m_fAngle, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(pInstancingShader, "WitchGrassMaskTexture", 0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, 0, pInstancingBuffer, WorldMatrices, 7)))
			return E_FAIL;

	}
	else if (TEXT("WitchForest_Prop_01a") == m_strObjectTag)
	{
		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		if (FAILED(pInstancingShader->Bind_RawValue("g_vRainbowColor", &m_vRainbowColor, sizeof(Vec4))))
			return E_FAIL;

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			_uint iPassIndex = 0;

			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_SPECULAR, "g_SpecularTexture")))
				iPassIndex = 0;
			else
				iPassIndex = 11;

			if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
				return E_FAIL;
		}
	}
	else if (/*TEXT("WitchForest_lantern_01") == m_strObjectTag ||*/ TEXT("WitchForest_PropA_01") == m_strObjectTag)
	{
		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();


		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			_uint iPassIndex = 0;
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_SPECULAR, "g_SpecularTexture")))
			{

			}

			if (i != 2)
			{
				if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 12)))
					return E_FAIL;
			}
		}
	}
	else
	{
		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			_uint iPassIndex = 0;

			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
			//if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			//	iPassIndex = 0;
			//else
			//	iPassIndex++;
			if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CProbs::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (true == m_bEnable)
		return S_OK;

	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;
	_float4 vCamPosition = GI->Get_CamPosition();
	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/
		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 10)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CProbs::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_NonAnim_Shader"), reinterpret_cast<CComponent**>(&m_pNonAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_") + m_strMapObjName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (TEXT("WitchForest_Plant_01") == m_strObjectTag)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Grass_MaskMap"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}


	return S_OK;
}

void CProbs::MoveProps(_float fTimeDelta)
{
	if (m_strMapObjName == TEXT("Evermore_DecoBuild_01"))
	{
		YRotation(1.5f, fTimeDelta);
		Vec4 vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);

		if (0 == m_iRandomCase)
		{
			if (m_iMoveTick > 0)
			{
				vPos.y += 3.0f * fTimeDelta;
				m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vPos);
			}
			else
			{
				vPos.y -= 3.0f * fTimeDelta;
				m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vPos);
			}
		}
		else if (1 == m_iRandomCase)
		{
			if (m_iMoveTick < 0)
			{
				vPos.y += 3.0f * fTimeDelta;
				m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vPos);
			}
			else
			{
				vPos.y -= 3.0f * fTimeDelta;
				m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vPos);
			}
		}


		--m_iMoveTick;

		if (-100 == m_iMoveTick)
			m_iMoveTick = 101;

	}
}

void CProbs::PrepareRainbowColor()
{
	if (TEXT("WitchForest_Prop_01a") == m_strObjectTag)
	{

		m_vRainbowColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
		//_uint iRandom = GI->RandomInt(0, 6);

		//switch (iRandom)
		//{
		//case RAINBOWCOLOR::RED:
		//	m_vRainbowColor = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
		//	break;
		//case RAINBOWCOLOR::ORANGE:
		//	m_vRainbowColor = Vec4(255.0f / 255.0f, 50.0f / 255.0f, 0.0f, 1.0f);
		//	break;
		//case RAINBOWCOLOR::YELLOW:
		//	m_vRainbowColor = Vec4(1.0f, 1.0f, 0.0f, 1.0f);
		//	break;
		//case RAINBOWCOLOR::GREEN:
		//	m_vRainbowColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
		//	break;
		//case RAINBOWCOLOR::BLUE:
		//	m_vRainbowColor = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
		//	break;
		//case RAINBOWCOLOR::INDIGO:
		//	m_vRainbowColor = Vec4(0.0f, 5.0f / 255.0f, 1.0f, 1.0f);
		//	break;
		//case RAINBOWCOLOR::PURPLE:
		//	m_vRainbowColor = Vec4(100.0f / 255.0f, 0.0f, 1.0f, 1.0f);
		//	break;
		//default:
		//	break;
		//}
	}
}

CProbs* CProbs::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag,
	const wstring& strFilePath, const wstring& strFileName, _int eObjType, _uint eModelType)
{
	CProbs* pInstance = new CProbs(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, strFilePath, strFileName)))
	{
		MSG_BOX("Create Failed to ProtoType : CProbs");
		Safe_Release<CProbs*>(pInstance);
	}

	return pInstance;
}

CGameObject* CProbs::Clone(void* pArg)
{
	CProbs* pInstance = new CProbs(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CProbs");
		Safe_Release<CProbs*>(pInstance);
	}

	return pInstance;
}

void CProbs::Free()
{
	__super::Free();

	Safe_Release<CTexture*>(m_pTextureCom);
}
