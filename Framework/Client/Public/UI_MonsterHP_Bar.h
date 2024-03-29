#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CUI_MonsterHP_Bar final : public CUI
{
protected:
	CUI_MonsterHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MonsterHP_Bar(const CUI_MonsterHP_Bar& rhs);
	virtual ~CUI_MonsterHP_Bar() = default;

public:
	void Set_MonsterInfo(class CMonster* pOwner);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	CTexture* m_pFXTextureCom = { nullptr };

	wstring m_strName = L"";
	wstring m_strMaxHP = L"";
	wstring m_strCurHP = L"";

	_float m_fMaxHP = { 0.f };
	_bool m_bLerp = { false };
	_float m_fPreHP = { 0.f };
	_float m_fCurHP = { 0.f };

	// For Text
	_float2 m_vDefaultPosition = _float2(g_iWinSizeX * 0.5f + 5.f, 32.f); // "/"
	_float2 m_vCurHPPosition = _float2(g_iWinSizeX * 0.5f - 5.f, 33.f); // 최대 체력
	_float2 m_vMaxHPPosition = _float2(g_iWinSizeX * 0.5f + 18.f, 33.f); // 현재 체력

	_float4 m_vFontColor = _float4(0.965f, 1.f, 0.878f, 1.f);
	_float4 m_vOutlineColor = _float4(0.525f, 0.557f, 0.561f, 1.f);

	ELEMENTAL_TYPE m_eElementalType = { ELEMENTAL_TYPE::FIRE };
	class CMonster* m_pTarget = { nullptr };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_MonsterHP_Bar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
