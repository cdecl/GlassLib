// FileName : mmake.cpp
//
// Modify Date	: 2003.01.24
// Programmed by cdecl 
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
using namespace std;

#ifdef _WIN32
 #include <windows.h>
#else
 #include <unistd.h>
 #include <dirent.h>
#endif


const string MMAKE_VERSION	= "1.2";

typedef vector<string> vec_string;
typedef vec_string::iterator vec_string_iter;


void GetHeaderFileIn(const string &str, vec_string &v);
void GetHeaderFile(const string &strFile, vec_string &v);
void GetCurrDirFiles(vec_string &v);
string GetFileExt(const string &strFileName);
string GetFileNoExt(const string &strFileName);
string LowerCase(const string &str);
void Usage(const string &strExe);
void MakefileHeader(ostream &os);


vec_string& GetCPPFILES() 
{
	static vec_string vCPP;
	vCPP.push_back("cpp");
	vCPP.push_back("c");
	vCPP.push_back("cxx");
	
	return vCPP;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		Usage(argv[0]);
		return -1;
	}

	const string strExeFile = argv[1];

	if (strExeFile == "-v" || strExeFile == "--v") {
		cout << "version - " << MMAKE_VERSION << endl;
		return 0;
	}

	vec_string vFiles;
	GetCurrDirFiles(vFiles);

	ostringstream osSource;
	ostringstream osObj;

	vec_string &vCPP = GetCPPFILES();
	vec_string_iter itFile = vFiles.begin();

	for (; itFile != vFiles.end(); ++itFile) {
		string strFile = *itFile;
		vec_string_iter itCPP = find(vCPP.begin(), vCPP.end(), LowerCase(GetFileExt(strFile)));

		if (itCPP != vCPP.end()) {
			string strOFile = (GetFileNoExt(strFile) + ".o");

			// OBJ
			osObj << strOFile << " ";

			// Source File
			osSource << strOFile << " : " << strFile << " ";

			vec_string vHeaders;
			GetHeaderFile(strFile, vHeaders);

			vec_string_iter itH = vHeaders.begin();
			for (; itH != vHeaders.end(); ++itH) {
				osSource << *itH << " ";
			}

			osSource << endl;
			osSource << "\t" << "$(CC) $(FLAGS) -c " << strFile << endl << endl;
		}
	}


	ofstream ofs("Makefile");
	MakefileHeader(ofs);

	ofs << "CC = g++" << endl;
	ofs << "FLAGS = -Wall" << endl;
	ofs << "LINK_FLAGS = " << endl;

	ofs << "OBJS = " << osObj.str() << endl;
	ofs << "BIN = " << strExeFile << endl << endl;

	ofs << "$(BIN) : $(OBJS)" << endl;
	ofs << "\t" << "$(CC) $(FLAGS) -o $(BIN) $(OBJS) $(LINK_FLAGS)" << endl << endl;

	ofs << osSource.str() << endl << endl;
	ofs << "clean : " << endl;

#ifdef _WIN32
	ofs << "\tdel $(BIN) $(OBJS)" << endl;  
#else
	ofs << "\trm -f $(BIN) $(OBJS)" << endl;  
#endif

	ofs.close();
	return 0;
}

void MakefileHeader(ostream &os)
{
	os << "###################################################" << endl;
	os << "## Makefile by mmake " << endl;
	os << "## mmake version : " << MMAKE_VERSION << endl;
	os << "## date  : " << __DATE__ << " " << __TIME__ << endl;
	os << "###################################################" << endl << endl;
}

void Usage(const string &strExe)
{
	cout << "Usage : " << strExe << " " << "[ExeFile]" << endl;
}

string LowerCase(const string &str)
{
	string s; 
	transform(str.begin(), str.end(), back_inserter(s), ::tolower);
	return s;
}

string GetFileExt(const string &strFileName) 
{
	string::size_type nP = strFileName.find_last_of('.');
	if (nP == string::npos) {
		return "";
	}
	
	return strFileName.substr(nP + 1);
}

string GetFileNoExt(const string &strFileName) 
{
	string::size_type nP = strFileName.find_last_of('.');
	if (nP == string::npos) {
		return strFileName;
	}
	
	return strFileName.substr(0, nP);
}

#ifdef _WIN32
void GetCurrDirFiles(vec_string &v)
{
	enum { SZ_SIZE = 1024 };

	char szBuff[SZ_SIZE + 1];
	GetCurrentDirectory(SZ_SIZE, szBuff);
	
	string str = szBuff;
	str += "\\*";

	WIN32_FIND_DATA wfd = {0};
	HANDLE h = FindFirstFile(str.c_str(), &wfd);

	while (FindNextFile(h, &wfd)) {
		v.push_back(wfd.cFileName);
	}

	FindClose(h);
}
#else 
void GetCurrDirFiles(vec_string &v)
{
	DIR     *dirp;
	struct  dirent *dp;

	if ((dirp = opendir(".")) != NULL) {
		while ((dp = readdir(dirp)) != NULL) {
			v.push_back(dp->d_name);
		}
		closedir(dirp);
	}
}

#endif


void GetHeaderFileIn(const string &str, vec_string &v)
{
	try {
		string::size_type pos = str.find("#");
		if (string::npos == pos) {
			return;
		}

		pos = str.find("include", pos + 1);
		if (string::npos == pos) {
			return;
		}

		pos = str.find("\"", pos + 7);
		if (string::npos == pos) {
			return;
		}

		string::size_type posEnd = str.find("\"", pos + 1);
		if (string::npos == posEnd) {
			return;
		}

		v.push_back(str.substr(pos + 1, posEnd - pos - 1));
	}
	catch (exception) {

	}
}

void GetHeaderFile(const string &strFile, vec_string &v)
{
	ifstream ifs(strFile.c_str());
	if (!ifs.is_open()) {
		return;
	}

	string str;
	while (!ifs.eof()) {
		getline(ifs, str);
		GetHeaderFileIn(str, v);
	}

	ifs.close();
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////




