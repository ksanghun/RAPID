// DlgResetPass.cpp : implementation file
//

#include "stdafx.h"
#include "PhotoID.h"
#include "DlgResetPass.h"
#include "afxdialogex.h"


// CDlgResetPass dialog

IMPLEMENT_DYNAMIC(CDlgResetPass, CDialog)

CDlgResetPass::CDlgResetPass(CString strCurr, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResetPass::IDD, pParent)
	, m_strCurrPass(_T(""))
	, m_strNewPass(_T(""))
{
	m_strCurr = strCurr;
}

CDlgResetPass::~CDlgResetPass()
{
}

void CDlgResetPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CURR_PASS, m_strCurrPass);
	DDX_Text(pDX, IDC_EDIT_NEW_PASS, m_strNewPass);
}


BEGIN_MESSAGE_MAP(CDlgResetPass, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgResetPass::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgResetPass message handlers


void CDlgResetPass::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strCurr == m_strCurrPass){
		//Update Password //		
		CDialog::OnOK();
	}
	else{
		AfxMessageBox(L"Current Pasword is incorrect.");
	}
	
}
