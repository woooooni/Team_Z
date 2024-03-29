#pragma once
#include "Tool.h"
#include "GameNpc.h"
#include "Monster.h"

BEGIN(Engine)
class CGameObject;
class CLight;
END

BEGIN(Client)

struct LightHelper
{
	Vec3 vRotationDeg;
	Vec3 vSpotAngles = Vec3(1.f, 1.f, 1.f);
};

class CTool_Map final : public CTool
{
public:
	enum LEVEL_LIST { LOBBY, EVERMORE, HALL, WINTER, WITCH, LEVEL_LIST_END };
	enum BRUSH_STATE { NONE_BRUSH, HEGIHT_BRUSH, TEXTURE_BRUSH, BRUSH_END };
	enum USE_SPACE { MAP_SPACE, LIGHT_SPACE, MOBSTER_SPACE, SPACE_END };

private:
	CTool_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Map() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void AddMapObject(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void DeleteObject(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void BatchObject(LEVELID iLevelID, LAYER_TYPE iLayerType);

	void AddMapMonster(LEVELID iLevelID, LAYER_TYPE iLayerType);

	void AddMapNPC(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void BatchNPC(LEVELID iLevelID, LAYER_TYPE iLayerType);

	void AddTrigger(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void BatchTrigger(LEVELID iLevelID, LAYER_TYPE iLayerType);

	void BatchDynamic(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void DeleteDynamic(LEVELID iLevelID, LAYER_TYPE iLayerType);

	void BatchWater(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void BatchAnimal(LEVELID iLevelID, LAYER_TYPE iLayerType);

	void DeleteLight(_uint iLightID);

	void DeleteTrigger(LEVELID iLevelID, LAYER_TYPE ILayerType);

	// NPC Roming Clear
	void RomingClear();
	void RomingPointDelete();


	// Animals
	void Animals_PointsClear();

	void Picking();
private:
	void MapObjectSpace();
	void MapLightSpace();
	void MapMonsterSpace();
	void MapNPCSpace();
	void MapWaterSpace();
	void MapAnimalPatrol();
	void Map_Fog(_float fTimeDelta);
	void Map_TriggerBox();

private:
	void ChangeState();

private:
	HRESULT Save_Map_Data(const wstring& strMapFileName);
	HRESULT Load_Map_Data(const wstring& strMapFileName);

	HRESULT Save_Dynamic_Data(const wstring& strMapFileName);
	HRESULT Load_Dynamic_Data(const wstring& strMapFileName);

	HRESULT Save_Light_Data(const wstring& strLightFilePath);
	HRESULT Load_Light_Data(const wstring& strLightFilePath);

	HRESULT Save_Monster_Data(const wstring& strMonsterFileName);
	HRESULT Load_Monster_Data(const wstring& strMonsterFileName);

	HRESULT Save_NPC_Data(const wstring& strNPCFileName);
	HRESULT Load_NPC_Data(const wstring& strNPCFileName);

	HRESULT Save_Trigger_Data(const wstring& strTriggerName);
	HRESULT Load_Trigger_Data(const wstring& strTriggerName);

private:
	class CRenderer* m_pRendererCom = nullptr;

private:
	// 오브젝트 상태제어 
	_bool m_bAddObject = false;
	_bool m_bAddMonster = false;
	_bool m_bAddNPC = false;
	_bool m_bPlantMode = false;
	_bool m_bAddDynamic = false;
	_bool m_bAddTrigger = false;

	_bool m_bAddLight = false;
	LightHelper m_LightHelper = {};

	// 내 컨트롤 제어상태
	_int m_iControlState = 0;
	_int m_iLightControlState = 0;
	_int m_iDynamicState = 0;
	_int m_iMonsterState = 0;
	_int m_iNPCState = 0;
	_int m_iTriggerState = 0;
	_bool m_bAnimalPointPick = false;

	wstring m_strLevelName = L"Evermore";
	wstring m_strLevelMonsterName = L"Winter";
	wstring m_strLevelNPCName = L"Evermore";
	wstring m_strLevelTrigger = L"Evermore";
	wstring m_strPrevLevelName = L"";
	_uint m_iCurrentLevel = 0;
	_uint m_iMonsterLevel = 0;
	_uint m_iTriggerLevel = 0;

	const char* m_ImguiSelectableNameList[LEVEL_LIST_END] =
	{ "Lobby", "Evermore", "Kingdom", "Winter", "Witch" };

	const char* m_ImguiSelectableNPCNameList[LEVEL_LIST_END] =
	{ "Lobby", "Evermore", "Kingdom", "Winter", "Witch" };

	const char* m_ImguiSelectableTrrigerNameList[LEVEL_LIST_END] =
	{ "Lobby", "Evermore", "Kingdom", "Winter", "Witch" };

	const char* m_ImguiLevelName[3] = { "Evermore", "Winter", "Witch" };

	// 선택한 오브젝트
	CGameObject* m_pSelectObj = nullptr;
	class CTrigger* m_pTriggerObj = nullptr;
	CLight* m_pSelectLight = nullptr;

	// IMGUI STATE
	_bool m_bWindowMoveFlag = false;
	_bool m_bWindowResizeFlag = false;



	// Terrain
	Vec2 m_vTerrainCount;
	
	_int m_iBrushType = 0;

	_float m_fBrushStrenght = 20.f;
	_float m_fBrushSize = 30.f;

	USE_SPACE m_eUseSpace = SPACE_END;

	// OBJECT_INIT_DATA
	const char* m_NpcStateSelectableName[CGameNpc::NPC_STATE::NPC_END] =
	{
		"IDLE", "TALK", "MOVE_ONEWAY", "MOVE_TWOWAY", "NPC_UNIQUENPC_WALK", 
		"NPC_UNIQUENPC_RUN", "NPC_UNIQUENPC_TALK", "BACKDOWN", 
		"FRONTDOWN", "ATTACKIDLE", "ATTACK", "NPC_UNIQUENPC_SEAT"
	};
	const char* m_InvasionMonsterStateSelectableName[CMonster::MONSTER_INVASION_STATE::INVASION_STATE_END] =
	{
		"ATTACK", "IDLE01", "IDLE02", "DEAD"
	};
	const char* m_TriggerSelectName[TRIGGER_TYPE::TRIGGER_END] = 
	{
		"CHANGE_BGM", "MAP_NAME", "GIANTY_ENTER", "STELLIA_ENTER", "WHALE_ENTER", "WOOD_ENTER", "ESCORT1", "ESCORT2"
	};

#pragma region Debug
#ifdef _DEBUG

	BasicEffect* m_pEffect = nullptr;
	BoundingSphere* m_pSphere = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;

	HRESULT Ready_DebugDraw();
	HRESULT Render_DebugDraw();
#endif // _DEBUG
#pragma endregion Debug

public:
	static CTool_Map* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
