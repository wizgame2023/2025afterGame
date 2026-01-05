/*!
@file Enemy.cpp
@brief 敵実体
担当：三瓶裕太
*/

#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

namespace basecross {
	Enemy::Enemy(const shared_ptr<Stage>& obj, const Vec3& pos, const Vec3& rot, const Vec3& scale, const shared_ptr<CheckPoint>& startCheckPoint, const shared_ptr<Actor>& trackingObj) :
		FighterAircraftBase(obj, pos, rot, scale, startCheckPoint, Col4(0.0f, 1.0f, 1.0f, 1.0f)),
		m_trackingObj(trackingObj)
	{

	}

	Enemy::Enemy(const shared_ptr<Stage>& obj, const Vec3& pos, const Quat& qt, const Vec3& scale, const shared_ptr<CheckPoint>& startCheckPoint, const shared_ptr<Actor>& trackingObj) :
		FighterAircraftBase(obj, pos, qt, scale, startCheckPoint, Col4(0.0f, 1.0f, 1.0f, 1.0f)),
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
		//m_trans->SetQuaternion(m_qt);
		m_trans->SetRotation(m_rot);
		m_trans->SetScale(Vec3(1.0f));

		//// 回転度取得
		//m_rot = m_trans->GetRotation();
		m_rot = Vec3(AdjustmentAngle(m_rot.x), AdjustmentAngle(m_rot.y), AdjustmentAngle(m_rot.z));

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
		//ptrCol->SetAfterCollision(AfterCollision::None);

		// ドロー処理
		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"Sentouki");
		m_draw->SetTextureResource(L"diffuse_TX");
		m_draw->SetMeshToTransformMatrix(spanMat);
		m_draw->SetDiffuse(m_color);
		m_draw->SetEmissive(m_color);
		SetAlphaActive(true);

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
		m_scoreCurrent = 10;

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

		// ヨーピッチロールに沿って回転する処理
		MoveRotate();

		// 無敵時用の処理
		Invincible();

		// Transform反映
		//m_trans->SetQuaternion(m_qt); // qt反映
		m_trans->SetRotation(m_rot);
		m_trans->SetPosition(m_pos + m_moveVec); // pos反映

		// 位置取得
		m_pos = GetComponent<Transform>()->GetPosition();

		// カラー適応
		m_draw->SetEmissive(m_color);
		m_draw->SetDiffuse(m_color);


		////デバック用
		//wstringstream wss(L"");
		//auto scene = App::GetApp()->GetScene<Scene>();

		//wss /* << L"デバッグ用文字列 "*/
		//	<< L"\nm_pitchAngle : " << m_pitchAngle
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
			GetStage()->RemoveGameObject<Bullet>(bullet);

			// 無敵フラグがオンならダメージ関係の処理はしない
			if (m_invincibleFlag) return;

			// 弾の所属がプレイヤーならダメージを受ける
			if (bulletAffiliation == true)
			{
				m_hpCurrent -= bullet->GetDamage();
			}

			// HPが０になったらリスポーンする
			if (m_hpCurrent <= 0)
			{
				// スコアを10%倒した敵に譲渡する
				DownTransferScore(bullet, 0.1f);

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

	// 角度の差が大きいときに別方向に進んだ角度の差を求める処理
	float Enemy::CorrectRotationDirection(float differenceAngle)
	{
		if (differenceAngle >= XMConvertToRadians(181.0f))
		{
			m_goalRotVec.y -= XMConvertToRadians(360.0f);
			differenceAngle = m_goalRotVec.y - m_rot.y;
		}
		if (differenceAngle <= XMConvertToRadians(-181.0f))
		{
			m_goalRotVec.y += XMConvertToRadians(360.0f);
			differenceAngle = m_goalRotVec.y - m_rot.y;
		}

		return differenceAngle;
	}


	// ステートの変更処理
	void Enemy::ChangeState(wstring stateName)
	{
		m_stateMachine->ChangeState(stateName);
	}

	// 対象に向かって追いかける処理
	void Enemy::TrackingMove(const Vec3& posPlayerDifference)
	{
		// 移動ベクトル加算
		auto forward = m_trans->GetForward();
		m_moveVec = forward * m_delta;

		return;
	}

	// 追いかける対象に向かってX軸回転方向で向く処理
	void Enemy::TrackingPitchQt(const Vec3& posPlayerDifference)
	{
		// ピッチの向きたい方向を求める処理
		// これで、向いている方向のY座標を0にしたものを求める
		auto posPlayerDifferenceZY = posPlayerDifference;
		posPlayerDifferenceZY.y = 0.0f;

		// ゼロベクトル対策
		if (posPlayerDifference == Vec3(0.0f))
		{
			return;
		}

		// 内積
		float dotf = posPlayerDifference.dot(posPlayerDifferenceZY);
		dotf = clamp(dotf, -1.0f, 1.0f); // acosfの引数範囲内になるように制限
		// なす角を求める
		m_pitchAngle = acosf(dotf);

		// 敵から見てプレイヤーが下にいたら角度をマイナスにする
		if (posPlayerDifference.y > 0)
		{
			m_pitchAngle = -m_pitchAngle;
		}

		auto a = 0.0f;
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

	// ヨーピッチロールに沿って回転する処理
	void Enemy::MoveRotate()
	{
		// 進みたい方向に回転
		// ピッチヨーロールをrotateに変換
		m_goalRotVec = Vec3(AdjustmentAngle(m_pitchAngle), AdjustmentAngle(m_yawAngle), AdjustmentAngle(m_rollAngle));
		Vec3 differenceRotVec = m_goalRotVec - m_rot;

		// 回転度の差が別の方向に回転したほうが小さいなら逆にする
		differenceRotVec.y = CorrectRotationDirection(differenceRotVec.y);
		differenceRotVec.x = CorrectRotationDirection(differenceRotVec.x);

		Vec3 addRotVec = differenceRotVec;
		addRotVec.normalize();//正規化

		// 少しずつ回転する処理
		if (differenceRotVec.length() > 0.01f)
		{
			m_rot += addRotVec * m_delta;
		}
		else if (differenceRotVec.length() <= 0.01f)
		{
			m_rot = m_goalRotVec;
		}

		// 回転度の整理
		m_rot = Vec3(AdjustmentAngle(m_rot.x), AdjustmentAngle(m_rot.y), m_rot.z);


		////デバック用
		//wstringstream wss(L"");
		//auto scene = App::GetApp()->GetScene<Scene>();

		//wss /* << L"デバッグ用文字列 "*/
		//	//<< L"\ndifferenceRotVec.x : " << differenceRotVec.x
		//	<< L"\n\n\n\nrotVec.x : " << XMConvertToDegrees(m_rot.x)
		//	<< L"\nrotVec.y : " << XMConvertToDegrees(m_rot.y)
		//	<< L"\nrotVec.z : " << XMConvertToDegrees(m_rot.z)
		//	<< endl;

		//scene->SetDebugString(wss.str());
		return;
	}

	// 障害物を避ける処理
	void Enemy::DodgeObstacles(const Vec3& posPlayerDifference)
	{
		auto objVec = GetStage()->GetGameObjectVec();

		Vec3 hitPos;			// 出力用：レイの交差地点(衝突点)
		TRIANGLE triangle;		// レイが交差したポリゴンを構成する頂点の座標
		size_t triangleNumber;	// レイが交差したポリゴンの番号

		for (auto obj : objVec)
		{
			auto obstacles = dynamic_pointer_cast<TestCsv>(obj);// 当たり判定の対象
		
			// 進行上の障害になりそうなものか判断
			if (obstacles)
			{
				auto ptrDraw = obstacles->GetComponent<SmBaseDraw>();
				auto endPoint = m_pos + (posPlayerDifference * 5);
				if (ptrDraw)
				{
					ptrDraw->HitTestStaticMeshSegmentTriangles(m_pos, endPoint, hitPos, triangle, triangleNumber);
				}
			}

			// 現在のステートの文字列を受け取る
			auto currentStateWstring = m_stateMachine->GetCurrentStateWString();

			// どのように回避するかのアンカーを付けたら復活させます
			// レイが当たったら動かないようにする (障害物を避けるステートに移行するための物なので連続して同じステートに変更しないようにしてます)
			//if (hitPos != Vec3(0.0f) && currentStateWstring != L"ObstaclesDodge")
			//{
			//	// ここを動かないようにじゃなくて迂回するルートを考える処理にする
			//	m_moveVec = Vec3(0.0f);

			//	// 障害物を避けるために進むルートを決める ポインタの関係でエラー吐く
			//	auto obstaclesDodgeObj = DodgeRoute();

			//	// 変更したステートに障害物を避けるためにこのオブジェクトを目印にしてほしいと伝える
			//	ChangeState(L"ObstaclesDodge");
			//	auto currentState = m_stateMachine->GetCurrentState(); // 現在のステート取得
			//	auto obstaclesDodgeState = dynamic_pointer_cast<StateObstaclesDodgeEnemy>(currentState);
			//	obstaclesDodgeState->SetObstaclesDodge(obstaclesDodgeObj);
			//}
		}

	}

	// 無敵時の処理
	void Enemy::Invincible()
	{
		if (m_invincibleFlag)
		{

			m_countTimeOfInvincible += m_delta;
			if (m_timeOfInvincible < m_countTimeOfInvincible)
			{
				// 無敵が切れる
				m_invincibleFlag = false;
				m_countTimeOfInvincible = 0.0f;

				// 点滅用の数値も初期化する
				m_color.w = 1.0f;
				m_countTimeOfBlinking = 0.0f;
			}

			// 無敵状態の時自分自身は点滅する
			DrawBlinking();
		}

		return;
	}

	// 無敵時の点滅処理
	void Enemy::DrawBlinking()
	{
		// 無敵状態の時自分自身は点滅する
		m_countTimeOfBlinking += m_delta;
		if (0.3f < m_countTimeOfBlinking)
		{
			if (m_color.w > 0.0f)
			{
				m_color.w = 0.0f;
			}
			else if (m_color.w <= 0.0f)
			{
				m_color.w = 1.0f;
			}

			m_countTimeOfBlinking = 0.0f;
		}

		return;
	}

	// 障害物を避けるルートを考える処理
	shared_ptr<ObstaclesDodge> Enemy::DodgeRoute()
	{
		auto objVec = GetStage()->GetGameObjectVec();
		vector<shared_ptr<ObstaclesDodge>> m_obstaclesDodgeObjs;

		for (auto obj : objVec)
		{
			// 回避ルート用のオブジェクトか確認した後回避ルートをきめる
			auto obstaclesDodgeObj = dynamic_pointer_cast<ObstaclesDodge>(obj);

			if (obstaclesDodgeObj)
			{
				// 障害物を回避するための目印か確認出来たら障害物一覧として受け取る
				if (obstaclesDodgeObj->FindTag(L"ObstaclesRoute"))
				{
					m_obstaclesDodgeObjs.push_back(obstaclesDodgeObj);
				}
			}
		}

		float min = 999999.9f; // 最短距離
		shared_ptr<ObstaclesDodge> targetObstaclesDodgeObj; // 障害物を避けるために追跡するオブジェクト

		// 障害物を避ける際の最短距離はどのオブジェクトを経由すればいいか確認する
		for (auto obj : m_obstaclesDodgeObjs)
		{
			auto objPos = obj->GetComponent<Transform>()->GetPosition();

			// 自分の位置と経由するオブジェクトの位置の差を求める
			auto differencePos = m_pos - objPos;
			float distanceVec = differencePos.length();

			// 最短距離だった場合そこを追跡対象として追いかける
			if (min > distanceVec && distanceVec != 0.0f)
			{
				min = distanceVec;
				targetObstaclesDodgeObj = obj;
			}
		}

		// 障害物を迂回するためにこのオブジェクトを追いかけてね
		return targetObstaclesDodgeObj;
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

	// 無敵フラグのゲッタ
	bool Enemy::GetInvincibleFlag()
	{
		return m_invincibleFlag;
	}

	// 無敵フラグをオンにする処理
	void Enemy::OnInvincibleFlag()
	{
		m_invincibleFlag = true;
		return;
	}


}
//end basecross