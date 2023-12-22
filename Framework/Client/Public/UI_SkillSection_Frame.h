#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillSection_Frame final : public CUI
{
public:
	enum UI_SKILLFRAME_TYPE { FRAME_CLASSIC, FRAME_SPECIAL, SKILLFRAME_END };
	enum UI_SPECIALSKILL_TYPE { FRAME_FIRST, FRAME_SECOND, FRAME_THIRD, FRAME_END };

protected:
	CUI_SkillSection_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UI_SKILLFRAME_TYPE eType, UI_SPECIALSKILL_TYPE eNumType = UI_SPECIALSKILL_TYPE::FRAME_END);
	CUI_SkillSection_Frame(const CUI_SkillSection_Frame& rhs);
	virtual ~CUI_SkillSection_Frame() = default;

public:
	void Set_ClassicFrameColor(ELEMENTAL_TYPE eType);
	void Set_SpecialFrameColor(CHARACTER_TYPE eType);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_SKILLFRAME_TYPE m_eFrameType = { SKILLFRAME_END };
	UI_SPECIALSKILL_TYPE m_eNumType = { FRAME_END };
	_float4 m_vColor = { 1.f, 1.f, 1.f, 1.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_SkillSection_Frame* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		UI_SKILLFRAME_TYPE eType, UI_SPECIALSKILL_TYPE eNumType = UI_SPECIALSKILL_TYPE::FRAME_END);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END