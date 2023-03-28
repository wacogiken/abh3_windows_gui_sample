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
	//ID					{textEN					textJP}
	{IDC_TITLE_SELECTID,	{_T("ABH3 adrs"),		_T("ABH3 アドレス")}},
	{IDC_TITLE_SELECTGROUP,	{_T("Group number"),	_T("グループ番号")}},
	{IDC_TITLE_SELECTTYPE,	{_T("Device type"),		_T("機種")}},
	{0xffffffff,			{_T("Select ID"),		_T("ID選択")}},		//ダイアログタイトル
	{0,						{NULL,					NULL}},
	};

//機種選択肢
static CConfigDlg::TBL_CONFIG g_selectid_type[] = {
	//{textEN			textJP}				value		textvalue
	{{_T("Normal"),		_T("標準")},		0,			_T("0")},
	{{_T("Small"),		_T("小型")},		1,			_T("1")},
	{{NULL,				NULL},				0,			NULL},
	};

//メッセージ類
static LANGTEXT  g_selectid_text[] = {
	//textEN						textJP
	{_T("Please select target-ID"),	_T("接続先IDを指定して下さい")},
	{_T("(used)"),					_T("(使用中)")},
	{_T("(hostID)"),				_T("(ホストID)")},
	{NULL,							NULL},
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

	//機種選択肢の構築
	m_selecttype.ResetContent();
	nPt = 0;
	while(g_selectid_type[nPt].text.pTextEN)
		{
		CConfigDlg::pTBL_CONFIG pItem = &g_selectid_type[nPt];
		if(pItem->text.pTextEN == NULL)
			break;
		m_selecttype.AddString(theApp.GetLangText(&pItem->text));
		++nPt;
		}
	m_selecttype.SetCurSel(0);

	//最後に選択した位置を設定
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	m_selectid.SetCurSel(pConfig->nSelectID);
	m_selectgroup.SetCurSel(pConfig->nSelectGroup);

	//IDに対応した機種を再設定
	UpdateType();

	//ボタンの有効無効を設定
	UpdateButton();
	//
	return TRUE;
	}

//機種の選択状態を更新します
void CSelectID::UpdateType()
	{
	int nSel = m_selectid.GetCurSel();
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	m_selecttype.SetCurSel(pConfig->type256[nSel]);
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
	//接続対象を取得
	int nSelectID = m_selectid.GetCurSel();
	if(nSelectID < 0)
		nSelectID = 0;
	//グループ番号を取得
	int nGroup = m_selectgroup.GetCurSel();
	if(nGroup < 0)
		nGroup = 0;
	//機種設定w取得
	int nType = m_selecttype.GetCurSel();
	if(nType < 0)
		nType = 0;
	//本クラスに設定値を保存
	m_var.nSelectID = (uint8_t)nSelectID;
	m_var.nGroup = (uint8_t)nGroup;
	m_var.nType = (uint8_t)nType;
	//接続対象、グループ番号、機種は環境設定へ設定し、さらにシステムへ保存
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	pConfig->nSelectID = m_var.nSelectID;
	pConfig->nSelectGroup = m_var.nGroup;
	pConfig->type256[pConfig->nSelectID] = m_var.nType;
	theConfig.reg2sys();

	//ダイアログを閉じて終了
	CDialogEx::OnOK();
	}

//描画色制御の為に呼び出されます
HBRUSH CSelectID::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
	{
	bool bDraw = false;
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
	UINT nUid = pWnd->GetDlgCtrlID();

	//固定色表示テーブル
	int nLoop = 0;
	while(-1)
		{
		IDCOLOR info = g_selectid_textcolor_tbl[nLoop];
		if(info.nBeginUid == 0)
			break;
		else if(((info.nBeginUid == nUid) && (info.nEndUid == 0)) || ((info.nBeginUid <= nUid) && (info.nEndUid >= nUid)))
			{
			colorItem = GetAppColor(info.index);
			return(true);
			}
		++nLoop;
		}
	return(false);
	}

//機種名を取得します
CString CSelectID::GetTypeName()
	{
	int nPt = 0;
	uint8_t nType = m_var.nType;
	while(-1)
		{
		CConfigDlg::TBL_CONFIG item = g_selectid_type[nPt];
		if(item.text.pTextEN == NULL)
			break;
		else if(nType == 0)
			return(theApp.GetLangText(&item.text));
		--nType;
		++nPt;
		}
	//範囲外は先頭要素を戻す
	return(theApp.GetLangText(&g_selectid_type[0].text));
	}
