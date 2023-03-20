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
#include "ConfigDlg.h"
#include "afxdialogex.h"

//
typedef enum _ENUM_CONFIG
	{
	DLLOPTION	= 0,
	} ENUM_CONFIG;


//固定表示文字列
static IDTEXT1 g_config_title[] = {
	//ID			{textEN					testJP}
	{IDC_TITLE1,	{_T("I/F select"),		_T("I/F選択")}},
	{IDC_TITLE2,	{_T("I/F ootion"),		_T("I/F番号指定")}},
	{IDC_TITLE3,	{_T("Host adrs"),		_T("ホストアドレス")}},
	{IDC_TITLE4,	{_T("Baudrate"),		_T("ボーレート")}},
	//{IDC_TITLE5,	{_T("Target type"),		_T("機種設定")}},
	{IDC_TITLE6,	{_T("Broadcast mode"),	_T("一括送信設定")}},	//保留
	{IDC_TITLE7,	{_T("Update cycle"),	_T("更新周期")}},
	{IDC_TITLE13,	{_T("Language"),		_T("表示言語")}},
	{IDC_SAVE,		{_T("Set"),				_T("設定")}},
	{IDCANCEL,		{_T("Cancel"),			_T("キャンセル")}},
	{0,				{NULL,					NULL}},
	};

//その他文字列
static LANGTEXT g_config_text[] = {
	//textEN	textJP
	{_T("%d"),
				_T("%d 本目")},
	{_T("Interface settings have been changed. Restart this software."),
				_T("インターフェース設定が変更された為、本ソフトを再起動します")},
	{_T("To change the update cycle, the target window must be reopened."),
				_T("更新周期の変更は、対象ウィンドウを開き直す必要が有ります")},
	{_T("language settings have been changed. Restart this software."),
				_T("言語設定が変更された為、本ソフトを再起動します")},
	{NULL,
				NULL},
	};
	
//言語選択肢
static CConfigDlg::TBL_CONFIG g_config_language[] = {
	//textEN			textJP				value		textvalue
	{{_T("English"),	_T("英語")},		0,			_T("0")},
	{{_T("Japanese"),	_T("日本語")},		1,			_T("1")},
	{{NULL,				NULL},				0,			NULL},
	};

//DLL選択肢
static CConfigDlg::TBL_CONFIG g_config_dll[] = {
	//textEN						textJP		value		textvalue
	{{_T("IXXAT USB-to-CAN V2"),	NULL},		0,		_T("CANabh3.dll")},
	{{_T("WACOGIKEN usbcan"),		NULL},		1,		_T("canwacousb.dll")},
	{{NULL,							NULL},		0,		NULL},
	};

//ボーレート選択肢
static CConfigDlg::TBL_CONFIG g_config_baudrate[] = {
	//{textEN			textJP}		value		textvalue
	{{_T("1000[Kbps]"),	NULL},		1000,		_T("1000")},
	{{_T(" 800[Kbps]"),	NULL},		800,		_T("800")},
	{{_T(" 500[Kbps]"),	NULL},		500,		_T("500")},
	{{_T(" 250[Kbps]"),	NULL},		250,		_T("250")},
	{{_T(" 125[Kbps]"),	NULL},		125,		_T("125")},
	{{_T(" 100[Kbps]"),	NULL},		100,		_T("100")},
	{{_T("  50[Kbps]"),	NULL},		50,			_T("50")},
	{{_T("  20[Kbps]"),	NULL},		20,			_T("20")},
	{{_T("  10[Kbps]"),	NULL},		10,			_T("10")},
	{{NULL,				NULL},		0,			NULL},
	};

//送信モード選択肢
static CConfigDlg::TBL_CONFIG g_config_br[] = {
	//{textEN			textJP}				value		textvalue
	{{_T("Target only"),_T("接続先のみ")},	0,			_T("0")},
	{{_T("All"),		_T("全機器")},		1,			_T("1")},
	{{NULL,				NULL},				0,			NULL},
	};

//更新周期選択肢
static CConfigDlg::TBL_CONFIG g_config_interval[] = {
	//{textEN					textJP}		value		textvalue
	{{_T("100.0[ms] (10 FPS)"),	NULL},		100,		_T("100")},
	{{_T(" 33.3[ms] (30 FPS)"),	NULL},		30,			_T("30")},
	{{_T(" 15.6[ms] (60 FPS)"),	NULL},		15,			_T("15")},
	{{NULL,						NULL},		0,			NULL},
	};

//
IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)


//
CConfigDlg::CConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFIG, pParent)
	{
	//設定領域の構築
	m_pConfig = new CONFIGDLG_CONFIG();
	//::ZeroMemory(&m_config,sizeof(CONFIGDLG_CONFIG));

	//描画用ブラシの構築
	m_brush.CreateSolidBrush(COLOR_BLACK);
	}

//
CConfigDlg::~CConfigDlg()
	{
	//設定領域の解放
	delete m_pConfig;
	//描画用ブラシの解放
	m_brush.DeleteObject();
	}

//DDE
void CConfigDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CONFIG_DLL,m_dll);
	DDX_Control(pDX,IDC_CONFIG_CABLE,m_dlloption);
	DDX_Control(pDX,IDC_CONFIG_HOSTID,m_hostid);
	DDX_Control(pDX,IDC_CONFIG_BAUDRATE,m_baudrate);
	DDX_Control(pDX,IDC_CONFIG_TYPE,m_type);
	DDX_Control(pDX,IDC_CONFIG_BR,m_br);
	DDX_Control(pDX,IDC_CONFIG_INTERVAL,m_interval);
	DDX_Control(pDX,IDC_CONFIG_LANGUAGE,m_language);
	}

//メッセージマップ
BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SAVE,&CConfigDlg::OnBnClickedSave)
	ON_WM_CTLCOLOR()
	ON_CBN_DROPDOWN(IDC_CONFIG_DLL,&CConfigDlg::OnCbnDropdownConfigDll)
	ON_CBN_SELCHANGE(IDC_CONFIG_DLL,&CConfigDlg::OnCbnDropdownConfigDll)
END_MESSAGE_MAP()

//ダイアログ初期化時に呼び出されます
BOOL CConfigDlg::OnInitDialog()
	{
	CDialogEx::OnInitDialog();

	//後で比較が必要な要素は、この時点の設定を保存しておく
	m_pConfig->nOldDLL = m_pConfig->nDLL;
	m_pConfig->nOldInterval = m_pConfig->nInterval;
	m_pConfig->nOldLanguage = m_pConfig->nLanguage;

	//画面構築
	initScreen();

	//
	return TRUE;
	}

//画面構築
void CConfigDlg::initScreen()
	{
	CString sText("");

	//固定文字列設定
	int nLoop = 0;
	while(-1)
		{
		IDTEXT1 item = g_config_title[nLoop];
		if(item.nUid == 0)
			break;
		GetDlgItem(item.nUid)->SetWindowText(theApp.GetLangText(&item.text));
		++nLoop;
		}

	//選択肢構築
	//	DLLオプションは、m_dll選択状態に対して動的構築する
	//	ホストIDはテーブルを使わない
	InitCombobox(&m_dll,g_config_dll);
	InitCombobox(&m_baudrate,g_config_baudrate);
	InitCombobox(&m_br,g_config_br);
	InitCombobox(&m_interval,g_config_interval);
	InitCombobox(&m_language,g_config_language);

	//選択肢構築・ホストID
	m_hostid.ResetContent();
	for(nLoop = 1;nLoop < 0xff;nLoop++)
		{
		sText.Format(_T("ID = 0x%02X (%d)"),nLoop,nLoop);
		m_hostid.AddString(sText);
		}

	//環境設定を表示物に設定
	reg2disp();

	//選択肢構築・DLLオプション
	CreateDLLoption(&m_dlloption,m_pConfig->nDLL,m_pConfig->nDLLoption);
	}

//
void CConfigDlg::OnCbnDropdownConfigDll()
	{
	int nSel = m_dll.GetCurSel();
	CreateDLLoption(&m_dlloption,nSel);
	}

//
void CConfigDlg::CreateDLLoption(CComboBox* pCombo,int nMode,int nSetSel /* -1 */)
	{
	//現在の選択番号を取得
	int nSel = pCombo->GetCurSel();
	//選択肢全消去
	pCombo->ResetContent();
	//構築
	CString sText("");
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		if(nMode == 0)
			{
			CString sFmt(theApp.GetLangText(&g_config_text[ENUM_CONFIG::DLLOPTION]));
			sText.Format((LPCTSTR)sFmt,nLoop + 1);
			}
		else
			sText.Format(_T("COM%d"),nLoop + 1);
		pCombo->AddString(sText);
		}

	//
	if(nSetSel >= 0)
		pCombo->SetCurSel(nSetSel);
	else
		{
		//元の選択に戻す
		pCombo->SetCurSel(nSel);
		}
	}

//テーブルを元にコンボボックスを初期化
void CConfigDlg::InitCombobox(CComboBox* pCombo,pTBL_CONFIG pTbl)
	{
	pCombo->ResetContent();
	int nLoop = 0;
	while(pTbl[nLoop].text.pTextEN)
		{
		pCombo->AddString(theApp.GetLangText(&pTbl[nLoop].text));
		++nLoop;
		}
	pCombo->SetCurSel(0);
	}

//システムから環境設定を取得
void CConfigDlg::sys2reg()
	{
	//

	CString sSection("base"),sItem("");
	//環境設定の項目
	m_pConfig->nDLL				= (uint8_t)theApp.GetProfileInt(sSection,_T("dll"),0x0);
	m_pConfig->nDLLoption		= (uint8_t)theApp.GetProfileInt(sSection,_T("dlloption"),0x0);
	m_pConfig->nHostID			= (uint8_t)theApp.GetProfileInt(sSection,_T("hostid"),0);
	m_pConfig->nBaudrate		= (uint8_t)theApp.GetProfileInt(sSection,_T("baudrate"),0);
	m_pConfig->nBr				= (uint8_t)theApp.GetProfileInt(sSection,_T("br"),0);
	m_pConfig->nInterval		= (uint8_t)theApp.GetProfileInt(sSection,_T("interval"),0);
	m_pConfig->nLanguage		= (uint8_t)theApp.GetProfileInt(sSection,_T("language"),0);
	//その他項目
	m_pConfig->nSelectID		= (uint8_t)theApp.GetProfileInt(sSection,_T("selectid"),0x0);
	m_pConfig->nSelectGroup		= (uint8_t)theApp.GetProfileInt(sSection,_T("selectgroup"),0x0);
	m_pConfig->nRecvCounter		= (uint8_t)theApp.GetProfileInt(sSection,_T("recvcounter"),0x0);
	m_pConfig->nFPS				= (uint8_t)theApp.GetProfileInt(sSection,_T("framerate"),0x0);
	for(int nLoop = 0;nLoop < 256;nLoop++)
		{
		//機種設定
		sItem.Format(_T("type%d"),nLoop);
		m_pConfig->type256[nLoop] = (uint8_t)theApp.GetProfileInt(sSection,sItem,0);
		//周期転送設定
		sItem.Format(_T("send%d"),nLoop);
		m_pConfig->send256[nLoop] = (uint8_t)theApp.GetProfileInt(sSection,sItem,0);
		}
	}

//現在の環境設定をシステムに保存
void CConfigDlg::reg2sys()
	{
	//
	CString sSection("base"),sItem("");
	//環境設定の項目
	theApp.WriteProfileInt(sSection,_T("dll"),int(m_pConfig->nDLL));
	theApp.WriteProfileInt(sSection,_T("dlloption"),int(m_pConfig->nDLLoption));
	theApp.WriteProfileInt(sSection,_T("hostid"),int(m_pConfig->nHostID));
	theApp.WriteProfileInt(sSection,_T("baudrate"),int(m_pConfig->nBaudrate));
	theApp.WriteProfileInt(sSection,_T("br"),int(m_pConfig->nBr));
	theApp.WriteProfileInt(sSection,_T("interval"),int(m_pConfig->nInterval));
	theApp.WriteProfileInt(sSection,_T("language"),int(m_pConfig->nLanguage));
	//その他項目
	theApp.WriteProfileInt(sSection,_T("selectid"),int(m_pConfig->nSelectID));
	theApp.WriteProfileInt(sSection,_T("selectgroup"),int(m_pConfig->nSelectGroup));
	theApp.WriteProfileInt(sSection,_T("recvcounter"),int(m_pConfig->nRecvCounter));
	theApp.WriteProfileInt(sSection,_T("framerate"),int(m_pConfig->nFPS));
	for(int nLoop = 0;nLoop < 256;nLoop++)
		{
		//機種設定
		sItem.Format(_T("type%d"),nLoop);
		theApp.WriteProfileInt(sSection,sItem,int(m_pConfig->type256[nLoop]));
		//周期転送設定
		sItem.Format(_T("send%d"),nLoop);
		theApp.WriteProfileInt(sSection,sItem,int(m_pConfig->send256[nLoop]));
		}
	}

//環境設定を表示物に設定
void CConfigDlg::reg2disp()
	{
	m_dll.SetCurSel(m_pConfig->nDLL);
	m_dlloption.SetCurSel(m_pConfig->nDLLoption);
	m_hostid.SetCurSel(m_pConfig->nHostID);
	m_baudrate.SetCurSel(m_pConfig->nBaudrate);
	m_br.SetCurSel(m_pConfig->nBr);
	m_interval.SetCurSel(m_pConfig->nInterval);
	m_language.SetCurSel(m_pConfig->nLanguage);
	}

//表示を環境設定に取り込み
void CConfigDlg::disp2reg()
	{
	m_pConfig->nDLL = (uint8_t)m_dll.GetCurSel();
	m_pConfig->nDLLoption = (uint8_t)m_dlloption.GetCurSel();
	m_pConfig->nHostID = (uint8_t)m_hostid.GetCurSel();
	m_pConfig->nBaudrate = (uint8_t)m_baudrate.GetCurSel();
	m_pConfig->nBr = (uint8_t)m_br.GetCurSel();
	m_pConfig->nInterval = (uint8_t)m_interval.GetCurSel();
	m_pConfig->nLanguage = (uint8_t)m_language.GetCurSel();
	}

//DLLファイル名を取得
TCHAR* CConfigDlg::getDllname()
	{
	return(g_config_dll[m_pConfig->nDLL].pValue);
	}

//DLLの番号を取得
uint8_t CConfigDlg::getDll()
	{
	return(m_pConfig->nDLL);
	}

//DLLオプション番号を取得
uint8_t CConfigDlg::getDllOption()
	{
	//注意点
	//	USB-to-CAN v2 (dll == 0) の場合は、何本目のケーブルを使うかを指定（0開始:0..1本目）
	//	WACOCAN (dll == 1) の場合は、COMポート番号を指定（0開始:0..COM1)

	return(m_pConfig->nDLLoption);
	}

//ホスト設定番号を取得
uint8_t CConfigDlg::getHostid(void)
	{
	return(m_pConfig->nHostID + 1);
	}

//ボーレート設定値[Kbps]取得
uint32_t CConfigDlg::getBaudrate()
	{
	uint32_t nResult = g_config_baudrate[m_pConfig->nBaudrate].nValue;
	return(nResult);
	}

//機種設定を取得
uint8_t CConfigDlg::getABH3type(uint8_t nDevice)
	{
	return(m_pConfig->type256[nDevice]);
	}

//ブロードキャスト送信設定を取得
uint8_t CConfigDlg::getBr(void)
	{
	return(m_pConfig->nBr);
	}

//周期設定を取得
uint32_t CConfigDlg::getInterval()
	{
	return((uint32_t)g_config_interval[m_pConfig->nInterval].nValue);
	}

//ダイアログ表示中にリターンキーが押されると呼び出されます
void CConfigDlg::OnOK()
	{
	//動作しない様に細工
	//	CDialogEx::OnOK();
	}

//設定ボタンが押されると呼び出されます
void CConfigDlg::OnBnClickedSave()
	{
	//表示から取り込み
	disp2reg();

	//再起動要求フラグを解除
	m_pConfig->nRestart = 0;

	//DLL設定が前と異なる？
	if(m_pConfig->nOldDLL != m_pConfig->nDLL)
		{
		//再起動要求設定
		m_pConfig->nRestart = 1;
		//インターフェース設定が変更された為、本ソフトを再起動します
		if(AfxMessageBox(theApp.GetLangText(&g_config_text[1]),MB_ICONWARNING | MB_OKCANCEL) != IDOK)
			return;
		}
	else if(m_pConfig->nOldInterval != m_pConfig->nInterval)
		{
		//更新周期の変更は、対象ウィンドウを開き直す必要が有ります
		if(AfxMessageBox(theApp.GetLangText(&g_config_text[2]),MB_ICONWARNING | MB_OKCANCEL) != IDOK)
			return;
		}
	else if(m_pConfig->nOldLanguage != m_pConfig->nLanguage)
		{
		//再起動要求設定
		m_pConfig->nRestart = 1;
		//言語設定が変更された為、本ソフトを再起動します
		if(AfxMessageBox(theApp.GetLangText(&g_config_text[3]),MB_ICONWARNING | MB_OKCANCEL) != IDOK)
			return;
		}

	//システムに保存
	reg2sys();

	//ダイアログ終了
	CDialogEx::OnOK();
	}

//
HBRUSH CConfigDlg::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
	{
	//描画対象のIDを取得
	UINT nUid = pWnd->GetDlgCtrlID();
	//タイトル類？
	if((nUid >= IDC_TITLE1) && (nUid <= IDC_TITLE14))
		{
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(COLOR_BLUE);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COLOR_WHITE);
		pDC->SetBkColor(COLOR_BLUE);
		return(m_brush);
		}

	HBRUSH hbr = CDialogEx::OnCtlColor(pDC,pWnd,nCtlColor);
	return hbr;
	}




