// DlgPass.cpp : implementation file
//

#include "stdafx.h"
#include "PhotoID.h"
#include "DlgPass.h"
#include "afxdialogex.h"


// CDlgPass dialog

IMPLEMENT_DYNAMIC(CDlgPass, CDialog)

CDlgPass::CDlgPass(CString strPass, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPass::IDD, pParent)
	, m_strPass(_T(""))
{
	m_strCurrPass = strPass;
}

CDlgPass::~CDlgPass()
{
}

void CDlgPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASS, m_strPass);
}


BEGIN_MESSAGE_MAP(CDlgPass, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPass::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BN_RESET_PASS, &CDlgPass::OnBnClickedBnResetPass)
END_MESSAGE_MAP()


// CDlgPass message handlers


void CDlgPass::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);


	if (m_strCurrPass == m_strPass){
		CDialog::OnOK();
	}
	else{
		AfxMessageBox(L"Pasword is incorrect.");
	}
}


#include "MainFrm.h"
#include "DlgResetPass.h"
void CDlgPass::OnBnClickedBnResetPass()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
	CDlgResetPass dlg(m_strCurrPass);




	_ADMIN_PASS p = pM->GetAdminInfo();
	if (dlg.DoModal() == IDOK){

		//CString sPath;
		//GetModuleFileName(nullptr, sPath.GetBuffer(_MAX_PATH + 1), _MAX_PATH);
		//sPath.ReleaseBuffer();
		//CString path = sPath.Left(sPath.ReverseFind(_T('\\')));

		TCHAR name[UNLEN + 1];
		DWORD size = UNLEN + 1;

		if (GetUserName((TCHAR*)name, &size)){
			CString strUser = name;
			CString strRoot = _T("C:\\Users\\") + strUser + _T("\\PhotoData");
			CString strFle = strRoot + "\\shape_predictor_68_face_landmarks.bin";

			CString strNewPass = dlg.GetNewPass();

			FILE* fp2 = 0;
			fopen_s(&fp2, (CStringA)strFle, "wb");
			memset(&p.d, 0x00, sizeof(wchar_t) * 32);
			wsprintf(p.d, strNewPass.GetBuffer());
			fwrite(&p, sizeof(_ADMIN_PASS), 1, fp2);
			fclose(fp2);

			pM->SetAdminInfo(p);
		}
		CDialog::OnCancel();
	}
}
