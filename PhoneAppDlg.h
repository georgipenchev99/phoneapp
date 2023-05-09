
// PhoneAppDlg.h : header file
//

#pragma once


// CPhoneAppDlg dialog
class CPhoneAppDlg : public CDialogEx
{
// Construction
public:
	CPhoneAppDlg(CWnd* pParent = nullptr);	// standard constructor
	void ResetListControls();
	void FetchData();
	void CloseConn();
	CString GetFirstWord(CString s);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHONEAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_tb_fn;
	CString m_tb_ln;
	CString m_tb_phone;
	CListCtrl m_list_control;
	CString m_list_value;
	CListBox m_list_names;
	CString m_search;
	afx_msg void OnBnClickedInsert();
	afx_msg void OnBnClickedRetrieve();
	afx_msg void OnBnClickedPartialMatch();
	afx_msg void OnBnClickedExactMatch();
	afx_msg void OnLbnSelchangeListNames();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedUpdate();
	CString m_edit_id;
	CString m_edit_fn;
	CString m_edit_ln;
	CString m_edit_phone;
	afx_msg void OnBnClickedCancel();
};
