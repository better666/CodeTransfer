using System; 
using System.Net; 
using System.Collections; 
using System.Collections.Generic;
using System.Text;
using System.Threading; 
using System.Runtime.InteropServices;

public class PacketReader
{
    static PacketReader pr;
    public static PacketReader instance()
    {
        if (pr == null)
        {
            pr = new PacketReader();
        }
        return pr;

    }
    PacketReader()
    {

    }
    private byte[] m_DataBuffer = null;

    public int m_TotalLen = 0;
	public int m_ReadPos  = 0;
	
	public byte[] GetData()
	{
        return m_DataBuffer;
	}
	
	//---------------------------------------------------------------------------------
    public Boolean BeginRead(byte[] data, int nSize)
    {
		m_DataBuffer = data;
        m_ReadPos = 0;
        m_TotalLen = nSize;

        return true;
    }

    public Boolean EndRead()
    {
        return true;
    }

	public SByte ReadInt8()
	{
        return (SByte)m_DataBuffer[m_ReadPos++];
	}

	public Int16 ReadInt16()
	{
        m_ReadPos += 2;
        return BitConverter.ToInt16(m_DataBuffer, m_ReadPos - 2);
	}
		
	public Int32 ReadInt32()
	{
        m_ReadPos += 4;
        return BitConverter.ToInt32(m_DataBuffer, m_ReadPos - 4);
	}

	public Int64 ReadInt64()
	{
        m_ReadPos += 8;
        return BitConverter.ToInt64(m_DataBuffer, m_ReadPos - 8);
	}
	
	public Byte ReadUint8()
	{
        return m_DataBuffer[m_ReadPos++];
	}

	public UInt16 ReadUint16()
	{
        m_ReadPos += 2;
        return BitConverter.ToUInt16(m_DataBuffer, m_ReadPos - 2);
	}

	public UInt32 ReadUint32()
	{
        m_ReadPos += 4;
        return BitConverter.ToUInt32(m_DataBuffer, m_ReadPos - 4);
	}
	
	public UInt64 ReadUint64()
	{
        m_ReadPos += 8;
        return BitConverter.ToUInt64(m_DataBuffer, m_ReadPos - 8);
	}
	
	public float ReadFloat()
	{
        m_ReadPos += 4;
        return BitConverter.ToSingle(m_DataBuffer, m_ReadPos - 4);
	}

	public double ReadDouble()
	{
        m_ReadPos += 8;
        return BitConverter.ToDouble(m_DataBuffer, m_ReadPos - 8);
	}
	
	public string ReadString()
	{
		int slen = ReadInt16();
        int offset = m_ReadPos;
		m_ReadPos += slen;
        return System.Text.Encoding.UTF8.GetString(m_DataBuffer, offset, slen);
	}

    public void ReadSkip(UInt32 v)
    {
        m_ReadPos += (int)v;
    }

    //---------------------------------------------------------------------------------
    public bool ReadEOF()
    {
        return (m_TotalLen - m_ReadPos) <= 0;
    }

    public int GetLeftSize()
    {
        return m_TotalLen - m_ReadPos;
    }

    public int GetDataLen()
    {
        return m_TotalLen;
    }

	//---------------------------------------------------------------------------------
	public byte[] GetBuffer()
	{
        byte[] buf = new byte[GetLeftSize()];
        Array.Copy(m_DataBuffer, m_ReadPos, buf, 0, GetLeftSize());
		return buf;
	}
	
	//---------------------------------------------------------------------------------
	public string toString()
	{
		string s = "";

		byte[] buf = GetBuffer();

		for(int i=0; i<buf.Length; i++)
		{
			s += buf[i];
			s += " ";
		}
		return s;
	}
}

public class PacketWriter
{
    PacketWriter() { }
    static PacketWriter instance;
    public static PacketWriter GetInstance()
    {
        if (instance==null)
        {
            instance = new PacketWriter();
        }
        return instance;
    }
      
    public const int BUFFER_MAX = 8096;
    private byte[] m_DataBuffer = new byte[BUFFER_MAX];

    public int m_WritePos = 0;

    public byte[] GetData()
    {
        return m_DataBuffer;
    }

    public void SetData(byte[] data)
    {
        m_DataBuffer = data;
    }

    //---------------------------------------------------------------------------------
    public void BeginWrite(short id)
    {
		m_WritePos = 0;
        WriteInt32(0);
		WriteInt16(0);
		WriteInt16(id);
    }

    public byte[] EndWrite()
    {   
        int temp = m_WritePos;
        m_WritePos = 0;
        WriteInt32(temp-8);
        m_WritePos = temp;
        return m_DataBuffer;
    }

    public void WriteInt8(SByte v)
    {
        m_DataBuffer[m_WritePos++] = (Byte)v;
    }

    public void WriteInt16(Int16 v)
    {
        WriteInt8((SByte)(v & 0xff));
        WriteInt8((SByte)(v >> 8 & 0xff));
    }

    public void WriteInt32(Int32 v)
    {
        for (int i = 0; i < 4; i++)
            WriteInt8((SByte)(v >> i * 8 & 0xff));
    }

    public void WriteInt64(Int64 v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    public void WriteUint8(Byte v)
    {
        m_DataBuffer[m_WritePos++] = v;
    }

    public void WriteUint16(UInt16 v)
    {
        WriteUint8((Byte)(v & 0xff));
        WriteUint8((Byte)(v >> 8 & 0xff));
    }

    public void WriteUint32(UInt32 v)
    {
        for (int i = 0; i < 4; i++)
            WriteUint8((Byte)(v >> i * 8 & 0xff));
    }

    public void WriteUint64(UInt64 v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    public void WriteFloat(float v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    public void WriteDouble(double v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    public void WriteString(string v)
    {
        if (v.Length > FreeSize())
        {
            return;
        }

        byte[] getdata = System.Text.Encoding.UTF8.GetBytes(v);
		WriteInt16((short)getdata.Length);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    //---------------------------------------------------------------------------------
    public UInt32 FreeSize()
    {
        return (UInt32)(BUFFER_MAX - m_WritePos);
    }
    //---------------------------------------------------------------------------------
    public int GetDataLen()
    {
        return m_WritePos;
    }

}


