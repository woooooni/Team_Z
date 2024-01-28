#include "stdafx.h"
#include "UI_WeaponSection_Recommend.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Weapon_Manager.h"
#include "Player.h"
#include "Weapon.h"
#include "Monster.h"

CUI_WeaponSection_Recommend::CUI_WeaponSection_Recommend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_WeaponSection_Recommend")
{
}

CUI_WeaponSection_Recommend::CUI_WeaponSection_Recommend(const CUI_WeaponSection_Recommend& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WeaponSection_Recommend::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WeaponSection_Recommend::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 0.7f;

	return S_OK;
}

void CUI_WeaponSection_Recommend::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (false == m_bUp)
		{
			// 아래로 움직인다
			m_tInfo.fY += fTimeDelta * m_fSpeed;

			if (m_tInfo.fY >= m_fGoalY)
			{
				m_bUp = true;
				m_tInfo.fY = m_fGoalY;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
		else
		{
			// 위로 움직인다
			m_tInfo.fY -= fTimeDelta * m_fSpeed;

			if (m_tInfo.fY <= m_fOriginY)
			{
				m_bUp = false;
				m_tInfo.fY = m_fOriginY;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}

		__super::Tick(fTimeDelta);
	}

	CCharacter* pCharacter = CUI_Manager::GetInstance()->Get_Character();
	CGameObject* pTarget = nullptr;
	pTarget = pCharacter->Get_Target();

	if (nullptr == pTarget)
	{
		if (true == Get_Active())
			Set_Active(false);

		return;
	}

	if (true == pTarget->Is_ReserveDead() || pTarget->Is_Dead())
		return;

	if (OBJ_TYPE::OBJ_MONSTER == pTarget->Get_ObjectType())
	{
		ELEMENTAL_TYPE eMonsterElemental = dynamic_cast<CMonster*>(pTarget)->Get_Stat().eElementType;
		ELEMENTAL_TYPE ePlayerElemental = pCharacter->Get_ElementalType();
		
		switch (eMonsterElemental)
		{
		case ELEMENTAL_TYPE::FIRE:
			if (ELEMENTAL_TYPE::FIRE == ePlayerElemental || ELEMENTAL_TYPE::WOOD == ePlayerElemental)
			{
				if (m_iCurIndex != 1)
				{
					// 2번째로 옮긴다. WATER가 이기는 속성
					Update_Position(1);
				}

				if (false == Get_Active())
					Set_Active(true);
			}
			else
			{
				if (ELEMENTAL_TYPE::WATER == ePlayerElemental)
				{
					if (true == Get_Active())
						Set_Active(false);
				}
			}
			break;

		case ELEMENTAL_TYPE::WATER:
			if (ELEMENTAL_TYPE::FIRE == ePlayerElemental || ELEMENTAL_TYPE::WATER == ePlayerElemental)
			{
				if (m_iCurIndex != 2)
				{
					// 3번째로 옮긴다. WOOD가 이기는 속성
					Update_Position(2);
				}

				if (false == Get_Active())
					Set_Active(true);
			}
			else
			{
				if (ELEMENTAL_TYPE::WOOD == ePlayerElemental)
				{
					if (true == Get_Active())
						Set_Active(false);
				}
			}
			break;

		case ELEMENTAL_TYPE::WOOD:
			if (ELEMENTAL_TYPE::WATER == ePlayerElemental || ELEMENTAL_TYPE::WOOD == ePlayerElemental)
			{
				if (m_iCurIndex != 0)
				{
					// 0번째로 옮긴다. FIRE가 이기는 속성
					Update_Position(0);
				}

				if (false == Get_Active())
					Set_Active(true);
			}
			else
			{
				if (ELEMENTAL_TYPE::FIRE == ePlayerElemental)
				{
					if (true == Get_Active())
						Set_Active(false);
				}
			}
			break;
		}

//		switch (ePlayerElemental)
//		{
//		case ELEMENTAL_TYPE::FIRE:
//			if (ELEMENTAL_TYPE::FIRE == eMonsterElemental || ELEMENTAL_TYPE::WATER == eMonsterElemental)
//			{
//				if (m_iCurIndex != 2)
//				{
//					// 3번째로 옮긴다 (WOOD가 이기는 무기)
//					Update_Position(2);
//					Set_Active(true);
//				}
//			}
//			else
//			{
//				if (ELEMENTAL_TYPE::WOOD == eMonsterElemental)
//				{
//					if (true == Get_Active())
//						Set_Active(false);
//				}
//			}
//			break;
//
//		case ELEMENTAL_TYPE::WATER:
//			if (ELEMENTAL_TYPE::WATER == eMonsterElemental || ELEMENTAL_TYPE::WOOD == eMonsterElemental)
//			{
//				if (m_iCurIndex != 0)
//				{
//					// 1번째로 옮긴다 (FIRE가 이기는 무기)
//					Update_Position(0);
//					Set_Active(true);
//				}
//			}
//			else
//			{
//				if (ELEMENTAL_TYPE::FIRE == eMonsterElemental)
//				{
//					if (true == Get_Active())
//						Set_Active(false);
//				}
//			}
//			break;
//
//		case ELEMENTAL_TYPE::WOOD:
//			if (ELEMENTAL_TYPE::WOOD == eMonsterElemental || ELEMENTAL_TYPE::FIRE == eMonsterElemental)
//			{
//				if (m_iCurIndex != 1)
//				{
//					// 2번째로 옮긴다 (Water가 이기는 무기)
//					Update_Position(1);
//					Set_Active(true);
//				}
//			}
//			else
//			{
//				if (ELEMENTAL_TYPE::WATER == eMonsterElemental)
//				{
//					if (true == Get_Active())
//						Set_Active(false);
//				}
//			}
//			break;
//		}
	}
}

void CUI_WeaponSection_Recommend::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_WeaponSection_Recommend::Render()
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

void CUI_WeaponSection_Recommend::Update_Position(_uint iSlotNum)
{
	if (2 < iSlotNum)
		return;

	switch (iSlotNum)
	{
	case 0:
		m_tInfo.fX = m_vPosition[0].x;
		m_tInfo.fY = m_vPosition[0].y;
		m_iCurIndex = 0;
		break;

	case 1:
		m_tInfo.fX = m_vPosition[1].x;
		m_tInfo.fY = m_vPosition[1].y;
		m_iCurIndex = 1;
		break;

	case 2:
		m_tInfo.fX = m_vPosition[2].x;
		m_tInfo.fY = m_vPosition[2].y;
		m_iCurIndex = 2;
		break;
	}

	m_bUp = false;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
}

HRESULT CUI_WeaponSection_Recommend::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Recommend"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WeaponSection_Recommend::Ready_State()
{
	_float fSize = 64.f * 0.3f;

	//_float2 vDefaultOffset = _float2(17.f, -170.f);
	_float2 vDefaultOffset = _float2(17.f, -240.f);
	_float fOffset = 85.f;

	// WeaponSection의 정보를 그대로 가져다가 사용함.
	m_tInfo.fCX = fSize;
	m_tInfo.fCY = fSize;

	_float fParentfCX = 400.f * 0.75f;
	_float fParentfCY = 400.f * 0.75f;
	_float fParentfX = g_iWinSizeX - fParentfCX * 0.5f;
	_float fParentfY = g_iWinSizeY - fParentfCY * 0.5f - 5.f;

	m_tInfo.fX = fParentfX + (vDefaultOffset.x - fOffset);
	m_tInfo.fY = fParentfY + vDefaultOffset.y;

	m_vPosition[0] = _float2(m_tInfo.fX, m_tInfo.fY);
	m_vPosition[1] = _float2(m_tInfo.fX + fOffset, m_tInfo.fY);
	m_vPosition[2] = _float2(m_tInfo.fX + fOffset * 2.f, m_tInfo.fY);

	_float fGoalY = 30.f;
	
	m_fOriginY = m_tInfo.fY;
	m_fGoalY = m_tInfo.fY + fGoalY;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_WeaponSection_Recommend::Bind_ShaderResources()
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

CUI_WeaponSection_Recommend* CUI_WeaponSection_Recommend::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WeaponSection_Recommend* pInstance = new CUI_WeaponSection_Recommend(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_WeaponSection_Recommend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WeaponSection_Recommend::Clone(void* pArg)
{
	CUI_WeaponSection_Recommend* pInstance = new CUI_WeaponSection_Recommend(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_WeaponSection_Recommend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WeaponSection_Recommend::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
