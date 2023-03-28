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
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "gui_canABH3.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "gui_canABH3Doc.h"
#include "gui_canABH3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//各機能クラス
CguicanABH3App		theApp;		//アプリケーションクラス
CA3_FUNCLIST		theABH3;	//ABH3制御用関数を使う為の構造体
CConfigDlg			theConfig;	//環境設定クラス

//================================================================================
//CAboutDlgクラス
//================================================================================

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
	{
	}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialogEx::DoDataExchange(pDX);
	}

BEGIN_MESSAGE_MAP(CAboutDlg,CDialogEx)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
	{
	CDialogEx::OnInitDialog();

	CString sText("");
	sText.Format(_T("%s version:%s"),(LPCTSTR)getAppMyname(),(LPCTSTR)getAppMyversion());
	GetDlgItem(IDC_VERSIONINFO)->SetWindowText(sText);
	return TRUE;
	}


//================================================================================
//アプリケーションクラス
//================================================================================

//
CguicanABH3App::CguicanABH3App() noexcept
	{
	//内部変数初期化
	::ZeroMemory(&m_var,sizeof(GUICANABH3_VAR));

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("WACOGIKEN.canABH3.CAN.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	}

//メッセージマップ
BEGIN_MESSAGE_MAP(CguicanABH3App,CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT,&CguicanABH3App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW,&CWinAppEx::OnFileNew)
END_MESSAGE_MAP()

//アプリケーション初期化時に呼び出されます
BOOL CguicanABH3App::InitInstance()
	{
	//================================================================================
	//再起動処理（前のプログラム終了を待ってから起動）
	//
	//概略
	//	環境設定で再起動が必要な要素が変更されると、環境設定を抜ける時にソフトが再起動する。
	//	再起動時はコマンドラインに「前のプログラムプロセス番号」が入っているので、
	//	そのプロセスが終了するのを待ってから処理を継続する。
	//
	//================================================================================

	if(__argc <= 1)
		{
		//オプション無しで起動したので、普通の起動と思われる
		//起動を継続する為、何もしない
		}
	else
		{
		//
		//オプションありで起動、オプションを取得して数値変換
		#ifdef _UNICODE
		 DWORD nPID = (DWORD)::wcstoul(__wargv[1],NULL,10);
		#else
		 DWORD nPID = (DWORD)::strtoul(__argv[1],NULL,10);
		#endif
		//有効な数値？
		if(nPID)
			{
			//その番号のプロセスハンドルを取得し有効なら終了待ち又は10秒待つ
			HANDLE hProc = ::OpenProcess(SYNCHRONIZE,FALSE,nPID);
			if (hProc != NULL)
				{
				//終了又は10秒(10000[ms])待つ
				if(::WaitForSingleObject(hProc,10000) == WAIT_TIMEOUT)
					{
					//前のアプリケーションが終了しないので、本アプリケーションを終了する
					return FALSE;
					}
				}
			}
		}

	//2重起動チェック
	m_var.hMutex = ::CreateMutex(FALSE,0,_T("gui_canABH3"));	//2重起動チェック用Mutex
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
		{
		//2重起動の為、起動させずに終了
		exit(1);
		}

	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// OLE ライブラリを初期化します。
	if(!AfxOleInit())
		{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
		}

	AfxEnableControlContainer();
	EnableTaskbarInteraction();

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です
	// AfxInitRichEdit2();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("GUIcanABH3"));
	LoadStdProfileSettings(0);

	//====================
	//環境設定を管理する
	//====================

	//システムから値を取得する
	theConfig.sys2reg();

	//設定へのポインタ
	CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();

	//====================
	//DLLを利用可能にする
	//====================

	//実行ファイル名から実行フォルダ名を抽出する
	PTCHAR pFullMyName = new TCHAR[_MAX_PATH]();
	PTCHAR pDrive = new TCHAR[_MAX_DRIVE]();
	PTCHAR pDir = new TCHAR[_MAX_DIR]();
	GetModuleFileName(NULL,(LPTSTR)pFullMyName,_MAX_PATH);
	_tsplitpath_s(pFullMyName,pDrive,_MAX_DRIVE,pDir,_MAX_DIR,NULL,0,NULL,0);
	CString sExecFolder = CString(pDrive) + CString(pDir);
	delete[] pDir;
	delete[] pDrive;
	delete[] pFullMyName;

	//DLL読み込み
	//　読み込みに失敗した場合はエラーを表示し、次の画面として環境設定を表示する
	//		環境設定を確定した場合、DLLが読み込めれば先に進み、読めなければ繰り返す
	//		キャンセルした場合はソフト自体を終了させる
	while(-1)
		{
		//環境設定からDLLファイル名を取得
		CString sDLLfilename(theConfig.getDllname());

		//実行フォルダ名＋DLLファイル名でABH3制御用関数を読み込み
		CString sDllFullPath = sExecFolder + sDLLfilename;
		m_var.hDLL = ::LoadLibrary(sDllFullPath);

		//読み込み失敗？
		if(m_var.hDLL == NULL)
			{
			//エラー表示
			if(theConfig.GetConfig()->nLanguage == 0)
				AfxMessageBox(_T("Can't load dll."),MB_ICONSTOP);
			else
				AfxMessageBox(_T("DLLが読み込めません"),MB_ICONSTOP);
			//環境設定を表示
			if(theConfig.DoModal() == IDOK)
				theConfig.reg2sys();	//システムに保存
			else
				return(FALSE);
			}
		else	//読み込み正常
			{
			//DLLの関数を変数(構造体)に読み込み
			GetFunctions(m_var.hDLL,&theABH3);

			//USB-to-CAN v2 インターフェースに固定する
			theABH3.InitInstance(0);

			//通信インターフェース用のスレッドを起動
			m_var.thread.bQuit = false;
			m_var.thread.hHandle = (HANDLE)_beginthreadex(NULL,0,CguicanABH3App::thread,this,0,&m_var.thread.nUid);

			//
			break;
			}
		}


	//言語別にリソースを切り替える為、IDを決定する
	UINT nMainMenuID = IDR_MAINFRAME;
	UINT nChildMenuID = IDR_guicanABH3TYPE;
	if(pConfig->nLanguage)
		{
		nMainMenuID = IDR_MAINFRAME_JP;
		nChildMenuID = IDR_guicanABH3TYPE_JP;
		}

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		nChildMenuID,
		RUNTIME_CLASS(CguicanABH3Doc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CguicanABH3View));
	if(!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// メイン MDI フレーム ウィンドウを作成します。
	CMainFrame* pMainFrame = new CMainFrame;
	if(!pMainFrame || !pMainFrame->LoadFrame(nMainMenuID))
		{
		delete pMainFrame;
		return FALSE;
		}
	m_pMainWnd = pMainFrame;

	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;	//最初に空のウィンドウを開かない
	ParseCommandLine(cmdInfo);

	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	if(!ProcessShellCommand(cmdInfo))
		return FALSE;

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//
	return TRUE;
	}

//アプリケーション終了時に呼び出されます
int CguicanABH3App::ExitInstance()
	{
	//スレッド停止
	if(m_var.thread.hHandle)
		{
		m_var.thread.bQuit = true;
		::WaitForSingleObject(m_var.thread.hHandle,INFINITE);
		m_var.thread.hHandle = NULL;
		}

	//Mutex
	::CloseHandle(m_var.hMutex);
	m_var.hMutex = NULL;

	//DLL解放
	if(m_var.hDLL)
		{
		theABH3.ExitInstance();
		FreeLibrary(m_var.hDLL);
		m_var.hDLL = NULL;
		}
	//
	AfxOleTerm(FALSE);
	//
	return CWinAppEx::ExitInstance();
	}

//Aboutダイアログを開く
void CguicanABH3App::OnAppAbout()
	{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	}

//現在の表示言語指定に合う文字列を戻す
TCHAR* CguicanABH3App::GetLangText(pLANGTEXT pText)
	{
	//日本語設定で日本語文字列が有るか？
	if((theConfig.GetConfig()->nLanguage) && (pText->pTextJP != NULL))
		return(pText->pTextJP);	//日本語文字列が戻る
	//上記以外の場合は英語文字列が戻る
	return(pText->pTextEN);
	}

//指定IDが利用されているか？
int CguicanABH3App::isUsedID(uint8_t nID)
	{
	if(nID == theConfig.getHostid())
		return 2;	//HOSTが利用中
	else if(m_var.usedID[nID])
		return 1;	//ウィンドウが利用中
	//未使用
	return 0;
	}

//非同期スレッド
unsigned int CguicanABH3App::thread(void* pParam)
	{
	//概要
	//	インターフェースを開いている時、予定外で受信したデータを「受信データ状況」に反映させる
	//	この受信周期は10[ms]周期で行われる
	//	環境設定内、表示更新周期の最速設定程度の時間周期で動作させる事を推奨

	//起動元クラスと制御変数へのポインタ
	CguicanABH3App* pClass = (CguicanABH3App*)pParam;
	volatile CguicanABH3App::pGUICANABH3_VAR pVar = &pClass->m_var;

	//スレッドループ
	while(!pVar->thread.bQuit)
		{
		//インターフェースを開いているか？
		if(theABH3.IsOpenInterface())
			{
			//受信バッファをフラッシュ（受信データ状況に反映）させる
			int32_t nCount = theABH3.abh3_can_flush();
			if(nCount == 0)
				{
				//バッファにデータが無い
				}
			}
		else
			{
			//インターフェースを閉じている
			}
		//規定周期
		Sleep(10);
		}
	//
	_endthreadex(0);
	return(0);
	}

//CANバス接続
int32_t CguicanABH3App::Connect()
	{
	//ケーブル番号又はCOMポート番号を設定
	theABH3.SetInterface(theConfig.getDllOption());

	//ホスト設定番号を取得
	uint8_t nHostID = theConfig.getHostid();
	theABH3.SetHostID(nHostID);

	//ボーレート設定値[Kbps]取得し設定
	uint32_t nBaudrate = theConfig.getBaudrate();
	theABH3.SetBaudrate(nBaudrate);

	//
	theABH3.SetOpenTimeout(1000);
	theABH3.SetSendTimeout(100);
	theABH3.SetRecvTimeout(100);

	//インターフェースを開く
	return(theABH3.abh3_can_port_init());
	}

//CANバス切断
int32_t CguicanABH3App::Disconnect()
	{
	//インターフェースを閉じる
	theABH3.abh3_can_finish();

	//全ての接続対象エラーを解除
	ClearErrorAll();

	//完了
	return(0);
	}

//CANバス再接続
int32_t CguicanABH3App::Reconnect()
	{
	//切断
	Disconnect();
	//接続
	return(Connect());
	}

//本ソフトの再起動を実行
int CguicanABH3App::RestartApplication()
	{
	//処理概略（再起動前）
	//・自分自身のファイル名とフォルダを取得
	//・自分のプロセスIDを取得
	//・自分のファイル名とプロセスIDを利用して、2つ目として新しいソフトを起動する
	//・メインフレームにソフトウェア終了メッセージを送る
	//・終了処理が非同期で進ませる為、本処理から抜ける
	//
	//処理概要（再起動後）
	//・起動時にコマンドラインオプションを調べる
	//・何かコマンドラインにあるなら、数値変換した物をプロセスIDとして、
	//	その番号のプロセスが終了するか、10秒経つ迄待つ
	//・プロセス終了なら起動を継続し、タイムアウトなら起動を中止してソフト終了する
	//
	//注意点
	//	上記起動処理を実現する為、InitInstanceの先頭近くに処理が入っている
	//

	//自分の実行ファイル名と作業フォルダを取得
	TCHAR sFullname[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(),sFullname,sizeof(sFullname) / sizeof(TCHAR));
	TCHAR sDir[MAX_PATH];
	::GetCurrentDirectory(sizeof(sDir) / sizeof(TCHAR),sDir);
	//プロセス番号取得
	CString sCmd("");
	sCmd.Format(_T("%u"),::GetCurrentProcessId());
	//新しいアプリケーションを起動
	::ShellExecute(NULL,NULL,sFullname,sCmd,sDir,SW_SHOWNORMAL);
	//自分自身を終了する為、メインフレームにアプリケーション終了を送る
	m_pMainWnd->SendMessage(WM_COMMAND,MAKEWPARAM(ID_APP_EXIT,0),NULL);
	//完了
	return(0);
	}

