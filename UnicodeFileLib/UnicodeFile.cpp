#include "stdafx.h"

#include "UnicodeFile.h"

#include <iostream>

#define MAXCHARS 511

#define Succeeded	TRUE
#define Failed		FALSE

/*---------------------------------------------------------------------------
**        Name : CUnicodeFile
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Construct function
**       Input : None
**      Output : None
**---------------------------------------------------------------------------*/
CUnicodeFile::CUnicodeFile(void)
{
	m_fileOperateFlag = 0;
	m_pFile = NULL;
	m_pBuffer = NULL;
}

/*---------------------------------------------------------------------------
**        Name : CUnicodeFile
**      Author : Barry Tang
**        Date : 2009/10/15
** Description :
**       Input : None
**      Output : None
**---------------------------------------------------------------------------*/
CUnicodeFile::~CUnicodeFile(void)
{
	// Do nothing
}

/*---------------------------------------------------------------------------
**        Name : Load
**      Author : Barry Tang
**        Date : 2010/1/8
** Description : Init the attribute of the object
**       Input : pFileName, the file name buffer
operateType, the operate method
**      Output : TRUE or FALSE
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::Load(TCHAR* pFileName, USHORT operateType)
{
	// Save the file name
	m_strFileName = pFileName;

	// Get the file name extension
	int find = m_strFileName.ReverseFind('.');

	if (find == -1)
	{
		m_strFileNameExt.Empty();
	}
	else
	{
		m_strFileNameExt = m_strFileName.Mid(find + 1).Trim();
	}

	// Set the operate method
	m_fileOperateFlag = operateType;

	switch (operateType)
	{
	case FOR_READ:
		_wfopen_s(&m_pFile, pFileName, TEXT("rb"));
		break;
	case FOR_WRITE:
		_wfopen_s(&m_pFile, pFileName, TEXT("wb"));
		break;
	case FOR_RE_WR:
		_wfopen_s(&m_pFile, pFileName, TEXT("w+")); // The file must exist
		break;
	default:
		break;
	}

	if (m_pFile == NULL)
	{
		/*std::cout << "Can not open the file" << pFileName << std::endl;
		system( "pause" );*/
		CString str = _T("Can not open the file ");
		str.Append(pFileName);
		AfxMessageBox(str);

		return FALSE;
	}

	m_pBuffer = new TCHAR[MAXCHARS];

	return TRUE;
}

/*---------------------------------------------------------------------------
**        Name : Release
**      Author : Barry Tang
**        Date : 2010/1/8
** Description : Free the resurce
**       Input : None
**      Output : None
**---------------------------------------------------------------------------*/
VOID CUnicodeFile::Release(VOID)
{
	if (m_pBuffer != NULL)
	{
		delete[] m_pBuffer;
	}

	m_strFileName = _T("");
	fclose(m_pFile);
}

/*---------------------------------------------------------------------------
**        Name : CheckUnicodeFile
**      Author : Barry Tang
**        Date : 2011/3/21
** Description : Check if the file is Unicode file
**       Input : None
**      Output : TRUE or FALSE
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::CheckUnicodeFile()
{
	TCHAR ch = fgetwc(m_pFile);

	if (ch == 0xfeff)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*---------------------------------------------------------------------------
**        Name : Release
**      Author : Barry Tang
**        Date : 2010/1/15
** Description : Delete file from disk
**       Input : fileDel, the CUnicodeFile object
**      Output : None
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::Delete(CUnicodeFile& fileDel)
{
	CString strName = fileDel.GetFileName();
	if (strName.IsEmpty())
	{
		AfxMessageBox(_T("The file is release..."));
		return FALSE;
	}

	int num = strName.GetLength();
	char* operate = new char[num + 5];
	operate[0] = 'D';
	operate[1] = 'e';
	operate[2] = 'l';
	operate[3] = ' ';

	int i;
	for (i = 0; i < num; i++)
	{
		operate[4 + i] = (char)LOWORD(strName.GetAt(i));
	}

	operate[4 + i] = '\0';

	fileDel.Release();

	system(operate);

	delete[] operate;

	return TRUE;
}

/*---------------------------------------------------------------------------
**        Name : Release
**      Author : Barry Tang
**        Date : 2010/1/15
** Description : Delete file from disk
**       Input : strFileName, the file name
**      Output : None
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::Delete(CString strFileName)
{
	int num = strFileName.GetLength();
	char* operate = new char[num + 5];
	operate[0] = 'D';
	operate[1] = 'e';
	operate[2] = 'l';
	operate[3] = ' ';

	int i;
	for (i = 0; i < num; i++)
	{
		operate[4 + i] = (char)LOWORD(strFileName.GetAt(i));
	}

	operate[4 + i] = '\0';

	system(operate);

	delete[] operate;

	return TRUE;
}

/*---------------------------------------------------------------------------
**        Name : CUnicodeFile
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Get the file pointer
**       Input : None
**      Output : The file pointer
**---------------------------------------------------------------------------*/
FILE* CUnicodeFile::GetFilePointer(VOID)
{
	return m_pFile;
}

/*---------------------------------------------------------------------------
**        Name : GetFlag
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Get the file operate flag
**       Input : None
**      Output : The flag
**---------------------------------------------------------------------------*/
USHORT CUnicodeFile::GetFlag(VOID)
{
	return m_fileOperateFlag;
}

/*---------------------------------------------------------------------------
**        Name : SetFlag
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Set the file operate flag
**       Input : flag, the flag for reading or writing
**      Output : None
**---------------------------------------------------------------------------*/
VOID CUnicodeFile::SetFlag(USHORT flag)
{
	m_fileOperateFlag = flag;
}

/*---------------------------------------------------------------------------
**        Name : ReadLine
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Read a line information into the cstr
**       Input : cstr, the string for getting the string
**      Output : Succeeded,
Failed,
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::ReadLine(CString& cstr)
{
	CString str;
	cstr.Empty();

	//The file can not for reading
	if (m_fileOperateFlag == FOR_WRITE)
	{
		return Failed;
	}

	do
	{
		if (fgetws(m_pBuffer, MAXCHARS, m_pFile) == NULL)
		{
			//Modify by Barry on 2011-01-21.
			if (cstr.IsEmpty())
			{
				return Failed;
			}
			else
			{
				break;
			}
		}
		str = m_pBuffer;
		cstr.Append(str);
	} while (str.Find(0x000a) == -1);

	if (cstr.GetAt(cstr.GetLength() - 1) == 0x000a)
	{
		cstr.Delete(cstr.GetLength() - 1);
	}
	else
	{
		//AfxMessageBox( _T("Read a part of line...") );
	}

	if (cstr.GetAt(cstr.GetLength() - 1) == 0x000d)
	{
		cstr.Delete(cstr.GetLength() - 1);
	}
	else
	{
		//AfxMessageBox( _T("Read a part of line...") );
	}

	return Succeeded;
}

/*---------------------------------------------------------------------------
**        Name : WirteLine
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Write the string cstr into the file
**       Input : cstr, the string will be wrote into the file
**      Output : Succeeded,Failed,
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::WriteLine(CString cstr)
{
	//The file can not for writing
	if (m_fileOperateFlag == FOR_READ)
	{
		return Failed;
	}

	fputws(cstr.GetBuffer(0), m_pFile);

	WriteLineEnd();

	return Succeeded;
}

/*---------------------------------------------------------------------------
**        Name : WirteSet
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Write the set vector into the file
**       Input : pCStrintSet, the set pointer
**      Output : Succeeded,Failed,
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::WriteSet(std::set< CString > *pCStringSet)
{
	//The file can not for writing
	if (m_fileOperateFlag == FOR_READ)
	{
		return Failed;
	}

	CString cstr;

	std::set< CString >::iterator it;
	for (it = pCStringSet->begin(); it != pCStringSet->end(); it++)
	{
		WriteLine(*it);
	}

	return Succeeded;
}

/*---------------------------------------------------------------------------
**        Name : WirteLine
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Write a line end into the file
**       Input : None
**      Output : None
**---------------------------------------------------------------------------*/
VOID CUnicodeFile::WriteLineEnd(VOID)
{
	//The file can not for writing
	if (m_fileOperateFlag == FOR_READ)
	{
		return;
	}

	fputwc(0x000d, m_pFile);
	fputwc(0x000a, m_pFile);
}

/*---------------------------------------------------------------------------
**        Name : SkipUnicodeFlag
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Skip over the Unicode flag
**       Input : None
**      Output : Succeeded,Failed,
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::SkipUnicodeFlag(VOID)
{
	// The file can not for reading
	if (m_fileOperateFlag == FOR_WRITE)
	{
		return Failed;
	}

	// Check if the file is Unicode file
	if (!CheckUnicodeFile())
	{
		CString str;
		str = _T("The file \"") + m_strFileName + _T("\" is not Unicode file!");
		AfxMessageBox(str);

		return Failed;
	}

	fseek(m_pFile, sizeof(TCHAR), SEEK_SET);

	return Succeeded;
}

/*---------------------------------------------------------------------------
**        Name : SetUnicodeFlag
**      Author : Barry Tang
**        Date : 2009/10/15
** Description : Set the Unicode flag into the file
**       Input : None
**      Output : None
**---------------------------------------------------------------------------*/
VOID CUnicodeFile::SetUnicodeFlag(VOID)
{
	//The file can not for writing
	if (m_fileOperateFlag == FOR_READ)
	{
		return;
	}

	fputwc(0xfeff, m_pFile);
}

/*---------------------------------------------------------------------------
**        Name : Seek
**      Author : Barry Tang
**        Date : 2009/12/11
** Description : Move the file pointer to another place
**       Input : offset, the steps of the moving (Byte)
origin, move method
**      Output :  0,-1,
**---------------------------------------------------------------------------*/
int CUnicodeFile::Seek(LONG offset, int origin)
{
	int ret;

	switch (origin)
	{
	case SEEK_SET:
		ret = fseek(m_pFile, offset, SEEK_SET);
		break;
	case SEEK_CUR:
		ret = fseek(m_pFile, offset, SEEK_CUR);
		break;
	case SEEK_END:
		ret = fseek(m_pFile, offset, SEEK_END);
		break;
	default:
		break;
	}

	return ret;
}

/*---------------------------------------------------------------------------
**        Name : WriteBuffer
**      Author : Barry Tang
**        Date : 2009/12/11
** Description : Write information in the buffer into the file
**       Input :
**      Output :
**---------------------------------------------------------------------------*/
int CUnicodeFile::WriteBuffer(const void* buffer, size_t itemSize, size_t count)
{
	return((int)fwrite(buffer, itemSize, count, m_pFile));
}

/*---------------------------------------------------------------------------
**        Name : ReadBuffer
**      Author : Barry Tang
**        Date : 2009/12/11
** Description : Read information into the buffer
**       Input :
**      Output :
**---------------------------------------------------------------------------*/
int CUnicodeFile::ReadBuffer(void* buffer, size_t itemSize, size_t count)
{
	return((int)fread(buffer, itemSize, count, m_pFile));
}

/*---------------------------------------------------------------------------
**        Name : WriteAWord
**      Author : Barry Tang
**        Date : 2009/12/11
** Description : Write a char into file
**       Input : ch, the char
**      Output :
**---------------------------------------------------------------------------*/
VOID CUnicodeFile::WriteAWord(TCHAR ch)
{
	//The file can not for writing
	if (m_fileOperateFlag == FOR_READ)
	{
		return;
	}

	fputwc(ch, m_pFile);
}

/*---------------------------------------------------------------------------
**        Name : ReadAWord
**      Author : Barry Tang
**        Date : 2009/12/11
** Description : Read a char from the file
**       Input : ch, the parameter to get the char
**      Output : None
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::ReadAWord(TCHAR& ch)
{
	//The file can not for writing
	if (m_fileOperateFlag == FOR_READ)
	{
		return Failed;
	}

	ch = fgetwc(m_pFile);

	if (ch == EOF)
	{
		return Failed;
	}
	else
	{
		return Succeeded;
	}
}

/*---------------------------------------------------------------------------
**        Name : TallOffset
**      Author : Barry Tang
**        Date : 2009/12/11
** Description : Get current pointer position
**       Input : None
**      Output : the position
**---------------------------------------------------------------------------*/
LONG CUnicodeFile::TellOffset(VOID)
{
	return ftell(m_pFile);
}

/*---------------------------------------------------------------------------
**        Name : GetFileName
**      Author : Barry Tang
**        Date : 2010/1/15
** Description : Get the file name
**       Input : None
**      Output : The file name
**---------------------------------------------------------------------------*/
CString CUnicodeFile::GetFileName(VOID)
{
	return m_strFileName;
}

/*---------------------------------------------------------------------------
**        Name : GetFileNameExt
**      Author : Barry Tang
**        Date : 2011/3/28
** Description : Get the file name extension
**       Input : None
**      Output : The file name extension
**---------------------------------------------------------------------------*/
CString CUnicodeFile::GetFileNameExt(VOID)
{
	return m_strFileNameExt;
}

/*---------------------------------------------------------------------------
**        Name : GetFileAsBuffer
**      Author : Barry Tang
**        Date : 2010/1/15
** Description : Copy a whole file into this file
**       Input : fileGet, the file need to copy
**      Output : TRUE,FALSE,
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::GetFileAsBuffer(CString strFileName)
{
	HANDLE hFile = CreateFile(strFileName.GetBuffer(0), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		AfxMessageBox(_T("\"CreateFile\" error!"));
	}

	DWORD dwFileLength = GetFileSize(hFile, NULL);
	HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, NULL, NULL, NULL);
	LPVOID lpBase = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);

	WriteBuffer(lpBase, sizeof(BYTE), dwFileLength);

	Flush();
	UnmapViewOfFile(lpBase);

	CloseHandle(hMap);
	CloseHandle(hFile);

	return TRUE;
}

/*---------------------------------------------------------------------------
**        Name : Flush
**      Author : Barry Tang
**        Date : 2010/1/15
** Description : Flush the buffer information into the file will be written
**       Input : None
**      Output : Succeeded,Failed,
**---------------------------------------------------------------------------*/
BOOL CUnicodeFile::Flush(VOID)
{
	//The file can not for writing
	if (m_fileOperateFlag == FOR_READ)
	{
		return Failed;
	}

	fflush(m_pFile);

	return Succeeded;
}