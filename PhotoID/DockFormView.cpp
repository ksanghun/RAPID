// DockFormView.cpp : implementation file
//

#include "stdafx.h"
#include "PhotoID.h"
#include "DockFormView.h"


// CDockFormView

IMPLEMENT_DYNAMIC(CDockFormView, CDockablePane)

CDockFormView::CDockFormView()
{
	m_pMyFormView = NULL;
}

CDockFormView::~CDockFormView()
{
	//if (m_pMyFormView)
	//	delete m_pMyFormView;
}


BEGIN_MESSAGE_MAP(CDockFormView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDockFormView message handlers

void CDockFormView::SetImageRotateValue(float _fAngle)
{
	if (m_pMyFormView){
		m_pMyFormView->SetImageRotateValue(_fAngle);
	}

}

void CDockFormView::SetSliderStatus(bool IsCropMode)
{
	if (m_pMyFormView){
		m_pMyFormView->SetSliderMode(IsCropMode);
	}
}

void CDockFormView::SetUndoButton(bool IsEnable, unsigned short _type)
{
	if (m_pMyFormView){
		m_pMyFormView->SetUndoButton(IsEnable, _type);
	}
}

void CDockFormView::ReSetSlideValues()
{
	if (m_pMyFormView){
		m_pMyFormView->ReSetSlideValues();
	}
}

void CDockFormView::DisplayPreview(void* pImg)
{
	if (m_pMyFormView){
		m_pMyFormView->DisplayPreview(pImg);
	}
}

void CDockFormView::LoadCountryFormat()
{
	if (m_pMyFormView){
		m_pMyFormView->LoadCountryListFile();
	}
}

int CDockFormView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pMyFormView = CPropFormView::CreateOne(this);

	

	return 0;
}


void CDockFormView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pMyFormView){
		m_pMyFormView->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}

BOOL CDockFormView::OnShowControlBarMenu(CPoint point)
{
	return TRUE;
}