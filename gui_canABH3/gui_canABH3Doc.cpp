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
#include "gui_canABH3Doc.h"
#include "SelectID.h"
#include <propkey.h>

#ifdef _DEBUG
 #define new DEBUG_NEW
#endif

//
IMPLEMENT_DYNCREATE(CguicanABH3Doc,CDocument)

//
BEGIN_MESSAGE_MAP(CguicanABH3Doc,CDocument)
END_MESSAGE_MAP()


//
CguicanABH3Doc::CguicanABH3Doc() noexcept
	{
	//ID未指定状態
	SetIDangGroup(0xFF,0x00,100);
	}

//
CguicanABH3Doc::~CguicanABH3Doc()
	{
	//アプリケーションクラスにこのIDの利用を終了した事を通知
	theApp.SetID(GetID(),false);
	}

// CguicanABH3Doc の診断
#ifdef _DEBUG
void CguicanABH3Doc::AssertValid() const
	{
	CDocument::AssertValid();
	}
void CguicanABH3Doc::Dump(CDumpContext& dc) const
	{
	CDocument::Dump(dc);
	}
#endif //_DEBUG

//
BOOL CguicanABH3Doc::OnNewDocument()
	{
	//接続先IDを選択
	CSelectID SELID;
	if(SELID.DoModal() != IDOK)
		return(FALSE);

	//選択したIDとグループを取得し、ドキュメントクラス内に登録
	uint8_t nSelectID = SELID.GetID();
	uint8_t nGroup = SELID.GetGroup();
	uint32_t nInterval = SELID.GetInterval();
	SetIDangGroup(nSelectID,nGroup,nInterval);

	//アプリケーションクラスにこのIDの利用を開始した事を通知
	theApp.SetID(nSelectID,true);

/*
	if(!CDocument::OnNewDocument())
		return FALSE;
	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。

*/
	return TRUE;
	}

//
void CguicanABH3Doc::Serialize(CArchive& ar)
	{
	if(ar.IsStoring())
		{
		// TODO: 格納するコードをここに追加してください。
		}
	else
		{
		// TODO: 読み込むコードをここに追加してください。
		}
	}

#ifdef SHARED_HANDLERS

//縮小版のサポート
void CguicanABH3Doc::OnDrawThumbnail(CDC& dc,LPRECT lprcBounds)
	{
	// このコードを変更してドキュメントのデータを描画します
	dc.FillSolidRect(lprcBounds,RGB(255,255,255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText,lprcBounds,DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
	}

// 検索ハンドラーのサポート
void CguicanABH3Doc::InitializeSearchContent()
	{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
	}

void CguicanABH3Doc::SetSearchContent(const CString& value)
	{
	if(value.IsEmpty())
		{
		RemoveChunk(PKEY_Search_Contents.fmtid,PKEY_Search_Contents.pid);
		}
	else
		{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if(pChunk != nullptr)
			{
			pChunk->SetTextValue(PKEY_Search_Contents,value,CHUNK_TEXT);
			SetChunkValue(pChunk);
			}
		}
	}
#endif // SHARED_HANDLERS


