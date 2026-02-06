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
		m_trackingObj(shared_ptr<ScoreObject>(nullptr))
	{

	}

	Enemy::Enemy(const shared_ptr<Stage>& obj, const Vec3& pos, const Quat& qt, const Vec3& scale, const shared_ptr<CheckPoint>& startCheckPoint, const shared_ptr<Actor>& trackingObj) :
		FighterAircraftBase(obj, pos, qt, scale, startCheckPoint, Col4(0.0f, 1.0f, 1.0f, 1.0f)),
		m_trackingObj(shared_ptr<ScoreObject>(nullptr))
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
		m_trans->SetScale(Vec3(m_scale));
		//// 回転度取得
		//m_rot = m_trans->GetRotation();
		m_rot = Vec3(AdjustmentAngle(m_rot.x), AdjustmentAngle(m_rot.y), AdjustmentAngle(m_rot.z));

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(180.0f), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		// コリジョン追加
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(false);
		//ptrCol->SetAfterCollision(AfterCollision::None);
		auto ptrShadow = AddComponent<Shadowmap>();

		// ドロー処理
		m_draw = AddComponent<PNTBoneModelDraw>();
		m_draw->SetMeshResource(L"Sentouki");
		m_draw->SetTextureResource(L"diffuse_TX");
		m_draw->SetMeshToTransformMatrix(spanMat);
		m_draw->SetDiffuse(m_color);
		m_draw->SetEmissive(m_color);
		SetAlphaActive(true);

		ptrShadow->SetMeshResource(L"Sentouki");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		// アニメーション追加
		m_draw->AddAnimation(L"PropellerMove", 0, 50, 60.0f);
		m_draw->AddAnimation(L"PropellerDown", 40, 20, 30.0f);
		m_draw->AddAnimation(L"PropellerStop", 0, 1, 0.0f);

		m_draw->ChangeCurrentAnimation(L"PropellerMove");


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
		m_timeOfReturn = 5.0f;
		m_scoreCurrent = 0;

		// ステートマシン作成
		m_stateMachine = unique_ptr<StateEnemyMachine>(new StateEnemyMachine(GetThis<MyGameObject>()));
		m_stateMachine->ChangeState(L"Tracking"); // 仮で最初のステートはベースステートに変更する

		// レイキャスト生成
	}

	void Enemy::OnUpdate()
	{
		if (m_pauseFlag)
		{
			return;
		}
		// レイを表示したい数
		//RayCast::InitRay(1);

		// アニメーション再生
		m_draw->UpdateAnimation(m_delta);




		FighterAircraftBase::OnUpdate();

		auto& gameManager = GameManager::GetGameManager();
		auto currentPhase = gameManager->GetCurrentPhase();//現在フェーズ取得
		
		auto objVec = GetStage()->GetGameObjectVec();

		// 追跡対象がいなくなったら一番近いものを決めて追跡すると決める		
		ChangeTarget(currentPhase, objVec);

		// Playerを狙っているときの処理(応急処置)
		if (m_playerLock)
		{
			m_timeOfPlayerLock += m_delta;
		}
		if (m_timeOfPlayerLock >= 6.0f)
		{
			//m_trackingObj.lock() = nullptr;
			m_playerLock = false;
			m_timeOfPlayerLock = 0.0f;
		}


		// 射線上に敵対する相手がいたら攻撃する
		Vec3 hitPos;			// 出力用：レイの交差地点(衝突点)
		TRIANGLE triangle;		// レイが交差したポリゴンを構成する頂点の座標
		size_t triangleNumber;	// レイが交差したポリゴンの番号
		float min = 9999999.9f;	//Playerから見てカメラの障害となる距離の最小値
		bool moveEnd = false;	//移動処理が終わったかを保存する変数

		// レイの長さを求める
		auto forward = GetComponent<Transform>()->GetForward();
		forward = forward.normalize();
		Vec3 rayLength = (forward *15.0f);
		//GetStage()->AddGameObject<>
		

		// 射線上に敵戦闘機がいるか確認
		for (auto obj : objVec)
		{

			auto fighter = dynamic_pointer_cast<FighterAircraftBase>(obj);

			// 戦闘機のメッシュがレイに当たっているか確認する
			if (fighter)
			{
				auto fighterPos = fighter->GetPos();
				auto ptrDraw = fighter->GetComponent<SmBaseDraw>();
				//RayCast::DebugRay(Line(m_pos, m_pos + rayLength), Col4(1.0f, 0.5f, 1.0f, 1.0f), GetStage());
				ptrDraw->HitTestStaticMeshSegmentTriangles(m_pos, m_pos + rayLength, hitPos, triangle, triangleNumber);
			}

		}


		m_countDebagBulletTime += m_delta;

		// レイ射線上に飛行機が当たったら弾を発射する
		if (hitPos != Vec3(0.0f) && m_countDebagBulletTime >= 0.5f)
		{
			GetStage()->AddGameObject<Bullet>(GetThis<Actor>());
			m_countDebagBulletTime = 0.0f;
		}



		// ステートのUpdate
		m_stateMachine->Update();

		// デバック用に弾を出す
		auto stateName = m_stateMachine->GetCurrentStateWString();

		//if (m_countDebagBulletTime >= 0.5f)
		//{
		//	if (stateName == L"Tracking")
		//	{
		//		GetStage()->AddGameObject<Bullet>(GetThis<Actor>());
		//	}
		//	m_countDebagBulletTime = 0.0f;
		//} 
		// リスポーン状態なら移動しないように変更
		if (stateName == L"Respawn")
		{
			m_moveVec = Vec3(0.0f);
		}

		// 追いかけるものが消えていたらUpdateしないようにする
		if (!m_trackingObj.lock())
		{
			return;
		}

		// ヨーピッチロールに沿って回転する処理
		MoveRotate();

		// 無敵時用の処理
		Invincible();

		// Transform反映
		m_trans->SetQuaternion(m_qt); // qt反映
		//m_trans->SetRotation(m_rot);
		m_trans->SetPosition(m_pos + m_moveVec); // pos反映

		// 位置取得
		m_pos = GetComponent<Transform>()->GetPosition();

		// カラー適応
		m_draw->SetEmissive(m_color);
		m_draw->SetDiffuse(m_color);

		//アニメーション再生
		//GetComponent<PNTBoneModelDraw>()->UpdateAnimation(m_delta);


		////デバック用
		//wstringstream wss(L"");
		//auto scene = App::GetApp()->GetScene<Scene>();

		//wss /* << L"デバッグ用文字列 "*/
		//	<< L"\nm_pitchAngle : " << (int)currentPhase
		//	<< endl;

		//scene->SetDebugString(wss.str());
	}

	// 当たり判定
	void Enemy::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		// リスポーンステートの時は当たり判定処理取らない
		wstring currentState = m_stateMachine->GetCurrentStateWString();
		if (currentState == L"Respawn") return;


		FighterAircraftBase::OnCollisionEnter(obj);

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

	// 目標の変更処理
	void Enemy::ChangeTarget(GamePhase currentPhase, const vector<shared_ptr<GameObject>>& objVec)
	{
		// 追跡対象がいなくなったら一番近いものを決めて追跡すると決める
		if (currentPhase == GamePhase::Score)
		{
			int minLenght = 999999.9f;
			int minDefault = 999999.9f;
			// スコアオブジェクトを継承しているものだけ取得
			for (const auto &obj : objVec)
			{
				auto scoreObjectCast = dynamic_pointer_cast<ScoreObject>(obj);
				//アクターを継承しているオブジェクト停止
				if (scoreObjectCast)
				{
					auto scorePos = scoreObjectCast->GetComponent<Transform>()->GetPosition();

					auto differenceVec = scorePos - m_pos;
					float differenceLength = differenceVec.length();

					if (minLenght >= abs(differenceLength))
					{
						m_trackingObj = scoreObjectCast;
						minLenght = differenceLength;
					}
				}
			}
			// スコアオブジェクトが無かったら他の戦闘機たちを倒す
			if (!m_trackingObj.lock())
			{
				m_trackingObj = GetStage()->GetSharedGameObject<Player>(L"Player");
				m_playerLock = true;
			}
		}
		// アイテムフェーズ時でだれを狙うか決める
		if (currentPhase == GamePhase::Item)
		{
			m_trackingObj = GetStage()->GetSharedGameObject<Player>(L"Player");
		}

	}

	// 対象に向かって追いかける処理
	void Enemy::TrackingMove(const Vec3& posPlayerDifference)
	{
		// 移動ベクトル加算
		auto forward = m_trans->GetForward();
		m_moveVec = (forward * m_delta) * m_speed;

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

		// 敵から見て目標が下にいたら角度をマイナスにする
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
		//if (input->GetButton(L"DLeft"))
		//{
		//	debugYX -= m_delta * 3.0f;
		//}
		//if (input->GetButton(L"DRight"))
		//{
		//	debugYX += m_delta * 3.0f;
		//}
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
		// 閾値(みなし距離)
		float Threshold = 1.0f;
		
		Vec3 differenceRotVec = m_goalRotVec - m_rot;// ここ内積でいくつの数値が少なかったらという形でやるべきだった

		m_qt = m_trans->GetQuaternion();

		Quat goalqt;
		goalqt.rotationRollPitchYawFromVector(m_goalRotVec);

		m_qt = m_qt.Slerp(m_qt, goalqt, 0.1f);

		// 現在の向きと目標の向きがほぼ同じなら目標の向きに代入する
		//if (differenceRotVec.length() <= 0.1f)
		//{
		//	m_rot = m_goalRotVec;
		//}

		// クォータニオンの内積計算
		float dot;
		dot = m_qt.dot(goalqt);
		dot = abs(dot);

		// ドット積から角度(ラジアン)計算
		float differenceAngle = 2.0f * acos(min(dot, 1.0f));

		// 閾値以下の回転の差なら、目標の回転にする
		if (differenceAngle <= XMConvertToRadians(Threshold))
		{
			m_qt = goalqt;
		}

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
			auto obstacles = dynamic_pointer_cast<StageObject>(obj);// 当たり判定の対象
		
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

	// ターゲットのセッタ
	void Enemy::SetTracking(const shared_ptr<Actor>& target)
	{
		m_trackingObj = target;
	}

	// ターゲットのゲッタ
	const weak_ptr<Actor>& Enemy::GetTracking()
	{
		return m_trackingObj;
	}


}
//end basecross