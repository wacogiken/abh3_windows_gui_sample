#pragma once

#include <afxwin.h>

//編集状態を表すENUM
enum class ENUM_AECOLOR
	{
	 AE_COLORSEL_NORMAL = 0		//!<未編集時を意味する
	,AE_COLORSEL_CHANGE = 1		//!<編集時を意味する
	};

class CAdvanceEdit : public CEdit
{
	DECLARE_DYNAMIC(CAdvanceEdit)

public:
	//読込専用状態か？
	bool IsReadOnly(void);
	//確定フラグを取得
	bool IsSetData(void);
	//フォーカスフラグを取得
	bool IsFocus(void)
		{
		return(m_var.status.bFocus);
		}
	//状態に対する表示色を設定します
	void SetColor(ENUM_AECOLOR nColorSel,bool bEnable,COLORREF nTextColor,COLORREF nBackColor);
	//描画色(文字列通常時)設定
	void SetNormalColor(bool bEnable,COLORREF nTextColor,COLORREF nBackColor);
	//描画色(文字列変更時)設定
	void SetChangeColor(bool bEnable,COLORREF nTextColor,COLORREF nBackColor);
	//編集文字列を設定します
	void SetWindowText(LPCTSTR lpszString);
	//コンストラクタ
	CAdvanceEdit();
	//デストラクタ
	virtual ~CAdvanceEdit();
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

private:
	//変更前のテキスト
	CString m_sOldText;
	//描画用ブラシ
	CBrush m_brush;

protected:
	//!内部変数管理用構造体
	typedef struct _ADVANCEEDIT_VAR
		{
		struct _ADVANCEEDIT_VAR_COLOR
			{
			bool		bEnable;	//!<色制御(trueで制御有効)
			COLORREF	nTextColor;	//!<テキスト色
			COLORREF	nBackColor;	//!<背景色
			} color[2];	//!<要素0が未編集時、要素1が編集時
		struct _ADVANCEEDIT_VAR_STATUS
			{
			bool		bSet;		//!<確定フラグ(trueで確定)
			bool		bFocus;		//!<フォーカスフラグ(trueで有り)
			} status;
		} ADVANCEEDIT_VAR,*pADVANCEEDIT_VAR;
	//!内部変数
	ADVANCEEDIT_VAR m_var;

	DECLARE_MESSAGE_MAP()
};
