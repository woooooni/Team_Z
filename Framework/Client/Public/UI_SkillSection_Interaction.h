#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillSection_Interaction final : public CUI
{
public:
	enum UI_INTERACTION_TYPE { INTERACTION_KICK, INTERACTION_PICKUP, INTERACTION_PICKDOWN, INTERACTION_END };

protected:
	CUI_SkillSection_Interaction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SkillSection_Interaction(const CUI_SkillSection_Interaction& rhs);
	virtual ~CUI_SkillSection_Interaction() = default;

public:
	void Set_InteractionType(UI_INTERACTION_TYPE eType) { m_eType = eType; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_INTERACTION_TYPE m_eType = { INTERACTION_END };

	_bool m_bRotate = { false };
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_SkillSection_Interaction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
