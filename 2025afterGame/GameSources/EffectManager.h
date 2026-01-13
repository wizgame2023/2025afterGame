/*!
@file EffectManager.h
@brief エフェクトの再生と操作を行うマネージャー
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	// EffectManager : エフェクトマネージャー
	//--------------------------------------------------------------------------------------
	class EffectManager
	{
	//--------------------------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------------------------
	private:
		ManagerRef m_manager;
		RendererRef m_renderer;
		map<wstring, EffectRef> m_effectList;
		bool m_isCreateEffectManager = false;
		bool m_isEffectPause = false;
	//--------------------------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------------------------
	private:
		//----------------------------------------------------------------------------------
		/*
			コンストラクタ
		*/
		EffectManager() {}
		//----------------------------------------------------------------------------------
		/*
			デストラクタ
		*/
		virtual ~EffectManager() {}
	public:
		//----------------------------------------------------------------------------------
		/*
			インスタンス
		*/
		static EffectManager& Instance();
		//----------------------------------------------------------------------------------
		/*
			レンダラーを取得
			@param なし
			@return m_renderer レンダラー
		*/
		RendererRef GetRenderer();
		//----------------------------------------------------------------------------------
		/*
			マネージャーを取得
			@param なし
			@return m_manager マネージャー
		*/
		ManagerRef GetManager();
		//----------------------------------------------------------------------------------
		/*
			Effekseerのインターフェース設定
			@param なし
			@return なし
		*/
		void CreateEfkInterface();
		//----------------------------------------------------------------------------------
		/*
			エフェクトを登録する
			@param EfkKey エフェクトの登録キーネーム
			@param EfkFilePash エフェクトのファイルパス
			@return	なし
		*/
		void RegisterEffect(const wstring EfkKey, const wstring EfkFilePash);
		//----------------------------------------------------------------------------------
		/*
			エフェクトを再生する
			@param EfkKey 再生したいエフェクトのキー
			@param Emitter 再生位置
			@param StartFreme 開始フレーム
			@return	なし
		*/
		Handle PlayEffect(const wstring EfkKey, const Vec3 Emitter, const float StartFreme = 0.0f);
		//----------------------------------------------------------------------------------
		/*
			エフェクトの位置に加算する
			@param EfkHandle エフェクトのハンドル
			@param Position 加算する値 
			@return	なし
		*/
		void AddPosition(const Handle& EfkHandle, const Vec3 Position);
		//----------------------------------------------------------------------------------
		/*
			エフェクトの位置を指定する
			@param EfkHandle エフェクトのハンドル
			@param Position 指定する値 
			@return	なし
		*/
		void SetPosition(const Handle& EfkHandle, const Vec3 Position);
		//----------------------------------------------------------------------------------
		/*
			エフェクトをオイラー角による回転で設定する
			@param EfkHandle エフェクトのハンドル
			@param EulerAngles オイラー角
			@return	なし
		*/
		void SetRotationFromEulerAngles(const Handle& EfkHandle, const Vec3 EulerAngles);
		//----------------------------------------------------------------------------------
		/*
			エフェクトをクォータニオンによる回転で設定する
			@param EfkHandle エフェクトのハンドル
			@param Quaternion クォータニオンの値
			@return	なし
		*/
		void SetRotationFromQuaternion(const Handle& EfkHandle, const Quat Quaternion);
		//----------------------------------------------------------------------------------
		/*
			エフェクトを軸と角度による回転で設定する
			@param EfkHandle エフェクトのハンドル
			@param AxisVec 軸（方向ベクトル）
			@param Angle 角度
			@return	なし
		*/
		void SetRotationFromAxisAngle(const Handle& EfkHandle, const Vec3 Axis, const float Angle);
		//----------------------------------------------------------------------------------
		/*
			エフェクトの拡大率を指定する。
			@param EfkHandle エフェクトのハンドル
			@param Scale 拡大する値
			@return	なし
		*/
		void SetScale(const Handle& EfkHandle, const Vec3 Scale);
		//----------------------------------------------------------------------------------
		/*
			エフェクトの全体の色を指定する
			@param EfkHandle エフェクトのハンドル
			@param Col 色の値
			@return	なし
		*/
		void SetAllColor(const Handle& EfkHandle, const Col4 Col);
		//----------------------------------------------------------------------------------
		/*
			エフェクトの再生を止める
			@param EfkHandle エフェクトのハンドル
			@return	なし
		*/
		void StopEffect(const Handle& EfkHandle);
		//----------------------------------------------------------------------------------
		/*
			エフェクトの再生を全て止める
			@param EfkHandle エフェクトのハンドル
			@return	なし
		*/
		void StopAllEffect();
		//----------------------------------------------------------------------------------
		/*
			エフェクトの再生の一時停止を切替
			@param なし
			@return	なし
		*/
		void PauseEffectSwitch();
		//----------------------------------------------------------------------------------
		/*
			エフェクトのポーズフラグをリセット
			@param なし
			@return	なし
		*/
		void ResetIsEffPause();
		//----------------------------------------------------------------------------------
		/*
			エフェクトのNull値を取得
			@param なし
			@return	effNull 値は-10000
		*/
		Handle GetEffNull();
		//----------------------------------------------------------------------------------
		/*
			エフェクトを止めてNull値に設定
			@param HEff エフェクトハンドル
			@return	なし
		*/
		void SetEffStopAndNull(Handle& HEff);
	};

	//--------------------------------------------------------------------------------------
	// S_EffHandle : エフェクトハンドル構造体
	//--------------------------------------------------------------------------------------
	struct S_EffHandle
	{
	private:
		Handle handle;
	public:
		//----------------------------------------------------------------------------------
		/*
			コンストラクタ
		*/
		S_EffHandle() : handle(EffectManager::Instance().GetEffNull()) {}
		//----------------------------------------------------------------------------------
		/*
			デストラクタ
		*/
		~S_EffHandle() {};
		//----------------------------------------------------------------------------------
		/*
			Handle型からの代入
		*/
		S_EffHandle& operator=(const Handle& H) { handle = H; return *this; }
		//----------------------------------------------------------------------------------
		/*
			Handle型への暗黙キャスト
		*/
		operator Handle() const { return handle; }
		//----------------------------------------------------------------------------------
		/*
			Handle型へ参照渡し
		*/
		operator Handle& () noexcept { return handle; }
		operator const Handle& () const noexcept { return handle; }
	};
}