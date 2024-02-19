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
	typedef enum
		{
		MAX_DLLCOUNT	= 32
		} ENUM;

	//指令関連
	typedef struct _CFG_ORDER
		{
		float		nA;				//指令値A（ユーザーの入力値そのまま）
		float		nB;				//指令値B（ユーザーの入力値そのまま）
		uint8_t		nType;			//指令切り替え（0..速度  1..トルク)
		} CFG_ORDER,*pCFG_ORDER;

	//設定用構造体（直接ポインタを操作する場合が有るのでpublic）
	typedef struct _CONFIGDLG_CONFIG
		{
		//ダイアログ初期化時のDLL設定（変化を調べる為に保存）
		int			nOldDLLnum;
//		TCHAR		sOldDLLname[32];
		//uint8_t		nOldDLL;
		uint8_t		nOldLanguage;

		//環境設定で設定する要素

		TCHAR		sDLLname[32];		//選択されたDLLファイル名
		uint8_t		nDLLoption;			//DLLオプション（ケーブル番号等）

		//uint8_t		nDLL;				//DLL番号
		//代替
		//uint8_t		nDLLoption2[16];	//DLL番号に対応したオプション値（初期値0）

		uint8_t		nHostID;			//ホストのID
		uint8_t		nBaudrate;			//ボーレート(選択肢番号)
		uint8_t		nLanguage;			//0..英語  1..日本語
		uint8_t		nRawdata;			//0..非表示　1..表示

		//他の場所で設定する要素
		uint8_t		nSelectID;			//ウィンドウを開く時のダイアログで使うID(0..ID1)
		uint8_t		nSelectGroup;		//ウィンドウを開く時のダイアログで使うグループ(0..GROUP0)
		uint8_t		nCanRatio;			//CAN-bus利用率表示
		uint8_t		nTransmitCounter;	//送受信カウンタ表示
		uint8_t		type256[256];		//機種設定(0..標準  1..小型)、要素1がID1なので要素0は使わない
		uint32_t	send256[256];		//周期転送フラグ(ビット単位)
		uint32_t	interval256[256];	//周期時間[ms]
		CFG_ORDER	order256[256];		//指令関連
		//保存しないが一時的に格納する要素
		uint8_t		nRestart;			//設定変更に伴うソフト再起動要求
		} CONFIGDLG_CONFIG,*pCONFIGDLG_CONFIG;

	//
	typedef struct _DLLTABLE_SGL
		{
		TCHAR	sFilename[32];			//ファイル名
		TCHAR	sDispname[64];			//表示用名称
		uint8_t	nWarning;				//警告表示（bit0..ボーレート設定)
		uint8_t	nUseCOM;				//オプションはCOMポート番号？
		} DLLTABLE_SGL,*pDLLTABLE_SGL;
	DLLTABLE_SGL m_dlltable[ENUM::MAX_DLLCOUNT];


protected:
	//内部変数へのポインタ（動的確保）
	pCONFIGDLG_CONFIG m_pConfig;

	//描画用ブラシ
	CBrush m_brush;

	//DLL検索用フォルダ指定
	CString m_dllfolder;

	//環境設定を表示物に設定
	void reg2disp(void);

	//表示を環境設定に取り込み
	void disp2reg(void);

	//画面構築
	void initScreen(void);

	//テーブルを元にコンボボックスを初期化
	//void InitCombobox(CComboBox* pCombo,pTBL_CONFIG pTbl);

	//DLLオプションの選択肢を構築する
	void CreateDLLoption(CComboBox* pCombo,int nUseCOM);

	//DLL選択による注意書き表示設定
	void CreateNotice(int nMode);

	//
	CString fullname2filename(CString sFullname)
		{
		PTCHAR pFilename = new TCHAR[_MAX_FNAME]();
		PTCHAR pExt = new TCHAR[_MAX_EXT]();
		_tsplitpath_s(sFullname,NULL,0,NULL,0,pFilename,_MAX_FNAME,pExt,_MAX_EXT);
		CString sResult = CString(pFilename) + CString(pExt);
		delete[] pExt;
		delete[] pFilename;
		return(sResult);
		}

	//
	bool filename2dllinfo(CString& sDispname,uint8_t& nWarning,uint8_t& nUseCOM,CString sFilename);



public:
	//
	CComboBox m_dll;
	CComboBox m_dlloption;
	CComboBox m_hostid;
	CComboBox m_baudrate;
	CComboBox m_type;
	CComboBox m_language;
	CComboBox m_rawdata;

	//DLLを探すフォルダを指定
	bool SetDLLfolder(CString sFolder)
		{
		//末尾にパス記号が無い場合は追加する
		if(sFolder.Right(1) != _T("\\"))
			sFolder += _T("\\");
		//格納
		m_dllfolder = sFolder;
		return(GetDLLlist());
		}

	//DLLを探すフォルダを取得
	CString GetDLLfolder(void)
		{
		return(m_dllfolder);
		}

	//DLLを探して一覧を作成
	bool GetDLLlist(void);

	//環境設定をシステムに保存
	void reg2sys(void);

	//システムから環境設定を読み込み
	void sys2reg(void);

	//環境設定変数へのポインタ取得
	pCONFIGDLG_CONFIG GetConfig(void)
		{
		return(m_pConfig);
		}

	//DLLファイル名を取得
	CString getDllname(void);

	//DLLの番号を取得
	//uint8_t getDll(void);

	//DLLオプション番号を取得
	uint8_t getDllOption(void);

	//ホスト設定番号を取得
	uint8_t getHostid(void);

	//ボーレート設定値[Kbps]取得
	uint32_t getBaudrate(void);

	//周期設定を取得
	//uint32_t getInterval(void);

	//再起動を要求されているか？
	bool IsRestartRequire(void)
		{
		return((m_pConfig->nRestart != 0) ? true : false);
		}

	//現在指定されているDLLに対して、表示用名称を取得
	CString GetDispnameFromDLL(void)
		{
		pDLLTABLE_SGL pCurrentInfo = GetCurrentDLLinfo();
		if(pCurrentInfo == NULL)
			return(CString(m_pConfig->sDLLname));	//該当無しは現在のファイル名が戻る
		return(CString(pCurrentInfo->sDispname));
		}

	//現在指定されているDLLに対して、Warningフラグを取得
	uint8_t GetWarningFromDLL(void)
		{
		pDLLTABLE_SGL pCurrentInfo = GetCurrentDLLinfo();
		if(pCurrentInfo == NULL)
			return(0);
		return(pCurrentInfo->nWarning);
		}

	//現在指定されているDLLに対して、UseCOMフラグを取得
	uint8_t GetUseCOMfromDLL(void)
		{
		pDLLTABLE_SGL pCurrentInfo = GetCurrentDLLinfo();
		if(pCurrentInfo == NULL)
			return(0);
		return(pCurrentInfo->nUseCOM);
		}

	//現在指定されているDLLに対して、情報を取得
	pDLLTABLE_SGL GetCurrentDLLinfo(void)
		{
		int nNum = GetCurrentDLLnum();
		if(nNum < 0)
			return(NULL);
		return(&m_dlltable[nNum]);
		}

	//現在ファイル名が指定されているDLLに対して、テーブルの要素番号を取得
	int GetCurrentDLLnum(void)
		{
		CString sCurrent(m_pConfig->sDLLname);
		sCurrent.MakeLower();
		for(int nLoop = 0;nLoop < ENUM::MAX_DLLCOUNT;nLoop++)
			{
			CString sTable(m_dlltable[nLoop].sFilename);
			sTable.MakeLower();
			if(sCurrent == sTable)
				return(nLoop);
			}
		return(-1);
		}

	//RAWDATA表示機能が有効か？
	bool IsRawdata(void)
		{
		return((bool)(m_pConfig->nRawdata != 0));	
		}


public:
	CConfigDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CConfigDlg();
	enum { IDD = IDD_CONFIG };
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedSave();
	virtual BOOL CanShowOnTaskBarTabs()
		{
		return(FALSE);
		}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
	afx_msg void OnCbnDropdownConfigDll();
};
