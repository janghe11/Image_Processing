
// JTHImageProView.h : CJTHImageProView Ŭ������ �������̽�
//

#pragma once


class CJTHImageProView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CJTHImageProView();
	DECLARE_DYNCREATE(CJTHImageProView)

// Ư���Դϴ�.
public:
	CJTHImageProDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CJTHImageProView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // JTHImageProView.cpp�� ����� ����
inline CJTHImageProDoc* CJTHImageProView::GetDocument() const
   { return reinterpret_cast<CJTHImageProDoc*>(m_pDocument); }
#endif

