#include <string>
#include <map>

struct PacketField
{
	std::string fieldType;
	std::string fieldName;
};


extern std::map<std::string, std::string> G_TypeFuncMap;
extern std::string strTab;
extern std::string strBlank;


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
bool ProcessPacketGo(PacketDef &Packet, FILE *pOutFile);

bool ConvertFileToCsharp(char *szSrcFile, char *szDestFile);
bool ConvertFileToGo(char *szSrcFile, char *szDestFile);

bool InitTypeFuncMap();