#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SetNickname_Window final : public CUI
{
protected:
	CUI_SetNickname_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SetNickname_Window(const CUI_SetNickname_Window& rhs);
	virtual ~CUI_SetNickname_Window() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_SetNickname_Window* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
