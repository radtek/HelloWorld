/******************************************************
���ܣ�	���ݰ�ʱ������
���ߣ�	�￡
ʱ�䣺	2018-12-14
�޸ģ�
******************************************************/
#ifndef __DB_BIND_DATE_TIME__
#define __DB_BIND_DATE_TIME__

#include "DBBindBase.hpp"

class CDBBindDateTime : public CDBBindBase
{
public:
	CDBBindDateTime();
	CDBBindDateTime(const CDBBindDateTime& Other) = delete;
	CDBBindDateTime(const char* strDateTime);
	CDBBindDateTime(const std::string& szDateTime);
	~CDBBindDateTime();

	CDBBindDateTime& operator = (const CDBBindDateTime& Other) = delete;
	CDBBindDateTime& operator = (const char* strDateTime);
	CDBBindDateTime& operator = (const std::string& szDateTime);

	bool CheckFormat(EDBDataType eDateTimeType, const char* strDateTime = nullptr);
	const void* GetData() const;
	std::string ToString() const override;
	void Clear(bool bFreeAll = false) override;

private:
	bool Init() override;
	bool BindOCIDateTime(); //��OCI��ʱ���
	bool CheckIsLeapYear(unsigned int iYear); //����Ƿ�������
	bool CheckDate(unsigned int iYear, unsigned int iMonth, unsigned int iDay); //��������Ƿ�����
	bool CheckTime(unsigned int iHour, unsigned int iMinute, unsigned int iSecond); //���ʱ���Ƿ�����

	friend class CMySQLInterface;
	friend class COCIInterface;

private:
	char        m_strBuffer[D_DB_DATETIME_MAX_LEN + 1]; //����ʱ���ַ���
	void        *m_pOCIEnv;                             //oracle OCI�������
	void        *m_pOCIErr;                             //oracle OCI������
	EDBDataType m_eOCIDateTimeType;                     //oracle ����ʱ�����ͣ�date��timestamp����һ�ΰ�ʱȷ��
	void        *m_pOCITimestamp;                       //oracle��timestampʹ��

};

#endif