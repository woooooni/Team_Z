#include "stdafx.h"
#include "UI_Dummy_Destroyer.h"
#include "GameInstance.h"
#include "HierarchyNode.h"

#include "Character_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

#include "UI_World_NameTag.h"

#include "State_Lobby_Destroyer_Neutral_Idle.h"
#include "State_Lobby_Destroyer_Sit.h"

CUI_Dummy_Destroyer::CUI_Dummy_Destroyer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext, L"UI_Lobby_Dummy_Destroyer", CHARACTER_TYPE::DESTROYER)
{
}

CUI_Dummy_Destroyer::CUI_Dummy_Destroyer(const CUI_Dummy_Destroyer& rhs)
	: CCharacter(rhs)
{

}

void CUI_Dummy_Destroyer::Set_ClickState(_bool bClicked)
{
	if (bClicked)
	{
		if (nullptr != m_pNameTag)
			m_pNameTag->Set_Pass(7);
	}
	else
	{
		if (nullptr != m_pNameTag)
			m_pNameTag->Set_Pass(1);
	}

	m_bClicked = bClicked;
}

HRESULT CUI_Dummy_Destroyer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Dummy_Destroyer::Initialize(void* pArg)
{
//	if (FAILED(__super::Initialize(pArg)))
//		return E_FAIL;

	if (nullptr != pArg)
		m_tStat = *((CHARACTER_STAT*)pArg);

	for (_uint i = 0; i < SOCKET_END; ++i)
		m_pTrails[i] = nullptr;

	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
		m_pCharacterPartModels[i] = nullptr;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_pRigidBodyCom->Set_Use_Gravity(false);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(3.5f, 1.23f, -11.2f, 1.f));
	m_pTransformCom->LookAt_ForLandObject(Vec4(1.f, 1.23f, -30.f, 1.f));

	CGameObject* pNameTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Lobby_NameTag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pNameTag)
		return E_FAIL;

	m_pNameTag = dynamic_cast<CUI_World_NameTag*>(pNameTag);
	m_pNameTag->Set_Owner(this);

	return S_OK;
}

void CUI_Dummy_Destroyer::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(3.5f, 1.23f, -11.2f, 1.f));

	m_pStateCom->Tick_State(fTimeDelta);

	if (nullptr != m_pNameTag)
		m_pNameTag->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CUI_Dummy_Destroyer::LateTick(_float fTimeDelta)
{
	//	m_pModelCom->LateTick(fTimeDelta);
	//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	if (nullptr != m_pNameTag)
		m_pNameTag->LateTick(fTimeDelta);

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Dummy_Destroyer::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };
	if (m_bClicked)
	{
		vRimColor = { 0.5f, 0.5f, 0.f, 1.f };
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		if (nullptr == m_pCharacterPartModels[i])
			continue;

		const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j)))
				return E_FAIL;
		}
	}

	return S_OK;
}


HRESULT CUI_Dummy_Destroyer::Ready_Components()
{
	// For Transform Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For Renderer Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For Shader Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel" ), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Destroyer_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For State Machine Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	// For RigidBody Component
	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
 		m_pCharacterPartModels[i] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::DESTROYER, PART_TYPE(i), 0);

	m_pModelCom->Set_Animation(0);

	return S_OK;
}

#pragma region Ready_States
HRESULT CUI_Dummy_Destroyer::Ready_States()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle01");
	strAnimationNames.push_back(L"SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle02");
	strAnimationNames.push_back(L"SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle03");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_IDLE, CState_Lobby_Destroyer_Neutral_Idle::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Destroyer_Merge.ao|Destroyer_ChairSitStart");
	strAnimationNames.push_back(L"SKM_Destroyer_Merge.ao|Destroyer_ChairSitIdle01");
	strAnimationNames.push_back(L"SKM_Destroyer_Merge.ao|Destroyer_ChairSitIdle02");
	strAnimationNames.push_back(L"SKM_Destroyer_Merge.ao|Destroyer_ChairSitLoop");
	strAnimationNames.push_back(L"SKM_Destroyer_Merge.ao|Destroyer_ClassAction01");
	m_pStateCom->Add_State(CCharacter::STATE::LOBBY_SIT, CState_Lobby_Destroyer_Sit::Create(m_pStateCom, strAnimationNames));

	m_pStateCom->Change_State(CCharacter::LOBBY_SIT);

	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CUI_Dummy_Destroyer::Ready_Colliders()
{
	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CUI_Dummy_Destroyer::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CUI_Dummy_Destroyer::Ready_Parts()
{
	return S_OK;
}
#pragma endregion

_float2 CUI_Dummy_Destroyer::Transpose_ProjectionPosition()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float4x4 matWorld = m_pTransformCom->Get_WorldFloat4x4();
	_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	_float4x4 matWindow;
	XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

	_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];

	vWindowPos.x /= vWindowPos.z;
	vWindowPos.y /= vWindowPos.z;
	_float fScreenX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
	_float fScreenY = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

	return _float2(fScreenX, fScreenY);
}

CUI_Dummy_Destroyer* CUI_Dummy_Destroyer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dummy_Destroyer* pInstance = new CUI_Dummy_Destroyer(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CUI_Dummy_Destroyer");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUI_Dummy_Destroyer::Clone(void* pArg)
{
	CUI_Dummy_Destroyer* pInstance = new CUI_Dummy_Destroyer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Dummy_Destroyer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dummy_Destroyer::Free()
{
	__super::Free();

	Safe_Release(m_pNameTag);
}

