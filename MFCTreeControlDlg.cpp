
// MFCTreeControlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCTreeControl.h"
#include "MFCTreeControlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

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


// CMFCTreeControlDlg dialog



CMFCTreeControlDlg::CMFCTreeControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCTREECONTROL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTreeControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_tree);
}

BEGIN_MESSAGE_MAP(CMFCTreeControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Add, &CMFCTreeControlDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_Delete, &CMFCTreeControlDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_Edit, &CMFCTreeControlDlg::OnBnClickedButtonEdit)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CMFCTreeControlDlg::OnTvnSelchangedTree)
END_MESSAGE_MAP()


// CMFCTreeControlDlg message handlers

BOOL CMFCTreeControlDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCTreeControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCTreeControlDlg::OnPaint()
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
HCURSOR CMFCTreeControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCTreeControlDlg::OnBnClickedButtonAdd()
{
	CString addText;
	GetDlgItemText(IDC_EDIT_Text, addText);//text selection in edit box with exception handling
	if (addText.GetLength() == 0)
	{
		AfxMessageBox(_T("No Text inserted to Add!"));
		return;
	}

	HTREEITEM hItem = m_tree.GetSelectedItem(); 
	if (hItem == NULL)					//if nothing were in the tree, then the upcoming item will be the root item
		hItem = TVI_ROOT;

	//setting the properties of the upcoming item
	TVINSERTSTRUCT ts = { 0 };
	ts.hParent = hItem;
	ts.hInsertAfter = TVI_LAST;
	ts.item.pszText = addText.GetBuffer();
	ts.item.mask = TVIF_TEXT;
	
	
	HTREEITEM hNewItem =  m_tree.InsertItem(&ts);//adding the item to the tree
	m_tree.SelectItem(hNewItem);				//give the selection to the added item
	m_tree.EnsureVisible(hNewItem);				//make the item visible
}


void CMFCTreeControlDlg::OnBnClickedButtonDelete()
{
	HTREEITEM hItem = m_tree.GetSelectedItem();
	if (hItem == NULL)
	{
		AfxMessageBox(_T("No item selected to delete!"));
		return;
	}
	HTREEITEM hParent = m_tree.GetParentItem(hItem); //before deleting the node, storing its parent information
	m_tree.DeleteItem(hItem); //deleting the item
	m_tree.SelectItem(hParent);//selection goes to parent
}


void CMFCTreeControlDlg::OnBnClickedButtonEdit()
{
	HTREEITEM hItem = m_tree.GetSelectedItem(); //item selection with exception handling
	if (hItem == NULL)
	{
		AfxMessageBox(_T("No item selected!"));
		return;
	}
	CString editText;
	GetDlgItemText(IDC_EDIT_Text, editText); //text selection in edit box with exception handling
	if (editText.GetLength() == 0)
	{
		AfxMessageBox(_T("No Text inserted to edit!"));
		return;
	}
	m_tree.SetItemText(hItem, editText);	//select the name of the selected item according to the edit box text
}


void CMFCTreeControlDlg::OnTvnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM hItem = m_tree.GetSelectedItem(); //if the tree is not null, then the item we select from dialog, in the edit box, that items text will appear
	if (hItem != NULL)
	{
		CString strText = m_tree.GetItemText(hItem);
		SetDlgItemText(IDC_EDIT_Text, strText);
	}
	*pResult = 0;
}
