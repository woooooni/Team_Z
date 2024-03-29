#include "stdafx.h"
#include "Grandprix_Engineer.h"
#include "GameInstance.h"

#include "Character_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

#include "State_Enemy_Flying_RunStart.h"
#include "State_Enemy_Flying_Stand.h"
#include "State_Enemy_Flying_Run.h"


CGrandprix_Engineer::CGrandprix_Engineer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGrandprix_Enemy(pDevice, pContext, L"Minigame_Grandprix_Engineer")
{
}

CGrandprix_Engineer::CGrandprix_Engineer(const CGrandprix_Engineer& rhs)
	: CGrandprix_Enemy(rhs)
{

}

HRESULT CGrandprix_Engineer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrandprix_Engineer::Initialize(void* pArg)
{
	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
		m_pCharacterPartModels[i] = nullptr;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_pRigidBodyCom->Set_Use_Gravity(false);
	m_pRigidBodyCom->Set_Ground(true);

	m_bActive = false;

	return S_OK;
}

void CGrandprix_Engineer::Tick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	__super::Tick(fTimeDelta);

	if (true == m_bInfinite)
	{
		m_fAccInfinite += fTimeDelta;
		if (m_fAccInfinite >= m_fInfiniteTime)
		{
			m_bInfinite = false;
			m_fAccInfinite = 0.f;
		}
	}

	if (true == m_bReserveDead)
	{
		m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
		if (m_fDissolveWeight >= m_fDissolveTotal)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
			Set_Dead(true);
		}
	}

}

void CGrandprix_Engineer::LateTick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (nullptr == m_pRendererCom)
		return;

	m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
}

HRESULT CGrandprix_Engineer::Render()
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

	_float4 vRimColor = true == m_bInfinite ? Vec4(0.f, 0.f, 1.f, 1.f) : Vec4(0.f, 0.f, 0.f, 0.f);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;

	// Dissolve -----------------
	if (FAILED(m_pDissolveTexture->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 51)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveDuration", &m_fDissolveDuration, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4))))
		return E_FAIL;
	// ----------------- Dissolve

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


HRESULT CGrandprix_Engineer::Ready_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Engineer_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For State Machine Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	// For RigidBody Component
	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	m_pCharacterPartModels[PART_TYPE::HEAD] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::ENGINEER, PART_TYPE::HEAD, L"Adventure");
	m_pCharacterPartModels[PART_TYPE::HAIR] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::ENGINEER, PART_TYPE::HAIR, 0);
	m_pCharacterPartModels[PART_TYPE::FACE] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::ENGINEER, PART_TYPE::FACE, 0);
	m_pCharacterPartModels[PART_TYPE::BODY] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::ENGINEER, PART_TYPE::BODY, L"Adventure");

	//m_pModelCom->Set_Animation(0);

	m_pDissolveTexture = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise")));
	if (m_pDissolveTexture == nullptr)
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_States
HRESULT CGrandprix_Engineer::Ready_States()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Engineer_SoulDiver.ao|Engineer_SitRunStart_Biplane");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_RUNSTART, CState_Enemy_Flying_RunStart::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Engineer_SoulDiver.ao|Engineer_SitStand_Biplane");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND, CState_Enemy_Flying_Stand::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Engineer_SoulDiver.ao|Engineer_SitRun_Biplane");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_RUN, CState_Enemy_Flying_Run::Create(m_pStateCom, strAnimationNames));

	m_pStateCom->Change_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND);

	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CGrandprix_Engineer::Ready_Colliders()
{
	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CGrandprix_Engineer::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CGrandprix_Engineer::Ready_Parts()
{
	return S_OK;
}
#pragma endregion

_float2 CGrandprix_Engineer::Transpose_ProjectionPosition()
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

CGrandprix_Engineer* CGrandprix_Engineer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGrandprix_Engineer* pInstance = new CGrandprix_Engineer(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CGrandprix_Engineer");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGrandprix_Engineer::Clone(void* pArg)
{
	CGrandprix_Engineer* pInstance = new CGrandprix_Engineer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGrandprix_Engineer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrandprix_Engineer::Free()
{
	__super::Free();
	Safe_Release(m_pDissolveTexture);
}

