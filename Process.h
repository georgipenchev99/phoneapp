#pragma once
#include <vector>
#include "odbcinst.h"
#include "afxdb.h"
#include "afxdialogex.h"
#include "Employee.h"
#define CONN_STRING L"Driver={ODBC Driver 17 for SQL Server};Server={(localdb)\\MSSQLLocalDB}; Database=work; Trusted_Connection=yes;"

using namespace std;
class Process
{
private:
	CDatabase database;
	bool conn_success;
	vector <Employee> employees;

public:
	Process() {
		conn_success = database.Open(NULL, false, false, CONN_STRING);
	}
	~Process() {
		database.Close();
	}
	bool Connection_Success() {
		return conn_success;
	}

	vector<Employee> FetchEmployees() {
		employees.clear();
		CString SqlString;

		CString strID, strName, strLastName, strPhone;

		int iRec = 0;

		TRY{
			CRecordset recset(&database);

			SqlString = "SELECT id, FirstName, LastName FROM Employees";

			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);

			while (!recset.IsEOF()) {

				recset.GetFieldValue(L"id",strID);
				recset.GetFieldValue(L"FirstName",strName);
				recset.GetFieldValue(L"LastName", strLastName);
				//recset.GetFieldValue(L"phone", strPhone);

				/*int index = m_list_names.AddString(strName + " " + strLastName);
				DWORD d = _wtol(strID);

				m_list_names.SetItemData(index, d);*/

				employees.push_back(Employee(strID, strName, strLastName));

				recset.MoveNext();
			}
		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
		return employees;
	}

	void InsertQuery(CString id, CString fn, CString ln, CString phone) {
		CString SqlString;
		CString strID, strFirstName, strLastName, strPhone;

		int iRec = 0;

		TRY{

			SqlString.Format(L"INSERT INTO employees (id,FirstName,LastName) VALUES ('%s', '%s','%s')",id, fn, ln);
			database.ExecuteSQL(SqlString);
			SqlString.Format(L"INSERT INTO telephones (employee_id,phone) VALUES ('%s', '%s')", id, phone);
			database.ExecuteSQL(SqlString);

		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}

	vector<Employee> PartialMatch(CString searched_value) {
		CString SqlString;
		CString strID, strName, strLastName, strPhone;
		bool first = true;
		int iRec = 0;

		employees.clear();
		TRY{

			CRecordset recset(&database);

			SqlString = L"SELECT e.ID, FirstName, LastName, phone FROM Employees e left join telephones t on e.id=t.employee_id WHERE FirstName like '%" + searched_value + L"%' or LastName like '%" + searched_value + L"%' or phone like '%" + searched_value + L"%'";

			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);

			
			if (recset.IsEOF())
			{
				AfxMessageBox(L"No data found!");
			}
			else {
				int i = 0;
				while (!recset.IsEOF()) {

					recset.GetFieldValue(L"ID", strID);
					recset.GetFieldValue(L"FirstName", strName);
					recset.GetFieldValue(L"LastName", strLastName);
					recset.GetFieldValue(L"phone", strPhone);

					if (first) {
						first = false;
						employees.push_back(Employee(strID, strName, strLastName, strPhone));
						i++;
					}
					else {
						if (strID == employees.at(i-1).GetID())
						{
							employees.at(i - 1).Push_Phone(strPhone);
						}
						else {
							employees.push_back(Employee(strID, strName, strLastName, strPhone));
							i++;
						}
					}

					
					recset.MoveNext();
				}
			}
		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;

		return employees;
	}

	vector<Employee> ExactMatch(CString searched_value) {
		CString SqlString;
		CString strID, strName, strLastName, strPhone;

		int iRec = 0;
		bool first = true;

		employees.clear();
		TRY{

			CRecordset recset(&database);

			SqlString.Format(L"SELECT e.ID, FirstName, LastName, phone FROM Employees e left join telephones t on e.id= t.employee_id WHERE FirstName like '%s' or LastName like '%s' or phone like '%s'",searched_value, searched_value, searched_value);

			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);

			
			if (recset.IsEOF())
			{
				AfxMessageBox(L"No data found!");
			}
			else {
				int i = 0;
				while (!recset.IsEOF()) {

					recset.GetFieldValue(L"ID", strID);
					recset.GetFieldValue(L"FirstName", strName);
					recset.GetFieldValue(L"LastName", strLastName);
					recset.GetFieldValue(L"phone", strPhone);

					if (first) {
						first = false;
						employees.push_back(Employee(strID, strName, strLastName, strPhone));
						i++;
					}
					else {
						if (strID == employees.at(i - 1).GetID())
						{
							employees.at(i - 1).Push_Phone(strPhone);
						}
						else {
							employees.push_back(Employee(strID, strName, strLastName, strPhone));
							i++;
						}
					}

					
					recset.MoveNext();
				}
			}
		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
		return employees;
	}

	vector<Employee> SearchWhereByID(DWORD id) {
		employees.clear();
		CString SqlString;
		CString strID, strName, strLastName, strPhone;
		bool first = true;
		int i = 0;
		TRY{
			CRecordset recset(&database);

			SqlString.Format(L"SELECT e.ID, FirstName, LastName, phone FROM Employees e left join telephones t on e.id= t.employee_id WHERE e.id like '%d'", id);
								
			recset.Open(CRecordset::forwardOnly, SqlString, CRecordset::readOnly);
			while (!recset.IsEOF()) {

				recset.GetFieldValue(L"ID", strID);
				recset.GetFieldValue(L"FirstName", strName);
				recset.GetFieldValue(L"LastName", strLastName);
				recset.GetFieldValue(L"phone", strPhone);

				if (first) {
					first = false;
					employees.push_back(Employee(strID, strName, strLastName, strPhone));
					i++;
				}
				else {
					if (strID == employees.at(i - 1).GetID())
					{
						employees.at(i - 1).Push_Phone(strPhone);
					}
					else {
						employees.push_back(Employee(strID, strName, strLastName, strPhone));
						i++;
					}
				}


				recset.MoveNext();
			}
		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
		return employees;
	}

	void DeleteQuery(DWORD id) {
		CString SqlString;

		TRY{
			SqlString.Format(L"DELETE FROM telephones WHERE employee_id like %d",id);
			database.ExecuteSQL(SqlString);
			SqlString.Format(L"DELETE FROM Employees WHERE id like %d",id);
			database.ExecuteSQL(SqlString);
		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}

	vector<Employee> UpdateQuery(CString id, CString fn, CString ln, vector<CString> phones, CString phone_id) {
		CString SqlString;
		CString strID, strName, strLastName, strPhone;

		int iRec = 0;
		bool first = true;
		int i = 0;

		employees.clear();

		TRY{
			CRecordset recset(&database);
			SqlString.Format(L"UPDATE Employees set Firstname='%s', LastName = '%s' WHERE id = %s", fn, ln, id);
			database.ExecuteSQL(SqlString);
			for (size_t i = 0; i < phones.size(); i++)
			{
				SqlString.Format(L"UPDATE telephones set phone='%s' WHERE employee_id = %s and phone='%s'", phones.at(i), id, phone_id);
				database.ExecuteSQL(SqlString);
			}
			
			
		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
		return employees;
	}
};

