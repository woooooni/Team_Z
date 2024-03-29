#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_WeaponSection_Slot final : public CUI
{
public:
	enum UI_WEAPONSLOT { WEAPONSLOT_FIRST, WEAPONSLOT_SECOND, WEAPONSLOT_THIRD, WEAPONSLOT_END };

protected:
	CUI_WeaponSection_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_WEAPONSLOT eSlotType);
	CUI_WeaponSection_Slot(const CUI_WeaponSection_Slot& rhs);
	virtual ~CUI_WeaponSection_Slot() = default;

public:
	virtual void Set_CharacterType(CHARACTER_TYPE eType) override;
	void Use_BurstSkill();
	void Hide_UI(_bool bHide);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;

private:
	UI_WEAPONSLOT m_eSlotType = { WEAPONSLOT_END };
	_bool m_bWear = { false }; // 무기를 착용한 상태인가
	_bool m_bClick = { false };

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 500.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);

	class CUI_WeaponSection_DefaultWeapon* m_pNoWeapon = { nullptr };
	class CUI_SkillSection_CoolTimeFrame* m_pFrame = { nullptr };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);
	void Movement_BasedOnHiding(_float fTimeDelta);

public:
	static CUI_WeaponSection_Slot* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_WEAPONSLOT eSlotType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
