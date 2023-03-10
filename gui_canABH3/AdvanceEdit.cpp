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

//!�R���X�g���N�^
CAdvanceEdit::CAdvanceEdit()
	{
    /*!
	 @param	�p�����[�^�͗L��܂���
    */
	ZeroMemory(&m_var,sizeof(ADVANCEEDIT_VAR));
	m_brush.CreateSolidBrush(RGB(0,0,0));
	m_sOldText = "";
	}

//�f�X�g���N�^
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

//���b�Z�[�W����菈��
BOOL CAdvanceEdit::PreTranslateMessage(MSG* pMsg)
	{
	if(pMsg->message == WM_KEYDOWN)
		{
		//�����L�[����
		if(pMsg->wParam == VK_RETURN)
			{
			//���^�[���L�[����
			m_var.status.bSet = true;
			GetWindowText(m_sOldText);
			SetWindowText(m_sOldText);	//�ʒm�y�ѕ\���F�̕ύX�ɕK�v
			SetSel(0,-1);
			}
		}
	return CEdit::PreTranslateMessage(pMsg);
	}

//�t�H�[�J�X���ɌĂяo����܂�
void CAdvanceEdit::OnEnSetfocus()
	{
	m_var.status.bFocus = true;
	}

//�t�H�[�J�X���X�g���ɌĂяo����܂�
void CAdvanceEdit::OnEnKillfocus()
	{
	m_var.status.bFocus = false;
	SetWindowText(m_sOldText);			//�Ō�̃f�[�^�ɖ߂�
	SetModify(FALSE);
	}

//�`�掞�ɐF�����肷��׌Ăяo����܂�
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
		return(m_brush);	//�w��F�ŕ`��
		}
	return NULL;	//�W���F�ŕ`��
	}

//!�Ǎ���p��Ԃ��H
bool CAdvanceEdit::IsReadOnly()
	{
	/*!
	 @brief		�Ǎ���p��Ԃ��m�F���܂�
	 @param		�p�����[�^�͗L��܂���
	 @return	�Ǎ���p�Ȃ�true���߂�܂�
	*/
	if(GetStyle() & ES_READONLY)
		return(true);	//�Ǎ���p
	return(false);		//���͉\
	}


//!�������͊m��H
bool CAdvanceEdit::IsSetData()
	{
    /*!
	 @brief		�������͂̊m��t���O��Ԃ��擾���܂�
	 @param		�p�����[�^�͗L��܂���
	 @return	�������͂����^�[���L�[�Ŋm�肳�ꂽ�ꍇ�Atrue���߂�܂�
	 @warning	�擾���Ɋm��t���O�̓N���A(false�ɐݒ�)����܂�
    */
	bool bResult = m_var.status.bSet;
	m_var.status.bSet = false;
	return(bResult);
	}	

//!�ҏW��ԕʂ̕\���F�ݒ�
void CAdvanceEdit::SetColor(ENUM_AECOLOR nColorSel,bool bEnable,COLORREF nTextColor,COLORREF nBackColor)
	{
	/*!
	@brief	�ҏW��Ԃɑ΂���\���F��ݒ肵�܂�
	@param	nColorSel	�F��ݒ肷���Ԃ��w�肵�܂�
	@param	bEnable		true�̏ꍇ�A�F�ݒ肪�L���ɂȂ�܂�
	@param	nTextColor	�e�L�X�g�`��F���w�肵�܂�
	@param	nBackColor	�w�i�`��F���w�肵�܂�
	@return	�߂�l�͗L��܂���
	*/
	UINT nSel = 0;
	if(nColorSel == ENUM_AECOLOR::AE_COLORSEL_CHANGE)
		nSel = 1;
	m_var.color[nSel].bEnable = bEnable;
	m_var.color[nSel].nTextColor = nTextColor;
	m_var.color[nSel].nBackColor = nBackColor;
	}

//!���ҏW���`��F�ݒ�
void CAdvanceEdit::SetNormalColor(bool bEnable,COLORREF nTextColor,COLORREF nBackColor)
	{
	/*!
	@brief	������ʏ펞(���ҏW���)�ɑ΂���\���F��ݒ肵�܂�
	@param	bEnable		true�̏ꍇ�A�F�ݒ肪�L���ɂȂ�܂�
	@param	nTextColor	�e�L�X�g�`��F���w�肵�܂�
	@param	nBackColor	�w�i�`��F���w�肵�܂�
	@return	�߂�l�͗L��܂���
	*/
	SetColor(ENUM_AECOLOR::AE_COLORSEL_NORMAL,bEnable,nTextColor,nBackColor);
	}

//!�ҏW���`��F�ݒ�
void CAdvanceEdit::SetChangeColor(bool bEnable,COLORREF nTextColor,COLORREF nBackColor)
	{
	/*!
	@brief	������ύX��(�ҏW���)�ɑ΂���\���F��ݒ肵�܂�
	@param	bEnable		true�̏ꍇ�A�F�ݒ肪�L���ɂȂ�܂�
	@param	nTextColor	�e�L�X�g�`��F���w�肵�܂�
	@param	nBackColor	�w�i�`��F���w�肵�܂�
	@return	�߂�l�͗L��܂���
	*/
	SetColor(ENUM_AECOLOR::AE_COLORSEL_CHANGE,bEnable,nTextColor,nBackColor);
	}

//!�ҏW������ݒ�
void CAdvanceEdit::SetWindowText(LPCTSTR lpszString)
	{
	/*!
	 @brief	�R���g���[���ɕ������ݒ肷��ꍇ�ɌĂяo����܂�
	 @param	lpszString �ݒ肷�镶����
	 @note	�ݒ肵�������񂪁A�u�ύX�O�v�̕�����ɂȂ�܂�
	*/
	SetModify(FALSE);
	::SetWindowText(GetSafeHwnd(),lpszString);
	GetWindowText(m_sOldText);
	}
