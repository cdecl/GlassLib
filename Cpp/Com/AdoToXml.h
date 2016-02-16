// AdoToXml.h: interface for the ADO class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////


#ifndef		_ADOTOXML_H_CDECL_
#define		_ADOTOXML_H_CDECL_

#include <string>
#include <vector>
#include <stdexcept>


#import	"msxml3.dll" 
#import	"C:\Program Files\Common Files\System\ado\msado15.dll"	\
								rename("EOF", "adoEOF")

namespace GLASS 
{


class AdoToXml
{
public:
	typedef MSXML2::IXMLDOMDocumentPtr XMLDOMDocumentPtr;
	typedef MSXML2::IXMLDOMNodePtr XMLDOMNodePtr;
	typedef MSXML2::IXMLDOMElementPtr XMLDOMElementPtr;

	typedef std::vector<std::string> VecFileds;
	typedef VecFileds::iterator VecFiledsIter;

public:
	AdoToXml();

private:
	AdoToXml(const AdoToXml&);
	AdoToXml& operator=(const AdoToXml&);

public:
	XMLDOMDocumentPtr MakeXmlDom(ADODB::_RecordsetPtr &spRs);

private:
	void WriteXmlFileFiled(ADODB::_RecordsetPtr &spRs, XMLDOMDocumentPtr &xmlDom, XMLDOMNodePtr &xmlNodeTable, const std::string &strFiledName);
	void WriteXmlFileRow(ADODB::_RecordsetPtr &spRs, XMLDOMDocumentPtr &xmlDom, XMLDOMNodePtr &xmlRootNode, VecFileds &vecFileds);
	void GetFiledName(ADODB::_RecordsetPtr &spRs, VecFileds &vecFileds);

};


}

#endif 



