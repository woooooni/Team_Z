#include "stdafx.h"
#include "UI_Flare.h"
#include "GameInstance.h"

CUI_Flare::CUI_Flare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Flare")
{
}

CUI_Flare::CUI_Flare(const CUI_Flare& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Flare::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Flare::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 0.1f;
	m_bActive = false;

	return S_OK;
}

void CUI_Flare::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!m_bReverse)
		{
			m_fAlpha -= fTimeDelta;

			if (0.1f > m_fAlpha)
			{
				m_bReverse = true;
				m_fAlpha = 0.1f;
			}
		}
		else
		{
			m_fAlpha += fTimeDelta;

			if (1.f < m_fAlpha)
			{
				m_bReverse = false;
				m_fAlpha = 1.f;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Flare::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Flare::Render()
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

HRESULT CUI_Flare::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo_Flare"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Flare::Ready_State()
{
	// 원본이미지 3배
	m_tInfo.fCX = 128.f * 3.f;
	m_tInfo.fCY = 128.f * 3.f;

	m_tInfo.fX = g_iWinSizeX * 0.5f + 185.f;
	m_tInfo.fY = g_iWinSizeY * 0.5f - 140.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.1f, 1.f));

	return S_OK;
}

HRESULT CUI_Flare::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Flare* CUI_Flare::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Flare* pInstance = new CUI_Flare(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Flare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Flare::Clone(void* pArg)
{
	CUI_Flare* pInstance = new CUI_Flare(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Flare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Flare::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
