#pragma once

#include "UI.h"

// PlayerStatus UI

BEGIN(Client)
class CUI_MonsterHP_Background final : public CUI
{
protected:
	CUI_MonsterHP_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MonsterHP_Background(const CUI_MonsterHP_Background& rhs);
	virtual ~CUI_MonsterHP_Background() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	wstring m_strName = L"";

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_MonsterHP_Background* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END