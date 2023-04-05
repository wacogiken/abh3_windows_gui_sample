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
//列挙子
//================================================================================

//色を適用する為の分類
enum class APPCOLOR
	{
	APPC_NOCOLOR,		//指定無し
	APPC_NORMAL,		//標準の色（背景青で白文字）
	APPC_INFO,			//機種情報表示
	APPC_AXISA,			//A軸の色
	APPC_AXISB,			//B軸の色
	APPC_POWER,			//電源系（主電源・制御電源）の色
	APPC_ANALOG,		//アナログ系（弱電）
	APPC_MONITOR,		//監視要素の色
	APPC_WARNING,		//警告の色
	APPC_ERROR,			//異常の色
	APPC_CTRL1,			//制御フラグ用
	APPC_CTRL2,			//入力フラグ用
	APPC_CTRL3,			//I/Oフラグ用
	APPC_CTRL4,			//異常/警告項目の色
	APPC_HEARTBEAT,		//ハートビート(受信中表示)用
	};

//接続先機種
enum class MTYPE
	{
	MTYPE_NORMAL = 0,	//標準
	MTYPE_SMALL	= 1,	//小型
	MTYPE_HOST = 2,		//ホスト
	};


//================================================================================
//構造体
//================================================================================

typedef struct _COLORITEM
	{
	APPCOLOR	index;
	COLORREF	nText;
	COLORREF	nBack;
	} COLORITEM,*pCOLORITEM;;

typedef struct _IDCOLOR
	{
	UINT		nBeginUid;
	UINT		nEndUid;
	APPCOLOR	index;
	} IDCOLOR,*pIDCOLOR;

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
	UINT	nSubUid;
	LANGTEXT	text;
	PACKETTYPE	nType;
	uint8_t	nAdrs;
	} IDTEXT5,*pIDTEXT5;

//文字列と数値をペアで扱う為の構造体
typedef struct _TEXTARRAY
	{
	LANGTEXT	text;			//日英のテキスト
	int32_t		nValue;			//数値としての値
	TCHAR*		pValue;			//文字列としての値
	} TEXTARRAY,*pTEXTARRAY;

//接続先情報を扱う構造体
typedef struct _IDSET
	{
	uint8_t		nID;			//対象ID
	uint8_t		nGroup;			//ブロードキャスト要求に使用するグループID
	MTYPE		type;			//機種設定
	TCHAR		sType32[32];	//機種名
	} IDSET,*pIDSET;

//================================================================================
//全体から利用可能な関数
//================================================================================

//本アプリケーション名を取得します
CString getAppMyname(void);

//実行ファイルのリソース内からバージョン番号を取得します
bool getAppMyversion(int& nVersion1,int& nVersion2,int& nVersion3,int& nVersion4);
CString getAppMyversion(int nDigitCount = 4);

//明るさ変更
COLORREF brightness(COLORREF nColor,int nPercentage);

//色変換(RGBからHSV)
void rgb2hsv(COLORREF nColor,int& nH,int& nS,int& nV);

//色変換(HSVからRGB)
COLORREF hsv2rgb(int nH,int nS,int nV);

//色取得
COLORITEM GetAppColor(APPCOLOR colorIndex);

//4bit以上同じビットが続く数を算出
uint32_t CalcBitStuff(uint8_t* pValue,uint8_t nLength);

//指定値に対する特定ビットが成立しているか取得
bool IsBit(uint32_t nValue,int nBit);

//文字列テーブルから値に一致するエントリを取得
int FindValueFromTextArray(pTEXTARRAY pSrc,int nFindValue);


#endif //PCH_H
