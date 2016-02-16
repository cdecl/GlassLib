// SqliteWrap.h
//////////////////////////////////////////////////////////////////////////

#ifndef __SQLITEWRAP_H_BY_CDECL__
#define __SQLITEWRAP_H_BY_CDECL__


#include <map>
#include <vector>
#include <string>


namespace GLASS {


class SQLite
{
public:
	typedef std::map<std::string, int> ColMap;
	typedef std::vector<std::string> ColList;

public:
	SQLite() : pDB_(NULL), pStmt_(NULL) {}
	virtual ~SQLite() {  Close(); }

private:
	SQLite(const SQLite&);
	SQLite& operator=(const SQLite&);

public:
	int Open(const char *pszFileName) 
	{
		return sqlite3_open(pszFileName, &pDB_);
	}

	int Close()
	{
		Finalize();
		
		int nResult = 0;
		if (pDB_) {
			nResult = sqlite3_close(pDB_);
			pDB_ = NULL;
		}

		return nResult;
	}

	bool IsOpen()
	{
		return pDB_ != NULL;
	}

	int Execute(const std::string &strQuery)
	{
		sqlite3_stmt* pStmt = NULL;
		return sqlite3_prepare(pDB_, strQuery.c_str(), strQuery.length(), &pStmt, NULL);
	}

	int Prepare(const std::string &strQuery) 
	{
		Finalize();

		int nError = sqlite3_prepare(pDB_, strQuery.c_str(), strQuery.length(), &pStmt_, NULL);
		ColMapping();

		return nError;
	}

	std::string operator()(int nCol) 
	{
		return GetColText(nCol);
	}

	std::string operator()(const std::string &strColName)
	{
		return GetColText(strColName);
	}

	std::string GetColText(int nCol)
	{
		const char *pstr = reinterpret_cast<const char *>(sqlite3_column_text(pStmt_, nCol));
		return pstr ? pstr : "(null)";
	}

	std::string GetColText(const std::string &strColName)
	{
		return GetColText(colm_[strColName]);
	}

	int GetColInt(int nCol)
	{
		return sqlite3_column_int(pStmt_, nCol);
	}

	int GetColInt(const std::string &strColName)
	{
		return sqlite3_column_int(pStmt_, colm_[strColName]);
	}

	int GetCols()
	{
		return colList_.size();
	}

	std::string GetColName(int nCol)
	{
		return colList_[nCol];
	}

	double GetColDouble(int nCol)
	{
		return sqlite3_column_double(pStmt_, nCol);
	}

	double GetColDouble(const std::string &strColName)
	{
		return sqlite3_column_double(pStmt_, colm_[strColName]);
	}

	int GetRows()
	{
		return sqlite3_data_count(pStmt_);
	}

	int Finalize()
	{
		int nResult = 0;
		if (pStmt_) {
			nResult = sqlite3_finalize(pStmt_);
			pStmt_ = NULL;
		}

		return nResult;
	}

	int Reset()
	{
		return sqlite3_reset(pStmt_);
	}

	bool Next()
	{
		return sqlite3_step(pStmt_) == SQLITE_ROW;
	}


protected:
	void ColMapping()
	{
		colm_.clear();
		colList_.clear();

		int nCols = sqlite3_column_count(pStmt_);
		colList_.reserve(nCols);

		for (int i = 0; i < nCols; ++i) {
			colList_.push_back(sqlite3_column_name(pStmt_, i));
			colm_[sqlite3_column_name(pStmt_, i)] = i;
		}
	}
	
private:
	sqlite3 *pDB_;
	sqlite3_stmt *pStmt_;

	ColMap colm_;
	ColList colList_;
};


}


#endif 


