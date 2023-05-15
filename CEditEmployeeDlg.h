#pragma once
#include <vector>
#include "Process.h"
#include "Employee.h"

using namespace std;

// CEditEmployeeDlg dialog

class CEditEmployeeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditEmployeeDlg)

public:
	CEditEmployeeDlg(CWnd* pParent = nullptr);   // standard constructor
	CEditEmployeeDlg(CString id, CString fn, CString ln, CString tel, CWnd* pParent = nullptr);  
	// constructor with passed params
	CEditEmployeeDlg(vector<Employee> v, CWnd* pParent = nullptr);

	virtual ~CEditEmployeeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_edit_id;
	CString m_id_fn;
	CString m_edit_ln;
	CString m_edit_phone;
	CListBox m_list_phones;
	afx_msg void OnLbnSelchangeList1();
	CString m_list_phones_val;
	afx_msg void OnEnKillfocusEditPhone();
	vector<Employee> empl;
	afx_msg void OnBnClickedButton1();
	void Load_Phones();
	afx_msg void OnEnKillfocusEditId();
	afx_msg void OnEnKillfocusEditFn();
	afx_msg void OnEnKillfocusEditLn();
	afx_msg void OnBnClickedButton2();
	vector<CString> toReplace;
};
