#include "stdafx.h"
#include <string>
#include <vector>
#include "transfer.h"
#include <map>

std::string GetCsType(std::string tType)
{
	if (tType == "INT8") return "SByte";
	if (tType == "INT16") return "Int16";
	if (tType == "INT32") return "Int32";
	if (tType == "INT64") return "Int64";
	if (tType == "UINT8") return "Byte";
	if (tType == "UINT16") return "Uint16";
	if (tType == "UINT32") return "Uint32";
	if (tType == "UINT64") return "Uint64";
	if (tType == "FLOAT") return "Single";
	if (tType == "STRING") return "string";

	return tType;
}

bool ConvertFileToCsharp(const char *szSrcFile, const char *szDestFile)
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

	fputs("using System;\n\r", pOutFile);
	for(size_t j = 0; j < m_PakcetList.size(); j++)
	{
		PacketDef pf = m_PakcetList.at(j);
		ProcessPacket(pf, pOutFile);
	}

	fclose(pOutFile);

	return false;
}

bool ProcessFieldDeclare(PacketField &fd, FILE *pOutFile)
{	
    fputs(strTab.c_str(), pOutFile);
	fputs("public ", pOutFile);
	if(fd.fieldName.find("[") == std::string::npos)
	{
		 if (GetCsType(fd.fieldType) != fd.fieldType) 
		{
			fputs(GetCsType(fd.fieldType).c_str(), pOutFile);
			fputs(" ", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
		}
		else
		{
			fputs(GetCsType(fd.fieldType).c_str(), pOutFile);
			fputs(" ", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
			fputs("= new ", pOutFile);
			fputs(GetCsType(fd.fieldType).c_str(), pOutFile);
			fputs("()", pOutFile);
        }      
	}
	else 
	{
		std::string SimpleName = fd.fieldName.substr(0,fd.fieldName.find("["));
		fputs(GetCsType(fd.fieldType).c_str(), pOutFile);
		fputs(" [] ", pOutFile);
		fputs(SimpleName.c_str(), pOutFile);
		fputs(" ", pOutFile);
        if (fd.fieldName.substr(fd.fieldName.find("[")) == "[1]")
        {
            fputs("= null", pOutFile);
        }
        else
        {
            fputs("= new ", pOutFile);
            fputs(GetCsType(fd.fieldType).c_str(), pOutFile);
            fputs(fd.fieldName.substr(fd.fieldName.find("[")).c_str(), pOutFile);
        }
		
	}

	fputs(";", pOutFile);
	if (fd.fieldCmt.size() >0 ){
		fputs(strTab.c_str(), pOutFile);
		fputs(strTab.c_str(), pOutFile);
		fputs(fd.fieldCmt.c_str(), pOutFile);
	}
	fputs("\n", pOutFile);
	return true;
}

bool ProcessReadMethod(std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs(strTab.c_str(), pOutFile);
	fputs("public void Read(PacketReader reader)\n",pOutFile);
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
				fputs("reader.Read", pOutFile);
				fputs(it->second.c_str(), pOutFile);
				fputs("();\n", pOutFile);
            }
			else
			{
				fputs(fd.fieldName.c_str(), pOutFile);
				fputs(".Read(reader);\n", pOutFile);
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
                  fputs(GetCsType(fd.fieldType).c_str(), pOutFile);
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
                fputs("[i] = reader.Read", pOutFile);
                 fputs(it->second.c_str(), pOutFile);
                  fputs("();\n", pOutFile);
            }
			else
			{
                fputs(fileName.c_str(), pOutFile);
                fputs("[i] = new ", pOutFile);
                fputs(fd.fieldType.c_str(), pOutFile);
                fputs("();\n ", pOutFile);
                fputs("			", pOutFile);
                fputs(fileName.c_str(), pOutFile);
                fputs("[i].Read(reader);\n", pOutFile);
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

bool ProcessWriteMethod(std::vector<PacketField> &fdlist, FILE *pOutFile)
{
     fputs(strTab.c_str(), pOutFile);
	fputs("public void Write(PacketWriter writer)\n",pOutFile);
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
                    fputs("writer.Write", pOutFile);
                   fputs(it->second.c_str(), pOutFile);
                    fputs("(", pOutFile);
                    fputs(fd.fieldName.c_str(), pOutFile);
                    fputs(");\n", pOutFile);
                }
                else
                {
                    fputs(fd.fieldName.c_str(), pOutFile);
                    fputs(".Write(writer);\n", pOutFile);
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
                        fputs("writer.Write", pOutFile);
                        fputs(it->second.c_str(), pOutFile);
                        fputs("(", pOutFile);
                        fputs(fileName.c_str(), pOutFile);
                        fputs("[i]);\n", pOutFile);
                    }
                    else
                    {
                        fputs(fileName.c_str(), pOutFile);
                        fputs("[i].Write(writer);\n", pOutFile);
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

bool ProcessPacket(PacketDef &Packet, FILE *pOutFile)
{
	fputs(Packet.PacketCmt.c_str(), pOutFile);
	if (Packet.PacketCmt.size() > 0) {
		fputs("\n", pOutFile);
	}
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

	fputs("};\n\n", pOutFile);

	return true;
}


