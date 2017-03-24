#include "stdafx.h"
#include <string>
#include <vector>
#include "transfer.h"
#include <map>

std::string GetCppType(std::string tType)
{
	if (tType == "INT8") return "INT8";
	if (tType == "INT16") return "INT16";
	if (tType == "INT32") return "INT32";
	if (tType == "INT64") return "INT64";
	if (tType == "UINT8") return "UINT8";
	if (tType == "UINT16") return "UINT16";
	if (tType == "UINT32") return "UINT32";
	if (tType == "UINT64") return "UINT64";
	if (tType == "FLOAT") return "FLOAT";
	if (tType == "STRING") return "string";

	return tType;
}

bool ConvertFileToCpp(const char *szSrcFile, const char *szDestFile)
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

	fputs("#include \"stdafx.h>\"\n\r", pOutFile);
	for(size_t j = 0; j < m_PakcetList.size(); j++)
	{
		PacketDef pf = m_PakcetList.at(j);
		ProcessPacketCpp(pf, pOutFile);
	}

	fclose(pOutFile);

	return false;
}

bool ProcessFieldDeclareCpp(PacketField &fd, FILE *pOutFile)
{	
	fputs(strTab.c_str(), pOutFile);
	fputs(GetCppType(fd.fieldType).c_str(), pOutFile);
	fputs(" ", pOutFile);
	fputs(fd.fieldName.c_str(), pOutFile);
	fputs(";", pOutFile);
	if (fd.fieldCmt.size() >0 ){
		fputs(strTab.c_str(), pOutFile);
		fputs(strTab.c_str(), pOutFile);
		fputs(fd.fieldCmt.c_str(), pOutFile);
	}
	fputs("\n", pOutFile);
	return true;
}

bool ProcessReadMethodCpp(std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs(strTab.c_str(), pOutFile);
	fputs("void Read(PacketReader *pReader)\n",pOutFile);
	fputs(strTab.c_str(), pOutFile);
	fputs("{\n", pOutFile);
	for(size_t m = 0; m < fdlist.size(); m++)
	{
		fputs(strTab.c_str(), pOutFile);
		fputs(strTab.c_str(), pOutFile);
		PacketField fd = fdlist.at(m);
		if(fd.fieldName.find("[") == std::string::npos)
		{
			std::map<std::string, std::string> ::iterator it = G_TypeFuncMap.find(fd.fieldType);
			if (it != G_TypeFuncMap.end()) 
			{
				fputs(fd.fieldName.c_str(), pOutFile);
				fputs(" = ", pOutFile);
				fputs("pReader->Read", pOutFile);
				fputs(it->second.c_str(), pOutFile);
				fputs("();\n", pOutFile);
			}
			else
			{
				fputs(fd.fieldName.c_str(), pOutFile);
				fputs(".Read(pReader);\n", pOutFile);
			}       
		}
		else
		{
			std::string Num = fd.fieldName.substr(fd.fieldName.find("[")+1);
			std::string fileName = fd.fieldName.substr(0,fd.fieldName.find("["));
			Num.erase(Num.size()-1, 1);

			if (Num == "1")
			{
				std::string countName = fileName+"_Cnt";
				Num = countName;
				fputs(fileName.c_str(), pOutFile);
				fputs(" = new ", pOutFile);
				fputs(GetCppType(fd.fieldType).c_str(), pOutFile);
				fputs("[", pOutFile);
				fputs(countName.c_str(), pOutFile);
				fputs("];\n", pOutFile);
				fputs(strTab.c_str(), pOutFile);
				fputs(strTab.c_str(), pOutFile);
			}
			fputs("for(int i = 0; i < ", pOutFile);fputs(Num.c_str(), pOutFile);fputs("; i++)\n", pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs("{\n", pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);


			std::map<std::string, std::string> ::iterator it = G_TypeFuncMap.find(fd.fieldType);
			if (it != G_TypeFuncMap.end()) 
			{
				fputs(fileName.c_str(), pOutFile);
				fputs("[i] = pReader->Read", pOutFile);
				fputs(it->second.c_str(), pOutFile);
				fputs("();\n", pOutFile);
			}
			else
			{
				fputs(fileName.c_str(), pOutFile);
				fputs("[i].Read(pReader);\n", pOutFile);
			} 
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs("}\n", pOutFile);
		}
	}

	fputs(strTab.c_str(), pOutFile);
	fputs(strTab.c_str(), pOutFile);
	fputs("return ;\n", pOutFile);
	fputs(strTab.c_str(), pOutFile);
	fputs("}\n", pOutFile);

	return true;
}

bool ProcessWriteMethodCpp(std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs(strTab.c_str(), pOutFile);
	fputs("void Write(PacketWriter *pWriter)\n",pOutFile);
	fputs(strTab.c_str(), pOutFile);
	fputs("{\n", pOutFile);
	for(size_t m = 0; m < fdlist.size(); m++)
	{
		fputs(strTab.c_str(), pOutFile);
		fputs(strTab.c_str(), pOutFile);
		PacketField fd =fdlist.at(m);
		if(fd.fieldName.find("[") == std::string::npos)
		{
			std::map<std::string, std::string> ::iterator it = G_TypeFuncMap.find(fd.fieldType);
			if (it != G_TypeFuncMap.end()) 
			{
				fputs("pWriter->Write", pOutFile);
				fputs(it->second.c_str(), pOutFile);
				fputs("(", pOutFile);
				fputs(fd.fieldName.c_str(), pOutFile);
				fputs(");\n", pOutFile);
			}
			else
			{
				fputs(fd.fieldName.c_str(), pOutFile);
				fputs(".Write(pWriter);\n", pOutFile);
			}       
		}
		else
		{
			std::string Num = fd.fieldName.substr(fd.fieldName.find("[")+1);
			std::string fileName = fd.fieldName.substr(0,fd.fieldName.find("["));
			Num.erase(Num.size()-1, 1);
			if (Num == "1")
			{
				std::string countName = fileName+"_Cnt";
				Num = countName;
			}
			fputs("for(int i = 0; i < ", pOutFile);fputs(Num.c_str(), pOutFile);fputs("; i++)\n", pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs("{\n", pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);

			std::map<std::string, std::string> ::iterator it = G_TypeFuncMap.find(fd.fieldType);
			if (it != G_TypeFuncMap.end()) 
			{
				fputs("pWriter->Write", pOutFile);
				fputs(it->second.c_str(), pOutFile);
				fputs("(", pOutFile);
				fputs(fileName.c_str(), pOutFile);
				fputs("[i]);\n", pOutFile);
			}
			else
			{
				fputs(fileName.c_str(), pOutFile);
				fputs("[i].Write(pWriter);\n", pOutFile);
			}       
			fputs(strTab.c_str(), pOutFile);
			fputs(strTab.c_str(), pOutFile);
			fputs("}\n", pOutFile);
		}
	}
	fputs(strTab.c_str(), pOutFile);
	fputs(strTab.c_str(), pOutFile);
	fputs("return ;\n", pOutFile);
	fputs(strTab.c_str(), pOutFile);
	fputs("}\n", pOutFile);

	return true;
}

bool ProcessPacketCpp(PacketDef &Packet, FILE *pOutFile)
{
	if (Packet.PacketCmt.size() > 0) {
		fputs(Packet.PacketCmt.c_str(), pOutFile);
		fputs("\n", pOutFile);
	}
	fputs("class ", pOutFile);
	fputs(Packet.PacketName.c_str(), pOutFile);
	fputs("\n", pOutFile);
	fputs("{\n", pOutFile);

	for(size_t m = 0; m < Packet.fieldList.size(); m++)
	{
		ProcessFieldDeclareCpp(Packet.fieldList.at(m), pOutFile);
	}	

	ProcessReadMethodCpp(Packet.fieldList, pOutFile);
	ProcessWriteMethodCpp(Packet.fieldList, pOutFile);

	fputs("};\n\n", pOutFile);

	return true;
}


