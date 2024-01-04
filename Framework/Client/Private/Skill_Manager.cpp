#include "stdafx.h"
#include "GameInstance.h"
#include "Skill_Manager.h"
#include "Utils.h"
#include "Skill.h"
#include "Game_Manager.h"
#include "Character_Manager.h"
#include "Skill_Destroyer_LeafSlam.h"
#include "Skill_Destroyer_WheelWind.h"
#include "Skill_Destroyer_BrutalStrike.h"
#include "Skill_SwordMan_PerfectBlade.h"
#include "Skill_SwordMan_SipohoningLunge.h"
#include "Skill_SwordMan_SpinningAssault.h"
#include "Skill_Engineer_BurstCall.h"
#include "Skill_Engineer_ElementalBlast.h"
#include "Skill_Engineer_ExplosionShot.h"


IMPLEMENT_SINGLETON(CSkill_Manager)

CSkill_Manager::CSkill_Manager()
{

}

HRESULT CSkill_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);


	
	
	CCharacter* pSwordMan = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN);
	if (nullptr != pSwordMan)
	{
		m_Skills[CHARACTER_TYPE::SWORD_MAN].emplace(SKILL_TYPE::SWORDMAN_PERFECT_BLADE, CSkill_SwordMan_PerfectBlade::Create(m_pDevice, m_pContext, pSwordMan));
		m_Skills[CHARACTER_TYPE::SWORD_MAN].emplace(SKILL_TYPE::SWORDMAN_SIPOHONINGLUNGE, CSkill_SwordMan_SipohoningLunge::Create(m_pDevice, m_pContext, pSwordMan));
		m_Skills[CHARACTER_TYPE::SWORD_MAN].emplace(SKILL_TYPE::SWORDMAN_SPINNING_ASSAULT, CSkill_SwordMan_SpinningAssault::Create(m_pDevice, m_pContext, pSwordMan));
	}
	

	CCharacter* pEngineer = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER);
	if(nullptr != pEngineer)
	{
		m_Skills[CHARACTER_TYPE::ENGINEER].emplace(SKILL_TYPE::ENGINEER_BURSTCALL, CSkill_Engineer_BurstCall::Create(m_pDevice, m_pContext, pEngineer));
		m_Skills[CHARACTER_TYPE::ENGINEER].emplace(SKILL_TYPE::ENGINEER_ELEMENTAL_BLAST, CSkill_Engineer_ElementalBlast::Create(m_pDevice, m_pContext, pEngineer));
		m_Skills[CHARACTER_TYPE::ENGINEER].emplace(SKILL_TYPE::ENGINEER_EXPLOSIONSHOT, CSkill_Engineer_ExplosionShot::Create(m_pDevice, m_pContext, pEngineer));
	}
	

	CCharacter* pDestroyer = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::DESTROYER);
	if (nullptr != pDestroyer)
	{
		m_Skills[CHARACTER_TYPE::DESTROYER].emplace(SKILL_TYPE::DESTROYER_LEAFSLAM, CSkill_Destroyer_LeafSlam::Create(m_pDevice, m_pContext, pDestroyer));
		m_Skills[CHARACTER_TYPE::DESTROYER].emplace(SKILL_TYPE::DESTROYER_WHEELWIND, CSkill_Destroyer_WheelWind::Create(m_pDevice, m_pContext, pDestroyer));
		m_Skills[CHARACTER_TYPE::DESTROYER].emplace(SKILL_TYPE::DESTROYER_BRUTALSTRIKE, CSkill_Destroyer_BrutalStrike::Create(m_pDevice, m_pContext, pDestroyer));
	}
	


	m_bReserved = true;


	return S_OK;
}

void CSkill_Manager::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{
		for (auto iter : m_Skills[i])
		{
			iter.second->Tick(fTimeDelta);
		}
	}
}

void CSkill_Manager::LateTick(_float fTimeDelta)
{
	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{
		for (auto iter : m_Skills[i])
		{
			iter.second->LateTick(fTimeDelta);
		}
	}
}

_bool CSkill_Manager::Use_Skill(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType)
{
	if (CHARACTER_TYPE::CHARACTER_END >= eCharacterType || SKILL_TYPE::SKILL_END >= eSkillType)
		return false;

	return true;
}

_bool CSkill_Manager::Is_Useable(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType)
{
	if (CHARACTER_TYPE::CHARACTER_END >= eCharacterType || SKILL_TYPE::SKILL_END >= eSkillType)
		return false;

	auto iter = m_Skills[eCharacterType].find(eSkillType);
	if (iter == m_Skills[eCharacterType].end())
		return false;

	return iter->second->Is_Useable();
}

CSkill* CSkill_Manager::Get_Skill(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType)
{
	if (CHARACTER_TYPE::CHARACTER_END >= eCharacterType || SKILL_TYPE::SKILL_END >= eSkillType)
		return nullptr;

	auto iter = m_Skills[eCharacterType].find(eSkillType);
	if (iter == m_Skills[eCharacterType].end())
		return nullptr;

	
	return iter->second;
}


void CSkill_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{
		for (auto iter : m_Skills[i])
		{
			Safe_Release(iter.second);
		}
		m_Skills[i].clear();
	}
	
}



