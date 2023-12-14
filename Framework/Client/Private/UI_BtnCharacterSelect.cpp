#include "stdafx.h"
#include "UI_BtnCharacterSelect.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CUI_BtnCharacterSelect::CUI_BtnCharacterSelect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_SELECTBTN_TYPE eBtnType, UI_SELECTBTN_CHARACTER ePlayerType)
	: CUI(pDevice, pContext, strObjectTag)
	, m_ePlayerType(ePlayerType)
	, m_eTextureType(eBtnType)
{
}

CUI_BtnCharacterSelect::CUI_BtnCharacterSelect(const CUI_BtnCharacterSelect& rhs)
	: CUI(rhs)
	, m_ePlayerType(rhs.m_ePlayerType)
	, m_eTextureType(rhs.m_eTextureType)
	, m_iTextureIndex(rhs.m_iTextureIndex)
{
}

HRESULT CUI_BtnCharacterSelect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnCharacterSelect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (BTN_CLICKED == m_eTextureType)
	{
		// ����� ���� ������ �����Ѵ�.
		m_vOriginSize.x = m_tInfo.fCX;
		m_vOriginSize.y = m_tInfo.fCY;

		m_vMaxSize.x = m_tInfo.fCX * 1.5f;
		m_vMaxSize.y = m_tInfo.fCY * 1.5f;

		// ��ġ�� ���� ������ �����Ѵ�.
		m_vOriginPosition.x = m_tInfo.fX;
		m_vOriginPosition.y = m_tInfo.fY;

		m_vGoalPosition.x = m_tInfo.fX + 40.f;
	}

	return S_OK;
}

void CUI_BtnCharacterSelect::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (BTN_CLICKED == m_eTextureType)
		{
			if (!m_bArrived)
			{
				// Ŀ����
				if (m_tInfo.fCX > m_vMaxSize.x) // ����� ����ũ�� �̻����� Ŀ���� �ʵ��� �Ѵ�.
				{
					m_tInfo.fCX = m_vMaxSize.x;
					m_tInfo.fCY = m_vMaxSize.y;
				}
				else
				{
					m_tInfo.fCX += fTimeDelta * 50.f;
					m_tInfo.fCY += fTimeDelta * 50.f;
				}

				// �����δ�
				if (m_tInfo.fX >= m_vGoalPosition.x)
				{
					m_bArrived = true;

					m_tInfo.fX = m_vGoalPosition.x;
				}
				else
				{
					m_tInfo.fX += fTimeDelta * 100.f;
				}

				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
			}
			else // Arrived�� True�϶�
			{
				if (m_bMoveStart) // ����ġ�� ���ƿ���� ������ ������
				{
					// �۾�����
					if (m_tInfo.fCX < m_vOriginSize.x) // ����� ����ũ�� ���Ϸ� �۾����� �ʵ��� �Ѵ�.
					{
						m_tInfo.fCX = m_vOriginSize.x;
						m_tInfo.fCY = m_vOriginSize.y;

						m_bArrived = false;
					}
					else
					{
						m_tInfo.fCX -= fTimeDelta * 50.f;
						m_tInfo.fCY -= fTimeDelta * 50.f;
					}

					// �����δ�
					if (m_tInfo.fX < m_vOriginPosition.x)
					{
						m_tInfo.fX = m_vOriginPosition.x;

						m_bMoveEnd = true;

						m_bActive = false;
						m_bMoveStart = false;
					}
					else
					{
						m_tInfo.fX -= fTimeDelta * 100.f;
					}

					m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
					m_pTransformCom->Set_State(CTransform::STATE_POSITION,
						XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
				}
			}

		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnCharacterSelect::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BtnCharacterSelect::Render()
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

void CUI_BtnCharacterSelect::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
	// ������ ����
}

void CUI_BtnCharacterSelect::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		// Ŭ�� �� �ؽ�ó�� ���콺 ��Ŭ���� �ϸ� m_bClicked�� true�� ��ȯ�ȴ�
		// UIManager���� Loop�� ���Ƽ� Clicked True�� �����Ǹ� Unclicked Texture Active�� False�� �ٲٰ�
		// ������ �÷��̾��� Clicked Texture�� Active���� True�� �ٲ��ش�.
		// -> True�� �Ǹ� �������� ���۵ȴ�.
		if (KEY_TAP(KEY::LBTN))
		{
			if (BTN_UNCLIKED == m_eTextureType)
			{
				if (!m_bClicked)
					m_bClicked = true;
			}
		}
	}
}

void CUI_BtnCharacterSelect::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

void CUI_BtnCharacterSelect::Reset_InitializeInfo()
{
	if (BTN_CLICKED == m_eTextureType)
	{
		m_bActive = false;
		m_bClicked = false;
		m_bArrived = false;
		m_bMoveStart = false;
		m_bMoveEnd = false;
	}
}

HRESULT CUI_BtnCharacterSelect::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eTextureType)
	{
	case BTN_UNCLIKED:
		Ready_UnclickedTexture();
		break;

	case BTN_CLICKED:
		Ready_ClickedTexture();
		break;
	}
	
	return S_OK;
}

HRESULT CUI_BtnCharacterSelect::Ready_UnclickedTexture()
{
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Unclick_Texture"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	switch (m_ePlayerType)
	{
	case BTN_ROGUE:
		m_iTextureIndex = 2;
		break;

	case BTN_DESTROYER:
		m_iTextureIndex = 0;
		break;

	case BTN_WITCH:
		m_iTextureIndex = 4;
		break;

	case BTN_ENGINEER:
		m_iTextureIndex = 1;
		break;

	case BTN_SWORDMAN:
		m_iTextureIndex = 3;
		break;
	}

	m_bActive = true;

	return S_OK;
}

HRESULT CUI_BtnCharacterSelect::Ready_ClickedTexture()
{
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Click_Texture"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	switch (m_ePlayerType)
	{
	case BTN_ROGUE:
		m_iTextureIndex = 2;
		break;

	case BTN_DESTROYER:
		m_iTextureIndex = 0;
		break;

	case BTN_WITCH:
		m_iTextureIndex = 4;
		break;

	case BTN_ENGINEER:
		m_iTextureIndex = 1;
		break;

	case BTN_SWORDMAN:
		m_iTextureIndex = 3;
		break;
	}

	m_bActive = false;

	return S_OK;
}

HRESULT CUI_BtnCharacterSelect::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnCharacterSelect::Bind_ShaderResources()
{
	if (m_iTextureIndex < 0 || 4 < m_iTextureIndex)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CUI_BtnCharacterSelect* CUI_BtnCharacterSelect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_SELECTBTN_TYPE eBtnType, UI_SELECTBTN_CHARACTER ePlayerType)
{
	CUI_BtnCharacterSelect* pInstance = new CUI_BtnCharacterSelect(pDevice, pContext, strObjectTag, eBtnType, ePlayerType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BtnCharacterSelect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnCharacterSelect::Clone(void* pArg)
{
	CUI_BtnCharacterSelect* pInstance = new CUI_BtnCharacterSelect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BtnCharacterSelect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnCharacterSelect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}