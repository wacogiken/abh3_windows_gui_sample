#pragma once

#include <afxwin.h>

class CEnumFile
{
public:

	//�w��t�H���_�ɑ��݂���t�@�C���̓��A�t�@�C�����̏����ɍ��������
	static void EnumFile(CStringArray& dst,CString sFolder,CString sFind,bool bRecursive = false);

	//�w��t�H���_�ɑ��݂���t�H���_���
	static void EnumFolder(CStringArray& dst,CString sFolder,bool bRecursive = false);

	//�t�H���_���̐��K��
	static CString FixedFolder(CString sFolder)
		{
		//�T�v
		//	�t�H���_���̐��K��
		//�p�����[�^
		//	sFolder		���K������O�̃t�H���_��
		//�߂�l
		//	���K�����ꂽ�t�H���_��
		//�ڍ�
		//	���K�� == �t�H���_���̖����Ƀp�X�L���������ꍇ�͒ǋL
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
