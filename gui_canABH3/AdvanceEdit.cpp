/*
 ******************************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2023, Y.OGAWA.
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
#include "advanceedit.h"

//
IMPLEMENT_DYNAMIC(CAdvanceEdit, CEdit)

//!コンストラクタ
CAdvanceEdit::CAdvanceEdit()
	{
    /*!
	 @param	パラメータは有りません
    */
	ZeroMemory(&m_var,sizeof(ADVANCEEDIT_VAR));
	m_brush.CreateSolidBrush(RGB(0,0,0));
	m_sOldText = "";
	}

//デストラクタ
CAdvanceEdit::~CAdvanceEdit()
	{
	m_brush.DeleteObject();
	}

//
BEGIN_MESSAGE_MAP(CAdvanceEdit, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS,OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//メッセージ横取り処理
BOOL CAdvanceEdit::PreTranslateMessage(MSG* pMsg)
	{
	if(pMsg->message == WM_KEYDOWN)
		{
		//何かキー入力
		if(pMsg->wParam == VK_RETURN)
			{
			//リターンキー入力
			m_var.status.bSet = true;
			GetWindowText(m_sOldText);
			SetWindowText(m_sOldText);	//通知及び表示色の変更に必要
			SetSel(0,-1);
			}
		}
	return CEdit::PreTranslateMessage(pMsg);
	}

//フォーカス時に呼び出されます
void CAdvanceEdit::OnEnSetfocus()
	{
	m_var.status.bFocus = true;
	}

//フォーカスロスト時に呼び出されます
void CAdvanceEdit::OnEnKillfocus()
	{
	m_var.status.bFocus = false;
	SetWindowText(m_sOldText);			//最後のデータに戻す
	SetModify(FALSE);
	}

//描画時に色を決定する為呼び出されます
HBRUSH CAdvanceEdit::CtlColor(CDC* pDC, UINT nCtlColor)
	{
	UNUSED_ALWAYS(nCtlColor);

	UINT nSel = 0;
	if(GetModify())
		nSel = 1;
	if(m_var.color[nSel].bEnable)
		{
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(m_var.color[nSel].nBackColor);
		pDC->SetTextColor(m_var.color[nSel].nTextColor);
		pDC->SetBkColor(m_var.color[nSel].nBackColor);
		return(m_brush);	//指定色で描画
		}
	return NULL;	//標準色で描画
	}

//!読込専用状態か？
bool CAdvanceEdit::IsReadOnly()
	{
	/*!
	 @brief		読込専用状態か確認します
	 @param		パラメータは有りません
	 @return	読込専用ならtrueが戻ります
	*/
	if(GetStyle() & ES_READONLY)
		return(true);	//読込専用
	return(false);		//入力可能
	}


//!文字入力確定？
bool CAdvanceEdit::IsSetData()
	{
    /*!
	 @brief		文字入力の確定フラグ状態を取得します
	 @param		パラメータは有りません
	 @return	文字入力がリターンキーで確定された場合、trueが戻ります
	 @warning	取得毎に確定フラグはクリア(falseに設定)されます
    */
	bool bResult = m_var.status.bSet;
	m_var.status.bSet = false;
	return(bResult);
	}	

//!編集状態別の表示色設定
void CAdvanceEdit::SetColor(ENUM_AECOLOR nColorSel,bool bEnable,COLORREF nTextColor,COLORREF nBackColor)
	{
	/*!
	@brief	編集状態に対する表示色を設定します
	@param	nColorSel	色を設定する状態を指定します
	@param	bEnable		trueの場合、色設定が有効になります
	@param	nTextColor	テキスト描画色を指定します
	@param	nBackColor	背景描画色を指定します
	@return	戻り値は有りません
	*/
	UINT nSel = 0;
	if(nColorSel == ENUM_AECOLOR::AE_COLORSEL_CHANGE)
		nSel = 1;
	m_var.color[nSel].bEnable = bEnable;
	m_var.color[nSel].nTextColor = nTextColor;
	m_var.color[nSel].nBackColor = nBackColor;
	}

//!未編集時描画色設定
void CAdvanceEdit::SetNormalColor(bool bEnable,COLORREF nTextColor,COLORREF nBackColor)
	{
	/*!
	@brief	文字列通常時(未編集状態)に対する表示色を設定します
	@param	bEnable		trueの場合、色設定が有効になります
	@param	nTextColor	テキスト描画色を指定します
	@param	nBackColor	背景描画色を指定します
	@return	戻り値は有りません
	*/
	SetColor(ENUM_AECOLOR::AE_COLORSEL_NORMAL,bEnable,nTextColor,nBackColor);
	}

//!編集時描画色設定
void CAdvanceEdit::SetChangeColor(bool bEnable,COLORREF nTextColor,COLORREF nBackColor)
	{
	/*!
	@brief	文字列変更時(編集状態)に対する表示色を設定します
	@param	bEnable		trueの場合、色設定が有効になります
	@param	nTextColor	テキスト描画色を指定します
	@param	nBackColor	背景描画色を指定します
	@return	戻り値は有りません
	*/
	SetColor(ENUM_AECOLOR::AE_COLORSEL_CHANGE,bEnable,nTextColor,nBackColor);
	}

//!編集文字列設定
void CAdvanceEdit::SetWindowText(LPCTSTR lpszString)
	{
	/*!
	 @brief	コントロールに文字列を設定する場合に呼び出されます
	 @param	lpszString 設定する文字列
	 @note	設定した文字列が、「変更前」の文字列になります
	*/
	SetModify(FALSE);
	::SetWindowText(GetSafeHwnd(),lpszString);
	GetWindowText(m_sOldText);
	}
