/*!
@file StateBase.cpp
@brief 基盤ステートのクラス
担当：三瓶裕太
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	StateBase::StateBase(const shared_ptr<MyGameObject>& parentObj):
		m_parentObj(parentObj)
	{
	}

	StateBase::~StateBase()
	{
	}

	void StateBase::OnEnter()
	{
		m_deltaTime = GameManager::GetGameManager()->GetDeltaTime();
	}

	void StateBase::OnUpdate()
	{
		m_deltaTime = GameManager::GetGameManager()->GetDeltaTime();
	}

	void StateBase::OnExit()
	{

	}



	// ステートマシンのコンストラクタ
	StateMachineBase::StateMachineBase(const shared_ptr<MyGameObject>& parentObj):
		m_parentObj(parentObj)
	{
		
	}
	// ステートマシンのデストラクタ
	StateMachineBase::~StateMachineBase()
	{

	}

	// ステートの追加処理
	void StateMachineBase::AddState(wstring stateName, const shared_ptr<StateBase>& state)
	{
		// 入れたいステートが同じ名前や型ならエラーを起こす
		for (auto it : m_stateTypes)
		{
			if (it.first == stateName)
			{
				throw BaseException
				{
					L"重複しているステートの名前があります",
					L"if (it.first == stateName)",
					L"void StateMachineBase::AddState(wstring stateName, const shared_ptr<StateBase>& state)"
				};
			}
			if (it.second == state)
			{
				throw BaseException
				{
					L"重複しているステートの型があります",
					L"if (it.second == state)",
					L"void StateMachineBase::AddState(wstring stateName, const shared_ptr<StateBase>& state)"
				};
			}
		}
		m_stateTypes[stateName] = state;
	}

	// ステート変更用の関数
	// 引数1 移行したいステートの名前
	shared_ptr<StateBase> StateMachineBase::ChangeState(wstring stateName)
	{
		// フラグとしてステート変更ができているかをチェックする変数を作る
		bool changeStateFlag = false;
		
		// ステート管理配列内に同じ名前のステートがあるか探索
		for (auto it : m_stateTypes)
		{
			if (it.first == stateName)
			{
				m_stateName = stateName;
				// もし、ステート移行するときに元々ステートが入っているときは終了処理をしてからにする
				if (m_stateCurrent != nullptr)
				{
					// ここに移行前のステートの終了処理を書く
					m_stateCurrent->OnExit();
				}

				// ステートを移行する処理
				m_stateBefor = m_stateCurrent;
				m_stateCurrent = m_stateTypes[stateName];
				m_stateCurrent->OnEnter(); // ステートの初期化処理をする
				changeStateFlag = true; // ステート変更できているフラグにする

				// 変更できたステートを渡す
				return m_stateCurrent;
			}
			
		}

		// ステート変更できていなかったらエラー文を出るようにする
		if (!changeStateFlag)
		{
			throw BaseException
			(
				L"指定している名前のステートは存在しません",
				L"if (it.first == stateName)",
				L"StateMachineBase::ChangeState(wstring stateName)"
			);
		}

		return nullptr;
	}

	// 現在ステートのゲッタ
	shared_ptr<StateBase> StateMachineBase::GetCurrentState()
	{
		return m_stateCurrent;
	}

	// 現在ステート文字列のゲッタ
	wstring StateMachineBase::GetCurrentStateWString()
	{
		// 現在のステートがどのような文字列で登録したか渡す
		for (auto type : m_stateTypes)
		{
			if (type.second == m_stateCurrent)
			{
				return type.first;
			}
		}

		return nullptr;
	}

	// 更新
	void StateMachineBase::Update()
	{
		m_stateCurrent->OnUpdate();
	}

}
//end basecross


