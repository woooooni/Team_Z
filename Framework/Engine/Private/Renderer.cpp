#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Utils.h"
#include "GameInstance.h"
#include "Shader.h"
#include "PhysX_Manager.h"
#include "..\..\Client\Public\Effect.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::Initialize_Prototype()
{
	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

#pragma region MRT_GameObjects : Target_Diffuse / Target_Normal / Target_Depth
	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Lights : Target_Shade / Target_Specular
	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Shadow : Target_ShadowDepth
	/* For.Target_ShadowDepth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth"),
		ViewportDesc.Width * 3.f, ViewportDesc.Height * 3.f, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
#pragma endregion    
	

#pragma region MRT_Effect : Target_Effect_Diffuse / Target_Effect_Brightness
	/* For.Target_Effect */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Diffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Effect_Brightness */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Brightness"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Blur */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Blur"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_EffectUI : Target_Effect_UI_Diffuse / Target_Effect_UI_Brightness
	/* For.Target_Effect_UI_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_UI_Diffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Effect_UI_Brightness */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_UI_Brightness"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion



#pragma region MRT_Blend : Target_Blend // FinalTarget
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blend"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.729f, 0.816f, 0.851f, 0.f))))
		return E_FAIL;
#pragma endregion



	///* For.Target_Bloom */
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Bloom"),
	//	ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
	//	return E_FAIL;

	///* For.Target_Blur_Effect */
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_Effect"),
	//	ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
	//	return E_FAIL;

	///* For.Target_Blur_Bloom */
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_Bloom"),
	//	ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
	//	return E_FAIL;

	// For.Target_Blur_DownSampling
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_DownSampling"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Blur_Horizontal
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_Horizontal"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Blur_Vertical
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_Vertical"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Blur_UpSampling
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_UpSampling"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// MRT_GameObjects
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
			return E_FAIL;
	}

	// MRT_Lights
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Specular"))))
			return E_FAIL;
	}

	// MRT_Shadow
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_ShadowDepth"))))
			return E_FAIL;
	}

	// MRT_Effect
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect_Diffuse"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect_Brightness"))))
			return E_FAIL;
	}

	// MRT_Effect_UI
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect_UI"), TEXT("Target_Effect_UI_Diffuse"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect_UI"), TEXT("Target_Effect_UI_Brightness"))))
			return E_FAIL;
	}
	
	// MRT_Blend
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Blend", L"Target_Blend")))
			return E_FAIL;
	}


	//Blur
	{
		///* For.MRT_Blur_Effect */
		//if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Effect"), TEXT("Target_Blur_Effect"))))
		//	return E_FAIL;

		///* For.MRT_Blur_Bloom */
		//if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Bloom"), TEXT("Target_Blur_Bloom"))))
		//	return E_FAIL;


		/* For.MRT_BlurDownSampling */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_DownSampling"), TEXT("Target_Blur_DownSampling"))))
			return E_FAIL;

		/* For.MRT_Blur_Horizontal */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Horizontal"), TEXT("Target_Blur_Horizontal"))))
			return E_FAIL;

		/* For.MRT_Blur_Vertical */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Vertical"), TEXT("Target_Blur_Vertical"))))
			return E_FAIL;

		/* For.MRT_BlurUpSampling */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_UpSampling"), TEXT("Target_Blur_UpSampling"))))
			return E_FAIL;


		/* For.MRT_BlurUpSampling */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Effect"), TEXT("Target_Effect_Blur"))))
			return E_FAIL;
	}

	// Debug_Ready
#ifdef _DEBUG

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 100.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 100.f, 500.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 300.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 300.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth"), 300.f, 500.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Diffuse"), 1300, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Brightness"), 1300, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Blur"), 1300, 500.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_UI_Diffuse"), 1500, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_UI_Brightness"), 1500, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Blend"), 1500, 500.f, 200.f, 200.f)))
		return E_FAIL;

#endif // DEBUG

#pragma region m_pVIBuffer / m_pShader / m_pIntancingShaders
	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pIntancingShaders[INSTANCING_SHADER_TYPE::ANIM_MODEL] = CShader::Create(m_pDevice, m_pContext,
		TEXT("../Bin/ShaderFiles/Shader_AnimModel_Instance.hlsl"),
		VTXANIMMODELINSTANCE_DECLARATION::Elements,
		VTXANIMMODELINSTANCE_DECLARATION::iNumElements);

	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::ANIM_MODEL])
		return E_FAIL;

	m_pIntancingShaders[INSTANCING_SHADER_TYPE::MODEL] = CShader::Create(m_pDevice, m_pContext,
		TEXT("../Bin/ShaderFiles/Shader_Model_Instance.hlsl"),
		VTXMODELINSTANCE_DECLARATION::Elements,
		VTXMODELINSTANCE_DECLARATION::iNumElements);

	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::MODEL])
		return E_FAIL;

	m_pIntancingShaders[INSTANCING_SHADER_TYPE::RECT] = CShader::Create(m_pDevice, m_pContext,
		TEXT("../Bin/ShaderFiles/Shader_Rect_Instance.hlsl"), 
		VTXRECTINSTANCE_DECLARATION::Elements, VTXRECTINSTANCE_DECLARATION::iNumElements);

	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::RECT])
		return E_FAIL;

	m_pIntancingShaders[INSTANCING_SHADER_TYPE::EFFECT_TEXTURE] = CShader::Create(m_pDevice, m_pContext,
		TEXT("../Bin/ShaderFiles/Shader_TextureEffect_Instance.hlsl"), 
		VTXRECTINSTANCE_DECLARATION::Elements, 
		VTXRECTINSTANCE_DECLARATION::iNumElements);

	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::EFFECT_TEXTURE])
		return E_FAIL;

	m_pIntancingShaders[INSTANCING_SHADER_TYPE::EFFECT_MODEL] = CShader::Create(m_pDevice, m_pContext,
		TEXT("../Bin/ShaderFiles/Shader_MeshEffect_Instance.hlsl"), VTXMODELINSTANCE_DECLARATION::Elements, VTXMODELINSTANCE_DECLARATION::iNumElements);

	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::EFFECT_MODEL])
		return E_FAIL;


	m_pVIBuffer_Instancing = CVIBuffer_Instancing::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer_Instancing)
		return E_FAIL;
#pragma endregion

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = ViewportDesc.Width;
	m_WorldMatrix._22 = ViewportDesc.Height;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	return S_OK;
}

HRESULT CRenderer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pGameObject);
	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_Instancing(RENDERGROUP eRenderGroup, INSTANCING_SHADER_TYPE eShaderType, CGameObject* pGameObject, _float4x4 WorldMatrix)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	auto iter = m_Render_Instancing_Objects[eRenderGroup].find(pGameObject->Get_PrototypeTag());
	if (iter == m_Render_Instancing_Objects[eRenderGroup].end())
	{
		INSTANCING_DESC InstancingDesc;
		InstancingDesc.pGameObject = pGameObject;
		Safe_AddRef(pGameObject);

		InstancingDesc.eShaderType = eShaderType;
		InstancingDesc.WorldMatrices.reserve(100);
		InstancingDesc.WorldMatrices.push_back(WorldMatrix);

		m_Render_Instancing_Objects[eRenderGroup].emplace(pGameObject->Get_PrototypeTag(), InstancingDesc);
	}
	else
	{
		if (nullptr == iter->second.pGameObject)
		{
			iter->second.pGameObject = pGameObject;
			Safe_AddRef(pGameObject);
		}

		iter->second.WorldMatrices.push_back(WorldMatrix);
	}
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_Instancing_Effect(RENDERGROUP eRenderGroup, INSTANCING_SHADER_TYPE eShaderType, CGameObject* pGameObject, _float4x4 WorldMatrix, const EFFECT_INSTANCE_DESC& EffectInstanceDesc)
{

	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	auto iter = m_Render_Instancing_Objects[eRenderGroup].find(pGameObject->Get_PrototypeTag());
	if (iter == m_Render_Instancing_Objects[eRenderGroup].end())
	{
		INSTANCING_DESC InstancingDesc;
		InstancingDesc.pGameObject = pGameObject;
		Safe_AddRef(pGameObject);

		InstancingDesc.eShaderType = eShaderType;
		InstancingDesc.WorldMatrices.reserve(500);
		InstancingDesc.WorldMatrices.push_back(WorldMatrix);

		InstancingDesc.EffectInstancingDesc.reserve(500);
		InstancingDesc.EffectInstancingDesc.push_back(EffectInstanceDesc);

		m_Render_Instancing_Objects[eRenderGroup].emplace(pGameObject->Get_PrototypeTag(), InstancingDesc);
	}
	else
	{
		if (nullptr == iter->second.pGameObject)
		{
			iter->second.pGameObject = pGameObject;
			Safe_AddRef(pGameObject);
		}
		
		if (iter->second.WorldMatrices.size() <= 500)
		{
			iter->second.WorldMatrices.push_back(WorldMatrix);
			iter->second.EffectInstancingDesc.push_back(EffectInstanceDesc);
		}
		
	}
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_AnimInstancing(RENDERGROUP eRenderGroup, CGameObject* pGameObject, _float4x4 WorldMatrix, const TweenDesc& TweenInstanceDesc)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	auto iter = m_Render_Instancing_Objects[eRenderGroup].find(pGameObject->Get_PrototypeTag());
	if (iter == m_Render_Instancing_Objects[eRenderGroup].end())
	{
		INSTANCING_DESC InstancingDesc;
		InstancingDesc.pGameObject = pGameObject;
		Safe_AddRef(pGameObject);

		InstancingDesc.eShaderType = INSTANCING_SHADER_TYPE::ANIM_MODEL;
		InstancingDesc.WorldMatrices.reserve(500);
		InstancingDesc.WorldMatrices.push_back(WorldMatrix);

		InstancingDesc.TweenDesc.reserve(500);
		InstancingDesc.TweenDesc.push_back(TweenInstanceDesc);

		m_Render_Instancing_Objects[eRenderGroup].emplace(pGameObject->Get_PrototypeTag(), InstancingDesc);
	}

	else
	{
		if (nullptr == iter->second.pGameObject)
		{
			iter->second.pGameObject = pGameObject;
			Safe_AddRef(pGameObject);
		}

		if (iter->second.WorldMatrices.size() <= 500)
		{
			iter->second.WorldMatrices.push_back(WorldMatrix);
			iter->second.TweenDesc.push_back(TweenInstanceDesc);
		}
	}

	return S_OK;
}


HRESULT CRenderer::Draw()
{
	if (KEY_TAP(KEY::F2))
		m_bDebugDraw = !m_bDebugDraw;


	if(FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonLight()))
		return E_FAIL;


	if (FAILED(Render_NonAlphaBlend()))
		return E_FAIL;
	if (FAILED(Render_Shadow()))
		return E_FAIL;
	if(FAILED(Render_Lights()))
		return E_FAIL;


	if(FAILED(Render_Deferred()))
		return E_FAIL;


	if(FAILED(Render_AlphaBlend()))
		return E_FAIL;

	if (FAILED(Render_Effect()))
		return E_FAIL;

	// 1번째 타겟에 있는 색상을 2번째 타겟에 그려낸다.
	if (FAILED(Render_Blur(L"Target_Effect_Brightness", L"MRT_Blur_Effect", true)))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_Text()))
		return E_FAIL;

	if (FAILED(Render_UIEffectNonBlend()))
		return E_FAIL;

	if (FAILED(Render_UIEffectBlend()))
		return E_FAIL;

	/* 
	if (FAILED(Render_Blur(L"Target_Bloom", L"MRT_Blur_Bloom", true)))
		return E_FAIL;*/

	// FinalRender
	if (FAILED(Render_Final()))
		return E_FAIL;

	if (FAILED(Render_Cursor()))
		return E_FAIL;


#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#endif // DEBUG

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_Priority()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_PRIORITY])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_PRIORITY])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc)))
			return E_FAIL;

		Pair.second.EffectInstancingDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.TweenDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// X
HRESULT CRenderer::Render_NonLight()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_NONLIGHT])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_NONLIGHT].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_NONLIGHT])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc)))
			return E_FAIL;

		Pair.second.EffectInstancingDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.TweenDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	return S_OK;
}

// MRT_GameObjects
HRESULT CRenderer::Render_NonAlphaBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_NONBLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}

	m_RenderObjects[RENDER_NONBLEND].clear();


	for (auto& Pair : m_Render_Instancing_Objects[RENDER_NONBLEND])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc)))
			return E_FAIL;


		Pair.second.EffectInstancingDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.TweenDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Shadow
HRESULT CRenderer::Render_Shadow()
{
	if (FAILED(m_pTarget_Manager->Begin_Shadow_MRT(m_pContext, TEXT("MRT_Shadow"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_SHADOW])
	{
		/*if (FAILED(iter->Render_ShadowDepth()))
			return E_FAIL;*/
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_SHADOW].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_SHADOW])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		/*if (FAILED(Pair.second.pGameObject->Render_Instance_Shadow(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;*/

		//if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel_Shadow(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc)))
		//	return E_FAIL;

		Pair.second.EffectInstancingDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.TweenDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Lights
HRESULT CRenderer::Render_Lights()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Lights"))))
		return E_FAIL;

	/* 사각형 버퍼를 직교투영으로 Shade타겟의 사이즈만큼 꽉 채워서 그릴꺼야. */
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	
	_float4x4 ProjMatrixInv, ViewMatrixInv;
	ProjMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_PROJ);
	ViewMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", &ViewMatrixInv)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", &ProjMatrixInv)))
		return E_FAIL;
	
	_float4 vCamPosition = GI->Get_CamPosition();
	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Normal"), "g_NormalTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Depth"), "g_DepthTarget")))
		return E_FAIL;

	m_pLight_Manager->Render(m_pShader, m_pVIBuffer);

	/* 다시 장치의 0번째 소켓에 백 버퍼를 올린다. */
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_Deferred()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;


	/* 디퓨즈 타겟과 셰이드 타겟을 서로 곱하여 백버퍼에 최종적으로 찍어낸다. */
	_float4 vCamPosition = GI->Get_CamPosition();
	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;


	if (KEY_TAP(KEY::OPEN_SQUARE_BRACKET))
	{
		if (KEY_HOLD(KEY::SHIFT))
		{
			m_fBias += 0.0001f;
		}
		else
		{
			m_fBias += 0.001f;
		}
		
	}

	if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET))
	{
		if (KEY_HOLD(KEY::SHIFT))
		{
			m_fBias -= 0.0001f;
		}
		else
		{
			m_fBias -= 0.001f;
		}

	}

	if (FAILED(m_pShader->Bind_RawValue("g_fBias", &m_fBias, sizeof(_float))))
		return E_FAIL;


	/*if (FAILED(m_pShader->Bind_RawValue("g_vFogColor", &m_vFogColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_vFogStartEnd", &m_vFogStartEnd, sizeof(_float2))))
		return E_FAIL;*/

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	_float4x4 ViewMatrixInv;
	ViewMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", &ViewMatrixInv)))
		return E_FAIL;

	_float4x4 ProjMatrixInv;
	ProjMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", &ProjMatrixInv)))
		return E_FAIL;


	_float4x4 CamProjMatrix = GI->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShader->Bind_Matrix("g_CamProjMatrix", &CamProjMatrix)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Diffuse"), "g_DiffuseTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Normal"), "g_NormalTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Depth"), "g_DepthTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Shade"), "g_ShadeTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Specular"), "g_SpecularTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_ShadowDepth"), "g_ShadowTarget")))
		return E_FAIL;

	
	_float4x4 LightMatix = GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel());
	if (FAILED(m_pShader->Bind_Matrix("g_LightViewMatrix", &LightMatix)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_AlphaBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_ALPHABLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_ALPHABLEND].clear();


	for (auto& Pair : m_Render_Instancing_Objects[RENDER_ALPHABLEND])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc)))
			return E_FAIL;

		Pair.second.EffectInstancingDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.TweenDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	
	return S_OK;
}

// MRT_Effect
HRESULT CRenderer::Render_Effect()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Effect"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_EFFECT])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_EFFECT].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_EFFECT])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_EffectDesc", Pair.second.EffectInstancingDesc.data(), sizeof(EFFECT_INSTANCE_DESC) * Pair.second.WorldMatrices.size())))
			return E_FAIL;

 		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		
		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc)))
			return E_FAIL;

		Pair.second.EffectInstancingDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.TweenDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_UI()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_UI])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_UI].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_Text()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderTexts)
	{
		if (FAILED(GI->Render_Fonts(iter.strFontTag, iter.strText,
			iter.vPosition,
			XMLoadFloat4(&iter.vColor),
			iter.fAngle,
			iter.vOrigin,
			iter.vScale)))
			return E_FAIL;
	}
	m_RenderTexts.clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Effect_UI
HRESULT CRenderer::Render_UIEffectNonBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Effect_UI"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_UIEFFECT_NONBLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_UIEFFECT_NONBLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_UIEffectBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_UIEFFECT_BLEND])
	{
 		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_UIEFFECT_BLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// Final
HRESULT CRenderer::Render_Final()
{
	// Diffuse
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, L"Target_Blend", "g_DiffuseTarget")))
		return E_FAIL;

	// Effect
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Effect_Diffuse"), "g_EffectDiffuseTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Effect_Brightness"), "g_EffectBrightnessTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Effect_Blur"), "g_EffectBlurTarget")))
		return E_FAIL;

	// Effect_UI
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Effect_UI_Diffuse"), "g_EffectUIDiffuseTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Effect_UI_Brightness"), "g_EffectUIBrightnessTarget")))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Bloom"), "g_OriginBloomTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Blur_Effect"), "g_BlurEffectTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Blur_Bloom"), "g_BlurBloomTarget")))
		return E_FAIL;*/

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Cursor()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_CURSOR])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_CURSOR].clear();

	return S_OK;
}


#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	for (auto& pDebugCom : m_RenderDebug)
	{
		if (true == m_bDebugDraw)
			pDebugCom->Render();
		
		Safe_Release(pDebugCom);
	}
	m_RenderDebug.clear();
	
	if (false == m_bDebugDraw)
		return S_OK;

	wstring strPlayerPosition = L"";
	strPlayerPosition += L"X : ";
	strPlayerPosition += to_wstring(m_vPlayerPosition.x);
	strPlayerPosition += L'\n';
	strPlayerPosition += L"Y : ";
	strPlayerPosition += to_wstring(m_vPlayerPosition.y);
	strPlayerPosition += L'\n';
	strPlayerPosition += L"Z : ";
	strPlayerPosition += to_wstring(m_vPlayerPosition.z);
	strPlayerPosition += L'\n';
	strPlayerPosition += L"W : ";
	strPlayerPosition += to_wstring(m_vPlayerPosition.w);


	GI->Render_Fonts(L"Basic", strPlayerPosition.c_str(), _float2(1600.f / 2.f, 0.f));

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(CPhysX_Manager::GetInstance()->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Lights"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Shadow"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Effect"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Blur_Effect"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Effect_UI"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Blend"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	return S_OK;
}
#endif // DEBUG

HRESULT CRenderer::Render_Blur(const wstring& strStartTargetTag , const wstring& strFinalTragetTag, _bool bClear)
{
	if (FAILED(Render_BlurDownSample(strStartTargetTag)))
		return E_FAIL;

	if (FAILED(Render_Blur_Horizontal()))
		return E_FAIL;

	if (FAILED(Render_Blur_Vertical()))
		return E_FAIL;

	if (FAILED(Render_BlurUpSample(strFinalTragetTag, bClear)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BlurDownSample(const wstring& strTargetTag)
{
	// 블러(DownSample)
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, L"MRT_Blur_DownSampling")))
		return E_FAIL;

	_float4x4 WorldMatrix = m_WorldMatrix;
	WorldMatrix._11 = m_WorldMatrix._11 * 0.5f;
	WorldMatrix._22 = m_WorldMatrix._22 * 0.5f;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, strTargetTag, "g_BlurTarget")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(5)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))

		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blur_Horizontal()
{

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, L"MRT_Blur_Horizontal")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Blur_DownSampling"), "g_BlurTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Effect_Brightness"), "g_BlurPowerTarget")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	if (FAILED(m_pShader->Begin(6)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blur_Vertical()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, L"MRT_Blur_Vertical")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Blur_Horizontal"), "g_BlurTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Effect_Brightness"), "g_BlurPowerTarget")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	if (FAILED(m_pShader->Begin(7)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BlurUpSample(const wstring& strFinalMrtTag, _bool bClear)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, strFinalMrtTag, bClear)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Blur_Vertical"), "g_BlurTarget")))
		return E_FAIL;

	_float4x4 WorldMatrix = m_WorldMatrix;
	WorldMatrix._11 = m_WorldMatrix._11 * 2.f;
	WorldMatrix._22 = m_WorldMatrix._22 * 2.f;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(8)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pObj : m_RenderObjects[i])
			Safe_Release(pObj);

		m_RenderObjects[i].clear();
	}


	for (auto& pComponent : m_RenderDebug)
		Safe_Release(pComponent);

	m_RenderDebug.clear();


	Safe_Release(m_pVIBuffer_Instancing);


	for (_uint i = 0; i < INSTANCING_SHADER_TYPE::TYPE_END; ++i)
		Safe_Release(m_pIntancingShaders[i]);


	for (_uint i = 0; i < RENDERGROUP::RENDER_END; ++i)
	{
		for (auto iter : m_Render_Instancing_Objects[i])
		{
			iter.second.EffectInstancingDesc.clear();
			iter.second.TweenDesc.clear();
			iter.second.WorldMatrices.clear();
			Safe_Release(iter.second.pGameObject);
		}

		m_Render_Instancing_Objects[i].clear();
	}
	
}
