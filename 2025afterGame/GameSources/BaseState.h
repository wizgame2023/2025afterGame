/*!
@file BaseState.h
@brief ステートの基盤クラス
担当：三瓶裕太
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross {
	class BaseState;
	class StateMachineBase
	{
	protected:
		map<wstring, shared_ptr<BaseState>> m_stateTypes; // ステート一覧
		shared_ptr<BaseState> m_stateCurrent; // 現在のステート
		wstring m_stateName; //ステートの名前
		shared_ptr<BaseState> m_stateBefor; // 前のステート

		weak_ptr<MyGameObject> m_parentObj; // ステートマシンを持つ親オブジェクト

		// ステートの追加処理
		void AddState(wstring stateName, const shared_ptr<BaseState>& state);

	public:

		StateMachineBase(shared_ptr<MyGameObject> parent);
		~StateMachineBase();

		// ステート変更用の関数
		void ChangeState(wstring stateName);

		void Update(); // 更新
	};


	class BaseState
	{
	private:

	public:
		BaseState();
		~BaseState();

		virtual void OnEnter(); // 開始処理
		virtual void OnUpdate(); // 継続処理
		virtual void OnExit(); // 終了処理
	};

}

//end basecross

