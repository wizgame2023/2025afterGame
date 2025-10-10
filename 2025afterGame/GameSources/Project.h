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
#include "TitleStage.h"

// 殿堂入りMyGameObject本体
#include "MyGameObject.h";


// 親クラスがMyGameObject//////////////////////////
#include "Actor.h";
#include "AttackCollision.h"
#include "BaseState.h";
#include "MainCameraManager.h"
///////////////////////////////////////////////////

// 親クラスがActor/////////////////////////////////
#include "Player.h";
#include "Bullet.h";
#include "Barrier.h";
///////////////////////////////////////////////////

// ステージ関係は一番下のブロックでインクルードしてください
#include "GameStage.h"
#include "MultiViewStage.h"
///////////////////////////////////////////////////



