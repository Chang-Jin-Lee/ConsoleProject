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
		int m_ipcontentSize = -1;
		int m_ippcontentSize = -1;
		FCOORDSNode* m_pDrawCOORDS = NULL;

		FUI()
		{
			m_fAxis.X = 0;
			m_fAxis.Y = 0;
			m_pcontent = NULL;
			m_ppcontent = NULL;
			m_pDrawCOORDS = NULL;
		}

		FUI(int x, int y)
		{
			m_pcontent = NULL;
			m_ppcontent = NULL;
			m_fAxis.X = x;
			m_fAxis.Y = y;
		}

		FUI(int x, int y, char* content)
		{
			m_fAxis.X = x;
			m_fAxis.Y = y;
			m_pcontent = content;
			m_ppcontent = NULL;
		}
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
	};

	void Release(FUI* ui);
	void Release(FVideo* video);

	FCOORDSNode* AddCOORDNode(FCOORDSNode* Root, COORD data);
	void DeleteCOORDNode(FCOORDSNode* curNode, COORD data);
	void TravelNodeCOORDNode(FCOORDSNode* Root);
}