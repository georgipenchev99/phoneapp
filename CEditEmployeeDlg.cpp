// CEditEmployeeDlg.cpp : implementation file
//

#include "pch.h"
#include "PhoneApp.h"
#include "CEditEmployeeDlg.h"
#include "afxdialogex.h"


// CEditEmployeeDlg dialog

IMPLEMENT_DYNAMIC(CEditEmployeeDlg, CDialogEx)

CEditEmployeeDlg::CEditEmployeeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_DLG, pParent)
	, m_edit_id(_T(""))
	, m_id_fn(_T(""))
	, m_edit_ln(_T(""))
	, m_edit_phone(_T(""))
{

}
CEditEmployeeDlg::CEditEmployeeDlg(CString id, CString fn, CString ln, CString tel, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_DLG, pParent)
	, m_edit_id(id)
	, m_id_fn(fn)
	, m_edit_ln(ln)
	, m_edit_phone(tel)
{
	
}

CEditEmployeeDlg::~CEditEmployeeDlg()
{
}

void CEditEmployeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_edit_id);
	DDX_Text(pDX, IDC_EDIT_FN, m_id_fn);
	DDX_Text(pDX, IDC_EDIT_LN, m_edit_ln);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_edit_phone);
}


BEGIN_MESSAGE_MAP(CEditEmployeeDlg, CDialogEx)
END_MESSAGE_MAP()


// CEditEmployeeDlg message handlers
