#include "stdafx.h"
#include "UI_Minigame_Basic.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Player.h"
#include "TowerDefence_Manager.h"
#include "UIMinigame_Manager.h"

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

void CUI_Minigame_Basic::Set_Active(_bool bActive)
{
	if (true == bActive)
	{

	}
	else
	{
		if (m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIX_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END)
		{
			Play_Sound();

			m_fAlpha = 0.f;

			m_bStart = false;
			m_bEnd = false;

			m_tInfo.fCX = m_vMaxSize.x;
			m_tInfo.fCY = m_vMaxSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}
		else if (m_eType == GRANDPRIX_BIPLANE)
		{
			m_tInfo.fX = 600.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
	}

	m_bActive = bActive;
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

	if (FAILED(Ready_TextInformation()))
		return E_FAIL;

	if (m_eType == GRANDPRIX_SPACE)
	{
		m_vMaxSize = _float2(m_tInfo.fCX, m_tInfo.fCY); // OriginSize로 쓰임.
		m_vOriginSize = _float2(m_tInfo.fCX * 0.7f, m_tInfo.fCY * 0.7f); // MinSize로 쓰임.
	}

	return S_OK;
}

void CUI_Minigame_Basic::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIX_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END)
			Tick_Count(fTimeDelta);

		if (m_eType == GRANDPRIX_SPACE)
		{
			if (false == m_bResize)
			{
				// 작아진다
				m_tInfo.fCX -= fTimeDelta * m_vOriginSize.x * 5.f;
				m_tInfo.fCY -= fTimeDelta * m_vOriginSize.y * 5.f;

				if (m_tInfo.fCX <= m_vOriginSize.x)
				{
					m_bResize = true;
					m_tInfo.fCX = m_vOriginSize.x;
					m_tInfo.fCY = m_vOriginSize.y;
				}
			}
			else
			{
				// 커진다
				m_tInfo.fCX += fTimeDelta * m_vOriginSize.x * 5.f;
				m_tInfo.fCY += fTimeDelta * m_vOriginSize.y * 5.f;

				if (m_tInfo.fCX >= m_vMaxSize.x)
				{
					m_bResize = false;
					m_tInfo.fCX = m_vMaxSize.x;
					m_tInfo.fCY = m_vMaxSize.y;
				}
			}

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}

		if (m_eType == GRANDPRIX_GAUGEGLOW)
		{
			if (false == m_bStart)
			{
				m_fAlpha -= fTimeDelta;

				if (m_fAlpha <= 0.f)
				{
					m_bStart = true;
					m_fAlpha = 0.f;
				}
			}
			else
			{
				m_fAlpha += fTimeDelta;

				if (m_fAlpha >= 1.f)
				{
					m_bStart = false;
					m_fAlpha = 1.f;
				}
			}
		}

		if (m_eType == GRANDPRIX_BIPLANE)
		{
			if (m_tInfo.fX < 1120.f)
			{
				// fX 520.f의 거리를 2 * 22번의 SpaceBar Tap으로 가야함. 11.8씩 가야함.

				if (KEY_TAP(KEY::SPACE))
				{
					m_tInfo.fX += 11.8f;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION,
						XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
				}
			}
		}

		if (m_eType == GRANDPRIX_ERROR)
			m_fTimeAcc += fTimeDelta;


		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_Basic::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIX_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END)
			LateTick_Count(fTimeDelta);

		if (TOWERDEFENCE_GOLD == m_eType)
		{
			// 골드 텍스트
			CRenderer::TEXT_DESC TextDesc = {};
			TextDesc.strFontTag = L"Default_Bold";
			TextDesc.strText = to_wstring(CGame_Manager::GetInstance()->Get_Player()->Get_Gold());
			TextDesc.vColor = Vec4(1.f, 0.7f, 0.f, 1.f);
			TextDesc.vPosition = Vec2(m_tInfo.fX + 40.f - (TextDesc.strText.size() * 8.5f), m_tInfo.fY - 10.f);
			TextDesc.vScale = Vec2(0.45f, 0.45f);
			m_pRendererCom->Add_Text(TextDesc);
		}

		if (m_eType == GRANDPRIX_ERROR)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_MINIMAP_ICON, this);
		}
		else
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
		}
	}
}

HRESULT CUI_Minigame_Basic::Render()
{
	if (true == m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Minigame_Basic::Render_Minimap()
{
	if (m_bActive)
	{
		if (true == CUIMinigame_Manager::GetInstance()->Is_RaderError())
		{
			if (FAILED(Bind_ShaderResources()))
				return E_FAIL;

			m_pShaderCom->Begin(21);

			m_pVIBufferCom->Render();
		}
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
		if (KEY_TAP(KEY::LBTN) && m_eType == UI_MINIGAMEBASIC::TOWERDEFENCE_START)
		{
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("ui_gmk_korsica_start_01.wav"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

			CTowerDefence_Manager::GetInstance()->Start_Defence();
			return;
		}
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

	m_bActive = false;

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

	case UI_MINIGAMEBASIC::GRANDPRIX_FRAME:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_CLASSFRAME:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_SkillFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 0;
		m_bActive = true;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_SPECIALFRAME:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_SkillFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 1;
		m_bActive = true;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_READY:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 0;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_THREE:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text_Number"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 2;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_TWO:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text_Number"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 1;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_ONE:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text_Number"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 0;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_START:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 1;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_END:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 2;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_BIPLANE:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_BiplaneIcon"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_GAUGEBACK:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_GaugeBackground"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.6f;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_GAUGEGLOW:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_GaugeGlowBackground"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bStart = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_SPACE:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_SpaceIcon"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_ERROR:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_TextError"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Minigame_Basic::Ready_State()
{
	if (UI_MINIGAMEBASIC::GRANDPRIX_ERROR == m_eType)
	{
		m_tInfo.fCX = g_iWinSizeX * 0.8f;
		m_tInfo.fCY = 370.f * 0.8f;
		m_tInfo.fX = g_iWinSizeX * 0.5f;
		m_tInfo.fY = g_iWinSizeY * 0.5f;

		m_bActive = true;
	}

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

	if (GRANDPRIX_ERROR == m_eType)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fTimeAcc, sizeof(_float))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Minigame_Basic::Ready_TextInformation()
{
	if (!(m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
		m_eType == GRANDPRIX_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END))
		return S_OK;

	m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
	m_vMaxSize = _float2(m_vOriginSize.x * 10.f, m_vOriginSize.y * 10.f);

	m_tInfo.fCX = m_vMaxSize.x;
	m_tInfo.fCY = m_vMaxSize.y;
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));

	m_fAlpha = 0.f;

	return S_OK;
}

void CUI_Minigame_Basic::Tick_Count(_float fTimeDelta)
{
	if (true == m_bStart && false == m_bEnd)
	{
		if (m_tInfo.fCX <= m_vOriginSize.x)
			m_tInfo.fCX = m_vOriginSize.x;
		
		if (m_tInfo.fCY <= m_vOriginSize.y)
		{
			m_tInfo.fCY = m_vOriginSize.y;
			m_bEnd = true;
		}

		if (m_fAlpha > 1.f)
			m_fAlpha = 1.f;

		m_fAlpha += fTimeDelta;

		m_tInfo.fCX -= fTimeDelta * (m_vOriginSize.x * 10.f);
		m_tInfo.fCY -= fTimeDelta * (m_vOriginSize.y * 10.f);

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	}
}

void CUI_Minigame_Basic::LateTick_Count(_float fTimeDelta)
{

}

void CUI_Minigame_Basic::Play_Sound()
{
	if (m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO || m_eType == GRANDPRIX_ONE)
	{
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("Grandprix_CountDown_0.wav"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
	}
	else if (m_eType == GRANDPRIX_START)
	{
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("Grandprix_CountDown_1.wav"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
	}
	else if (m_eType == GRANDPRIX_END)
	{
//		GI->Stop_Sound(CHANNELID::SOUND_UI);
//		GI->Play_Sound(TEXT("sp_ie_spectra_room_teleport_whoosh_01.wav"), CHANNELID::SOUND_UI,
//			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
	}
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
