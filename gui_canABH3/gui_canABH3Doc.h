﻿/*
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

#include <stdint.h>

//
class CguicanABH3Doc : public CDocument
{
protected:
	//ID保存用
	IDSET	m_idset;

	//ID類設定を一括設定
	void SetIDSET(uint8_t nID,uint8_t nGroup,MTYPE type,LPCTSTR pType)
		{
		IDSET info;
		::ZeroMemory(&info,sizeof(IDSET));
		info.nID		= nID;
		info.nGroup		= nGroup;
		info.type		= type;
		::_tcscpy_s(info.sType32,32,pType);
		SetIDSET(&info);
		}
	void SetIDSET(pIDSET pInfo)
		{
		::CopyMemory(&m_idset,pInfo,sizeof(IDSET));
		}

	//
	CguicanABH3Doc() noexcept;
	DECLARE_DYNCREATE(CguicanABH3Doc)
	DECLARE_MESSAGE_MAP()

public:
	//ID類設定を一括取得
	pIDSET GetIDSET(void)
		{
		return(&m_idset);
		}

	//仮想関数とイベントハンドラ
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//基本構造
	virtual ~CguicanABH3Doc();

protected:
#ifdef SHARED_HANDLERS
	// 検索ハンドラーの検索コンテンツを設定するヘルパー関数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
