
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


// CJTHImageProView

IMPLEMENT_DYNCREATE(CJTHImageProView, CScrollView)

BEGIN_MESSAGE_MAP(CJTHImageProView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CJTHImageProView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
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
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	for(int y = 0; y < 256; y++)
		for(int x = 0; x < 256; x++)
			pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
									pDoc->inputImg[y][x],
									pDoc->inputImg[y][x]));
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
