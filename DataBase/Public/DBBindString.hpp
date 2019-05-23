/******************************************************
���ܣ�	���ݰ�ʱ������
���ߣ�	�￡
ʱ�䣺	2018-12-14
�޸ģ�
******************************************************/
#ifndef __DB_BIND_STRING_HPP__
#define __DB_BIND_STRING_HPP__

#include "DBBindBase.hpp"

class CDBBindString : public CDBBindBase
{
public:
	CDBBindString();
	CDBBindString(const CDBBindString& Other);
	CDBBindString(const char* strString);
	CDBBindString(const std::string& szString);
	~CDBBindString();

	CDBBindString& operator = (const CDBBindString& Other);
	CDBBindString& operator = (const char* strString);
	CDBBindString& operator = (const std::string& szString);

	bool SetBuffer(unsigned long lBufferLength);
	bool SetBufferAndValue(unsigned long lBufferLength, unsigned char* pValue, unsigned long lValueLength);
	bool SetValue(unsigned char* pValue, unsigned long lValueLength);
	const char* GetData() const;
	std::string ToString() const override;
	void Clear(bool bFreeAll = false) override;

private:
	bool Init() override;

	friend class CMySQLInterface;
	friend class COCIInterface;

private:
	char m_strBuffer[D_DB_STRING_MAX_LEN + 1]; //�ַ���

	unsigned int   m_iRawBufferLength; //RAW���ݻ��泤�ȣ�oraclʹ��
	unsigned int   m_iRawDataLength;   //RAW���ݳ��ȣ�oraclʹ��
	unsigned char* m_pRawData;         //RAW���ݣ�oraclʹ��

};

#endif