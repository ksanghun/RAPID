// DlgLogList.cpp : implementation file
//

#include "stdafx.h"
#include "PhotoID.h"
#include "DlgLogList.h"
#include "afxdialogex.h"


// CDlgLogList dialog
#define START_YEAR 2018
#define START_MONTH 1

IMPLEMENT_DYNAMIC(CDlgLogList, CDialog)

CDlgLogList::CDlgLogList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogList::IDD, pParent)
{
	m_nAddCnt = 0;
}

CDlgLogList::~CDlgLogList()
{
}

void CDlgLogList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_comboYear);
	DDX_Control(pDX, IDC_COMBO_MONTH, m_comboMonth);
	DDX_Control(pDX, IDC_LIST1, m_listLog);
}


BEGIN_MESSAGE_MAP(CDlgLogList, CDialog)
	ON_BN_CLICKED(IDC_BN_SEARCH_LOG, &CDlgLogList::OnBnClickedBnSearchLog)
END_MESSAGE_MAP()


// CDlgLogList message handlers


void CDlgLogList::OnBnClickedBnSearchLog()
{
	// TODO: Add your control notification handler code here
	//SYSTEMTIME st;
	//GetSystemTime(&st);

	UpdateData(FALSE);
	int yPos = m_comboYear.GetCurSel();
	int mPos = m_comboMonth.GetCurSel();



	CString strName;
	strName.Format(L"\\%d%d.log", yPos+START_YEAR, mPos+START_MONTH);

	//CString sPath;
	//GetModuleFileName(nullptr, sPath.GetBuffer(_MAX_PATH + 1), _MAX_PATH);
	//sPath.ReleaseBuffer();
	//CString path = sPath.Left(sPath.ReverseFind(_T('\\')));
	TCHAR name[UNLEN + 1];
	DWORD size = UNLEN + 1;

	if (GetUserName((TCHAR*)name, &size)){
		CString strUser = name;
		CString strRoot = _T("C:\\Users\\") + strUser + _T("\\PhotoData");
		CString strFle = strRoot + strName;


		FILE* fp = 0;
		fopen_s(&fp, (CStringA)strFle, "rb");
		if (fp){
			fseek(fp, 0, SEEK_END);
			int size = ftell(fp) / sizeof(_WORK_ITEM);
			fseek(fp, 0, SEEK_SET);


			_WORK_ITEM item;
			m_listLog.DeleteAllItems();
			m_nAddCnt = 0;
			for (auto i = 0; i < size; i++){
				fread(&item, sizeof(_WORK_ITEM), 1, fp);
				AddLogList(item);
			}
			fclose(fp);
		}
		else{
			AfxMessageBox(L"No search results found");
		}
	}

}

void CDlgLogList::AddLogList(_WORK_ITEM item)
{
	CString strItem;

//	strItem.Format(L"%d", item.workId);
	strItem.Format(L"%d", m_nAddCnt+1);
	m_listLog.InsertItem(m_nAddCnt, strItem, 0);

	if (item.workType == __WORK_SAVE){
		m_listLog.SetItem(m_nAddCnt, 1, LVIF_TEXT, L"Save File", 0, 0, 0, NULL);
	}
	else{
		m_listLog.SetItem(m_nAddCnt, 1, LVIF_TEXT, L"Print File", 0, 0, 0, NULL);
	}

	m_listLog.SetItem(m_nAddCnt, 2, LVIF_TEXT, item.finename, 0, 0, 0, NULL);

	strItem.Format(L"%d/%d/%d  %d:%d:%d", item.date.wMonth, item.date.wDay, item.date.wYear, item.date.wHour, item.date.wMinute, item.date.wSecond);
	m_listLog.SetItem(m_nAddCnt, 3, LVIF_TEXT, strItem, 0, 0, 0, NULL);
	m_nAddCnt++;
	
}

BOOL CDlgLogList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_listLog.InsertColumn(0, L"ID", LVCFMT_LEFT, 50, -1);
	m_listLog.InsertColumn(1, L"TYPE", LVCFMT_LEFT, 100, -1);
	m_listLog.InsertColumn(2, L"FILE", LVCFMT_LEFT, 200, -1);
	m_listLog.InsertColumn(3, L"DATE", LVCFMT_LEFT, 200, -1);
	


	CString strItem;
	for (int i = 0; i < 12; i++){
		int year = START_YEAR + i;
		int month = START_MONTH + i;

		strItem.Format(L"%d", year);
		m_comboYear.AddString(strItem);

		strItem.Format(L"%d", month);
		m_comboMonth.AddString(strItem);
	}



	SYSTEMTIME st;
	GetSystemTime(&st);

	int yearPos = st.wYear - START_YEAR;
	int monthPos = st.wMonth - START_MONTH;

	m_comboYear.SetCurSel(yearPos);
	m_comboMonth.SetCurSel(monthPos);

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
