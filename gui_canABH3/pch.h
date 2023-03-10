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

// pch.h: プリコンパイル済みヘッダー ファイルです。
// 次のファイルは、その後のビルドのビルド パフォーマンスを向上させるため 1 回だけコンパイルされます。
// コード補完や多くのコード参照機能などの IntelliSense パフォーマンスにも影響します。
// ただし、ここに一覧表示されているファイルは、ビルド間でいずれかが更新されると、すべてが再コンパイルされます。
// 頻繁に更新するファイルをここに追加しないでください。追加すると、パフォーマンス上の利点がなくなります。

#ifndef PCH_H
#define PCH_H

// プリコンパイルするヘッダーをここに追加します
#include "framework.h"

//================================================================================
//構造体
//================================================================================

typedef struct _IDCOLOR
	{
	int			nIndex;
	COLORREF	nText;
	COLORREF	nBack;
	} IDCOLOR,*pIDCOLOR;

typedef struct _IDCOLOR1
	{
	UINT		nBeginUid;
	UINT		nEndUid;
	int			nColorIndex;
	} IDCOLOR1,*pIDCOLOR1;

//英語と日本語テキストを扱う為の構造体
typedef struct _LANGTEXT
	{
	TCHAR*	pTextEN;
	TCHAR*	pTextJP;
	} LANGTEXT,*pLANGTEXT;

typedef struct _IDTEXT
	{
	int			nBit;
	UINT		nUid;
	LANGTEXT	text;
	} IDTEXT,*pIDTEXT;

typedef struct _IDTEXT1
	{
	UINT		nUid;
	LANGTEXT	text;
	} IDTEXT1,*pIDTEXT1;

typedef struct _IDTEXT2
	{
	int			nBit;
	UINT		nOffUid;
	LANGTEXT	offtext;
	UINT		nOnUid;
	LANGTEXT	ontext;
	} IDTEXT2,*pIDTEXT2;

typedef struct _IDTEXT3
	{
	int		nBit;
	UINT	nUid;
	LANGTEXT	offtext;
	LANGTEXT	ontext;
	} IDTEXT3,*pIDTEXT3;

typedef struct _IDTEXT4
	{
	UINT		nTitleUid;
	LANGTEXT	text;
	UINT		nValueUid;
	} IDTEXT4,*pIDTEXT4;

typedef struct _IDTEXT5
	{
	UINT	nUid;
	TCHAR*	pText;			//EN only
	PACKETTYPE	nType;
	uint8_t	nAdrs;
	} IDTEXT5,*pIDTEXT5;

//================================================================================
//全体から利用可能な関数
//================================================================================

//本アプリケーション名を取得します
CString getAppMyname(void);

//実行ファイルのリソース内からバージョン番号を取得します
bool getAppMyversion(int& nVersion1,int& nVersion2,int& nVersion3,int& nVersion4);
CString getAppMyversion(int nDigitCount = 4);

#endif //PCH_H
