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
#include "framework.h"
#include "gui_canABH3.h"
#include "MainFrm.h"
#include "gui_canABH3Doc.h"

#ifdef _DEBUG
 #define new DEBUG_NEW
#endif

//
static IDTEXT1 g_menu_tr1[] = {
	{ID_APP_ABOUT,			{_T("EN"),				_T("JP")}},
	};


IMPLEMENT_DYNAMIC(CMainFrame,CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame,CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(IDC_CONFIG,&CMainFrame::OnConfig)
    ON_COMMAND(IDC_CONNECT,&CMainFrame::OnConnect)
	ON_COMMAND(IDC_DISCONNECT,&CMainFrame::OnDisconnect)
	ON_UPDATE_COMMAND_UI(IDC_CONNECT,&CMainFrame::OnUpdateConnect)
	ON_UPDATE_COMMAND_UI(IDC_DISCONNECT,&CMainFrame::OnUpdateDisconnect)
    ON_COMMAND(IDC_RECONNECT,&CMainFrame::OnReconnect)
	ON_UPDATE_COMMAND_UI(IDC_RECONNECT,&CMainFrame::OnUpdateReconnect)
	ON_WM_TIMER()
    ON_COMMAND(IDC_INFO_CANRATIO,&CMainFrame::OnInfoCanratio)
	ON_UPDATE_COMMAND_UI(IDC_INFO_CANRATIO,&CMainFrame::OnUpdateInfoCanratio)
    ON_COMMAND(IDC_INFO_COUNTER,&CMainFrame::OnInfoCounter)
	ON_UPDATE_COMMAND_UI(IDC_INFO_COUNTER,&CMainFrame::OnUpdateInfoCounter)
END_MESSAGE_MAP()

//
static UINT indicators[] =
	{
	ID_SEPARATOR,           // ステータス ライン インジケーター
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	};

//
CMainFrame::CMainFrame() noexcept
	{
	//
	::ZeroMemory(&m_var,sizeof(MAIN_VAR));
	}

//
CMainFrame::~CMainFrame()
	{
	}

//
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
	if(CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//MDIタブグループで作成
	//	注意点：mdiTabParams.m_bDocumentMenuをTRUEにすると右端にボタンが出るが、押したらアサートされる
	//	注意点：タブをドラッグして縦配置、横配置にする機能は制御できない（disable不可）
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 使用可能なその他の視覚スタイル...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // タブ領域の右部に [閉じる] ボタンを配置するには、FALSE に設定します
	mdiTabParams.m_bTabIcons = FALSE;    // MDI タブでドキュメント アイコンを有効にするには、TRUE に設定します
	mdiTabParams.m_bAutoColor = TRUE;    // MDI タブの自動色設定を無効にするには、FALSE に設定します
	mdiTabParams.m_bDocumentMenu = FALSE; // タブ領域の右端にあるドキュメント メニューを無効にします

	EnableMDITabbedGroups(TRUE,mdiTabParams);	//TRUEでタブ、FALSEでマルチウィンドウ
	//
	if(!m_wndStatusBar.Create(this))
		{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できない場合
		}
	m_wndStatusBar.SetIndicators(indicators,sizeof(indicators) / sizeof(UINT));

	// ウィンドウ タイトル バーでドキュメント名とアプリケーション名の順序を切り替えます。これにより、
	//ドキュメント名を縮小版で表示できるため、タスク バーの使用性が向上します。
	//ModifyStyle(0, FWS_PREFIXTITLE);

	//
	UpdateTitle();

	SetTimer(theApp.GetTimerNum(),1000,NULL);

	return 0;
	}

//
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
	if(!CMDIFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	//タイトルを自由に付ける為、自動設定要素を解除
	cs.style &= ~FWS_ADDTOTITLE;


	//
	return TRUE;
	}

//
#ifdef _DEBUG
void CMainFrame::AssertValid() const
	{
	CMDIFrameWndEx::AssertValid();
	}
void CMainFrame::Dump(CDumpContext& dc) const
	{
	CMDIFrameWndEx::Dump(dc);
	}
#endif //_DEBUG

//「環境設定」が選択されると呼び出されます
void CMainFrame::OnConfig()
	{
	//環境設定が確定して終了したか？
	if(theConfig.DoModal() == IDOK)
		{
		//再起動要求？
		if(theConfig.IsRestartRequire())
			theApp.RestartApplication();
		}
	}

//「インターフェースの利用開始」メニューが選択されると呼び出されます
void CMainFrame::OnConnect()
    {
	//処理自体は再接続と同じ
	OnReconnect();
    }
void CMainFrame::OnUpdateConnect(CCmdUI* pCmdUI)
	{
	if(theABH3.IsOpenInterface())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
	}

//「インターフェースの利用終了」メニューが選択されると呼び出されます
void CMainFrame::OnDisconnect()
	{
	//インターフェースを閉じる
	theApp.Disconnect();
	//タイトル更新
	UpdateTitle();
	}
void CMainFrame::OnUpdateDisconnect(CCmdUI* pCmdUI)
	{
	if(theABH3.IsOpenInterface())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	}

//「インターフェースの再接続」メニューが選択されると呼び出されます
void CMainFrame::OnReconnect()
    {
	//再接続
	int32_t nResult = theApp.Reconnect();

	//タイトル更新
	UpdateTitle();
	//
	if(nResult)
		{
		CString sText("");
		sText.Format(_T("接続に失敗しました。(code:%d)"),nResult);
		AfxMessageBox(sText,MB_ICONERROR);
		}
    }
void CMainFrame::OnUpdateReconnect(CCmdUI* pCmdUI)
	{
	//接続中のみ有効なので、「インターフェースの利用終了」メニューと同じ処理
	OnUpdateDisconnect(pCmdUI);
	}

//メインフレームのタイトル変更
void CMainFrame::UpdateTitle()
	{
	CString sText(""),sTmp("");

	//環境設定へのポインタ
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();

	//CANバス占有率
	uint32_t nBaudrate = theConfig.getBaudrate();	//[kbps]
	uint32_t nBitCounter = theABH3.GetCounter();	//bit(s)
	double nIFoccupancy = double(nBitCounter) / double(nBaudrate) / 10.0;

	//送受信カウンタ
	uint32_t nCounterSend = theABH3.GetTransmitCounter(0);
	uint32_t nCounterRecv = theABH3.GetTransmitCounter(1);

	//
	if(theABH3.IsOpenInterface())
		{
		//ホストID
		sText.Format(_T("Connected  HostID(%d)"),theABH3.GetHostID());

		//バス利用率の表示指定？
		if(pConfig->nCanRatio)
			{
			sTmp.Format(_T("  BUS(%.2f[%%])"),nIFoccupancy);
			sText += sTmp;
			}

		//送受信カウンタの表示指定？
		if(pConfig->nTransmitCounter)
			{
			sTmp.Format(_T("  SEND(%d)  RECV(%d)"),nCounterSend,nCounterRecv);
			sText += sTmp;
			}

		//
#ifdef _DEBUG
		uint32_t nSendSec = nCounterSend - m_var.nLastCounterSend;
		uint32_t nRecvSec = nCounterRecv - m_var.nLastCounterRecv;
		uint32_t nAvg = nBitCounter / (nSendSec + nRecvSec + 1);
		sTmp.Format(_T("  XS(%d)  XR(%d)  XA(%d)"),nSendSec,nRecvSec,nAvg);
		sText += sTmp;
#endif

		//最終データ格納
		m_var.nLastBitCounter = nBitCounter;
		m_var.nLastCounterSend = nCounterSend;
		m_var.nLastCounterRecv = nCounterRecv;
		}
	else
		sText.Format(_T("Disconnect"));

	//タイトル設定
	SetWindowText(sText);
	}

//タイマー割り込み時に呼び出されます
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
	{
	//回線を開いている場合はタイトルを更新する
	if(theABH3.IsOpenInterface())
		UpdateTitle();
	CMDIFrameWndEx::OnTimer(nIDEvent);
	}

//「CAN-busの利用率表示」が選択されると呼び出されます
void CMainFrame::OnInfoCanratio()
    {
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	if(pConfig->nCanRatio == 0)
		pConfig->nCanRatio = 1;
	else
		pConfig->nCanRatio = 0;
	//システムへ保存
	theConfig.reg2sys();
    }
void CMainFrame::OnUpdateInfoCanratio(CCmdUI* pCmdUI)
	{
	if(theConfig.GetConfig()->nCanRatio)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
	}

//「送受信カウンタの表示」が選択されると呼び出されます
void CMainFrame::OnInfoCounter()
    {
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
	if(pConfig->nTransmitCounter == 0)
		pConfig->nTransmitCounter = 1;
	else
		pConfig->nTransmitCounter = 0;
	//システムへ保存
	theConfig.reg2sys();
    }
void CMainFrame::OnUpdateInfoCounter(CCmdUI* pCmdUI)
	{
	if(theConfig.GetConfig()->nTransmitCounter)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
	}
