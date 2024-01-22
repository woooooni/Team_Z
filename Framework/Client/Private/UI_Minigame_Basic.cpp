#include "stdafx.h"
#include "UI_Minigame_Basic.h"
#include "GameInstance.h"

CUI_Minigame_Basic::CUI_Minigame_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIGAMEBASIC eType)
	: CUI(pDevice, pContext, L"UI_Minigame_Basic")
	, m_eType(eType)
{
}

CUI_Minigame_Basic::CUI_Minigame_Basic(const CUI_Minigame_Basic& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Minigame_Basic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Basic::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CUI_Minigame_Basic::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_Basic::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (TOWERDEFENCE_GOLD == m_eType)
		{
			// ��� �ؽ�Ʈ
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_Basic::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Minigame_Basic::On_MouseEnter(_float fTimeDelta)
{
	if (true == m_bActive)
	{
		if (TOWERDEFENCE_START == m_eType)
			m_iTextureIndex = 1;
	}
}

void CUI_Minigame_Basic::On_Mouse(_float fTimeDelta)
{
	if (true == m_bActive)
	{

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Minigame_Basic::On_MouseExit(_float fTimeDelta)
{
	if (true == m_bActive)
	{
		if (TOWERDEFENCE_START == m_eType)
			m_iTextureIndex = 0;

		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Minigame_Basic::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UI_MINIGAMEBASIC::TOWERDEFENCE_MENU:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_Background"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MINIGAMEBASIC::TOWERDEFENCE_START:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_StartBtn"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_bUseMouse = true;
		break;

	case UI_MINIGAMEBASIC::TOWERDEFENCE_GOLD:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_Gold"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Minigame_Basic::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_Basic::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (true == m_bUseIndex)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	return S_OK;
}

CUI_Minigame_Basic* CUI_Minigame_Basic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIGAMEBASIC eType)
{
	CUI_Minigame_Basic* pInstance = new CUI_Minigame_Basic(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Basic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Basic::Clone(void* pArg)
{
	CUI_Minigame_Basic* pInstance = new CUI_Minigame_Basic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Basic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Basic::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
