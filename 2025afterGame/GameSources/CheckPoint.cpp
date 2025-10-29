/*!
@file CheckPoint.cpp
@brief �`�F�b�N�|�C���g�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	CheckPoint::CheckPoint(const shared_ptr<Stage>& stageptr) :
		Actor(stageptr,Vec3(-100.0f, -100.0f, -100.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)),
		m_raceTime(0.0f),
		m_previewTime(0.0f),
		m_checkPointID(0)
	{

	}

	CheckPoint::~CheckPoint()
	{

	}

	void CheckPoint::OnCreate()
	{
		//初期位置設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_pos.x += rand() % 200;
		m_pos.y += rand() % 200;
		m_pos.z += rand() % 200;
		m_trans->SetQuaternion(m_qt);
		m_trans->SetScale(m_scale);

		//コリジョンの設定
		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);
		ptrCol->SetAfterCollision(AfterCollision::None);

		//メッシュの描画
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void CheckPoint::OnUpdate()
	{
		//ゲームマネージャーを取得
		auto& game = GameManager::GetGameManager();
		//レース時間の取得
		m_raceTime = game->GetTimeGamePlaying();
		//位置を指定
		m_trans->SetPosition(Vec3(m_pos.x, m_pos.y, m_pos.z));
	}

	//プレイヤ―の接触で実行
	void CheckPoint::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto playermachine = dynamic_pointer_cast<Player>(obj);
		//プレイヤーが接触した時のみ実行
		if (playermachine)
		{
			//初めてこのチェックポイントに触れたか判定
			if (m_previewTime > 0.0f)
			{
				//2機目からは先に通った機体との差分の時間を返す
				playermachine->SetTimeCheckPointDifferece(GetDifferenceTime());
			}
			else
			{
				//1機目の場合基準タイムを設定する
				playermachine->SetTimeCheckPointDifferece(0);
				m_previewTime = m_raceTime;
			}
			//次のチェックポイントをプレイヤーに返す
			playermachine->SetNextCheckPointPos(GetNextCheckPoint());
		}
	}

	//差分の時間をプレイヤーに返すゲッター
	float CheckPoint::GetDifferenceTime()
	{
		auto& game = GameManager::GetGameManager();
		float differencetime = m_previewTime - game->GetTimeGamePlaying();
		m_previewTime = game->GetTimeGamePlaying();
		return differencetime;
	}

	//チェックポイントのIDを設定
	void CheckPoint::SetCheckPointID(int id)
	{
		m_checkPointID = id;
	}

	//次のチェックポイントをプレイヤーに返すゲッター
	Vec3 CheckPoint::GetNextCheckPoint() 
	{
		if (m_checkPointID != 3)
		{
			auto& game = GameManager::GetGameManager();
			auto& checkPoint = game->GetCheckPoint(m_checkPointID);
			Vec3 newPoint = checkPoint->GetComponent<Transform>()->GetPosition();
			return newPoint;
		}
	}
}
//end basecross
