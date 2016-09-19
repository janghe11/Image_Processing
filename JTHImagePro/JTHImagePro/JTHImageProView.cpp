
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


// CJTHImageProView

IMPLEMENT_DYNCREATE(CJTHImageProView, CScrollView)

BEGIN_MESSAGE_MAP(CJTHImageProView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CJTHImageProView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
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
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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
