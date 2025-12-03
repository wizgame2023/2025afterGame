/*!
@file Enemy.cpp
@brief 敵実体
担当：三瓶裕太
*/

#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

namespace basecross {
	Enemy::Enemy(const shared_ptr<Stage>& obj,const Vec3& pos,const Quat& qt,const Vec3& scale,const shared_ptr<CheckPoint>& startCheckPoint, const shared_ptr<Actor>& trackingObj):
		FighterAircraftBase(obj,pos,qt,scale,startCheckPoint),
		m_trackingObj(trackingObj)
	{

	}

	Enemy::~Enemy()
	{

	}

	void Enemy::OnCreate()
	{
		FighterAircraftBase::OnCreate();
		// Trans処理追加
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetQuaternion(m_qt);
		m_trans->SetScale(Vec3(1.0f));

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.25f, 0.25f, 0.25f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(180.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		// コリジョン追加
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(false);
		ptrCol->SetAfterCollision(AfterCollision::None);

		// ドロー処理
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"Sentouki");
		ptrDraw->SetTextureResource(L"diffuse_TX");
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 1.0f, 1.0f));
		ptrDraw->SetEmissive(Col4(0.0f, 1.0f, 1.0f, 1.0f));
		SetAlphaActive(true);

		// ステートマシン作成
		//m_stateMachine = unique_ptr<StateBarrierMachine>(new StateBarrierMachine(GetThis<MyGameObject>()));
		//m_stateMachine->ChangeState(L"Default");

		// 敵タグ追加
		AddTag(L"Enemy");


		// Qt回転
		//XMVECTOR axis = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		//float testAngle = XMConvertToRadians(90.0f);
		//XMVECTOR quat = XMQuaternionRotationAxis(axis, testAngle);
		
		//m_rot= XMVector3Rotate(m_trans->GetForward(), quat);
		//m_qt = m_qt * Quat(0.0f, (sin(testAngle / 2.0f)), 0.0f, 0.0f);
		//m_trans->SetQuaternion(m_qt);

		// 初期化
		m_hpCurrent = 30;
		m_hpMax = 30;
		m_timeOfReturn = 3.0f;

		// ステートマシン作成
		m_stateMachine = unique_ptr<StateEnemyMachine>(new StateEnemyMachine(GetThis<MyGameObject>()));
		m_stateMachine->ChangeState(L"Base"); // 仮で最初のステートはベースステートに変更する

	}

	void Enemy::OnUpdate()
	{
		FighterAircraftBase::OnUpdate();

		// ステートのUpdate
		m_stateMachine->Update();

		// デバック用に弾を出す
		m_countDebagBulletTime += m_delta;
		if (m_countDebagBulletTime >= 0.5f)
		{
			//GetStage()->AddGameObject<Bullet>(GetThis<Actor>());
			m_countDebagBulletTime = 0.0f;
		}

		// 追いかけるものが消えていたらUpdateしないようにする
		shared_ptr<Actor> lockTrackingObj = m_trackingObj.lock();
		if (!lockTrackingObj)
		{
			return;
		}

		// xz方面の距離の差を求める 次はzy方面の距離の差を求める
		//auto goalObj = GetStage()->GetSharedGameObject<DebagPlayer>(L"Player"); // いったんゴールの位置をプレイヤーにする
		auto goalPos = lockTrackingObj->GetComponent<Transform>()->GetPosition();
		Vec3 posPlayerDifference = goalPos - m_pos; // ゴールと敵の位置の差を求める
		Vec2 differenceYZ = Vec2(posPlayerDifference.y, abs(posPlayerDifference.z));
		differenceYZ.normalize();
		posPlayerDifference.normalize(); // 正規化

		// 目的地の角度を取得
		float goalAngle = atan2f(posPlayerDifference.x, posPlayerDifference.z);
		float goalAngleZY = atan2f(posPlayerDifference.y, abs(posPlayerDifference.z));
		float goalAngleYX = atan2f(posPlayerDifference.y, -posPlayerDifference.x);


		// 角度がマイナスつかないように変更
		goalAngle = AdjustmentAngle(goalAngle);
		goalAngleZY = -AdjustmentAngle(goalAngleZY);
		goalAngleYX = AdjustmentAngle(goalAngleYX);

		// デバック用のロール回転
		static float debugYX = 0.0f;
		auto& input = InputManager::GetInputManager();
		if (input->GetButton(L"DLeft"))
		{
			debugYX -= m_delta * 3.0f;
		}
		if (input->GetButton(L"DRight"))
		{
			debugYX += m_delta * 3.0f;
		}
		goalAngleYX = debugYX;
		//


		// ピッチの向きたい方向を求める処理
		// これで、向いている方向のY座標を0にしたものを求める
		auto posPlayerDifferenceZY = posPlayerDifference;
		posPlayerDifferenceZY.y = 0.0f;
		
		
		// 内積
		float dotf = posPlayerDifference.dot(posPlayerDifferenceZY);
		// なす角を求める
		auto pitchAngle = acosf(dotf);

		// 敵から見てプレイヤーが下にいたら角度をマイナスにする
		if (posPlayerDifference.y > 0)
		{
			pitchAngle = -pitchAngle;
		}

		// 敵が追いかける際反転するか決める処理
		auto forward = GetComponent<Transform>()->GetForward();
		// 向いているZX平面の角度を計算
		auto forwardAngle = atan2f(forward.z, forward.x); 
		forwardAngle = AdjustmentAngle(forwardAngle);

		// 敵から見てプレイヤーのいるZX平面の角度
		auto playerAngle = atan2f(posPlayerDifference.z, posPlayerDifference.x);
		playerAngle = AdjustmentAngle(playerAngle);

		// 進むスピード(仮)
		float speed = 1.0f;

		// Pos移動
		m_pos.x += cos(playerAngle) * speed * m_delta;
		m_pos.z += sin(playerAngle) * speed * m_delta;

		// y方向の差が＋かーか確認する
		int ysign = 0;
		if (posPlayerDifference.y > 0.05f)
		{
			ysign = 1;
		}
		else if(posPlayerDifference.y < -0.05f)
		{
			ysign = -1;
		}
		else
		{
			ysign = 0;
		}
		m_pos.y += ysign * speed * m_delta; // 向いている角度によってスピード変えないと違和感が出るかも

		// Qt回転
		// 個別の軸ずつ回転計算をしています
		m_qt = Quat(0.0f, 0.0f, (sin(goalAngleYX / 2.0f)), cos((goalAngleYX / 2.0f))); // Z軸回転
		m_qt *= Quat((sin(pitchAngle / 2.0f)), 0.0f, 0.0f, cos((pitchAngle / 2.0f))); // X軸回転
		m_qt *= Quat(0.0f, (sin(goalAngle / 2.0f)), 0.0f, cos((goalAngle / 2.0f))); // Y軸回転

		//auto rot = XMMatrixRotationAxis(axisYX, goalAngleYX);
		//auto world = m_trans->GetWorldMatrix();
		//world.rotation((Quat)XMQuaternionRotationMatrix(rot));
		//m_qt = world.quatInMatrix();

		// Transform反映
		m_trans->SetQuaternion(m_qt); // qt反映
		//m_trans->SetRotation(m_rot);
		m_trans->SetPosition(m_pos); // pos反映


		


		//////デバック用
		//wstringstream wss(L"");
		//auto scene = App::GetApp()->GetScene<Scene>();

		//wss /* << L"デバッグ用文字列 "*/
		//	<< L"\ngoalAngle : " << XMConvertToDegrees(goalAngle)
		//	<< L"\ngoalAngleYX : " << XMConvertToDegrees(pitchAngle)
		//	<< endl;

		//scene->SetDebugString(wss.str());

	}

	// 当たり判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto bullet = dynamic_pointer_cast<Bullet>(obj);

		// 弾に当たった場合
		if (bullet)
		{
			bool bulletAffiliation = bullet->GetAffiliation();

			// 弾の所属がプレイヤーならダメージを受ける
			if (bulletAffiliation == true)
			{
				m_hpCurrent -= bullet->GetDamage();
			}

			// HPが０になったらリスポーンする
			if (m_hpCurrent < 0)
			{
				// リスポーンステートに遷移する
				ChangeState(L"Respawn");
			}
		}
	}


	//角度の調整0~360度までしか出ないようにする
	float Enemy::AdjustmentAngle(float angle)
	{
		if (angle >= XMConvertToRadians(360.0f))
		{
			angle -= XMConvertToRadians(360.0f);
		}
		else if (angle < XMConvertToRadians(0.0f))
		{
			angle += XMConvertToRadians(360.0f);
		}

		return angle;
	}


	// ステートの変更処理
	void Enemy::ChangeState(wstring stateName)
	{
		m_stateMachine->ChangeState(stateName);
	}


}
//end basecross