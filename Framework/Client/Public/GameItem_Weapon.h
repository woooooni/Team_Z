#pragma once

#include "GameItem.h"

BEGIN(Client)

class CGameItem_Weapon final : public CGameItem
{
public:
	enum ITEMCATEGORY_WEAPON { WEAPON_SWORD = 0, WEAPON_GAUNTLET, WEAPON_HAMMER, WEAPON_RIFLE, WEAPON_END };

	typedef struct tagWeaponDesc : public CGameItem::ITEM_DESC
	{
		ITEMCATEGORY_WEAPON eWeaponCategory = WEAPON_END;
		_float	fAtk = 0.f;
	}ITEMDESC_WEAPON;

public:
	CGameItem_Weapon();
	CGameItem_Weapon(const CGameItem_Weapon& rhs);
	virtual ~CGameItem_Weapon() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);

public:
	ITEMDESC_WEAPON Get_ItemDesc() { return m_tWeaponDesc; }

protected:
	ITEMDESC_WEAPON m_tWeaponDesc = {};

public:
	static CGameItem_Weapon* Create(void* pArg);
	virtual void Free() override;

};

END

