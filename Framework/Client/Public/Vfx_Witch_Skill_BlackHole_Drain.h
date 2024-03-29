#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Witch_Skill_BlackHole_Drain final : public CVfx
{
private:
	enum TYPE {
		TYPE_E_SPHERE,
		TYPE_E_DRAIN,

		TYPE_D_WARNING,

		TYPE_E_SPHERE_SMALLER,
		TYPE_E_DRAND_SMALLER,

		TYPE_E_SPHERE_SMALLER02,
		TYPE_E_DRAND_SMALLER02,

		TYPE_END
	};

protected:
	CVfx_Witch_Skill_BlackHole_Drain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Witch_Skill_BlackHole_Drain(const CVfx_Witch_Skill_BlackHole_Drain& rhs);
	virtual ~CVfx_Witch_Skill_BlackHole_Drain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pBlackHole = nullptr;
	class CEffect* m_pBlackHoleSmaller = nullptr;
	class CEffect* m_pBlackHoleSmaller02 = nullptr;

	class CDecal* m_pWarningDecal = nullptr;

	_bool m_bIsCreate = false;
	_bool m_bIsSmaller = false;
	_bool m_bIsSmaller02 = false;


public:
	static CVfx_Witch_Skill_BlackHole_Drain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END