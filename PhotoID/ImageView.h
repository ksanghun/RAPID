#pragma once
#include "OGLWnd.h"

#include "opencv/cv.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

#include "SNImage.h"
#include "GLHScrollBar.h"
#include "GuideLine.h"


enum _RENDERMODE { _BYGROUP = 0, _BYORDER };
enum _PICKMODE { _PICK_SELECT=0 };
//enum _FACEPOS { _EYE_CENTER, _LEFT_EYE, _RIGHT_EYE, _TOP_EYE, _BOTTOM_EYE, _LIP, _NOSE, _TOPHEAD, _CHIN};
//#define _LNADMARK_POS_NUM 9
#define SEL_BUFF_SIZE 1024



class CViewTree;

typedef std::vector<CSNImage*> _vecSNImage;


static UINT MyThread(LPVOID lpParam);
static bool  m_bIsThreadEnd;


class CImageView :
	public COGLWnd
{
public:
	CImageView();
	~CImageView();

	void Render();
	void Render2D();
	void RenderMenu();
	void DrawDebugInfo();
	void DrawCircle(POINT2D pos, float fScale);
	void InitGLview(int _nWidth, int _nHeight);
	void MouseWheel(short zDelta);

	bool LoadSNImage(CString strPath, CSNImage* pInfo);
	GLuint LoadSNImage(CString strPath);

	void ReleaseImageData();
	CBitmap* GetLogCBitmap(CString strFile);

	void ThreadFaceDataLoad();
	void SetPhotoIDimg(CString strPath);
	float RotateImage(float fAngle, bool IsRedetect = true);
	float GetImgAngle(); 
	float GetDeSkewAngle() { return m_fDeSkewAngle; }

	//POINT2D convertScreenToImageSpace(POINT2D pnt);
	//POINT2D convertImageToScreenSpace(POINT2D pnt);

	void ChangeBrightness(float _value, bool IsApply);
	void ChangeContrast(float _value, bool IsApply);
	void SetUserCursorSize(int _size);
	void SetPhotoFomat(_PHOTOID_FORMAT _format);
	void Undo();
	void ColorBalance();
	void RemoveHightlights();
	void SaveCrop(CString strPath, CString strName, UINT w, UINT h, int type);
	bool GetCropImgInfo(CString& path, CString& name, UINT& _w, UINT& _h, float& r);

	GLuint Load4ChannelImage(char* sz);

	IplImage* GetPrintPhoto(bool IsSample);
	CString GetPrintPhotoName();
	void SetCropPhoto();
	void BlurPhoto(int _size);
	void StampImage();
	CSNImage* GetPhotoIDImg() { return m_pPhotoImg; }

	float GetDetectedScale(){ return m_fImgDetectScale;  }

private:
//	CWinThread* g_pl;

	int m_maxTextureSize;

	CPoint m_mousedown;
	CPoint m_preMmousedown;
	short m_mouseMode;

	POINT3D		m_lookAt;
	LOGFONT		m_LogFont;
	LOGFONT		m_LogFontBig;
	BITMAPINFO* m_pBmpInfo;

	int			m_nWidth;
	int			m_nHeight;

	CGLHScrollBar m_glHScrollBar;

	float m_left, m_right, m_bottom, m_top;

	// Image Data Set //
	CSNImage* m_pPhotoImg;

	float	m_iconSize;
	float	m_fScreenScale;
	void ReSizeIcon();

	int select_object_2d(int x, int y, int rect_width, int rect_height, _PICKMODE _mode);
	bool process_select(GLuint* index, int hit_num, int selmode);
	void ResetIconTextureId();
	void GenerateThumbnail();
	

	POINT3D GetColor(float fvalue);
	POINT3D m_result_color[10];

	bool FaceDetection(IplImage* pImg);
	
	

	
	std::vector<POINT2D> m_faceLandmark;
	std::vector<POINT2D> m_faceLandmarkDraw;
	cv::Rect m_rectEyes[2];
	cv::Rect m_rectFace;
//	POINT3D* m_faceLandmarkDraw;
	dlib::shape_predictor m_sp;
	float m_fDeSkewAngle;

	float m_fXScale, m_fYScale;
	float m_fImgDetectScale;
	CString m_strMousePos;

	unsigned int m_iFrameCnt;
	

	POINT3D m_vecCircle[36];
	POINT2D m_curPos;	
	float m_cursorSize;
	float m_cursorSizeForDisp;
	bool m_bBlurMode;
	bool m_bStampMode;
	bool m_bStampCopied;

	// Image Guide Line =================================================//
	void DrawCropArea();
	void SetCropArea(bool IsPreview);
	

	POINT2D m_guidePosDraw[_LNADMARK_POS_NUM];
	POINT2D m_vecOutBounderyDraw[4];
	POINT2D m_vecInBounderyDraw[4];
	CGuideLine m_guideLine[5];// 0: bottom 1: center: 2: top, 3: vertical center
	int m_selButtonId;
	//===================================================================//



//	cv::Mat m_imgStampcut;
	cv::Rect m_rectForStamp;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);



	

	afx_msg void OnNcDestroy();


	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

