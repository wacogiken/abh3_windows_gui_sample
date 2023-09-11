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

//色付けテキスト
static IDCOLOR g_selectid_textcolor_tbl[] = {
	//beginID					endID					colorIndex
	{IDC_TITLE_SELECTID,		0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_SELECTGROUP,		0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_SELECTTYPE,		0,						APPCOLOR::APPC_NORMAL},
	{0,							0,						APPCOLOR::APPC_NOCOLOR},
	};

//固定テキスト
static IDTEXT1 g_selectid_tbl[] = {
	//ID					textEN					textJP
	{IDC_TITLE_SELECTID,	{_T("ABH3 adrs"),		_T("ABH3 アドレス")}},
	{IDC_TITLE_SELECTGROUP,	{_T("Group number"),	_T("グループ番号")}},
	{IDC_TITLE_SELECTTYPE,	{_T("Device type"),		_T("機種")}},
	{IDOK,					{_T("OK"),				_T("OK")}},
	{IDCANCEL,				{_T("Cancel"),			_T("キャンセル")}},

	{0,						{NULL,					NULL}},
	};


//機種選択肢（この順番の選択肢になる）
static TEXTARRAY g_selectid_type[] = {
	//{textEN			textJP}				value						textvalue
	{{_T("Normal"),		_T("標準")},		(int)MTYPE::MTYPE_NORMAL,	_T("")},
	{{_T("Small"),		_T("小型")},		(int)MTYPE::MTYPE_SMALL,	_T("")},
	{{_T("Host"),		_T("ホスト")},		(int)MTYPE::MTYPE_HOST,		_T("")},
	{{NULL,				NULL},				0,							NULL},
	};

//汎用テキスト
static LANGTEXT  g_selectid_text[] = {
	//textEN						textJP
	{_T("Please select target-ID"),	_T("接続先IDを指定して下さい")},
	{_T("(used)"),					_T("(使用中)")},
	{_T("(hostID)"),				_T("(ホストID)")},
	{_T("Select ID"),				_T("ID選択")},
	{NULL,							NULL},
	};

//
IMPLEMENT_DYNAMIC(CSelectID,CDialogEx)

//コンストラクタ
CSelectID::CSelectID(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECTID,pParent)
	{
	//内部変数初期化
	::ZeroMemory(&m_var,sizeof(IDSET));

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
	DDX_Control(pDX,IDC_SELECTTYPE,m_selecttype);
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

	//
	CString sText("");

	//ダイアログアイテムに固定テキスト設定
	pIDTEXT1 pTbl = g_selectid_tbl;
	while(pTbl->text.pTextEN)
		{
		GetDlgItem(pTbl->nUid)->SetWindowText(theApp.GetLangText(&pTbl->text));
		++pTbl;
		}

	//ウィンドウタイトル
	SetWindowText(theApp.GetLangText(&g_selectid_text[3]));

	//ID選択肢構築
	CreateIDlist();

	//グループ選択肢の構築
	m_selectgroup.ResetContent();
	for(int nLoop = 0;nLoop < 8;nLoop++)
		{
		sText.Format(_T("%d"),nLoop);
		m_selectgroup.AddString(sText);
		}
	m_selectgroup.SetCurSel(0);

	//機種選択肢の構築
	theApp.InitCombobox(&m_selecttype,g_selectid_type);

	//環境設定を取得
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();

	//選択肢の設定を最後の設定に戻す
	m_selectid.SetCurSel(pConfig->nSelectID);
	m_selectgroup.SetCurSel(pConfig->nSelectGroup);
	UpdateType();

	//ボタンの有効無効を設定
	UpdateButton();
	//
	return TRUE;
	}

//現在のID指定に対する「環境設定に保存されている機種」を選択します
void CSelectID::UpdateType()
	{
	//現在選択されているID
	uint8_t nID = (uint8_t)m_selectid.GetCurSel();

	//環境設定で保存されている機種番号
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	uint8_t nType = pConfig->type256[nID];

	//機種番号に対するテーブル要素番号を取得し、選択する
	int nTypeSel = FindValueFromTextArray(g_selectid_type,(int)nType);
	m_selecttype.SetCurSel(nTypeSel);
	}

//ID選択コンボボックスのドロップダウン時に呼び出されます
void CSelectID::OnCbnDropdownSelectid()
	{
	CreateIDlist();
	}

//ID選択の選択肢を構築します
void CSelectID::CreateIDlist()
	{
	CString sText("");

	//現在の選択番号
	int nCurSel = m_selectid.GetCurSel();
	if(nCurSel < 0)
		nCurSel = 0;

	//選択肢の全消去と先頭に選択指示を追加
	m_selectid.ResetContent();
	m_selectid.AddString(theApp.GetLangText(&g_selectid_text[0]));

	//選択肢の構築
	for(uint8_t nID = 1;nID < 255;nID++)
		{
		int nStatus = theApp.isUsedID(nID);
		sText.Format(_T("ID = %d"),nID);
		//USED?
		if(nStatus == 1)
			sText.Format(_T("%s %s"),(LPCTSTR)sText,(LPCTSTR)theApp.GetLangText(&g_selectid_text[1]));
		//HOSTID?
		else if(nStatus == 2)
			sText.Format(_T("%s %s"),(LPCTSTR)sText,(LPCTSTR)theApp.GetLangText(&g_selectid_text[2]));
		m_selectid.AddString(sText);
		}

	//呼び出し時の選択肢を選択
	m_selectid.SetCurSel(nCurSel);
	}

//ID選択が行われた時に呼び出されます
void CSelectID::OnCbnSelchangeSelectid()
	{
	UpdateButton();
	UpdateType();
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
	//接続IDを取得（選択要素番号をそのまま使う）
	int nSelectID = m_selectid.GetCurSel();
	if(nSelectID < 0)
		nSelectID = 0;

	//グループ番号を取得（選択要素番号をそのまま使う）
	int nGroup = m_selectgroup.GetCurSel();
	if(nGroup < 0)
		nGroup = 0;

	//機種設定を取得（選択要素に対する値に変換した物も用意）
	int nTypeSel = m_selecttype.GetCurSel();
	if(nTypeSel < 0)
		nTypeSel = 0;
	MTYPE nType = (MTYPE)g_selectid_type[nTypeSel].nValue;

	//本クラスに今回の設定値を保存（ビューから取得する為）
	SetIDSET((uint8_t)nSelectID,(uint8_t)nGroup,nType,theApp.GetLangText(&g_selectid_type[nTypeSel].text));

	//今回の設定を環境設定へ設定し、さらにシステムへ保存
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	pConfig->nSelectID			= (uint8_t)nSelectID;
	pConfig->nSelectGroup		= (uint8_t)nGroup;
	pConfig->type256[m_var.nID]	= (uint8_t)nType;
	theConfig.reg2sys();

	//ダイアログを閉じて終了
	CDialogEx::OnOK();
	}

//描画色制御の為に呼び出されます
HBRUSH CSelectID::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
	{
	//
	COLORITEM colorItem = GetAppColor(APPCOLOR::APPC_NORMAL);

	//色付け対象か？
	if(DrawCheck(pWnd,colorItem))
		{
		//
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(colorItem.nBack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(colorItem.nBack);
		pDC->SetTextColor(colorItem.nText);
		return(m_brush);
		}

	//標準処理
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC,pWnd,nCtlColor);
	return hbr;
	}

//色制御確認
bool CSelectID::DrawCheck(CWnd* pWnd,COLORITEM& colorItem)
	{
	//ID取得
	UINT nItemID = pWnd->GetDlgCtrlID();

	//固定色表示テーブル
	pIDCOLOR pTbl = g_selectid_textcolor_tbl;
	while(pTbl->nBeginUid)
		{
		if(((pTbl->nBeginUid == nItemID) && (pTbl->nEndUid == 0))
			|| ((pTbl->nBeginUid <= nItemID) && (pTbl->nEndUid >= nItemID)))
			{
			colorItem = GetAppColor(pTbl->index);
			return(true);
			}
		++pTbl;
		}
	return(false);
	}

