#pragma once



enum COLLISION_GROUP { CHARACTER, MONSTER, PROP, NPC, PORTAL, GROUP_END };

enum class TIMER_TYPE
{
	DEFAULT,
	GAME_PLAY,
	UI,
	TIMER_END
};

enum PHYSX_RIGID_TYPE
{
	STATIC,
	DYNAMIC,
	GROUND,
	RIGID_TYPE_END,
};

enum PHYSX_COLLIDER_TYPE
{
	BOX,
	SPHERE,
	MESH,
	COLLIDER_TYPE_END,
};



enum class KEY_STATE
{
	NONE,
	TAP,
	HOLD,
	AWAY
};

enum class KEY
{
	LEFT_ARROW,
	RIGHT_ARROW,
	UP_ARROW,
	DOWN_ARROW,
	TAB,
	OPEN_SQUARE_BRACKET,
	CLOSE_SQUARE_BRACKET,
	F1, F2, F3, F4, F5, F6, F7, F8, F9,
	INSERT, DEL, HOME, END_KEY, PAGE_UP, PAGE_DOWN,
	NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	ALT, CTRL, SHIFT,
	SPACE, ENTER, ESC,
	LBTN, RBTN,
	TYPE_END
};

enum DIMK { DIMK_LBUTTON, DIMK_RBUTTON, DIMK_WHEEL, DIMK_X, DIMK_END };
enum DIMM { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };

namespace Engine
{

#pragma region Channel ID 

	enum CHANNELID /* enum 수정시 아래 문자열도 순서에 맞게 교체 필요 */
	{
		SOUND_UI,
		SOUND_VOICE_CHARACTER,
		SOUND_VOICE_MONSTER1,
		SOUND_VOICE_MONSTER2,
		SOUND_VOICE_MONSTER3,
		SOUND_SKILL,
		SOUND_SLASH,
		SOUND_SLASH_HIT,
		SOUND_NORMAL_HIT,
		SOUND_FOOT_CHARACTER,
		SOUND_FOOT_MONSTER,
		SOUND_BGM,
		SOUND_SHAKE,
		SOUND_JUMP,
		SOUND_BOSS,
		MAXCHANNEL
	};

	static const char* szChannelIDNames[CHANNELID::MAXCHANNEL]
	{
		"SOUND_UI",
		"SOUND_VOICE_CHARACTER",
		"SOUND_VOICE_MONSTER1",
		"SOUND_VOICE_MONSTER2",
		"SOUND_VOICE_MONSTER3",
		"SOUND_SKILL",
		"SOUND_SLASH",
		"SOUND_SLASH_HIT",
		"SOUND_NORMAL_HIT",
		"SOUND_FOOT_CHARACTER",
		"SOUND_FOOT_MONSTER",
		"SOUND_BGM",
		"SOUND_SHAKE",
		"SOUND_JUMP",
		"SOUND_BOSS"
	};

#pragma endregion


#pragma region Animation Event
	enum ANIM_EVENT_TYPE			{ SOUND, EFFECT, CAMERA, COLLIDER, ANIM_EVENT_TYPE_END};

	enum ANIM_EVENT_SOUND_TYPE		{ PLAY_SOUND, ANIM_EVENT_SOUND_TYPE_END };
	enum ANIM_EVENT_EFFECT_TYPE		{ GENERATE, ANIM_EVENT_EFFECT_TYPE_END };
	enum ANIM_EVENT_CAMERA_TYPE		{ ACTION, CHANGE, ANIM_EVENT_CAMERA_TYPE_END };
	enum ANIM_EVENT_COLLIDER_TYPE	{ ON, OFF, ANIM_EVENT_COLLIDER_TYPE_END };

	static const wstring strAnimEventTypeNames[ANIM_EVENT_TYPE::ANIM_EVENT_TYPE_END]{ L"Sound", L"Effect", L"Camera", L"Collider" };
	static const char* szAnimEventTypeNames[ANIM_EVENT_TYPE::ANIM_EVENT_TYPE_END]{ "Sound", "Effect", "Camera", "Collider" };

	static const wstring strAnimEventSoundTypeNames[ANIM_EVENT_SOUND_TYPE::ANIM_EVENT_SOUND_TYPE_END]{ L"Play Sound" };
	static const char* szAnimEventSoundTypeNames[ANIM_EVENT_SOUND_TYPE::ANIM_EVENT_SOUND_TYPE_END]{ "Play Sound" };

	static const wstring strAnimEventEffectTypeNames[ANIM_EVENT_EFFECT_TYPE::ANIM_EVENT_EFFECT_TYPE_END]{ L"Generate" };
	static const char* szAnimEventEffectTypeNames[ANIM_EVENT_EFFECT_TYPE::ANIM_EVENT_EFFECT_TYPE_END]{ "Generate" };

	static const wstring strAnimEventCameraTypeNames[ANIM_EVENT_CAMERA_TYPE::ANIM_EVENT_CAMERA_TYPE_END]{ L"Action", L"Change" };
	static const char* szAnimEventCameraTypeNames[ANIM_EVENT_CAMERA_TYPE::ANIM_EVENT_CAMERA_TYPE_END]{ "Action", "Change" };

	static const wstring strAnimEventColliderTypeNames[ANIM_EVENT_COLLIDER_TYPE::ANIM_EVENT_COLLIDER_TYPE_END]{ L"On", L"Off" };
	static const char* szAnimEventColliderTypeNames[ANIM_EVENT_COLLIDER_TYPE::ANIM_EVENT_COLLIDER_TYPE_END]{ "On", "Off" };
#pragma endregion

/* 아이템 코드 */
	enum ITEM_CODE
	{
#pragma region Sword
		SWORD_CHRISTMAS,
		SWORD_CROSSSERVER,
		SWORD_DARKNESS,
		SWORD_DARKNESS02,
		SWORD_FIRE,
		SWORD_FIRE02,
		SWORD_FISH,
		SWORD_FLOWER,
		SWORD_FOOD,
		SWORD_LIGHT,
		SWORD_LIGHT02,
		SWORD_QQCYBERPUBKS,
		SWORD_QQSUITS,
		SWORD_RAIDCYBORGX,
		SWORD_WATER,
		SWORD_WATER02,
		SWORD_WOOD,
		SWORD_WOOD02,
#pragma endregion

#pragma region Hammer
		HAMMER_CHRISTMAS,
		HAMMER_CROSSSERVEVR,
		HAMMER_DARKNESS,
		HAMMER_DARKNESS02,
		HAMMER_FIRE,
		HAMMER_FIRE02,
		HAMMER_FISH,
		HAMMER_FLOWER,
		HAMMER_FOOD,
		HAMMER_FOOL02,
		HAMMER_LIGHT,
		HAMMER_LIGHT02,
		HAMMER_QQSUITS,
		HAMMER_RAIDCYBORGX,
		HAMMER_WATER,
		HAMMER_WATER02,
		HAMMER_WOOD,
		HAMMER_WOOD02,
#pragma endregion

#pragma region Gauntlet
		GAUNTLET_CHRISTMAS,
		GAUNTLET_CROSSSEVER,
		GAUNTLET_FISH,
		GAUNTLET_FLOWER,
		GAUNTLET_FOOD,
		GAUNTLET_QQCYBERPUNKS,
		GAUNTLET_QQSUITS,
		GAUNTLET_RAIDCYBORGX,
		GAUNTLET_T04,
		GAUNTLET_T04RARE,
#pragma endregion

#pragma region Rifle

#pragma endregion
		CODE_END
	};

}