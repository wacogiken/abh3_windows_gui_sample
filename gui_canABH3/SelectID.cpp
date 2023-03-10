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

#include "pch.h"
#include "gui_canABH3.h"
#include "SelectID.h"
#include "afxdialogex.h"

//色テーブル
static IDCOLOR g_selectid_color_tbl[] = {
	//colorIndex		textColor			backColor
	{0,					COLOR_WHITE,		COLOR_BLUE},
	{1,					COLOR_WHITE,		COLOR_DARKGREEN},
	{2,					COLOR_WHITE,		COLOR_DARKBLUE},
	{3,					COLOR_WHITE,		COLOR_DARKMAGENTA},
	{4,					COLOR_WHITE,		COLOR_SKYBLUE},
	{5,					COLOR_WHITE,		COLOR_GREEN},
	{6,					COLOR_BLACK,		COLOR_YELLOW},
	{7,					COLOR_WHITE,		COLOR_RED},
	{8,					COLOR_WHITE,		COLOR_BLACK},
	{9,					COLOR_BLACK,		COLOR_WHITE},
	{-1,				0,					0},
	};

//色付けテキスト
static IDCOLOR1 g_selectid_textcolor_tbl[] = {
	//beginID					endID					colorIndex
	{IDC_TITLE_SELECTID,		0,						0},
	{IDC_TITLE_SELECTGROUP,		0,						0},
	{IDC_TITLE_SELECTINTERVAL,	0,						0},
	{0,							0,						0},
	};

//固定テキスト
static IDTEXT1 g_selectid_tbl[] = {
	//ID					{textEN					textJP}
	{IDC_TITLE_SELECTID,	{_T("ABH3 adrs"),		_T("ABH3 アドレス")}},
	{IDC_TITLE_SELECTGROUP,	{_T("Group number"),	_T("グループ番号")}},
	{0xffffffff,			{_T("Select ID"),		_T("ID選択")}},
	{0,						{NULL,					NULL}},
	};

//周期設定の選択肢（一部数字に補正有り）
static CConfigDlg::TBL_CONFIG g_selectid_interval[] = {
	//{textEN			textJP}							value		textvalue
	{{_T(" 250.0[ms]          "),	NULL},				250,		_T("250")},
	{{_T(" 125.0[ms]          "),	NULL},				125,		_T("125")},
	{{_T(" 100.0[ms]  (10 FPS)"),	NULL},				100,		_T("100")},
	{{_T("  66.6[ms]  (15 FPS)"),	NULL},				60,			_T("60")},
	{{_T("  33.3[ms]  (30 FPS)"),	NULL},				30,			_T("30")},
	{{_T("  16.6[ms]  (60 FPS)"),	NULL},				15,			_T("15")},
	{{NULL,							NULL},				0,			NULL},
	};

//
IMPLEMENT_DYNAMIC(CSelectID,CDialogEx)

//コンストラクタ
CSelectID::CSelectID(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECTID,pParent)
	{
	//内部変数初期化
	::ZeroMemory(&m_var,sizeof(SELECTID_VAR));
	//描画用ブラシ構築
	m_brush.CreateSolidBrush(COLOR_BLACK);
	}

//デストラクタ
CSelectID::~CSelectID()
	{
	//描画用ブラシ解放
	m_brush.DeleteObject();
	}

//DDE
void CSelectID::DoDataExchange(CDataExchange* pDX)
	{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SELECTID,m_selectid);
	DDX_Control(pDX,IDC_SELECTGROUP,m_selectgroup);
	DDX_Control(pDX,IDC_SELECTINTERVAL,m_selectinterval);
	}

//メッセージMAP
BEGIN_MESSAGE_MAP(CSelectID,CDialogEx)
	ON_CBN_DROPDOWN(IDC_SELECTID,&CSelectID::OnCbnDropdownSelectid)
	ON_CBN_SELCHANGE(IDC_SELECTID,&CSelectID::OnCbnSelchangeSelectid)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//ダイアログ初期化時に呼び出されます
BOOL CSelectID::OnInitDialog()
	{
	CDialogEx::OnInitDialog();

	//固定テキスト
	int nPt = 0;
	while(g_selectid_tbl[nPt].nUid)
		{
		if(g_selectid_tbl[nPt].nUid != 0xffffffff)
			GetDlgItem(g_selectid_tbl[nPt].nUid)->SetWindowText(theApp.GetLangText(&g_selectid_tbl[nPt].text));
		else
			SetWindowText(theApp.GetLangText(&g_selectid_tbl[nPt].text));
		++nPt;
		}

	//ID選択肢構築
	CreateIDlist();

	//グループ選択肢の構築
	m_selectgroup.ResetContent();
	for(int nLoop = 0;nLoop < 8;nLoop++)
		{
		CString sText("");
		sText.Format(_T("%d"),nLoop);
		m_selectgroup.AddString(sText);
		}
	m_selectgroup.SetCurSel(0);

	//更新周期設定の選択肢構築
	CreateIntervalList();

	//IDとグループは、最後に選択した位置を設定
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	m_selectid.SetCurSel(pConfig->nSelectID);
	m_selectgroup.SetCurSel(pConfig->nSelectGroup);

	//周期設定は、環境設定の値をデフォルトとする
	SetIntervalList(theConfig.getInterval());

	//ボタンの有効無効を設定
	UpdateButton();
	//
	return TRUE;
	}

//更新周期設定の選択肢構築
void CSelectID::CreateIntervalList()
	{
	//環境設定から「数値」で周期を取得する
	uint32_t nInterval = theConfig.getInterval();

	//選択肢を構築する
	m_selectinterval.ResetContent();
	int nPt = 0;
	while(g_selectid_interval[nPt].text.pTextEN)
		{
		CConfigDlg::TBL_CONFIG item = g_selectid_interval[nPt];
		m_selectinterval.AddString(theApp.GetLangText(&item.text));
		++nPt;
		}
	m_selectinterval.SetCurSel(0);
	}

//更新周期設定を周期時間指定して選択
bool CSelectID::SetIntervalList(int nInterval1,int nInterval2 /* 100 */)
	{
	int nPt = 0;
	while(g_selectid_interval[nPt].text.pTextEN)
		{
		CConfigDlg::TBL_CONFIG item = g_selectid_interval[nPt];
		if(item.nValue == nInterval1)
			{
			m_selectinterval.SetCurSel(nPt);
			return(true);
			}
		++nPt;
		}
	if(nInterval1 != nInterval2)
		{
		if(SetIntervalList(nInterval2,nInterval2))
			return(true);
		}
	//両方の周期指定に該当する物が無い
	m_selectinterval.SetCurSel(0);
	//
	return(false);
	}

//ID選択コンボボックスのドロップダウン時に呼び出されます
void CSelectID::OnCbnDropdownSelectid()
	{
	CreateIDlist();
	}

//ID選択の選択肢を構築します
void CSelectID::CreateIDlist()
	{
	//現在の選択番号
	int nCurSel = m_selectid.GetCurSel();
	if(nCurSel < 0)
		nCurSel = 0;

	//選択肢の全消去
	m_selectid.ResetContent();
	m_selectid.AddString(_T("接続先IDを指定して下さい"));

	//選択肢の構築
	CString sText("");
	for(uint8_t nID = 1;nID < 255;nID++)
		{
		int nStatus = theApp.isUsedID(nID);
		sText.Format(_T("ID = 0x%02X"),nID);
		if(nStatus == 1)
			sText.Format(_T("%s (使用中)"),(LPCTSTR)sText);
		else if(nStatus == 2)
			sText.Format(_T("%s (ホストID)"),(LPCTSTR)sText);
		m_selectid.AddString(sText);
		}

	//呼び出し時の選択肢を選択
	m_selectid.SetCurSel(nCurSel);
	}

//ID選択が行われた時に呼び出されます
void CSelectID::OnCbnSelchangeSelectid()
	{
	UpdateButton();
	}

//ボタンの有効無効を設定します
void CSelectID::UpdateButton()
	{
	//現在の選択肢番号を取得
	int nCurSel = m_selectid.GetCurSel();
	//選択されてない？（範囲外の扱い）
	if(nCurSel < 0)
		{
		//先頭要素を選択する
		nCurSel = 0;
		m_selectid.SetCurSel(nCurSel);
		}

	//先頭要素、又は既に使用済みID？
	if((nCurSel == 0) || (theApp.isUsedID(uint8_t(nCurSel))))
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	//上記以外は未使用扱い
	else
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}

//「OK」が押された時に呼び出されます
void CSelectID::OnOK()
	{
	//接続対象を取得
	int nSelectID = m_selectid.GetCurSel();
	if(nSelectID < 0)
		nSelectID = 0;
	//グループ番号を取得
	int nGroup = m_selectgroup.GetCurSel();
	if(nGroup < 0)
		nGroup = 0;
	//周期設定を取得
	int nInterval = m_selectinterval.GetCurSel();
	if(nInterval < 0)
		nInterval = 0;

	//本クラスに設定値を保存
	m_var.nSelectID = (uint8_t)nSelectID;
	m_var.nGroup = (uint8_t)nGroup;
	m_var.nInterval = g_selectid_interval[nInterval].nValue;

	//接続対象とグループ番号は環境設定へ設定し、さらにシステムへ保存
	//周期設定は保存しない（環境設定の値以外を適用したい用途を想定）
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	pConfig->nSelectID = m_var.nSelectID;
	pConfig->nSelectGroup = m_var.nGroup;
	theConfig.reg2sys();

	//ダイアログを閉じて終了
	CDialogEx::OnOK();
	}

//描画色制御の為に呼び出されます
HBRUSH CSelectID::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
	{
	bool bDraw = false;
	COLORREF nTextColor = COLOR_BLACK;
	COLORREF nBackColor = COLOR_WHITE;

	//色付け対象か？
	if(DrawCheck(pWnd,nTextColor,nBackColor))
		{
		//
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(nBackColor);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(nBackColor);
		pDC->SetTextColor(nTextColor);
		return(m_brush);
		}

	//標準処理
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC,pWnd,nCtlColor);
	return hbr;
	}

//色制御確認
bool CSelectID::DrawCheck(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor)
	{
	//ID取得
	UINT nUid = pWnd->GetDlgCtrlID();

	//固定色表示テーブル
	int nLoop = 0;
	while(-1)
		{
		IDCOLOR1 info = g_selectid_textcolor_tbl[nLoop];
		if(info.nBeginUid == 0)
			break;
		else if(((info.nBeginUid == nUid) && (info.nEndUid == 0)) || ((info.nBeginUid <= nUid) && (info.nEndUid >= nUid)))
			{
			nTextColor = g_selectid_color_tbl[info.nColorIndex].nText;
			nBackColor = g_selectid_color_tbl[info.nColorIndex].nBack;
			return(true);
			}
		++nLoop;
		}
	return(false);
	}
