//概要
//	指定フォルダにある条件を満たすファイル一覧を扱うクラス
//
//履歴
//	2021/11/25	Y.OGAWA		1st release
//
//本コードの利用について
//	商用/非商用関わらず自由、但し改造利用した場合は、上記履歴欄を抹消する事が利用条件となる。
//
//注意点
//	MFC依存
//	大量の検索を伴う場合は、非同期スレッドからの利用を推奨
//
//利用例
//・ファイル検索/下位フォルダのファイルは探さない
//	CStringArray fileList;
//	CEnumFile::EnumFile(fileList,_T("e:\\temp"),_T("*.*"),false);
//
//・フォルダ検索/下位フォルダは探さない
//	CStringArray folderList;
//	CEnumFile::EnumFolder(folderList,_T("e:\\temp"),false);
//

#include <pch.h>
#include "EnumFile.h"

CEnumFile::CEnumFile()
	{
	}

CEnumFile::~CEnumFile()
	{
	}

//指定フォルダに存在するファイルを列挙
void CEnumFile::EnumFile(CStringArray& dst,CString sFolder,CString sFind,bool bRecursive /* false */)
	{
	//概要
	//	指定フォルダに存在するファイルの内、条件に合う物を列挙
	//パラメータ
	//	dst				検索結果の格納先
	//	sFolder			検索対象のフォルダ
	//	sFind			検索条件（ファイル名比較）
	//	bRecursive		trueで下位のフォルダも検索対象とする
	//戻り値
	//	無し
	//注意点
	//	検索が完了するまで処理が戻りません
	//

	//格納先初期化
	dst.RemoveAll();

	//フォルダ名修正
	sFolder = FixedFolder(sFolder);

	//検索指定が未指定の場合は全てを対象とする
	if(sFind.IsEmpty())
		sFind = _T("*.*");

	//フォルダ一覧を構築
	CStringArray folderList;
	folderList.RemoveAll();
	if(!bRecursive)
		folderList.Add(sFolder);
	else
		EnumFolder(folderList,sFolder,bRecursive);

	//取得した各フォルダに対してファイル検索
	int nCount = folderList.GetCount();
	for(int nLoop = 0;nLoop < nCount;nLoop++)
		{
		//
		CString sFullName = FixedFolder(folderList.GetAt(nLoop)) + sFind;

		//
		CFileFind FF;
		BOOL bWork = FF.FindFile(sFullName);
		while(bWork)
			{
			bWork = FF.FindNextFile();
			if(FF.IsDots())
				{
				//これはフォルダの特殊フォルダ("." or "..")
				}
			else if(FF.IsDirectory())
				{
				//これはフォルダ
				}
			else
				{
				//これは多分ファイル
				CString sFoundFile = FF.GetFilePath();
				dst.Add(sFoundFile);
				}
			}
		}
	}

//指定フォルダに存在するフォルダを列挙
void CEnumFile::EnumFolder(CStringArray& dst,CString sFolder,bool bRecursive /* false */)
	{
	//概要
	//	指定フォルダに含まれるサブフォルダ（下位のフォルダ）を列挙します
	//パラメータ
	//	dst				検索結果の格納先
	//	sFolder			検索を行うフォルダ
	//	bRecursive		trueで下位のフォルダも検索対象とする
	//戻り値
	//	無し
	//注意点
	//	検索結果の格納先に対して、常に追記となります
	//	検索が完了するまで処理が戻りません
	//

	//フォルダ名の修正（必要が有れば）
	sFolder = FixedFolder(sFolder);

	//検索対象
	CString sFullName = sFolder + _T("*.*");
	//
	CFileFind FF;
	BOOL bWork = FF.FindFile(sFullName);
	while(bWork)
		{
		bWork = FF.FindNextFile();
		if(FF.IsDots())
			{
			//"." or ".."なので無視
			}
		else if(FF.IsDirectory())
			{
			//フォルダ確定の為、追記
			CString sFoundFolder = FF.GetFilePath();
			dst.Add(sFoundFolder);
			//再帰指定が有る場合は、下位も検索
			if(bRecursive)
				EnumFolder(dst,sFoundFolder,true);
			}
		}
	}
