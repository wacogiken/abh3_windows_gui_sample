/*
 ******************************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2023, Waco Giken Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the Waco Giken nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
*/

#pragma once

#include "framework.h"

class CguicanABH3View : public CFormView
{
//===============================================================
//内部用
//===============================================================
protected:

	//列挙子
	typedef enum
		{
		QTY_HEARTBEAT = 8,		//周期送信要素の個数
		} ENUM;

	//内部変数用構造体
	typedef struct _VIEW_VAR
		{
		UINT	nTimerNum;		//周期設定で使うタイマー番号
		UINT	n1sec;			//1秒割り込みで使うタイマー番号

		//ハートビート関連
		struct _HEATBEAT
			{
			uint8_t	lifeTime[QTY_HEARTBEAT];	//受信に対する残り寿命、表示周期(100ms)毎に1引かれ、0で寿命扱い
			} heartbeat;
	
		//非同期送信スレッド関連
		struct _THREAD
			{
			bool		bQuit;		//スレッド停止要求
			bool		bRequest;	//周期送信処理の開始フラグ
			HANDLE		hThread;	//スレッドハンドル
			UINT		nUid;		//スレッドID
			uint32_t	nResult;	//周期送信処理の実行結果（0以外の場合にタイマー割り込み側で処理される）
			} thread;

		//最終受信データ（バス上から取得したABH3データ）
		CANABH3_LASTRECV	lastdata;

		//ユーザー側から制御する要素（ローカルのみで保存）
		struct _USERCTRL
			{
			uint32_t	nRequestFlag;	//操作フラグ
			} userctrl;

		} VIEW_VAR,*pVIEW_VAR;
	
	//========================================
	//ハートビート関連（受信の有無判断）
	//========================================

	//指定番号のハートビートが有りました
	void UpHeartBeat(int nNum);

	//全体のハートビートカウンタを減らします
	void DownHeartBeat(void);

	//ハートビート中か？
	bool IsHeartBeat(int nNum)
		{
		return((bool)(m_var.heartbeat.lifeTime[nNum] != 0) ? true : false);
		}

	//========================================
	//画面構築関連
	//========================================

	//表示画面構築
	void CreateScreen(void);

	//画面アイテムの再配置
	void ArrangeScreenItem(void);

	//指定IDのダイアログアイテムをクライアント座標の指定位置に移動
	CRect MoveItem(UINT nUid,POINT clientLeftTop)
		{
		CRect itemRect,moveRect;
		GetDlgItem(nUid)->GetWindowRect(itemRect);
		moveRect.left = clientLeftTop.x;
		moveRect.top = clientLeftTop.y;
		moveRect.right = moveRect.left + itemRect.Width();
		moveRect.bottom = moveRect.top + itemRect.Height();
		GetDlgItem(nUid)->MoveWindow(moveRect);
		return(moveRect);
		}
	CRect MoveItem(UINT nUid,CPoint topLeft,CSize size)
		{
		CRect moveRect;
		moveRect.left = topLeft.x;
		moveRect.right = moveRect.left + size.cx;
		moveRect.top = topLeft.y;
		moveRect.bottom = moveRect.top + size.cy;
		GetDlgItem(nUid)->MoveWindow(moveRect);
		return(moveRect);
		}

	//ウィンドウタイトルを更新
	void UpdateWindowTitle(void);

	//シート情報を更新
	void UpdateSheetInfo(void);

	//テーブルを元にテキストを一括設定
	void SetTextTbl(pIDTEXT pIDtbl);
	void SetTextTbl(pIDTEXT1 pIDtbl);
	void SetTextTbl(pIDTEXT2 pIDtbl);
	void SetTextTbl(pIDTEXT4 pIDtbl);
	void SetTextTbl(pIDTEXT5 pIDtbl);

	//テーブルを元にボタンを一括操作
	void SetButtonTbl(pIDTEXT2 pIDtbl,bool bOn);
	void SetButtonTbl(pIDTEXT5 pIDtbl,bool bOn);

	//指令切り替えのボタン制御
	void CtrlButtonOrder(bool bSpeed);

	//周期送信のボタン制御
	void CtrlButtonInterval(bool bON);

	//比較機能付きアイテムテキスト設定
	bool FastSetText(UINT nUid,CString sNewText)
		{
		CString sOldText("");
		GetDlgItem(nUid)->GetWindowText(sOldText);
		if(sOldText == sNewText)
			return(false);
		GetDlgItem(nUid)->SetWindowText(sNewText);
		return(true);
		}
	bool FastSetText(UINT nUid,TCHAR* pNewText)
		{
		return(FastSetText(nUid,CString(pNewText)));
		}

	//色制御全体確認
	bool DrawCheck(CWnd* pWnd,COLORITEM& colorItem);
	//固定テキスト類アイテムの色制御確認
	bool DrawCheck_0(CWnd* pWnd,COLORITEM& colorItem);
	//入力フラグの色制御確認
	bool DrawCheck_1(CWnd* pWnd,COLORITEM& colorItem);
	//制御フラグの色制御確認
	bool DrawCheck_2(CWnd* pWnd,COLORITEM& colorItem);
	//I/Oフラグの色制御確認
	bool DrawCheck_3(CWnd* pWnd,COLORITEM& colorItem);
	//警告フラグの色制御確認
	bool DrawCheck_4(CWnd* pWnd,COLORITEM& colorItem);
	//異常フラグの色制御確認
	bool DrawCheck_5(CWnd* pWnd,COLORITEM& colorItem);
	//ハートビート関連
	bool DrawCheck_6(CWnd* pWnd,COLORITEM& colorItem);

	//全データ部分の更新
	void UpdateView(bool bForce = false);
	//制御フラグの更新(要素0)
	void UpdateView_0(bool bForce = false);
	//異常・警告フラグの更新(要素1)
	void UpdateView_1(bool bForce = false);
	//I/Oフラグ・入力フラグの更新(要素2)
	void UpdateView_2(bool bForce = false);
	//速度指令・速度帰還の更新(要素3)
	void UpdateView_3(bool bForce = false);
	//電流指令・負荷率の更新(要素4)
	void UpdateView_4(bool bForce = false);
	//パルス積算値の更新(要素5)
	void UpdateView_5(bool bForce = false);
	//アナログ入力・電源電圧の更新(要素6)
	void UpdateView_6(bool bForce = false);
	//モニタデータの更新(要素7)
	void UpdateView_7(bool bForce = false);

	//========================================
	//設定関連
	//========================================

	//設定の復帰
	void reg2disp(void);

	//指令を設定しシステムへ保存
	void SetOrder(int nNum,CString sValue);
	void SetOrder(int nNum,float nValue);

	//指令モードを設定しシステムへ保存
	void SetOrderType(int nMode);
	
	//指令と指令選択の復帰
	void RestoreOrder();

	//周期設定を設定しシステムへ保存
	void SetInterval(uint32_t nTimeMS);

	//周期設定を復帰
	void RestoreInterval(void);

	//周期転送対象の保存
	//void SaveSendButton(uint8_t nID);

	//周期送信対象の復帰
	void RestoreRequestTarget(void);

	//周期送信対象の取り込み
	uint32_t StockRequestTarget(bool bNoSave = false);

	//========================================
	//変換系
	//========================================

	//floatから指定フォーマット文字列に変換
	CString float2text(TCHAR* pFmt,float nValue)
		{
		CString sResult("");
		sResult.Format(pFmt,nValue);
		return(sResult);
		}

	//uintから指定フォーマット文字列に変換
	CString uint2text(TCHAR* pFmt,uint32_t nValue)
		{
		CString sResult("");
		sResult.Format(pFmt,nValue);
		return(sResult);
		}

	//========================================
	//処理系
	//========================================

	//周期要求設定の実行
	uint32_t ExecCycleRequest(void);

public:
	//内部変数（スレッドから利用する為、publicに置く）
	VIEW_VAR m_var;

	//コントロール変数
	CBrush m_brush;
	CStatic m_id;
	CStatic m_result_bit[32];
	CStatic m_input_bit[32];
	CStatic m_io_bit[32];
	CButton m_ctrl_off_bit[32];
	CButton m_ctrl_on_bit[32];
	CAdvanceEdit m_edit[2];
	CStatic m_order_title;
	CButton m_order_value[2];
	CAdvanceEdit m_interval;
	CButton m_interval_btn[2];
	CStatic m_status[8];

	//非同期送信スレッド
	static unsigned __stdcall SendThread(void* pParam);

	//指定値に対する特定ビットが成立しているか取得
	bool IsBit(uint32_t nValue,int nBit)
		{
		if(nValue & (1 << nBit))
			return(true);
		return(false);
		}

	//イベントハンドラとオーバライド
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeInterval();
	afx_msg void OnEnChangeEdit(UINT nUid);
	afx_msg void OnBnClickedOffCtrlid(UINT nUid);
	afx_msg void OnBnClickedOnCtrlid(UINT nUid);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRequestTarget(UINT nUid);
	afx_msg void OnBnClickedOrder(UINT nUid);
	afx_msg void OnBnClickedInterval(UINT nUid);
	afx_msg void OnUpdateOnInterval(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOffInterval(CCmdUI* pCmdUI);

	//その他
	enum{ IDD = IDD_GUI_CANABH3_FORM };
	CguicanABH3Doc* GetDocument() const;
	virtual ~CguicanABH3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//基本構造
	CguicanABH3View() noexcept;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();
	DECLARE_DYNCREATE(CguicanABH3View)
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // gui_canABH3View.cpp のデバッグ バージョン
inline CguicanABH3Doc* CguicanABH3View::GetDocument() const
   { return reinterpret_cast<CguicanABH3Doc*>(m_pDocument); }
#endif

