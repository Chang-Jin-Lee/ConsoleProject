#pragma once

#include "Windows.h"
#include "BaseDefine.h"

namespace UI
{
	typedef struct FCOORDSNode
	{
		COORD data;
		struct FCOORDSNode* next;
	} FCOORDSNode;

	typedef struct FUI	// 이중포인터쓰는 UI ex) 파일에서 읽은 스트링, 대화창 틀 등
	{
		COORD m_fAxis;
		char* m_pcontent = NULL;
		char** m_ppcontent = NULL;
		CHAR_INFO* m_ppInfoContent = NULL;
		size_t m_ipcontentSize = -1;
		size_t m_ippcontentSize = -1;
		int m_iUIColor = 0;
		FCOORDSNode* m_pDrawCOORDS = NULL;

		FUI()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_pcontent = NULL;
			m_ppcontent = NULL;
			m_ipcontentSize = -1;
			m_ippcontentSize = -1;
			m_pDrawCOORDS = NULL;
			m_iUIColor = 0;
		}

		FUI(int x, int y)
		{
			m_pcontent = NULL;
			m_ppcontent = NULL;
			m_ipcontentSize = -1;
			m_ippcontentSize = -1;
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_iUIColor = 0;
		}

		FUI(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_ipcontentSize = -1;
			m_ippcontentSize = -1;
			m_pcontent = content;
			m_ppcontent = NULL;
			m_iUIColor = 0;
		}
	} FUI;

	typedef struct FBUBBLEUI	// 이중포인터쓰는 UI ex) 파일에서 읽은 스트링, 대화창 틀 등
	{
		FUI m_fbackGround;
		FUI m_fcontent;
		bool bVisible = false;

		FBUBBLEUI() {}
	} FBUBBLEUI;

	struct FGAMEDIALOGANIMATIONSCENE	// text_0001_rapi_30_.txt
	{
		int Number = 0;
		UI::FUI m_fspeaker;
		UI::FUI Type;
		UI::FUI m_fDialogue;
		bool m_aSpeakerTalkable[MAX_CHARACTER_SIZE] = { false, };	// Choice Size
		char* m_sSelectDialogue[MAX_SELECTBUBBLE_SIZE];
		int NextIdx;
		int SelectNextDialogue[MAX_SELECTBUBBLE_SIZE];
		SHORT m_iTalkingCharacterSize = 1;		// 화면에 나올 캐릭터들
		SHORT m_iChoiceSize = 1;				// 선택지 개수
		UI::FBUBBLEUI m_fSelectBubble[MAX_SELECTBUBBLE_SIZE];
		int m_iselectIndex = 0;
		//FUI SceneName;
	};

	struct FGAMEDIALOG
	{
		int InSize = 0;
		FUI Speaker;
		FUI Dialog;
		FUI Type;
		FUI Answer;
		FUI Likeability;
		FUI SceneName;
	};

	typedef struct PlayScenePlayerObj
	{
		FUI Player1Idle;
		FUI Player1Blink;
		FUI Player2Idle;
		FUI Player2Blink;
		FUI Player3Idle;
		FUI Player3Blink;
	} PlayScenePlayerObj;

	typedef struct PlaySceneObj
	{
		FUI CatIdle;
		FUI CatCurios;
		FUI CatEyeOpen;
		FUI CatEyeOpen_TailDown;
		FUI CatFace;
		FUI CatJump;
		FUI CatOnRoof;
		FUI CatScared;
		FUI CatWalking;
		FUI CatWeird;

		FUI BirdAttack;
		FUI BirdIdle;
	} PlaySceneObj;


	typedef struct FVideo	// 이중포인터쓰는 UI ex) 파일에서 읽은 스트링, 대화창 틀 등
	{
		COORD m_fAxis;
		FUI m_fui[MAX_VIDEO_SIZE];
		bool m_bPlayable;
		int m_iPlaybackSeconds;
		int m_iMaxLength;

		FVideo()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_bPlayable = false;
			m_iPlaybackSeconds = 0;
			m_iMaxLength = 0;
		}

		FVideo(int x, int y)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_bPlayable = false;
			m_iPlaybackSeconds = 0;
			m_iMaxLength = 0;
		}

		FVideo(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_bPlayable = false;
			m_iPlaybackSeconds = 0;
			m_iMaxLength = 0;
		}
	} FVideo;

	void Release(FUI* ui);
	void Release(FVideo* video);

	void CreateBubbleUI(FUI* ui, int width, int height, int xAxis, int yAxis, float thickNess, int color);
	void CreateBubbleUIContent(UI::FBUBBLEUI* ui, const char* filename, int color);
	void CreateBulletUI(UI::FUI* ui, int width, int height, int color);
	void CreateStoneUI(UI::FUI* ui, int width, int height, int color);

	FCOORDSNode* AddCOORDNode(FCOORDSNode* Root, COORD data);
	void DeleteCOORDNode(FCOORDSNode* curNode, COORD data);
	void TravelNodeCOORDNode(FCOORDSNode* Root);
}