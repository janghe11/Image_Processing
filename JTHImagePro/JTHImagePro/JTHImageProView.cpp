
// JTHImageProView.cpp : CJTHImageProView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CJTHImageProView

IMPLEMENT_DYNCREATE(CJTHImageProView, CScrollView)

BEGIN_MESSAGE_MAP(CJTHImageProView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
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
END_MESSAGE_MAP()

// CJTHImageProView ����/�Ҹ�

CJTHImageProView::CJTHImageProView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CJTHImageProView::~CJTHImageProView()
{
}

BOOL CJTHImageProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CJTHImageProView �׸���

void CJTHImageProView::OnDraw(CDC* pDC)
{
	CJTHImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;

	if(pDoc->depth == 1) {
		for(int y = 0; y < pDoc->imageHeight; y++)
			for(int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
				pDoc->inputImg[y][x], pDoc->inputImg[y][x]));

		if(viewMode == THREE_IMAGES){
			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0;x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
					RGB(pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x]));

			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0;x < pDoc->imageWidth; x++)
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
		else{
			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0;x < pDoc->imageWidth; x++)
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
				pDoc->inputImg[y][3 * x + 1], pDoc->inputImg[y][3 * x + 2]));

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
		else{
			for(int y = 0; y < pDoc->imageHeight; y++)
				for(int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
					RGB(pDoc->resultImg[y][3 * x],
						pDoc->resultImg[y][3 * x + 1],
						pDoc->resultImg[y][3 * x + 2]));
		}
	}

	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = 2048;
	sizeTotal.cy = 1024;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CJTHImageProView �μ�


void CJTHImageProView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CJTHImageProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CJTHImageProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CJTHImageProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CJTHImageProView ����

#ifdef _DEBUG
void CJTHImageProView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CJTHImageProView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CJTHImageProDoc* CJTHImageProView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJTHImageProDoc)));
	return (CJTHImageProDoc*)m_pDocument;
}
#endif //_DEBUG


// CJTHImageProView �޽��� ó����


void CJTHImageProView::OnPixelAdd()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PixelSub();

	viewMode = TWO_IMAGES;

	Invalidate(FALSE);
}


void CJTHImageProView::OnPixelHistoEq()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PixelHistoEq();

	viewMode = TWO_IMAGES;

	Invalidate(FALSE);
}


void CJTHImageProView::OnPixelTwoImageAdd()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->RegionSobel();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnRegionMedian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->RegionMedian();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnErosion()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->Erosion();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnDilation()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->Dilation();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnOpening()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->Opening();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnClosing()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->Closing();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryZoominPixelCopy()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominPixelCopy();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CJTHImageProView::OnGeometryZoominInterpolation()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CJTHImageProDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominInterpolation();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}
