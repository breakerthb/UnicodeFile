// CallLibProj.cpp : 定义控制台应用程序的入口点。
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

