#pragma once
#include <vector>
using namespace std;
class Employee
{
protected:
	CString id;
	CString fn;
	CString ln;
	vector<CString> phones;
public:
	Employee() {

	}
	Employee(CString id, CString fn, CString ln) {
		this->id = id;
		this->fn = fn;
		this->ln = ln;
	}
	Employee(CString id, CString fn, CString ln, CString phone) {
		this->id = id;
		this->fn = fn;
		this->ln = ln;
		Push_Phone(phone);
	}
	CString GetID() {
		return id;
	}
	CString GetFirstName() {
		return fn;
	}
	CString GetLastName() {
		return ln;
	}
	vector<CString> GetPhones() {
		return phones;
	}
	void Push_Phone(CString phone) {
		phones.push_back(phone);
	}
	void SetPhone(DWORD id, CString newValue) {
		phones.at(id) = newValue;
	}
	void SetID(CString newID) {
		this->id = newID;
	}
	void SetFirstName(CString newName) {
		this->fn = newName;
		int c = 0;
	}
	void SetLastName(CString newLastName) {
		this->ln = newLastName;
		int c = 0;
	}
};

