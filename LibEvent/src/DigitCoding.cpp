#include "../include/DigitCoding.hpp"

bool CDigitCoding::m_bIsBigEndian = true;
bool CDigitCoding::m_bCPUEndian = CDigitCoding::CheckCpuEndian();

CDigitCoding::CDigitCoding()
{
}

CDigitCoding::~CDigitCoding()
{
}

/*********************************************************************
���ܣ�	���ô�С�����ͣ������ֽ���Ϊ���
������	bEndian ��С�����ͣ�true����ˣ�false��С��
���أ�	��
�޸ģ�
*********************************************************************/
void CDigitCoding::SetEndianType(bool bEndian)
{
	m_bIsBigEndian = bEndian;
}

/*********************************************************************
���ܣ�	��ȡ��С������
������	��
���أ�	true����ˣ�false��С��
�޸ģ�
*********************************************************************/
bool CDigitCoding::GetEndianType()
{
	return m_bIsBigEndian;
}

/*********************************************************************
���ܣ�	���CPU��С������
������	��
���أ�	true����ˣ�false��С��
�޸ģ�
*********************************************************************/
bool CDigitCoding::CheckCpuEndian()
{
	UINT16_UNION w;
	w.u16 = 0x1122;

	return (0x11 == w.b[0]);
}

char* CDigitCoding::put_int8(char *buf, my_int8_ data)
{
	if (nullptr == buf)
		return nullptr;

	*buf = data;

	return (buf + my_int8_len);
}
const char* CDigitCoding::get_int8(const char *buf, my_int8_ *data)
{
	if (nullptr == buf || nullptr == data)
		return nullptr;

	*data = *buf;

	return (buf + my_int8_len);
}

char* CDigitCoding::put_uint8(char *buf, my_uint8_ data)
{
	if (nullptr == buf)
		return nullptr;

	*buf = data;

	return (buf + my_int8_len);
}
const char* CDigitCoding::get_uint8(const char *buf, my_uint8_ *data)
{
	if (nullptr == buf || nullptr == data)
		return nullptr;

	*data = *buf;

	return (buf + my_int8_len);
}

char* CDigitCoding::put_int16(char *buf, my_int16_ data)
{
	if (nullptr == buf)
		return nullptr;

	INT16_UNION u;
	u.i16 = data;

	for (unsigned int i = 0; i < my_int16_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			buf[i] = u.b[i];
		else
			buf[i] = u.b[my_int16_len - i - 1];
	}

	return (buf + my_int16_len);
}
const char* CDigitCoding::get_int16(const char *buf, my_int16_ *data)
{
	if (nullptr == buf || nullptr == data)
		return nullptr;

	INT16_UNION u;

	for (unsigned int i = 0; i < my_int16_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			u.b[i] = buf[i];
		else
			u.b[my_int16_len - i - 1] = buf[i];
	}

	*data = u.i16;

	return (buf + my_int16_len);
}

char* CDigitCoding::put_uint16(char *buf, my_uint16_ data)
{
	if (nullptr == buf)
		return nullptr;

	UINT16_UNION u;
	u.u16 = data;

	for (unsigned int i = 0; i < my_int16_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			buf[i] = u.b[i];
		else
			buf[i] = u.b[my_int16_len - i - 1];
	}

	return (buf + my_int16_len);
}
const char* CDigitCoding::get_uint16(const char *buf, my_uint16_ *data)
{
	if (nullptr == buf || nullptr == data)
		return nullptr;

	UINT16_UNION u;

	for (unsigned int i = 0; i < my_int16_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			u.b[i] = buf[i];
		else
			u.b[my_int16_len - i - 1] = buf[i];
	}

	*data = u.u16;

	return (buf + my_int16_len);
}

char* CDigitCoding::put_int32(char *buf, my_int32_ data)
{
	if (nullptr == buf)
		return nullptr;

	INT32_UNION u;
	u.i32 = data;

	for (unsigned int i = 0; i < my_int32_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			buf[i] = u.b[i];
		else
			buf[i] = u.b[my_int32_len - i - 1];
	}

	return (buf + my_int32_len);
}
const char* CDigitCoding::get_int32(const char *buf, my_int32_ *data)
{
	if (nullptr == buf || nullptr == data)
		return nullptr;

	INT32_UNION u;

	for (unsigned int i = 0; i < my_int32_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			u.b[i] = buf[i];
		else
			u.b[my_int32_len - i - 1] = buf[i];
	}

	*data = u.i32;

	return (buf + my_int32_len);
}

char* CDigitCoding::put_uint32(char *buf, my_uint32_ data)
{
	if (nullptr == buf)
		return nullptr;

	UINT32_UNION u;
	u.u32 = data;

	for (unsigned int i = 0; i < my_int32_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			buf[i] = u.b[i];
		else
			buf[i] = u.b[my_int32_len - i - 1];
	}

	return (buf + my_int32_len);
}
const char* CDigitCoding::get_uint32(const char *buf, my_uint32_ *data)
{
	if (nullptr == buf || nullptr == data)
		return nullptr;

	UINT32_UNION u;

	for (unsigned int i = 0; i < my_int32_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			u.b[i] = buf[i];
		else
			u.b[my_int32_len - i - 1] = buf[i];
	}

	*data = u.u32;

	return (buf + my_int32_len);
}

char* CDigitCoding::put_int64(char *buf, my_int64_ data)
{
	if (nullptr == buf)
		return nullptr;

	INT64_UNION u;
	u.i64 = data;

	for (unsigned int i = 0; i < my_int64_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			buf[i] = u.b[i];
		else
			buf[i] = u.b[my_int64_len - i - 1];
	}

	return (buf + my_int64_len);
}
const char* CDigitCoding::get_int64(const char *buf, my_int64_ *data)
{
	if (nullptr == buf || nullptr == data)
		return nullptr;

	INT64_UNION u;

	for (unsigned int i = 0; i < my_int64_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			u.b[i] = buf[i];
		else
			u.b[my_int64_len - i - 1] = buf[i];
	}

	*data = u.i64;

	return (buf + my_int64_len);
}

char* CDigitCoding::put_uint64(char *buf, my_uint64_ data)
{
	if (nullptr == buf)
		return nullptr;

	UINT64_UNION u;
	u.u64 = data;

	for (unsigned int i = 0; i < my_int64_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			buf[i] = u.b[i];
		else
			buf[i] = u.b[my_int64_len - i - 1];
	}

	return (buf + my_int64_len);
}
const char* CDigitCoding::get_uint64(const char *buf, my_uint64_ *data)
{
	if (nullptr == buf || nullptr == data)
		return nullptr;

	UINT64_UNION u;

	for (unsigned int i = 0; i < my_int64_len; i++)
	{
		if (m_bIsBigEndian == m_bCPUEndian)
			u.b[i] = buf[i];
		else
			u.b[my_int64_len - i - 1] = buf[i];
	}

	*data = u.u64;

	return (buf + my_int64_len);
}

char* CDigitCoding::put_str(char *buf, const char *in, unsigned int in_len)
{
	if (nullptr == buf)
		return nullptr;

	//bufҪ����������ռ�
	if (nullptr != in)
		memcpy(buf, in, in_len);
	else
		memset(buf, 0, in_len);

	return (buf + in_len);
}
char* CDigitCoding::put_str(char *buf, const unsigned char *in, unsigned int in_len)
{
	return put_str(buf, (const char*)in, in_len);
}
const char* CDigitCoding::get_str(const char *buf, char *out, unsigned int out_len)
{
	if (nullptr == buf)
		return nullptr;

	//outҪ����������ռ�
	if (nullptr != out)
		memcpy(out, buf, out_len);

	return (buf + out_len);
}
const char* CDigitCoding::get_str(const char *buf, unsigned char *out, unsigned int out_len)
{
	return get_str(buf, (char*)out, out_len);
}

/*********************************************************************
���ܣ�	16���ƴ�תchar*��������0x
������	hex 16���ƴ�
*		hex_len 16���ƴ�����
*		data ת���buf
*		data_len ת���buf���泤��
���أ�	ת�󴮵ĳ���
�޸ģ�
*********************************************************************/
unsigned int CDigitCoding::from_hex(const char* hex, unsigned int hex_len, char *data, unsigned int data_len)
{
	if (nullptr == hex || 1 >= hex_len || nullptr == data || 0 == data_len)
		return 0;

	unsigned int out_len = 0;
	char h = 0, l = 0;

	for (unsigned int i = 0; i < hex_len; i += 2)
	{
		h = FromHexChar(hex[i]);
		l = FromHexChar(hex[i + 1]);
		data[out_len++] = (h << 4) | l;

		//��ֹԽ��
		if (out_len >= data_len)
			break;
	}

	return out_len;
}

/*********************************************************************
���ܣ�	char*ת16���ƴ�������0x
������	data char*��
*		data_len char*������
*		isLowercase �Ƿ�Сд��ĸ
���أ�	ת��Ĵ�
�޸ģ�
*********************************************************************/
std::string CDigitCoding::get_hex(const unsigned char* data, unsigned int data_len, bool isLowercase)
{
	std::string szReturn;

	if (nullptr == data || 0 >= data_len)
		return std::move(szReturn);

	int n = 0;
	char *pStrTemp = new char[data_len * 2 + 1]{ '\0' };

	for (unsigned int i = 0; i < data_len; ++i)
	{
		n += snprintf(pStrTemp + n, 3, (isLowercase ? "%02x" : "%02X"), data[i]);
	}

	pStrTemp[n] = '\0';
	szReturn = pStrTemp;
	delete [] pStrTemp;

	return std::move(szReturn);
}

std::string CDigitCoding::get_hex(my_int16_ nData, bool isLowercase)
{
	char buf[my_int16_len] = { '\0' };
	put_int16(buf, nData);
	return get_hex(buf, sizeof(buf), isLowercase);
}

std::string CDigitCoding::get_hex(my_int32_ iData, bool isLowercase)
{
	char buf[my_int32_len] = { '\0' };
	put_int32(buf, iData);
	return get_hex(buf, sizeof(buf), isLowercase);
}

std::string CDigitCoding::get_hex(my_int64_ llData, bool isLowercase)
{
	char buf[my_int64_len] = { '\0' };
	put_int64(buf, llData);
	return get_hex(buf, sizeof(buf), isLowercase);
}

std::string CDigitCoding::get_hex(my_uint16_ unData, bool isLowercase)
{
	char buf[my_int16_len] = { '\0' };
	put_uint16(buf, unData);
	return get_hex(buf, sizeof(buf), isLowercase);
}

std::string CDigitCoding::get_hex(my_uint32_ uiData, bool isLowercase)
{
	char buf[my_int32_len] = { '\0' };
	put_uint32(buf, uiData);
	return get_hex(buf, sizeof(buf), isLowercase);
}

std::string CDigitCoding::get_hex(my_uint64_ ullData, bool isLowercase)
{
	char buf[my_int64_len] = { '\0' };
	put_uint64(buf, ullData);
	return get_hex(buf, sizeof(buf), isLowercase);
}

/*********************************************************************
���ܣ�	char*ת16���ƴ�������0x
������	data char*��
*		data_len char*������
*		isLowercase �Ƿ�Сд��ĸ
���أ�	ת��Ĵ�
�޸ģ�
*********************************************************************/
std::string CDigitCoding::get_hex(const char* data, unsigned int data_len, bool isLowercase)
{
	return get_hex((const unsigned char*)data, data_len, isLowercase);
}

/*********************************************************************
���ܣ�	���ֽڵ�16���ƴ�ת����,������0x
������	sz2BytesHex ���ֽڵ�16���ƴ�
���أ�	ת�������
�޸ģ�
*********************************************************************/
char CDigitCoding::from_hex(const std::string sz2BytesHex)
{
	if (sz2BytesHex.length() >= 2)
	{
		char h = FromHexChar(sz2BytesHex[0]);
		char l = FromHexChar(sz2BytesHex[1]);

		return ((h << 4) & l);
	}
	else
		return 0;
}

/*********************************************************************
���ܣ�	����ת���ֽ�16���ƴ�������0x
������	data Ҫת������
���أ�	ת����ַ���
�޸ģ�
*********************************************************************/
std::string CDigitCoding::get_hex(const unsigned char data)
{
	std::string szRet;

	szRet = GetHexChar((const char)(data >> 4)); //ǰ4λ
	szRet += GetHexChar((const char)(data & 0x0F)); //��4λ

	return std::move(szRet);
}

/*********************************************************************
���ܣ�	��ȡ16���Ƶ��ַ�
������	ch 1�ֽ�ǰ���4λ��ֵ
*		isLowercase �Ƿ�Сд��ĸ
���أ�	ת����ַ�
�޸ģ�
*********************************************************************/
char CDigitCoding::GetHexChar(const char &ch, bool isLowercase)
{
	switch (ch)
	{
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	case 10:
		return (isLowercase ? 'a' : 'A');
	case 11:
		return (isLowercase ? 'b' : 'B');
	case 12:
		return (isLowercase ? 'c' : 'C');
	case 13:
		return (isLowercase ? 'd' : 'D');
	case 14:
		return (isLowercase ? 'e' : 'E');
	default:
		break;
	}

	return (isLowercase ? 'f' : 'F');
}

/*********************************************************************
���ܣ�	16���Ƶ��ַ�ת��
������	ch 1�ֽ�16�����ַ�
���أ�	ת�����
�޸ģ�
*********************************************************************/
char CDigitCoding::FromHexChar(const char &ch)
{
	switch (ch)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'a':
	case 'A':
		return 10;
	case 'b':
	case 'B':
		return 11;
	case 'c':
	case 'C':
		return 12;
	case 'd':
	case 'D':
		return 13;
	case 'e':
	case 'E':
		return 14;
	default:
		break;
	}

	return 15;
}