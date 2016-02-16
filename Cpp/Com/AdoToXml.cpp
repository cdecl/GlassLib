
#include "stdafx.h"
#include "AdoToXml.h"
using namespace ADODB;
using namespace GLASS;

inline void TESTHR(HRESULT hr)
{
	if (FAILED(hr)) {
		_com_raise_error(hr);
	}
}

AdoToXml::AdoToXml()
{
}


void AdoToXml::GetFiledName(ADODB::_RecordsetPtr &spRs, VecFileds &vecFileds)
{
	long lFiledsCount = spRs->GetFields()->GetCount(); 

	for (long i = 0; i < (int)lFiledsCount; ++i) {
		vecFileds.push_back((LPCSTR)spRs->GetFields()->GetItem(i)->GetName());
	}
}


void AdoToXml::WriteXmlFileFiled(ADODB::_RecordsetPtr &spRs, XMLDOMDocumentPtr &xmlDom, XMLDOMNodePtr &xmlNodeTable, const std::string &strFiledName)
{
	XMLDOMNodePtr xmlNode = xmlDom->createElement(strFiledName.c_str());

	_variant_t var = spRs->GetCollect(strFiledName.c_str());
	if (var.vt == VT_NULL) {
		var = "";
	}
	
	xmlNode->Puttext((_bstr_t)var);
	xmlNodeTable->appendChild(xmlNode);
}


void AdoToXml::WriteXmlFileRow(ADODB::_RecordsetPtr &spRs, XMLDOMDocumentPtr &xmlDom, XMLDOMNodePtr &xmlRootNode, VecFileds &vecFileds)
{
	XMLDOMNodePtr xmlNodeTable = xmlDom->createElement("Table");

	VecFiledsIter it = vecFileds.begin();
	for (; it != vecFileds.end(); ++it) {
		WriteXmlFileFiled(spRs, xmlDom, xmlNodeTable, *it);
	}

	xmlRootNode->appendChild(xmlNodeTable); 
}


AdoToXml::XMLDOMDocumentPtr AdoToXml::MakeXmlDom(ADODB::_RecordsetPtr &spRs)
{
	const std::string strXml = "<NewDataSet></NewDataSet>";
	
	XMLDOMDocumentPtr xmlDom;
	TESTHR(xmlDom.CreateInstance(__uuidof(MSXML2::DOMDocument30)));
	xmlDom->Putasync(VARIANT_FALSE);
	
	xmlDom->loadXML(strXml.c_str());
	XMLDOMNodePtr xmlRootNode = xmlDom->selectSingleNode("/NewDataSet");

	VecFileds vecFileds;
	GetFiledName(spRs, vecFileds);
	
	// ROW가 없을때 예외 발생해서 주석처리 
	//TESTHR(spRs->MoveFirst()); 
	while (VARIANT_TRUE != spRs->GetadoEOF()) {
		WriteXmlFileRow(spRs, xmlDom, xmlRootNode, vecFileds); 
		spRs->MoveNext();
	}

	return xmlDom;
}


