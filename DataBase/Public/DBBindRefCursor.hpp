/******************************************************
���ܣ�	oracle��SYS_REFCURSOR��
���ߣ�	�￡
ʱ�䣺	2018-12-18
�޸ģ�
******************************************************/
#ifndef __DB_BIND_REF_CURSOR_HPP__
#define __DB_BIND_REF_CURSOR_HPP__

#include "DBBindBase.hpp"

class CDBBindRefCursor : public CDBBindBase
{
public:
	CDBBindRefCursor();
	CDBBindRefCursor(const CDBBindRefCursor& Other) = delete;
	~CDBBindRefCursor();
	CDBBindRefCursor& operator = (const CDBBindRefCursor& Other) = delete;

	std::string ToString() const override;
	void Clear(bool bFreeAll = false) override;

private:
	bool Init() override;

	friend class COCIInterface;

private:
	void *m_pOCIErr;     //OCI����������ʱȷ��
	void *m_pRefCursor;  //������OCIStmt*����ʱȷ��

};

#endif