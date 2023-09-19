//�T�v
//	�w��t�H���_�ɂ�������𖞂����t�@�C���ꗗ�������N���X
//
//����
//	2021/11/25	Y.OGAWA		1st release
//
//�{�R�[�h�̗��p�ɂ���
//	���p/�񏤗p�ւ�炸���R�A�A���������p�����ꍇ�́A��L���𗓂𖕏����鎖�����p�����ƂȂ�B
//
//���ӓ_
//	MFC�ˑ�
//	��ʂ̌����𔺂��ꍇ�́A�񓯊��X���b�h����̗��p�𐄏�
//
//���p��
//�E�t�@�C������/���ʃt�H���_�̃t�@�C���͒T���Ȃ�
//	CStringArray fileList;
//	CEnumFile::EnumFile(fileList,_T("e:\\temp"),_T("*.*"),false);
//
//�E�t�H���_����/���ʃt�H���_�͒T���Ȃ�
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

//�w��t�H���_�ɑ��݂���t�@�C�����
void CEnumFile::EnumFile(CStringArray& dst,CString sFolder,CString sFind,bool bRecursive /* false */)
	{
	//�T�v
	//	�w��t�H���_�ɑ��݂���t�@�C���̓��A�����ɍ��������
	//�p�����[�^
	//	dst				�������ʂ̊i�[��
	//	sFolder			�����Ώۂ̃t�H���_
	//	sFind			���������i�t�@�C������r�j
	//	bRecursive		true�ŉ��ʂ̃t�H���_�������ΏۂƂ���
	//�߂�l
	//	����
	//���ӓ_
	//	��������������܂ŏ������߂�܂���
	//

	//�i�[�揉����
	dst.RemoveAll();

	//�t�H���_���C��
	sFolder = FixedFolder(sFolder);

	//�����w�肪���w��̏ꍇ�͑S�Ă�ΏۂƂ���
	if(sFind.IsEmpty())
		sFind = _T("*.*");

	//�t�H���_�ꗗ���\�z
	CStringArray folderList;
	folderList.RemoveAll();
	if(!bRecursive)
		folderList.Add(sFolder);
	else
		EnumFolder(folderList,sFolder,bRecursive);

	//�擾�����e�t�H���_�ɑ΂��ăt�@�C������
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
				//����̓t�H���_�̓���t�H���_("." or "..")
				}
			else if(FF.IsDirectory())
				{
				//����̓t�H���_
				}
			else
				{
				//����͑����t�@�C��
				CString sFoundFile = FF.GetFilePath();
				dst.Add(sFoundFile);
				}
			}
		}
	}

//�w��t�H���_�ɑ��݂���t�H���_���
void CEnumFile::EnumFolder(CStringArray& dst,CString sFolder,bool bRecursive /* false */)
	{
	//�T�v
	//	�w��t�H���_�Ɋ܂܂��T�u�t�H���_�i���ʂ̃t�H���_�j��񋓂��܂�
	//�p�����[�^
	//	dst				�������ʂ̊i�[��
	//	sFolder			�������s���t�H���_
	//	bRecursive		true�ŉ��ʂ̃t�H���_�������ΏۂƂ���
	//�߂�l
	//	����
	//���ӓ_
	//	�������ʂ̊i�[��ɑ΂��āA��ɒǋL�ƂȂ�܂�
	//	��������������܂ŏ������߂�܂���
	//

	//�t�H���_���̏C���i�K�v���L��΁j
	sFolder = FixedFolder(sFolder);

	//�����Ώ�
	CString sFullName = sFolder + _T("*.*");
	//
	CFileFind FF;
	BOOL bWork = FF.FindFile(sFullName);
	while(bWork)
		{
		bWork = FF.FindNextFile();
		if(FF.IsDots())
			{
			//"." or ".."�Ȃ̂Ŗ���
			}
		else if(FF.IsDirectory())
			{
			//�t�H���_�m��ׁ̈A�ǋL
			CString sFoundFolder = FF.GetFilePath();
			dst.Add(sFoundFolder);
			//�ċA�w�肪�L��ꍇ�́A���ʂ�����
			if(bRecursive)
				EnumFolder(dst,sFoundFolder,true);
			}
		}
	}
