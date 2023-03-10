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

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル
#include "ConfigDlg.h"

//Aboutダイアログクラス
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;
	enum { IDD = IDD_ABOUTBOX };

protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
		DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};


//APPクラス
class CguicanABH3App : public CWinAppEx
{
protected:
	//
	typedef struct _GUICANABH3_VAR
		{
		//
		HMODULE		hDLL;			//DLLのハンドル

		int			nCodePage;		//現在のWindows表示言語
		int			nLastTimerNum;	//タイマー割り込みに使う値
		uint8_t		usedID[256];	//bit0..client  bit1..notuse  bit2..error
		uint8_t		nRequestID;		//ウィンドウを開く時に要求したID

		struct _VAR_THREAD
			{
			HANDLE	hHandle;
			UINT	nUid;
			bool	bQuit;
			} thread;
		} GUICANABH3_VAR,*pGUICANABH3_VAR;
		GUICANABH3_VAR m_var;

	//スレッド
	static unsigned int __stdcall thread(void* pParam);

public:

	//本ソフトの再起動
	int RestartApplication(void);


	int32_t Connect(void);
	int32_t Disconnect(void);
	int32_t Reconnect(void);
	CguicanABH3App() noexcept;
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	afx_msg void OnAppAbout();

//	HMODULE m_hDLL;

	//現在の表示言語指定に合う文字列を戻す
	TCHAR* GetLangText(pLANGTEXT pText);

	//タイマー割り込み用の値を取得
	int GetTimerNum(void)
		{
		m_var.nLastTimerNum++;
		return(m_var.nLastTimerNum);
		}

	//1つ以上ウィンドウを開いているか？
	bool isUsedClientWindow(void)
		{
		for(int nLoop = 1;nLoop < 256;nLoop++)
			{
			//bit0が成立しているか？（ID利用中のウィンドウが有るか？）
			if(isUsedID(nLoop) & 1)
				return(true);
			}
		//開いてない
		return(false);
		}

	//指定接続先のウィンドウでエラーが出ているか？
	bool isErrorID(uint8_t nID)
		{
		if(m_var.usedID[nID] & 0x04)
			return(true);
		return(false);
		}

	//各接続先のウィンドウエラーを全解除
	void ClearErrorAll(void)
		{
		for(int nLoop = 1;nLoop < 256;nLoop++)
			{
			m_var.usedID[nLoop] &= 0x01;
			}
		}

	//通信エラーが発生したIDを記録
	void SetErrorID(uint8_t nID)
		{
		if(isUsedID(nID) & 1)
			m_var.usedID[nID] |= 0x04;
		}

	//指定IDが利用されているか？
	int isUsedID(uint8_t nID);

	//各ウィンドウでのID利用状況を登録
	void SetID(uint8_t nID,bool bUsed = true)
		{
		//注意点
		//	エラーフラグ(bit2)は自動的に解除される
		if(bUsed)
			m_var.usedID[nID] = 1;
		else
			m_var.usedID[nID] = 0;
		}

	//








	DECLARE_MESSAGE_MAP()
};

extern CguicanABH3App	theApp;
extern CA3_FUNCLIST		theABH3;
extern CConfigDlg		theConfig;
