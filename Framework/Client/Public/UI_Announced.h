#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Announced final : public CUI
{
public:
	enum UI_ANNNOUNCE_TYPE{ ANNOUNCE_CAMERA, ANNOUNCE_SKILL, ANNOUNCED_END };

protected:
	CUI_Announced(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_ANNNOUNCE_TYPE eType);
	CUI_Announced(const CUI_Announced& rhs);
	virtual ~CUI_Announced() = default;

public:
	void Set_Active(_bool bActive, _int iMagicNum = 0);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Tick_CameraWindow(_float fTimeDelta);
	void Tick_SkillWindow(_float fTimeDelta);
	void LateTick_CameraWindow(_float fTimeDelta);
	void LateTick_SkillWindow(_float fTimeDelta);

private:
	UI_ANNNOUNCE_TYPE m_eType = { ANNOUNCED_END };
	CTexture* m_pFXTextureCom = { nullptr };

	_int m_iTextureIndex = { 0 };
	_bool m_bArrived = { false };

	_float m_fTimeAcc = { 0.f };
	_float m_fSpeed = { 80.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vStartPosition = _float2(0.f, 0.f);

	_bool m_bAlpha = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Announced* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_ANNNOUNCE_TYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
