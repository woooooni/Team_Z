#include "stdafx.h"
#include "..\Public\SkyPlane.h"
#include "GameInstance.h"


CSkyPlane::CSkyPlane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
}

CSkyPlane::CSkyPlane(const CSkyPlane& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyPlane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkyPlane::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	SkyPlaneRSCreate();

	return S_OK;
}

void CSkyPlane::Tick(_float fTimeDelta)
{
	//m_fTextureTranslation[0] += m_fTranslationSpeed[0]; // firstX
	//m_fTextureTranslation[1] += m_fTranslationSpeed[1]; // firstZ
	//m_fTextureTranslation[2] += m_fTranslationSpeed[2]; // secondX
	//m_fTextureTranslation[3] += m_fTranslationSpeed[3]; // secondZ

	//if (m_fTextureTranslation[0] > 1.0f) m_fTextureTranslation[0] -= 1.0f;
	//if (m_fTextureTranslation[1] > 1.0f) m_fTextureTranslation[1] -= 1.0f;
	//if (m_fTextureTranslation[2] > 1.0f) m_fTextureTranslation[2] -= 1.0f;
	//if (m_fTextureTranslation[3] > 1.0f) m_fTextureTranslation[3] -= 1.0f;

	//m_pTransformCom->Move(Vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.05f, fTimeDelta);

	m_fTranslation += 0.0001f;
	if (m_fTranslation > 1.0f)
		m_fTranslation -= 1.0f;
}

void CSkyPlane::LateTick(_float fTimeDelta)
{
	Vec4 vCamePos = GI->Get_CamPosition();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamePos);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY, this);
}

HRESULT CSkyPlane::Render()
{
	//m_pContext->RSSetState(m_pRasterStateNoCulling);


	if (FAILED(m_pShaderCom->Bind_RawValue("worldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("viewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("projectionMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("fTranslation", &m_fTranslation, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("fScale", &m_fScale, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("fbrightness", &m_fBrightness, sizeof(_float))))
		return E_FAIL;

	if (m_strObjectTag == TEXT("Sky_Plane"))
	{
		if (FAILED(m_pTextureCom[PLANE_TEX::CLOUDE_NOISE]->Bind_ShaderResource(m_pShaderCom, "cloudTexture")))
			return E_FAIL;
		if (FAILED(m_pTextureCom[PLANE_TEX::CLOUDE_PERT]->Bind_ShaderResource(m_pShaderCom, "perturbTexture")))
			return E_FAIL;

	/*	if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;*/
		if (FAILED(m_pModelCom->Render(m_pShaderCom, 0)))
			return E_FAIL;
	}
	else if (m_strObjectTag == TEXT("Sky_Plane2"))
	{

		_int iIndex = m_pTextureCom[PLANE_TEX::CLOUDE_NOISE]->Find_Index(TEXT("T_Image_014"));
		if (FAILED(m_pTextureCom[PLANE_TEX::CLOUDE_NOISE]->Bind_ShaderResource(m_pShaderCom, "WinterNoiseTexture", iIndex)))
			return E_FAIL;
		iIndex = m_pTextureCom[PLANE_TEX::CLOUDE_NOISE2]->Find_Index(TEXT("T_Noise_702"));
		if (FAILED(m_pTextureCom[PLANE_TEX::CLOUDE_NOISE2]->Bind_ShaderResource(m_pShaderCom, "WinterNoiseTexture2", iIndex)))
			return E_FAIL;
		if (FAILED(m_pTextureCom[PLANE_TEX::CLOUDE_PERT]->Bind_ShaderResource(m_pShaderCom, "perturbTexture")))
			return E_FAIL;
		if (FAILED(m_pTextureCom[PLANE_TEX::CLOUDE_AURA]->Bind_ShaderResource(m_pShaderCom, "WinterAuraTexture", 0)))
			return E_FAIL;

		/*if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;*/
		if (FAILED(m_pModelCom->Render(m_pShaderCom, 0, 1)))
			return E_FAIL;
	}

	
	//if (FAILED(m_pSkyPlaneCom->Render()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CSkyPlane::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Plane"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Sky_Plane"),
	//	TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pSkyPlaneCom))))
	//	return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SkyPlane"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (m_strObjectTag == TEXT("Sky_Plane"))
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sky_Cloud"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[PLANE_TEX::CLOUDE_NOISE]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sky_Cloud2"),
			TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[PLANE_TEX::CLOUDE_PERT]))))
			return E_FAIL;
	}
	else if (m_strObjectTag == TEXT("Sky_Plane2"))
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Image"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[PLANE_TEX::CLOUDE_NOISE]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sky_Cloud2"),
			TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[PLANE_TEX::CLOUDE_PERT]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Aura"),
			TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom[PLANE_TEX::CLOUDE_AURA]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
			TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom[PLANE_TEX::CLOUDE_NOISE2]))))
			return E_FAIL;
	}



	return S_OK;
}

HRESULT CSkyPlane::SkyPlaneRSCreate()
{
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterStateNoCulling)))
		return E_FAIL;

	return S_OK;
}

CSkyPlane* CSkyPlane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CSkyPlane* pInstance = new CSkyPlane(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyPlane");
		Safe_Release<CSkyPlane*>(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyPlane::Clone(void* pArg)
{
	CSkyPlane* pInstance = new CSkyPlane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkyPlane");
		Safe_Release<CSkyPlane*>(pInstance);
	}

	return pInstance;
}

void CSkyPlane::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);

	for(_uint i = 0; i < PLANE_TEX::TEX_END; ++i)
		Safe_Release<CTexture*>(m_pTextureCom[i]);

	Safe_Release<CModel*>(m_pModelCom);
	Safe_Release<CVIBuffer_SkyPlane*>(m_pSkyPlaneCom);

	Safe_Release<ID3D11RasterizerState*>(m_pRasterStateNoCulling);
}
