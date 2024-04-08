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

//version.libをリンク
#pragma comment(lib,"version.lib")

//色テーブル
static COLORITEM g_app_color[] = {
	//APPCOLOR						textColor		backColor
	{APPCOLOR::APPC_NORMAL,			COLOR_WHITE,	COLOR_BLUE},
	{APPCOLOR::APPC_INFO,			COLOR_WHITE,	RGB(103,60,183)},
	{APPCOLOR::APPC_AXISA,			COLOR_WHITE,	COLOR_DARKGREEN},	//A/Y軸側の色
	{APPCOLOR::APPC_AXISB,			COLOR_WHITE,	COLOR_DARKBLUE},	//B/X軸側の色
	{APPCOLOR::APPC_POWER,			COLOR_WHITE,	COLOR_DARKMAGENTA},	//電源系
	{APPCOLOR::APPC_ANALOG,			COLOR_BLACK,	COLOR_YELLOW},		//アナログ入出力
	{APPCOLOR::APPC_WARNING,		COLOR_BLACK,	COLOR_YELLOW},		//警告
	{APPCOLOR::APPC_ERROR,			COLOR_WHITE,	COLOR_RED},			//異常
	{APPCOLOR::APPC_MONITOR,		COLOR_WHITE,	COLOR_BLACK},		//アナログモニタ
	{APPCOLOR::APPC_CTRL1,			COLOR_BLACK,	RGB(170,170,255)},	//制御フラグONの色
	{APPCOLOR::APPC_CTRL2,			COLOR_BLACK,	RGB(0,227,227)},	//入力フラグONの色
	{APPCOLOR::APPC_CTRL3,			COLOR_BLACK,	RGB(255,98,255)},	//I/OフラグONの色
	{APPCOLOR::APPC_CTRL4,			COLOR_BLACK,	RGB(255,188,155)},	//異常/警告タイトルの色
	{APPCOLOR::APPC_HEARTBEAT,		COLOR_BLACK,	RGB(0,236,0)},		//ハートビート（受信中）の色
	{APPCOLOR::APPC_LOG_CONNECTED,	COLOR_BLACK,	COLOR_YELLOW},		//ログ機能有効時のインターフェース接続中色
	{APPCOLOR::APPC_LOG_DISCONNECTED,COLOR_WHITE,	COLOR_BLACK},		//ログ機能有効時のインターフェース未接続色
	{APPCOLOR::APPC_NOCOLOR,		0,				0},
	};

//色取得
COLORITEM GetAppColor(APPCOLOR colorIndex)
	{
	COLORITEM result;
	int nLoop = 0;
	while(-1)
		{
		result = g_app_color[nLoop];
		if(result.index == APPCOLOR::APPC_NOCOLOR)
			break;	//not found
		else if(result.index == colorIndex)
			break;	//hit
		++nLoop;
		}
	return(result);
	}

//本アプリケーション名を取得します
CString getAppMyname()
	{
	//概要
	//	アプリケーション名（ファイル名から拡張子を除いた物）を戻します
	TCHAR* pFull = new TCHAR[_MAX_PATH];
	TCHAR* pName = new TCHAR[_MAX_FNAME];
	GetModuleFileName(NULL,(LPTSTR)pFull,_MAX_PATH);
	_tsplitpath_s(pFull,NULL,0,NULL,0,pName,_MAX_FNAME,NULL,0);
	CString sResult(pName);
	delete [] pName;
	delete [] pFull;
	return(sResult);
	}

//実行ファイルのリソース内からバージョン番号を取得します
bool getAppMyversion(int& nVersion1,int& nVersion2,int& nVersion3,int& nVersion4)
	{
	//戻り値
	bool bResult = false;

	//初期化（エラー時の値）
	nVersion1 = -1;
	nVersion2 = -1;
	nVersion3 = -1;
	nVersion4 = -1;

	//一時領域
	PTCHAR pPath = NULL;
	LPVOID pData = NULL;

	//領域確保
	pPath = new TCHAR[MAX_PATH]();
	if(pPath)
		{
		//ファイル名取得
		::GetModuleFileName(AfxGetInstanceHandle(),pPath,MAX_PATH);
		DWORD nDummy = 0;
		//ファイル情報のサイズを取得
		DWORD nSize = GetFileVersionInfoSize(pPath,&nDummy); 
		pData = malloc(nSize);
		if(pData)
			{
			//ファイル情報取得
			GetFileVersionInfo(pPath,0,nSize,pData); 
			LPVOID pBuffer = NULL;
			UINT nSize2 = 0;
			VerQueryValue(pData,_T("\\"),&pBuffer,&nSize2);
			//ファイル情報のデコード
			VS_FIXEDFILEINFO* pInfo = (VS_FIXEDFILEINFO*)pBuffer; 
			nVersion1 = (int)((pInfo->dwFileVersionMS >> 16) & 0xffff);
			nVersion2 = (int)(pInfo->dwFileVersionMS & 0xff);
			nVersion3 = (int)((pInfo->dwFileVersionLS >> 16) & 0xffff);
			nVersion4 = (int)(pInfo->dwFileVersionLS & 0xff);
			//戻り値を正常値に設定
			bResult = true;
			}
		}
	//解放
	if(pData)
		free(pData);
	if(pPath)
		delete [] pPath;
	//完了
	return(bResult);
	}
CString getAppMyversion(int nDigitCount /* 4 */)
	{
	int version[4];
	getAppMyversion(version[0],version[1],version[2],version[3]);
	//
	CString sResult("");
	if(nDigitCount <= 1)
		sResult.Format(_T("%d"),version[0]);
	else if(nDigitCount <= 2)
		sResult.Format(_T("%d.%d"),version[0],version[1]);
	else if(nDigitCount <= 3)
		sResult.Format(_T("%d.%d.%d"),version[0],version[1],version[2]);
	else
		sResult.Format(_T("%d.%d.%d.%d"),version[0],version[1],version[2],version[3]);
	return(sResult);
	}

//!明るさ変更
COLORREF brightness(COLORREF nColor,int nPercentage)
	{
	/*!
	@brief	指定色の明るさを変更します
	@param	nColor	元となる色
	@param	nPercentage	明るさを[%]で指定します(0以上100以下)
	@return	作成された色
	@warning	元の色より明るくは出来ません
	*/
	//nPercentage[0-100]
	if(nPercentage < 0)
		nPercentage = 0;
	else if(nPercentage > 100)
		nPercentage = 100;
	return(RGB(
			(nColor & 0xff) * nPercentage / 100
		,((nColor >> 8) & 0xff) * nPercentage / 100
		,((nColor >> 16) & 0xff) * nPercentage / 100));
	}

//!色変換(RGBからHSV)
void rgb2hsv(COLORREF nColor,int& nH,int& nS,int& nV)
	{
	/*!
	@brief	RGB形式の色をHSB形式に変換します
	@param	nColor	元とRGB形式の色
	@param[out]	nH	変換されたHSVのH要素
	@param[out]	nS	変換されたHSVのS要素
	@param[out]	nV	変換されたHSVのV要素
	@return	戻り値は有りません
	@warning	原理上、同じ色にはならない場合があります
	*/

	//H[0-359],S[0-100],V[0-100]
	//色を分解する
	BYTE nR = (BYTE)((DWORD)nColor & 0xff);
	BYTE nG = (BYTE)(((DWORD)nColor >> 8) & 0xff);
	BYTE nB = (BYTE)(((DWORD)nColor >> 16) & 0xff);
	//最小と最大を求める
	BYTE nMax = nR;
	BYTE nMin = nR;
	if(nMax < nG)
		nMax = nG;
	if(nMax < nB)
		nMax = nB;
	if(nMin > nG)
		nMin = nG;
	if(nMin > nB)
		nMin = nB;
	//特殊判定
	nV = nMax * 100 / 255;
	if((nV == 0) || ((nMax - nMin) == 0))
		{
		//元の色が無い、又は色差が無い
		nH = 0;
		nS = 0;
		nV = 0;
		}
	else
		{
		nS = (nMax - nMin) * 100 / nMax;
		if(nMax == nR)
			nH = (((nMax - nB) * 100 / (nMax - nMin)) - ((nMax - nG) * 100 / (nMax - nMin))) * 60 / 100;
		else if(nMax == nG)
			nH = (((nMax - nR) * 100 / (nMax - nMin)) - ((nMax - nB) * 100 / (nMax - nMin))) * 60 / 100 + 120;
		else
			nH = (((nMax - nG) * 100 / (nMax - nMin)) - ((nMax - nR) * 100 / (nMax - nMin))) * 60 / 100 + 240;
		}
	if(nH < 0)
		nH += 360;
	}

//!色変換(HSVからRGB)
COLORREF hsv2rgb(int nH,int nS,int nV)
	{
	/*!
	@brief	HSV形式の色をRGB形式に変換します
	@param	nH	変換されたHSVのH要素
	@param	nS	変換されたHSVのS要素
	@param	nV	変換されたHSVのV要素
	@return	RGB形式に変換された色が戻ります
	@warning	原理上、同じ色にはならない場合があります
	*/
	//範囲制限
	while(nH >= 360)
		nH -= 360;
	if(nS > 100)
		nS = 100;
	else if(nS < 0)
		nS = 0;
	if(nV > 100)
		nV = 100;
	else if(nV < 0)
		nV = 0;
	//
	COLORREF nColor = COLOR_BLACK;
	if(nS == 0)
		nColor = RGB(nV * 255 / 100,nV * 255 / 100,nV * 100 / 255);
	else
		{
		int nL = nH / 60;
		int nF = nH - nL;
		int nM = (100 - nS) * nV * 100 / 60;
		int nN = (100 - (nF * nS / 100) * nV) / 60;
		int nK = (100 - ((100 - nF) * nS / 100)) * nV / 60;
		if(nL == 0)
			nColor = RGB(nV * 255 / 100,nK * 255 / 100,nM * 255 / 100);
		else if(nL == 1)
			nColor = RGB(nN * 255 / 100,nV * 255 / 100,nM * 255 / 100);
		else if(nL == 2)
			nColor = RGB(nM * 255 / 100,nV * 255 / 100,nK * 255 / 100);
		else if(nL == 3)
			nColor = RGB(nM * 255 / 100,nN * 255 / 100,nV * 255 / 100);
		else if(nL == 4)
			nColor = RGB(nK * 255 / 100,nM * 255 / 100,nV * 255 / 100);
		else if(nL == 5)
			nColor = RGB(nV * 255 / 100,nM * 255 / 100,nN * 255 / 100);
		}
	return(nColor);
	}

//4bit以上同じビットが続く数を算出
uint32_t CalcBitStuff(uint8_t* pValue,uint8_t nLength)
	{
	//注意点
	//	MSB側から調べる(bit7 -> bit0)

	//
	if(nLength == 0)
		return(0);
	//
	uint32_t nResult = 0;
	uint32_t nBit = 7;
	uint32_t nCount = 0;
	uint8_t nLastBit = ~(*pValue >> 7) & 1;
	//
	while(nLength)
		{
		uint8_t nBitData = (*pValue >> nBit) & 1;
		if(nBitData == nLastBit)
			{
			if(++nCount >= 3)
				{
				nCount = 0;
				nLastBit = ~nLastBit & 1;
				++nResult;
				}
			}
		else
			{
			nLastBit = nBitData;
			nCount = 0;
			}
		if(nBit == 0)
			{
			nBit = 7;
			++pValue;
			--nLength;
			}
		else
			--nBit;
		}
	return(nResult);
	}

//指定値に対する特定ビットが成立しているか取得
bool IsBit(uint32_t nValue,int nBit)
	{
	if(nValue & (1 << nBit))
		return(true);
	return(false);
	}

//文字列テーブルから値に一致するエントリを取得
int FindValueFromTextArray(pTEXTARRAY pSrc,int nFindValue)
	{
	//戻り値
	//	発見した場合はその要素番号が戻ります
	//	見つからない場合は、-1が戻ります

	int nPt = 0;
	while(pSrc[nPt].text.pTextEN)
		{
		if(pSrc[nPt].nValue == nFindValue)
			return(nPt);
		++nPt;
		}
	return(-1);
	}

