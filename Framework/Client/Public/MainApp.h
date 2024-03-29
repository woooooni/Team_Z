#pragma once

#include "Client_Defines.h"

/* 클래스를 만드는 방법. */
/* 
class C클래스이름
{
private or protected:
	생성자. 
	소멸자.
public: 
	Getter 
public:	 
	Setter 
public:
	함수
protected:
	변수
protected:
	함수
private:
	변수
private:
	함수

public:
	생성관련한 함수. (Create, Clone)	
	삭제관련한 함수. (Free)
};
*/

/* 내 게임의 전반적인 초기화(Initialize), 갱신(Tick), 렌더링(Render)을 수행한다. */

#include "Base.h"


BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

BEGIN(Client)
class CNetwork_Manager;

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	/* 내 게임의 시작. */
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CRenderer*				m_pRenderer_Com = { nullptr };


private:
private:
	_float			m_fTimeAcc = 0.f;
	_tchar			m_szFPS[MAX_PATH] = TEXT("");
	_uint			m_iNumDraw = 0;

private:
	HRESULT Open_Level(LEVELID eLevelID, const wstring& strFolderName);

private:
	HRESULT Initialize_Client();
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_UI_TextureComponent();
	HRESULT Ready_Effect_TextureComponent();
	HRESULT Ready_CameraObject();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END

