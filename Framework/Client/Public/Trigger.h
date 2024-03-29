#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CTrigger final : public CGameObject
{
public:
	typedef struct tagTriggerDesc
	{
		TRIGGER_TYPE eTriggerType = TRIGGER_TYPE::TRIGGER_END;

		// Default
		Vec4 vStartPosition = { 0.f, 0.f, 0.f, 1.f };
		Vec3 vExtents = { 100.f, 100.f, 100.f };

		// For.Trigger ChangeBGM
		string strBGM = "";

		// For.Trigger MapName
		wstring strMapName = TEXT("");

		Vec4 vEye;
		Vec4 vAt;

	} TRIGGER_DESC;

private:
	CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrigger(const CTrigger& rhs);
	virtual ~CTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo);
	virtual void Collision_Continue(const COLLISION_INFO& tInfo);
	virtual void Collision_Exit(const COLLISION_INFO& tInfo);

private:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_Collider();

private:
	void Load_Monster_Data(const wstring& strFilePath, _uint Section);

public:
	const TRIGGER_TYPE& Get_TriggerType() const { return m_eTriggerType; }
	void Set_TriggerType(TRIGGER_TYPE eType) { m_eTriggerType = eType; }

	void Set_BgmName(const wstring& bgmName) { m_strBGM = bgmName; }
	void Set_strMapName(const wstring& mapName) { m_strMapName = mapName; }

	const wstring& Get_BgmName() const { return m_strBGM; }
	const wstring& Get_MapName() const { return m_strMapName; }

	void Set_At(Vec4 vAt) { m_vAt = vAt; }
	void Set_Eye(Vec4 vEye) { m_vEye = vEye; }
	void Set_Up(Vec4 vUp) { m_vUp = vUp; }

	Vec4& Get_At() { return m_vAt; }
	Vec4& Get_Eye() { return m_vEye; }
	Vec4& Get_Up() { return m_vUp; }

private:
	TRIGGER_TYPE m_eTriggerType = TRIGGER_TYPE::TRIGGER_END;
	Vec3 m_vExtents = { 100.f, 100.f, 100.f };
	
private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;


private:
	wstring m_strBGM = L"";
	wstring m_strMapName = L"";
	
	_bool m_bWhaleCutScene = false;

	Vec4 m_vAt = Vec4(0.0f,0.0f,1.0f,1.0f);
	Vec4 m_vEye = Vec4(0.0f,0.0f, 0.0f,1.0f);
	Vec4 m_vUp = Vec4(0.0f, 1.0f, 0.0f, 1.0f);

public:
	static CTrigger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

