// CallLibProj.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "UnicodeFile.h"

int main()
{
	CUnicodeFile file;

	file.Load(_T("abc.txt"), FOR_WRITE);
	file.SetUnicodeFlag();
	file.WriteLine(_T("aaaa"));
	file.WriteLineEnd();
	file.Release();

    return 0;
}

