#ifndef _PACKET_HELPER_H_
#define _PACKET_HELPER_H_

class CReadHelper
{
public:
	CReadHelper()
	{
		m_pDataBuffer = NULL;
		m_dwTotalLen = 0;
		m_dwCurPos = 0;
	}

	~CReadHelper(void)
	{
		m_pDataBuffer = NULL;
		m_dwTotalLen = 0;
		m_dwCurPos = 0;
	}

public:
	
	BOOL BeginRead(CHAR *pBuf, UINT32 nSize)
	{
		m_pDataBuffer = pBuf;

		m_dwTotalLen = nSize;

		m_dwCurPos = 0;
		
	}

	CHAR*		GetCurrentPoint()
	{
		return (CHAR*)(m_pDataBuffer + m_dwCurPos);
	}

	BOOL Seek( INT32 nOffset )
	{
		m_dwCurPos += nOffset;

		return TRUE;
	}

	template<typename T>
	UINT32		Read(T &_value);

	UINT32		Read(std::string &strValue)
	{
		CHAR  szValue[1024];

		UINT32 dwSize = Read(szValue);

		strValue = szValue;

		return dwSize;
	}


	UINT32		Read(CHAR *pszValue)
	{
		ASSERT(pszValue != NULL);

		UINT16 wLen = 0;

		Read(wLen);

		memcpy(pszValue, m_pDataBuffer->GetBuffer() + m_dwCurPos, wLen);

		pszValue[wLen] = 0;

		m_dwCurPos += wLen;

		return wLen + sizeof(UINT16);
	}

	UINT32	Read(BYTE *pData, UINT32 dwBytes)
	{
		ASSERT(pData != NULL);

		memcpy(pData, m_pDataBuffer->GetBuffer() + m_dwCurPos, dwBytes);

		m_dwCurPos += dwBytes;

		return dwBytes;
	}

private:
	CHAR *m_pDataBuffer;

	UINT32 m_dwCurPos;

	UINT32 m_dwTotalLen;
};

template<typename T>
UINT32 CReadHelper::Read( T &_value )
{
	if((m_dwCurPos+sizeof(T)) > m_dwTotalLen)
	{
		return 0;
	}

	_value = *(T*)(m_pDataBuffer + m_dwCurPos);

	m_dwCurPos   += sizeof(T);

	return sizeof(T);
}

class CWriteHelper
{
public:
	CWriteHelper(CHAR *pBuf, UINT32 nSize)
	{
		m_pDataBuffer = pBuf;

		m_dwTotalLen = nSize;

		m_dwCurPos = 0;
	}

	~CWriteHelper(void)
	{
		m_pDataBuffer = NULL;

		m_dwTotalLen = 0;

		m_dwCurPos = 0;
	}

public:
	BOOL	BeginWrite(UINT16 wCommandID, UINT16 dwSceneID, UINT64 u64CharID)
	{
		if(m_pDataBuffer == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}


		Write(0xff);
		Write(wCommandID);
		Write(dwSceneID);
		Write(u64CharID);

		return TRUE;
	}


	BOOL		EndWrite()
	{
		WriteCheckBufferCode();

		GetPacketHeader()->dwSize = m_dwCurPos;

		m_pDataBuffer->SetTotalLenth(m_dwCurPos);

		return TRUE;
	}

	CHAR*		GetCurrentPoint()
	{
		return (CHAR*)(m_pDataBuffer + m_dwCurPos);
	}

	BOOL Seek( INT32 nOffset )
	{
		m_dwCurPos += nOffset;

		return TRUE;
	}

	template <typename T>
	UINT32		Write(T& _Value);

	UINT32		Write(std::string &strValue)
	{
		return Write(strValue.c_str());
	}

	UINT32		Write(CHAR *pszValue)
	{
		UINT16 wLen = strlen(pszValue)+1;

		Write(wLen);

		memcpy(m_pDataBuffer->GetBuffer() + m_dwCurPos, pszValue,  wLen);

		m_dwCurPos += wLen;

		ASSERT((m_dwCurPos+100) <= m_pDataBuffer->GetBufferSize());

		return wLen + sizeof(UINT16);
	}

	UINT32		Write(const CHAR *pszValue)
	{
		UINT16 wLen = strlen(pszValue)+1;

		Write(wLen);

		memcpy(m_pDataBuffer->GetBuffer() + m_dwCurPos, pszValue,  wLen);

		m_dwCurPos += wLen;

		ASSERT((m_dwCurPos+100) <= m_pDataBuffer->GetBufferSize());

		return wLen + sizeof(UINT16);
	}

	UINT32		Write(BYTE *pData, UINT32 dwBytes)
	{
		ASSERT(pData != NULL);

		memcpy(m_pDataBuffer->GetBuffer() + m_dwCurPos, pData,  dwBytes);

		m_dwCurPos += dwBytes;

		ASSERT((m_dwCurPos+100) <= m_pDataBuffer->GetBufferSize());

		return dwBytes;
	}

private:
	CHAR *m_pDataBuffer;

	UINT32 m_dwCurPos;

	UINT32 m_dwTotalLen;
};

template <typename T>
UINT32 CWriteHelper::Write( T& _Value )
{
	T *pValue = (T*)(m_pDataBuffer->GetBuffer()+m_dwCurPos);

	*pValue = _Value;

	m_dwCurPos += sizeof(T);

	ASSERT((m_dwCurPos+100) <= m_pDataBuffer->GetBufferSize());

	return sizeof(T);
}

#endif
