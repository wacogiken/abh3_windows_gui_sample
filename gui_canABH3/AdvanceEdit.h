#pragma once

#include <afxwin.h>

//�ҏW��Ԃ�\��ENUM
enum class ENUM_AECOLOR
	{
	 AE_COLORSEL_NORMAL = 0		//!<���ҏW�����Ӗ�����
	,AE_COLORSEL_CHANGE = 1		//!<�ҏW�����Ӗ�����
	};

class CAdvanceEdit : public CEdit
{
	DECLARE_DYNAMIC(CAdvanceEdit)

public:
	//�Ǎ���p��Ԃ��H
	bool IsReadOnly(void);
	//�m��t���O���擾
	bool IsSetData(void);
	//�t�H�[�J�X�t���O���擾
	bool IsFocus(void)
		{
		return(m_var.status.bFocus);
		}
	//��Ԃɑ΂���\���F��ݒ肵�܂�
	void SetColor(ENUM_AECOLOR nColorSel,bool bEnable,COLORREF nTextColor,COLORREF nBackColor);
	//�`��F(������ʏ펞)�ݒ�
	void SetNormalColor(bool bEnable,COLORREF nTextColor,COLORREF nBackColor);
	//�`��F(������ύX��)�ݒ�
	void SetChangeColor(bool bEnable,COLORREF nTextColor,COLORREF nBackColor);
	//�ҏW�������ݒ肵�܂�
	void SetWindowText(LPCTSTR lpszString);
	//�R���X�g���N�^
	CAdvanceEdit();
	//�f�X�g���N�^
	virtual ~CAdvanceEdit();
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

private:
	//�ύX�O�̃e�L�X�g
	CString m_sOldText;
	//�`��p�u���V
	CBrush m_brush;

protected:
	//!�����ϐ��Ǘ��p�\����
	typedef struct _ADVANCEEDIT_VAR
		{
		struct _ADVANCEEDIT_VAR_COLOR
			{
			bool		bEnable;	//!<�F����(true�Ő���L��)
			COLORREF	nTextColor;	//!<�e�L�X�g�F
			COLORREF	nBackColor;	//!<�w�i�F
			} color[2];	//!<�v�f0�����ҏW���A�v�f1���ҏW��
		struct _ADVANCEEDIT_VAR_STATUS
			{
			bool		bSet;		//!<�m��t���O(true�Ŋm��)
			bool		bFocus;		//!<�t�H�[�J�X�t���O(true�ŗL��)
			} status;
		} ADVANCEEDIT_VAR,*pADVANCEEDIT_VAR;
	//!�����ϐ�
	ADVANCEEDIT_VAR m_var;

	DECLARE_MESSAGE_MAP()
};
