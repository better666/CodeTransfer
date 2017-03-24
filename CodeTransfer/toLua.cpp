#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
#include "transfer.h"


std::string GetLuaType(std::string tType)
{
	if (tType == "INT8") return " = 0";
	if (tType == "INT16") return "= 0";
	if (tType == "INT32") return "= 0";
	if (tType == "INT64") return "= 0";
	if (tType == "UINT8") return "= 0";
	if (tType == "UINT16") return "= 0";
	if (tType == "UINT32") return "= 0";
	if (tType == "UINT64") return "= 0";
	if (tType == "FLOAT") return "= 0";
	if (tType == "STRING") return "=\'\'";
	return tType;
}

bool ConvertFileToLua(const char *szSrcFile, const char *szDestFile)
{
	std::vector<std::string> m_WorldList;

	GetWorldList(szSrcFile, m_WorldList);

	std::vector<PacketDef> m_PakcetList;

	GetPacketList(m_WorldList, m_PakcetList);

	FILE *pOutFile = fopen(szDestFile,"w+");
	if(pOutFile == NULL)
	{
		printf("create %s file failure!!!", szDestFile);
		getchar();
		return false;
	}

	for(size_t j = 0; j < m_PakcetList.size(); j++)
	{
		PacketDef pf = m_PakcetList.at(j);
		ProcessPacketLua(pf, pOutFile);
	}

	fclose(pOutFile);

	return false;
}

bool ProcessFieldDeclareLua(PacketField &fd, FILE *pOutFile)
{	
	fputs(strTab.c_str(), pOutFile);
	if (fd.fieldName.find("[") == std::string::npos)
	{
		fputs(fd.fieldName.c_str(), pOutFile);
		fputs(" ", pOutFile);
		fputs(GetLuaType(fd.fieldType).c_str(), pOutFile);
	}
	else
	{
		std::string Num = fd.fieldName.substr(fd.fieldName.find("["));
		std::string SimpleName = fd.fieldName.substr(0,fd.fieldName.find("["));
		fputs(SimpleName.c_str(), pOutFile);
		//if (Num == "[1]")
		//{
		//	fputs("[]", pOutFile);
		//}
		//else
		//{
		//	fputs(fd.fieldName.substr(fd.fieldName.find("[")).c_str(), pOutFile);
		//}
		fputs(" ", pOutFile);
		fputs("= { }", pOutFile);
	}
	fputs(";", pOutFile);

	if (fd.fieldCmt.size()>0)
	{
		fputs(strTab.c_str(), pOutFile);
		fputs(strTab.c_str(), pOutFile);
		fputs("--", pOutFile);
		fputs(fd.fieldCmt.c_str(), pOutFile);
	}

	fputs("\n", pOutFile);
	return true;
}

bool ProcessReadMethodLua(std::string packname, std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs(strTab.c_str(), pOutFile);
	fputs("Read = function (self, reader) \n",pOutFile);
	for(size_t m = 0; m < fdlist.size(); m++)
	{
		fputs(strTab.c_str(), pOutFile);
		fputs(strTab.c_str(), pOutFile);
		PacketField fd = fdlist.at(m);
		if(fd.fieldName.find("[") == std::string::npos)
		{
			fputs("self.", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			if (GetLuaType(fd.fieldType) != fd.fieldType) 
			{
				fputs(" = ", pOutFile);
				fputs("reader:Read", pOutFile);
				fputs(G_TypeFuncMap[fd.fieldType].c_str(), pOutFile);
				fputs("()\n", pOutFile);
			}
			else
			{
				fputs(fd.fieldType.c_str(), pOutFile);
				fputs(".Read(reader)\n", pOutFile);
			}      
		}
		else
		{
			std::string Num = fd.fieldName.substr(fd.fieldName.find("[")+1);
			std::string fileName = fd.fieldName.substr(0,fd.fieldName.find("["));
			Num.erase(Num.size()-1, 1);
// 			if (Num =="1")
// 			{
// 				std::string countName = fileName+"_Cnt";
// 				Num = "self."+countName;
// 				fputs("self.", pOutFile);
// 				fputs(fileName.c_str(), pOutFile);
// 				fputs(" = make([]", pOutFile);
// 				fputs(GetLuaType(fd.fieldType).c_str(), pOutFile);
// 				fputs(",", pOutFile);
// 				fputs(Num.c_str(), pOutFile);
// 				fputs(")\n", pOutFile);
// 				fputs(strTab.c_str(), pOutFile);
// 				fputs(strTab.c_str(), pOutFile);
// 			}
			
			fputs("for i = 1,", pOutFile);fputs(Num.c_str(), pOutFile);fputs(",1 do \n", pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			///////////////////////////////
			if (GetLuaType(fd.fieldType) != fd.fieldType) 
			{
				fputs("self.", pOutFile);
				fputs(fileName.c_str(), pOutFile);
				fputs("[i] = reader:Read", pOutFile);
				fputs(G_TypeFuncMap[fd.fieldType] .c_str(), pOutFile);
				fputs("()\n", pOutFile);
			}
			else
			{
				fputs("self.", pOutFile);
				fputs(fileName.c_str(), pOutFile);
				fputs("[i] = ", pOutFile);
				fputs(fd.fieldType.c_str(), pOutFile);
				fputs(".Read(reader)\n", pOutFile);
			}     
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs("end\n", pOutFile);
		}
	}
	fputs("	end;\n", pOutFile);

	return true;
}

bool ProcessWriteMethodLua(std::string packname, std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs(strTab.c_str(), pOutFile);
	fputs("Write = function (self, writer) \n",pOutFile);
	for(size_t m = 0; m < fdlist.size(); m++)
	{
		PacketField fd =fdlist.at(m);
		fputs(strTab.c_str(), pOutFile);
		fputs(strTab.c_str(), pOutFile);
		if(fd.fieldName.find("[") == std::string::npos)
		{
			if (GetLuaType(fd.fieldType) != fd.fieldType) 
			{
				fputs("writer:Write", pOutFile);
				fputs(G_TypeFuncMap[fd.fieldType].c_str(), pOutFile);
				fputs("(self.", pOutFile);
				fputs(fd.fieldName.c_str(), pOutFile);
				fputs(")\n", pOutFile);
			}
			else
			{
				fputs("self.", pOutFile);
				fputs(fd.fieldName.c_str(), pOutFile);
				fputs(".Write(writer)\n", pOutFile);
			} 
		}
		else
		{
			std::string Num = fd.fieldName.substr(fd.fieldName.find("[")+1);
			std::string fileName = fd.fieldName.substr(0,fd.fieldName.find("["));
			Num.erase(Num.size()-1, 1);
			if (Num == "1")
			{
				std::string countName = "self." + fileName+"_Cnt";
				Num = countName;
			}

			fputs("for i = 1,", pOutFile);fputs(Num.c_str(), pOutFile);fputs(",1 do\n", pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			//////
			if (GetLuaType(fd.fieldType) != fd.fieldType) 
			{
				fputs("writer:Write", pOutFile);
				fputs(G_TypeFuncMap[fd.fieldType].c_str(), pOutFile);
				fputs("(self.", pOutFile);
				fputs(fileName.c_str(), pOutFile);
				fputs("[i]);\n", pOutFile);
			}
			else
			{
				fputs("self.", pOutFile);
				fputs(fileName.c_str(), pOutFile);
				fputs("[i]:Write(writer)\n", pOutFile);
			} 
			//////
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs("end\n", pOutFile);

		}
	}
	fputs("	end;\n\n", pOutFile);

	return true;
}

bool ProcessPacketLua(PacketDef &Packet, FILE *pOutFile)
{
	if (Packet.PacketCmt.size() > 0) {
		fputs("--", pOutFile);
		fputs(Packet.PacketCmt.c_str(), pOutFile);
		fputs("\n", pOutFile);
	}

	fputs(Packet.PacketName.c_str(), pOutFile);
	fputs(" =\n{\n", pOutFile);
	for(size_t m = 0; m < Packet.fieldList.size(); m++)
	{
		ProcessFieldDeclareLua(Packet.fieldList.at(m), pOutFile);
	}	
	
	ProcessReadMethodLua(Packet.PacketName, Packet.fieldList, pOutFile);
	ProcessWriteMethodLua(Packet.PacketName, Packet.fieldList, pOutFile);

	fputs("}\n\n", pOutFile);

	return true;
}