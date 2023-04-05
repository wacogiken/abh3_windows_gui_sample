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

#include <stdint.h>
#include "resource.h"

//
class CSelectID : public CDialogEx
{
protected:

	//選択結果の格納先
	IDSET m_var;

	//描画用ブラシ
	CBrush m_brush;

	//ID選択の選択肢を構築します
	void CreateIDlist(void);

	//現在のID設定を元に、機種を選択します
	void UpdateType(void);

	//ボタンの有効無効を設定します
	void UpdateButton(void);

	//色制御確認
	bool DrawCheck(CWnd* pWnd,COLORITEM& colorItem);

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
		::CopyMemory(&m_var,pInfo,sizeof(IDSET));
		}

	//DDE
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	//
	DECLARE_DYNAMIC(CSelectID)
	DECLARE_MESSAGE_MAP()

public:
	//ID類設定を一括取得
	pIDSET GetIDSET(void)
		{
		return(&m_var);
		}

	//コントロール変数
	CComboBox m_selectid;
	CComboBox m_selectgroup;
	CComboBox m_selecttype;

	//仮想関数とイベントハンドラ
	virtual BOOL OnInitDialog();
    afx_msg void OnCbnDropdownSelectid();
	afx_msg void OnCbnSelchangeSelectid();
	virtual void OnOK();
    afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);

public:
	//基本構造
	CSelectID(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSelectID();
	enum { IDD = IDD_SELECTID };
};
