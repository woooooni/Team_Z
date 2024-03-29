#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_SwordMan_Skill_SpinningAssault final : public CVfx
{
protected:
	CVfx_SwordMan_Skill_SpinningAssault(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_SwordMan_Skill_SpinningAssault(const CVfx_SwordMan_Skill_SpinningAssault& rhs);
	virtual ~CVfx_SwordMan_Skill_SpinningAssault() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_int m_iType = -1;
	_float3 m_fMainColor;
	_float3 m_fLightColor;

private:
	class CEffect* m_pEffect_Trail0 = nullptr; // 특정 조각 생성할 때 해당 uv 반복 끄기
	class CEffect* m_pEffect_Trail1 = nullptr;

public:
	static CVfx_SwordMan_Skill_SpinningAssault* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END