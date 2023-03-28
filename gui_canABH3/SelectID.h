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
	DECLARE_DYNAMIC(CSelectID)
protected:
	//
	typedef struct _SELECTID_VAR
		{
		uint8_t		nSelectID;
		uint8_t		nGroup;
		uint8_t		nType;		//0..標準
		} SELECTID_VAR,*pSELECTID_VAR;
	//内部変数
	SELECTID_VAR m_var;
	//描画用ブラシ
	CBrush m_brush;
	//ID選択の選択肢を構築します
	void CreateIDlist(void);
	//機種の選択状態を更新します
	void UpdateType(void);
	//ボタンの有効無効を設定します
	void UpdateButton(void);
	//色制御確認
	bool DrawCheck(CWnd* pWnd,COLORITEM& colorItem);

public:
	//選択IDを取得します
	uint8_t GetID(void)
		{
		//指定されたIDがそのままの値で戻る
		return(m_var.nSelectID);
		}
	//選択グループを取得します
	uint8_t GetGroup(void)
		{
		return(m_var.nGroup);
		}
	//機種設定を取得します
	uint32_t GetType(void)
		{
		return(m_var.nType);
		}
	//機種名を取得します
	CString GetTypeName(void);


	CComboBox m_selectid;
	CComboBox m_selectgroup;
	CComboBox m_selecttype;

public:
	CSelectID(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSelectID();
	enum { IDD = IDD_SELECTID };
	virtual BOOL OnInitDialog();
    afx_msg void OnCbnDropdownSelectid();
	afx_msg void OnCbnSelchangeSelectid();
	virtual void OnOK();
    afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()
    };
