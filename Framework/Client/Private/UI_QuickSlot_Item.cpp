#include "stdafx.h"
#include "UI_QuickSlot_Item.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Inventory_Manager.h"

CUI_QuickSlot_Item::CUI_QuickSlot_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType)
	: CUI(pDevice, pContext, L"UI_QuickSlot_Item")
	, m_eType(eType)
{
}

CUI_QuickSlot_Item::CUI_QuickSlot_Item(const CUI_QuickSlot_Item& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_QuickSlot_Item::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_QuickSlot_Item::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	switch (m_eType)
	{
	case QUICKITEM_FIRST:
		m_fHideSpeed = 650.f;
		break;

	case QUICKITEM_SECOND:
		m_fHideSpeed = 700.f;
		break;

	case QUICKITEM_THIRD:
		m_fHideSpeed = 750.f;
		break;
	}

	m_bHide = false;
	m_bHideFinish = false;
	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);
	m_vHidePosition.x = -1.f * m_tInfo.fCX * 0.5f;
	m_vHidePosition.y = m_tInfo.fY;

	m_bUseMouse = true;

	return S_OK;
}

void CUI_QuickSlot_Item::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUICKITEM_END == m_eType)
			return;

		// 첫번째 퀵슬롯
		if (QUICKITEM_FIRST == m_eType)
		{
			// GARA
			// 포션의 개수를 파악해서 갱신한다.
			// 인벤에서 아이템 개수를 받아와서 0개면 m_bUsable = false, 1개이상이면 true
			
			_uint iCount = CInventory_Manager::GetInstance()->Get_InvenCount(ITEM_CODE::CONSUMPSION_HP);

			if (0 < iCount)
			{
				m_bUsable = true;
				m_iTextureIndex = 1;

				m_eCode = ITEM_CODE::CONSUMPSION_HP;
			}
			else
			{
				m_bUsable = false;

				m_eCode = ITEM_CODE::CODE_END;
			}
		}

		Use_Item(); // Key Event
		Movement_BasedOnHiding(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_QuickSlot_Item::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUICKITEM_END == m_eType)
			return;

		if (true == m_bUsable)
		{
			if (true == CUI_Manager::GetInstance()->Is_FadeFinished())
			{
				_uint iCount = CInventory_Manager::GetInstance()->Get_InvenCount(ITEM_CODE::CONSUMPSION_HP);

				// 1보다 큰 경우에만 Text로 개수를 보여준다.
				if (1 < iCount)
				{
					_float2 vDefault = _float2(m_tInfo.fX - 5.5f, m_tInfo.fY + 2.f);
					_float fOffset = (to_wstring(iCount).length() - 1) * 4.f;

					CRenderer::TEXT_DESC  CountDesc;
					CountDesc.strText = to_wstring(iCount);
					CountDesc.strFontTag = L"Default_Bold";
					CountDesc.vScale = { 0.3f, 0.3f };
					// Outline
					CountDesc.vColor = { 0.3f, 0.3f, 0.3f, 1.f };
					CountDesc.vPosition = _float2(vDefault.x - fOffset + 1.f, vDefault.y);
					m_pRendererCom->Add_Text(CountDesc);
					CountDesc.vPosition = _float2(vDefault.x - fOffset - 1.f, vDefault.y);
					m_pRendererCom->Add_Text(CountDesc);
					CountDesc.vPosition = _float2(vDefault.x - fOffset, vDefault.y + 1.f);
					m_pRendererCom->Add_Text(CountDesc);
					CountDesc.vPosition = _float2(vDefault.x - fOffset, vDefault.y - 1.f);
					m_pRendererCom->Add_Text(CountDesc);

					// Text
					CountDesc.vPosition = _float2(vDefault.x - fOffset, vDefault.y);
					CountDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
					m_pRendererCom->Add_Text(CountDesc);
				}
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_QuickSlot_Item::Render()
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

void CUI_QuickSlot_Item::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_QuickSlot_Item::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_QuickSlot_Item::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_QuickSlot_Item::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuickSlot_Item_Bg"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuickSlot_Item"),
		TEXT("Com_Texture_01"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_QuickSlot_Item::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_QuickSlot_Item::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (true == m_bUsable)
	{
		if (FAILED(m_pItemTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_QuickSlot_Item::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
	}
}

void CUI_QuickSlot_Item::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (m_tInfo.fX <= m_vHidePosition.x)
			{
				m_bHideFinish = true;
				m_tInfo.fX = m_vHidePosition.x;
			}
			else
			{
				m_tInfo.fX -= fTimeDelta * m_fHideSpeed;

				if (m_fAlpha <= 0.f)
					m_fAlpha = 0.f;
				else
					m_fAlpha -= fTimeDelta;
			}
		}
		else // 드러낸다
		{
			if (m_tInfo.fX >= m_vOriginPosition.x)
			{
				m_bHideFinish = true;
				m_tInfo.fX = m_vOriginPosition.x;
			}
			else
			{
				m_tInfo.fX += fTimeDelta * m_fHideSpeed;

				if (m_fAlpha >= 1.f)
					m_fAlpha = 1.f;
				else
					m_fAlpha += fTimeDelta;
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
}

void CUI_QuickSlot_Item::Use_Item()
{
	if (false == m_bUsable)
		return;

	// 첫번째 퀵슬롯만 사용한다는 전제하에
	if (KEY_TAP(KEY::P))
	{
		if (QUICKITEM_FIRST == m_eType)
		{
			CInventory_Manager::GetInstance()->Use_Item(m_eCode);

			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Potion_HP_1_St.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
		}
	}
}

CUI_QuickSlot_Item* CUI_QuickSlot_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType)
{
	CUI_QuickSlot_Item* pInstance = new CUI_QuickSlot_Item(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_QuickSlot_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_QuickSlot_Item::Clone(void* pArg)
{
	CUI_QuickSlot_Item* pInstance = new CUI_QuickSlot_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_QuickSlot_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_QuickSlot_Item::Free()
{
	__super::Free();

	Safe_Release(m_pItemTextureCom);
	Safe_Release(m_pTextureCom);
}
