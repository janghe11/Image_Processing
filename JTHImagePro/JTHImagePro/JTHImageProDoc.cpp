
// JTHImageProDoc.cpp : CJTHImageProDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CJTHImageProDoc ����/�Ҹ�

CJTHImageProDoc::CJTHImageProDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CJTHImageProDoc serialization

void CJTHImageProDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		LoadImageFile(ar);
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CJTHImageProDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CJTHImageProDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CJTHImageProDoc ����

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


// CJTHImageProDoc ���


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
			AfxMessageBox("256X256ũ���� ���ϸ� ��밡���մϴ�.");
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
			AfxMessageBox("256X256ũ���� ���ϸ� ��밡���մϴ�.");
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
