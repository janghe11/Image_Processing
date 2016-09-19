
// HelloView.cpp : CHelloView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Hello.h"
#endif

#include "HelloDoc.h"
#include "HelloView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHelloView

IMPLEMENT_DYNCREATE(CHelloView, CView)

BEGIN_MESSAGE_MAP(CHelloView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHelloView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CHelloView ����/�Ҹ�

CHelloView::CHelloView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CHelloView::~CHelloView()
{
}

BOOL CHelloView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CHelloView �׸���

void CHelloView::OnDraw(CDC* pDC)
{
	CHelloDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	pDC->TextOut(100, 100, "ù ��° ����ó�� ���α׷� �����Դϴ� ");
}


// CHelloView �μ�


void CHelloView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHelloView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CHelloView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CHelloView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CHelloView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHelloView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHelloView ����

#ifdef _DEBUG
void CHelloView::AssertValid() const
{
	CView::AssertValid();
}

void CHelloView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHelloDoc* CHelloView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHelloDoc)));
	return (CHelloDoc*)m_pDocument;
}
#endif //_DEBUG


// CHelloView �޽��� ó����
