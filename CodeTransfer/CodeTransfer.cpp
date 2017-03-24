// TestTransfer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "transfer.h"



std::map<std::string, std::string> G_TypeFuncMap;
std::string strTab = "	";
std::string strBlank = " ";

int _tmain(int argc, _TCHAR* argv[])
{
    InitTypeFuncMap();
	//ConvertFileToCsharp("D:\\Heros\\server\\src\\battlesvr\\mainlogic\\battle_msg.h","D:\\Heros\\server\\src\\msg\\battle_msg.cs");
	//ConvertFileToGo("D:\\Heros\\server\\src\\battlesvr\\mainlogic\\battle_msg.h","D:\\Heros\\server\\src\\msg\\battle_msg.go");
	//ConvertFileToLua("D:\\Heros\\server\\src\\battlesvr\\mainlogic\\battle_msg.h","C:\\Users\\Administrator\\Desktop\\testlua\\battle_msg.lua");
	ConvertFileToCpp("D:\\Heros\\server\\src\\battlesvr\\mainlogic\\battle_msg.h","D:\\Heros\\server\\src\\msg\\battle_msg.cpp");
		/*
	////以下转换走的配制
	FILE *pFile = fopen(GetConfigName().c_str(),"r+");
	if(pFile == NULL)
	{
		return FALSE;
	}

	CHAR szBuff[256] = {0};
	do
	{
		fgets(szBuff, 256, pFile);
		if(szBuff[0] == '#')
		{
			continue;
		}

		CHAR *pChar = strchr(szBuff,'>');
		if(pChar == NULL)
		{
			continue;
		}

		std::string strSrcFile;
		strSrcFile.assign(szBuff,pChar-szBuff);
		std::string strDestFile = pChar+1;
		strDestFile = strDestFile.substr(0,strDestFile.find_last_not_of(" \n\r\t")+1); 
		const char *ext=strrchr(strDestFile.c_str(),'.');
		if (strcmp(ext+1, "cs") == 0)
		{
				ConvertFileToCsharp(strSrcFile.c_str(), strDestFile.c_str());
		}

		if (strcmp(ext+1, "go") == 0)
		{
			ConvertFileToGo(strSrcFile.c_str(), strDestFile.c_str());
		}




	}while(!feof(pFile));

	fclose(pFile);

	*/
	return 0;
}

std::string GetConfigName()
{
	char szPath[256];
	_getcwd(szPath, 256);
	return std::string(szPath)+"\\config.ini";
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

bool GetWorldList(const char *szFile, std::vector<std::string> &WordList)
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

		szLine[strlen(szLine)-1] = 0;
        
		for(int i = 0; i < MAX_LINE_LEN; i++)
		{
			if ((szLine[i]=='#')||(szLine[i]==0)||(szLine[i]=='\r')||(szLine[i]=='\n')||(szLine[i]=='/'))
			{
				if(bWordBegin)
				{
					WordList.push_back(szWord);
					memset(szWord,0, 64);
					nWordIndex = 0;
					bWordBegin = false;
				}

				if ((szLine[i] == '/' )&&(szLine[i+1] == '/'))
				{
					WordList.push_back(szLine+i);
				}

				break;
			}
			else if((szLine[i]==' ')||(szLine[i]=='\t')||(szLine[i]==';'))
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
			fieldstate = fs_type;
		}
		else if(str == "}")
		{
			packetstate = ps_none;
			fieldstate = fs_type;
			PacketList.push_back(tempPakcet);
			tempPakcet.PacketCmt = "";
		}
		else if (str[0] == '/')
		{
			if (packetstate == ps_none)
			{
				tempPakcet.PacketCmt = str;
			}
			else if (packetstate == ps_field)
			{
				tempPakcet.fieldList.at(tempPakcet.fieldList.size()-1).fieldCmt = str;
			}
		}
		else
		{
				if(packetstate == ps_name)
				{ 
					tempPakcet.PacketName = str;
					packetstate = ps_field;
					fieldstate  = fs_type;
				}
				else if(packetstate == ps_field)
				{  
					if(fieldstate == fs_type)
					{   
						tempFiled.fieldType = str;
						fieldstate = fs_name;
					}
					else if(fieldstate == fs_name)
					{ 
						tempFiled.fieldName = str;
						tempPakcet.fieldList.push_back(tempFiled);
						fieldstate = fs_type;
					}
			}
		}
	}

	return true;
}






