#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_World_NPCSpeechBalloon final : public CUI
{
protected:
	CUI_World_NPCSpeechBalloon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_World_NPCSpeechBalloon(const CUI_World_NPCSpeechBalloon& rhs);
	virtual ~CUI_World_NPCSpeechBalloon() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Set_Owner(class CGameObject* pOwner, _float fOffsetY);
	void Set_Balloon(const wstring& pText);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CGameNpc* m_pOwner = { nullptr };
	wstring m_strContents;

	_float2 m_vOwnerPos = _float2(0.f, 0.f);
	_float2 m_vTextPos = _float2(0.f, 0.f);

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMinSize = _float2(0.f, 0.f);
	_float m_fOffsetY = { 0.f };
	_bool m_bResizeDone = { false };

	_float2 m_fSpeed = _float2(0.f, 0.f);
	_float m_fActiveTimeAcc = { 0.f };

	_bool m_bHide = { true }; // 숨겨야할때 -> 시간 누적을 위함
	_float m_fHideTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_World_NPCSpeechBalloon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
