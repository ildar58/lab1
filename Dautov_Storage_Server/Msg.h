#pragma once

enum Messages
{
	M_INIT,
	M_EXIT,
	M_GETDATA,
	M_NODATA,
	M_DATA,
	M_CONFIRM,
	M_GET_ALL_DATA
};

enum Members
{
	M_BROKER = 0,
	M_ALL = 10,
	M_STORAGE = 1000
};

struct MsgHeader
{
	unsigned int m_To;
	unsigned int m_From;
	unsigned int m_Type;
	unsigned int m_Size;
};

struct Message
{
	MsgHeader m_Header;
	string m_Data;

	Message()
	{
		m_Header = { 0 };
	}

	Message(unsigned int To, unsigned int From, unsigned int Type = M_DATA, const string& Data = "")
		:m_Data(Data)
	{
		m_Header = { To, From, Type, Data.length() };
	}

	void Send(CSocket& s)
	{
		s.Send(&m_Header, sizeof(MsgHeader));
		if (m_Header.m_Size)
		{
			s.Send(m_Data.c_str(), m_Header.m_Size);
		}
	}

	int Receive(CSocket& s)
	{
		if (!s.Receive(&m_Header, sizeof(MsgHeader)))
		{
			return M_NODATA;
		}
		if (m_Header.m_Size)
		{
			vector <char> v(m_Header.m_Size);
			s.Receive(&v[0], m_Header.m_Size);
			m_Data = string(&v[0], m_Header.m_Size);
		}
		return m_Header.m_Type;
	}

	static void Send(CSocket& s, unsigned int To, unsigned int From, unsigned int Type = M_DATA, const string& Data = "");
	static Message Send(unsigned int Type = M_DATA, const string& Data = "");
};