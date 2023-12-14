#include "stdafx.h"
#include "UI_LevelUp.h"
#include "GameInstance.h"

CUI_LevelUp::CUI_LevelUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UILEVELUP_TYPE eType)
	: CUI(pDevice, pContext, L"UI_LevelUp")
	, m_eType(eType)
{
}

CUI_LevelUp::CUI_LevelUp(const CUI_LevelUp& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_LevelUp::Set_Active(_bool bActive)
{
	if (bActive)
	{
		m_bSetAlpha = false;
		m_fTimeAcc = 0.f;
		m_fAlpha = 0.1f;
	}

	m_bActive = bActive;
}

HRESULT CUI_LevelUp::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LevelUp::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CUI_LevelUp::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_bSetAlpha)
		{
			if (m_fTimeAcc > 1.f)
			{
				if (m_fAlpha < 0.1f)
				{
					m_bActive = false;
					m_fAlpha = 0.f;
				}
				else
					m_fAlpha -= fTimeDelta * 0.3f;
			}
		}
		else
		{
			// MapName이 활성화 되면
			if (m_fTimeAcc > 0.1f) // 프레임을 돌린다.
			{
//				if (m_iTextureIndex < 6)
//					m_iTextureIndex++;

				m_fTimeAcc = 0.f;
			}
			
			if (m_fAlpha > 1.f)
				m_fAlpha = 1.f;
			else
				m_fAlpha += fTimeDelta;
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_LevelUp::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{


		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_LevelUp::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_LevelUp::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	switch (m_eType)
	{
	case UILEVELUP_TYPE::UILEVELUP_FRAME:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LevelUp_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UILEVELUP_TYPE::UILEVELUP_BG:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LevelUp_Background"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CUI_LevelUp::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_LevelUp::Bind_ShaderResources()
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

CUI_LevelUp* CUI_LevelUp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UILEVELUP_TYPE eType)
{
	CUI_LevelUp* pInstance = new CUI_LevelUp(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_LevelUp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LevelUp::Clone(void* pArg)
{
	CUI_LevelUp* pInstance = new CUI_LevelUp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_LevelUp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LevelUp::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
