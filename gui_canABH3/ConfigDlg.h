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

#include "pch.h"
#include <stdint.h>
#include "resource.h"


class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:

	//
	typedef struct _TBL_CONFIG
		{
		LANGTEXT	text;
		uint32_t	nValue;			//数値としての値
		TCHAR*		pValue;			//文字列としての値
		} TBL_CONFIG,*pTBL_CONFIG;

	//設定用構造体（直接ポインタを操作する場合が有るのでpublic）
	typedef struct _CONFIGDLG_CONFIG
		{
		//ダイアログ初期化時のDLL設定（変化を調べる為に保存）
		uint8_t		nOldDLL;
		uint8_t		nOldInterval;
		uint8_t		nOldLanguage;

		//環境設定で設定する要素
		uint8_t		nDLL;			//DLL番号
		uint8_t		nDLLoption;		//USB-to-CAN v2の場合はケーブル番号、WACOCANの場合はCOMポート番号、どちらも0開始
		uint8_t		nHostID;		//ホストのID
		uint8_t		nBaudrate;		//ボーレート(選択肢番号)
		uint8_t		nType;			//機種設定(0..標準  1..小型)
		uint8_t		nBr;			//ブロードキャスト送信(0..接続先のみ  1..全ABH3)
		uint8_t		nInterval;		//割り込み周期(0..100[ms]  1..31[ms]  2..15[ms])
		uint8_t		nLanguage;		//0..英語  1..日本語

		//他の場所で設定する要素
		uint8_t		nSelectID;		//ウィンドウを開く時のダイアログで使うID
		uint8_t		nSelectGroup;	//ウィンドウを開く時のダイアログで使うグループ
		uint8_t		nRecvCounter;	//受信カウンタ表示
		uint8_t		nFPS;			//フレームレート表示

		//保存しないが一時的に格納する要素
		uint8_t		nRestart;		//設定変更に伴うソフト再起動要求

		} CONFIGDLG_CONFIG,*pCONFIGDLG_CONFIG;

protected:
	//
	CONFIGDLG_CONFIG m_config;

	//描画用ブラシ
	CBrush m_brush;

	//環境設定を表示物に設定
	void reg2disp(void);

	//表示を環境設定に取り込み
	void disp2reg(void);

	//画面構築
	void initScreen(void);

	//テーブルを元にコンボボックスを初期化
	void InitCombobox(CComboBox* pCombo,pTBL_CONFIG pTbl);

	//DLLオプションの選択肢を構築する
	void CreateDLLoption(CComboBox* pCombo,int nMode,int nSetSel = -1);

public:
	//
	CComboBox m_dll;
	CComboBox m_dlloption;
	CComboBox m_hostid;
	CComboBox m_baudrate;
	CComboBox m_type;
	CComboBox m_br;
	CComboBox m_interval;
	CComboBox m_language;


	//環境設定をシステムに保存
	void reg2sys(void);

	//システムから環境設定を読み込み
	void sys2reg(void);

	//環境設定変数へのポインタ取得
	pCONFIGDLG_CONFIG GetConfig(void)
		{
		return(&m_config);
		}

	//DLLファイル名を取得
	TCHAR* getDllname(void);

	//DLLの番号を取得
	uint8_t getDll(void);

	//DLLオプション番号を取得
	uint8_t getDllOption(void);

	//ホスト設定番号を取得
	uint8_t getHostid(void);

	//ボーレート設定値[Kbps]取得
	uint32_t getBaudrate(void);

	//機種設定を取得
	uint8_t getABH3type(void);

	//(保留中)ブロードキャスト送信設定を取得
	uint8_t getBr(void);

	//周期設定を取得
	uint32_t getInterval(void);

	//再起動を要求されているか？
	bool IsRestartRequire(void)
		{
		return((m_config.nRestart != 0) ? true : false);
		}

public:
	CConfigDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CConfigDlg();
	enum { IDD = IDD_CONFIG };
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
	afx_msg void OnCbnDropdownConfigDll();
};
