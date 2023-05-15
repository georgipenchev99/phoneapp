// CEditEmployeeDlg.cpp : implementation file
//

#include "pch.h"
#include "PhoneApp.h"
#include "CEditEmployeeDlg.h"
#include "afxdialogex.h"
#include "Employee.h"
#include <vector>



// CEditEmployeeDlg dialog

IMPLEMENT_DYNAMIC(CEditEmployeeDlg, CDialogEx)

CEditEmployeeDlg::CEditEmployeeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_DLG, pParent)
	, m_edit_id(_T(""))
	, m_id_fn(_T(""))
	, m_edit_ln(_T(""))
	, m_edit_phone(_T(""))
	, m_list_phones_val(_T(""))
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
CEditEmployeeDlg::CEditEmployeeDlg(vector<Employee> v, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_DLG, pParent)
	, m_edit_id(v.at(0).GetID())
	, m_id_fn(v.at(0).GetFirstName())
	, m_edit_ln(v.at(0).GetLastName())
	, m_edit_phone(L"")
	, empl(v)
	
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
	DDX_Control(pDX, IDC_LIST1, m_list_phones);
	DDX_LBString(pDX, IDC_LIST1, m_list_phones_val);
	
}


BEGIN_MESSAGE_MAP(CEditEmployeeDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEditEmployeeDlg::OnLbnSelchangeList1)
	ON_EN_KILLFOCUS(IDC_EDIT_PHONE, &CEditEmployeeDlg::OnEnKillfocusEditPhone)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditEmployeeDlg::OnBnClickedButton1)
	ON_EN_KILLFOCUS(IDC_EDIT_ID, &CEditEmployeeDlg::OnEnKillfocusEditId)
	ON_EN_KILLFOCUS(IDC_EDIT_FN, &CEditEmployeeDlg::OnEnKillfocusEditFn)
	ON_EN_KILLFOCUS(IDC_EDIT_LN, &CEditEmployeeDlg::OnEnKillfocusEditLn)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditEmployeeDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CEditEmployeeDlg message handlers


void CEditEmployeeDlg::Load_Phones() 
{
	UpdateData();
	m_list_phones.ResetContent();
	for (size_t i = 0; i < empl.at(0).GetPhones().size(); i++)
	{
		int index = m_list_phones.AddString(empl.at(0).GetPhones().at(i));
		m_list_phones.SetItemData(index, i);
		
	}
	UpdateData(false);
}

void CEditEmployeeDlg::OnLbnSelchangeList1()
{
	UpdateData();
	
	m_edit_phone = m_list_phones_val;
	
	UpdateData(false);
}


void CEditEmployeeDlg::OnEnKillfocusEditPhone()
{
	if (m_list_phones.GetCurSel()!=-1)
	{
		toReplace.push_back(m_edit_phone);
		UpdateData();
		DWORD id = m_list_phones.GetItemData(m_list_phones.GetCurSel());
		empl.at(0).SetPhone(id, m_edit_phone);
		m_edit_phone = "";
		UpdateData(false);
	}
	Load_Phones();
	return;
}


void CEditEmployeeDlg::OnBnClickedButton1()
{
	Load_Phones();
	return;
}


void CEditEmployeeDlg::OnEnKillfocusEditId()
{
	//violates FK constraint
	//empl.at(0).SetID(m_edit_id);
	return;
}


void CEditEmployeeDlg::OnEnKillfocusEditFn()
{
	UpdateData();
	empl.at(0).SetFirstName(m_id_fn);
	UpdateData(false);
	return;
}


void CEditEmployeeDlg::OnEnKillfocusEditLn()
{
	UpdateData();
	empl.at(0).SetLastName(m_edit_ln);
	UpdateData(false);
	return;
}


void CEditEmployeeDlg::OnBnClickedButton2()
{
	UpdateData();
	empl.at(0).Push_Phone(m_edit_phone);
	toInsert.push_back(m_edit_phone);
	UpdateData(false);
	return;
}
