#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"

#include "MySlider.h"
#include "HoverButton.h"
#include "data_type.h"

// CPropFormView form view
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;




class CPropFormView : public CFormView
{
	DECLARE_DYNCREATE(CPropFormView)

protected:
	CPropFormView();           // protected constructor used by dynamic creation
	virtual ~CPropFormView();

	
public:
	enum { IDD = IDD_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif


	static CPropFormView* CreateOne(CWnd *pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	
	void SetImageRotateValue(float _fAngle);
	void SetSliderMode(bool IsCropMode);
	void SetUndoButton(bool IsEnable, unsigned short _type);
	void DisplayPreview(void* _pImg);
	void ReSetSlideValues();
	
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	int m_sliderRotate;


	CMySlider m_ctrlSliderRotate;
	int m_preRotateSliderPos;

//	afx_msg void OnEnChangeEditRotValue();
	afx_msg void OnNMCustomdrawSliderRotate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderRotate(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strRotValue;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	CEdit m_EditCtrlRotate;
	afx_msg void OnBnClickedBnAutofit();
	afx_msg void OnNMReleasedcaptureSliderBringtness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderContrast(NMHDR *pNMHDR, LRESULT *pResult);
	int m_fBrightNess;
	int m_fContrast;
	CSliderCtrl m_SliderBrightness;
	CSliderCtrl m_SliderContrast;
	afx_msg void OnBnClickedBnPrint();
	CHoverButton m_pButtonAutoFit;
	CHoverButton m_pButtonCrop;
	CHoverButton m_pButtonStamp;
	CHoverButton m_pButtonBlur;
	CHoverButton m_pButtonPrint;
	CHoverButton m_btnSave;
	CHoverButton m_btnAdmin;

	CHoverButton m_btnRemoveHL;
	CHoverButton m_btnColorBalance;

	bool m_IsBtnCreated;
	int m_fPreBrightness;
	int m_CurBrightness;
	
	int m_UndoBrightness;
	int m_UndoContrast;

	int m_fPreContrast;
	int m_fCurContrast;


	void LoadCountryListFile();
	std::vector<_PHOTOID_FORMAT>m_countryList;
	_PHOTOID_FORMAT m_selContryList;

	afx_msg void OnBnClickedBnCropimg();
	//float m_fEditBrightness;
	//float m_fEditContrast;
	afx_msg void OnNMCustomdrawSliderBringtness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderContrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBnBlur();
	afx_msg void OnBnClickedBnStemp();
	CSliderCtrl m_sliderCurSize;
	afx_msg void OnNMCustomdrawSliderCursorSize(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_comboCountryList;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedBnUndo();
	CHoverButton m_btnUndo;
//	CHoverButton m_bnExitApp;
//	afx_msg void OnBnClickedBnExitPohtoid();
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	UINT m_topMargin;
	UINT m_botMargin;
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedBnSavecrop();
	
	afx_msg void OnBnClickedBnAdmin();
	afx_msg void OnBnClickedBnCb();
	afx_msg void OnBnClickedBnReomveHl();
	
	CSliderCtrl m_sliderHL;
	CSliderCtrl m_sliderCB;
	int m_cbTh;
	int m_hlTh;
	afx_msg void OnNMReleasedcaptureSliderHl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderCb(NMHDR *pNMHDR, LRESULT *pResult);
};


