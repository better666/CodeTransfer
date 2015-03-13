// TestTransfer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <vector>

struct PacketField
{
	std::string fieldType;
	std::string fieldName;
};


struct PacketDef
{
	std::string PacketName;
	std::vector<PacketField> fieldList;
};

#define MAX_LINE_LEN 256

enum packState
{
	ps_none,
	ps_name,
	ps_field
};

bool GetWorldList(char *szFile, std::vector<std::string> &WordList);
bool GetPacketList(std::vector<std::string> &WorldList, std::vector<PacketDef> &PacketList);
bool ProcessPacket(PacketDef &Packet, FILE *pOutFile);

bool ConvertFile(char *szSrcFile, char *szDestFile)
{
	std::vector<std::string> m_WorldList;

	GetWorldList(szSrcFile, m_WorldList);

	std::vector<PacketDef> m_PakcetList;

	GetPacketList(m_WorldList, m_PakcetList);

	FILE *pOutFile = fopen(szDestFile,"w+");
	if(pOutFile == NULL)
	{
		int a = errno;
		return false;
	}

	fputs("using System;\n", pOutFile);

	for(size_t j = 0; j < m_PakcetList.size(); j++)
	{
		PacketDef pf = m_PakcetList.at(j);

		ProcessPacket(pf, pOutFile);
	}

	fclose(pOutFile);

	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ConvertFile("C:\\Users\\loverains\\Documents\\GitHub\\GameProject\\project\\Common\\PacketDef\\ClientPacket.h","C:\\Users\\loverains\\Documents\\GitHub\\GameProject\\project\\Client\\TestCSharp\\TestCSharp\\ClientPacket.cs");
	ConvertFile("C:\\Users\\loverains\\Documents\\GitHub\\GameProject\\project\\Common\\PacketDef\\ClientPacket.h","C:\\Users\\loverains\\Documents\\GitHub\\U3dClient\\Assets\\Core\\ClientPacket.cs");

	ConvertFile("C:\\Users\\loverains\\Documents\\GitHub\\GameProject\\project\\Common\\PacketDef\\PacketHeader.h","C:\\Users\\loverains\\Documents\\GitHub\\GameProject\\project\\Client\\TestCSharp\\TestCSharp\\PacketHeader.cs");
	ConvertFile("C:\\Users\\loverains\\Documents\\GitHub\\GameProject\\project\\Common\\PacketDef\\PacketHeader.h","C:\\Users\\loverains\\Documents\\GitHub\\U3dClient\\Assets\\Core\\PacketHeader.cs");
	
		
	return 0;
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
			if(szLine[i]=='#')
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
			else if(szLine[i]==';')
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
			else if(szLine[i]==0)
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
			else if(szLine[i]=='\r')
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
			else if(szLine[i]=='\n')
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
			else if(szLine[i]=='/')
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
			else if(szLine[i]==' ')
			{
				if(!bWordBegin)
				{
					continue;
				}
				else
				{
					WordList.push_back(szWord);
					memset(szWord,0, 64);
					nWordIndex = 0;
					bWordBegin = false;
				}
			}
			else if(szLine[i]=='	')
			{
				if(!bWordBegin)
				{
					continue;
				}
				else
				{
					WordList.push_back(szWord);
					memset(szWord,0, 64);
					nWordIndex = 0;
					bWordBegin = false;
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
				if(tempFiled.fieldType == "CHAR")
				{
					str.erase(str.find("["));
				}

				tempFiled.fieldName = str;
				tempPakcet.fieldList.push_back(tempFiled);
				fieldstate = 0;
			}
		}
	}

	return true;
}

bool ProcessFieldDeclare(PacketField &fd, FILE *pOutFile)
{	
	fputs("	public ", pOutFile);
	if(fd.fieldType == "UINT32")
	{
		fputs("UInt32 ", pOutFile);
		fputs(fd.fieldName.c_str(), pOutFile);
	}
	else if(fd.fieldType == "UINT8")
	{
		fputs("Byte ", pOutFile);
		fputs(fd.fieldName.c_str(), pOutFile);
	}
	else if(fd.fieldType == "CHAR")
	{
		fputs("string ", pOutFile);
		fputs(fd.fieldName.c_str(), pOutFile);
	}
	else if(fd.fieldType == "UINT16")
	{
		fputs("UInt16 ", pOutFile);
		fputs(fd.fieldName.c_str(), pOutFile);
	}
	else if(fd.fieldType == "UINT64")
	{
		fputs("UInt64 ", pOutFile);
		fputs(fd.fieldName.c_str(), pOutFile);
	}
	else if(fd.fieldType == "FLOAT")
	{
		fputs("Single ", pOutFile);
		fputs(fd.fieldName.c_str(), pOutFile);
	}
	else  if(fd.fieldName.find("[") == std::string::npos)
	{
		fputs(fd.fieldType.c_str(), pOutFile);
		fputs(" ", pOutFile);
		fputs(fd.fieldName.c_str(), pOutFile);
		fputs("= new ", pOutFile);
		fputs(fd.fieldType.c_str(), pOutFile);
		fputs("()", pOutFile);
	}
	else 
	{
		std::string SimpleName = fd.fieldName.substr(0,fd.fieldName.find("["));
		fputs(fd.fieldType.c_str(), pOutFile);
		fputs("[] ", pOutFile);
		fputs(SimpleName.c_str(), pOutFile);
		fputs(" ", pOutFile);
		fputs("= new ", pOutFile);
		fputs(fd.fieldType.c_str(), pOutFile);
		fputs(fd.fieldName.substr(fd.fieldName.find("[")).c_str(), pOutFile);
	}

	
	fputs(";\n", pOutFile);


	return true;
}

bool ProcessReadMethod(std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs("	public void Read(ReadBufferHelper bh)\n",pOutFile);
	fputs("	{\n", pOutFile);
	for(size_t m = 0; m < fdlist.size(); m++)
	{
		PacketField fd = fdlist.at(m);
		if(fd.fieldType == "UINT32")
		{
			fputs("		", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(" = ", pOutFile);
			fputs("bh.ReadUint32();\n", pOutFile);
		}
		else if(fd.fieldType == "UINT8")
		{
			fputs("		", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(" = ", pOutFile);
			fputs("bh.ReadUint8();\n", pOutFile);
		}
		else if(fd.fieldType == "CHAR")
		{
			fputs("		", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(" = ", pOutFile);
			fputs("bh.ReadFixString(32);\n", pOutFile);
		}
		else if(fd.fieldType == "UINT16")
		{
			fputs("		", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(" = ", pOutFile);
			fputs("bh.ReadUint16();\n", pOutFile);
		}
		else if(fd.fieldType == "UINT64")
		{
			fputs("		", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(" = ", pOutFile);
			fputs("bh.ReadUint64();\n", pOutFile);
		}
		else if(fd.fieldType == "FLOAT")
		{
			fputs("		", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(" = ", pOutFile);
			fputs("bh.ReadFloat();\n", pOutFile);
		}
		else if(fd.fieldName.find("[") == std::string::npos)
		{
			fputs("		", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(".Read(bh);\n", pOutFile);
		}
		else
		{
			std::string Num = fd.fieldName.substr(fd.fieldName.find("[")+1);
			std::string fileName = fd.fieldName.substr(0,fd.fieldName.find("["));
			Num.erase(Num.size()-1, 1);

			fputs("		for(int i = 0; i < ", pOutFile);fputs(Num.c_str(), pOutFile);fputs("; i++)\n", pOutFile);
			fputs("		{\n", pOutFile);
			fputs("			", pOutFile);
			fputs(fileName.c_str(), pOutFile);
			fputs("[i].Read(bh);\n", pOutFile);
			fputs("		}\n", pOutFile);
		}

		fputs("", pOutFile);
	}

	fputs("		return ;\n", pOutFile);
	fputs("	}\n", pOutFile);

	return true;
}

bool ProcessWriteMethod(std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs("	public void Write(WriteBufferHelper bh)\n",pOutFile);
	fputs("	{\n", pOutFile);
	for(size_t m = 0; m < fdlist.size(); m++)
	{
		PacketField fd =fdlist.at(m);
		fputs("		", pOutFile);
		if(fd.fieldType == "UINT32")
		{
			fputs("bh.WriteUint32(", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(");\n", pOutFile);
		}
		else if(fd.fieldType == "UINT8")
		{
			fputs("bh.WriteUint8(", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(");\n", pOutFile);
		}
		else if(fd.fieldType == "CHAR")
		{
			fputs("bh.WriteFixString(", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(",32", pOutFile);
			fputs(");\n", pOutFile);
		}
		else if(fd.fieldType == "UINT16")
		{
			fputs("bh.WriteUint16(", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(");\n", pOutFile);
		}
		else if(fd.fieldType == "UINT64")
		{
			fputs("bh.WriteUint64(", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(");\n", pOutFile);
		}
		else if(fd.fieldType == "FLOAT")
		{
			fputs("bh.WriteFloat(", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(");\n", pOutFile);
		}
		else if(fd.fieldName.find("[") == std::string::npos)
		{
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs(".Write(bh);\n", pOutFile);
		}
		else
		{
			std::string Num = fd.fieldName.substr(fd.fieldName.find("[")+1);
			std::string fileName = fd.fieldName.substr(0,fd.fieldName.find("["));
			Num.erase(Num.size()-1, 1);

			fputs("for(int i = 0; i < ", pOutFile);fputs(Num.c_str(), pOutFile);fputs("; i++)\n", pOutFile);
			fputs("		{\n", pOutFile);
			fputs("			", pOutFile);
			fputs(fileName.c_str(), pOutFile);
			fputs("[i].Write(bh);\n", pOutFile);
			fputs("		}\n", pOutFile);

		}
	}
	fputs("		return ;\n", pOutFile);
	fputs("	}\n", pOutFile);

	return true;
}

bool ProcessPacket(PacketDef &Packet, FILE *pOutFile)
{
	fputs("public class ", pOutFile);
	fputs(Packet.PacketName.c_str(), pOutFile);
	fputs("\n", pOutFile);
	fputs("{\n", pOutFile);

	for(size_t m = 0; m < Packet.fieldList.size(); m++)
	{
		ProcessFieldDeclare(Packet.fieldList.at(m), pOutFile);
	}	

	ProcessReadMethod(Packet.fieldList, pOutFile);

	ProcessWriteMethod(Packet.fieldList, pOutFile);

	fputs("};\n", pOutFile);

	return true;
}


