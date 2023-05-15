
// PhoneAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PhoneApp.h"
#include "PhoneAppDlg.h"
#include "afxdialogex.h"
#include "odbcinst.h"
#include "afxdb.h"
#include "CEditEmployeeDlg.h"
#include "Employee.h"
#include "Process.h"
#include <vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//global var for selected item in listbox
static int SELECTED = -1;

//global var for connection to database and database operations
Process process;

//global array of employees
vector<Employee> employees;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPhoneAppDlg dialog



CPhoneAppDlg::CPhoneAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PHONEAPP_DIALOG, pParent)
	, m_tb_fn(_T(""))
	, m_tb_ln(_T(""))
	, m_tb_phone(_T(""))
	, m_list_value(_T(""))
	, m_search(_T(""))
	, m_tb_id(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhoneAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FN, m_tb_fn);
	DDX_Text(pDX, IDC_LN, m_tb_ln);
	DDX_Text(pDX, IDC_PHONE, m_tb_phone);
	DDX_Control(pDX, IDC_TABLE, m_list_control);
	DDX_LBString(pDX, IDC_LIST_NAMES, m_list_value);
	DDX_Control(pDX, IDC_LIST_NAMES, m_list_names);
	DDX_Text(pDX, IDC_SEARCH, m_search);
	DDX_Text(pDX, IDC_ID, m_tb_id);
}

BEGIN_MESSAGE_MAP(CPhoneAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSERT, &CPhoneAppDlg::OnBnClickedInsert)
	ON_BN_CLICKED(IDC_RETRIEVE, &CPhoneAppDlg::OnBnClickedRetrieve)
	ON_BN_CLICKED(IDC_PARTIAL_MATCH, &CPhoneAppDlg::OnBnClickedPartialMatch)
	ON_BN_CLICKED(IDC_EXACT_MATCH, &CPhoneAppDlg::OnBnClickedExactMatch)
	ON_LBN_SELCHANGE(IDC_LIST_NAMES, &CPhoneAppDlg::OnLbnSelchangeListNames)
	ON_BN_CLICKED(IDC_DELETE, &CPhoneAppDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_UPDATE, &CPhoneAppDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDCANCEL, &CPhoneAppDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPhoneAppDlg message handlers

BOOL CPhoneAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	FetchData();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPhoneAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPhoneAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPhoneAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CPhoneAppDlg::GetFirstWord(CString s) {
	CString substr;

	for (size_t i = 0; i < s.GetLength(); i++)
	{
		if (s[i] != ' ')
		{
			substr += s[i];
		}
		else
		{
			break;
		}
	}
	return substr;
}

void CPhoneAppDlg::ResetListControls() {
	UpdateData();
	m_list_control.DeleteAllItems();
	int iNbrOfColumns = 0;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_list_control.GetDlgItem(0);
	if (pHeader) {
		iNbrOfColumns = pHeader->GetItemCount();
	}
	for (int i = iNbrOfColumns; i >= 0; i--) {
		m_list_control.DeleteColumn(i);
	}
	m_list_names.ResetContent();

	// populate Grids
	ListView_SetExtendedListViewStyle(m_list_control, LVS_EX_GRIDLINES);

	// Column width and heading
	m_list_control.InsertColumn(0, L"ID", LVCFMT_LEFT, -1, 0);
	m_list_control.InsertColumn(1, L"First Name", LVCFMT_LEFT, -1, 1);
	m_list_control.InsertColumn(2, L"Last Name", LVCFMT_LEFT, -1, 1);
	m_list_control.InsertColumn(3, L"Telephone", LVCFMT_LEFT, -1, 1);
	m_list_control.SetColumnWidth(0, 60);
	m_list_control.SetColumnWidth(1, 100);
	m_list_control.SetColumnWidth(2, 100);
	m_list_control.SetColumnWidth(3, 100);
	m_list_control.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	UpdateData(false);
}

void CPhoneAppDlg::FetchData() {
	if (!process.Connection_Success())
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else
	{
		ResetListControls();
		employees.clear();
		employees = process.FetchEmployees();

		for (size_t i = 0; i < employees.size(); i++)
		{
			int index = m_list_names.AddString(employees.at(i).GetFirstName() + " " + employees.at(i).GetLastName());
			DWORD d = _wtol(employees.at(i).GetID());

			m_list_names.SetItemData(index, d);
		}
		
	}
	
}

void CPhoneAppDlg::OnBnClickedInsert()
{
	if (!process.Connection_Success())
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		UpdateData();

		process.InsertQuery(m_tb_id, m_tb_fn, m_tb_ln, m_tb_phone);

		m_tb_id = "";
		m_tb_fn = "";
		m_tb_ln = "";
		m_tb_phone = "";

		UpdateData(FALSE);
		FetchData();
	}
	
}


void CPhoneAppDlg::OnBnClickedRetrieve()
{
	if (!process.Connection_Success())
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else
		FetchData();
}


void CPhoneAppDlg::OnBnClickedPartialMatch()
{
	if (!process.Connection_Success())
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		UpdateData();
		ResetListControls();
		employees.clear();
		employees = process.PartialMatch(m_search);

		for (size_t i = 0; i < employees.size(); i++)
		{
			for (size_t j = 0; j < employees.at(i).GetPhones().size(); j++)
			{
				int iRec = m_list_control.InsertItem(0, employees.at(i).GetID(), 0);
				m_list_control.SetItemText(0, 1, employees.at(i).GetFirstName());
				m_list_control.SetItemText(0, 2, employees.at(i).GetLastName());
				m_list_control.SetItemText(0, 3, employees.at(i).GetPhones().at(j));//this may be deleted
			}
			

			int index = m_list_names.AddString(employees.at(i).GetFirstName() + " " + employees.at(i).GetLastName());
			DWORD d = _wtol(employees.at(i).GetID());

			m_list_names.SetItemData(index, d);
		}
	}
	
}


void CPhoneAppDlg::OnBnClickedExactMatch()
{
	if (!process.Connection_Success())
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		UpdateData();
		ResetListControls();
		employees.clear();
		employees = process.ExactMatch(m_search);

		for (size_t i = 0; i < employees.size(); i++)
		{
			for (size_t j = 0; j < employees.at(i).GetPhones().size(); j++)
			{
				int iRec = m_list_control.InsertItem(0, employees.at(i).GetID(), 0);
				m_list_control.SetItemText(0, 1, employees.at(i).GetFirstName());
				m_list_control.SetItemText(0, 2, employees.at(i).GetLastName());
				m_list_control.SetItemText(0, 3, employees.at(i).GetPhones().at(j));//this may be deleted
			}

			int index = m_list_names.AddString(employees.at(i).GetFirstName() + " " + employees.at(i).GetLastName());
			DWORD d = _wtol(employees.at(i).GetID());

			m_list_names.SetItemData(index, d);
		}

		
	}
}


void CPhoneAppDlg::OnLbnSelchangeListNames()
{
	UpdateData();
	employees.clear();

	SELECTED = m_list_names.GetCurSel();
	if (SELECTED!=-1)
	{
		DWORD d = m_list_names.GetItemData(SELECTED);
		m_list_control.DeleteAllItems();

		employees = process.SearchWhereByID(d);

		m_list_control.DeleteAllItems();
		for (size_t i = 0; i < employees.size(); i++)
		{
			for (size_t j = 0; j < employees.at(i).GetPhones().size(); j++)
			{
				int iRec = m_list_control.InsertItem(0, employees.at(i).GetID(), 0);
				m_list_control.SetItemText(0, 1, employees.at(i).GetFirstName());
				m_list_control.SetItemText(0, 2, employees.at(i).GetLastName());
				m_list_control.SetItemText(0, 3, employees.at(i).GetPhones().at(j));//this may be deleted
			}
		}
	}

	UpdateData(FALSE);
}


void CPhoneAppDlg::OnBnClickedDelete()
{
	if (!process.Connection_Success())
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		if (SELECTED!= -1)
		{
			if (MessageBoxA(NULL, "Do you want to delete this item?", "Warning", MB_YESNO) == IDYES) {
				
				process.DeleteQuery(m_list_names.GetItemData(SELECTED));

				UpdateData(FALSE);
				FetchData();
				SELECTED = -1;
			}
		}
		else
		{
			AfxMessageBox(L"No person selected!");
		}
	}
	
}


void CPhoneAppDlg::OnBnClickedUpdate()
{
	employees.clear();
	if (!process.Connection_Success())
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		if (SELECTED !=-1) {
			UpdateData();
			employees = process.SearchWhereByID(m_list_names.GetItemData(SELECTED));

			CEditEmployeeDlg dlg(employees);
			if (dlg.DoModal() == IDOK) {
				UpdateData();
				employees = dlg.empl;
				vector<CString> old_phones = dlg.toReplace;
				vector<CString> new_phones = dlg.toInsert;

				//bugged
				/*if (old_phones.size()>0)
				{
					for (size_t i = 0; i < old_phones.size(); i++)
					{
						process.UpdateQuery(employees.at(0).GetID(), employees.at(0).GetFirstName(), employees.at(0).GetLastName(), employees.at(0).GetPhones(), old_phones.at(i));
					}
				}
				else {
					process.UpdateQuery(employees.at(0).GetID(), employees.at(0).GetFirstName(), employees.at(0).GetLastName(), employees.at(0).GetPhones());
				}
				
				if (new_phones.size() > 0)
				{
					for (size_t i = 0; i < new_phones.size(); i++)
					{
						process.InsertPhoneQuery(employees.at(i).GetID(), new_phones.at(i));

					}
				}*/



			//temporary solution
				process.DeletePhonesQuery(employees.at(0).GetID());
				process.UpdateQuery(employees.at(0).GetID(), employees.at(0).GetFirstName(), employees.at(0).GetLastName(), employees.at(0).GetPhones());
			//temporary solution

				UpdateData(false);
			}
			ResetListControls();
			FetchData();
		}
		else
		{
			AfxMessageBox(L"No person selected!");
		}
	}
	
}


void CPhoneAppDlg::OnBnClickedCancel()
{
	if (MessageBoxA(NULL, "Are you sure you want to exit?", "Exit", MB_YESNO) == IDYES) {
		CloseConn();
		CDialogEx::OnCancel();
	}
}

void CPhoneAppDlg::CloseConn() {
	process.~Process();
}