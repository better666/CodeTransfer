#pragma once

// ���º궨��Ҫ������ƽ̨��Ҫ������ƽ̨
// �Ǿ�������Ӧ�ó������蹦�ܵ� Windows��Internet Explorer �Ȳ�Ʒ��
// ����汾��ͨ����ָ���汾�����Ͱ汾��ƽ̨���������п��õĹ��ܣ������
// ����������

// �������Ҫ��Ե�������ָ���汾��ƽ̨�����޸����ж��塣
// �йز�ͬƽ̨��Ӧֵ��������Ϣ����ο� MSDN��
#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista��
#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
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