#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillSection_SpecialSkill final : public CUI
{
public:
	enum UI_SPECIALSKILL { SKILL_FIRST, SKILL_SECOND, SKILL_THIRD, UICLASSICSKILL_END };

protected:
	CUI_SkillSection_SpecialSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SPECIALSKILL eType);
	CUI_SkillSection_SpecialSkill(const CUI_SkillSection_SpecialSkill& rhs);
	virtual ~CUI_SkillSection_SpecialSkill() = default;

public:
	virtual void Set_CharacterType(CHARACTER_TYPE eType) override;
	void Hide_UI(_bool bHide) { m_bHide = bHide; m_bHideFinish = false; }
	void Set_Clicked(_bool bClick);
	_bool Is_Clicked() { return m_bClicked; }

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
	UI_SPECIALSKILL m_eType = { UICLASSICSKILL_END };
	class CSkill* m_pSkill = { nullptr };

	CTexture* m_pMaskTextureCom = { nullptr };
	CTexture* m_pFXTextureCom = { nullptr };

	_int m_iTextureIndex = { -1 };
	_uint m_iPass = { 5 };

	_bool m_bClicked = { false }; // 클릭되었다 -> 사용되었다.
	_float m_fOriginCoolTime = { 0.f }; // 스킬마다 가지고있는 고유의 쿨타임
	_float m_fCoolTime = { 0.f }; // 셰이더에 던져질, 폰트로 띄워질 잔여 쿨타임
	_float2 m_vTextPosition = _float2(0.f, 0.f);

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 500.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);
	void Set_SkillType();
	void Movement_BasedOnHiding(_float fTimeDelta);

public:
	static CUI_SkillSection_SpecialSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SPECIALSKILL eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
