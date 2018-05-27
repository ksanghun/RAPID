// PropFormView.cpp : implementation file
//
#include <algorithm>        // std::sort
#include "stdafx.h"
#include "PhotoID.h"
#include "PropFormView.h"
#include "PhotoIDView.h"

// CPropFormView
#include "DlgApply.h"
#include "DlgSaveImage.h"
using namespace cv;

IMPLEMENT_DYNCREATE(CPropFormView, CFormView)

CPropFormView::CPropFormView()
	: CFormView(CPropFormView::IDD)
	, m_sliderRotate(0)

	, m_strRotValue(_T("0"))
	, m_fBrightNess(0)
	, m_fContrast(0)
	//, m_fEditBrightness(0)
	//, m_fEditContrast(0)
	, m_topMargin(0)
	, m_botMargin(0)
{
	m_preRotateSliderPos = 0;
	m_IsBtnCreated = false;

	m_fPreBrightness = 0;
	m_fPreContrast = 0;

	m_CurBrightness = 0;
	m_fCurContrast = 0.0f;

	m_UndoBrightness = 0;
	m_UndoContrast = 0;
}

CPropFormView::~CPropFormView()
{
}


void CPropFormView::ReSetSlideValues()
{
	m_fPreBrightness = 0;
	m_fPreContrast = 0;

	m_CurBrightness = 0;
	m_fCurContrast = 0.0f;

	m_UndoBrightness = 0;
	m_UndoContrast = 0;
}

void CPropFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_ROTATE, m_sliderRotate);
	DDV_MinMaxInt(pDX, m_sliderRotate, -300, 300);
	DDX_Control(pDX, IDC_SLIDER_ROTATE, m_ctrlSliderRotate);
	//DDX_Text(pDX, IDC_EDIT_ROT_VALUE, m_editRotate);
	//DDV_MinMaxFloat(pDX, m_editRotate, -90.0, 90.0);
	//	DDX_Text(pDX, IDC_EDIT_ROT_VALUE, m_strRotValue);
	//	DDV_MaxChars(pDX, m_strRotValue, 5);
	//	DDX_Control(pDX, IDC_EDIT_ROT_VALUE, m_EditCtrlRotate);
	DDX_Slider(pDX, IDC_SLIDER_BRINGTNESS, m_fBrightNess);
	DDV_MinMaxInt(pDX, m_fBrightNess, -100, 100);
	DDX_Slider(pDX, IDC_SLIDER_CONTRAST, m_fContrast);
	DDV_MinMaxInt(pDX, m_fContrast, -100, 100);
	DDX_Control(pDX, IDC_SLIDER_BRINGTNESS, m_SliderBrightness);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_SliderContrast);
	DDX_Control(pDX, IDC_BN_AUTOFIT, m_pButtonAutoFit);
	DDX_Control(pDX, IDC_BN_CROPIMG, m_pButtonCrop);
	DDX_Control(pDX, IDC_BN_STEMP, m_pButtonStamp);
	DDX_Control(pDX, IDC_BN_STEMP2, m_pButtonBlur);
	DDX_Control(pDX, IDC_BN_PRINT, m_pButtonPrint);
	//	DDX_Text(pDX, IDC_EDIT_BRINGT_VALUE, m_fEditBrightness);
	//	DDX_Text(pDX, IDC_EDIT_CONT_VALUE, m_fEditContrast);
	DDX_Control(pDX, IDC_SLIDER_CURSOR_SIZE, m_sliderCurSize);
	DDX_Control(pDX, IDC_COMBO1, m_comboCountryList);
	DDX_Control(pDX, IDC_BN_UNDO, m_btnUndo);
	//	DDX_Control(pDX, IDC_BN_EXIT_POHTOID, m_bnExitApp);
	DDX_Text(pDX, IDC_EDIT_TOP_MARGIN, m_topMargin);
	DDV_MinMaxUInt(pDX, m_topMargin, 0, 99999);
	DDX_Text(pDX, IDC_EDIT_BOT_MARGIN, m_botMargin);
	DDV_MinMaxUInt(pDX, m_botMargin, 0, 9999);
	DDX_Control(pDX, IDC_BN_SAVECROP, m_btnSave);
	DDX_Control(pDX, IDC_BN_ADMIN, m_btnAdmin);
	DDX_Control(pDX, IDC_BN_REOMVE_HL, m_btnRemoveHL);
	DDX_Control(pDX, IDC_BN_CB, m_btnColorBalance);
}

BEGIN_MESSAGE_MAP(CPropFormView, CFormView)
	ON_WM_MOUSEACTIVATE()
//	ON_EN_CHANGE(IDC_EDIT_ROT_VALUE, &CPropFormView::OnEnChangeEditRotValue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROTATE, &CPropFormView::OnNMCustomdrawSliderRotate)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_ROTATE, &CPropFormView::OnNMReleasedcaptureSliderRotate)
	ON_BN_CLICKED(IDC_BN_AUTOFIT, &CPropFormView::OnBnClickedBnAutofit)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BRINGTNESS, &CPropFormView::OnNMReleasedcaptureSliderBringtness)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CONTRAST, &CPropFormView::OnNMReleasedcaptureSliderContrast)
	ON_BN_CLICKED(IDC_BN_PRINT, &CPropFormView::OnBnClickedBnPrint)
	ON_BN_CLICKED(IDC_BN_CROPIMG, &CPropFormView::OnBnClickedBnCropimg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BRINGTNESS, &CPropFormView::OnNMCustomdrawSliderBringtness)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CONTRAST, &CPropFormView::OnNMCustomdrawSliderContrast)
	ON_BN_CLICKED(IDC_BN_STEMP2, &CPropFormView::OnBnClickedBnBlur)
	ON_BN_CLICKED(IDC_BN_STEMP, &CPropFormView::OnBnClickedBnStemp)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CURSOR_SIZE, &CPropFormView::OnNMCustomdrawSliderCursorSize)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPropFormView::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BN_UNDO, &CPropFormView::OnBnClickedBnUndo)
//	ON_BN_CLICKED(IDC_BN_EXIT_POHTOID, &CPropFormView::OnBnClickedBnExitPohtoid)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CPropFormView::OnDeltaposSpin2)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CPropFormView::OnDeltaposSpin3)
ON_BN_CLICKED(IDC_BN_SAVECROP, &CPropFormView::OnBnClickedBnSavecrop)
ON_BN_CLICKED(IDC_BN_ADMIN, &CPropFormView::OnBnClickedBnAdmin)
ON_BN_CLICKED(IDC_BN_CB, &CPropFormView::OnBnClickedBnCb)
ON_BN_CLICKED(IDC_BN_REOMVE_HL, &CPropFormView::OnBnClickedBnReomveHl)
END_MESSAGE_MAP()


// CPropFormView diagnostics

#ifdef _DEBUG
void CPropFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPropFormView message handlers

void CPropFormView::LoadCountryListFile()
{

	std::ifstream in;
	in.open("PhotoSize.txt");
	std::string strName, strType;
	int width, height, top, bot, sw, sh;


	m_countryList.clear();

	while (!in.eof())
	{
		in >> strType >> strName >> width >> height >> top >> bot >> sw >> sh;		
		_PHOTOID_FORMAT item;
		item.strSort = strName;
		item.name = strName.data();
		item.type = strType.data();


		item.photoSizeW = 0;
		item.photoSizeH = 0;

		item.widthMM = width;
		item.heightMM= height;
		item.topMarginMM = top;
		item.botMarginMM = bot;

		item.saveWidth = sw;
		item.saveHeight = sh;

		m_countryList.push_back(item);
	}

	in.close();


	std::sort(m_countryList.begin(), m_countryList.end(),
		[](const _PHOTOID_FORMAT& lhs, const _PHOTOID_FORMAT& rhs) {
			return lhs.strSort < rhs.strSort;
		}
	);

	for (auto i = 0; i < m_countryList.size(); i++){
		CString strItem, str;
		if (m_countryList[i].saveWidth == 0){
			strItem.Format(L"%s(%s): %dmm X %dmm, %d-%d", m_countryList[i].name, m_countryList[i].type, m_countryList[i].widthMM, m_countryList[i].heightMM, m_countryList[i].topMarginMM, m_countryList[i].botMarginMM);
		}
		else{
			strItem.Format(L"%s(%s): %dmm X %dmm, %d-%d, (%d X %d)", m_countryList[i].name, m_countryList[i].type, m_countryList[i].widthMM, m_countryList[i].heightMM, m_countryList[i].topMarginMM, m_countryList[i].botMarginMM, m_countryList[i].saveWidth, m_countryList[i].saveHeight);
		}
		m_comboCountryList.InsertString(i, strItem);
	}

	if (m_countryList.size() > 0){
		m_comboCountryList.SetCurSel(0);
		pView->SetPhotoFomat(m_countryList[0]);

		m_selContryList = m_countryList[0];
		m_topMargin = m_countryList[0].topMarginMM;
		m_botMargin = m_countryList[0].botMarginMM;

		UpdateData(FALSE);
	}

	// Load File//
	// Format //
	// Nation, width(mm), height(mm) //
	// ex) Canada: 50mm X 70mm 
	// Add list //
	// Set ComboBox //



}
void CPropFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	m_ctrlSliderRotate.SetRange(-300, 300, TRUE);
	m_ctrlSliderRotate.SetPos(0);
	m_ctrlSliderRotate.SetTicFreq(100);


	m_SliderBrightness.SetRange(-50, 50, TRUE);
	m_SliderBrightness.SetPos(0);
	m_SliderBrightness.SetTicFreq(10);

	m_SliderContrast.SetRange(-50, 50, TRUE);
	m_SliderContrast.SetPos(0);
	m_SliderContrast.SetTicFreq(10);
	
	m_ctrlSliderRotate.Invalidate(TRUE);



	m_sliderCurSize.SetRange(10, 100, TRUE);
	m_sliderCurSize.SetPos(55);
	m_sliderCurSize.SetTicFreq(2);

	pView->SetUserCursorSize(m_sliderCurSize.GetPos());

	SetSliderMode(false);
	m_btnUndo.EnableWindow(FALSE);

	// Set Button Icon //

	
//	m_bnExitApp.MoveWindow(297, 3, 32, 32);

	int xpos = 2, ypos = 5;
	int bsize = 40;
	m_pButtonAutoFit.MoveWindow(xpos, ypos, bsize, bsize);		xpos += (bsize + 1);
	m_pButtonCrop.MoveWindow(xpos, ypos, bsize, bsize);			xpos += (bsize + 1);
	m_pButtonStamp.MoveWindow(xpos, ypos, bsize, bsize);			xpos += (bsize + 1);
	m_pButtonBlur.MoveWindow(xpos, ypos, bsize, bsize);			xpos += (bsize + 1);
	m_pButtonPrint.MoveWindow(xpos, ypos, bsize, bsize);			xpos += (bsize + 1);
	m_btnSave.MoveWindow(xpos, ypos, bsize, bsize);				xpos += (bsize + 1);
	m_btnUndo.MoveWindow(xpos, ypos, bsize, bsize);				xpos += (bsize + 1);
	m_btnAdmin.MoveWindow(xpos, ypos, bsize, bsize);
	// second line //
	xpos = 2, ypos = 55;
	m_btnColorBalance.MoveWindow(xpos, ypos, bsize, bsize);		xpos += (bsize + 1);
	m_btnRemoveHL.MoveWindow(xpos, ypos, bsize, bsize);

	//m_btnUndo.ShowWindow(SW_HIDE);


	

	//m_pButtonAutoFit.SetBitmap((HBITMAP)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_FITFACE), IMAGE_BITMAP, 64, 64, LR_COLOR));
	//m_pButtonCrop.SetBitmap((HBITMAP)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_CROP), IMAGE_BITMAP, 64, 64, LR_COLOR));
	//m_pButtonStamp.SetBitmap((HBITMAP)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAMP), IMAGE_BITMAP, 64, 64, LR_COLOR));
	//m_pButtonBlur.SetBitmap((HBITMAP)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_BLUR), IMAGE_BITMAP, 64, 64, LR_COLOR));
	//m_pButtonPrint.SetBitmap((HBITMAP)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_PRINT), IMAGE_BITMAP, 64, 64, LR_COLOR));
	//CRect rect;
	//m_pButtonAutoFit.GetWindowRect(&rect);

	if (m_IsBtnCreated == false){
		CString text = _T("Automatic Adjustment of Image Angle");
		m_pButtonAutoFit.LoadBitmap(IDB_BITMAP_FITFACE);
		m_pButtonAutoFit.SetToolTipText(&text);

		text = _T("Crop Image for Photo ID");
		m_pButtonCrop.LoadBitmap(IDB_BITMAP_CROP);
		m_pButtonCrop.SetToolTipText(&text);

		text = _T("Stamp Tool");
		m_pButtonStamp.LoadBitmap(IDB_BITMAP_STAMP);
		m_pButtonStamp.SetToolTipText(&text);

		text = _T("Blur Tool");
		m_pButtonBlur.LoadBitmap(IDB_BITMAP_BLUR);
		m_pButtonBlur.SetToolTipText(&text);

		text = _T("Print");
		m_pButtonPrint.LoadBitmap(IDB_BITMAP_PRINT);
		m_pButtonPrint.SetToolTipText(&text);

		text = _T("Save");
		m_btnSave.LoadBitmap(IDB_BITMAP_SAVE);
		m_btnSave.SetToolTipText(&text);

		text = _T("Undo");
		m_btnUndo.LoadBitmap(IDB_BITMAP_UNDO);
		m_btnUndo.SetToolTipText(&text);
		
		text = _T("Admin");
		m_btnAdmin.LoadBitmap(IDB_BITMAP_ADMIN);
		m_btnAdmin.SetToolTipText(&text);


		text = _T("Color Balance");
		m_btnColorBalance.LoadBitmap(IDB_BITMAP_COLORBAL);
		m_btnColorBalance.SetToolTipText(&text);


		text = _T("Remove Highlights");
		m_btnRemoveHL.LoadBitmap(IDB_BITMAP_REMOVE_HL);
		m_btnRemoveHL.SetToolTipText(&text);
		
		
		m_IsBtnCreated = true;
	}


	UpdateData(TRUE);

}


void CPropFormView::SetUndoButton(bool IsEnable, unsigned short _type)
{
	if (IsEnable == false){  // finish undo
		m_btnUndo.EnableWindow(FALSE);
		//m_btnUndo.ShowWindow(SW_HIDE);

		//if (_type == 0){
		//	m_SliderBrightness.SetPos(m_UndoBrightness);
		//	m_CurBrightness = m_UndoBrightness;
		//}
		//else if (_type == 1){
		//	m_SliderContrast.SetPos(m_UndoContrast);
		//	m_fCurContrast = m_UndoContrast;
		//}
	}
	else{
		m_btnUndo.EnableWindow(TRUE);
		//m_btnUndo.ShowWindow(SW_SHOW);
	}

	UpdateData(FALSE);
}

void CPropFormView::SetSliderMode(bool IsCropMode)
{
	if (IsCropMode == false){
		m_ctrlSliderRotate.EnableWindow(TRUE);
		m_SliderBrightness.EnableWindow(FALSE);
		m_SliderContrast.EnableWindow(FALSE);
		m_sliderCurSize.EnableWindow(FALSE);
	}
	else{
		m_ctrlSliderRotate.EnableWindow(FALSE);
		m_SliderBrightness.EnableWindow(TRUE);
		m_sliderCurSize.EnableWindow(TRUE);
		m_SliderContrast.EnableWindow(TRUE);

		m_SliderContrast.SetPos(0);
		m_SliderBrightness.SetPos(0);
		m_sliderCurSize.SetPos(55);
	}
}

CPropFormView *CPropFormView::CreateOne(CWnd *pParent)
{
	CPropFormView *pFormView = new CPropFormView;
	//CMyFormView *pFormView = NULL;
	//CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CMyFormView);
	//pFormView = (CMyFormView *)pRuntimeClass->CreateObject();

	//CDockableFormViewAppDoc *pDoc = CDockableFormViewAppDoc::CreateOne();
	//pFormView->m_pDocument = pDoc;

	CCreateContext *pContext = NULL;

#if 0
	if (!pFormView->CreateEx(0, NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 205, 157),
		pParent, -1, pContext))
#else
	if (!pFormView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 205, 157), pParent, 0, pContext))
#endif
		//if( !pFormView->CreateEx( 0, AfxRegisterWndClass(0, 0, 0, 0), NULL,
		//  WS_CHILD | WS_VISIBLE, CRect( 0, 0, 205, 157), pParent, -1, pContext) )
	{
		AfxMessageBox(_T("Failed in creating CMyFormView"));
	}

	pFormView->OnInitialUpdate();

	return pFormView;
}

int CPropFormView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

//	return CFormView::OnMouseActivate(pDesktopWnd, nHitTest, message);

	int nResult = 0;

	CFrameWnd* pParentFrame = GetParentFrame();

	if (pParentFrame == pDesktopWnd)
	{
		// When this is docked
		nResult = CFormView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	else
	{
		// When this is not docked

		BOOL isMiniFrameWnd = pDesktopWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd));
		BOOL isPaneFrameWnd = pDesktopWnd->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd));
		BOOL isMultiPaneFrameWnd = pDesktopWnd->IsKindOf(RUNTIME_CLASS(CMultiPaneFrameWnd));

		// pDesktopWnd is the frame window for CDockablePane

		nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);

		//nResult = CWnd::OnMouseActivate( pDesktopWnd, nHitTest, message );
		//if( nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT )
		//  return nResult;

		//if (pDesktopWnd != NULL)
		//{
		//  // either re-activate the current view, or set this view to be active
		//  //CView* pView = pDesktopWnd->GetActiveView();
		//  //HWND hWndFocus = ::GetFocus();
		//  //if (pView == this &&
		//  //  m_hWnd != hWndFocus && !::IsChild(m_hWnd, hWndFocus))
		//  //{
		//  //  // re-activate this view
		//  //  OnActivateView(TRUE, this, this);
		//  //}
		//  //else
		//  //{
		//  //  // activate this view
		//  //  pDesktopWnd->SetActiveView(this);
		//  //}
		//}
	}

	return nResult;
}





//void CPropFormView::OnEnChangeEditRotValue()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CFormView::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Add your control notification handler code here
//
//	//UpdateData(TRUE);
//	//float fAngle = _wtof(m_strRotValue);
//	//if ((!m_strRotValue.IsEmpty()) && (fAngle)){
//
//	//	int sPos = fAngle * 10;
//	//	m_ctrlSliderRotate.SetPos(sPos);
//	//	pView->RotateImage(fAngle);
//	//}
//	//UpdateData(FALSE);
//}




void CPropFormView::OnNMCustomdrawSliderRotate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	//


		if (m_preRotateSliderPos != m_ctrlSliderRotate.GetPos()){

			float fRotate = m_ctrlSliderRotate.GetPos();
			m_strRotValue.Format(L"%3.1f", fRotate*0.1f);
	//		pView->RotateImage(fRotate*0.1f, false);
			UpdateData(FALSE);

			m_preRotateSliderPos = m_ctrlSliderRotate.GetPos();

		}
	

	*pResult = 0;
}


void CPropFormView::OnNMReleasedcaptureSliderRotate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	//UpdateData(TRUE);
	//float fRotate = m_ctrlSliderRotate.GetPos();
	//m_strRotValue.Format(L"%3.1f", fRotate*0.1f);
	//pView->RotateImage(fRotate*0.1f);
	////UpdateData(FALSE);

	*pResult = 0;
}


void CPropFormView::SetImageRotateValue(float _fAngle)
{
	UpdateData(TRUE);
//	m_editRotate = _fAngle;
	m_strRotValue.Format(L"%3.1f", _fAngle);
	int sPos = _fAngle * 10;
	m_ctrlSliderRotate.SetPos(sPos);
	UpdateData(FALSE);
}

BOOL CPropFormView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)){

		UpdateData(TRUE);
		float fAngle = _wtof(m_strRotValue);
		//if ((!m_strRotValue.IsEmpty()) && m_EditCtrlRotate.GetFocus()){
		if ((!m_strRotValue.IsEmpty())){

			int sPos = fAngle * 10;
			m_ctrlSliderRotate.SetPos(sPos);
			pView->RotateImage(fAngle);
		}
		UpdateData(FALSE);
	}


	return CFormView::PreTranslateMessage(pMsg);
}


void CPropFormView::OnBnClickedBnAutofit()
{
	// TODO: Add your control notification handler code here
	pView->ProcAutoFitImage();
	SetSliderMode(false);
}




void CPropFormView::OnNMReleasedcaptureSliderBringtness(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//CDlgApply dlg;
	//if (dlg.DoModal() == IDOK){
		UpdateData(TRUE);
		

	//	pView->ChangeBrightness(m_fBrightNess, true);		
		int diff = m_fBrightNess - m_CurBrightness;
		pView->ChangeBrightness(diff, true);
		
		m_UndoBrightness = m_CurBrightness;
		m_CurBrightness = m_fBrightNess;
	//	m_fPreBrightness = 0;
	//	m_SliderBrightness.SetPos(0);
		
	//	m_fEditBrightness = 0;
		UpdateData(FALSE);
	//}
	//else{
	//	UpdateData(TRUE);
	//	
	//	pView->ChangeBrightness(0, true);
	//			
	//	m_fPreBrightness = 0;
	//	m_SliderBrightness.SetPos(0);	
	////	m_fEditBrightness = 0;
	//	
	//	UpdateData(FALSE);
	//}	

	*pResult = 0;
}


void CPropFormView::OnNMReleasedcaptureSliderContrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//CDlgApply dlg;
	//if (dlg.DoModal() == IDOK){
		UpdateData(TRUE);
		float diff = m_fContrast - m_fCurContrast;

		float contrast = diff*0.005f + 1.0f;
		pView->ChangeContrast(contrast, true);

		m_UndoContrast = m_fCurContrast;
		m_fCurContrast = m_fContrast;


		//m_fContrast = 0;
		//m_fPreContrast = 0;		
		//m_SliderContrast.SetPos(0);
	//	m_fEditContrast = 0;
		UpdateData(FALSE);
	//}
	//else{
	//	UpdateData(TRUE);

	//	pView->ChangeBrightness(1.0f, true);		
	//	
	//	m_fContrast = 0;
	//	m_fPreContrast = 0;
	//	m_SliderContrast.SetPos(0);
	////	m_fEditContrast = 0;
	//	
	//	UpdateData(FALSE);
	//}




	*pResult = 0;
}


void CPropFormView::OnBnClickedBnPrint()
{
	// TODO: Add your control notification handler code here
	pView->PrintBitmap(L"PhtoID", false);
}



void CPropFormView::OnBnClickedBnCropimg()
{
	// TODO: Add your control notification handler code here
	pView->CropImage();	
	SetSliderMode(true);
}


void CPropFormView::OnNMCustomdrawSliderBringtness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_fPreBrightness != m_fBrightNess){


		int diff = m_fBrightNess - m_CurBrightness;
		pView->ChangeBrightness(diff, false);
	//	pView->ChangeBrightness(m_fBrightNess, false);
	//	m_fEditBrightness = m_fBrightNess;
		m_fPreBrightness = m_fBrightNess;
		
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CPropFormView::OnNMCustomdrawSliderContrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_fPreContrast != m_fContrast){

		float diff = m_fContrast - m_fCurContrast;

		float contrast = diff*0.01f + 1.0f;
		pView->ChangeContrast(contrast, false);
	//	m_fEditContrast = m_fContrast;
		m_fPreContrast = m_fContrast;

	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CPropFormView::OnBnClickedBnBlur()
{
	// TODO: Add your control notification handler code here


	pView->BlurImage(0);

	// TEST // iPlimge --> cv mat --> blur --> Iplimage
	//IplImage *pimg = cvLoadImage("D:/face.jpg");
	//cvShowImage("before", pimg);
	//
	//cv::Mat m = cv::cvarrToMat(pimg);
	//int orignaltype = m.type();

	//Mat mask = Mat::zeros(m.size(), m.type());
	//// mask is a disk
	//int radious = 100;
	//circle(mask, Point(200, 200), radious, Scalar(255, 255, 255), -1);
	//Mat md;
	//m.copyTo(md);

	//Size blurSize(7, 7);
	//blur(mask, mask, blurSize);
	//blur(md, md, blurSize);

	//mask.convertTo(mask, CV_32FC3, 1.0 / 255); // 
	//md.convertTo(md, CV_32FC3);
	//m.convertTo(m, CV_32FC3);

	//multiply(mask, md, md);
	//multiply(Scalar::all(1.0) - mask, m, m);

	//Mat ouImage = Mat::zeros(m.size(), m.type());
	//add(md, m, ouImage);

	//ouImage.convertTo(ouImage, orignaltype);

	//cvReleaseImage(&pimg);
	//pimg = new IplImage(ouImage);
	//cvShowImage("blur", pimg);





	
	// ** Direct Access Pixels //
	//void alphaBlend(Mat& foreground, Mat& background, Mat& alpha, Mat& outImage)
	//{
	//	// Find number of pixels. 
	//	int numberOfPixels = foreground.rows * foreground.cols * foreground.channels();

	//	// Get floating point pointers to the data matrices
	//	float* fptr = reinterpret_cast<float*>(foreground.data);
	//	float* bptr = reinterpret_cast<float*>(background.data);
	//	float* aptr = reinterpret_cast<float*>(alpha.data);
	//	float* outImagePtr = reinterpret_cast<float*>(outImage.data);

	//	// Loop over all pixesl ONCE
	//	for (
	//		int i = 0;
	//		i < numberOfPixels;
	//	i++, outImagePtr++, fptr++, aptr++, bptr++
	//		)
	//	{
	//		*outImagePtr = (*fptr)*(*aptr) + (*bptr)*(1 - *aptr);
	//	}
	//}
}


void CPropFormView::OnBnClickedBnStemp()
{
	// TODO: Add your control notification handler code here
	pView->StampImage();
}


void CPropFormView::OnNMCustomdrawSliderCursorSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	pView->SetUserCursorSize(m_sliderCurSize.GetPos());

	*pResult = 0;
}


void CPropFormView::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int pos = m_comboCountryList.GetCurSel();
	if (pos < m_countryList.size()){
		pView->SetPhotoFomat(m_countryList[pos]);
		m_selContryList = m_countryList[pos];

		m_topMargin = m_countryList[pos].topMarginMM;
		m_botMargin = m_countryList[pos].botMarginMM;
	}

	UpdateData(FALSE);
}


void CPropFormView::OnBnClickedBnUndo()
{
	// TODO: Add your control notification handler code here
	pView->Undo();
}


//void CPropFormView::OnBnClickedBnExitPohtoid()
//{
//	// TODO: Add your control notification handler code here
//	exit(0);
//}


void CPropFormView::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMUpDown->iDelta == -1) {
		m_topMargin++;
	}

	else if (pNMUpDown->iDelta == 1)  {
		m_topMargin--;
	}

	m_selContryList.topMarginMM = m_topMargin;
	pView->SetPhotoFomat(m_selContryList);

	*pResult = 0;

	UpdateData(FALSE);
}


void CPropFormView::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMUpDown->iDelta == -1) {
		m_botMargin++;
	}

	else if (pNMUpDown->iDelta == 1)  {
		m_botMargin--;
	}
	*pResult = 0;

	m_selContryList.botMarginMM = m_botMargin;
	pView->SetPhotoFomat(m_selContryList);

	UpdateData(FALSE);
}


void CPropFormView::DisplayPreview(void* _pImg)
{
	IplImage* pImg = (IplImage*)_pImg;


	if (pImg != nullptr){
		CWnd* pWnd_ImageTraget = GetDlgItem(IDC_PREVIEW);
		CClientDC dcImageTraget(pWnd_ImageTraget);
		RECT rcImageTraget;
		pWnd_ImageTraget->GetClientRect(&rcImageTraget);

		BITMAPINFO bitmapInfo;
		memset(&bitmapInfo, 0, sizeof(bitmapInfo));
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = pImg->width;
		bitmapInfo.bmiHeader.biHeight = -pImg->height;

		IplImage *tempImage = nullptr;

		if (pImg->nChannels == 1)
		{
			tempImage = cvCreateImage(cvSize(pImg->width, pImg->height), IPL_DEPTH_8U, 3);
			cvCvtColor(pImg, tempImage, CV_GRAY2BGR);
		}
		else if (pImg->nChannels == 3)
		{			
			tempImage = cvCloneImage(pImg);
		}

		bitmapInfo.bmiHeader.biBitCount = tempImage->depth * tempImage->nChannels;

		dcImageTraget.SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(dcImageTraget.GetSafeHdc(), rcImageTraget.left, rcImageTraget.top, rcImageTraget.right, rcImageTraget.bottom,
			0, 0, tempImage->width, tempImage->height, tempImage->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}

void CPropFormView::OnBnClickedBnSavecrop()
{
	// TODO: Add your control notification handler code here
	CString strPath, strName;
	UINT w, h;
	float r;
	int type=0;
	if (pView->GetCropImgInfo(strPath, strName, w, h, r)){

		if (m_selContryList.saveHeight > 0){
			w = m_selContryList.saveWidth;
			h = m_selContryList.saveHeight;
		}

		CDlgSaveImage dlgSave(strPath, strName, w, h, r);
		if (dlgSave.DoModal() == IDOK){

			dlgSave.GetSaveInfo(strPath, strName, w, h, type);
			pView->SaveCrop(strPath, strName, w, h, type);
			pView->PrintBitmap(L"PhtoID", true);
		}
	}

//	pView->SaveCrop();
}

#include "MainFrm.h"
#include "DlgLogList.h"
#include "DlgPass.h"
void CPropFormView::OnBnClickedBnAdmin()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
	CString strCurrPass = pM->GetAdminInfo().d;

	CDlgPass dlg(strCurrPass);
	if (dlg.DoModal() == IDOK){
		CDlgLogList dlgLog;
		if (dlgLog.DoModal() == IDOK){

		}
	}

	//CString sPath;
	//GetModuleFileName(nullptr, sPath.GetBuffer(_MAX_PATH + 1), _MAX_PATH);
	//sPath.ReleaseBuffer();
	//CString path = sPath.Left(sPath.ReverseFind(_T('\\')));



	//// Read Current Password //
	//_ADMIN_PASS p;
	//CString strFle = path + "\\data\\shape_predictor_68_face_landmarks.bin";
	//FILE* fp = 0;
	//fopen_s(&fp, (CStringA)strFle, "rb");
	//if (fp == nullptr){
	//	// create new //
	//	FILE* fp2 = 0;
	//	fopen_s(&fp2, (CStringA)strFle, "wb");		
	//	p.a = 512;
	//	p.b = 32;
	//	p.c = 0;

	//	strCurrPass = L"1111";
	//	memset(&p.d, 0x00, sizeof(wchar_t)*32);
	//	wsprintf(p.d, strCurrPass.GetBuffer());
	//	fwrite(&p, sizeof(_ADMIN_PASS), 1, fp2);
	//	fclose(fp2);
	//}
	//else{
	//	_ADMIN_PASS p;
	//	fread(&p, sizeof(_ADMIN_PASS) * 32, 1, fp);		
	//	strCurrPass = p.d;
	//	fclose(fp);
	//}
	//===================================//


	
}


void CPropFormView::OnBnClickedBnCb()
{
	// TODO: Add your control notification handler code here
	pView->ColorBalance();
}


void CPropFormView::OnBnClickedBnReomveHl()
{
	// TODO: Add your control notification handler code here
	pView->RemoveHightlights();
}
