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

