#include "stdafx.h"
#include "UI_SkillSection_BtnInteraction.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SkillSection_BtnInteraction::CUI_SkillSection_BtnInteraction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_SkillSection_BtnInteraction")
{
}

CUI_SkillSection_BtnInteraction::CUI_SkillSection_BtnInteraction(const CUI_SkillSection_BtnInteraction& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SkillSection_BtnInteraction::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_BtnInteraction::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	return S_OK;
}

void CUI_SkillSection_BtnInteraction::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_BtnInteraction::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_SkillSection_BtnInteraction::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

//void CUI_SkillSection_BtnInteraction::On_MouseEnter(_float fTimeDelta)
//{
//}
//
//void CUI_SkillSection_BtnInteraction::On_Mouse(_float fTimeDelta)
//{
//	if (m_bActive)
//	{
//		Key_Input(fTimeDelta);
//	}
//}
//
//void CUI_SkillSection_BtnInteraction::On_MouseExit(_float fTimeDelta)
//{
//}

HRESULT CUI_SkillSection_BtnInteraction::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_InteractionBtn"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SkillSection_BtnInteraction::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_BtnInteraction::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_SkillSection_BtnInteraction::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
	}
}

CUI_SkillSection_BtnInteraction* CUI_SkillSection_BtnInteraction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SkillSection_BtnInteraction* pInstance = new CUI_SkillSection_BtnInteraction(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_BtnInteraction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_BtnInteraction::Clone(void* pArg)
{
	CUI_SkillSection_BtnInteraction* pInstance = new CUI_SkillSection_BtnInteraction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_BtnInteraction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_BtnInteraction::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
