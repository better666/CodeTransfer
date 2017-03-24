#include <string>
#include <map>

struct PacketField
{
	std::string fieldType;
	std::string fieldName;
	std::string fieldCmt;
};


extern std::map<std::string, std::string> G_TypeFuncMap;
extern std::string strTab;
extern std::string strBlank;


struct PacketDef
{
	std::string PacketCmt; 
	std::string PacketName;
	std::vector<PacketField> fieldList;
};

#define MAX_LINE_LEN 256

enum PackState
{
	ps_none,
	ps_name,
	ps_field
};

enum FieldState
{
	fs_type,
	fs_name,
};

bool GetWorldList(const char *szFile, std::vector<std::string> &WordList);
bool GetPacketList(std::vector<std::string> &WorldList, std::vector<PacketDef> &PacketList);
bool ProcessPacketCs(PacketDef &Packet, FILE *pOutFile);
bool ProcessPacketGo(PacketDef &Packet, FILE *pOutFile);
bool ProcessPacketCpp(PacketDef &Packet, FILE *pOutFile);
bool ProcessPacketLua(PacketDef &Packet, FILE *pOutFile);

bool ConvertFileToCsharp(const char *szSrcFile, const char *szDestFile);
bool ConvertFileToGo(const char *szSrcFile, const char *szDestFile);
bool ConvertFileToCpp(const char *szSrcFile, const char *szDestFile);
bool ConvertFileToLua(const char *szSrcFile, const char *szDestFile);

bool InitTypeFuncMap();
std::string GetConfigName();