#pragma once


// CDlgPass dialog

class CDlgPass : public CDialog
{
	DECLARE_DYNAMIC(CDlgPass)

public:
	CDlgPass(CString strPass, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPass();

// Dialog Data
	enum { IDD = IDD_DLG_PASS };

private:
	CString m_strCurrPass;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPass;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBnResetPass();
};
