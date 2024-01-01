#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_CutScene final : public CCamera
{
private:
	CCamera_CutScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_CutScene(const CCamera_CutScene& rhs);
	virtual ~CCamera_CutScene() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Start_CutScene(const string& strCutSceneName);
	const _bool Is_Playing_CutScenc() const { return m_tTimeDesc.bActive; }
public:
	static Vec4 Get_Point_In_Bezier(Vec3 vPoints[MAX_BEZIER_POINT], const _float& fRatio);

public:
	HRESULT Save_CutSceneDescs();
	HRESULT Load_CutSceneDescs();

public:
	/* Tool */
	HRESULT	Add_CutSceneDesc(const CAMERA_CUTSCENE_DESC& desc);
	HRESULT Del_CutSceneDesc(const string& strCutSceneName);
	HRESULT Change_CutSceneDesc(const _int& iIndex, const CAMERA_CUTSCENE_DESC& desc);
	CAMERA_CUTSCENE_DESC* Find_CutSceneDesc(const string& strCutSceneName);
	const vector<CAMERA_CUTSCENE_DESC>& Get_CutSceneDescs() const { return m_CutSceneDescs; }

private:
	virtual HRESULT Ready_Components() override;

private:
	vector<CAMERA_CUTSCENE_DESC>	m_CutSceneDescs;

	CAMERA_CUTSCENE_DESC*			m_pCurCutSceneDesc = nullptr;

	LERP_TIME_DESC					m_tTimeDesc;
	
public:
	static CCamera_CutScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END