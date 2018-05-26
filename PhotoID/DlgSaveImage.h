#pragma once
#include "afxwin.h"


// CDlgSaveImage dialog

class CDlgSaveImage : public CDialog
{
	DECLARE_DYNAMIC(CDlgSaveImage)

public:
	CDlgSaveImage(CWnd* pParent = NULL);   // standard constructor
	CDlgSaveImage(CString strPath, CString strFile, UINT _w, UINT _h, float aRatio);
	virtual ~CDlgSaveImage();

// Dialog Data
	enum { IDD = IDD_DLG_SAVEIMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSavePath;
	CString m_strFileName;
	UINT m_uintSaveWidth;
	UINT m_uintSaveHeight;
	float m_aRatio;
	CComboBox m_comboImgFormat;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBnPath();
	afx_msg void OnEnChangeEditSaveWidth();
	afx_msg void OnEnChangeEditSaveHeight();


	void GetSaveInfo(CString& strPath, CString& strName, int w, int h, int type);
};
