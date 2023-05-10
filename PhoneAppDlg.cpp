
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
#include <vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//Uid=User-PC\\user;Pwd=;
//define connection string with windows authentication
#define CONN_STRING L"Driver={ODBC Driver 17 for SQL Server};Server={(localdb)\\MSSQLLocalDB}; Database=work; Trusted_Connection=yes;"
//global database var with open connection
CDatabase database;
//if this is false, there will be no fetching data
bool conn_success = database.Open(NULL,false,false,CONN_STRING);

//global var for selected item in listbox
static int SELECTED = -1;

//global array of employees
std::vector<Employee> employees;

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
	FetchData();
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
	if (!conn_success)
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else
	{
		//CDatabase database;
		CString SqlString;
		
		CString strID, strName, strLastName, strPhone;
		
		int iRec = 0;

		TRY{
			//database.Open(NULL,false,false,CONN_STRING);

			// Allocate the recordset
			CRecordset recset(&database);

		// Build the SQL statement
		SqlString = "SELECT id, FirstName, LastName FROM Employees";

		// Execute the query
		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
		// Reset List control if there is any data
		ResetListControls();


		// Loop through each record
		while (!recset.IsEOF()) {
			// Copy each column into a variable
			recset.GetFieldValue(L"id",strID);
			recset.GetFieldValue(L"FirstName",strName);
			recset.GetFieldValue(L"LastName", strLastName);
			//recset.GetFieldValue(L"phone", strPhone);

			// Insert values into the list control
			/*iRec = m_list_control.InsertItem(0,strID,0);
			m_list_control.SetItemText(0,1,strName);
			m_list_control.SetItemText(0, 2, strLastName);
			m_list_control.SetItemText(0, 3, strPhone);*/

			//old
			//int index = m_list_names.AddString(strID + " " + strName + " " + strLastName);
			int index = m_list_names.AddString(strName + " " + strLastName);
			DWORD d = _wtol(strID);
			
			m_list_names.SetItemData(index, d);

			/*CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_NAMES);
			CString toAdd = strID + " " + strName + " " + strLastName;
			int i = pListBox->AddString(toAdd);
			pListBox->SetItemData(i, toAdd);
			
			DWORD_PTR d = pListBox->GetItemData(i);
			*/
			/*CString s;
			s.Format(L"%d  %d",index, d);
			MessageBox(s);*/
			// goto next record
			recset.MoveNext();
		 }
		// Close the database
		//database.Close();
		}CATCH(CDBException, e) {
			// If a database exception occured, show error msg
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	
}

void CPhoneAppDlg::OnBnClickedInsert()
{
	if (!conn_success)
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		UpdateData();

		CString SqlString;
		CString strID, strFirstName, strLastName, strPhone;

		int iRec = 0;

		TRY{

			SqlString.Format(L"INSERT INTO employees (id,FirstName,LastName) VALUES ('%s', '%s','%s')",m_tb_id,m_tb_fn,m_tb_ln);
			database.ExecuteSQL(SqlString);
			SqlString.Format(L"INSERT INTO telephones (employee_id,phone) VALUES ('%s', '%s')", m_tb_id, m_tb_phone);
			database.ExecuteSQL(SqlString);

		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;

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
	if (!conn_success)
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else
		FetchData();
}


void CPhoneAppDlg::OnBnClickedPartialMatch()
{
	if (!conn_success)
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		UpdateData();

		CString SqlString;
		CString strID, strName, strLastName, strPhone;

		int iRec = 0;

		TRY{

			CRecordset recset(&database);

			SqlString = L"SELECT e.ID, FirstName, LastName, phone FROM Employees e left join telephones t on e.id=t.employee_id WHERE FirstName like '%" + m_search + L"%' or LastName like '%" + m_search + L"%' or phone like '%" + m_search + L"%'";

			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);

			ResetListControls();
			if (recset.IsEOF())
			{
				MessageBox(L"No data found!");
			}
			else {
				while (!recset.IsEOF()) {

					recset.GetFieldValue(L"ID", strID);
					recset.GetFieldValue(L"FirstName", strName);
					recset.GetFieldValue(L"LastName", strLastName);
					recset.GetFieldValue(L"phone", strPhone);

					iRec = m_list_control.InsertItem(0, strID, 0);
					m_list_control.SetItemText(0, 1, strName);
					m_list_control.SetItemText(0, 2, strLastName);
					m_list_control.SetItemText(0, 3, strPhone);

					int index = m_list_names.AddString(strName + " " + strLastName);
					DWORD d = _wtol(strID);

					m_list_names.SetItemData(index, d);
					recset.MoveNext();
				}
			}
		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	
}


void CPhoneAppDlg::OnBnClickedExactMatch()
{
	if (!conn_success)
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		UpdateData();
		CString SqlString;
		CString strID, strName, strLastName, strPhone;

		int iRec = 0;

		TRY{

			CRecordset recset(&database);

			SqlString.Format(L"SELECT e.ID, FirstName, LastName, phone FROM Employees e left join telephones t on e.id= t.employee_id WHERE FirstName like '%s' or LastName like '%s' or phone like '%s'",m_search, m_search, m_search);

			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);

			ResetListControls();
			if (recset.IsEOF())
			{
				MessageBox(L"No data found!");
			}
			else {
				while (!recset.IsEOF()) {

					recset.GetFieldValue(L"ID", strID);
					recset.GetFieldValue(L"FirstName", strName);
					recset.GetFieldValue(L"LastName", strLastName);
					recset.GetFieldValue(L"phone", strPhone);

					iRec = m_list_control.InsertItem(0, strID, 0);
					m_list_control.SetItemText(0, 1, strName);
					m_list_control.SetItemText(0, 2, strLastName);
					m_list_control.SetItemText(0, 3, strPhone);

					int index = m_list_names.AddString(strName + " " + strLastName);
					DWORD d = _wtol(strID);

					m_list_names.SetItemData(index, d);
					recset.MoveNext();
				}
			}
		}CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
}


void CPhoneAppDlg::OnLbnSelchangeListNames()
{
	UpdateData();
	CString SqlString;
	CString strID, strName, strLastName, strPhone;

	SELECTED = m_list_names.GetCurSel();
	if (SELECTED!=-1)
	{
		DWORD d = m_list_names.GetItemData(SELECTED);
		m_list_control.DeleteAllItems();

		CRecordset recset(&database);

		SqlString.Format(L"SELECT e.ID, FirstName, LastName, phone FROM Employees e left join telephones t on e.id= t.employee_id WHERE e.id like '%d'", d);

		recset.Open(CRecordset::forwardOnly, SqlString, CRecordset::readOnly);
		while (!recset.IsEOF()) {

			recset.GetFieldValue(L"ID", strID);
			recset.GetFieldValue(L"FirstName", strName);
			recset.GetFieldValue(L"LastName", strLastName);
			recset.GetFieldValue(L"phone", strPhone);

			m_list_control.InsertItem(0, strID, 0);
			m_list_control.SetItemText(0, 1, strName);
			m_list_control.SetItemText(0, 2, strLastName);
			m_list_control.SetItemText(0, 3, strPhone);

			recset.MoveNext();
		}
	}

	UpdateData(FALSE);
}


void CPhoneAppDlg::OnBnClickedDelete()
{
	if (!conn_success)
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		if (SELECTED!= -1)
		{
			if (MessageBoxA(NULL, "Do you want to delete this item?", "Warning", MB_YESNO) == IDYES) {
				CString SqlString;

				TRY{
					SqlString.Format(L"DELETE FROM telephones WHERE employee_id like %d",m_list_names.GetItemData(SELECTED));
					database.ExecuteSQL(SqlString);
					SqlString.Format(L"DELETE FROM Employees WHERE id like %d",m_list_names.GetItemData(SELECTED));
					database.ExecuteSQL(SqlString);
				}CATCH(CDBException, e) {
					AfxMessageBox(L"Database error: " + e->m_strError);
				}
				END_CATCH;
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
	if (!conn_success)
	{
		MessageBox(L"No connection to the database at the moment!");
	}
	else {
		if (SELECTED !=-1) {
			UpdateData();
			CString SqlString;
			CString strID, strName, strLastName, strPhone;

			int iRec = 0;

			TRY{
				CRecordset recset(&database);

				SqlString.Format(L"SELECT e.ID, FirstName, LastName, phone FROM Employees e join telephones t on e.id=t.employee_id WHERE e.id like %d",m_list_names.GetItemData(SELECTED));
				MessageBox(SqlString);
				recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);

				recset.GetFieldValue(L"ID",strID);
				recset.GetFieldValue(L"FirstName",strName);
				recset.GetFieldValue(L"LastName", strLastName);
				recset.GetFieldValue(L"phone", strPhone);

				CEditEmployeeDlg dlg(strID, strName,strLastName,strPhone);
				if (dlg.DoModal() == IDOK) {
					UpdateData();
					m_edit_fn = dlg.m_id_fn;
					m_edit_ln = dlg.m_edit_ln;
					m_edit_phone = dlg.m_edit_phone;
					m_edit_id = dlg.m_edit_id;

					SqlString.Format(L"UPDATE Employees set Firstname='%s', LastName = '%s', Telephone = '%s' WHERE Employees.id = %s",m_edit_fn,m_edit_ln,m_edit_phone,m_edit_id);
					database.ExecuteSQL(SqlString);
					MessageBox(L"Data Updated Successfully!");
					SELECTED = -1;
					UpdateData(false);
				}
				ResetListControls();
				FetchData();

			}CATCH(CDBException, e) {
				AfxMessageBox(L"Database error: " + e->m_strError);
			}
			END_CATCH;
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
	database.Close();
}