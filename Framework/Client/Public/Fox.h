#pragma once

#include "Animals.h"

BEGIN(Client)

class CFox final : public CAnimals
{
private:
	explicit CFox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CFox(const CFox& rhs);
	virtual ~CFox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Ready_State();
	HRESULT Ready_Collider();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

private:
	class CUI_World_AnimalTag* m_pTag = { nullptr };
	class CUI_World_Interaction* m_pInteractionBtn = { nullptr };

public:
	static CFox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);

public:
	virtual void Free() override;
};

END

