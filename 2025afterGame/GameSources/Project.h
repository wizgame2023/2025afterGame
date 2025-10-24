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
#include "StateBase.h"
#include "StateEnemy.h"

// 殿堂入りMyGameObject本体
#include "MyGameObject.h";


// 親クラスがMyGameObject//////////////////////////
#include "Actor.h";
#include "AttackCollision.h"
#include "BaseState.h";
#include "MainCameraManager.h"
#include "GameManager.h"
///////////////////////////////////////////////////

// 親クラスがActor/////////////////////////////////
#include "FighterAircraftBase.h"
#include "Player.h";
#include "Bullet.h";
#include "Barrier.h";
#include "DisableShield.h"
#include "CheckPoint.h"
#include "StageWall.h"
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



#include "StateBarrier.h"