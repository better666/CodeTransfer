#pragma once

// 以下宏定义要求的最低平台。要求的最低平台
// 是具有运行应用程序所需功能的 Windows、Internet Explorer 等产品的
// 最早版本。通过在指定版本及更低版本的平台上启用所有可用的功能，宏可以
// 正常工作。

// 如果必须要针对低于以下指定版本的平台，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

/*
bool GetParamList(char *szParmString, std::vector<std::string> &ParmList)
{
	char szWord[64]={0};
	int nWordIndex = 0;
	bool bWordBegin = false;

	int nStringLen = strlen(szParmString);
	if(nStringLen <= 0)
	{
		return false;
	}

	for(int i = 0; i < nStringLen; i++)
	{
		if(szParmString[i]==' ')
		{
			if(!bWordBegin)
			{
				continue;
			}
			else
			{
				ParmList.push_back(szWord);
				memset(szWord,0, 64);
				nWordIndex = 0;
				bWordBegin = false;
			}
		}
		else
		{
			if(bWordBegin)
			{
				szWord[nWordIndex] = szParmString[i];
				nWordIndex ++;
				continue;
			}
			else
			{
				bWordBegin = true;
				nWordIndex = 0;
				szWord[nWordIndex] = szParmString[i];
				nWordIndex ++;
			}
		}
	}

	if(bWordBegin)
	{
		ParmList.push_back(szWord);
		nWordIndex = 0;
		bWordBegin = false;
	}

	return true;
}

*/