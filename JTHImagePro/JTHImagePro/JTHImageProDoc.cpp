
// JTHImageProDoc.cpp : CJTHImageProDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "JTHImagePro.h"
#endif

#include "JTHImageProDoc.h"

#include <propkey.h>
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14159
#define NUM_FRAMES 10

// CJTHImageProDoc

IMPLEMENT_DYNCREATE(CJTHImageProDoc, CDocument)

BEGIN_MESSAGE_MAP(CJTHImageProDoc, CDocument)
END_MESSAGE_MAP()


// CJTHImageProDoc 생성/소멸

CJTHImageProDoc::CJTHImageProDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	inputImg = NULL;
	inputImg2 = NULL;
	resultImg = NULL;
	gResultImg = NULL;
}

CJTHImageProDoc::~CJTHImageProDoc()
{
}

BOOL CJTHImageProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CJTHImageProDoc serialization

void CJTHImageProDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		LoadImageFile(ar);
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CJTHImageProDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CJTHImageProDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CJTHImageProDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CJTHImageProDoc 진단

#ifdef _DEBUG
void CJTHImageProDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CJTHImageProDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CJTHImageProDoc 명령


void CJTHImageProDoc::PixelAdd(void)
{
	int value;

	for(int y = 0; y < imageHeight; y++)
		for(int x = 0; x < imageWidth; x++) {
			value = inputImg[y][x] + 100;
			if(value > 255) resultImg[y][x] = 255;
			else resultImg[y][x] = value;
		}
}


void CJTHImageProDoc::PixelSub(void)
{
	int value;

	for(int y = 0; y < 256; y++)
		for(int x = 0; x < 256; x++) {
			value = inputImg[y][x] - 100;
			if(value < 0) resultImg[y][x] = 0;
			else resultImg[y][x] = value;
		}
}


void CJTHImageProDoc::PixelHistoEq(void)
{
	int x, y, i, k;
	int acc_hist = 0;
	float N = (float)(imageWidth * imageHeight);
	int hist[256], sum[256];
	
	for(k = 0; k < 256; k++) hist[k] = 0;

	for(y = 0; y < imageHeight; y++)
		for(x = 0; x < imageWidth; x++) {
			k = inputImg[y][x];
			hist[k] = hist[k] + 1;
		}

	for(i = 0; i < 256; i++) {
		acc_hist = acc_hist + hist[i];
		sum[i] = acc_hist;
	}

	for(y = 0; y < imageHeight; y++)
		for(x = 0; x < imageWidth; x++) {
			k = inputImg[y][x];
			resultImg[y][x] = (unsigned char)(sum[k] / N * 255);
		}
}


void CJTHImageProDoc::PixelTwoImageAdd(void)
{
	int value;

	LoadTwoImages();

	for(int y = 0; y < imageHeight; y++)
		for(int x = 0; x < imageWidth * depth; x++) {
			value = inputImg[y][x] + inputImg2[y][x];

			if(value > 255) resultImg[y][x] = 255;
			else resultImg[y][x] = value;
		}
}


void CJTHImageProDoc::LoadTwoImages(void)
{
	CFile file;
	CFileDialog dlg(TRUE);

	AfxMessageBox("Select First Image");

	if(dlg.DoModal()==IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		LoadImageFile(ar);
		//file.Read(inputImg, 256 * 256);
		file.Close();
	}

	AfxMessageBox("Select the Second Image");

	if(dlg.DoModal()==IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		//file.Read(inputImg2, 256 * 256);
		LoadSecondImageFile(ar);
		file.Close();
	}
}


void CJTHImageProDoc::LoadImageFile(CArchive& ar)
{
	int i, maxValue;
	char type[16], buf[256];
	CFile *fp = ar.GetFile();
	CString fname = fp->GetFilePath();

	if(strcmp(strrchr(fname, '.'), ".ppm") == 0 ||
	   strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
	   strcmp(strrchr(fname, '.'), ".PGM") == 0 ||
	   strcmp(strrchr(fname, '.'), ".pgm") == 0)
	{
		ar.ReadString(type, 15);

		do{
			ar.ReadString(buf, 255);
		} while(buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

		do{
			ar.ReadString(buf, 255);
		} while(buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);

		if(strcmp(type, "P5") == 0) depth = 1;
		else depth = 3;

	}
	else if(strcmp(strchr(fname, '.'), ".raw") == 0 ||
		strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		if(fp->GetLength() != 256 * 256){
			AfxMessageBox("256X256크기의 파일만 사용가능합니다.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	inputImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	resultImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));

	for(i = 0; i < imageHeight; i++){
		inputImg[i] = (unsigned char *)malloc(imageWidth * depth);
		resultImg[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	for(i = 0; i < imageHeight; i++)
		ar.Read(inputImg[i], imageWidth * depth);
}


void CJTHImageProDoc::LoadSecondImageFile(CArchive& ar)
{
	int i, maxValue;
	char type[16], buf[256];

	CFile *fp = ar.GetFile();
	CString fname = fp->GetFilePath();

	if(strcmp(strrchr(fname, '.'), ".ppm") == 0 ||
	   strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
	   strcmp(strrchr(fname, '.'), ".PGM") == 0 ||
	   strcmp(strrchr(fname, '.'), ".pgm") == 0)

	   {
		ar.ReadString(type, 15);

		do{
			ar.ReadString(buf, 255);
		} while(buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

		do{
			ar.ReadString(buf, 255);
		} while(buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);

		if(strcmp(type, "P5") == 0) depth = 1;
		else depth = 3;

	}
	else if(strcmp(strrchr(fname, '.'), ".raw") == 0 ||
		strcmp(strrchr(fname, '.'), ".RAW") == 0)
	{
		if(fp->GetLength() != 256 * 256){
			AfxMessageBox("256X256크기의 파일만 사용가능합니다.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	inputImg2 = (unsigned char **)malloc(imageHeight * sizeof(unsigned char*));

	for(i = 0; i < imageHeight; i++){
		inputImg2[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	for(i = 0; i < imageHeight; i++)
		ar.Read(inputImg2[i], imageWidth * depth);
}


void CJTHImageProDoc::RegionSharpening()
{
	float kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

	Convolve(inputImg, resultImg, imageWidth, imageHeight, kernel, 0, depth);
}


void CJTHImageProDoc::Convolve(unsigned char **inputImg, unsigned char **resultImg, int cols, int rows, float mask[][3], int bias, int depth)
{
	int i, j, x, y;
	int red, green, blue;
	int sum;
	unsigned char **tmpImg;

	tmpImg = (unsigned char **)malloc((imageHeight + 2) * sizeof(unsigned char *));

	for(i = 0; i < imageHeight + 2; i++)
		tmpImg[i] = (unsigned char *)malloc((imageWidth + 2) * depth);

	for(y = 0; y < imageHeight + 2; y++)
		for(x = 0; x < (imageWidth + 2) * depth; x++)
			tmpImg[y][x] = 0;

	for(y = 1; y < imageHeight + 1; y++)
		for(x = 1; x < imageWidth + 1; x++)
			if(depth == 1)tmpImg[y][x] = inputImg[y - 1][x - 1];
			else if(depth == 3){
				tmpImg[y][3 * x] = inputImg[y - 1][3 * (x - 1)];
				tmpImg[y][3 * x + 1] = inputImg[y - 1][3 * (x - 1) + 1];
				tmpImg[y][3 * x + 2] = inputImg[y - 1][3 * (x - 1) + 2];
			}

		for(y = 0; y < imageHeight; y++)
			for(x = 0; x < imageWidth; x++) {
				if(depth == 1) {
					sum = 0;
					for(i = 0; i < 3; i++)
						for(j = 0; j < 3; j++)
							sum += (int)(tmpImg[y + i][x + j] * mask[i][j]);

					if(sum > 255) sum = 255;
					if(sum < 0) sum = 0;
					resultImg[y][x] = (unsigned char) sum;
				}
				else if(depth == 3) {
					red = 0;
					green = 0;
					blue = 0;

					for(i = 0; i < 3; i++)
						for(j = 0; j < 3; j++) {
							red += (int)(tmpImg[y + i][3 * (x + j)] * mask[i][j]);
							green += (int)(tmpImg[y + i][3 * (x + j) + 1] * mask[i][j]);
							blue += (int)(tmpImg[y + i][3 * (x + j) + 2] * mask[i][j]);
						}

					if(red > 255) red = 255;
					if(red < 0) red = 0;
					if(green > 255) green = 255;
					if(green < 0) green = 0;
					if(blue > 255) blue = 255;
					if(blue < 0) blue = 0;

					resultImg[y][3 * x] = (unsigned char)red;
					resultImg[y][3 * x + 1] = (unsigned char)green;
					resultImg[y][3 * x + 2] = (unsigned char)blue;
				}
			}

			for(i = 0; i< imageHeight + 2; i++) free(tmpImg[i]);
			free(tmpImg);
}

void CJTHImageProDoc::RegionSobel(void)
{
	int i, x, y, sum;
	float mask1[3][3] = {1, 0, -1,
						 2, 0, -2,
						 1, 0, -1};
	float mask2[3][3] = {-1, -2, -1,
						 0, 0, 0,
						 1, 2, 1};
	unsigned char **Er, **Ec;

	Er = (unsigned char **) malloc((imageHeight) * sizeof(unsigned char *));
	Ec = (unsigned char **) malloc((imageHeight) * sizeof(unsigned char *));

	for(i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char *) malloc(imageWidth * depth);
		Ec[i] = (unsigned char *) malloc(imageWidth * depth);
	}

	Convolve(inputImg, Er, imageWidth, imageHeight, mask1, 0, depth);
	Convolve(inputImg, Ec, imageWidth, imageHeight, mask2, 0, depth);

	for(y = 0; y < imageHeight; y++) {
		for(x = 0; x < imageWidth * depth; x++) {
			sum = sqrtl(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]);
			if(sum > 255) sum = 255;
			if(sum < 0) sum = 0;
			resultImg[y][x] = (unsigned char) sum;
		}
	}
}


void CJTHImageProDoc::RegionMedian()
{
	int i, j, temp, x, y;
	int n[9];

	for(y = 1; y < imageHeight - 1; y++)
		for(x = 1; x < imageWidth - 1; x++)
		{
			n[0] = inputImg[y - 1][x - 1];
			n[1] = inputImg[y - 1][x];
			n[2] = inputImg[y - 1][x + 1];
			n[3] = inputImg[y][x - 1];
			n[4] = inputImg[y][x];
			n[5] = inputImg[y][x + 1];
			n[6] = inputImg[y + 1][x - 1];
			n[7] = inputImg[y + 1][x];
			n[8] = inputImg[y + 1][x + 1];

			for(i = 8; i > 0; i--)
				for(j = 0; j < i; j++)
					if(n[j] > n[j + 1]) {
						temp = n[j + 1];
						n[j + 1] = n[j];
						n[j] = temp;
				}

			resultImg[y][x] = n[4];
		}
}


void CJTHImageProDoc::Erosion()
{
	int x, y, min;
	for(y = 1; y < imageHeight - 1; y++)
		for(x = 1; x < imageWidth  - 1; x++) {
			min = 255;
			if(inputImg[y - 1][x - 1]	< min) min = inputImg[y - 1][x - 1];
			if(inputImg[y - 1][x]		< min) min = inputImg[y - 1][x];
			if(inputImg[y - 1][x + 1]	< min) min = inputImg[y - 1][x + 1];
			if(inputImg[y][x - 1]		< min) min = inputImg[y][x - 1];
			if(inputImg[y][x]			< min) min = inputImg[y][x];
			if(inputImg[y][x + 1]		< min) min = inputImg[y][x + 1];
			if(inputImg[y + 1][x - 1]	< min) min = inputImg[y + 1][x - 1];
			if(inputImg[y + 1][x]		< min) min = inputImg[y + 1][x];
			if(inputImg[y + 1][x + 1]	< min) min = inputImg[y + 1][x + 1];

			resultImg[y][x] = min;
		}
}


void CJTHImageProDoc::Dilation()
{
	int x, y, max;

	for(y = 1; y < imageHeight - 1; y++)
		for(x = 1; x < imageWidth - 1; x++) {
			max = 0;
			if(inputImg[y - 1][x - 1]	> max) max = inputImg[y - 1][x - 1];
			if(inputImg[y - 1][x]		> max) max = inputImg[y - 1][x];
			if(inputImg[y - 1][x + 1]	> max) max = inputImg[y - 1][x + 1];
			if(inputImg[y][x - 1]		> max) max = inputImg[y][x - 1];
			if(inputImg[y][x]			> max) max = inputImg[y][x];
			if(inputImg[y][x + 1]		> max) max = inputImg[y][x + 1];
			if(inputImg[y + 1][x - 1]	> max) max = inputImg[y + 1][x - 1];
			if(inputImg[y + 1][x]		> max) max = inputImg[y + 1][x];
			if(inputImg[y + 1][x + 1]	> max) max = inputImg[y + 1][x + 1];

			resultImg[y][x] = max;
		}
}


void CJTHImageProDoc::Opening()
{
	Erosion();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();
}


void CJTHImageProDoc::CopyResultToInput()
{
	int x, y;

	for(y = 0; y < imageHeight; y++)
		for(x = 0; x < imageWidth; x++)
			inputImg[y][x] = resultImg[y][x];
}


void CJTHImageProDoc::Closing()
{
	Dilation();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();
}


void CJTHImageProDoc::GeometryZoominPixelCopy(void)
{
	int i, y, x;
	gImageWidth = imageWidth * 3;
	gImageHeight = imageHeight * 3;

	gResultImg = (unsigned char **) malloc(gImageHeight * sizeof(unsigned char *));

	for(i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char *) malloc(gImageWidth * depth);
	}

	for(y = 0; y < gImageHeight; y++)
		for(x = 0; x < gImageWidth; x++)
			gResultImg[y][x] = inputImg[y/3][x/3];
}


void CJTHImageProDoc::GeometryZoominInterpolation()
{
	int i, y, x;
	float src_x, src_y;
	float alpha, beta;
	int scale_x, scale_y;
	int E, F;
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;

	scale_x = 3;
	scale_y = 3;

	gImageWidth = imageWidth * scale_x;
	gImageHeight = imageHeight * scale_y;

	gResultImg = (unsigned char **) malloc(gImageHeight * sizeof(unsigned char *));
	for(i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char *) malloc(gImageWidth * depth);
	}

	for(y = 0; y < gImageHeight; y++)
		for(x = 0; x < gImageWidth; x++) {
			src_x = x / (float) scale_x;
			src_y = y / (float) scale_y;
			alpha = src_x - x / scale_x;
			beta = src_y - y / scale_y;

			Ax = x / scale_x;
			Ay = y / scale_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;

			if(Bx > imageWidth - 1) Bx = imageWidth - 1;
			if(Dx > imageWidth - 1) Bx = imageWidth - 1;
			if(Cy > imageHeight - 1) Cy = imageHeight - 1;
			if(Dy > imageHeight - 1) Dy = imageHeight - 1;

			E = (int)(inputImg[Ay][Ax] * (1 - alpha) + inputImg[By][Bx] * alpha);
			F = (int)(inputImg[Cy][Cx] * (1 - alpha) + inputImg[Dy][Dx] * alpha);

			gResultImg[y][x] = (unsigned char)(E * (1 - beta) + F * beta);
		}
}

void CJTHImageProDoc::GeometryZoomoutSubsampling(void)
{
	int i, y, x;
	int src_x, src_y;
	int scale_x = 3, scale_y = 3;

	gImageWidth = imageWidth / scale_x;
	gImageHeight = imageHeight / scale_y;

	gResultImg = (unsigned char **) malloc(gImageHeight * sizeof(unsigned char *));
	for(i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char *) malloc(gImageWidth * depth);
	}

	for(y = 0; y < gImageHeight; y++)
		for(x = 0; x < gImageWidth; x++) {
			src_y = y * scale_y;
			src_x = x * scale_x;
			if(src_x > imageWidth - 1) src_x = imageWidth - 1;
			if(src_y > imageHeight - 1) src_y = imageHeight - 1;
			gResultImg[y][x] = inputImg[src_y][src_x];
		}
}

void CJTHImageProDoc::GeometryZoomoutAvg()
{
	int i, j, x, y;
	int sum;
	int src_x, src_y;
	int scale_x = 3, scale_y = 3;

	gImageWidth = imageWidth / scale_x + 1;
	gImageHeight = imageHeight / scale_y + 1;

	gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));
	for(i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char *) malloc(gImageWidth * depth);
	}

	for(y = 0; y < imageHeight; y = y + scale_y)
		for(x = 0; x < imageWidth; x = x + scale_x) {
			sum = 0;
			for(i = 0; i < scale_y; i++)
				for(j = 0; j < scale_x; j++) {
					src_x = x + j;
					src_y = y + i;

					if(src_x > imageWidth - 1) src_x = imageWidth - 1;
					if(src_y > imageHeight - 1) src_y = imageHeight - 1;

					sum += inputImg[src_y][src_x];
				}

				sum = sum / (scale_x * scale_y);
				if(sum > 255) sum = 255;
				if(sum < 0) sum = 0;

				gResultImg[y / scale_y][x / scale_x] = (unsigned char) sum;
		}
}


void CJTHImageProDoc::GeometryRotate()
{
	int y, x, x_source, y_source, Cx, Cy;
	float angle;
	int Oy;
	int i, xdiff, ydiff;

	Oy = imageHeight - 1;

	angle = PI / 180.0 * 30.0;

	Cx = imageWidth / 2;
	Cy = imageHeight / 2;

	gImageWidth = (int)(imageHeight * cos(PI / 2.0 - angle) +
						imageWidth * cos(angle));
	gImageHeight = (int)(imageHeight * cos(angle) +
						imageWidth * cos(PI / 2.0 - angle));

	gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));

	for(i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
	}

	xdiff = (gImageWidth - imageWidth) / 2;
	ydiff = (gImageHeight - imageHeight) / 2;

	for(y = -ydiff; y < gImageHeight - ydiff; y++)
		for(x = -xdiff; x < gImageWidth - xdiff; x++) {
			x_source = (int)(((Oy - y) - Cy) * sin(angle) + (x - Cx) * cos(angle) + Cx);
			y_source = (int)(((Oy - y) - Cy) * cos(angle) - (x - Cx) * sin(angle) + Cy);

			y_source = Oy - y_source;

			if(x_source < 0 || x_source > imageWidth - 1 ||
			   y_source < 0 || y_source > imageHeight - 1)
			   gResultImg[y + ydiff][x + xdiff] = 255;
			else
				gResultImg[y + ydiff][x + xdiff] = inputImg[y_source][x_source];
		}
}


void CJTHImageProDoc::GeometryMirror(void)
{
	int y, x;

	for(y = 0; y < imageHeight; y++)
		for(x = 0; x < imageWidth; x++)
			resultImg[y][x] = inputImg[y][imageWidth - 1 - x];
}


void CJTHImageProDoc::GeometryFlip()
{
	int y, x;

	for(y = 0; y < imageHeight; y++)
		for(x = 0; x < imageWidth; x++)
			resultImg[imageHeight - 1 - y][x] = inputImg[y][x];
}

typedef struct
{
	int Px;
	int Py;
	int Qx;
	int Qy;
} control_line;

void CJTHImageProDoc::GeometryWarping(void)
{
	control_line source_lines[23] = {{116, 7, 207, 5}, {34, 109, 90, 21}, {55, 249, 30, 128}, {118, 320, 65, 261},
									 {123, 321, 171, 321}, {179, 319, 240, 264}, {247, 251, 282, 135}, {281, 114, 228, 8},
									 {78, 106, 123, 109}, {187, 115, 235, 114}, {72, 142, 99, 128}, {74, 150, 122, 154},
									 {108, 127, 123, 146}, {182, 152, 213, 132}, {183, 159, 229, 157}, {219, 131, 240, 154},
									 {80, 246, 117, 212}, {127, 222, 146, 223}, {154, 227, 174, 221}, {228, 252, 183, 213},
									 {114, 255, 186, 257}, {109, 258, 143, 277}, {152, 278, 190, 262}};

	control_line dest_lines[23] = {{120, 8, 200, 6}, {12, 93, 96, 16}, {74, 271, 16, 110}, {126, 336, 96, 290},
								  {142, 337, 181, 335}, {192, 335, 232, 280}, {244, 259, 288, 108}, {285, 92, 212, 13},
								  {96, 135, 136, 118}, {194, 119, 223, 125}, {105, 145, 124, 134}, {110, 146, 138, 151},
								  {131, 133, 139, 146}, {188, 146, 198, 134}, {189, 153, 218, 146}, {204, 133, 221, 140},
								  {91, 268, 122, 202}, {149, 206, 159, 209}, {170, 209, 181, 204}, {235, 265, 208, 199},
								  {121, 280, 205, 284}, {112, 286, 160, 301}, {166, 301, 214, 287}};

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;

	double weight;
	double totalWeight;
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;

	int x1, x2, y1, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;

	int num_lines = 23;
	int line;
	int x, y;
	int source_x, source_y;
	int last_row, last_col;

	last_row = imageHeight - 1;
	last_col = imageWidth - 1;

	for(y = 0; y < imageHeight; y++)
	{
		for(x = 0; x < imageWidth; x++)
		{
			totalWeight = 0.0;
			tx = 0.0;
			ty = 0.0;

			for(line = 0; line < num_lines; line++)
			{
				x1 = dest_lines[line].Px;
				y1 = dest_lines[line].Py;
				x2 = dest_lines[line].Qx;
				y2 = dest_lines[line].Qy;

				dest_line_length = sqrt((double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));

				u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

				if(u < 0) d = sqrt((double)((x - x1) * (x - x1) + (y - y1) * (y - y1)));
				else if(u > 1) d = sqrt((double) ((x - x2) * (x - x2) + (y - y2) * (y - y2)));
				else d = fabs(h);

				src_x1 = source_lines[line].Px;
				src_y1 = source_lines[line].Py;
				src_x2 = source_lines[line].Qx;
				src_y2 = source_lines[line].Qy;
				src_line_length = sqrt((double)((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1)));

				xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
				yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

				weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

				tx += (xp - x) * weight;
				ty += (yp - y) * weight;
				totalWeight += weight;
			}

			source_x = x + (int)(tx / totalWeight + 0.5);
			source_y = y + (int)(ty / totalWeight + 0.5);

			if(source_x < 0) source_x = 0;
			if(source_x > last_col) source_x = last_col;
			if(source_y < 0) source_y = 0;
			if(source_y > last_row) source_y = last_row;

			resultImg[y][x] = inputImg[source_y][source_x];
		}
	}
}


void CJTHImageProDoc::GeometryMorphing(void)
{
	control_line source_lines[23] = {{116, 7, 207, 5}, {34, 109, 90, 21}, {55, 249, 30, 128}, {118, 320, 65, 261},
									 {123, 321, 171, 321}, {179, 319, 240, 264}, {247, 251, 282, 135}, {281, 114, 228, 8},
									 {78, 106, 123, 109}, {187, 115, 235, 114}, {72, 142, 99, 128}, {74, 150, 122, 154},
									 {108, 127, 123, 146}, {182, 152, 213, 132}, {183, 159, 229, 157}, {219, 131, 240, 154},
									 {80, 246, 117, 212}, {127, 222, 146, 223}, {154, 227, 174, 221}, {228, 252, 183, 213},
									 {114, 255, 186, 257}, {109, 258, 143, 277}, {152, 278, 190, 262}};

	control_line dest_lines[23] = {{120, 8, 200, 6}, {12, 93, 96, 16}, {74, 271, 16, 110}, {126, 336, 96, 290},
								  {142, 337, 181, 335}, {192, 335, 232, 280}, {244, 259, 288, 108}, {285, 92, 212, 13},
								  {96, 135, 136, 118}, {194, 119, 223, 125}, {105, 145, 124, 134}, {110, 146, 138, 151},
								  {131, 133, 139, 146}, {188, 146, 198, 134}, {189, 153, 218, 146}, {204, 133, 221, 140},
								  {91, 268, 122, 202}, {149, 206, 159, 209}, {170, 209, 181, 204}, {235, 265, 208, 199},
								  {121, 280, 205, 284}, {112, 286, 160, 301}, {166, 301, 214, 287}};

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;

	double weight;
	double totalWeight;
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;

	unsigned char **warpedImg;
	unsigned char **warpedImg2;
	int frame;
	double fweight;
	control_line warp_lines[23];
	double tx2, ty2;
	double xp2, yp2;
	int dest_x1, dest_y1, dest_x2, dest_y2;
	int source_x2, source_y2;

	int x1, x2, y1, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;

	int i, j;
	int num_lines = 23;
	int line;
	int x, y;
	int source_x, source_y;
	int last_row, last_col;

	LoadTwoImages();

	warpedImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	for(i = 0; i < imageHeight; i++) {
		warpedImg[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	warpedImg2 = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	for(i = 0; i < imageHeight; i++) {
		warpedImg2[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	for(i = 0; i < NUM_FRAMES; i++) {
		morphedImg[i] = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));

		for(j = 0; j < imageHeight; j++) {
			morphedImg[i][j] = (unsigned char *)malloc(imageWidth * depth);
		}
	}

	last_row = imageHeight - 1;
	last_col = imageWidth - 1;

	for(frame = 1; frame <= NUM_FRAMES; frame++)
	{
		fweight = (double)(frame) / NUM_FRAMES;

		for(line = 0; line < num_lines; line++)
		{
			warp_lines[line].Px = source_lines[line].Px + (dest_lines[line].Px - source_lines[line].Px) * fweight;
			warp_lines[line].Py = source_lines[line].Py + (dest_lines[line].Py - source_lines[line].Py) * fweight;
			warp_lines[line].Qx = source_lines[line].Qx + (dest_lines[line].Qx - source_lines[line].Qx) * fweight;
			warp_lines[line].Qy = source_lines[line].Qy + (dest_lines[line].Qy - source_lines[line].Qy) * fweight;
		}

		for(y = 0; y < imageHeight; y++)
		{
			for(x = 0; x < imageWidth; x++)
			{
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;

				tx2 = 0.0;
				ty2 = 0.0;

				for(line = 0; line < num_lines; line++)
				{
					x1 = warp_lines[line].Px;
					y1 = warp_lines[line].Py;
					x2 = warp_lines[line].Qx;
					y2 = warp_lines[line].Qy;

					dest_line_length = sqrt((double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));

					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
						(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					if(u < 0) d = sqrt((double)((x - x1) * (x - x1) + (y - y1) * (y - y1)));
					else if(u > 1) d = sqrt((double)((x - x2) * (x - x2) + (y - y2) * (y - y2)));
					else d = fabs(h);

					src_x1 = source_lines[line].Px;
					src_y1 = source_lines[line].Py;
					src_x2 = source_lines[line].Qx;
					src_y2 = source_lines[line].Qy;
					src_line_length = sqrt((double)((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1)));

					dest_x1 = dest_lines[line].Px;
					dest_y1 = dest_lines[line].Py;
					dest_x2 = dest_lines[line].Qx;
					dest_y2 = dest_lines[line].Qy;
					src_line_length = sqrt((double)((dest_x2 - dest_x1) * (dest_x2 - dest_x1) + (dest_y2 - dest_y1) * (dest_y2 - dest_y1)));

					xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

					xp2 = dest_x1 + u * (dest_x2 - dest_x1) - h * (dest_y2 - dest_y1) / dest_line_length;
					yp2 = dest_y1 + u * (dest_y2 - dest_y1) + h * (dest_x2 - dest_x1) / dest_line_length;

					weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

					tx += (xp - x) * weight;
					ty += (yp - y) * weight;

					tx2 += (xp2 - x) * weight;
					ty2 += (yp2 - y) * weight;

					totalWeight += weight;
				}

				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);

				source_x2 = x + (int)(tx2 / totalWeight + 0.5);
				source_y2 = y + (int)(ty2 / totalWeight + 0.5);

				if(source_x < 0) source_x = 0;
				if(source_x > last_col) source_x = last_col;
				if(source_y < 0) source_y = 0;
				if(source_y > last_row) source_y = last_row;

				if(source_x2 < 0) source_x2 = 0;
				if(source_x2 > last_col) source_x2 = last_col;
				if(source_y2 < 0) source_y2 = 0;
				if(source_y2 > last_row) source_y2 = last_row;

				warpedImg[y][x] = inputImg[source_y][source_x];
				warpedImg2[y][x] = inputImg2[source_y2][source_x2];
			}
		}

		for(y = 0; y < imageHeight; y++)
			for(x = 0; x < imageWidth; x++) {
				int val = (int) ((1.0 - fweight) * warpedImg[y][x] + fweight * warpedImg2[y][x]);
				if(val < 0) val = 0;
				if(val > 255) val = 255;
				morphedImg[frame - 1][y][x] = val;
			}
	}
}


void CJTHImageProDoc::DCT(void)
{
	int u, v;
	int x, y;
	int value;

	dct_result = (double **) malloc(sizeof(double *) * imageHeight);
	for(y = 0; y < imageHeight; y++)
		dct_result[y] = (double *) malloc(sizeof(double) * imageWidth);

	for(u = 0; u < imageWidth; u++) {
		for(v = 0; v < imageHeight; v++) {
			dct_result[u][v] = 0.0;
			for(x = 0; x < imageWidth; x++) {
				for(y = 0; y < imageHeight; y++) {
					dct_result[u][v] += inputImg[y][x] * 
						cos(((2.0 * x + 1.0) * u * PI) / (2.0 * imageWidth)) *
						cos(((2.0 * y + 1.0) * v * PI) / (2.0 * imageHeight));
				}
			}
			dct_result[u][v] *= 2.0 / sqrt((double)(imageWidth * imageHeight));
			if(u == 0) dct_result[u][v] *= 1.0 / sqrt(2.0);
			if(v == 0) dct_result[u][v] *= 1.0 / sqrt(2.0);
		}
	}

	for(y = 0; y < imageWidth; y++)
		for(x = 0; x < imageHeight; x++) {
			value = (int) (30 * log(1.0 + dct_result[y][x]));
			if(value < 0) value = 0;
				if(value > 255) value = 255;
					resultImg[y][x] = value;
		}
}


void CJTHImageProDoc::FastDCT(void)
{
	int i, j, x, y, value;
	double *in, *out;

	dct_result = (double **) malloc(sizeof(double *) * imageHeight);
	for(y = 0; y < imageHeight; y++)
		dct_result[y] = (double *) malloc(sizeof(double) * imageWidth);

	in = (double *) malloc(sizeof(double) * imageHeight);
	out = (double *) malloc(sizeof(double) * imageWidth);

	for(j = 0; j < imageWidth; j++)
	{
		for(i = 0; i < imageWidth; i++)
			in[i] = inputImg[j][i];
		DCT_1D(in ,out, imageWidth);
		for(i = 0; i < imageWidth; i++) dct_result[j][i] = out[i];
	}

	free(in);
	free(out);
	out = (double * ) malloc(sizeof(double) * imageHeight);
	in =  (double * ) malloc(sizeof(double) * imageHeight);

	for(j = 0; j < imageWidth; j++)
	{
		for(i = 0; i < imageHeight; i++)
			in[i] = dct_result[i][j];
		DCT_1D(in, out, imageHeight);
		for(i = 0; i < imageHeight; i++) dct_result[i][j] = out[i];
	}

	for(y = 0; y < imageWidth; y++)
		for(x = 0; x < imageHeight; x++) {
			value = (int) (30 * log(1.0 + dct_result[y][x]));
			if(value < 0) value = 0;
			if(value > 255) value = 255;
			resultImg[y][x] = value;
		}
}


void CJTHImageProDoc::DCT_1D(double * in, double * out, const int count)
{
	int x, u;

	for(u = 0; u < count; u++) {
		double z = 0;
		for(x = 0; x < count; x++) {
			z += in[x] * cos(PI * (double)u * (double)(2 * x + 1) / (double)(2 * count));
		}

		if(u == 0) z = z * (1.0 / sqrt(2.0));
		out[u] = z * (sqrt(2.0 / (double) count));
	}
}


void CJTHImageProDoc::FastIDCT(void)
{
	int i, j, x, y;
	double *in, *out, **idct_result;

	idct_result = (double **) malloc(sizeof(double *) * imageHeight);
	for(y = 0; y < imageHeight; y++)
		idct_result[y] = (double *) malloc(sizeof(double) * imageWidth);
		
	in = (double *) malloc(sizeof(double) * imageHeight);
	out = (double *) malloc(sizeof(double) * imageWidth);

	for(j = 0; j < imageHeight; j++)
	{
		for(i = 0; i < imageWidth; i++)
			in[i] = dct_result[j][i];

		IDCT_1D(in, out, imageWidth);

		for(i = 0; i < imageWidth; i++)
			idct_result[j][i] = out[i];
	}

	free(in);
	free(out);
	out = (double *) malloc(sizeof(double) * imageHeight);
	in = (double *) malloc(sizeof(double) * imageHeight);

	for(j = 0; j < imageWidth; j++)
	{
		for(i = 0; i < imageHeight; i++)
			in[i] = idct_result[i][j];

		IDCT_1D(in, out, imageHeight);

		for(i = 0; i < imageHeight; i++)
			idct_result[i][j] = out[i];
	}

	for(y = 0; y < imageWidth; y++)
		for(x = 0; x < imageHeight; x++)
			resultImg[y][x] = (unsigned char) idct_result[y][x];
}


void CJTHImageProDoc::IDCT_1D(double *in, double *out, const int count)
{
	int x, u;
	double cv;

	for(x = 0; x < count; x++)
	{
		double z = 0;

		for(u = 0; u < count; u++)
		{
			if(u == 0) cv = 1.0 / sqrt(2.0);
			else cv = 1.0;

			z += in[u] * cv * cos(PI * (double)u * (double)(2 * x + 1) / (double)(2 * count));
		}
		out[x] = z * (sqrt(2.0 / (double)count));
	}
}


void CJTHImageProDoc::SaveJpeg(void)
{
	CString fname;
	CFile file;
	CFileDialog dlg(FALSE);
	unsigned char *img;
	short *Y, *Cb, *Cr;

	if(dlg.DoModal() == IDOK) {
		fname = dlg.GetPathName();
		if(file.Open(fname, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) == 0)
			return;

		if(strcmp(strchr(fname, '.'), ".JPG") == 0 ||
			strcmp(strchr(fname, '.'), ".jpg") == 0)
		{
			int i, j;
			int img_W;
			int img_H;

			WriteSOI(&file);
			WriteDQT(&file);
			WriteDHT(&file);
			WriteSOF(&file, imageWidth, imageHeight);
			WriteSOS(&file);

			img_W = ((imageWidth - 1) / 8 + 1) * 8;
			img_H = ((imageHeight - 1) / 8 + 1) * 8;

			img = (unsigned char *) malloc(img_W * img_H * 3);
			memset(img, 0, img_W * img_H * 3);

			for(int y = 0; y < imageHeight; y++)
				if(depth == 3)
					memcpy(&img[y * (img_W * 3)], inputImg[y], imageWidth * depth);
				else
					memcpy(&img[y * (img_W)], inputImg[y], imageWidth);

			if(depth == 3) {
				float R, G, B;
				float y_val, cb, cr;

				for(i = 0; i < imageHeight; i++)
				{
					for(j = 0; j < imageWidth; j++)
					{
						R = (float)img[(i * img_W + j) * 3];
						G = (float)img[(i * img_W + j) * 3 + 1];
						B = (float)img[(i * img_W + j) * 3 + 2];

						y_val = 0.299f * R + 0.587f * G + 0.114f * B;
						cb = -0.1687f * R - 0.3313f * G + 0.5f * B + 128;
						cr = 0.5f * R - 0.4187f * G - 0.0813f * B + 128;

						if(y_val > 255.0) y_val = 255.0;
						if(y_val < 0.0) y_val = 0.0;
						if(cb > 255.0) cb = 255.0;
						if(cb < 0.0) cb = 0.0;
						if(cr > 255.0) cr = 255.0;
						if(cr < 0.0) cr = 0.0;

						img[(i * img_W + j) * 3] = (unsigned char) y_val;
						img[(i * img_W + j) * 3 + 1] = (unsigned char) cb;
						img[(i * img_W + j) * 3 + 2] = (unsigned char) cr;
					}
				}

				Y = (short *) malloc(sizeof(short) * img_W * img_H);
				memset(Y, 0, sizeof(short) * img_W * img_H);
				Cb = (short *) malloc(sizeof(short) * img_W * img_H);
				memset(Cb, 0, sizeof(short) * img_W * img_H);
				Cr = (short *) malloc(sizeof(short) * img_W * img_H);
				memset(Cr, 0, sizeof(short) * img_W * img_H);

				int idx1, idx2;

				for(i = 0; i < imageHeight; i++)
				{
					for(j = 0; j < imageWidth; j++)
					{
						idx1 = i * img_W + j;
						idx2 = i * (img_W * 3) + j * 3;
						Y[idx1] = img[idx2] - 128;
						Cb[idx1] = img[idx2 + 1] - 128;
						Cr[idx1] = img[idx2 + 2] - 128;
					}
				}

				for(i = 0; i < img_H; i = i + 8)
				{
					for(j = 0; j < img_W; j = j + 8)
					{
						DCT_Zigzag_Quantization(&Y[i * img_W + j], img_W, 0);
						DCT_Zigzag_Quantization(&Cb[i * img_W + j], img_W, 1);
						DCT_Zigzag_Quantization(&Cr[i * img_W + j], img_W, 1);
					}
				}

				int nBlocks_X = img_W / 8;
				int nBlocks_Y = img_H / 8;
				int idx = 0;

				short *DC1 = (short *) malloc (sizeof(short) * nBlocks_X * nBlocks_Y);
				short *DC2 = (short *) malloc (sizeof(short) * nBlocks_X * nBlocks_Y);
				short *DC3 = (short *) malloc (sizeof(short) * nBlocks_X * nBlocks_Y);

				for(i = 0; i < img_H; i = i + 8)
				{
					for(j = 0; j < img_W; j = j + 8)
					{
						DC1[idx] = Y[i * img_W + j];
						DC2[idx] = Cb[i * img_W + j];
						DC3[idx] = Cr[i * img_W + j];
						idx++;
					}
				}

				for(i = nBlocks_X * nBlocks_Y - 1; i > 0; i--)
				{
					DC1[i] = DC1[i] - DC1[i - 1];
					DC2[i] = DC2[i] - DC2[i - 1];
					DC3[i] = DC3[i] - DC3[i - 1];
				}

				idx = 0;
				for(i = 0; i < img_H; i = i + 8)
				{
					for(j = 0; j < img_W; j = j + 8)
					{
						Y[i * img_W + j] = DC1[idx];
						Cb[i * img_W + j] = DC2[idx];
						Cr[i * img_W + j] = DC3[idx];
						idx++;
					}
				}
			}
			else {
				int i;
				Y = (short *) malloc(sizeof(short) * img_W * img_H);
				memset(Y, 0, sizeof(short) * img_W * img_H);

				for(i = 0; i < img_W * img_H; i++)
					Y[i] = img[i] - 128;
				for(i = 0; i < img_H; i = i + 8)
					for(j = 0; j < img_W; j = j + 8)
						DCT_Zigzag_Quantization(&Y[i * img_W + j], img_W, 0);

				int nBlocks_X = img_W / 8;
				int nBlocks_Y = img_H / 8;
				int idx = 0;

				short *DC = (short *) malloc (sizeof(short) * nBlocks_X * nBlocks_Y);

				for(i = 0; i < img_H; i = i + 8)
					for(j = 0; j < img_W; j = j + 8)
						DC[idx++] = Y[i * img_W + j];

				for(i = nBlocks_X * nBlocks_Y - 1; i > 0; i--)
					DC[i] = DC[i] - DC[i - 1];

				idx = 0;
				for(i = 0; i < img_H; i = i + 8)
					for(j = 0; j < img_W; j = j + 8)
						Y[i * img_W + j] = DC[idx++];
			}

			int cnt = 0;

			for(i = 0; i < img_H; i = i + 8)
			{
				for(j = 0; j < img_W; j = j + 8)
				{
					Encode(&file, &Y[i * img_W + j], 0, img_W, &cnt);
					if(depth == 3) {
						Encode(&file, &Cb[i * img_W + j], 1, img_W, &cnt);
						Encode(&file, &Cr[i * img_W + j], 1, img_W, &cnt);
					}
				}
			}

			if(cnt != 0)
				WriteCode(&file, 0, 8 - cnt, &cnt);

			WriteEOI(&file);
		}
		file.Close();
	}
}


void CJTHImageProDoc::WriteSOI(CFile * file)
{
	unsigned char buf[2] = {0xFF, 0xD8};
	file->Write(buf, 2);
}


void CJTHImageProDoc::WriteDQT(CFile * file)
{
	unsigned char marker[2] = {0xFF, 0xDB};
	short int tableSize;
	unsigned char c;

	unsigned char QT0[64] = {16, 11, 12, 14, 12, 10, 16, 14,
							 13, 14, 18, 17, 16, 19, 24, 40,
							 26, 24, 22, 22, 24, 49, 35, 37,
							 29, 40, 58, 51, 61, 60, 57, 51,
							 56, 55, 64, 72, 92, 78, 64, 68,
							 87, 69, 55, 56, 80, 109, 81, 87,
							 95, 98, 103, 104, 103, 62, 77, 113,
							 121, 112, 100, 120, 92, 101, 103, 99};

    unsigned char QT1[64] = {17, 18, 18, 24, 21, 24, 47, 26,
							 26, 47, 99, 66, 56, 66, 99, 99,
							 99 ,99, 99, 99, 99, 99, 99, 99, 
							 99, 99, 99, 99, 99, 99, 99, 99, 
							 99, 99, 99, 99, 99, 99, 99, 99, 
							 99, 99, 99, 99, 99, 99, 99, 99, 
							 99, 99, 99, 99, 99, 99, 99, 99, 
							 99, 99, 99, 99, 99, 99, 99, 99};

	file->Write(marker, 2);

	if(depth == 3) tableSize = 132;
	else tableSize = 67;

	tableSize = (tableSize << 8) | (tableSize >> 8);
	file->Write(&tableSize, 2);

	c = 0;
	file->Write(&c, 1);
	file->Write(QT0, 64);

	if(depth == 3) {
		c = 1;
		file->Write(&c, 1);
		file->Write(QT1, 64);
	}
}


void CJTHImageProDoc::WriteDHT(CFile * file)
{
	unsigned char marker[2] = {0xFF, 0xC4};
	short int tableSize;
	unsigned char c;

	unsigned char HT_code_0[] = {0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0};

    unsigned char HT_symbol_0[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    unsigned char HT_code_16[] = {0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125};

    unsigned char HT_symbol_16[] = {1, 2, 3, 0, 4, 17, 5, 18, 33, 49,
									65, 6, 19, 81, 97, 7, 34, 113, 20, 50,
									129, 145, 161, 8, 35, 66, 177, 193, 21, 
									82, 209, 240, 36, 51, 98, 114, 130, 9, 
									10, 22, 23, 24, 25, 26, 37, 38, 39, 40, 
									41, 42, 52, 53, 54, 55, 56, 57, 58, 67,
									68, 69, 70, 71, 72, 73, 74, 83, 84, 85, 
									86, 87, 88, 89, 90, 99, 100, 101, 102, 
									103, 104, 105, 106, 115, 116, 117, 118, 
									119, 120, 121, 122, 131, 132, 133, 134,
									135, 136, 137, 138, 146, 147, 148, 149, 
									150, 151, 152, 153, 154, 162, 163, 164, 
									165, 166, 167, 168, 169, 170, 178, 179,
									180, 181, 182, 183, 184, 185, 186, 194, 
									195, 196, 197, 198, 199, 200, 201, 202, 
									210, 211, 212, 213, 214, 215, 216, 217, 
									218, 225, 226, 227, 228, 229, 230, 231, 
									232, 233, 234, 241, 242, 243, 244, 245, 
									246, 247, 248, 249, 250};

    unsigned char HT_code_1[] = {0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};

    unsigned char HT_symbol_1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    unsigned char HT_code_17[] = {0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 119};

    unsigned char HT_symbol_17[] = {0, 1, 2, 3, 17, 4, 5, 33, 49,
									6, 18, 65, 81, 7, 97, 113, 19, 34, 50, 
									129, 8, 20, 66, 145, 161, 177, 193, 9, 
									35, 51, 82, 240, 21, 98, 114, 209, 10, 
									22, 36, 52, 225, 37, 241, 23, 24, 25, 26, 
									38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 
									58, 67, 68, 69, 70, 71, 72, 73, 74, 83, 
									84, 85, 86, 87, 88, 89, 90, 99, 100, 101, 
									102, 103, 104, 105, 106, 115, 116, 117, 
									118, 119, 120, 121, 122, 130, 131, 132, 
									133, 134, 135, 136, 137, 138, 146, 147, 
									148, 149, 150, 151, 152, 153, 154, 162, 
									163, 164, 165, 166, 167, 168, 169, 170, 
									178, 179, 180, 181, 182, 183, 184, 185, 
									186, 194, 195, 196, 197, 198, 199, 200, 
									201, 202, 210, 211, 212, 213, 214, 215, 
									216, 217, 218, 226, 227, 228, 229, 230, 
									231, 232, 233, 234, 242, 243, 244, 245, 
									246, 247, 248, 249, 250};

	file->Write(marker, 2);

	if(depth == 3) tableSize = 418;
	else tableSize = 210;

	tableSize = (tableSize << 8) | (tableSize >> 8);
	file->Write(&tableSize, 2);

	c = 0;
	file->Write(&c, 1);
	file->Write(HT_code_0, 16);
	file->Write(HT_symbol_0, 12);

	c = 16;
	file->Write(&c, 1);
	file->Write(HT_code_16, 16);
	file->Write(HT_symbol_16, 162);

	if(depth == 3) {
		c = 1;
		file->Write(&c, 1);
		file->Write(HT_code_1, 16);
		file->Write(HT_symbol_1, 12);

		c = 17;
		file->Write(&c, 1);
		file->Write(HT_code_17, 16);
		file->Write(HT_symbol_17, 162);
	}
}


void CJTHImageProDoc::WriteSOF(CFile * file, int Width, int Height)
{
	unsigned short headerSize, w;
	unsigned char c;
	int i;
	unsigned char marker[2] = {0xFF, 0xC0};

	file->Write(marker, 2);

	headerSize = 8 + depth * 3;
	headerSize = (headerSize << 8) | (headerSize >> 8);
	file->Write(&headerSize, 2);

	c = 8;
	file->Write(&c, 1);

	w = (unsigned short)Height;
	w = (w << 8) | (w >> 8);
	file->Write(&w, 2);

	w = (unsigned short)Width;
	w = (w << 8) | (w >> 8);
	file->Write(&w, 2);

	c = depth;
	file->Write(&c, 1);

	for(i = 1; i <= depth; i++)
	{
		c = (unsigned char)i;
		file->Write(&c, 1);

		c = 0x11;
		file->Write(&c, 1);

		if(i == 1) c = 0;
		else c = 1;
		file->Write(&c, 1);
	}
}


void CJTHImageProDoc::WriteSOS(CFile * file)
{
	unsigned short headerSize;
	unsigned char c;
	unsigned char marker[2] = {0xFF, 0xDA};
	int i;

	file->Write(marker, 2);

	headerSize = 6 + depth * 2; 
    headerSize = (headerSize << 8) | (headerSize >> 8);
    file->Write(&headerSize, 2); 

    c = depth;
    file->Write(&c, 1); 

    for(i=1; i <= depth; i++)
    {
        c = (unsigned char)i;
        file->Write(&c, 1); 
        
        if(i==1)
            c=0;
        else
            c=0x11;
        file->Write(&c, 1);
    }

    c = 0;
    file->Write(&c, 1); 
    c = 0x3f;
    file->Write(&c, 1); 
    c = 0;
    file->Write(&c, 1); 
}

void CJTHImageProDoc::WriteEOI(CFile * file)
{
	unsigned char marker[2] = {0xFF, 0xD9};
    file->Write(marker, 2); 
}

void CJTHImageProDoc::Encode(CFile * file, short * ptr, int num, int img_W, int * cnt)
{
	unsigned char symbol_1;    
    unsigned char size;    
    short data[64], symbol_2;    
    int idx = 0;    
    int i, j;
    unsigned char k = 0, run_length = 0;
    
    unsigned short int DC_HUFFCODE[2][13] = {
        {0x0, 0x2, 0x3, 0x4, 0x5, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe, 0},
        {0x0, 0x1, 0x2, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe, 0x3fe, 0x7fe, 0}
    };

    unsigned short int AC_HUFFCODE[2][251] = {
        {0xa,0x0,0x1,0x4,0xb,0x1a,0x78,0xf8,0x3f6,0xff82,0xff83,0,0,0,0,0,
         0,0xc,0x1b,0x79,0x1f6,0x7f6,0xff84,0xff85,0xff86,0xff87,0xff88,0,0,0,0,0,
         0,0x1c,0xf9,0x3f7,0xff4,0xff89,0xff8a,0xff8b,0xff8c,0xff8d,0xff8e,0,0,0,0,0,
         0,0x3a,0x1f7,0xff5,0xff8f,0xff90,0xff91,0xff92,0xff93,0xff94,0xff95,0,0,0,0,0,
         0,0x3b,0x3f8,0xff96,0xff97,0xff98,0xff99,0xff9a,0xff9b,0xff9c,0xff9d,0,0,0,0,0,
         0,0x7a,0x7f7,0xff9e,0xff9f,0xffa0,0xffa1,0xffa2,0xffa3,0xffa4,0xffa5,0,0,0,0,0,
         0,0x7b,0xff6,0xffa6,0xffa7,0xffa8,0xffa9,0xffaa,0xffab,0xffac,0xffad,0,0,0,0,0,
         0,0xfa,0xff7,0xffae,0xffaf,0xffb0,0xffb1,0xffb2,0xffb3,0xffb4,0xffb5,0,0,0,0,0,
         0,0x1f8,0x7fc0,0xffb6,0xffb7,0xffb8,0xffb9,0xffba,0xffbb,0xffbc,0xffbd,0,0,0,0,0,
         0,0x1f9,0xffbe,0xffbf,0xffc0,0xffc1,0xffc2,0xffc3,0xffc4,0xffc5,0xffc6,0,0,0,0,0,
         0,0x1fa,0xffc7,0xffc8,0xffc9,0xffca,0xffcb,0xffcc,0xffcd,0xffce,0xffcf,0,0,0,0,0,
         0,0x3f9,0xffd0,0xffd1,0xffd2,0xffd3,0xffd4,0xffd5,0xffd6,0xffd7,0xffd8,0,0,0,0,0,
         0,0x3fa,0xffd9,0xffda,0xffdb,0xffdc,0xffdd,0xffde,0xffdf,0xffe0,0xffe1,0,0,0,0,0,
         0,0x7f8,0xffe2,0xffe3,0xffe4,0xffe5,0xffe6,0xffe7,0xffe8,0xffe9,0xffea,0,0,0,0,0,
         0,0xffeb,0xffec,0xffed,0xffee,0xffef,0xfff0,0xfff1,0xfff2,0xfff3,0xfff4,0,0,0,0,0,
         0x7f9,0xfff5,0xfff6,0xfff7,0xfff8,0xfff9,0xfffa,0xfffb,0xfffc,0xfffd,0xfffe},
        {0x0,0x1,0x4,0xa,0x18,0x19,0x38,0x78,0x1f4,0x3f6,0xff4,0,0,0,0,0,
         0,0xb,0x39,0xf6,0x1f5,0x7f6,0xff5,0xff88,0xff89,0xff8a,0xff8b,0,0,0,0,0,
         0,0x1a,0xf7,0x3f7,0xff6,0x7fc2,0xff8c,0xff8d,0xff8e,0xff8f,0xff90,0,0,0,0,0,
         0,0x1b,0xf8,0x3f8,0xff7,0xff91,0xff92,0xff93,0xff94,0xff95,0xff96,0,0,0,0,0,
         0,0x3a,0x1f6,0xff97,0xff98,0xff99,0xff9a,0xff9b,0xff9c,0xff9d,0xff9e,0,0,0,0,0,
         0,0x3b,0x3f9,0xff9f,0xffa0,0xffa1,0xffa2,0xffa3,0xffa4,0xffa5,0xffa6,0,0,0,0,0,
         0,0x79,0x7f7,0xffa7,0xffa8,0xffa9,0xffaa,0xffab,0xffac,0xffad,0xffae,0,0,0,0,0,
         0,0x7a,0x7f8,0xffaf,0xffb0,0xffb1,0xffb2,0xffb3,0xffb4,0xffb5,0xffb6,0,0,0,0,0,
         0,0xf9,0xffb7,0xffb8,0xffb9,0xffba,0xffbb,0xffbc,0xffbd,0xffbe,0xffbf,0,0,0,0,0,
         0,0x1f7,0xffc0,0xffc1,0xffc2,0xffc3,0xffc4,0xffc5,0xffc6,0xffc7,0xffc8,0,0,0,0,0,
         0,0x1f8,0xffc9,0xffca,0xffcb,0xffcc,0xffcd,0xffce,0xffcf,0xffd0,0xffd1,0,0,0,0,0,
         0,0x1f9,0xffd2,0xffd3,0xffd4,0xffd5,0xffd6,0xffd7,0xffd8,0xffd9,0xffda,0,0,0,0,0,
         0,0x1fa,0xffdb,0xffdc,0xffdd,0xffde,0xffdf,0xffe0,0xffe1,0xffe2,0xffe3,0,0,0,0,0,
         0,0x7f9,0xffe4,0xffe5,0xffe6,0xffe7,0xffe8,0xffe9,0xffea,0xffeb,0xffec,0,0,0,0,0,
         0,0x3fe0,0xffed,0xffee,0xffef,0xfff0,0xfff1,0xfff2,0xfff3,0xfff4,0xfff5,0,0,0,0,0,
         0x3fa,0x7fc3,0xfff6,0xfff7,0xfff8,0xfff9,0xfffa,0xfffb,0xfffc,0xfffd,0xfffe}
    };

    unsigned char DC_HUFFSIZE[2][13] = {
        {2, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, 0},
        {2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0}
    };

    unsigned char AC_HUFFSIZE[2][251] = {
        {4, 2, 2, 3, 4, 5, 7, 8, 10, 16, 16, 0, 0, 0, 0, 0, 
         0, 4, 5, 7, 9, 11, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 5, 8, 10, 12, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 6, 9, 12, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 6, 10, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 7, 12, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 8, 12, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 9, 15, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
        {2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 12, 0, 0, 0, 0, 0, 
         0, 4, 6, 8, 9, 11, 12, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 5, 8, 10, 12, 15, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 5, 8, 10, 12, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 6, 9, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 6, 10, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 
         0, 14, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 
         0, 10, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16}
    };

    for(i=0; i<8; i++)
        for(j=0; j<8; j++)
        {
            data[idx] = ptr[i*img_W+j];
            idx++;
        }

    symbol_2 = data[0];
    symbol_1 = CountBits(symbol_2);
    if(symbol_2 < 0) symbol_2 = symbol_2 - 1;    
    WriteCode(file, DC_HUFFCODE[num][symbol_1], 
				DC_HUFFSIZE[num][symbol_1], cnt);
    WriteCode(file, (unsigned short)symbol_2, symbol_1, cnt);
 
    for (k = 1; k <= 63; k++) 
    {
        if(data[k] == 0)
        {
            run_length++;
        }
        else
        {
            while(run_length>15)
            {
                symbol_1 = 0xf0;
                WriteCode(file, (unsigned short)AC_HUFFCODE[num][symbol_1],
                          AC_HUFFSIZE[num][symbol_1], cnt);
                run_length = run_length - 16;
            }
            
            size = CountBits(data[k]);

            symbol_1 = run_length * 16 + size;
            WriteCode(file, (unsigned short)AC_HUFFCODE[num][symbol_1], 
                      AC_HUFFSIZE[num][symbol_1], cnt);

            symbol_2 = data[k];
            if(symbol_2 < 0) symbol_2 = symbol_2 - 1;
            WriteCode(file, (unsigned short) symbol_2, size, cnt);

            run_length = 0;
        }
    }

    symbol_1 = 0x00;
    WriteCode(file, (unsigned short)AC_HUFFCODE[num][symbol_1], 
              AC_HUFFSIZE[num][symbol_1], cnt);
}


void CJTHImageProDoc::WriteCode(CFile * file, unsigned short code, int size, int * cnt)
{
	int i;
	unsigned char bit;
	static unsigned char ch = 0;

	for(i=0; i<size; i++)
    {
        bit = (code >> (size - 1 - i)) & 0x01; 
        bit = bit << (7 - (*cnt));
        ch = ch | bit;
        
        (*cnt)++;  

        if(*cnt == 8) 
        {
            *cnt = 0;
            file->Write(&ch, 1);

            if(ch == 0xFF) 
            {
                ch = 0;
                file->Write(&ch, 1);
			}
            ch = 0;
        }
    }
}

unsigned char CJTHImageProDoc::CountBits(short V)
{
	unsigned char Num = 0;

	if(V < 0) V = -V;
    while(V != 0)
    {
        V = V >> 1;
        Num++;
    }
    return Num; 
}

void CJTHImageProDoc::DCT_Zigzag_Quantization(short * ptr, int img_W, int tableNo)
{
	unsigned char Qtb0[64] = {16, 11, 12, 14, 12, 10, 16, 14,
                    13, 14, 18, 17, 16, 19, 24, 40,
                    26, 24, 22, 22, 24, 49, 35, 37,
                    29, 40, 58, 51, 61, 60, 57, 51,
                    56, 55, 64, 72, 92, 78, 64, 68,
                    87, 69, 55, 56, 80, 109, 81, 87,
                    95, 98, 103, 104, 103, 62, 77, 113,
                    121, 112, 100, 120, 92, 101, 103, 99};

    unsigned char Qtb1[64] ={17, 18, 18, 24, 21, 24, 47, 26,
                    26, 47, 99, 66, 56, 66, 99, 99,
                    99 ,99, 99, 99, 99, 99, 99, 99, 
                    99, 99, 99, 99, 99, 99, 99, 99, 
                    99, 99, 99, 99, 99, 99, 99, 99, 
                    99, 99, 99, 99, 99, 99, 99, 99, 
                    99, 99, 99, 99, 99, 99, 99, 99, 
                    99, 99, 99, 99, 99, 99, 99, 99    };

    int Index[64] = 
           {0,  1,  5,  6,  14, 15, 27, 28,
            2,  4,  7,  13, 16, 26, 29, 42, 
            3,  8,  12, 17, 25, 30, 41, 43, 
            9,  11, 18, 24, 31, 40, 44, 53, 
            10, 19, 23, 32, 39, 45, 52, 54, 
            20, 22, 33, 38, 46, 51, 55, 60,
            21, 34, 37, 47, 50, 56, 59, 61,
            35, 36, 48, 49, 57, 58, 62, 63};


    int i, j;
    int x, y, u, v;    
    short ZZ[64]; 
    double result;
    short Temp[64];
    int idx;

    for(u=0; u<8; u++) {
        for(v=0; v<8; v++) {
            result = 0.0;
            for(x=0; x<8; x++) {
                for(y=0; y<8; y++) {
                    result += ptr[(int)(y*img_W+x)] * 
                                            cos(((2.0*x+1.0)*u*PI)/(2.0 * 8)) * 
                                            cos(((2.0*y+1.0)*v*PI)/(2.0 * 8));
                }
            }
            result *=  2.0 / sqrt((double)(8 * 8));
            if(u == 0) result *= 1.0 / sqrt(2.0);
            if(v == 0) result *= 1.0 / sqrt(2.0);
            ZZ[(int)(v*8+u)] = (short) result;    
        }
    }

    memcpy(Temp, ZZ, 64 * sizeof(short));
    for(i=0; i<8; i++)
        for(j=0; j<8; j++)
        {
            idx = (i<<3)+j;
            ZZ[Index[idx]] = Temp[idx];
        }

    for(i=0; i<64; i++)
        if(tableNo == 1) 
            ZZ[i] = ZZ[i] / Qtb1[i];
        else
            ZZ[i] = ZZ[i] / Qtb0[i];

    for(i=0; i<8; i++)
        for(j=0; j<8; j++) 
           ptr[i*img_W+j] = ZZ[i*8+j];
}


