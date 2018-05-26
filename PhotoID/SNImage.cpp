#include "stdafx.h"
#include "SNImage.h"
#include "MainFrm.h"
#include "PhotoIDView.h"

#define PHOTO_WIDTH_MM 50.0f
#define PHOTO_HEIGHT_MM 70.0f
#define PRINT_WIDTH_MM 150.0f
#define PRIMT_HEIGHT_MM 100.0f

//#define DPI300_PIXEL 11.82f



//#define PRINT_SIZEW 2400
//#define PRINT_SIZEH 1600
//#define CANADA_SIZEW 793
//#define CANADA_SIZEH 1111
//#define CANADA_RATIO 0.7143


//#define PRINT_SIZEW 1800
//#define PRINT_SIZEH 1200

#define PRINT_SIZEW 3600
#define PRINT_SIZEH 2400



CSNImage::CSNImage()
{
	parentCode = 0;
	nWidth=0;
	nHeight=0;
	fARatio=0.0f;

	thTexId = texId = 0;
	m_fRectWidth = 0.0f;

	mtSetPoint3D(&m_vBgColor, 1.0f, 1.0f, 1.0f);
	mtSetPoint3D(&m_pos, -10000, 10000, 0.0f);

	m_fXScale = 1.0f;
	m_fYScale = 1.0f;

	m_imgWScale = 1.0f;
	m_imgHScale = 1.0f;

	m_pSrcImg = NULL;
	m_pSrcImgCopy = NULL;
	m_PrtImg = NULL;

	
	m_fImgAngle = 0.0f;
	m_fSrcBrightness = 0.0f;
	m_fSrcContrast = 1.0f;
	m_imgRectSize = 0;
	m_fImgDrawAngle = 0.0f;




	m_pCropImg = NULL;
	m_pCropImgSmall = NULL;
	m_IsCropImg = false;

	
//	m_printFormat.set(L"Canada", 50, 70);


	glGenTextures(1, &thTexId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	m_undoType = 0;
}

void CSNImage::Undo()
{
	if (m_pCropImg){

		CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
		cvReleaseImage(&m_pCropImg);
//		m_pCropImg = cvCloneImage(m_imgUndo);
		IplImage* pUndo = m_Undo.GetUndoImg();

		if (pUndo != nullptr){
			m_pCropImg = cvCloneImage(pUndo);
			m_Undo.PopUndo();

			SetGLTexture(m_pCropImg);
			cvResize(m_pCropImg, m_pCropImgSmall);
			cvCvtColor(m_pCropImgSmall, m_pCropImgSmall, CV_RGB2BGR);

			pM->DisplayPreview(m_pCropImgSmall);

			if (m_Undo.GetCurId() == 0){
				pM->SetUndoButtonState(false, m_undoType);
			}
		}
	}
}


bool CSNImage::GetCropImgInfo(CString& path, CString& name, UINT& _w, UINT& _h, float& r)
{
	if (m_pCropImg){
		path = strPName;
		name = strName + L"_crop";
		_w = m_pCropImg->width;
		_h = m_pCropImg->height;
		r = (float)m_pCropImg->width / (float)m_pCropImg->height;
		return true;
	}
	return false;
}
void CSNImage::SaveCrop(CString strPath, CString strName, UINT w, UINT h, int type)
{
	if (m_pCropImg){

		//CString path, filename;
		//int nIndex = strPath.ReverseFind(_T('.'));
		//if (nIndex > 0) {
		//	filename = strPath.Left(nIndex);
		//}
		////	filename += strExtension;	
		//path = filename + L"_crop.bmp";

		CString path;
		path = strPath + L"\\" + strName;
		if (type == 0){
			path += ".jpg";
		}
		else if (type == 1){
			path += L".bmp";
		}



		USES_CONVERSION;
		char* sz = T2A(path);
		sz = T2A(path);

		IplImage *pImgTmp = cvCreateImage(cvSize(w, h), m_pCropImg->depth, m_pCropImg->nChannels);
		cvResize(m_pCropImg, pImgTmp);
		cvCvtColor(pImgTmp, pImgTmp, CV_BGR2RGB);
		

		cvSaveImage(sz, pImgTmp);
		cvReleaseImage(&pImgTmp);


		//CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
		//cvReleaseImage(&m_pCropImg);
		//m_pCropImg = cvCloneImage(m_imgUndo);
		//SetGLTexture(m_pCropImg);

		//cvResize(m_pCropImg, m_pCropImgSmall);
		//cvCvtColor(m_pCropImgSmall, m_pCropImgSmall, CV_RGB2BGR);

		//pM->DisplayPreview(m_pCropImgSmall);
		//pM->SetUndoButtonState(false, m_undoType);
	}
}


void CSNImage::SetPhotoFomat(_PHOTOID_FORMAT _format)
{
	m_printFormat.set(_format.name, _format.widthMM, _format.heightMM, _format.topMarginMM, _format.botMarginMM);
}

CSNImage::~CSNImage()
{
	if (texId > 0){
		glDeleteTextures(1, &texId);
	}
	if (thTexId > 0){
		glDeleteTextures(1, &thTexId);
	}

	if (m_pSrcImg){
		cvReleaseImage(&m_pSrcImg);
	}

	if (m_pSrcImgCopy){
		cvReleaseImage(&m_pSrcImgCopy);
	}


	if (m_pCropImg){
		cvReleaseImage(&m_pCropImg);
	}

	if (m_pCropImgSmall){
		cvReleaseImage(&m_pCropImgSmall);
	}


	
	//if (m_pSrcImgSmall){
	//	cvReleaseImage(&m_pSrcImgSmall);
	//}
}

//void CSNImage::SetBgColor(float r, float g, float b)
//{
//	//mtSetPoint3D(&m_vBgColor, r,g,b);
//}

void CSNImage::SetSrcIplImage(IplImage* pimg)
{
	if (m_pSrcImg){
		cvReleaseImage(&m_pSrcImg);
	}
	if (m_pSrcImgCopy){
		cvReleaseImage(&m_pSrcImgCopy);
	}

	if (m_PrtImg){
		cvReleaseImage(&m_PrtImg);
	}

	m_IsCropImg = false;
	if (m_pCropImg){
		cvReleaseImage(&m_pCropImg);		
	}

	if (m_pCropImgSmall){
		cvReleaseImage(&m_pCropImgSmall);
	}

	m_Undo.InitUnDoImg();

	//if (m_imgUndo){
	//	cvReleaseImage(&m_imgUndo);
	//}


	m_pSrcImg = pimg;
//	m_PrtImg = cvCreateImage(cvSize(PRINT_SIZEW, PRINT_SIZEH), m_pSrcImg->depth, m_pSrcImg->nChannels);
	//if (m_PrtImg){
	//	cvReleaseImage(&m_PrtImg);
	//	m_PrtImg = NULL;
	//}
	//m_PrtImg = cvCreateImage(cvSize(PRINT_SIZEW, PRINT_SIZEH), m_pSrcImg->depth, m_pSrcImg->nChannels);


	// TEST Code //

	// Draw Text //
	//cv::Size textsize = getTextSize("Sample Image", FONT_HERSHEY_COMPLEX | FONT_ITALIC, 3.6, 6, 0);
	//cv::Point org((m_PrtImg->width - textsize.width) / 2, (m_PrtImg->height - textsize.height) / 2);

	//cvSet(m_PrtImg, cvScalar(255, 255, 255));
	//cv::Mat imgMat = cv::cvarrToMat(m_PrtImg);
	//cv::putText(imgMat, "Sample Image", org, FONT_HERSHEY_COMPLEX | FONT_ITALIC, 3.6, cv::Scalar(200, 200, 200), 6, CV_AA);
	//cv::imshow("Sample", imgMat);






	//int w = pimg->width / 2;
	//int h = pimg->height / 2;

	//m_pSrcImgSmall = cvCreateImage(cvSize(w, h), m_pSrcImg->depth, m_pSrcImg->nChannels);
	//cvResize(m_pSrcImg, m_pSrcImgSmall);
		

	m_pSrcImgCopy = cvCloneImage(m_pSrcImg);

	// TEST //
	//ChangeBrightness(m_pSrcImgCopy, 50);
	//ChangeConstrast(m_pSrcImgCopy, 0.5f);
	
	m_fImgDeskewAngle = 0.0f;
	m_fImgAngle = 0.0f;
	m_fSrcBrightness = 0.0f;
	m_fSrcContrast = 1.0f;


	CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
	pM->ReSetSlideValues();
	//CvMat M = cvMat(2, 3, CV_32F, m_matRot);
	//int w = pimg->width;
	//int h = pimg->height;
	//float angleRadians = 0.0f;
	//m_matRot[0] = (float)(cos(angleRadians));
	//m_matRot[1] = (float)(sin(angleRadians));
	//m_matRot[3] = -m_matRot[1];
	//m_matRot[4] = m_matRot[0];
	//m_matRot[2] = w*0.5f;
	//m_matRot[5] = h*0.5f;


	SetGLTexture(m_pSrcImgCopy);
	
}

bool CSNImage::AddMatchedPoint(_MATCHInfo info, int search_size)
{
/*	if (search_size > 0){
		if (IsDuplicate(info.pos, search_size) == false){
			m_matched_pos.push_back(info);			
		}
	}
	else{
		m_matched_pos.push_back(info);
	}*/
	return true;
}

void CSNImage::SetName(CString _strpath, CString _strpname, CString _strname, unsigned long _pcode, unsigned long _nCode)
{
	strPath = _strpath;
	strPName = _strpname;
	strName = _strname;
	parentCode = _pcode;
	nCode = _nCode;

	mtSetPoint3D(&m_pos, 0.0f, 0.0f, 0.0f);

}
void CSNImage::SetSize(unsigned short _w, unsigned short _h, float _size)
{
	m_imgRectSize = _size;
	float fMargin = 0.45f;
	nWidth = _w;
	nHeight = _h;
	fARatio = (float)_w / (float)_h;
	float w, h;

	if (_size > 0){
		if (fARatio <= 1.0f){
			w = _size*fARatio*fMargin;
			h = _size*fMargin;
		}
		else{
			w = _size*fMargin;
			h = (_size / fARatio)*fMargin;
		}
	}
	else{
		w = _w*0.5f;
		h = _h*0.5f;
	}


	//m_fXScale = w*2.0f / (float)nImgWidth;
	//m_fYScale = h*2.0f / (float)nImgHeight;

	mtSetPoint3D(&m_vertex[0], -w, -h, 0.0f);		mtSetPoint2D(&m_texcoord[0], 0.0f, 1.0f);
	mtSetPoint3D(&m_vertex[1], w, -h, 0.0f);		mtSetPoint2D(&m_texcoord[1], 1.0f, 1.0f);
	mtSetPoint3D(&m_vertex[2], w, h, 0.0f);			mtSetPoint2D(&m_texcoord[2], 1.0f, 0.0f);
	mtSetPoint3D(&m_vertex[3], -w, h, 0.0f);		mtSetPoint2D(&m_texcoord[3], 0.0f, 0.0f);



	float pw = w*0.25f;
	float ph = h*0.25f;
	mtSetPoint3D(&m_vertexBg[0], -pw, -ph, 0.0f);
	mtSetPoint3D(&m_vertexBg[1], pw, -ph, 0.0f);
	mtSetPoint3D(&m_vertexBg[2], pw, ph, 0.0f);
	mtSetPoint3D(&m_vertexBg[3], -pw, ph, 0.0f);

	m_fRectWidth = _size*fARatio;

	m_imgWScale = w*2.0f / _size;
	m_imgHScale = h*2.0f / _size;


	m_pntLT.x = w;
	m_pntLT.y = h;
	m_pntLT.z = 0;

	m_pntPreview.x = w + 5;
	m_pntPreview.y = -h + ph;
	m_pntPreview.z = 0;


	//if ((m_IsCropImg == false) && (m_pCropImgSmall)){
	//	IplImage* tmpImg = cvCreateImage(cvSize(64, 64), 8, 3);
	//	cvSet(tmpImg, CV_RGB(255, 255, 255));
	//	CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
	//	pM->DisplayPreview(tmpImg);
	//	cvReleaseImage(&tmpImg);
	//}

	//m_fXScale = (float)GetImgWidth() / (GetLeftTop().x * 2);
	//m_fYScale = (float)GetImgHeight() / (GetLeftTop().y * 2);



//	MakeGuideDrawPos();
}


void CSNImage::DrawForPicking()
{
	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);

	// Background//	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(m_vertexBg[0].x, m_vertexBg[0].y, m_vertexBg[0].z);
	glVertex3f(m_vertexBg[1].x, m_vertexBg[1].y, m_vertexBg[1].z);
	glVertex3f(m_vertexBg[2].x, m_vertexBg[2].y, m_vertexBg[2].z);
	glVertex3f(m_vertexBg[3].x, m_vertexBg[3].y, m_vertexBg[3].z);
	glEnd();
	//==================//
	glPopMatrix();

}

void CSNImage::SetSelecttion(bool _isSel) 
{ 
	if (_isSel == true){
		mtSetPoint3D(&m_vBgColor, 0.1f, 0.99f, 0.1f);
	}
	else{
		mtSetPoint3D(&m_vBgColor, 1.0f, 1.0f, 1.0f);
	}
	m_bIsSelected = _isSel; 
};

void CSNImage::SetTexId(GLuint _texid)
{
	if (_texid != texId){
		if (texId > 0){
			glDeleteTextures(1, &texId);
		}
		texId = _texid;
	}	

}

void CSNImage::DrawImage(float fAlpha)
{
	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);

	glBindTexture(GL_TEXTURE_2D, texId);
	glColor4f(1.0f, 1.0f, 1.0f, fAlpha);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(m_texcoord[0].x, m_texcoord[0].y);
	glVertex3f(m_vertex[0].x, m_vertex[0].y, m_vertex[0].z);

	glTexCoord2f(m_texcoord[1].x, m_texcoord[1].y);
	glVertex3f(m_vertex[1].x, m_vertex[1].y, m_vertex[1].z);

	glTexCoord2f(m_texcoord[2].x, m_texcoord[2].y);
	glVertex3f(m_vertex[2].x, m_vertex[2].y, m_vertex[2].z);

	glTexCoord2f(m_texcoord[3].x, m_texcoord[3].y);
	glVertex3f(m_vertex[3].x, m_vertex[3].y, m_vertex[3].z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void CSNImage::DrawBMPText()
{
	glPushMatrix();	

	glBindTexture(GL_TEXTURE_2D, texId);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(m_vertex[0].x, m_vertex[0].y, m_vertex[0].z);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(m_vertex[1].x, m_vertex[1].y, m_vertex[1].z);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(m_vertex[2].x, m_vertex[2].y, m_vertex[2].z);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_vertex[3].x, m_vertex[3].y, m_vertex[3].z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void CSNImage::DrawThumbNail(float fAlpha)
{
	if (thTexId == 0){
		return;
	}
	glPushMatrix();
	
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glRotatef(-m_fImgDrawAngle, 0, 0, 1);
	//glEnable(GL_TEXTURE_2D);
	// Background//
	//glDisable(GL_TEXTURE_2D);

	//glPushMatrix();
	//glTranslatef(m_pntPreview.x, m_pntPreview.y, m_pntPreview.z);

	//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//glBegin(GL_QUADS);	
	//glVertex3f(m_vertexBg[0].x, m_vertexBg[0].y, m_vertexBg[0].z);
	//glVertex3f(m_vertexBg[1].x, m_vertexBg[1].y, m_vertexBg[1].z);
	//glVertex3f(m_vertexBg[2].x, m_vertexBg[2].y, m_vertexBg[2].z);
	//glVertex3f(m_vertexBg[3].x, m_vertexBg[3].y, m_vertexBg[3].z);
	//glEnd();
	//glPopMatrix();
	//==================//


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, thTexId);

	glColor4f(1.0f, 1.0f, 1.0f, fAlpha);	

	glBegin(GL_QUADS);

	glTexCoord2f(m_texcoord[0].x, m_texcoord[0].y);
	glVertex3f(m_vertex[0].x, m_vertex[0].y, m_vertex[0].z);

	glTexCoord2f(m_texcoord[1].x, m_texcoord[1].y);
	glVertex3f(m_vertex[1].x, m_vertex[1].y, m_vertex[1].z);

	glTexCoord2f(m_texcoord[2].x, m_texcoord[2].y);
	glVertex3f(m_vertex[2].x, m_vertex[2].y, m_vertex[2].z);

	glTexCoord2f(m_texcoord[3].x, m_texcoord[3].y);
	glVertex3f(m_vertex[3].x, m_vertex[3].y, m_vertex[3].z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	

	// Detected //
	//if (m_matched_pos.size() > 0 ){
	//	glColor4f(1.0f, 0.2f, 0.1f, fAlpha);
		//glBegin(GL_LINE_STRIP);		
		//glVertex3f(m_vertex[0].x, m_vertex[0].y, m_vertex[0].z);	
		//glVertex3f(m_vertex[1].x, m_vertex[1].y, m_vertex[1].z);	
		//glVertex3f(m_vertex[2].x, m_vertex[2].y, m_vertex[2].z);	
		//glVertex3f(m_vertex[3].x, m_vertex[3].y, m_vertex[3].z);
		//glVertex3f(m_vertex[0].x, m_vertex[0].y, m_vertex[0].z);
		//glEnd();

	//	// Draw detected position //
	//	glColor4f(1.0f, 0.2f, 0.1f, 0.7f);
	//	glPushMatrix();		
	//		glScalef(m_fXScale, m_fYScale, 1.0f);
	//		glTranslatef(-nImgWidth*0.5f, -nImgHeight*0.5f, 0.0f);
	//		glBegin(GL_POINTS);
	//		for (int i = 0; i < m_matched_pos.size(); i++){
	//			glColor4f(m_matched_pos[i].color.x, m_matched_pos[i].color.y, m_matched_pos[i].color.z, 0.7f);
	//			glVertex3f(m_matched_pos[i].pos.x, nImgHeight - m_matched_pos[i].pos.y, 0.0f);
	//		}
	//		glEnd();
	//	glPopMatrix();

	//}


	//glColor3f(0.0f, 1.0f, 0.0f);
	//glBegin(GL_LINES);
	//glVertex3f(-m_drawWidth*0.5f, m_guidePosDraw[_CHIN].y, 0.0f);
	//glVertex3f(m_drawWidth*0.5f, m_guidePosDraw[_CHIN].y, 0.0f);

	//glVertex3f(-m_drawWidth*0.5f, m_guidePos[_EYE_CENTER].y, 0.0f);
	//glVertex3f(m_drawWidth*0.5f, m_guidePos[_EYE_CENTER].y, 0.0f);

	//glVertex3f(-m_drawWidth*0.5f, m_guidePos[_TOPHEAD].y, 0.0f);
	//glVertex3f(m_drawWidth*0.5f, m_guidePos[_TOPHEAD].y, 0.0f);

	//glEnd();
	//		
	glPopMatrix();




}


void CSNImage::SetThTex(GLuint _thtex)
{
	thTexId = _thtex;
}
void CSNImage::SetTex(GLuint _tex)
{
	texId = _tex;
}

void CSNImage::SetPosition(POINT3D pos)
{
	m_pos = pos;
}

void CSNImage::ClearMatchResult()
{
//	m_matched_pos.clear();
}

bool CSNImage::IsDuplicate(POINT3D pos, int search_size)
{
/*	bool IsDup = false;
	for (int i = 0; i < m_matched_pos.size(); i++){
		float fDist = mtDistance(pos, m_matched_pos[i].pos);
		if (fDist < search_size){
			IsDup = true;
			break;
		}
	}
	return IsDup*/
	return false;
}

void CSNImage::SetGLTexture(IplImage* pimg)
{
	SetImgSize(pimg->width, pimg->height);
	SetSize(pimg->width, pimg->height, m_imgRectSize);


	//if (thTexId > 0){
	////	glDeleteTextures(1, &thTexId);
	//	glGenTextures(1, &thTexId);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//}
	//glGenTextures(1, &thTexId);

	// glupload Image - Thumnail image=======================================================//
	glBindTexture(GL_TEXTURE_2D, thTexId);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY, pimg->width, pimg->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pimg->imageData);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pimg->width, pimg->height, GL_RGB, GL_UNSIGNED_BYTE, pimg->imageData);
	//======================================================================================//
}

void CSNImage::RotateImage(float _fAngle, int nWidth, int nHeight, bool IsRot, IplImage* pImg)
{
	if (pImg){
		m_fImgDrawAngle += _fAngle;		
		TRACE(L"%3.2f - %3.2f\n", m_fImgDrawAngle, _fAngle);

		if (IsRot){			
		//	m_fImgAngle = fAngleDiff;
			m_fImgAngle += m_fImgDrawAngle;
			m_fImgDrawAngle = 0.0f;

			//float m[6];// , mat[9];
			CvMat M = cvMat(2, 3, CV_32F, m_matRot);
			int w = pImg->width;
			int h = pImg->height;
		//	float angleRadians = fAngleDiff * ((float)CV_PI / 180.0f);
			float angleRadians = m_fImgAngle * ((float)CV_PI / 180.0f);
			m_matRot[0] = (float)(cos(angleRadians));
			m_matRot[1] = (float)(sin(angleRadians));
			m_matRot[3] = -m_matRot[1];
			m_matRot[4] = m_matRot[0];
			m_matRot[2] = w*0.5f;
			m_matRot[5] = h*0.5f;

			// Make a spare image for the result
			CvSize sizeRotated;
			sizeRotated.width = cvRound(w);
			sizeRotated.height = cvRound(h);


			// Rotate
			IplImage *imageRotated = cvCreateImage(sizeRotated, pImg->depth, pImg->nChannels);
			// Transform the image
			//	cvGetQuadrangleSubPix(m_pSrcImg, imageRotated, &M);
			cvGetQuadrangleSubPix(pImg, imageRotated, &M);

			cvReleaseImage(&m_pSrcImgCopy);
			m_pSrcImgCopy = imageRotated;
			SetGLTexture(m_pSrcImgCopy);

			m_fImgDrawAngle = 0.0f;
		}
	}
}

void CSNImage::RestoreGuidePos()
{
	for (int i = 0; i < _LNADMARK_POS_NUM; i++){
		m_guidePosOri[i] = m_guidePos[i];
	}
}

void CSNImage::ChangeRotation(IplImage* pSrc, IplImage* pDst)
{
//	if ((m_fImgAngle > 0.01f) || (m_fImgAngle < 0.01f)){


		CvMat M = cvMat(2, 3, CV_32F, m_matRot);
		int w = pSrc->width;
		int h = pSrc->height;

		// Make a spare image for the result
		CvSize sizeRotated;
		sizeRotated.width = cvRound(w);
		sizeRotated.height = cvRound(h);

		// Rotate
		IplImage *imageRotated = cvCreateImage(sizeRotated, pSrc->depth, pSrc->nChannels);

		// Transform the image
		cvGetQuadrangleSubPix(pSrc, imageRotated, &M);

		cvReleaseImage(&pDst);
		pDst = imageRotated;
//	}
}


POINT2D CSNImage::convertScreenToImageSpace(POINT2D pnt, int nWidth, int nHeight)
{
	POINT2D curPos;


		//m_fXScale = (float)GetImgWidth() / m_imgRectSize;
	//m_fYScale = (float)GetImgHeight() / m_imgRectSize;

	m_fXScale = (float)GetImgWidth() / (GetLeftTop().x * 2);
	m_fYScale = (float)GetImgHeight() / (GetLeftTop().y * 2);


	float xOffset = nWidth - (GetLeftTop().x + nWidth*0.5f);
	float yOffset = nHeight - (GetLeftTop().y + nHeight*0.5f);

	curPos.x = ((pnt.x - xOffset)*m_fXScale);
	curPos.y = ((pnt.y - yOffset)*m_fYScale);

	return curPos;
}

POINT2D CSNImage::convertImageToScreenSpace(POINT2D pnt, int nWidth, int nHeight, bool IsScaled)
{
	POINT2D curPos;

	if (IsScaled){
		// restore original size //
		pnt.x = pnt.x*m_fImgDetectScale;
		pnt.y = pnt.y*m_fImgDetectScale;

		//=========================//
	}
	pnt.y = GetImgHeight() - pnt.y;

	m_fXScale = (float)GetImgWidth() / (GetLeftTop().x * 2);
	m_fYScale = (float)GetImgHeight() / (GetLeftTop().y * 2);



	float xOffset = nWidth - (GetLeftTop().x + nWidth*0.5f);
	float yOffset = nHeight - (GetLeftTop().y + nHeight*0.5f);

	curPos.x = pnt.x / m_fXScale + xOffset;
	curPos.y = pnt.y / m_fYScale + yOffset;

	return curPos;
}


void CSNImage::SetRotateionAngle(float _fangle)
{ 
//	m_fImgDeskewAngle += _fangle; 
//	m_fImgAngle += m_fImgDeskewAngle;
//	m_fImgDeskewAngle = _fangle;
	m_fImgDrawAngle = _fangle;
}

void CSNImage::ChangeBrightness(float _value, bool IsApply)
{
	CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
	if (m_pCropImg == NULL){
		AfxMessageBox(L"Image is not cropped");
	}
	else{
		if (IsApply){

			//if (m_imgUndo){
			//	cvReleaseImage(&m_imgUndo);
			//}
			// For Undo=============== //
			//m_imgUndo = cvCloneImage(m_pCropImg);		
			//m_Undo.PushUndo(m_pCropImg);
			//m_undoType = 0;
			//pM->SetUndoButtonState(true, m_undoType);
			//========================//

			CvScalar brVal = cvScalarAll(_value);
			cvAddS(m_pCropImg, brVal, m_pCropImg, NULL);
		//	cvAddS(m_pCropImgOri, brVal, m_pCropImg, NULL);

			m_fSrcBrightness = 0;
			SetGLTexture(m_pCropImg);

			// Update small crop image
			cvResize(m_pCropImg, m_pCropImgSmall);
			cvCvtColor(m_pCropImgSmall, m_pCropImgSmall, CV_RGB2BGR);
			TRACE(L"Apply Brightness\n");
		}
		else{
		//	IplImage* pTmp = cvCloneImage(m_pCropImgSmall);
			IplImage* pTmp = cvCreateImage(cvSize(m_pCropImgSmall->width, m_pCropImgSmall->height), m_pCropImgSmall->depth, m_pCropImgSmall->nChannels);
			CvScalar brVal = cvScalarAll(_value);
			cvAddS(m_pCropImgSmall, brVal, pTmp, NULL);

			m_fSrcBrightness = _value;
	//		SetGLTexture(pTmp);
	//		cvCvtColor(pTmp, pTmp, CV_BGR2RGB);

			
			pM->DisplayPreview(pTmp);
			cvReleaseImage(&pTmp);

			TRACE(L"Not Apply Brightness\n");
		}
	}
}

void CSNImage::ChangeConstrast(float _value, bool IsApply)
{
	CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
	if (m_pCropImg == NULL){
		AfxMessageBox(L"Image is not cropped");
	}
	else{
		if (IsApply){

			//if (m_imgUndo){
			//	cvReleaseImage(&m_imgUndo);
			//}
			//// For Undo=============== //
			//m_imgUndo = cvCloneImage(m_pCropImg);	
			//m_undoType = 1;
			//pM->SetUndoButtonState(true, m_undoType);
			//========================//


			IplImage *pTempImg = cvCreateImage(cvGetSize(m_pCropImg), IPL_DEPTH_8U, m_pCropImg->nChannels);
			cvSet(pTempImg, cvScalarAll(1), NULL);
			cvMul(m_pCropImg, pTempImg, m_pCropImg, _value);
			cvReleaseImage(&pTempImg);

			SetGLTexture(m_pCropImg);
			m_fSrcContrast = 0;

			// Update small crop image
			cvResize(m_pCropImg, m_pCropImgSmall);
			cvCvtColor(m_pCropImgSmall, m_pCropImgSmall, CV_RGB2BGR);
		}
		else{

			IplImage* pTmp = cvCreateImage(cvSize(m_pCropImgSmall->width, m_pCropImgSmall->height), m_pCropImgSmall->depth, m_pCropImgSmall->nChannels);
	//		IplImage* pTmp = cvCloneImage(m_pCropImgSmall);
			IplImage *pTempImg = cvCreateImage(cvGetSize(m_pCropImgSmall), IPL_DEPTH_8U, m_pCropImgSmall->nChannels);
			cvSet(pTempImg, cvScalarAll(1), NULL);
			cvMul(m_pCropImgSmall, pTempImg, pTmp, _value);
			cvReleaseImage(&pTempImg);

			//SetGLTexture(pTmp);
			pM->DisplayPreview(pTmp);
			m_fSrcContrast = _value;
			cvReleaseImage(&pTmp);
		}
	}
}

void CSNImage::SetBoundary(int _width, int _height)
{
	mtSetPoint2D(&m_vecOutBoundery[0], 0, 0);
	mtSetPoint2D(&m_vecOutBoundery[1], _width, 0);
	mtSetPoint2D(&m_vecOutBoundery[2], _width, _height);
	mtSetPoint2D(&m_vecOutBoundery[3], 0, _height);

	SetInBoundary(_width, _height);
}

void CSNImage::SetInBoundary(int _width, int _height)
{
	int xOffst = 50;
	int yOffset = 50;
	int w = _width*0.5f;
	int h = _height*0.5f;

	mtSetPoint2D(&m_vecInBoundery[0], xOffst, yOffset);
	mtSetPoint2D(&m_vecInBoundery[1], w, yOffset);
	mtSetPoint2D(&m_vecInBoundery[2], w, h);
	mtSetPoint2D(&m_vecInBoundery[3], xOffst, h);

}

void CSNImage::DrawCroppingArea()
{
	//glEnable(GL_DEPTH_TEST);
	//glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	//glBegin(GL_QUADS);
	//// inner //
	//glVertex3f(m_vecInBounderyDraw[3].x, m_vecInBounderyDraw[3].y, 1.0f);
	//glVertex3f(m_vecInBounderyDraw[2].x, m_vecInBounderyDraw[2].y, 1.0f);
	//glVertex3f(m_vecInBounderyDraw[1].x, m_vecInBounderyDraw[1].y, 1.0f);
	//glVertex3f(m_vecInBounderyDraw[0].x, m_vecInBounderyDraw[0].y, 1.0f);
	//glEnd();


	//glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
	//glBegin(GL_QUADS);
	////// outter //
	//glVertex3f(m_vecOutBounderyDraw[3].x, m_vecOutBounderyDraw[3].y, 0.5f);
	//glVertex3f(m_vecOutBounderyDraw[2].x, m_vecOutBounderyDraw[2].y, 0.5f);
	//glVertex3f(m_vecOutBounderyDraw[1].x, m_vecOutBounderyDraw[1].y, 0.5f);
	//glVertex3f(m_vecOutBounderyDraw[0].x, m_vecOutBounderyDraw[0].y, 0.5f);	

	//glEnd();

	//glDisable(GL_DEPTH_TEST);

}

void CSNImage::SetCropArea(float yFaceBot, float yFaceTop, float xFaceCenter, float yFaceCenter, bool IsPreview)
{	

	float faceLength = (yFaceTop - yFaceBot);
	float faceeLengthMM = m_printFormat.heightMM - (m_printFormat.botMarginMM + m_printFormat.topMarginMM);
	float topMarginLength = m_printFormat.topMarginMM*faceLength / faceeLengthMM;
	float botMarginLength = m_printFormat.botMarginMM*faceLength / faceeLengthMM;


	float cropHeight = faceLength + topMarginLength + botMarginLength;  // face length X 2
	float cropWidth = cropHeight*m_printFormat.aRatio;

	m_rectCrop.set(xFaceCenter - cropWidth*0.5f, xFaceCenter + cropWidth*0.5f,
		yFaceBot - topMarginLength, yFaceTop + botMarginLength);

	m_rectCrop.width = m_rectCrop.x2 - m_rectCrop.x1;
	m_rectCrop.height =  m_rectCrop.y2 - m_rectCrop.y1;

	mtSetPoint2D(&m_vecInBoundery[0], m_rectCrop.x1, m_rectCrop.y1);
	mtSetPoint2D(&m_vecInBoundery[1], m_rectCrop.x1, m_rectCrop.y2);
	mtSetPoint2D(&m_vecInBoundery[2], m_rectCrop.x2, m_rectCrop.y2);
	mtSetPoint2D(&m_vecInBoundery[3], m_rectCrop.x2, m_rectCrop.y1);


	
	if (IsPreview){
		float fScale = pView->GetDetectedScale();
		UpdatePreview(fScale);
	}
	
	//if (m_IsCropImg){
	//	float fScale = pView->GetDetectedScale();
	//	SetCropImg(fScale);
	//}




	// need to know the ratio of face length / image height
	//float cropHeight = (yFaceTop - yFaceBot) * 1.5;  // face length X 2
	//float cropWidth = cropHeight*m_printFormat.aRatio;
	//


	//m_rectCrop.set(xFaceCenter - cropWidth*0.5f, xFaceCenter + cropWidth*0.5f,
	//	yFaceCenter - cropHeight*0.5f, yFaceCenter + cropHeight*0.5f);
	//m_rectCrop.width = m_rectCrop.x2 - m_rectCrop.x1;
	//m_rectCrop.height =  m_rectCrop.y2 - m_rectCrop.y1;



	//mtSetPoint2D(&m_vecInBoundery[0], m_rectCrop.x1, m_rectCrop.y1);
	//mtSetPoint2D(&m_vecInBoundery[1], m_rectCrop.x1, m_rectCrop.y2);
	//mtSetPoint2D(&m_vecInBoundery[2], m_rectCrop.x2, m_rectCrop.y2);
	//mtSetPoint2D(&m_vecInBoundery[3], m_rectCrop.x2, m_rectCrop.y1);


}

IplImage* CSNImage::GetPrintImg(float _fScale, bool IsSample)
{
	if (m_pCropImg==NULL){
		//cvReleaseImage(&m_pCropImg);
		//m_pCropImg = NULL;
		return NULL;
	}

//	m_rectCrop.x1 *= _fScale;
//	m_rectCrop.x2 *= _fScale;
//	m_rectCrop.y1 *= _fScale;
//	m_rectCrop.y2 *= _fScale;
//
//	if (m_rectCrop.y1 < 0)					m_rectCrop.y1 = 0;
//	if (m_rectCrop.y2 > m_pSrcImg->height)	m_rectCrop.y2 = m_pSrcImg->height;
//	if (m_rectCrop.x1 < 0)					m_rectCrop.x1 = 0;
//	if (m_rectCrop.x2 > m_pSrcImg->width)	m_rectCrop.x2 = m_pSrcImg->width;
//
//
//	m_rectCrop.width = m_rectCrop.x2 - m_rectCrop.x1;	
//	m_rectCrop.height = m_rectCrop.y2 - m_rectCrop.y1;
//
//	m_pCropImg = cvCreateImage(cvSize(CANADA_SIZEW, CANADA_SIZEH), m_pSrcImg->depth, m_pSrcImg->nChannels);
//	cvSetImageROI(m_pSrcImg, cvRect(m_rectCrop.x1, m_rectCrop.y1, m_rectCrop.width, m_rectCrop.height));		// posx, posy = left - top
////	cvCopy(m_pSrcImg, m_pCropImg);
//	cvResize(m_pSrcImg, m_pCropImg);
//	cvResetImageROI(m_pSrcImg);


	
	if (m_PrtImg){
		cvReleaseImage(&m_PrtImg);
		m_PrtImg = NULL;
	}
	m_PrtImg = cvCreateImage(cvSize(PRINT_SIZEW, PRINT_SIZEH), m_pSrcImg->depth, m_pSrcImg->nChannels);	
	cvSet(m_PrtImg, cvScalar(255, 255, 255));

//#define PRINT_SIZEW 1800
//#define PRINT_SIZEH 1200
//#define CANADA_SIZEW 590
//#define CANADA_SIZEH 826

	//int wMargin = (PRINT_SIZEW - CANADA_SIZEW * 2) / 4;
	//int hMargin = (PRINT_SIZEH - CANADA_SIZEW ) / 2;

	int wMargin = (PRINT_SIZEW - m_printFormat.photoSizeW * 2) / 4;
	int hMargin = (PRINT_SIZEH - m_printFormat.photoSizeH) / 2;


	cvSetImageROI(m_PrtImg, cvRect(wMargin, hMargin, m_pCropImg->width, m_pCropImg->height));		// posx, posy = left - top
	cvCopy(m_pCropImg, m_PrtImg);
	cvResetImageROI(m_PrtImg);

	//DrawCrossMark(50, 1, wMargin - 1, hMargin-1, m_PrtImg);
	//DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 1, hMargin - 1, m_PrtImg);
	//DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 1, hMargin + m_pCropImg->height + 1, m_PrtImg);
	//DrawCrossMark(50, 1, wMargin - 1, hMargin + m_pCropImg->height + 1, m_PrtImg);
	DrawCrossMark(50, 1, wMargin - 2, hMargin - 2, m_PrtImg);
	DrawCrossMark(50, 1, wMargin - 3, hMargin - 3, m_PrtImg);

	DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 2, hMargin - 2, m_PrtImg);
	DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 3, hMargin - 3, m_PrtImg);

	DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 2, hMargin + m_pCropImg->height + 2, m_PrtImg);
	DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 3, hMargin + m_pCropImg->height + 3, m_PrtImg);

	DrawCrossMark(50, 1, wMargin - 2, hMargin + m_pCropImg->height + 2, m_PrtImg);
	DrawCrossMark(50, 1, wMargin - 3, hMargin + m_pCropImg->height + 3, m_PrtImg);


	wMargin += (wMargin * 2 + m_pCropImg->width);
	cvSetImageROI(m_PrtImg, cvRect(wMargin, hMargin, m_pCropImg->width, m_pCropImg->height));		// posx, posy = left - top
	cvCopy(m_pCropImg, m_PrtImg);
	cvResetImageROI(m_PrtImg);

	DrawCrossMark(50, 1, wMargin - 2, hMargin - 2, m_PrtImg);
	DrawCrossMark(50, 1, wMargin - 3, hMargin - 3, m_PrtImg);

	DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 2, hMargin - 2, m_PrtImg);
	DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 3, hMargin - 3, m_PrtImg);

	DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 2, hMargin + m_pCropImg->height + 2, m_PrtImg);
	DrawCrossMark(50, 1, wMargin + m_pCropImg->width + 3, hMargin + m_pCropImg->height + 3, m_PrtImg);

	DrawCrossMark(50, 1, wMargin - 2, hMargin + m_pCropImg->height + 2, m_PrtImg);
	DrawCrossMark(50, 1, wMargin - 3, hMargin + m_pCropImg->height + 3, m_PrtImg);

//IplImage* rotated = cvCreateImage(cvSize(m_PrtImg->height, m_PrtImg->width), m_PrtImg->depth, m_PrtImg->nChannels);
//cvTranspose(m_PrtImg, rotated);

//	cvShowImage("Crop img", m_PrtImg);


	if (IsSample){
		// Add water mark //

		cv::Size textsize = getTextSize("Sample Image", FONT_HERSHEY_COMPLEX | FONT_ITALIC, 3.6, 6, 0);
		cv::Point org((m_PrtImg->width - textsize.width) / 2, (m_PrtImg->height - textsize.height) / 2);		
		cv::Mat imgMat(m_PrtImg->height, m_PrtImg->width, CV_MAKETYPE(CV_8U, m_PrtImg->nChannels), cv::Scalar(255, 255, 255));
		cv::putText(imgMat, "Sample Image", org, FONT_HERSHEY_COMPLEX | FONT_ITALIC, 3.6, cv::Scalar(200, 200, 200), 6, CV_AA);
		cv::Mat prtImg = cv::cvarrToMat(m_PrtImg);

		imgMat.convertTo(imgMat, CV_32FC3, 1.0/255);
		prtImg.convertTo(prtImg, CV_32FC3);
		cv::multiply(prtImg, imgMat, prtImg);
		prtImg.convertTo(prtImg, CV_8UC3);
		
//		IplImage* image = cvCreateImage(cvSize(mat.cols, mat.rows), 8, 3);
		IplImage ipltemp = prtImg;
		cvCopy(&ipltemp, m_PrtImg);
	//	cvShowImage("Sample", m_PrtImg);
	}


	// Resize for 334 by 600 DPI //
	int dpiWidth = (float)m_PrtImg->width*0.56f;
	IplImage* pPrtDPI = cvCreateImage(cvSize(dpiWidth, m_PrtImg->height), m_PrtImg->depth, m_PrtImg->nChannels);
	cvResize(m_PrtImg, pPrtDPI);

//	return m_PrtImg;
	return pPrtDPI;

}

void CSNImage::DrawCrossMark(int length, int thickness, int _x, int _y, IplImage* pImg)
{
	//	int id = y*matBinary->widthStep + x;
	//	matBinary->imageData[id]
	// x direction //
	for (int x = (_x - length); x < (_x + length); x++){
		int id = _y*pImg->widthStep + x * 3;
		pImg->imageData[id] = 0;
		pImg->imageData[id + 1] = 0;
		pImg->imageData[id + 2] = 0;
	}


	for (int y = (_y - length); y < (_y + length); y++){
		int id = y*pImg->widthStep + _x * 3;
		pImg->imageData[id] = 0;
		pImg->imageData[id + 1] = 0;
		pImg->imageData[id + 2] = 0;
	}
}

void CSNImage::UpdatePreview(float _fScale)
{
	CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();

	//	if (m_IsCropImg == true) return;
	if (m_pSrcImg == NULL) return;

	if (m_pCropImgSmall){
		cvReleaseImage(&m_pCropImgSmall);
		m_pCropImgSmall = NULL;
	}

	m_rectCrop.x1 *= _fScale;
	m_rectCrop.x2 *= _fScale;
	m_rectCrop.y1 *= _fScale;
	m_rectCrop.y2 *= _fScale;

	if (m_rectCrop.y1 < 0)					m_rectCrop.y1 = 0;
	if (m_rectCrop.y2 > m_pSrcImgCopy->height)	m_rectCrop.y2 = m_pSrcImgCopy->height;
	if (m_rectCrop.x1 < 0)					m_rectCrop.x1 = 0;
	if (m_rectCrop.x2 > m_pSrcImgCopy->width)	m_rectCrop.x2 = m_pSrcImgCopy->width;


	m_rectCrop.width = m_rectCrop.x2 - m_rectCrop.x1;
	m_rectCrop.height = m_rectCrop.y2 - m_rectCrop.y1;

	float fRatio = m_printFormat.aRatio;
	int sHeight = 200;
	int sWidth = sHeight * fRatio;

	m_pCropImgSmall = cvCreateImage(cvSize(sWidth, sHeight), m_pSrcImgCopy->depth, m_pSrcImgCopy->nChannels);
	cvSetImageROI(m_pSrcImgCopy, cvRect(m_rectCrop.x1, m_rectCrop.y1, m_rectCrop.width, m_rectCrop.height));		// posx, posy = left - top
	cvResize(m_pSrcImgCopy, m_pCropImgSmall);
	cvResetImageROI(m_pSrcImgCopy);

	cvCvtColor(m_pCropImgSmall, m_pCropImgSmall, CV_RGB2BGR);
	pM->DisplayPreview((void*)m_pCropImgSmall);
}


void CSNImage::SetCropImg(float _fScale)
{
	CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
	
	if (m_IsCropImg == true) return;

	if (m_pSrcImg == NULL) return;

	if (m_pCropImg){
		cvReleaseImage(&m_pCropImg);
		m_pCropImg = NULL;

		cvReleaseImage(&m_pCropImgSmall);
		m_pCropImgSmall = NULL;
	}

	m_rectCrop.x1 *= _fScale;
	m_rectCrop.x2 *= _fScale;
	m_rectCrop.y1 *= _fScale;
	m_rectCrop.y2 *= _fScale;

	if (m_rectCrop.y1 < 0)					m_rectCrop.y1 = 0;
	if (m_rectCrop.y2 > m_pSrcImgCopy->height)	m_rectCrop.y2 = m_pSrcImgCopy->height;
	if (m_rectCrop.x1 < 0)					m_rectCrop.x1 = 0;
	if (m_rectCrop.x2 > m_pSrcImgCopy->width)	m_rectCrop.x2 = m_pSrcImgCopy->width;


	m_rectCrop.width = m_rectCrop.x2 - m_rectCrop.x1;
	m_rectCrop.height = m_rectCrop.y2 - m_rectCrop.y1;

	float fRatio = m_printFormat.aRatio;
	int sHeight = 200;
	int sWidth = sHeight * fRatio;
//	m_pCropImg = cvCreateImage(cvSize(CANADA_SIZEW, CANADA_SIZEH), m_pSrcImgCopy->depth, m_pSrcImgCopy->nChannels);
	m_pCropImg = cvCreateImage(cvSize(m_printFormat.photoSizeW, m_printFormat.photoSizeH), m_pSrcImgCopy->depth, m_pSrcImgCopy->nChannels);
	m_pCropImgSmall = cvCreateImage(cvSize(sWidth, sHeight), m_pSrcImgCopy->depth, m_pSrcImgCopy->nChannels);
	cvSetImageROI(m_pSrcImgCopy, cvRect(m_rectCrop.x1, m_rectCrop.y1, m_rectCrop.width, m_rectCrop.height));		// posx, posy = left - top
	//	cvCopy(m_pSrcImg, m_pCropImg);
	cvResize(m_pSrcImgCopy, m_pCropImg);
	cvResize(m_pSrcImgCopy, m_pCropImgSmall);
	cvResetImageROI(m_pSrcImgCopy);


	

	// Set Crop image as main image
	SetGLTexture(m_pCropImg);
//	SetGLTexture(m_pCropImgOri);
	m_IsCropImg = true;


	nImgWidth = m_pCropImg->width;
	nImgHeight = m_pCropImg->height;
	
	cvCvtColor(m_pCropImgSmall, m_pCropImgSmall, CV_RGB2BGR);
	pM->DisplayPreview((void*)m_pCropImgSmall);
}


void CSNImage::CpoyForStamp(cv::Rect targetRect, cv::Mat& stampCut)
{
	if (m_IsCropImg == true){
		cv::Rect cutRect = targetRect;
		int margin = 10;
		cutRect.x -= margin;
		cutRect.y -= margin;
		cutRect.width += margin * 2;
		cutRect.height += margin * 2;

		if ((cutRect.x < 0) || (cutRect.y < 0) ||
			((cutRect.x + cutRect.width) >= nWidth) || ((cutRect.y + cutRect.height) >= nHeight)){
			return;
		}

		cv::Mat imgMat = cv::cvarrToMat(m_pCropImg);
		imgMat(cutRect).copyTo(stampCut);
	}
}


void CSNImage::BlurImage(cv::Rect targetRect, cv::Size blurSize)
{
	if (m_IsCropImg == true){

		//if (m_imgUndo){
		//	cvReleaseImage(&m_imgUndo);
		//}
		// For Undo=============== //
		//m_imgUndo = cvCloneImage(m_pCropImg);
		m_Undo.PushUndo(m_pCropImg);
		CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
		pM->SetUndoButtonState(true, 3);
		//========================//
		
		cv::Mat imgMat = cv::cvarrToMat(m_pCropImg);	

		

		int orignaltype = imgMat.type();

		cv::Rect cutRect = targetRect;
		int margin = 10;
		cutRect.x -= margin;
		cutRect.y -= margin;
		cutRect.width += margin * 2;
		cutRect.height += margin * 2;


		if ((cutRect.x < 0) || (cutRect.y < 0) ||
			((cutRect.x + cutRect.width) >= nWidth) || ((cutRect.y + cutRect.height) >= nHeight)){
			return;
		}


		cv::Mat mask = cv::Mat::zeros(cv::Size(cutRect.width, cutRect.height), imgMat.type());
		// mask is a disk	
		int radious = targetRect.width / 2;
		circle(mask, cv::Point(cutRect.width / 2, cutRect.height/2), radious, cv::Scalar(100, 100, 100), -1);
		cv::Mat imgBlur, imgOri;
		imgMat(cutRect).copyTo(imgBlur);
		imgMat(cutRect).copyTo(imgOri);

	//	imgBlur.setTo(255);

	//	cv::Size blurSize(7, 7);
		blur(mask, mask, blurSize);
		blur(imgBlur, imgBlur, blurSize);

		//cv::imshow("blur1", mask);
		//cv::imshow("blur2", imgBlur);

		mask.convertTo(mask, CV_32FC3, 1.0 / 255.0f); // 
		imgBlur.convertTo(imgBlur, CV_32FC3);
		imgOri.convertTo(imgOri, CV_32FC3);

		multiply(mask, imgBlur, imgBlur);
		multiply(cv::Scalar::all(1.0) - mask, imgOri, imgOri);

		cv::Mat ouImage = cv::Mat::zeros(imgOri.size(), imgOri.type());
		add(imgBlur, imgOri, ouImage);

		ouImage.convertTo(ouImage, orignaltype);
		ouImage.copyTo(imgMat(cutRect));

//		cv::imshow("Result", imgMat);

		//cvReleaseImage(&pimg);
		//pimg = new IplImage(ouImage);
//		cvShowImage("cropimg", m_pCropImg);

		SetGLTexture(m_pCropImg);
	}
}





void CSNImage::balance_white(cv::Mat& mat) 
{
	double discard_ratio = 0.05;
	int hists[3][256];
	memset(hists, 0, 3 * 256 * sizeof(int));

	for (int y = 0; y < mat.rows; ++y) {
		uchar* ptr = mat.ptr<uchar>(y);
		for (int x = 0; x < mat.cols; ++x) {
			for (int j = 0; j < 3; ++j) {
				hists[j][ptr[x * 3 + j]] += 1;
			}
		}
	}

	// cumulative hist
	int total = mat.cols*mat.rows;
	int vmin[3], vmax[3];
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 255; ++j) {
			hists[i][j + 1] += hists[i][j];
		}
		vmin[i] = 0;
		vmax[i] = 255;
		while (hists[i][vmin[i]] < discard_ratio * total)
			vmin[i] += 1;
		while (hists[i][vmax[i]] > (1 - discard_ratio) * total)
			vmax[i] -= 1;
		if (vmax[i] < 255 - 1)
			vmax[i] += 1;
	}


	for (int y = 0; y < mat.rows; ++y) {
		uchar* ptr = mat.ptr<uchar>(y);
		for (int x = 0; x < mat.cols; ++x) {
			for (int j = 0; j < 3; ++j) {
				int val = ptr[x * 3 + j];
				if (val < vmin[j])
					val = vmin[j];
				if (val > vmax[j])
					val = vmax[j];
				ptr[x * 3 + j] = static_cast<uchar>((val - vmin[j]) * 255.0 / (vmax[j] - vmin[j]));
			}
		}
	}
}


void CSNImage::ColorBalance()
{
	if (m_IsCropImg == true){
		cv::Mat in = cv::cvarrToMat(m_pCropImg);
		balance_white(in);

	//	cv::imshow("before", in);
		//float half_percent = 1.0f / 200.0f;
		//vector<Mat> tmpsplit; split(in, tmpsplit);
		//for (int i = 0; i<3; i++) {
		//	//find the low and high precentile values (based on the input percentile)
		//	Mat flat; tmpsplit[i].reshape(1, 1).copyTo(flat);
		//	cv::sort(flat, flat, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
		//	int lowval = flat.at<uchar>(cvFloor(((float)flat.cols) * half_percent));
		//	int highval = flat.at<uchar>(cvCeil(((float)flat.cols) * (1.0 - half_percent)));
		////	cout << lowval << " " << highval << endl;

		//	//saturate below the low percentile and above the high percentile
		//	tmpsplit[i].setTo(lowval, tmpsplit[i] < lowval);
		//	tmpsplit[i].setTo(highval, tmpsplit[i] > highval);

		//	//scale the channel
		//	normalize(tmpsplit[i], tmpsplit[i], 0, 255, NORM_MINMAX);
		//}
		//merge(tmpsplit, in);
	//	cv::imshow("after", in);
		SetGLTexture(m_pCropImg);
	}
}


void CSNImage::RemoveHighlights()
{
	if (m_IsCropImg == true){
		cv::Mat in = cv::cvarrToMat(m_pCropImg);

		cv::resize(in, in, cv::Size(500, 500));
		cv::cvtColor(in, in, CV_BGR2RGB);

		cv::Mat mask, res;
		cv::cvtColor(in, mask, CV_RGB2GRAY);
		cv::threshold(mask, mask, 170, 255, CV_THRESH_BINARY);
		cv::imshow("mask", mask);

		cv::imshow("inpaint_before", in);
		cv::inpaint(in, mask, in, 5, cv::INPAINT_NS);
		cv::imshow("inpaint", in);
		
		SetGLTexture(m_pCropImg);
	}
}






void CSNImage::StampImage(cv::Rect _srcRect, cv::Rect targetRect, cv::Size blurSize)
{
	if (m_IsCropImg == true){

		//if (m_imgUndo){
		//	cvReleaseImage(&m_imgUndo);
		//}
		// For Undo=============== //
		//m_imgUndo = cvCloneImage(m_pCropImg);
		m_Undo.PushUndo(m_pCropImg);
		CMainFrame* pM = (CMainFrame*)AfxGetMainWnd();
		pM->SetUndoButtonState(true, 3);
		//========================//

		cv::Mat imgMat = cv::cvarrToMat(m_pCropImg);
		int orignaltype = imgMat.type();

		cv::Rect cutRect = targetRect;
		int margin = 10;
		cutRect.x -= margin;
		cutRect.y -= margin;
		cutRect.width += margin * 2;
		cutRect.height += margin * 2;

		cv::Rect srcRect = _srcRect;
		//srcRect.x -= margin;
		//srcRect.y -= margin;
		//srcRect.width += margin * 2;
		//srcRect.height += margin * 2;


		if ((cutRect.x < 0) || (cutRect.y < 0) ||
			((cutRect.x + cutRect.width) >= nWidth) || ((cutRect.y + cutRect.height) >= nHeight)){
			return;
		}

		//if ((srcRect.x < 0) || (srcRect.y < 0) ||
		//	((srcRect.x + srcRect.width) >= nWidth) || ((srcRect.y + srcRect.height) >= nHeight)){
		//	return;
		//}


		cv::Mat mask = cv::Mat::zeros(cv::Size(cutRect.width, cutRect.height), imgMat.type());
		// mask is a disk	
		int radious = targetRect.width / 2;
		circle(mask, cv::Point(cutRect.width / 2, cutRect.height / 2), radious, cv::Scalar(255, 255, 255), -1);
		cv::Mat cutImg, srcImg;
		imgMat(cutRect).copyTo(cutImg);
		imgMat(srcRect).copyTo(srcImg);

		//	cv::Size blurSize(7, 7);
		blur(mask, mask, blurSize);

		//cv::imshow("blur1", mask);
		//cv::imshow("blur2", imgBlur);

		mask.convertTo(mask, CV_32FC3, 1.0 / 255); // 
		cutImg.convertTo(cutImg, CV_32FC3);
		srcImg.convertTo(srcImg, CV_32FC3);

		multiply(mask, srcImg, srcImg);
		multiply(cv::Scalar::all(1.0) - mask, cutImg, cutImg);

		cv::Mat ouImage = cv::Mat::zeros(srcImg.size(), srcImg.type());
		add(cutImg, srcImg, ouImage);

		ouImage.convertTo(ouImage, orignaltype);
		ouImage.copyTo(imgMat(cutRect));

		//		cv::imshow("Result", imgMat);

		//cvReleaseImage(&pimg);
		//pimg = new IplImage(ouImage);
	//	cvShowImage("cropimg", m_pCropImg);

		SetGLTexture(m_pCropImg);
	}
}