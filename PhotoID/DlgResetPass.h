#pragma once


// CDlgResetPass dialog

class CDlgResetPass : public CDialog
{
	DECLARE_DYNAMIC(CDlgResetPass)

public:
	CDlgResetPass(CString strCurr, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgResetPass();

	CString GetNewPass(){ return m_strNewPass; }
// Dialog Data
	enum { IDD = IDD_DLG_RESET_PASS };

private:
	CString m_strCurr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strCurrPass;
	CString m_strNewPass;
};
