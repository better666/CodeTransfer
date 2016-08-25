#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
#include "transfer.h"


std::string GetGoType(std::string tType)
{
	if (tType == "INT8") return "int8";
	if (tType == "INT16") return "int16";
	if (tType == "INT32") return "int32";
	if (tType == "INT64") return "int64";
	if (tType == "UINT8") return "uint8";
	if (tType == "UINT16") return "uint16";
	if (tType == "UINT32") return "uint32";
	if (tType == "UINT64") return "uint64";
	if (tType == "FLOAT") return "float32";
	if (tType == "STRING") return "string";
	return tType;
}

bool ConvertFileToGo(const char *szSrcFile, const char *szDestFile)
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

	fputs("package msg;\n\r", pOutFile);

	for(size_t j = 0; j < m_PakcetList.size(); j++)
	{
		PacketDef pf = m_PakcetList.at(j);
		ProcessPacketGo(pf, pOutFile);
	}

	fclose(pOutFile);

	return false;
}

bool ProcessFieldDeclareGo(PacketField &fd, FILE *pOutFile)
{	
	fputs(strTab.c_str(), pOutFile);
	//非数组的变量的处理。
	if(fd.fieldName.find("[") == std::string::npos)
	{
		fputs(fd.fieldName.c_str(), pOutFile);
		fputs(" ", pOutFile);
		fputs(GetGoType(fd.fieldType).c_str(), pOutFile);
	}
	else //有数组的变量的处理。
	{
		std::string Num = fd.fieldName.substr(fd.fieldName.find("["));
		std::string SimpleName = fd.fieldName.substr(0,fd.fieldName.find("["));
		fputs(SimpleName.c_str(), pOutFile);
		if (Num == "[1]")
		{
			fputs("[]", pOutFile);
		}
		else
		{
			fputs(fd.fieldName.substr(fd.fieldName.find("[")).c_str(), pOutFile);
		}

		fputs(" ", pOutFile);
		fputs(GetGoType(fd.fieldType).c_str(), pOutFile);
	}

	if (fd.fieldCmt.size() >0 ){
		fputs(strTab.c_str(), pOutFile);
		fputs(strTab.c_str(), pOutFile);
		fputs(fd.fieldCmt.c_str(), pOutFile);
	}

	fputs("\n", pOutFile);
	return true;
}

bool ProcessReadMethodGo(std::string packname, std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs("func (self *",pOutFile);
    fputs(packname.c_str(),pOutFile);
    fputs(") Read(reader *PacketReader) bool {\n",pOutFile);
	for(size_t m = 0; m < fdlist.size(); m++)
	{
         fputs(strTab.c_str(), pOutFile);
		PacketField fd = fdlist.at(m);
		if(fd.fieldName.find("[") == std::string::npos)
		{
			fputs("self.", pOutFile);
			fputs(fd.fieldName.c_str(), pOutFile);
            if (GetGoType(fd.fieldType) != fd.fieldType) 
            {
				fputs(" = ", pOutFile);
				fputs("reader.Read", pOutFile);
				fputs(G_TypeFuncMap[fd.fieldType].c_str(), pOutFile);
				fputs("()\n", pOutFile);
            }
            else
            {
                fputs(".Read(reader)\n", pOutFile);
            }      
		}
		else
		{
			std::string Num = fd.fieldName.substr(fd.fieldName.find("[")+1);
			std::string fileName = fd.fieldName.substr(0,fd.fieldName.find("["));
			Num.erase(Num.size()-1, 1);
            if (Num =="1")
			{
				std::string countName = fileName+"_Cnt";
				Num = "self."+countName;
				fputs("self.", pOutFile);
				fputs(fileName.c_str(), pOutFile);
				fputs(" = make([]", pOutFile);
				fputs(GetGoType(fd.fieldType).c_str(), pOutFile);
				fputs(",", pOutFile);
				fputs(Num.c_str(), pOutFile);
				fputs(")\n", pOutFile);
				fputs(strTab.c_str(), pOutFile);
            }

			fputs("for i := 0; i < int(", pOutFile);fputs(Num.c_str(), pOutFile);fputs("); i++ {\n", pOutFile);
            fputs(strTab.c_str(), pOutFile);
            fputs(strTab.c_str(), pOutFile);
            ///////////////////////////////
            if (GetGoType(fd.fieldType) != fd.fieldType) 
            {
                fputs("self.", pOutFile);
                fputs(fileName.c_str(), pOutFile);
                fputs("[i] = reader.Read", pOutFile);
                fputs(G_TypeFuncMap[fd.fieldType] .c_str(), pOutFile);
               fputs("()\n", pOutFile);
            }
            else
            {
                fputs("self.", pOutFile);
                fputs(fileName.c_str(), pOutFile);
                fputs("[i].Read(reader)\n", pOutFile);
            }     
			 fputs(strTab.c_str(), pOutFile);
			fputs("}\n", pOutFile);
		}
	}
	fputs(strTab.c_str(), pOutFile);
	fputs("return true\n", pOutFile);
	fputs("}\n\n", pOutFile);

	return true;
}

bool ProcessWriteMethodGo(std::string packname, std::vector<PacketField> &fdlist, FILE *pOutFile)
{
	fputs("func (self *",pOutFile);
    fputs(packname.c_str(),pOutFile);
    fputs(") Write(writer *PacketWriter) {\n",pOutFile);
	for(size_t m = 0; m < fdlist.size(); m++)
	{
		PacketField fd =fdlist.at(m);
		 fputs(strTab.c_str(), pOutFile);
		if(fd.fieldName.find("[") == std::string::npos)
		{
            if (GetGoType(fd.fieldType) != fd.fieldType) 
            {
                 fputs("writer.Write", pOutFile);
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

			fputs("for i := 0; i < int(", pOutFile);fputs(Num.c_str(), pOutFile);fputs("); i++ {\n", pOutFile);
		    fputs(strTab.c_str(), pOutFile);
             fputs(strTab.c_str(), pOutFile);
            //////
          if (GetGoType(fd.fieldType) != fd.fieldType) 
            {
                 fputs("writer.Write", pOutFile);
                 fputs(G_TypeFuncMap[fd.fieldType].c_str(), pOutFile);
                fputs("(self.", pOutFile);
                fputs(fileName.c_str(), pOutFile);
                fputs("[i]);\n", pOutFile);
            }
            else
            {
                fputs("self.", pOutFile);
                fputs(fileName.c_str(), pOutFile);
                fputs("[i].Write(writer)\n", pOutFile);
            } 
            //////
			fputs(strTab.c_str(), pOutFile);
			fputs("}\n", pOutFile);
			
		}
	}
	fputs(strTab.c_str(), pOutFile);
	fputs("return\n", pOutFile);
	fputs("}\n\n", pOutFile);

	return true;
}

bool ProcessPacketGo(PacketDef &Packet, FILE *pOutFile)
{
	fputs(Packet.PacketCmt.c_str(), pOutFile);
	if (Packet.PacketCmt.size() > 0) {
		fputs("\n", pOutFile);
	}
	fputs("type ", pOutFile);
	fputs(Packet.PacketName.c_str(), pOutFile);
	fputs(" struct {\n", pOutFile);
	for(size_t m = 0; m < Packet.fieldList.size(); m++)
	{
		ProcessFieldDeclareGo(Packet.fieldList.at(m), pOutFile);
	}	
    fputs("}\n\n", pOutFile);

   ProcessReadMethodGo(Packet.PacketName, Packet.fieldList, pOutFile);
	ProcessWriteMethodGo(Packet.PacketName, Packet.fieldList, pOutFile);

	return true;
}