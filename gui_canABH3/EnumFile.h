#pragma once

#include <afxwin.h>

class CEnumFile
{
public:

	//指定フォルダに存在するファイルの内、ファイル名の条件に合う物を列挙
	static void EnumFile(CStringArray& dst,CString sFolder,CString sFind,bool bRecursive = false);

	//指定フォルダに存在するフォルダを列挙
	static void EnumFolder(CStringArray& dst,CString sFolder,bool bRecursive = false);

	//フォルダ名の正規化
	static CString FixedFolder(CString sFolder)
		{
		//概要
		//	フォルダ名の正規化
		//パラメータ
		//	sFolder		正規化する前のフォルダ名
		//戻り値
		//	正規化されたフォルダ名
		//詳細
		//	正規化 == フォルダ名の末尾にパス記号が無い場合は追記
		//

		if(sFolder.IsEmpty())
			sFolder = _T(".\\");
		else if(sFolder.Right(1) != "\\")
			sFolder += _T("\\");
		return(sFolder);
		}

public:
	CEnumFile();
	virtual ~CEnumFile();
};
