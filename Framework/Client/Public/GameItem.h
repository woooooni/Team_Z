#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CGameItem abstract : public CBase
{
public:
	enum ITEMCATEGORY_MAIN { CATEGORY_WEAPON = 0, CATEGORY_ARMOR, CATEGORY_ACC, CATEGORY_POTION, CATEGORY_ETC, CATEGORY_END };

	typedef struct tagItemDesc
	{
		ITEM_CODE eCode = CODE_END;
		wstring strName = TEXT("");
		wstring strContent = TEXT("");
		ITEMCATEGORY_MAIN eMainCategory = CATEGORY_END;
	}ITEMDESC;

protected:
	CGameItem();
	CGameItem(const CGameItem& rhs);
	virtual ~CGameItem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);

public:
	virtual void Free() override;
};

END

