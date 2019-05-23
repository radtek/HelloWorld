/******************************************************
���ܣ�	���ݿ�����ֲ���
���ߣ�	�￡
ʱ�䣺	2018-12-18
�޸ģ�
******************************************************/
#ifndef __DB_BIND_DIGIT__
#define __DB_BIND_DIGIT__

#include "DBBindBase.hpp"

class CDBBindNumber : public CDBBindBase
{
public:
	//���ֲ���
	union UDigitParm
	{
		int8_t   _char;
		uint8_t  _uchar;
		int16_t  _short;
		uint16_t _ushort;
		int32_t  _int;
		uint32_t _uint;
		int64_t  _longlong;
		uint64_t _ulonglong;
		float    _float;
		double   _double;
	};

	//�󶨲�����������
	enum ENumberDataType
	{
		E_NUMBER_DATA_NONE   = 0,
		E_NUMBER_DATA_INT8   = 1,
		E_NUMBER_DATA_INT16  = 2,
		E_NUMBER_DATA_INT32  = 3,
		E_NUMBER_DATA_INT64  = 4,
		E_NUMBER_DATA_FLOAT  = 5,
		E_NUMBER_DATA_DOUBLE = 6,
		E_NUMBER_DATA_STRING = 7
	};

public:
	CDBBindNumber();
	CDBBindNumber(const CDBBindNumber& Other);
	CDBBindNumber(const char* strNumber);
	CDBBindNumber(const std::string& szNumber);
	CDBBindNumber(const int8_t& ch);
	CDBBindNumber(const uint8_t& uch);
	CDBBindNumber(const int16_t& st);
	CDBBindNumber(const uint16_t& ust);
	CDBBindNumber(const int32_t& it);
	CDBBindNumber(const uint32_t& uit);
	CDBBindNumber(const long& lg);
	CDBBindNumber(const unsigned long& ulg);
	CDBBindNumber(const int64_t& ll);
	CDBBindNumber(const uint64_t& ull);
	CDBBindNumber(const float& ft);
	CDBBindNumber(const double& dt);
	~CDBBindNumber();

	CDBBindNumber& operator = (const CDBBindNumber& Other);
	CDBBindNumber& operator = (const char* strNumber);
	CDBBindNumber& operator = (const std::string& szNumber);
	CDBBindNumber& operator = (const int8_t& ch);
	CDBBindNumber& operator = (const uint8_t& uch);
	CDBBindNumber& operator = (const int16_t& st);
	CDBBindNumber& operator = (const uint16_t& ust);
	CDBBindNumber& operator = (const int32_t& it);
	CDBBindNumber& operator = (const uint32_t& uit);
	CDBBindNumber& operator = (const long& lg);
	CDBBindNumber& operator = (const unsigned long& ulg);
	CDBBindNumber& operator = (const int64_t& ll);
	CDBBindNumber& operator = (const uint64_t& ull);
	CDBBindNumber& operator = (const float& ft);
	CDBBindNumber& operator = (const double& de);

	void* GetDataPtr(EDBDataType eType);
	void* GetDataPtr(ENumberDataType eType);
	unsigned long GetDataLength(EDBDataType eType);
	unsigned long GetDataLength(ENumberDataType eType);
	const UDigitParm& GetData() const;
	void SetFloatScale(unsigned short scale); //���ø�����
	std::string ToString() const override;
	void Clear(bool bFreeAll = false) override;

private:
	bool Init() override;
	unsigned short ReckonScale(const char* strNumber, unsigned long length);
	void ConvertStrToDigit(EDBDataType eType);

	friend class CMySQLInterface;
	friend class COCIInterface;

private:
	ENumberDataType  m_eDataType; //��������
	bool             m_bUnsigned; //�Ƿ����޷��ŵ�
	UDigitParm       m_UData;     //��ͨ��������
	char             m_strBuffer[D_DB_NUMBER_MAX_LEN + 1]; //decimal/number�ַ���
	unsigned short   m_nScale;    //���ȣ�С������λ����oracleʹ��

};

#endif