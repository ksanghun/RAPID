#pragma once
#include "opencv/cv.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "math_tool.h"
#include "UnDoObj.h"
enum _FACEPOS { _EYE_CENTER, _LEFT_EYE, _RIGHT_EYE, _FCENTER, _TOP_EYE, _BOTTOM_EYE, _NOSE, _TOPHEAD, _CHIN };
#define _LNADMARK_POS_NUM 9
#define _FACE_IMG_RESIZE 500.0f

class CSNImage
{
public:
	CSNImage();
	~CSNImage();

	void SetName(CString _strpath, CString _strpname, CString _strname, unsigned long _pcode, unsigned long _nCode);
	void SetSize(unsigned short _w, unsigned short _h, float _size);
	void SetThTex(GLuint _thtex);
	void SetTexId(GLuint _texid);
	void SetTex(GLuint _tex);
	void SetPosition(POINT3D pos);
	void SetSelecttion(bool _isSel);
	void SetImgSize(unsigned short _w, unsigned long _h) { nImgWidth = _w; nImgHeight = _h; };
	void SetImgDrawAngle(float _angle) { m_fImgDrawAngle = _angle; }

	void SetColorBalanceCoff(float _coff) { m_cbCoff = _coff; }
	void SetRemoveHLCoff(int _coff) { m_minThforHL = _coff; }
//	void SetBgColor(float r, float g, float b);

	void SetBoundary(int _width, int _height);
	void SetInBoundary(int _width, int _height);

	void DrawThumbNail(float fAlpha);
	void DrawForPicking();
	void DrawImage(float fAlpha);
	void DrawBMPText();
	void DrawCroppingArea();

	bool AddMatchedPoint(_MATCHInfo info, int search_size);

	GLuint GetTexId() { return texId; };
	GLuint GetTxTex() { return thTexId; };
	CString GetPath() { return strPath; };
	unsigned short GetWidth() { return nWidth; };
	unsigned short GetHeight() { return nHeight; };
	POINT3D GetPos() { return m_pos; };
	unsigned short GetImgWidth() { return nImgWidth; };
	unsigned short GetImgHeight() { return nImgHeight; };

	unsigned long GetCode() { return nCode;  }
	CString GetName() { return strName; }

	bool IsDuplicate(POINT3D pos, int search_size);
	float GetAratio() { return fARatio; };

	float GetImgWScale() { return m_imgWScale; };
	float GetImgHScale() { return m_imgHScale; };
	POINT3D GetLeftTop() { return m_pntLT; };
	void SetCropArea(float yFaceBot, float yFaceTop, float xFaceCenter, float yFaceCenter, bool IsPreview, cv::Rect rectLeftEye, cv::Rect rectRightEye, cv::Rect rectFace);
	void SetPhotoFomat(_PHOTOID_FORMAT _format);
	void Undo();
	void SaveCrop(CString strPath, CString strName, UINT w, UINT h, int type);
	bool GetCropImgInfo(CString& path, CString& name, UINT& _w, UINT& _h, float& r);

	void ClearMatchResult();


	// Image processing //
	void SetSrcIplImage(IplImage* pimg);
	IplImage* GetSrcIplImage() { return m_pSrcImg; }
	IplImage* GetSrcCopyIplImage() { return m_pSrcImgCopy; }
	IplImage* GetPrintImg(float _fScale, bool IsSample); // { return m_pCropImg; }
	void SetCropImg(float _fScale);
	void UpdatePreview(float _fScale);

//	IplImage* GetSrcSmallIplImage() { return m_pSrcImgSmall; }
	void SetGLTexture(IplImage* pimg);
	void RotateImage(float _fAngle, int nWidth, int nHeight, bool IsRot, IplImage* pImg);
	void SetImageCenter(POINT3D _cpos) { m_vSrcImgCenter = _cpos; }
	void SetRotateionAngle(float _fangle); 
	void SetDetectScale(float _scale) { m_fImgDetectScale = _scale; }
	float GetImgAngle() { return m_fImgAngle; }
	float GetImgBrightness() { return m_fSrcBrightness; }
	float GetImgContraast() { return m_fSrcContrast; }


//	std::vector<_MATCHInfo>* GetMatchResult() { return &m_matched_pos; };
//	int GetResultSize() { return m_matched_pos.size(); }

	void RestoreGuidePos();
	bool IsCropImage() { return m_IsCropImg; }
	void SetCropImgStatus(bool isCrop) { m_IsCropImg = isCrop; }

	POINT2D convertImageToScreenSpace(POINT2D pnt, int nWidth, int nHeight, bool IsScaled);
	POINT2D convertScreenToImageSpace(POINT2D pnt, int nWidth, int nHeight);

	
	void ChangeBrightness(float _value, bool IsApply);
	void ChangeConstrast(float _value, bool IsApply);
	void ChangeRotation(IplImage* pSrc, IplImage* pDst);
	void DrawCrossMark(int length, int thickness, int _x, int _y, IplImage* pImg);


	void BlurImage(cv::Rect targetRect, cv::Size blurSize);
	void CpoyForStamp(cv::Rect targetRect, cv::Mat& stampCut);
	void StampImage(cv::Rect srcRect, cv::Rect targetRect, cv::Size blurSize);

	void ColorBalance();
	void RemoveHighlights();
	void balance_white(cv::Mat& mat);
	int median(cv::Mat channel);

	POINT2D m_guidePosOri[_LNADMARK_POS_NUM];
	POINT2D m_guidePos[_LNADMARK_POS_NUM];
	POINT2D m_vecOutBoundery[4];
	POINT2D m_vecInBoundery[4];
	RECT2D m_rectCrop;

	cv::Rect m_leftEye, m_rightEye, m_faceRect;

private:
	CString strPath;
	CString strPName;
	CString strName;

	unsigned long parentCode;
	unsigned long nCode;

	unsigned short nWidth;
	unsigned short nHeight;
	float fARatio;

	GLuint thTexId, texId;

	float	m_fRectWidth;
	POINT3D m_pos;
	POINT3D m_vertex[4];
	POINT3D m_vertexBg[4];
	POINT2D m_texcoord[4];

	POINT3D m_vBgColor;
	bool	m_bIsSelected;
	bool	m_IsCropImg;


	unsigned short nImgWidth;
	unsigned short nImgHeight;

	int m_imgRectSize;

//	std::vector<_MATCHInfo> m_matched_pos;

	float m_fXScale, m_fYScale;
	float m_imgWScale;
	float m_imgHScale;
	POINT3D m_pntLT;
	POINT3D m_pntPreview;

	IplImage *m_pSrcImg;
	IplImage *m_pSrcImgCopy;


	IplImage* m_pCropImg;
	IplImage *m_pCropImgSmall;

//	IplImage* m_imgUndo;
	CUnDoObj m_Undo;

	float m_fImgDeskewAngle, m_fSrcBrightness, m_fSrcContrast, m_fImgAngle;
	float m_fImgDrawAngle;
	POINT3D m_vSrcImgCenter;
	float m_fImgDetectScale;

	float m_matRot[6];

	IplImage* m_PrtImg;

	_PHOTOID_FORMAT m_printFormat;

	unsigned short m_undoType;
	int m_minThforHL;
	int m_minThforHL_pre;
	float m_cbCoff;
	float m_cbCoff_pre;
};

