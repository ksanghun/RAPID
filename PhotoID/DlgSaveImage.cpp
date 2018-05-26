// DlgSaveImage.cpp : implementation file
//

#include "stdafx.h"
#include "PhotoID.h"
#include "DlgSaveImage.h"
#include "afxdialogex.h"


// CDlgSaveImage dialog

IMPLEMENT_DYNAMIC(CDlgSaveImage, CDialog)

CDlgSaveImage::CDlgSaveImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSaveImage::IDD, pParent)
	, m_strSavePath(_T(""))
	, m_strFileName(_T(""))
	, m_uintSaveWidth(0)
	, m_uintSaveHeight(0)
{

}

CDlgSaveImage::CDlgSaveImage(CString strPath, CString strFile, UINT _w, UINT _h, float aRatio)
	: CDialog(CDlgSaveImage::IDD, NULL)
{
	m_strSavePath = strPath;
	m_strFileName = strFile;
	m_uintSaveWidth = _w;
	m_uintSaveHeight = _h;
	m_aRatio = aRatio;
}

CDlgSaveImage::~CDlgSaveImage()
{
}

void CDlgSaveImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_strSavePath);
	DDX_Text(pDX, IDC_EDIT2, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_SAVE_WIDTH, m_uintSaveWidth);
	DDV_MinMaxUInt(pDX, m_uintSaveWidth, 1, 9999);
	DDX_Text(pDX, IDC_EDIT_SAVE_HEIGHT, m_uintSaveHeight);
	DDV_MinMaxUInt(pDX, m_uintSaveHeight, 1, 9999);
	DDX_Control(pDX, IDC_COMBO_IMGFORMAT, m_comboImgFormat);
}


BEGIN_MESSAGE_MAP(CDlgSaveImage, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSaveImage::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BN_PATH, &CDlgSaveImage::OnBnClickedBnPath)
	ON_EN_CHANGE(IDC_EDIT_SAVE_WIDTH, &CDlgSaveImage::OnEnChangeEditSaveWidth)
	ON_EN_CHANGE(IDC_EDIT_SAVE_HEIGHT, &CDlgSaveImage::OnEnChangeEditSaveHeight)
END_MESSAGE_MAP()


// CDlgSaveImage message handlers


void CDlgSaveImage::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CDlgSaveImage::GetSaveInfo(CString& strPath, CString& strName, int w, int h, int type)
{
	strPath = m_strSavePath;
	strName = m_strFileName;
	w = m_uintSaveWidth;
	h = m_uintSaveHeight;
	type = m_comboImgFormat.GetCurSel();
}


BOOL CDlgSaveImage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_comboImgFormat.AddString(L"jpg");
	m_comboImgFormat.AddString(L"bmp");
	m_comboImgFormat.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSaveImage::OnBnClickedBnPath()
{
	// TODO: Add your control notification handler code here
	ITEMIDLIST *pidlBrowse;
	TCHAR       pszPathname[MAX_PATH];
	BROWSEINFO  BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
	if (pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	m_strSavePath = (LPCTSTR)pszPathname;

	UpdateData(FALSE);
}


void CDlgSaveImage::OnEnChangeEditSaveWidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	m_uintSaveHeight = m_uintSaveWidth / m_aRatio;
	UpdateData(FALSE);

}


void CDlgSaveImage::OnEnChangeEditSaveHeight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	m_uintSaveWidth = m_uintSaveHeight * m_aRatio;
	UpdateData(FALSE);
}
