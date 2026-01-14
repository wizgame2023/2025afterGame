/*!
@file Project.h
@brief コンテンツ用のヘッダをまとめる
*/

/*
   インクルードルール
   親クラスがMyGameObjectから近いクラス順に上の行に書いてください
   ○〇クラスが親クラスというのをコメントでブロックを作るのでそれを見て追加お願いします
*/

#pragma once

#include "ProjectShader.h"
#include "ProjectBehavior.h"
#include "Scene.h"
#include "Character.h"
#include "InputManager.h"
#include "Background.h"
#include "Plane.h"
#include "UIManager.h"
#include "StageWall.h"
#include "TestCsv.h"
#include "DashRing.h"
#include "ScoreObjectManager.h"
#include "StageCreateManager.h"

// 殿堂入りMyGameObject本体
#include "MyGameObject.h";

// 親クラスがMyGameObject//////////////////////////
#include "Actor.h";
#include "AttackCollision.h"
#include "MainCameraManager.h"
#include "GameManager.h"
#include "Sprite.h"
#include "BillBoard.h"
#include "ScoreManager.h"
///////////////////////////////////////////////////

// EffectManager
#include "EffectManager.h"
#include "EffectUpdateDrawManager.h"

// 親クラスがActor/////////////////////////////////
#include "FighterAircraftBase.h"
#include "Bullet.h"
#include "Barrier.h"
#include "DisableBarrier.h"
#include "CheckPoint.h"
#include "ObstaclesDodge.h"
#include "InvisibleCollision.h"
#include "ScoreObject.h"
///////////////////////////////////////////////////

//親クラスがFighterAircraftBase////////////////////
#include "Player.h"
#include "DebagPlayer.h"
#include "Enemy.h"
///////////////////////////////////////////////////

// 親クラスがSprite////////////////////////////////
#include "HpSprite.h"
#include "NumberSprite.h"
#include "PauseMenu.h"
#include "RankingUI.h"
///////////////////////////////////////////////////

// 親クラスがBillBoard////////////////////////////////
#include "BillBoardGauge.h"
///////////////////////////////////////////////////

// ステージ関係は一番下のブロックでインクルードしてください
#include "GameStage.h"
#include "MultiViewStage.h"
#include "ErionStage.h"
#include "KaitoStage.h"
#include "TomokiStage.h"
#include "YuutaStage.h"
#include "TitleStage.h"
#include "SelectStage.h"
///////////////////////////////////////////////////

// ステートインクルード
#include "StateBase.h";

// 親クラスがStateBase
#include "StateBarrier.h"
#include "StateDisableBarrier.h"
#include "StateEnemy.h"