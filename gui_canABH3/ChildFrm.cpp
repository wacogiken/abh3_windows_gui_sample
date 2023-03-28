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
#include "ChildFrm.h"

#ifdef _DEBUG
 #define new DEBUG_NEW
#endif

//
IMPLEMENT_DYNCREATE(CChildFrame,CMDIChildWndEx)

//
BEGIN_MESSAGE_MAP(CChildFrame,CMDIChildWndEx)
    ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

//
CChildFrame::CChildFrame() noexcept
	{
	m_bActive = false;
	}

//
CChildFrame::~CChildFrame()
	{
	}

// CChildFrame 診断
#ifdef _DEBUG
void CChildFrame::AssertValid() const
	{
	CMDIChildWndEx::AssertValid();
	}
void CChildFrame::Dump(CDumpContext& dc) const
	{
	CMDIChildWndEx::Dump(dc);
	}
#endif //_DEBUG

//
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
	// TODO: CREATESTRUCT cs を変更して、Window クラスまたはスタイルを変更します。
	if(!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | FWS_ADDTOTITLE | WS_THICKFRAME;
	cs.style &= ~FWS_ADDTOTITLE;

	return(TRUE);
	}

//ウィンドウのアクティブ状態が変化すると呼び出されます
void CChildFrame::OnMDIActivate(BOOL bActivate,CWnd* pActivateWnd,CWnd* pDeactivateWnd)
    {
    CMDIChildWndEx::OnMDIActivate(bActivate,pActivateWnd,pDeactivateWnd);
	//状態を保存
	m_bActive = (bool)(bActivate != 0);
    }
