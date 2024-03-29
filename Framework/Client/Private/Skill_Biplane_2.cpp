#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Biplane_2.h"

CSkill_Biplane_2::CSkill_Biplane_2()
{
	
}



HRESULT CSkill_Biplane_2::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 3.f;

	return S_OK;
}

void CSkill_Biplane_2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSkill_Biplane_2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Biplane_2::Use_Skill()
{
	if (true == __super::Use_Skill())
		return true;
	else	
		return false;	
}


CSkill_Biplane_2* CSkill_Biplane_2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Biplane_2* pInstance = new CSkill_Biplane_2;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Biplane_2");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Biplane_2::Free()
{
	__super::Free();
}
