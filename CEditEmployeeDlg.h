#pragma once


// CEditEmployeeDlg dialog

class CEditEmployeeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditEmployeeDlg)

public:
	CEditEmployeeDlg(CWnd* pParent = nullptr);   // standard constructor
	CEditEmployeeDlg(CString id, CString fn, CString ln, CString tel, CWnd* pParent = nullptr);  
	// constructor with passed params

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
};
