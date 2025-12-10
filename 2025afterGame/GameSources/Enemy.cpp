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
		m_stateMachine->ChangeState(L"Tracking"); // 仮で最初のステートはベースステートに変更する

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
		m_trakingObjLock = m_trackingObj.lock();
		if (!m_trakingObjLock)
		{
			return;
		}

		//// 自分と追尾対象の座標の差を計算する
		//auto goalPos = m_trakingObjLock->GetComponent<Transform>()->GetPosition();
		//Vec3 posPlayerDifference = goalPos - m_pos; // ゴールと敵の位置の差を求める
		//posPlayerDifference.normalize(); // 正規化

		//// 目的地の角度を取得
		//m_yawAngle = atan2f(posPlayerDifference.x, posPlayerDifference.z);
		//m_rollAngle = atan2f(posPlayerDifference.y, -posPlayerDifference.x);


		//// 角度がマイナスつかないように変更
		//m_yawAngle = AdjustmentAngle(m_yawAngle);
		//m_rollAngle = AdjustmentAngle(m_rollAngle);

		//// 追いかけるときのロール回転処理(デバック用処理しか書いていない)
		//TrackingRollQt();
		//// 追いかける対象にX軸に向く処理
		//TrackingPitchQt(posPlayerDifference);
		//// 追いかける対象に向かってヨーを回転させる処理
		//TrackingYawQt(posPlayerDifference);

		// この処理はいったん保留
		//// 敵が追いかける際反転するか決める処理
		//auto forward = GetComponent<Transform>()->GetForward();
		//// 向いているZX平面の角度を計算
		//auto forwardAngle = atan2f(forward.z, forward.x); 
		//forwardAngle = AdjustmentAngle(forwardAngle);

		// 対象に向かって追いかける処理
		//TrackingMove(posPlayerDifference);

		// Qt回転
		// 個別の軸ずつ回転計算をしています
		m_qt = Quat(0.0f, 0.0f, (sin(m_rollAngle / 2.0f)), cos((m_rollAngle / 2.0f))); // Z軸回転
		m_qt *= Quat((sin(m_pitchAngle / 2.0f)), 0.0f, 0.0f, cos((m_pitchAngle / 2.0f))); // X軸回転
		m_qt *= Quat(0.0f, (sin(m_yawAngle / 2.0f)), 0.0f, cos((m_yawAngle / 2.0f))); // Y軸回転


		// Transform反映
		m_trans->SetQuaternion(m_qt); // qt反映
		m_trans->SetPosition(m_pos); // pos反映

		//////デバック用
		//wstringstream wss(L"");
		//auto scene = App::GetApp()->GetScene<Scene>();

		//wss /* << L"デバッグ用文字列 "*/
		//	<< L"\ngoalAngle : " << XMConvertToDegrees(m_yawAngle)
		//	<< L"\ngoalAngleYX : " << XMConvertToDegrees(m_pitchAngle)
		//	<< endl;

		//scene->SetDebugString(wss.str());

	}

	// 当たり判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto bullet = dynamic_pointer_cast<Bullet>(obj);

		// 弾に当たった場合

		// HPがなかったら弾の処理は無視する
		if (m_hpCurrent < 0) return;

		if (bullet)
		{
			bool bulletAffiliation = bullet->GetAffiliation();

			// 弾の所属がプレイヤーならダメージを受ける
			if (bulletAffiliation == true)
			{
				m_hpCurrent -= bullet->GetDamage();
			}

			// HPが０になったらリスポーンする
			if (m_hpCurrent <= 0)
			{
				// リスポーンステートに遷移する
				ChangeState(L"Respawn");
			}

			GetStage()->RemoveGameObject<Bullet>(bullet);
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

	// 対象に向かって追いかける処理
	void Enemy::TrackingMove(const Vec3& posPlayerDifference)
	{
		// 敵から見てプレイヤーのいるZX平面の角度
		auto playerAngle = atan2f(posPlayerDifference.z, posPlayerDifference.x);
		playerAngle = AdjustmentAngle(playerAngle);

		// Pos移動
		m_pos.x += cos(playerAngle) * m_speed * m_delta;
		m_pos.z += sin(playerAngle) * m_speed * m_delta;

		// y方向の差が＋かーか確認する
		int ysign = 0;
		if (posPlayerDifference.y > 0.05f)
		{
			ysign = 1;
		}
		else if (posPlayerDifference.y < -0.05f)
		{
			ysign = -1;
		}
		else
		{
			ysign = 0;
		}
		m_pos.y += ysign * m_speed * m_delta; // 向いている角度によってスピード変えないと違和感が出るかも

		return;
	}

	// 追いかける対象に向かってX軸回転方向で向く処理
	void Enemy::TrackingPitchQt(const Vec3& posPlayerDifference)
	{
		// ピッチの向きたい方向を求める処理
		// これで、向いている方向のY座標を0にしたものを求める
		auto posPlayerDifferenceZY = posPlayerDifference;
		posPlayerDifferenceZY.y = 0.0f;

		// 内積
		float dotf = posPlayerDifference.dot(posPlayerDifferenceZY);
		// なす角を求める
		m_pitchAngle = acosf(dotf);

		// 敵から見てプレイヤーが下にいたら角度をマイナスにする
		if (posPlayerDifference.y > 0)
		{
			m_pitchAngle = -m_pitchAngle;
		}
	}

	// 追いかける対象に向かってZ軸回転方向で向く処理
	void Enemy::TrackingRollQt()
	{
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
		m_rollAngle = debugYX;
		//

		return;
	}

	// 追いかける対象に向かってY軸回転方向で向く処理
	void Enemy::TrackingYawQt(const Vec3& posPlayerDifference)
	{
		m_yawAngle = atan2f(posPlayerDifference.x, posPlayerDifference.z);
		return;
	}

	// 追いかける対象ポインタのゲッタ
	shared_ptr<Actor> Enemy::GetTrackingObj()
	{
		auto trackingObjLock = m_trackingObj.lock();
		if (!trackingObjLock)
		{
			throw BaseException
			{
				L"追いかける対象が存在しません",
				L"if (!trackingObjLock)",
				L"shared_ptr<Actor> Enemy::GetTrackingObj()"
			};

			return nullptr;
		}

		return trackingObjLock;
	}


}
//end basecross