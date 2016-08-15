// TestTransfer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <vector>
#include "transfer.h"


std::map<std::string, std::string> G_TypeFuncMap;
std::string strTab = "	";
std::string strBlank = " ";

int _tmain(int argc, _TCHAR* argv[])
{
    InitTypeFuncMap();
	ConvertFileToCsharp("D:\\Heros\\server\\src\\battlesvr\\mainlogic\\battle_msg.h","D:\\Heros\\client\\Assets\\AOH\\Script\\net\\battle_msg.cs");
    ConvertFileToGo("D:\\Heros\\server\\src\\battlesvr\\mainlogic\\battle_msg.h","D:\\Heros\\server\\src\\msg\\battle_msg.go");
	return 0;
}

bool InitTypeFuncMap()
{
	G_TypeFuncMap["INT8"]="Int8";
    G_TypeFuncMap["INT16"]="Int16";
    G_TypeFuncMap["INT32"]="Int32";
    G_TypeFuncMap["INT64"]="Int64";
    G_TypeFuncMap["UINT8"]="Uint8";
    G_TypeFuncMap["UINT16"]="Uint16";
    G_TypeFuncMap["UINT32"]="Uint32";
    G_TypeFuncMap["UINT64"]="Uint64";
    G_TypeFuncMap["FLOAT"]="Float";
	 G_TypeFuncMap["STRING"]="String";


    return true;
}

bool GetWorldList(char *szFile, std::vector<std::string> &WordList)
{
	FILE *pFile = fopen(szFile, "r+");
	if(pFile == NULL)
	{
		return false;
	}

	char szLine[MAX_LINE_LEN]	= {0};
	char szWord[64]		={0};
	int nWordIndex		= 0;

	bool bWordBegin = false;

	do
	{
		fgets(szLine, MAX_LINE_LEN, pFile);
		if(strlen(szLine) <= 0)
		{
			continue;
		}
        
		for(int i = 0; i < MAX_LINE_LEN; i++)
		{
			if ((szLine[i]=='#')||(szLine[i]==0)||(szLine[i]=='\r')||(szLine[i]=='\n')||(szLine[i]=='/')||(szLine[i]==';'))
			{
				if(bWordBegin)
				{
					WordList.push_back(szWord);
					memset(szWord,0, 64);
					nWordIndex = 0;
					bWordBegin = false;
				}

				break;
			}
			else if((szLine[i]==' ')||(szLine[i]=='\t'))
			{
				if(bWordBegin)
				{
					WordList.push_back(szWord);
					memset(szWord,0, 64);
					nWordIndex = 0;
					bWordBegin = false;
				}
				else
				{
					continue;
				}
			}
			else
			{
				if(!bWordBegin)
				{
					bWordBegin = true;
				}

				szWord[nWordIndex] = szLine[i];
				nWordIndex++;
			}
		}

	}while(!feof(pFile));

	fclose(pFile);

	return true;
}

bool GetPacketList(std::vector<std::string> &WorldList, std::vector<PacketDef> &PacketList)
{
	int packetstate = 0;
	int fieldstate = 0;

	PacketDef tempPakcet;
	PacketField tempFiled;
	for(size_t i = 0; i < WorldList.size(); i++)
	{
		std::string str = WorldList.at(i);
		if(str == "struct")
		{
			packetstate = ps_name;
			tempPakcet.fieldList.clear();
			tempPakcet.PacketName = "";
		}
		else if(str == "{")
		{   
			packetstate = ps_field;
		}
		else if(str == "}")
		{
			packetstate = ps_none;
			PacketList.push_back(tempPakcet);
		}
		else if(packetstate == ps_name)
		{ 
			tempPakcet.PacketName = str;
			packetstate = ps_field;
			fieldstate  = 0;
		}
		else if(packetstate == ps_field)
		{  
			if(fieldstate == 0)
			{   
				tempFiled.fieldType = str;
				fieldstate = 1;
			}
			else if(fieldstate == 1)
			{ 
				tempFiled.fieldName = str;
				tempPakcet.fieldList.push_back(tempFiled);
				fieldstate = 0;
			}
		}
	}

	return true;
}






