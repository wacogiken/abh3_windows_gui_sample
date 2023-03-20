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
protected:
	//
	typedef struct _VIEW_VAR
		{
		UINT	nTimerNum;		//周期設定で使うタイマー番号
		UINT	n1sec;			//1秒割り込みで使うタイマー番号
		UINT	nFPS;			//周期設定で呼び出された数（毎秒リセット）
		UINT	nSendCounter;	//非同期スレッドで送信した回数（毎秒リセット）

		struct _CYCLE
			{
			uint32_t	nCycleRequest;		//bit0側からbit単位で使う
			//uint8_t	nCycleCount;		//周期要求要素数
			//uint8_t	enableCycle[16];	//周期要素要求フラグ(0以外で要求)
			} cycle;


		//非同期送信スレッド関連
		struct _THREAD
			{
			bool		bQuit;		//スレッド停止要求
			HANDLE		hThread;	//スレッドハンドル
			UINT		nUid;		//スレッドID
			bool		bRequest;	//送信実行フラグ
			uint32_t	nResult;	//実行結果（0以外の場合にタイマー割り込み側で処理される）
			} thread;

		//最終受信データ（バス上から取得したABH3データ）
		CANABH3_LASTRECV	lastdata;

		//シングルパケット用データ格納領域（周期転送の為、ローカルに保存）
		struct _DP0R
			{
			float nValue[2];	//0..A指令  1..B指令（文字列を数値変換しただけの値）
			uint32_t nRequest;	//操作フラグ
			} DP0R;
		} VIEW_VAR,*pVIEW_VAR;

	//画面アイテムの再配置
	void ArrangementItem(void);
	CRect ArrangementItem_sub(UINT nBaseUID,UINT nLastUID,bool bVert = true);

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

	//画面上の周期要求要素を取り込み
	uint32_t disp2cycle(void);

	//ウィンドウタイトルを更新
	void UpdateTitle(void);

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

	//テーブルからダイアログアイテムにテキストを一括設定
	void SetTextTbl(pIDTEXT pIDtbl);
	void SetTextTbl(pIDTEXT1 pIDtbl);
	void SetTextTbl(pIDTEXT2 pIDtbl);
	void SetTextTbl(pIDTEXT4 pIDtbl);
	void SetTextTbl(pIDTEXT5 pIDtbl);

	//指定テーブルのボタンを操作
	void SetButtonTbl(pIDTEXT2 pIDtbl,bool bOn);
	void SetButtonTbl(pIDTEXT5 pIDtbl,bool bOn);

	//周期転送の復帰
	void RestoreSendButton(uint8_t nID);

	//周期転送の保存
	void SaveSendButton(uint8_t nID);

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

	//周期要求設定の実行
	uint32_t ExecCycleRequest(uint32_t nCycleRequest);

	//色制御全体確認
	bool DrawCheck(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor);
	//
	bool DrawCheck_0(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor);
	//入力フラグの色制御確認
	bool DrawCheck_1(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor);
	//制御フラグの色制御確認
	bool DrawCheck_2(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor);
	//I/Oフラグの色制御確認
	bool DrawCheck_3(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor);
	//警告フラグの色制御確認
	bool DrawCheck_4(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor);
	//異常フラグの色制御確認
	bool DrawCheck_5(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor);

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

public:
	//内部変数（スレッドから利用する為、publicに置く）
	VIEW_VAR m_var;

	CBrush m_brush;
	CStatic m_id;
	CStatic m_fps;
	CStatic m_warn_bit[32];
	CStatic m_err_bit[32];
	CStatic m_result_bit[32];
	CStatic m_input_bit[32];
	CStatic m_io_bit[32];
	CButton m_ctrl_off_bit[32];
	CButton m_ctrl_on_bit[32];
	CAdvanceEdit m_edit[2];

	//非同期送信スレッド
	static unsigned __stdcall SendThread(void* pParam);

	//指定値に対する特定ビットが成立しているか取得
	bool IsBit(uint32_t nValue,int nBit)
		{
		if(nValue & (1 << nBit))
			return(true);
		return(false);
		}

	//チェックボックスの状態を取得
	bool IsCheckbox(UINT nUid)
		{
		CButton* pBtn = (CButton*)GetDlgItem(nUid);
		if(pBtn->GetCheck() == 0)
			return(false);
		return(true);
		}

	//チェックボックスの状態を設定
	void SetCheckbox(UINT nUid,bool bCheck)
		{
		CButton* pBtn = (CButton*)GetDlgItem(nUid);
		//現在の状態を取得
		bool bCurrent = IsCheckbox(nUid);
		//現在の状態と異なる状態を要求しているか？
		if(bCurrent != bCheck)
			{
			//チェックを付ける要求？
			if(bCheck)
				pBtn->SetCheck(1);
			else
				pBtn->SetCheck(0);
			}
		}

public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEdit(UINT nUid);
	afx_msg void OnBnClickedOffCtrlid(UINT nUid);
	afx_msg void OnBnClickedOnCtrlid(UINT nUid);

protected:
	CguicanABH3View() noexcept;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();
	DECLARE_DYNCREATE(CguicanABH3View)
	DECLARE_MESSAGE_MAP()

public:
	enum{ IDD = IDD_GUI_CANABH3_FORM };
	CguicanABH3Doc* GetDocument() const;
	virtual ~CguicanABH3View();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnEnableCycle();
	afx_msg void OnDisableCycle();
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRequestItem();
};

#ifndef _DEBUG  // gui_canABH3View.cpp のデバッグ バージョン
inline CguicanABH3Doc* CguicanABH3View::GetDocument() const
   { return reinterpret_cast<CguicanABH3Doc*>(m_pDocument); }
#endif

