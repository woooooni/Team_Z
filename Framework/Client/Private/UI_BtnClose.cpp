#include "stdafx.h"
#include "UI_BtnClose.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"

CUI_BtnClose::CUI_BtnClose(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BtnClose")
{
}

CUI_BtnClose::CUI_BtnClose(const CUI_BtnClose& rhs)
	: CUI(rhs)
{
}

void CUI_BtnClose::Set_Active(_bool bActive)
{
	if (bActive) // true일때
	{
		m_bUsable = false;
		m_fTimeAcc = 0.f;
	}
	else
	{
		if (m_bEvent)
		{
			m_bEvent = false;
			//CUI_Manager::GetInstance()->Set_UIClicked(m_bEvent);
		}
	}

	m_bActive = bActive;
}

HRESULT CUI_BtnClose::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnClose::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	m_bUsable = false;
	m_bUseMouse = true;

	return S_OK;
}

void CUI_BtnClose::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// 활성화 상태가 된다면 시간을 누적한다.
		if (!m_bUsable)
			m_fTimeAcc += fTimeDelta;

		if (0.2f < m_fTimeAcc)
		{
			//일정 시간이 지나면 사용할 수 있는 상태이다.
			m_bUsable = true;
			m_fTimeAcc = 0.f;
		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnClose::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_EFFECT_BLEND, this);
	}
}

HRESULT CUI_BtnClose::Render()
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

void CUI_BtnClose::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_BtnClose::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if(m_bUsable)
			Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_BtnClose::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_BtnClose::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_Close"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BtnClose::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnClose::Bind_ShaderResources()
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

void CUI_BtnClose::Key_Input(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			GI->Stop_Sound(CHANNELID::SOUND_UI2);
			GI->Play_Sound(TEXT("UI_Fx_MainHud_Btn_Inventory_1.mp3"), CHANNELID::SOUND_UI2, GI->Get_ChannelVolume(CHANNELID::SOUND_UI2));

			CUI_Manager::GetInstance()->Using_CloseButton();
		}
	}
}

CUI_BtnClose* CUI_BtnClose::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BtnClose* pInstance = new CUI_BtnClose(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BtnClose");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnClose::Clone(void* pArg)
{
	CUI_BtnClose* pInstance = new CUI_BtnClose(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BtnClose");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnClose::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
