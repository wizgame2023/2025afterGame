/*!
@file StateBase.h
@brief ステートの基盤クラス
担当：三瓶裕太
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross {
	class StateBase
	{
	protected:
		weak_ptr<MyGameObject> m_parentObj;

		float m_deltaTime; // 次フレームに進むためにかかった時間

	public:
		StateBase(const shared_ptr<MyGameObject>& parentObj);
		~StateBase();

		virtual void OnEnter(); // 開始処理
		virtual void OnUpdate(); // 継続処理
		virtual void OnExit(); // 終了処理
	};


	class StateMachineBase
	{
	protected:
		map<wstring, shared_ptr<StateBase>> m_stateTypes; // ステート一覧
		shared_ptr<StateBase> m_stateCurrent; // 現在のステート
		wstring m_stateName; //ステートの名前
		shared_ptr<StateBase> m_stateBefor; // 前のステート

		weak_ptr<MyGameObject> m_parentObj; // ステートマシンを持つ親オブジェクト

		// ステートの追加処理
		void AddState(wstring stateName, const shared_ptr<StateBase>& state);

	public:

		StateMachineBase(const shared_ptr<MyGameObject>& parent);
		~StateMachineBase();

		// ステート変更用の関数
		void ChangeState(wstring stateName);

		void Update(); // 更新
	};



}

//end basecross

