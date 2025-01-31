
// JTHImageProView.cpp : CJTHImageProView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "JTHImagePro.h"
#endif

#include "JTHImageProDoc.h"
#include "JTHImageProView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TWO_IMAGES        1
#define THREE_IMAGES	  2
#define TWO_IMAGES_SCALED 4
#define MORPHING		  8


// CJTHImageProView

IMPLEMENT_DYNCREATE(CJTHImageProView, CScrollView)

BEGIN_MESSAGE_MAP(CJTHImageProView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CJTHImageProView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PIXEL_ADD, &CJTHImageProView::OnPixelAdd)
	ON_COMMAND(ID_PIXEL_SUB, &CJTHImageProView::OnPixelSub)
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &CJTHImageProView::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &CJTHImageProView::OnPixelTwoImageAdd)
	ON_COMMAND(ID_REGION_SHARPENING, &CJTHImageProView::OnRegionSharpening)
	ON_COMMAND(ID_REGION_SOBEL, &CJTHImageProView::OnRegionSobel)
	ON_COMMAND(ID_REGION_MEDIAN, &CJTHImageProView::OnRegionMedian)
	ON_COMMAND(ID_EROSION, &CJTHImageProView::OnErosion)
	ON_COMMAND(ID_DILATION, &CJTHImageProView::OnDilation)
	ON_COMMAND(ID_OPENING, &CJTHImageProView::OnOpening)
	ON_COMMAND(ID_CLOSING, &CJTHImageProView::OnClosing)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &CJTHImageProView::OnGeometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_INTERPOLATION, &CJTHImageProView::OnGeometryZoominInterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CJTHImageProView::OnGeometryZoomoutSubsampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_AVG, &CJTHImageProView::OnGeometryZoomoutAvg)
	ON_COMMAND(ID_GEOMETRY_ROTATE, &CJTHImageProView::OnGeometryRotate)
	ON_COMMAND(ID_GEOMETRY_MIRROR, &CJTHImageProView::OnGeometryMirror)
	ON_COMMAND(ID_GEOMETRY_FLIP, &CJTHImageProView::OnGeometryFlip)
	ON_COMMAND(ID_GEOMETRY_WARPING, &CJTHImageProView::OnGeometryWarping)
	ON_COMMAND(ID_GEOMETRY_MORPHING, &CJTHImageProView::OnGeometryMorphing)
	ON_COMMAND(ID_DCT, &CJTHImageProView::OnDCT)
	ON_COMMAND(ID_FastDCT, &CJTHImageProView::OnFastDCT)
	ON_COMMAND(ID_FastIDCT, &CJTHImageProView::OnFastIDCT)
	ON_COMMAND(ID_SAVE_JPEG, &CJTHImageProView::OnSaveJpeg)
END_MESSAGE_MAP()

// CJTHImageProView 생성/소멸

CJTHImageProView::CJTHImageProView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CJTHImageProView::~CJTHImageProView()
{
}

BOOL CJTHImageProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CJTHImageProView 그리기

void CJTHImageProView::OnDraw(CDC* pDC)
{
	CJTHImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	/*
	if (!pDoc)
		return;
	*/
	if(pDoc->depth == 1) {
		for(int y = 0; y < pDoc->imageHeight; y++)
			for(int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
										pDoc->inputImg[y][x], 
										pDoc->inputImg[y][x]));

		if(viewMode == THREE_IMAGES){
			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0;x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
					RGB(pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x]));

			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
					RGB(pDoc->resultImg[y][x],
						pDoc->resultImg[y][x],
						pDoc->resultImg[y][x]));
		}
		else if(viewMode == TWO_IMAGES_SCALED) {
			for(int y = 0; y < pDoc->gImageHeight; y++)
				for(int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
							RGB(pDoc->gResultImg[y][x],
								pDoc->gResultImg[y][x],
								pDoc->gResultImg[y][x]));
		}
		else if(viewMode == MORPHING) {
			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
							  RGB(pDoc->inputImg2[y][x],
								  pDoc->inputImg2[y][x],
								  pDoc->inputImg2[y][x]));

			for(int i = 0; i< 10; i++)
				for(int y = 0; y < pDoc->imageHeight; y++)
					for(int x = 0; x < pDoc->imageWidth; x++)
						pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
								  RGB(pDoc->morphedImg[i][y][x],
									  pDoc->morphedImg[i][y][x],
									  pDoc->morphedImg[i][y][x]));
		}
		else{
			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
					RGB(pDoc->resultImg[y][x],
						pDoc->resultImg[y][x],
						pDoc->resultImg[y][x]));
		}
	}
	else if(pDoc->depth == 3){
		for(int y = 0; y < pDoc->imageHeight; y++)
			for(int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][3 * x],
										pDoc->inputImg[y][3 * x + 1], 
										pDoc->inputImg[y][3 * x + 2]));

		if(viewMode == THREE_IMAGES){
			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
					RGB(pDoc->inputImg2[y][3 * x],
						pDoc->inputImg2[y][3 * x + 1],
						pDoc->inputImg2[y][3 * x + 2]));

			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
					RGB(pDoc->resultImg[y][3 * x],
						pDoc->resultImg[y][3 * x + 1],
						pDoc->resultImg[y][3 * x + 2]));
		}
		else if(viewMode == TWO_IMAGES_SCALED) {
			for(int y = 0; y < pDoc->gImageHeight; y++)
				for(int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
							  RGB(pDoc->gResultImg[y][3 * x],
								  pDoc->gResultImg[y][3 * x + 1],
								  pDoc->gResultImg[y][3 * x + 2]));
		}
		else{
			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
					RGB(pDoc->resultImg[y][3 * x],
						pDoc->resultImg[y][3 * x + 1],
						pDoc->resultImg[y][3 * x + 2]));
		}
	}

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	/*
	for(int y = 0; y < 256; y++)
		for(int x = 0; x < 256; x++)
			pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
									pDoc->inputImg[y][x],
									pDoc->inputImg[y][x]));

	if(viewMode == THREE_IMAGES) {
		for(int y = 0; y < 256; y++)
			for(int x = 0; x < 256; x++)
				pDC->SetPixel(x + 300, y, RGB(pDoc->inputImg2[y][x],
											  pDoc->inputImg2[y][x],
											  pDoc->inputImg2[y][x]));
		for(int y = 0; y < 256; y++)
			for(int x = 0; x < 256; x++)
				pDC->SetPixel(x + 600, y, RGB(pDoc->resultImg[y][x],
											  pDoc->resultImg[y][x],
											  pDoc->resultImg[y][x]));
	}
	else {
		for(int y = 0; y < 256; y++)
			for(int x = 0; x < 256; x++)
				pDC->SetPixel(x + 300, y, RGB(pDoc->resultImg[y][x],
											  pDoc->resultImg[y][x],
											  pDoc->resultImg[y][x]));
	}
	*/
}

void CJTHImageProView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = 2048;
	sizeTotal.cy = 1024;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CJTHImageProView 인쇄


void CJTHImageProView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CJTHImageProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CJTHImageProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CJTHImageProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CJTHImageProView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CJTHImageProView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CJTHImageProView 진단

#ifdef _DEBUG
void CJTHImageProView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CJTHImageProView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CJTHImageProDoc* CJTHImageProView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJTHImageProDoc)));
	return (CJTHImageProDoc*)m_pDocument;
}
#endif //_DEBUG


// CJTHImageProView 메시지 처리기


void CJTHImageProView::OnPixelAdd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->PixelAdd();

	viewMode = TWO_IMAGES;

	Invalidate(FALSE);
}


void CJTHImageProView::OnPixelSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PixelSub();

	viewMode = TWO_IMAGES;

	Invalidate(FALSE);
}


void CJTHImageProView::OnPixelHistoEq()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PixelHistoEq();

	viewMode = TWO_IMAGES;

	Invalidate(FALSE);
}


void CJTHImageProView::OnPixelTwoImageAdd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PixelTwoImageAdd();

	viewMode = THREE_IMAGES;

	Invalidate(FALSE);
}


void CJTHImageProView::OnRegionSharpening()
{
	// TODO: Add your command handler code here
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->RegionSharpening();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnRegionSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->RegionSobel();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnRegionMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->RegionMedian();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->Erosion();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->Dilation();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnOpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->Opening();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->Closing();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryZoominPixelCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominPixelCopy();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryZoominInterpolation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominInterpolation();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryZoomoutSubsampling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryZoomoutSubsampling();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryZoomoutAvg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryZoomoutAvg();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryRotate();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryMirror()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryMirror();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryFlip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryFlip();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryWarping()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryWarping();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryMorphing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->GeometryMorphing();
	viewMode = MORPHING;
	Invalidate(FALSE);
}


void CJTHImageProView::OnDCT()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->DCT();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnFastDCT()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->FastDCT();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnFastIDCT()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->FastIDCT();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnSaveJpeg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->SaveJpeg();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}
