#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CPhysX_Controller;
END

BEGIN(Client)

class CCamera_Follow final : public CCamera
{
	typedef struct tagCameraDampingDesc
	{
		/* 댐핑을 호출하면 현재 위치가 vCurPos에 등록되고, 매 프레임 vTargetPos에 카메라 위치가 저장되고 */
		/* 두 벡터의 차가 condition만큼 벌어지면 댐핑이 시작된다. */

		_bool	bDamping = true;
		_bool	bSet = false;

		Vec4	vCurPos;									/* 현재 카메라 포지션 */
		Vec4	vTargetPos;									/* 목표 카메라 포지션 */
		_float	fDampingCoefficient			= 0.025f;		/* 144 프레임 기준 댐핑 계수 0 ~ 1 (값이 클수록 빨리 따라감)*/
		_float	fDampingCoefficientBackMag	= 3.f;			/* 타겟의 룩과 카메라의 룩이 역방향일 경우 DampingCoeff에 곱해짐 (더 빨리 쫓아감) */
		_float	fDampingBackLimitRad		= 1.57f;		/* 역방향임을 판단할 기준 각도 */

	}DAMPING_DESC;

	enum class LOCK_PROGRESS { OFF, START_BLENDING, FINISH_BLEIDING };

private:
	CCamera_Follow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Follow(const CCamera_Follow& rhs);
	virtual ~CCamera_Follow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override { return S_OK; }

public:
	virtual void Tick_Blending(const _float fDeltaTime) override;

public:
	/* Access */
	const _float& Get_DampingCoefficient() const { return m_tDampingDesc.fDampingCoefficient; }
	void Set_DampingCoefficient(const _float& fCoefficient) { m_tDampingDesc.fDampingCoefficient = fCoefficient; }

	const _float& Get_DampingCoefficientBackMag() const { return m_tDampingDesc.fDampingCoefficientBackMag; }
	void Set_DampingCoefficientBackMag(const _float& fMag) { m_tDampingDesc.fDampingCoefficientBackMag = fMag; }

	const _float& Get_DampingBackLimitRad() const { return m_tDampingDesc.fDampingBackLimitRad; }
	void Set_DampingBackLimitRad(const _float& fRad) { m_tDampingDesc.fDampingBackLimitRad = fRad; }

	const _bool& Is_Damping() const { return m_tDampingDesc.bDamping; }
	void Set_Damping(const _bool& bDamping) { m_tDampingDesc.bDamping = bDamping; m_tDampingDesc.bSet = bDamping; }

	const _float& Get_MaxRotationLimitDeltaY() const { return m_fMaxRotLimitDeltaY; }
	const _float& Get_MinRotationLimitDeltaY() const { return m_fMinRotLimitDeltaY; }

	void Set_MaxRotationLimitDeltaY(const _float& fLimit) { m_fMaxRotLimitDeltaY = fLimit; }
	void Set_MinRotationLimitDeltaY(const _float& fLimit) { m_fMinRotLimitDeltaY = fLimit; }

	const _bool& Is_LockOn() const { return (LOCK_PROGRESS::OFF == m_eLockProgress) ? false : true; }

	void Set_Default_Position();

	void Reset_Damping() { m_tDampingDesc.bSet = false; }
	void Reset_DampingBackLimitRad() { m_tDampingDesc.fDampingBackLimitRad = 1.57f; }

	virtual Vec4 Get_LookAt() override;
	virtual void Set_Blending(const _bool& bBlending) override;

	void Set_CanWideView(const _bool& bCanWideView); /* 와이드뷰 전환 가능성을 아예 막거나 연다. */
	void Reset_WideView_To_DefaultView(const _bool& bDirect, const _float& fMag = 1.f); /* 와이드 뷰에서 디폴트 뷰로 바로 전환 혹은 서서히 전환한다. */
	const _bool& Is_WideView() const { return m_bWideView; }

	const CAMERA_VIEW_TYPE& Get_ViewType() const { return m_eViewType; }
	void Set_ViewType(const CAMERA_VIEW_TYPE& eType);

	void Set_MinMaxLimitY(const _float fMinY, const _float fMaxY);
	void Reset_MinMaxLimitY();

	void Set_Defualt_Setting();

	virtual void Set_Active(const _bool bActive) override;

public:
	/* Lock On */
	HRESULT Start_LockOn(CGameObject* pTargetObject, const Vec4& vTargetOffset, const Vec4& vLookAtOffset, const _float& fLockOnBlendingTime = Cam_LockOn_Time_BlendingIn);
	HRESULT Finish_LockOn(CGameObject* pTargetObject, const _float& fLockOnBlendingTime = Cam_LockOn_Time_BlendingOut);
	HRESULT Lock_LookHeight();
	const _bool Is_Lock_LookHeight() const { return m_bLockLookHeight; }
	void Set_LockBoneNumber(const _uint& iNum) { m_iLockBoneNumber = iNum; }

private:
	/* Calculation */
	Vec4 Calculate_WorldPosition(_float fTimeDelta);
	Vec4 Calculate_LoaclSphericalPosition(_float fTimeDelta);
	Vec4 Calculate_Look();
	Vec4 Calculate_DampingPosition(Vec4 vGoalPos);

private:
	void Check_Exception();
	void Check_WideView(_float fTimeDelta);

	void Set_WideView(const _bool& bWideView, const _float& fMag = 1.f);

	const _bool Is_Target_Exception();

	void Test(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;
	void Tick_Transform(const _float fDeltaTime);

private:
	/* 구면 좌표계 */
	Vec2			m_vAngle				= { -1.57f, 1.8f }; /* x가 0일 경우 플레이어 라이트에서 시작*/
	const _float	m_fDefaultAngleY		= 1.3f ;

	/* 구면 좌표계에서 카메라의 최대 최소 y 값*/
	_float			m_fMinLimitY			= Cam_Follow_Limit_Min_Y;
	_float			m_fMaxLimitY			= Cam_Follow_Limit_Max_Y;

	/* 회전량이 너무 많거나 적을경우 카메라가 획 도는 경우를 방지하기 위한 Limit값 */
	_float			m_fMaxRotLimitDeltaY	= 0.05f ;
	_float			m_fMinRotLimitDeltaY	= -0.05f ;

	/* Damping */
	DAMPING_DESC	m_tDampingDesc			= {};

	/* Lock On */
	LOCK_PROGRESS	m_eLockProgress			= LOCK_PROGRESS::OFF;
	_bool			m_bLockLookHeight		= false;
	_float			m_fLockLookHeight		= 0.f;
	_uint			m_iLockBoneNumber		= 0;

	/* 카메라의 월드 행렬 상태 변환으로 인해 오프셋의 y가 -가 되어 땅을 뚫는 현상 방지*/
	_float			m_fLockTargetOffsetMinY = 0.5f;

	Vec4			m_vPrevLookAt			= {};

	/* Wide Follow */
	_bool			m_bCanWideView			= true;
	_bool			m_bWideView				= false;
	_float			m_fAccForWideView		= 0.f;
	const _float	m_fWideViewCheckTime	= 4.f;

	const _float	m_fDefaultViewCheckTime = 3.f;
	const _float	m_fWideViewLerpTime		= 1.f;
	const LERP_MODE	m_eWideViewLerpMode		= LERP_MODE::SMOOTHER_STEP;

	/* View */
	CAMERA_VIEW_TYPE m_eViewType			= CAMERA_VIEW_TYPE::SHOLDER;
	
public:
	static CCamera_Follow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END