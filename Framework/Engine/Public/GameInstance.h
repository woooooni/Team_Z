#pragma once


#include "Sound_Manager.h"
#include "Component_Manager.h"

/* 클라이언트개발자가 엔진의 기능을 이용하고자할 때 접촉하는 객체.  */
/* 클라이언트에 보여줘야할 함수들을 모두 정의하고 있는다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(_uint iNumLevels, _uint iNumLayerType, 
		const GRAPHIC_DESC& GraphicDesc, 
		_Inout_ ID3D11Device** ppDevice, 
		_Inout_ ID3D11DeviceContext** ppContext, 
		_In_ HWND hWnd,
		_In_ HINSTANCE hInst);

	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

public:
	_float Compute_TimeDelta(TIMER_TYPE eTimerType);
	HRESULT Set_TimeScale(TIMER_TYPE eTimerType, _float fTimeScale);
	_float Get_TimeScale(TIMER_TYPE eTimerType);
	_float Get_TimeDelta(TIMER_TYPE eTimerType);
	HRESULT Set_Slow(TIMER_TYPE eTimerType, _float fSlowTime, _float fTimeScale, _bool bForce);
	HRESULT	Add_Timer(TIMER_TYPE eTimerType);
	class CTimer* Find_Timer(TIMER_TYPE eTimerType);


public: /* For.Graphic_Device */
	ID3D11Device* Get_Device();
	ID3D11DeviceContext* Get_Context();
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();
	
public: /* For.Input_Device */
	_char Get_DIKState(_uchar eKeyID);
	_char Get_DIMKeyState(DIMK eMouseKeyID);
	_long Get_DIMMoveState(DIMM eMouseMoveID);

	_bool Mouse_Down(DIMK eMouseKeyID);
	_bool Mouse_Up(DIMK eMouseKeyID);
	_bool Mouse_Pressing(DIMK eMouseKeyID);


public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	HRESULT Render_Debug();
	_uint Get_CurrentLevel();

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring & strPrototypeTag, class CGameObject* pPrototype, _uint iLayerType);
	HRESULT Add_GameObject(_uint iLevelIndex, const _uint iLayerType, const wstring & strPrototypeTag, void* pArg = nullptr, __out class CGameObject** ppOut = nullptr);
	HRESULT Add_GameObject(_uint iLevelIndex, const _uint iLayerType, class CGameObject* pGameObject);
	class CGameObject* Clone_GameObject(const wstring & strPrototypeTag, _uint iLayerType, void* pArg = nullptr);
	class CGameObject* Find_Prototype_GameObject(_uint iLayerType, const wstring & strPrototypeTag);
	const map<const wstring, class CGameObject*>& Find_Prototype_GameObjects(_uint iLayerType);
	class CGameObject* Find_GameObject(_uint iLevelIndex, const _uint iLayerType, const wstring & strObjectTag);
	class CGameObject* Find_GameObject(_uint iLevelIndex, const _uint iLayerType, _int iObjectID);
	list<CGameObject*>& Find_GameObjects(_uint iLevelIndex, const _uint iLayerType);
	HRESULT Clear_Layer(_uint iLevelIndex, const _uint iLayerType);

public: /* For. Componenet_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring & strProtoTypeTag, class CComponent* pPrototype);
	class CComponent* Find_Prototype_Component(_uint iLevelIndex, const wstring & strProtoTypeTag);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring & strProtoTypeTag, class CGameObject* pOwner = nullptr, void* pArg = nullptr);
	HRESULT Check_Prototype(_uint iLevelIndex, const wstring & strProtoTypeTag);

public: /* For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	HRESULT Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc);
	HRESULT Add_ShadowLight(_uint iLevelIndex, _vector vEye, _vector vAt, _vector vUp);
	_float4x4 Get_ShadowViewMatrix(_uint iLevelIndex);
	_float4x4 Get_ShadowLightViewMatrix_Inverse(_uint iLevelIndex);
	HRESULT Reset_Lights();


public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_float4x4 Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_float4x4 Get_TransformFloat4x4_TransPose(CPipeLine::TRANSFORMSTATE eTransformState) const;
	_float4 Get_CamPosition();
	_float4x4 Get_TransformMatrixInverse_Float4x4(CPipeLine::TRANSFORMSTATE eTransformState);
	_matrix Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE eTransformState) const;
	HRESULT Bind_TransformToShader(CShader* pShader, const char* pConstantName, CPipeLine::TRANSFORMSTATE eState);

/* For. KeyManager */
public:
	KEY_STATE GetKeyState(KEY _eKey);
	const POINT& GetMousePos();
	void Lock_Mouse();
	void UnLock_Mouse();

public:
	/* For. Model_Manager */
	HRESULT Ready_Model_Data_FromPath(_uint iLevelIndex, _uint eType, const wstring & strFolderPath);
	HRESULT Export_Model_Data(class CModel* pModel, const wstring & strSubFolderName, wstring strFileName);
	HRESULT Import_Model_Data(_uint iLevelIndex, const wstring & strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, __out class CModel** ppOut = nullptr);
	HRESULT Export_Model_Data_FromPath(_uint eType, wstring strFolderPath);


public:
	/* For. Font_Manager */
	HRESULT Add_Fonts(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strFontTag, const wstring & strFontFilePath);
	HRESULT Render_Fonts(const wstring & strFontTag, const wstring & strText, _float2 vPosition, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f),
		_float fAngle = 0.f, _float2 vOrigin = _float2(0.f, 0.f), _float2 vScale = _float2(1.f, 1.f));

	
public:
	/* For. Collision_Manager */
	HRESULT Add_CollisionGroup(COLLISION_GROUP eCollisionGroup, class CGameObject* pGameObject);
	void Reset_CollisionGroup();

public:
	_bool Intersect_Frustum_World(_fvector vWorldPos, _float fRadius = 0.f);



//public:
	/* For. Network_Manager */
//	void Set_ServerSession(ServerSessionRef session);
//	void Send(SendBufferRef sendBuffer);
//	bool Is_Connected();
//	ServerSessionRef& Get_ServerSession();

public:
	void Play_Sound(TCHAR* pSoundKey, CHANNELID eID, _float fVolume, _bool bStop = false);
	void Play_BGM(TCHAR* pSoundKey, _float fVolume, _bool bStop = false);
	void Stop_Sound(CHANNELID eID);
	void Stop_All();
	void Set_ChannelVolume(CHANNELID eID, float fVolume);
	FMOD_CHANNEL* Get_Channel(CHANNELID eID);


private:
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CLight_Manager*			m_pLight_Manager = { nullptr };
	class CKey_Manager*				m_pKey_Manager = { nullptr };
	class CModel_Manager*			m_pModel_Manager = { nullptr };
	class CFont_Manager*			m_pFont_Manager = { nullptr };
	class CCollision_Manager*		m_pCollision_Manager = { nullptr };
	class CTarget_Manager*			m_pTarget_Manager = { nullptr };
	class CFrustum*					m_pFrustum = { nullptr };
	// class CNetwork_Manager*			m_pNetwork_Manager = { nullptr };
	class CSound_Manager* m_pSound_Manager = { nullptr };


public:
	static void Release_Engine();
	virtual void Free() override;
};

END