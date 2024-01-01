#include "stdafx.h"
#include "Character_Destroyer.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"
#include "Character_Manager.h"
#include "Weapon_Manager.h"
#include "Weapon.h"
#include "Sword.h"

#include "State_SwordMan_Neutral_Idle.h"
#include "State_SwordMan_Neutral_Jump.h"
#include "State_SwordMan_Neutral_Walk.h"
#include "State_SwordMan_Neutral_Run.h"
#include "State_SwordMan_Neutral_Kick.h"

#include "State_SwordMan_Neutral_Crouch_Idle.h"
#include "State_SwordMan_Neutral_Crouch_Move.h"


#include "State_SwordMan_Neutral_Pick_Small_Idle.h"
#include "State_SwordMan_Neutral_Pick_Small_Walk.h"
#include "State_SwordMan_Neutral_Pick_Small_Run.h"
#include "State_SwordMan_Neutral_Pick_Small_Throw.h"
#include "State_SwordMan_Neutral_Pick_Small_Finish.h"

#include "State_SwordMan_Neutral_Pick_Large_Idle.h"
#include "State_SwordMan_Neutral_Pick_Large_Walk.h"
#include "State_SwordMan_Neutral_Pick_Large_Run.h"
#include "State_SwordMan_Neutral_Pick_Large_Throw.h"
#include "State_SwordMan_Neutral_Pick_Large_Finish.h"


#include "State_SwordMan_AbNormality_Stun.h"

#include "State_SwordMan_Battle_Guard.h"
#include "State_SwordMan_Battle_Idle.h"
#include "State_SwordMan_Battle_Jump.h"
#include "State_SwordMan_Battle_Walk.h"
#include "State_SwordMan_Battle_Run.h"
#include "State_SwordMan_Battle_Dash.h"

#include "State_SwordMan_Battle_Attack_0.h"
#include "State_SwordMan_Battle_Attack_1.h"
#include "State_SwordMan_Battle_Attack_2.h"
#include "State_SwordMan_Battle_Attack_3.h"

#include "State_SwordMan_BurstSkill_MegaSlash.h"

#include "State_SwordMan_Skill_PerfectBlade.h"
#include "State_SwordMan_Skill_SipohoningLunge.h"
#include "State_SwordMan_Skill_SpinningAssault.h"

#include "State_SwordMan_SpecialSkill_AcaneBarrier.h"
#include "State_SwordMan_SpecialSkill_FrozenStorm.h"
#include "State_SwordMan_SpecialSkill_SwordTempest.h"

#include "State_SwordMan_Damaged_Impact.h"
#include "State_SwordMan_Damaged_KnockDown.h"
#include "State_SwordMan_Damaged_Strong.h"
#include "State_SwordMan_Damaged_Weak.h"


#include "State_SwordMan_Dead.h"
#include "State_SwordMan_Revive.h"



CCharacter_Destroyer::CCharacter_Destroyer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CCharacter(pDevice, pContext, strObjectTag, CHARACTER_TYPE::SWORD_MAN)
{
}

CCharacter_Destroyer::CCharacter_Destroyer(const CCharacter_Destroyer& rhs)
	: CCharacter(rhs)
{

}

HRESULT CCharacter_Destroyer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCharacter_Destroyer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//if (FAILED(Ready_Sockets()))
	//	return E_FAIL;

	if (FAILED(Ready_Weapon()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

 	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CCharacter_Destroyer::Tick(_float fTimeDelta)
{
	m_pStateCom->Tick_State(fTimeDelta);

	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	m_pControllerCom->Tick_Controller(fTimeDelta);

	if (m_pWeapon != nullptr)
	{
		Matrix matSocketLocal = m_pModelCom->Get_SocketLocalMatrix(0);
		Matrix matSocketWorld = matSocketLocal * m_pTransformCom->Get_WorldMatrix();

		m_pWeapon->Set_SocketWorld(matSocketWorld);
	}
	__super::Tick(fTimeDelta);
}

void CCharacter_Destroyer::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG

	
}

HRESULT CCharacter_Destroyer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

void CCharacter_Destroyer::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CCharacter_Destroyer::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CCharacter_Destroyer::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CCharacter_Destroyer::On_Damaged(const COLLISION_INFO& tInfo)
{
	__super::On_Damaged(tInfo);
}


HRESULT CCharacter_Destroyer::Ready_Components()
{

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel" ), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordMan_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;



	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;


	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
		m_pCharacterPartModels[i] = CCharacter_Manager::GetInstance()->Get_PartModel(m_eCharacterType, PART_TYPE(i), 0);

	
	m_pControllerCom->Set_Active(false);
	m_pModelCom->Set_Animation(0);
	//m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Vec4(-10.0f, 50.0f, 0.0f, 1.0f));
	return S_OK;
}

#pragma region Ready_States
HRESULT CCharacter_Destroyer::Ready_States()
{
	list<wstring> strAnimationNames;


	// Neutral
	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralStand");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralIdle01");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralIdle02");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_IDLE, CState_SwordMan_Neutral_Idle::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralWalk");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_WALK, CState_SwordMan_Neutral_Walk::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralRun");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_RUN, CState_SwordMan_Neutral_Run::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_JumpUpLoop");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_JUMP, CState_SwordMan_Neutral_Jump::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CrouchStand");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CrouchIdle01");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_CROUCH_IDLE, CState_SwordMan_Neutral_Crouch_Idle::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CrouchWalk");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_CROUCH_MOVE, CState_SwordMan_Neutral_Crouch_Move::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Kick");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_KICK, CState_SwordMan_Neutral_Kick::Create(m_pStateCom, strAnimationNames));



	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickStandS");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_SMALL_IDLE, CState_SwordMan_Neutral_Pick_Small_Idle::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickWalkS");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_SMALL_WALK, CState_SwordMan_Neutral_Pick_Small_Walk::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickRunS");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_SMALL_RUN, CState_SwordMan_Neutral_Pick_Small_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickThrowS");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_SMALL_THROW, CState_SwordMan_Neutral_Pick_Small_Throw::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickFinishS");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_SMALL_FINISH, CState_SwordMan_Neutral_Pick_Small_Finish::Create(m_pStateCom, strAnimationNames));
	

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickStandL");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_LARGE_IDLE, CState_SwordMan_Neutral_Pick_Large_Idle::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickWalkL");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_LARGE_WALK, CState_SwordMan_Neutral_Pick_Large_Walk::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickRunL");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_LARGE_RUN, CState_SwordMan_Neutral_Pick_Large_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickThrowL");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_LARGE_THROW, CState_SwordMan_Neutral_Pick_Large_Throw::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_PickFinishL");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_PICK_LARGE_FINISH, CState_SwordMan_Neutral_Pick_Large_Finish::Create(m_pStateCom, strAnimationNames));
	
	
	// AbNormality
	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_StunLoop1");
	m_pStateCom->Add_State(CCharacter::STATE::ABNORMALITY_STUN, CState_SwordMan_AbNormality_Stun::Create(m_pStateCom, strAnimationNames));




	// Battle
	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CSBattleStand");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_FinishCombat");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_IDLE, CState_SwordMan_Battle_Idle::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_BattleRun");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_WALK, CState_SwordMan_Battle_Walk::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Dash");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_RUN, CState_SwordMan_Battle_Run::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();	
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_JumpUpLoop");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_JUMP, CState_SwordMan_Battle_Jump::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Attack01_1");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_ATTACK_0, CState_SwordMan_Battle_Attack_0::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Attack01_2");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_ATTACK_1, CState_SwordMan_Battle_Attack_1::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Attack01_3");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_ATTACK_2, CState_SwordMan_Battle_Attack_2::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Attack01_4");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_ATTACK_3, CState_SwordMan_Battle_Attack_3::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CSGuardLoop");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_GUARD, CState_SwordMan_Battle_Guard::Create(m_pStateCom, strAnimationNames));
	

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_SkillDash");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_DASH, CState_SwordMan_Battle_Dash::Create(m_pStateCom, strAnimationNames));
	
	


	// Skill
	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_SkillMegaSlash");
	m_pStateCom->Add_State(CCharacter::STATE::SKILL_BURST, CState_SwordMan_BurstSkill_MegaSlash::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_SkillPerfectBlades");
	m_pStateCom->Add_State(CCharacter::STATE::CLASS_SKILL_0, CState_SwordMan_Skill_PerfectBlade::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_SkillSiphoningLunge");
	m_pStateCom->Add_State(CCharacter::STATE::CLASS_SKILL_1, CState_SwordMan_Skill_SipohoningLunge::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_SkillSpinningAssault");
	m_pStateCom->Add_State(CCharacter::STATE::CLASS_SKILL_2, CState_SwordMan_Skill_SpinningAssault::Create(m_pStateCom, strAnimationNames));
	
	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_SkillArcaneBarrier_re");
	m_pStateCom->Add_State(CCharacter::STATE::SKILL_SPECIAL_0, CState_SwordMan_SpecialSkill_AcaneBarrier::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_SkillFrozenStorm");
	m_pStateCom->Add_State(CCharacter::STATE::SKILL_SPECIAL_1, CState_SwordMan_SpecialSkill_FrozenStorm::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_SkillSwordTempest");
	m_pStateCom->Add_State(CCharacter::STATE::SKILL_SPECIAL_2, CState_SwordMan_SpecialSkill_SwordTempest::Create(m_pStateCom, strAnimationNames));

	
	
	// Damaged
	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Impact_Start");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Impact_Loop");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Impact_Finish");
	m_pStateCom->Add_State(CCharacter::STATE::DAMAGED_IMPACT, CState_SwordMan_Damaged_Impact::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_KnockUpStart1");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_KnockDownLoop");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_KnockDownFinish");

	m_pStateCom->Add_State(CCharacter::STATE::DAMAGED_KNOCKDOWN, CState_SwordMan_Damaged_KnockDown::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_DamageStrong");
	m_pStateCom->Add_State(CCharacter::STATE::DAMAGED_STRONG, CState_SwordMan_Damaged_Strong::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_DamageWeak");
	m_pStateCom->Add_State(CCharacter::STATE::DAMAGED_WEAK, CState_SwordMan_Damaged_Weak::Create(m_pStateCom, strAnimationNames));

	// Dead & Revive
	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Death");
	m_pStateCom->Add_State(CCharacter::STATE::DEAD, CState_SwordMan_Dead::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Revive");
	m_pStateCom->Add_State(CCharacter::STATE::REVIVE, CState_SwordMan_Revive::Create(m_pStateCom, strAnimationNames));


	m_pStateCom->Change_State(CCharacter::NEUTRAL_IDLE);
	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CCharacter_Destroyer::Ready_Colliders()
{


	//CCollider_Sphere::SPHERE_COLLIDER_DESC ColliderDesc;
	//ZeroMemory(&ColliderDesc, sizeof ColliderDesc);

	//BoundingSphere tSphere;
	//ZeroMemory(&tSphere, sizeof(BoundingSphere));
	//tSphere.Radius = 1.f;
	//ColliderDesc.tSphere = tSphere;

	//ColliderDesc.pOwner = this;
	//ColliderDesc.pNode = nullptr;
	//ColliderDesc.pOwnerTransform = m_pTransformCom;
	//ColliderDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	//ColliderDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);
	//ColliderDesc.bLockAngle_X = false;
	//ColliderDesc.bLockAngle_Y = false;
	//ColliderDesc.bLockAngle_Z = false;

	//ColliderDesc.fAngularDamping = 0.f;
	//ColliderDesc.fDensity = 1.f;

	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &ColliderDesc)))
	//	return E_FAIL;


	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 50.f, 70.f, 50.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 70.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;



	OBBBox.Extents = { 100.f, 100.f, 50.f };
	OBBDesc.vOffsetPosition = Vec3(0.f, 70.f, -100.f);
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;


	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CCharacter_Destroyer::Ready_Sockets()
{
	//if (nullptr == m_pModelCom)
	//	return E_FAIL;

	//m_Sockets.resize(SOCKET_END);

	//m_Sockets[SOCKET_SWORD] = m_pModelCom->Get_HierarchyNode(L"R_HandCommon_1_Lct");
	//m_Sockets[SOCKET_RIGHT_HAND] = m_pModelCom->Get_HierarchyNode(L"R_Hand_1");
	//m_Sockets[SOCKET_SWEATH] = m_pModelCom->Get_HierarchyNode(L"L_Weapon_1_Lct");

	//m_Sockets[SOCKET_LEFT_FOOT] = m_pModelCom->Get_HierarchyNode(L"L_Foot_End");
	//m_Sockets[SOCKET_RIGHT_FOOT] = m_pModelCom->Get_HierarchyNode(L"R_Foot_End");
	//

	//CTrail::TRAIL_DESC TrailDesc = {};
	//TrailDesc.bTrail = true;
	//TrailDesc.fAccGenTrail = 0.f;
	//TrailDesc.fGenTrailTime = 0.01f;
	//TrailDesc.vDiffuseColor = { 0.f, 0.5f, 1.f, 0.5f };


	//// LeftFoot
	//m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT] = CTrail::Create(m_pDevice, m_pContext, L"LeftFoot_Trail", TrailDesc);
	//if (m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT] == nullptr)
	//	return E_FAIL;

	//if (FAILED(m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT]->Initialize(nullptr)))
	//	return E_FAIL;

	//m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT]->SetUp_Position(XMVectorSet(0.f, 0.0f, -0.0125f, 1.f), XMVectorSet(0.f, 0.0f, 0.0125f, 1.f));
	//m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT]->Set_VtxCount(66);



	//// RightFoot
	//m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT] = CTrail::Create(m_pDevice, m_pContext, L"RightFoot_Trail", TrailDesc);
	//if (m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT] == nullptr)
	//	return E_FAIL;

	//if (FAILED(m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT]->Initialize(nullptr)))
	//	return E_FAIL;

	//m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT]->SetUp_Position(XMVectorSet(0.f, 0.0f, -0.0125f, 1.f), XMVectorSet(0.f, 0.0f, 0.0125f, 1.f));
	//m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT]->Set_VtxCount(66);
	//



	return S_OK;
}
#pragma endregion

#pragma region Ready_Weapon
HRESULT CCharacter_Destroyer::Ready_Weapon()
{
	m_pWeapon = CSword::Create(m_pDevice, m_pContext, L"SwordMane_Sword");
	if (nullptr == m_pWeapon)
		return S_OK;
	

	m_pWeapon->Set_ModelCom(CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCharacterType, L"Flower01"));

	if (nullptr == m_pWeapon->Get_ModelCom())
	{
		Safe_Release(m_pWeapon);
		return S_OK;
	}
		



	return S_OK;
}
#pragma endregion

CCharacter_Destroyer* CCharacter_Destroyer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCharacter_Destroyer* pInstance = new CCharacter_Destroyer(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CCharacter_Destroyer");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CCharacter_Destroyer::Clone(void* pArg)
{
	CCharacter_Destroyer* pInstance = new CCharacter_Destroyer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCharacter_Destroyer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacter_Destroyer::Free()
{
	__super::Free();
}
