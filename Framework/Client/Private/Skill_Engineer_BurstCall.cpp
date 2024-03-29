#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_BurstCall.h"

CSkill_Engineer_BurstCall::CSkill_Engineer_BurstCall()
{
	
}



HRESULT CSkill_Engineer_BurstCall::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 10.f;
	return S_OK;
}

void CSkill_Engineer_BurstCall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_BurstCall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Engineer_BurstCall::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::CLASS_SKILL_0);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Engineer_BurstCall* CSkill_Engineer_BurstCall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Engineer_BurstCall* pInstance = new CSkill_Engineer_BurstCall;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_BurstCall");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_BurstCall::Free()
{
	__super::Free();
}
