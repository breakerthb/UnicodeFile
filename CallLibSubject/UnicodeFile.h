#pragma once

#include "stdafx.h"
#include <set>

#define FOR_READ	0x10
#define FOR_WRITE	0x11
#define FOR_RE_WR	0x12

class CUnicodeFile
{
public:
	CUnicodeFile(void);
	~CUnicodeFile(void);

	BOOL Load(TCHAR* pFileName, USHORT operateType);
	VOID Release(VOID);

	BOOL CheckUnicodeFile();

	BOOL SkipUnicodeFlag(VOID);
	VOID SetUnicodeFlag(VOID);

	static BOOL Delete(CUnicodeFile& fileDel);
	static BOOL Delete(CString strFileName);

	CString GetFileName(VOID);
	CString GetFileNameExt(VOID);

	FILE *GetFilePointer(VOID);

	VOID SetFlag(USHORT flag);
	USHORT GetFlag(VOID);

	BOOL ReadLine(CString& cstr);

	BOOL WriteLine(CString cstr);

	BOOL WriteSet(std::set<CString> *pCStringSet);

	VOID WriteLineEnd(VOID);

	int Seek(LONG offset, int origin);
	LONG TellOffset(VOID);

	VOID WriteAWord(TCHAR ch);
	BOOL ReadAWord(TCHAR& ch);

	int WriteBuffer(const void* buffer, size_t itemSize, size_t count);
	int ReadBuffer(void* buffer, size_t itemSize, size_t count);

	BOOL Flush(VOID);
	BOOL GetFileAsBuffer(CString strFileName);

protected:
	// The flag for operate type. FOR_READ, for reading; FOR_WRITE, for writing
	USHORT m_fileOperateFlag;
	// The pointer for file which will be operated
	FILE* m_pFile;
	// The file name
	CString m_strFileName;
	// The file name extension
	CString m_strFileNameExt;
	// The temp TCHAR buffer
	TCHAR* m_pBuffer;
};


