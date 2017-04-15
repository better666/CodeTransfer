// TestTransfer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "transfer.h"
#include <io.h>


std::map<std::string, std::string> G_TypeFuncMap;
std::string strTab = "	";
std::string strBlank = " ";

int _tmain(int argc, _TCHAR* argv[])
{
    InitTypeFuncMap();
	
	std::string strInDir;
	std::string strOutDir;
	std::string strType;

	for(int i = 1; i < argc; i++)
	{
		std::string str = argv[i];

		if(str[0] != '-')
		{
			continue;
		}

		if (str[1] == 'i')
		{
			strInDir = str.substr(2, str.length() -2);
		}

		if (str[1] == 'o')
		{
			strOutDir = str.substr(2, str.length() -2);
		}

		if (str[1] == 't')
		{
			strType = str.substr(2, str.length() -2);
		}
	}

	if(strOutDir.empty())
	{
		strOutDir = strInDir;
	}

	if(strType.empty())
	{
		strType = "all";
	}

	std::string temp = strInDir + "\\*.msg";
	long Handle;
	struct _finddata_t fileinfo;
	if((Handle=_findfirst(temp.c_str(),&fileinfo))==-1L)
	{
		printf("cannot find input file in %s\n", strInDir);
		printf("example:\n", strInDir);
		printf("-iindir -ooutdir -ttype\n", strInDir);
	}
	else
	{
		do 
		{
			std::string infile = strInDir + "\\" +fileinfo.name;
			char *pDot = strchr(fileinfo.name,'.');
			*pDot = 0;
			std::string outfile = strOutDir + "\\" + fileinfo.name;

			if(strType == "all")
			{
				ConvertFileToCsharp(infile.c_str(),(outfile+".cs").c_str());
				ConvertFileToGo(infile.c_str(),(outfile+".go").c_str());
				ConvertFileToLua(infile.c_str(),(outfile+".lua").c_str());
				ConvertFileToCpp(infile.c_str(),(outfile+".h").c_str());
			}
			else if(strType == "cs")
			{
				ConvertFileToCsharp(infile.c_str(),(outfile+".cs").c_str());
			}
			else if(strType == "go")
			{
				ConvertFileToGo(infile.c_str(),(outfile+".go").c_str());
			}
			else if(strType == "cpp")
			{
				ConvertFileToCpp(infile.c_str(),(outfile+".cpp").c_str());
			}
			else if(strType == "lua")
			{
				ConvertFileToLua(infile.c_str(),(outfile+".lua").c_str());
			}

		} while (_findnext(Handle,&fileinfo)==0);
	
		_findclose(Handle);
	}

	
	return 0;
}

/*
这里定义是解析流里的方用里到的函数据类
如ReadInt8, WriteInt8 之类的，和语言所在的类别无关
只因为帮助类定义的方法就是这样的，统一的。
*/
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






