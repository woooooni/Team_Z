#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_ExplosionShot.h"

CSkill_Engineer_ExplosionShot::CSkill_Engineer_ExplosionShot()
{
	
}



HRESULT CSkill_Engineer_ExplosionShot::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 15.f;
	return S_OK;
}

void CSkill_Engineer_ExplosionShot::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_ExplosionShot::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Engineer_ExplosionShot::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::CLASS_SKILL_2);
		return true;
	}
	else
	{
		return false;
	}
		
}


CSkill_Engineer_ExplosionShot* CSkill_Engineer_ExplosionShot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Engineer_ExplosionShot* pInstance = new CSkill_Engineer_ExplosionShot;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_ExplosionShot");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_ExplosionShot::Free()
{
	__super::Free();
}
