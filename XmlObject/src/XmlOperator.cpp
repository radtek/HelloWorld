#include "../include/XmlOperator.hpp"
using namespace GDASK;

//�����ʼ���ļ�·�����������ĵ�����
CXmlOperator::CXmlOperator():m_pXmlDoc(new TiXmlDocument)
{

}

//�����ĵ�����
CXmlOperator::~CXmlOperator()
{
	if (NULL != m_pXmlDoc)
	{
		delete m_pXmlDoc;
		m_pXmlDoc = NULL;
	}
}

//��ȡ�ļ�·��
const char* CXmlOperator::getXmlPath() const
{
	return m_szFilePath.c_str();
}

//��ȡ�ļ���
const char* CXmlOperator::getXmlName() const
{
	return m_szFileName.c_str();
}

//���������ļ�
bool CXmlOperator::loadFile(const string &szFileName, const string &szFilePath, string &szError)
{
	bool bSucc;

	m_pXmlDoc->Clear();		//m_pXmlDoc������ΪNULL

	m_szFilePath = szFilePath;
	if ('/' != m_szFilePath.at(m_szFilePath.length()-1))
	{
		m_szFilePath += '/';
	}
	m_szFileName = szFileName;

	bSucc = m_pXmlDoc->LoadFile(m_szFilePath+m_szFileName);
	if (!bSucc)
	{
		m_pXmlDoc->Clear();
		szError = m_pXmlDoc->ErrorDesc();
	}

	return bSucc;
}

//��������ļ�
bool CXmlOperator::saveFile(const string &szFileName, const string &szFilePath, string &szError) const
{
	//�ĵ���ʽ�Ƿ��д�
	if (m_pXmlDoc->Error())
	{
		szError = m_pXmlDoc->ErrorDesc();
		szError += ",fail to save file!";
		return false;
	}

	bool bSucc;
	string szTemp(szFilePath);
	if ('/' != szTemp.at(szTemp.length()-1))
	{
		szTemp += '/';
	}
	szTemp += szFileName;

	bSucc = m_pXmlDoc->SaveFile(szTemp);
	if (!bSucc)
	{
		szError = "fail to save file!";
	}

	return bSucc;
}

//�ַ������س��ĵ�����
bool CXmlOperator::loadStr(const string &szStrXml, string &szError)
{
	bool bSucc = true;

	m_pXmlDoc->Clear();
	m_pXmlDoc->Parse(szStrXml.c_str());
	if (m_pXmlDoc->Error())
	{
		bSucc = false;
		szError = m_pXmlDoc->ErrorDesc();
		m_pXmlDoc->Clear();
	}

	return bSucc;
}

//�ĵ����󱣴浽�ַ���
bool CXmlOperator::saveStr(string &szStrXml, string &szError) const
{
	bool bSucc = false;

	TiXmlPrinter printer;
	if (m_pXmlDoc->Accept(&printer))
	{
		bSucc = true;
		szStrXml = printer.CStr();
	}
	else
	{
		szError = m_pXmlDoc->ErrorDesc();
	}

	return bSucc;
}

// ��ȡ�ڵ�����
string CXmlOperator::getElemName(TiXmlElement* pCurElem) const
{
	return (NULL == pCurElem)?"":pCurElem->ValueStr();
}

//��ȡ���ڵ�
TiXmlElement* CXmlOperator::getRootElem() const
{
	return m_pXmlDoc->RootElement();
}

//��ȡָ���ڵ�Ԫ����һ����ָ����ͬ���ڵ�Ԫ��
TiXmlElement* CXmlOperator::getPreElem(TiXmlElement* pCurElem, const char* pcName /* = NULL */) const
{
	if (NULL != pCurElem)
	{
		TiXmlNode* pcNode = pCurElem;
		if (NULL != pcName)
		{
			do
			{
				//ֱ����ǰ�ҵ���Ԫ�صĽڵ㣬���Ҳ���
				pcNode = pcNode->PreviousSibling(pcName);
			} while (NULL != pcNode && TiXmlNode::TINYXML_ELEMENT != pcNode->Type());
		}
		else
		{
			do
			{
				pcNode = pcNode->PreviousSibling();
			} while (NULL != pcNode && TiXmlNode::TINYXML_ELEMENT != pcNode->Type());
		}

		if (NULL != pcNode)
		{
			return pcNode->ToElement();
		}
	}

	return NULL;
}

//��ȡָ���ڵ�Ԫ����һ(ָ��)ͬ���ڵ�Ԫ��
TiXmlElement* CXmlOperator::getNextElem(TiXmlElement* pPreElem, const char* pcName /* = NULL */) const
{
	if (NULL != pPreElem)
	{
		return (NULL == pcName)?pPreElem->NextSiblingElement():pPreElem->NextSiblingElement(pcName);
	}
	return NULL;
}

//��ȡָ���ڵ�Ԫ�ص�һ����ָ�����ӽڵ�Ԫ��
TiXmlElement* CXmlOperator::getFirstChildElem(TiXmlElement* pParElem, const char* pcName /* = NULL */) const
{
	if (NULL != pParElem)
	{
		return (NULL == pcName)?pParElem->FirstChildElement():pParElem->FirstChildElement(pcName);
	}
	return NULL;
}

//��ȡָ���ڵ�Ԫ�����һ����ָ�����ӽڵ�Ԫ��
TiXmlElement* CXmlOperator::getLastChildElem(TiXmlElement* pParElem, const char* pcName /* = NULL */) const
{
	if (NULL == pParElem)
	{
		return NULL;
	}

	//��ȡ����һ���ӽڵ�
	TiXmlElement* pSearchElem = pParElem->FirstChildElement();
	if (NULL != pSearchElem)
	{
		if (NULL != pcName)
		{
			//ѭ������ȡ
			while(pSearchElem->NextSiblingElement(pcName))
			{
				pSearchElem = pSearchElem->NextSiblingElement(pcName);
			}
		}
		else
		{
			while(pSearchElem->NextSiblingElement())
			{
				pSearchElem = pSearchElem->NextSiblingElement();
			}
		}

		return pSearchElem;
	}
	return NULL;
}

//��ȡָ���ڵ���һ����ָ����ͬ���ڵ�
TiXmlNode* CXmlOperator::getPreNode(TiXmlNode* pCurNode, const char* pcName /* = NULL */) const
{
	if (NULL != pCurNode)
	{
		return (NULL == pcName)?pCurNode->PreviousSibling():pCurNode->PreviousSibling(pcName);
	}
	return NULL;
}

//��ȡָ���ڵ���һ����ָ����ͬ���ڵ�
TiXmlNode* CXmlOperator::getNextNode(TiXmlNode* pCurNode, const char* pcName /* = NULL */) const
{
	if (NULL != pCurNode)
	{
		return (NULL == pcName)?pCurNode->NextSibling():pCurNode->NextSibling(pcName);
	}

	return NULL;
}

//��ȡָ���ڵ�ĵ�һ����ָ�����ӽڵ�
TiXmlNode* CXmlOperator::getFirstChildNode(TiXmlNode* pParNode, const char* pcName /* = NULL */) const
{
	if (NULL != pParNode)
	{
		return (NULL == pcName)?pParNode->FirstChild():pParNode->FirstChild(pcName);
	}
	return NULL;
}

//��ȡָ���ڵ�����һ����ָ�����ӽڵ�
TiXmlNode* CXmlOperator::getLastChildNode(TiXmlNode* pParNode, const char* pcName /* = NULL */) const
{
	if (NULL != pParNode)
	{
		return (NULL == pcName)?pParNode->LastChild():pParNode->LastChild(pcName);
	}
	return NULL;
}

//��ȡָ���ڵ�ĸ��ڵ�
TiXmlNode* CXmlOperator::getParent(TiXmlNode* pCurNode) const
{
	if (NULL != pCurNode)
	{
		return pCurNode->Parent();
	}
	return NULL;
}

//��ӡ���XML�ĵ�����
void CXmlOperator::printDoc() const
{
	if (NULL != m_pXmlDoc)
	{
		m_pXmlDoc->Print();
	}
}

//��ȡָ���ڵ�Ԫ�ذ������ı�ֵ
const char* CXmlOperator::getElemText(TiXmlElement* pElem) const
{
	const char* cpResult = NULL;
	if (NULL != pElem)
	{
		cpResult = pElem->GetText();
		return (NULL==cpResult)?"":cpResult;
	}
	return "";
}

//��ȡָ���ڵ�Ԫ�صģ�ָ�������Ե�����ֵ
string CXmlOperator::getElemAttriValue(const TiXmlElement* pElem,
	const char* pcName) const
{
	const char* cpResult = NULL;
	if ((NULL != pElem) && (NULL != pcName))
	{
		cpResult = pElem->Attribute(pcName);
		return (NULL==cpResult)?"":cpResult;
	}
	printf("param error!\n");
	return "";
}

//��ȡָ���ڵ�Ԫ�صĵ�һ�����Զ���
TiXmlAttribute* CXmlOperator::getFirstAttribute(TiXmlElement* pElem) const
{
	if (NULL != pElem)
	{
		return pElem->FirstAttribute();
	}

	return NULL;
}

//��ȡָ���ڵ�Ԫ�ص����һ�����Զ���
TiXmlAttribute* CXmlOperator::getLastAttribute(TiXmlElement* pElem) const
{
	if (NULL != pElem)
	{
		return pElem->LastAttribute();
	}
	return NULL;
}

//��Ӹ��ڵ�
TiXmlElement* CXmlOperator::addRootElement(const char* pcRootName)
{
	if (NULL != pcRootName)
	{
		//�������ڵ�
		TiXmlElement* pRootElement = new TiXmlElement(pcRootName);
		if (NULL != pRootElement)
		{
			m_pXmlDoc->LinkEndChild(pRootElement);
			return m_pXmlDoc->RootElement();
		}
	}
	return NULL;
}

//��ָ���Ľڵ�Ԫ���������ӽڵ�Ԫ��(�ڵ��ı�ȱʡ)
TiXmlElement* CXmlOperator::addChildElement(TiXmlElement* pParElem,
	const char* pcChilName, const char* pcChilText /* = NULL */)
{
	//�ڵ�Ԫ��ָ��������ڵ����ƶ���Ϊ��
	if ((NULL != pParElem) && (NULL != pcChilName))
	{
		TiXmlElement* pChildElem = new TiXmlElement(pcChilName);
		if (NULL != pChildElem)
		{
			pParElem->LinkEndChild(pChildElem);
			//�ڵ��ı���Ϊ��
			if (NULL != pcChilText)
			{
				addElemText(pChildElem, pcChilText);
			}
			return pChildElem;
		}
	}
	return NULL;
}

//����XML�ĵ�����
bool CXmlOperator::addDeclaration(const char* pcVersion /* = "1.0" */,
	const char* pcEnCoding /* = "gb2312" */, const char* pcStandalone /* = "Yes" */)
{
	if ((NULL != pcVersion) && (NULL != pcEnCoding) && (NULL != pcStandalone))
	{
		TiXmlDeclaration* pDeclaration = new TiXmlDeclaration(pcVersion, pcEnCoding, pcStandalone);
		if (NULL != pDeclaration)
		{
			m_pXmlDoc->LinkEndChild(pDeclaration);
			return true;
		}
	}
	return false;
}

//����XML�ĵ�ע��
bool CXmlOperator::addComment(TiXmlNode* pElem, const char* pcComment)
{
	if ((NULL != pElem) && (NULL != pcComment))
	{
		TiXmlComment* pComment = new TiXmlComment(pcComment);
		pElem->LinkEndChild(pComment);
		return true;
	}
	return false;
}

//Ϊָ���ڵ�Ԫ������ı�
bool CXmlOperator::addElemText(TiXmlElement* pElem, const char* pcText)
{
	if ((NULL != pElem) && (NULL != pcText))
	{
		TiXmlText* pText = new TiXmlText(pcText);
		pElem->LinkEndChild(pText);
		return true;
	}
	return false;
}

//Ϊָ���ڵ�Ԫ����� ���� �� ����ֵ
bool CXmlOperator::addElemAttribute(TiXmlElement* pElem,
	const char* pcAttriName, const char* pcAttriValue)
{
	if ((NULL != pElem) && (NULL != pcAttriName) && (NULL != pcAttriValue))
	{
		pElem->SetAttribute(pcAttriName, pcAttriValue);
		return true;
	}
	return false;
}

//��ָ�����ӽڵ�λ��ǰ����ڵ�
TiXmlNode* CXmlOperator::insertBeforeChild(TiXmlNode* pFather,
	TiXmlNode* pBefThis, const TiXmlNode& txAddThis)
{
	if ((NULL != pFather) && (NULL != pBefThis) && (NULL != pFather))
	{
		return pFather->InsertBeforeChild(pBefThis, txAddThis);;
	}
	return NULL;
}

//��ָ�����ӽڵ�λ�ú����ڵ�
TiXmlNode* CXmlOperator::insertAfterChild(TiXmlNode* pFather,
	TiXmlNode* pAftThis, const TiXmlNode& txAddThis)
{
	if ((NULL != pFather) && (NULL != pAftThis))
	{
		return pFather->InsertAfterChild(pAftThis, txAddThis);
	}
	return NULL;
}

//�滻ָ�����ӽڵ�
TiXmlNode* CXmlOperator::replaceChild(TiXmlNode* pFather,
	TiXmlNode* pReplaceThis, const TiXmlNode& txWithThis)
{
	if ((NULL != pFather) && (NULL != pReplaceThis))
	{
		return pFather->ReplaceChild(pReplaceThis, txWithThis);
	}
	return NULL;
}

//ɾ��ָ�����ӽڵ�
bool CXmlOperator::removeChild(TiXmlNode* pFathNode, TiXmlNode* pRemoveThis)
{
	if ((NULL != pFathNode) && (NULL != pRemoveThis))
	{
		return pFathNode->RemoveChild(pRemoveThis);
	}
	return false;
}

//ɾ��ָ���ڵ�Ԫ�ص�ָ������
bool CXmlOperator::removeAttribute(TiXmlElement* pElem, const char* pcName)
{
	if ((NULL != pElem) && (NULL != pcName))
	{
		pElem->RemoveAttribute(pcName);
		return true;
	}
	return false;
}

//ɾ��ָ���ڵ�Ԫ�ص������ӽڵ�
bool CXmlOperator::clear(TiXmlElement* pElem)
{
	if (NULL != pElem)
	{
		pElem->Clear();
		return true;
	}
	return false;
}

void CXmlOperator::ClearDocument()
{
	m_pXmlDoc->Clear();
}

//�޸�ָ���ڵ�Ԫ�ص��ı�
bool CXmlOperator::modifyElemText(TiXmlElement* pElem, const char* pcNewValue)
{
	if ((NULL != pElem) && (NULL != pcNewValue))
	{
		TiXmlNode* pOldNode = NULL;
		TiXmlNode* pResult = NULL;

		//��ȡ��һ�ı��ڵ�
		pOldNode = pElem->FirstChild();
		if (NULL != pOldNode)
		{
			TiXmlText newText(pcNewValue);
			//�滻���µ��ı��ڵ�
			pResult = pElem->ReplaceChild(pOldNode, newText);
			if (NULL != pResult)
			{
				return true;
			}
		}
	}
	return false;
}

//�޸�ָ���ڵ�Ԫ�صģ�ָ�������Ե�����ֵ
bool CXmlOperator::modifyElemAttri(TiXmlElement* pElem,
	const char* pcAttriName, const char* pcAttriValue)
{
	if ((NULL != pElem) && (NULL != pcAttriName) && (NULL != pcAttriValue ))
	{
		TiXmlAttribute* pOldAttribute = NULL;
		//��ȡ��һ������ָ��
		pOldAttribute = pElem->FirstAttribute();
		while(NULL != pOldAttribute)
		{
			if (0 == strcmp(pOldAttribute->Name(), pcAttriName))
			{
				pOldAttribute->SetValue(pcAttriValue);
				return true;
			}
			pOldAttribute = pOldAttribute->Next();
		}
	}
	return false;
}

//ֱ�ӷ��ؽڵ��������һ���ӽڵ�ĸ���
int CXmlOperator::countChildNode(TiXmlNode* pNode,
	string szNodeName/* ="" */) const
{
	int iCount = 0;
	if (NULL != pNode)
	{
		TiXmlNode* pCurrentNode = NULL;

		if (strcmp("",szNodeName.c_str()))
		{
			//��ȡ��һ���ڵ�ָ��
			pCurrentNode = pNode->FirstChild(szNodeName);
			while (NULL != pCurrentNode)
			{
				++iCount;
				pCurrentNode = pCurrentNode->NextSibling(szNodeName);
			}
		}
		else
		{
			//��ȡ��һ���ڵ�ָ��
			pCurrentNode = pNode->FirstChild();
			while (NULL != pCurrentNode)
			{
				++iCount;
				pCurrentNode = pCurrentNode->NextSibling();
			}
		}
	}

	return iCount;
}

//ֱ�ӷ��ؽڵ��������һ���ӽڵ�Ԫ�صĸ���
int CXmlOperator::countChildElem(TiXmlElement* pElem,
	string szElemName /* = "" */) const
{
	int iCount = 0;
	if (NULL != pElem)
	{
		TiXmlElement* pCurrentElem = NULL;

		if (strcmp("",szElemName.c_str()))
		{
			//��ȡ��һ���ڵ�Ԫ��ָ��
			pCurrentElem = pElem->FirstChildElement(szElemName);
			while (NULL != pCurrentElem)
			{
				++iCount;
				pCurrentElem = pCurrentElem->NextSiblingElement(szElemName);
			}
		}
		else
		{
			pCurrentElem = pElem->FirstChildElement();
			while (NULL != pCurrentElem)
			{
				++iCount;
				pCurrentElem = pCurrentElem->NextSiblingElement();
			}
		}
	}

	return iCount;
}

//ֱ�Ӹ��ݽڵ�ָ�����ӽڵ����ŵõ��ӽڵ��ָ��
TiXmlNode* CXmlOperator::getChildByCount(TiXmlNode* pNode, const int iOrder) const
{
	int iMaxCount = 0;
	int iCount = 1;

	if (NULL != pNode)
	{
		//�����ӽڵ����
		iMaxCount = countChildNode(pNode);
		pNode = pNode->FirstChild();
		if ((iOrder > 0) && (iMaxCount >= iOrder))
		{
			for (; iCount<iOrder; ++iCount)
			{
				pNode = pNode->NextSibling();
			}
			return pNode;
		}
	}

	return NULL;
}

//ֱ�Ӹ��ݽڵ�ָ����ӽڵ�����Ƶõ��ӽڵ��ָ��
TiXmlNode* CXmlOperator::getChildByName(TiXmlNode* pNode, const char* pcName) const
{
	if (NULL != pNode)
	{
		if (NULL != pcName)
		{
			return pNode->FirstChild(pcName);
		}
	}
	return NULL;
}

//���ݽڵ�ָ��һ��ȡ�����е����Լ�����ֵ
bool CXmlOperator::getAllAttribute(TiXmlElement* pElem,
	map<string, string> &mapAttribute) const
{
	if (NULL != pElem)
	{
		TiXmlAttribute* pAttribute = pElem->FirstAttribute();
		if (NULL != pAttribute)
		{
			string szName = "";
			string szValue = "";

			//�ӵ�һ�����Կ�ʼѭ����ȡ
			while(pAttribute)
			{
				szName = pAttribute->NameTStr();
				szValue = pAttribute->ValueStr();
				mapAttribute.insert(pair<string, string>(szName, szValue));

				pAttribute = pAttribute->Next();
			}
			return true;
		}
	}
	//�ڵ�ΪNULL �� �����������򷵻�false
	return false;
}

//�Ը��ӽڵ���Ϊ���Ա�����Ϣ����������ݽڵ�ָ��һ��ȡ���������Լ�����ֵ
bool CXmlOperator::getAllAttributeByChild(TiXmlElement* pElem,
	map<string, string> &mapAttribute) const
{
	if (NULL != pElem)
	{
		TiXmlElement* pChileElem = pElem->FirstChildElement();
		if (NULL != pChileElem)
		{
			string szName = "";
			char* pcValue = NULL;
			const char* pcTemp = NULL;

			//�ӵ�һ���ӽڵ�Ԫ�ؿ�ʼѭ����ȡ
			while (pChileElem)
			{
				szName = pChileElem->ValueStr();
			    pcTemp = pChileElem->GetText();

				if (pcTemp == NULL)
				{
					mapAttribute.insert(pair<string, string>(szName, ""));
					pChileElem = pChileElem->NextSiblingElement();
					continue;
				}
				pcValue = new char[strlen(pcTemp) + 1];
				memcpy(pcValue, pcTemp, strlen(pcTemp) + 1);
				//strcpy(pcValue, pcTemp);

				mapAttribute.insert(pair<string, string>(szName, pcValue));
				pChileElem = pChileElem->NextSiblingElement();
				delete pcValue;//�ͷ��ڴ�
			}
			pcValue = NULL;
			return true;
		}
	}

	return false;
}

//��ȡ�ĵ�����
TiXmlDocument* CXmlOperator::getDoc() const
{
	return m_pXmlDoc;
}
