/*!
@file Enemy.cpp
@brief 敵実体
担当：三瓶裕太
*/

#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

namespace basecross {
	Enemy::Enemy(const shared_ptr<Stage>& obj,const Vec3& pos,const Quat& qt,const Vec3& scale):
		FighterAircraftBase(obj,pos,qt,scale)
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
		m_trans->SetScale(m_scale);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
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


	}

	void Enemy::OnUpdate()
	{
		// xz方面の距離の差を求める 次はzy方面の距離の差を求める
		FighterAircraftBase::OnUpdate();
		auto goal = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto goalPos = goal->GetComponent<Transform>()->GetPosition();
		auto posPlayerDifference = m_pos - goalPos; // ゴールと敵の位置の差を求める
		posPlayerDifference.normalize();

		// 目的地の角度を取得
		float goalAngle = atan2f(posPlayerDifference.z, -posPlayerDifference.x);
		float goalAngleZY = atan2f(-posPlayerDifference.y, -posPlayerDifference.z);

		float speed = 0.0f;
		float testAngle = XMConvertToRadians(45.0f);

		// Pos移動
		m_pos.x += cos(goalAngle) * speed * m_delta;
		m_pos.z += sin(goalAngle) * speed * m_delta;

		// Qt回転
		//m_qt = Quat(0.0f, 0.0f, (sin(goalAngleZY / 2.0f)), cos((goalAngle / 2.0f))) *
		//	Quat(0.0f, (sin(goalAngle / 2.0f)), 0.0f, cos((goalAngle / 2.0f)));
		m_qt = Quat(0.0f, 0.0f, (sin(goalAngleZY / 2.0f)), cos((goalAngleZY / 2.0f)));

		//auto cameraManager = GetStage()->GetSharedGameObject<MainCameraManager>(L"MainCameraManager");
		//cameraManager->DebugLog(L"goalAngle : ", XMConvertToDegrees(goalAngle));
		//cameraManager->DebugLog(L"goalAngleZY : ",XMConvertToDegrees(goalAngleZY));

		//m_qt = Quat(0.0f, 0.0f, (sin(goalAngleZY / 2.0f)), cos((goalAngle / 2.0f)));
		m_trans->SetQuaternion(m_qt);
		//m_trans->SetRotation(0.0f, -goalAngle, 0.0f);

		m_trans->SetPosition(m_pos);

		////デバック用
		wstringstream wss(L"");
		auto scene = App::GetApp()->GetScene<Scene>();

		wss /* << L"デバッグ用文字列 "*/
			<< L"\ngoalAngle : " << XMConvertToDegrees(goalAngle)
			<< L"\ngoalAngleZY : " << XMConvertToDegrees(goalAngleZY)
			<< endl;

		scene->SetDebugString(wss.str());

	}

}
//end basecross