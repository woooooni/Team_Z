#pragma once

#include "Base.h"

BEGIN(Engine)

class CCascadeMatrixSet final : public CBase
{
public:
	CCascadeMatrixSet();
	virtual ~CCascadeMatrixSet() = default;

public:
	void Init(int iShadowMapSize);
	void Tick(const Vec3& vDirectionalDir);

	void SubdivisionFrustum(const Vec3& vDirectinoalLight);
	void FrustumUpdate();
	void UpdateShadowMatrix(const Vec3& vLightDirectional);

	static const int m_iTotalCascades = 3;
public:
	const Matrix* GetWorldToShadowSpace() const { return &m_WorldToShadowSpace; }
	const Matrix* GetWorldToCascadeProj(_int i) const { return &m_arrWorldToCascadeProj[i]; }
	const Vec4 GetToCascadeOffsetX() const { return m_vToCascadeOffsetX; }
	const Vec4 GetToCascadeOffsetY() const { return m_vToCascadeOffsetY; }
	const Vec4 GetToCascadeScale() const { return m_vToCascadeScale; }

public:
	const _float Get_CascadeEnd(_int index) const { return m_fCascadeEnd[index]; }
	const Matrix Get_ShadowOrthoProj(_int index) const { return m_shadowOrthoProj[index]; }

private:
	void ExtractFrustumPoints(_float fNear, _float fFar, Vec3* arrFrustumCorners);
	void ExtractFrustumBoundSphere(_float fNear, _float fFar, Vec3& vBoundCenter, _float& fBoundRadius);
	bool CascadeNeedsUpdate(const Matrix& mShadowView, _int iCascadeIdx, const Vec3& newCenter, Vec3& vOffset);


public:
	const Matrix Get_ArrayOrthoMatrix(_int i) { return m_ArrayLightOrthoMatrix[i]; }
	const _float Get_CascadePercent(_int i) { return m_fCascadePercent[i]; }

private:
	void CreateLights(const Vec3& vLightDirectional);

public:
	const Matrix Get_LightViewMatrix(_int i) { return m_LightViewProjMatrix[i]; }

private: // 첫 번째 방법
	_int m_iShadowMapSize = 0;
	_float m_fCascadeTotalRange = 50.0f;
	_float m_arrCascadeRanges[4] = {};

	Vec3 m_vShadowBoundCenter;
	_float m_fShadowBoundRadius = 0.0f;
	Vec3 m_arrCascadeBoundCenter[m_iTotalCascades];
	_float m_arrCascadeBoundRadius[m_iTotalCascades] = {};

	Matrix m_WorldToShadowSpace;
	Matrix m_arrWorldToCascadeProj[m_iTotalCascades];

	Vec4 m_vToCascadeOffsetX = ::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	Vec4 m_vToCascadeOffsetY = ::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	Vec4 m_vToCascadeScale = ::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	_bool m_bAntiFlickerOn = true;

private: // 두 번째 방법
	_float m_fCascadeEnd[4] = {};
	Matrix m_shadowOrthoProj[3];

private: // 3번째 방법
	Matrix m_ArrayLightOrthoMatrix[3];
	_float m_fCascadePercent[3] = {};

private: // 4번째 방법
	Matrix m_LightViewProjMatrix[3];
public:
	virtual void Free() override;
};

END

