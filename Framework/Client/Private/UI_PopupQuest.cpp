#include "stdafx.h"
#include "UI_PopupQuest.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UIMinigame_Manager.h"

CUI_PopupQuest::CUI_PopupQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTPOPUP eType, UI_POPUP_SEPARATOR eSeparatorType)
	: CUI(pDevice, pContext, L"UI_PopupQuest")
	, m_eType(eType)
	, m_eSeparator(eSeparatorType)
{
}

CUI_PopupQuest::CUI_PopupQuest(const CUI_PopupQuest& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
	, m_eSeparator(rhs.m_eSeparator)
{
}

void CUI_PopupQuest::Set_Active(_bool bActive)
{
	if (POPUPFRAME_TOP == m_eType || POPUPFRAME_BOTTOM == m_eType || POPUP_SEPARATOR == m_eType)
	{
		if (bActive)
			m_fAppearProg = 1.f;
	}
	else
	{
		if (m_bEvent)
			m_bEvent = false;
	}

	m_bActive = bActive;
}

void CUI_PopupQuest::Set_Contents(const wstring& strQuestType, const wstring& strTitle, const wstring& strContents)
{
	// 병합시 오류 방지. 임시로 살려둠.

	if (CUI_PopupQuest::POPUP_WINDOW != m_eType)
		return;

	if (4 <= m_Quest.size())
		return;

	/*
	_float4(0.957f, 0.784f, 0.067f, 1.f) 메인
	_float4(0.165f, 0.984f, 0.957f, 1.f) 명성
	_float4(0.373f, 0.863f, 0.647f, 1.f) 제비
	_float4(0.898f, 0.624f, 0.333f, 1.f) 토벌
	_float4(0.914f, 0.443f, 0.392f, 1.f) 수배

	_float4(0.804f, 0.843f, 0.741f, 1.f) 내용
	*/

	m_bProgressing = true; // 퀘스트가 완료되면 false로 전환할 수 있는 매개가 필요함.

	if (TEXT("[메인]") == strQuestType)
		m_vTypeColor = _float4(0.957f, 0.784f, 0.067f, 1.f);
	else if (TEXT("[서브]") == strQuestType)
		m_vTypeColor = _float4(0.165f, 0.984f, 0.957f, 1.f);
	else if (TEXT("[수배]") == strQuestType)
		m_vTypeColor = _float4(0.914f, 0.443f, 0.392f, 1.f);
	else
		m_vTypeColor = _float4(0.373f, 0.863f, 0.647f, 1.f);

	m_vTextColor = _float4(0.804f, 0.843f, 0.741f, 1.f);
	m_vMeterColor = _float4(0.67f, 0.7f, 0.63f, 1.f);

	QUEST_INFO QuestDesc = {};
 	QuestDesc.strType = strQuestType;
	QuestDesc.strTitle = strTitle;
	QuestDesc.strContents = strContents;
	m_Quest.push_back(QuestDesc);
}

void CUI_PopupQuest::Set_Contents(const QUEST_INFO& tQuestInfo)
{
	if (4 <= m_Quest.size())
		return;

	QUEST_INFO QuestDesc = {};
	QuestDesc = tQuestInfo;

	m_bProgressing = true; // 퀘스트가 완료되면 false로 전환할 수 있는 매개가 필요함.

	if (TEXT("[메인]") == QuestDesc.strType)
		m_vTypeColor = _float4(0.957f, 0.784f, 0.067f, 1.f);
	else if (TEXT("[서브]") == QuestDesc.strType)
		m_vTypeColor = _float4(0.165f, 0.984f, 0.957f, 1.f);
	else if (TEXT("[수배]") == QuestDesc.strType)
		m_vTypeColor = _float4(0.914f, 0.443f, 0.392f, 1.f);
	else
		m_vTypeColor = _float4(0.373f, 0.863f, 0.647f, 1.f);

	m_vTextColor = _float4(0.804f, 0.843f, 0.741f, 1.f);
	m_vMeterColor = _float4(0.67f, 0.7f, 0.63f, 1.f);
	//m_vMeterColor = _float4(0.4f, 0.44f, 0.34f, 1.f);

	m_Quest.push_back(QuestDesc);
}

void CUI_PopupQuest::Update_QuestContents(const wstring& strPreTitle, const wstring& strQuestType, const wstring& strTitle, const wstring& strContents)
{
	if (CUI_PopupQuest::POPUP_WINDOW != m_eType)
		return;

	if (0 >= m_Quest.size())
		return;

	for (auto iter = m_Quest.begin(); iter != m_Quest.end(); ++iter)
	{
		if (strPreTitle == iter->strTitle)
		{
			if (iter->strContents == strContents)
				return; // 추가

			iter->strType = strQuestType;
			iter->strTitle = strTitle;
			iter->strContents = strContents;
			break;
		}
	}
}

void CUI_PopupQuest::Update_QuestContents(const wstring& strPreTitle, const QUEST_INFO& tQuestInfo)
{
	if (CUI_PopupQuest::POPUP_WINDOW != m_eType)
		return;

	if (0 >= m_Quest.size())
		return;

	QUEST_INFO QuestDesc = {};
	QuestDesc = tQuestInfo;

	for (auto iter = m_Quest.begin(); iter != m_Quest.end(); ++iter)
	{
		if (strPreTitle == iter->strTitle)
		{
			// 콘텐츠가 같아도 업데이트 되어야한다
			//if (iter->strContents == QuestDesc.strContents)
			//	return;

			iter->strType = QuestDesc.strType;
			iter->strTitle = QuestDesc.strTitle;
			iter->strContents = QuestDesc.strContents;
			iter->bCreateSpot = QuestDesc.bCreateSpot;
			iter->vDestPosition = QuestDesc.vDestPosition;
			break;
		}
	}
}

void CUI_PopupQuest::Clear_Quest(const wstring& strTitle)
{
	if (CUI_PopupQuest::POPUP_WINDOW != m_eType)
		return;

	if (0 >= m_Quest.size())
		return;

	for (auto iter = m_Quest.begin(); iter != m_Quest.end(); ++iter)
	{
		if (strTitle == iter->strTitle)
		{
			m_Quest.erase(iter);
			break;
		}
	}
}

_int CUI_PopupQuest::Get_NumOfQuest()
{
	if (CUI_PopupQuest::POPUP_WINDOW != m_eType)
		return -1;

	return m_Quest.size();
}

const CUI_PopupQuest::QUEST_INFO& CUI_PopupQuest::Get_QuestContents(_int iSlotNum)
{
	// 첫번째 윈도우에서 클릭된 창에 해당하는 퀘스트 정보를 가지고 온다.
	// 예외처리는 UIManager에서 수행한다.

	return m_Quest[iSlotNum];
}

HRESULT CUI_PopupQuest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PopupQuest::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_bUseMouse = true;

	if (m_eType == POPUP_WINDOW)
	{
		m_Quest.reserve(4);
	}

	return S_OK;
}

void CUI_PopupQuest::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUESTPOPUP_END == m_eType)
			return;

		if (m_fAppearProg <= 1.f)
			m_fAppearProg += fTimeDelta * 10.f;

		__super::Tick(fTimeDelta);
	}
}

void CUI_PopupQuest::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUESTPOPUP_END == m_eType)
			return;

		if (POPUP_WINDOW == m_eType)
		{
			_float fOffsetY = 70.f;

			if (m_bProgressing)
			{
				if (LEVELID::LEVEL_EVERMORE == GI->Get_CurrentLevel() &&
					true == CUIMinigame_Manager::GetInstance()->Is_GrandprixIntroStarted())
					return;

				/*
				길이에 따른 Text Size, TextPosition
				TitleDesc.strText Length를 구한다.
				0.3f, 0.3f로 사이즈를 잡고
				포지션도 y값은 +5.f를 해주면 됨.
				*/
				_int iLength;
				_float2 vScale = _float2(0.4f, 0.4f);
				_float fScaleOffset = 0.f;

				if (0 < m_Quest.size() && 4 >= m_Quest.size())
				{
					if (TEXT("[메인]") == m_Quest[0].strType)
						m_vTypeColor = _float4(0.957f, 0.784f, 0.067f, 1.f);
					else if (TEXT("[서브]") == m_Quest[0].strType)
						m_vTypeColor = _float4(0.165f, 0.984f, 0.957f, 1.f);
					else if (TEXT("[수배]") == m_Quest[0].strType)
						m_vTypeColor = _float4(0.914f, 0.443f, 0.392f, 1.f);
					else
						m_vTypeColor = _float4(0.373f, 0.863f, 0.647f, 1.f);

					CRenderer::TEXT_DESC TypeDesc;
					TypeDesc.strText = m_Quest[0].strType;
					TypeDesc.strFontTag = L"Default_Bold";
					TypeDesc.vScale = { 0.4f, 0.4f };
					TypeDesc.vColor = m_vTypeColor;
					TypeDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY - 20.f);
					m_pRendererCom->Add_Text(TypeDesc);

					CRenderer::TEXT_DESC TitleDesc;
					TitleDesc.strText = m_Quest[0].strTitle;
					TitleDesc.strFontTag = L"Default_Bold";
					TitleDesc.vScale = { 0.4f, 0.4f };
					TitleDesc.vColor = m_vTextColor;
					TitleDesc.vPosition = _float2(m_tInfo.fX - 50.f, m_tInfo.fY - 20.f);
					m_pRendererCom->Add_Text(TitleDesc);

					CRenderer::TEXT_DESC ContentsDesc;
					ContentsDesc.strText = m_Quest[0].strContents;
					iLength = CUI_Manager::GetInstance()->Count_OnlyWords(m_Quest[0].strContents);
					if (18 < iLength)
					{
						vScale = _float2(0.3f, 0.3f);
						fScaleOffset = 5.f;
					}
					else
					{
						vScale = _float2(0.4f, 0.4f);
						fScaleOffset = 0.f;
					}
					ContentsDesc.strFontTag = L"Default_Bold";
					ContentsDesc.vScale = vScale;
					ContentsDesc.vColor = m_vTextColor;
					ContentsDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY + fScaleOffset);
					m_pRendererCom->Add_Text(ContentsDesc);

					if (true == m_Quest[0].bCreateSpot)
					{
						_float fDistance = CUI_Manager::GetInstance()->Calculate_Distance_FromPlayer(m_Quest[0].vDestPosition);
						_float fOffset = (to_wstring(_int(fDistance)).length() - 1) * 5.f;

						if (1.f <= fDistance)
						{
							CRenderer::TEXT_DESC DistanceDesc;
							DistanceDesc.strText = to_wstring(_int(fDistance)) + L"M";
							DistanceDesc.strFontTag = L"Default_Medium";
							DistanceDesc.vScale = { 0.25f, 0.25f };
							DistanceDesc.vColor = m_vMeterColor;
							DistanceDesc.vPosition = _float2(m_tInfo.fX + 110.f - fOffset, m_tInfo.fY + 20.f);
							m_pRendererCom->Add_Text(DistanceDesc);
						}
					}

					if (2 <= m_Quest.size())
					{
						if (TEXT("[메인]") == m_Quest[1].strType)
							m_vTypeColor = _float4(0.957f, 0.784f, 0.067f, 1.f);
						else if (TEXT("[서브]") == m_Quest[1].strType)
							m_vTypeColor = _float4(0.165f, 0.984f, 0.957f, 1.f);
						else if (TEXT("[수배]") == m_Quest[1].strType)
							m_vTypeColor = _float4(0.914f, 0.443f, 0.392f, 1.f);
						else
							m_vTypeColor = _float4(0.373f, 0.863f, 0.647f, 1.f);

						// 두번째 퀘스트 Text추가함.
						CRenderer::TEXT_DESC TypeDesc;
						TypeDesc.strText = m_Quest[1].strType;
						TypeDesc.strFontTag = L"Default_Bold";
						TypeDesc.vScale = { 0.4f, 0.4f };
						TypeDesc.vColor = m_vTypeColor;
						TypeDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY - 20.f + fOffsetY);
						m_pRendererCom->Add_Text(TypeDesc);

						CRenderer::TEXT_DESC TitleDesc;
						TitleDesc.strText = m_Quest[1].strTitle;
						TitleDesc.strFontTag = L"Default_Bold";
						TitleDesc.vScale = { 0.4f, 0.4f };
						TitleDesc.vColor = m_vTextColor;
						TitleDesc.vPosition = _float2(m_tInfo.fX - 50.f, m_tInfo.fY - 20.f + fOffsetY);
						m_pRendererCom->Add_Text(TitleDesc);

						CRenderer::TEXT_DESC ContentsDesc;
						ContentsDesc.strText = m_Quest[1].strContents;
						iLength = CUI_Manager::GetInstance()->Count_OnlyWords(m_Quest[1].strContents);
						if (18 < iLength)
						{
							vScale = _float2(0.3f, 0.3f);
							fScaleOffset = 5.f;
						}
						else
						{
							vScale = _float2(0.4f, 0.4f);
							fScaleOffset = 0.f;
						}
						ContentsDesc.strFontTag = L"Default_Bold";
						ContentsDesc.vScale = vScale;
						ContentsDesc.vColor = m_vTextColor;
						ContentsDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY + fOffsetY + fScaleOffset);
						m_pRendererCom->Add_Text(ContentsDesc);

						if (true == m_Quest[1].bCreateSpot)
						{
							_float fDistance = CUI_Manager::GetInstance()->Calculate_Distance_FromPlayer(m_Quest[1].vDestPosition);
							_float fOffset = (to_wstring(_int(fDistance)).length() - 1) * 5.f;

							if (1.f <= fDistance)
							{
								CRenderer::TEXT_DESC DistanceDesc;
								DistanceDesc.strText = to_wstring(_int(fDistance)) + L"M";
								DistanceDesc.strFontTag = L"Default_Medium";
								DistanceDesc.vScale = { 0.25f, 0.25f };
								DistanceDesc.vColor = m_vMeterColor;
								DistanceDesc.vPosition = _float2(m_tInfo.fX + 110.f - fOffset, m_tInfo.fY + 20.f + fOffsetY);
								m_pRendererCom->Add_Text(DistanceDesc);
							}
						}

						if (3 <= m_Quest.size())
						{
							if (TEXT("[메인]") == m_Quest[2].strType)
								m_vTypeColor = _float4(0.957f, 0.784f, 0.067f, 1.f);
							else if (TEXT("[서브]") == m_Quest[2].strType)
								m_vTypeColor = _float4(0.165f, 0.984f, 0.957f, 1.f);
							else if (TEXT("[수배]") == m_Quest[2].strType)
								m_vTypeColor = _float4(0.914f, 0.443f, 0.392f, 1.f);
							else
								m_vTypeColor = _float4(0.373f, 0.863f, 0.647f, 1.f);

							// 세번째 퀘스트 Text추가함.
							CRenderer::TEXT_DESC TypeDesc;
							TypeDesc.strText = m_Quest[2].strType;
							TypeDesc.strFontTag = L"Default_Bold";
							TypeDesc.vScale = { 0.4f, 0.4f };
							TypeDesc.vColor = m_vTypeColor;
							TypeDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY - 20.f + (fOffsetY * 2.f));
							m_pRendererCom->Add_Text(TypeDesc);

							CRenderer::TEXT_DESC TitleDesc;
							TitleDesc.strText = m_Quest[2].strTitle;
							TitleDesc.strFontTag = L"Default_Bold";
							TitleDesc.vScale = { 0.4f, 0.4f };
							TitleDesc.vColor = m_vTextColor;
							TitleDesc.vPosition = _float2(m_tInfo.fX - 50.f, m_tInfo.fY - 20.f + (fOffsetY * 2.f));
							m_pRendererCom->Add_Text(TitleDesc);

							CRenderer::TEXT_DESC ContentsDesc;
							ContentsDesc.strText = m_Quest[2].strContents;
							iLength = CUI_Manager::GetInstance()->Count_OnlyWords(m_Quest[2].strContents);
							if (18 < iLength)
							{
								vScale = _float2(0.3f, 0.3f);
								fScaleOffset = 5.f;
							}
							else
							{
								vScale = _float2(0.4f, 0.4f);
								fScaleOffset = 0.f;
							}
							ContentsDesc.strFontTag = L"Default_Bold";
							ContentsDesc.vScale = vScale;
							ContentsDesc.vColor = m_vTextColor;
							ContentsDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY + (fOffsetY * 2.f) + fScaleOffset);
							m_pRendererCom->Add_Text(ContentsDesc);

							if (true == m_Quest[2].bCreateSpot)
							{
								_float fDistance = CUI_Manager::GetInstance()->Calculate_Distance_FromPlayer(m_Quest[2].vDestPosition);
								_float fOffset = (to_wstring(_int(fDistance)).length() - 1) * 5.f;

								if (1.f <= fDistance)
								{
									CRenderer::TEXT_DESC DistanceDesc;
									DistanceDesc.strText = to_wstring(_int(fDistance)) + L"M";
									DistanceDesc.strFontTag = L"Default_Medium";
									DistanceDesc.vScale = { 0.25f, 0.25f };
									DistanceDesc.vColor = m_vMeterColor;
									DistanceDesc.vPosition = _float2(m_tInfo.fX + 110.f - fOffset, m_tInfo.fY + 20.f + (fOffsetY * 2.f));
									m_pRendererCom->Add_Text(DistanceDesc);
								}
							}

							if (4 <= m_Quest.size())
							{
								if (TEXT("[메인]") == m_Quest[3].strType)
									m_vTypeColor = _float4(0.957f, 0.784f, 0.067f, 1.f);
								else if (TEXT("[서브]") == m_Quest[3].strType)
									m_vTypeColor = _float4(0.165f, 0.984f, 0.957f, 1.f);
								else if (TEXT("[수배]") == m_Quest[3].strType)
									m_vTypeColor = _float4(0.914f, 0.443f, 0.392f, 1.f);
								else
									m_vTypeColor = _float4(0.373f, 0.863f, 0.647f, 1.f);

								CRenderer::TEXT_DESC TypeDesc;
								TypeDesc.strText = m_Quest[3].strType;
								TypeDesc.strFontTag = L"Default_Bold";
								TypeDesc.vScale = { 0.4f, 0.4f };
								TypeDesc.vColor = m_vTypeColor;
								TypeDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY - 20.f + (fOffsetY * 3.f));
								m_pRendererCom->Add_Text(TypeDesc);

								CRenderer::TEXT_DESC TitleDesc;
								TitleDesc.strText = m_Quest[3].strTitle;
								TitleDesc.strFontTag = L"Default_Bold";
								TitleDesc.vScale = { 0.4f, 0.4f };
								TitleDesc.vColor = m_vTextColor;
								TitleDesc.vPosition = _float2(m_tInfo.fX - 50.f, m_tInfo.fY - 20.f + (fOffsetY * 3.f));
								m_pRendererCom->Add_Text(TitleDesc);

								CRenderer::TEXT_DESC ContentsDesc;
								ContentsDesc.strText = m_Quest[3].strContents;
								iLength = CUI_Manager::GetInstance()->Count_OnlyWords(m_Quest[3].strContents);
								if (18 < iLength)
								{
									vScale = _float2(0.3f, 0.3f);
									fScaleOffset = 5.f;
								}
								else
								{
									vScale = _float2(0.4f, 0.4f);
									fScaleOffset = 0.f;
								}
								ContentsDesc.strFontTag = L"Default_Bold";
								ContentsDesc.vScale = vScale;
								ContentsDesc.vColor = m_vTextColor;
								ContentsDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY + (fOffsetY * 3.f) + fScaleOffset);
								m_pRendererCom->Add_Text(ContentsDesc);

								if (true == m_Quest[3].bCreateSpot)
								{
									_float fDistance = CUI_Manager::GetInstance()->Calculate_Distance_FromPlayer(m_Quest[3].vDestPosition);
									_float fOffset = (to_wstring(_int(fDistance)).length() - 1) * 5.f;

									if (1.f <= fDistance)
									{
										CRenderer::TEXT_DESC DistanceDesc;
										DistanceDesc.strText = to_wstring(_int(fDistance)) + L"M";
										DistanceDesc.strFontTag = L"Default_Medium";
										DistanceDesc.vScale = { 0.25f, 0.25f };
										DistanceDesc.vColor = m_vMeterColor;
										DistanceDesc.vPosition = _float2(m_tInfo.fX + 110.f - fOffset, m_tInfo.fY + 20.f + (fOffsetY * 3.f));
										m_pRendererCom->Add_Text(DistanceDesc);
									}
								}
							}
						}
					}
				}
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PopupQuest::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(8);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_PopupQuest::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_PopupQuest::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_PopupQuest::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

void CUI_PopupQuest::Move_BottomFrame(_int iNumOfQuest)
{
	// 퀘스트 개수에 따른 BottomFrame 위치 변경
	if (0 >= iNumOfQuest || 4 < iNumOfQuest)
		return;

	_float2 m_vPoisition = _float2(0.f, 0.f);

	switch (iNumOfQuest)
	{
	case 1:
		m_vPoisition = _float2(210.f, 205.f);
		break;
	case 2:
		m_vPoisition = _float2(210.f, 275.f);
		break;
	case 3:
		m_vPoisition = _float2(210.f, 345.f);
		break;
	case 4:
		m_vPoisition = _float2(210.f, 415.f);
		break;
	}

	m_tInfo.fX = m_vPoisition.x;
	m_tInfo.fY = m_vPoisition.y;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
}

HRESULT CUI_PopupQuest::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case POPUPFRAME_TOP:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case POPUPFRAME_BOTTOM:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case POPUP_WINDOW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Window"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.4f;
		break;

	case POPUP_SEPARATOR:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_PopupQuest::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_PopupQuest::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (POPUPFRAME_TOP == m_eType || POPUPFRAME_BOTTOM == m_eType || POPUP_SEPARATOR == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
	}
	else if (POPUP_WINDOW == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_LoadingProgress", &m_fAppearProg, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CUI_PopupQuest::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (POPUP_WINDOW == m_eType)
		{
			_int iWindow = -1;

			if (TEXT("UI_PopupQuest_FirstWindow") == Get_ObjectTag())
				iWindow = 0;
			else if (TEXT("UI_PopupQuest_SecondWindow") == Get_ObjectTag())
				iWindow = 1;
			else if (TEXT("UI_PopupQuest_ThirdWindow") == Get_ObjectTag())
				iWindow = 2;
			else if (TEXT("UI_PopupQuest_FourthWindow") == Get_ObjectTag())
				iWindow = 3;
			else
				iWindow = -1;

			if (-1 == iWindow)
				return;

			CUI_Manager::GetInstance()->Set_QuestDestSpot(iWindow);
			// + Sound
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_1.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
		}
	}
}

CUI_PopupQuest* CUI_PopupQuest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTPOPUP eType, UI_POPUP_SEPARATOR eSeparatorType)
{
	CUI_PopupQuest* pInstance = new CUI_PopupQuest(pDevice, pContext, eType, eSeparatorType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_PopupQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PopupQuest::Clone(void* pArg)
{
	CUI_PopupQuest* pInstance = new CUI_PopupQuest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_PopupQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PopupQuest::Free()
{
	__super::Free();

//	for (auto& iter : m_Quest)
//		Safe_Release(iter);
//	m_Quest.clear();
//
	Safe_Release(m_pTextureCom);
}
