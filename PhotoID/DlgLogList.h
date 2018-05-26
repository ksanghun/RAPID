#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MainFrm.h"

// CDlgLogList dialog

class CDlgLogList : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogList)

public:
	CDlgLogList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLogList();

	void AddLogList(_WORK_ITEM item);

// Dialog Data
	enum { IDD = IDD_DILG_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

private:
	CComboBox m_comboYear;
	CComboBox m_comboMonth;
	afx_msg void OnBnClickedBnSearchLog();
	virtual BOOL OnInitDialog();
	CListCtrl m_listLog;
	int m_nAddCnt;

};
