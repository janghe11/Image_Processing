
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
	int viewMode;

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
public:
	afx_msg void OnPixelAdd();
	afx_msg void OnPixelSub();
	afx_msg void OnPixelHistoEq();
	void PixelHistoEq(void);
	afx_msg void OnPixelTwoImageAdd();
	afx_msg void OnRegionSharpening();
	afx_msg void OnRegionSobel();
	afx_msg void OnRegionMedian();
	afx_msg void OnErosion();
	afx_msg void OnDilation();
	afx_msg void OnOpening();
	afx_msg void OnClosing();
	afx_msg void OnGeometryZoominPixelCopy();
	afx_msg void OnGeometryZoominInterpolation();
	afx_msg void OnGeometryZoomoutSubsampling();
	afx_msg void OnGeometryZoomoutAvg();
	afx_msg void OnGeometryRotate();
	afx_msg void OnGeometryMirror();
	afx_msg void OnGeometryFlip();
	afx_msg void OnGeometryWarping();
	afx_msg void OnGeometryMorphing();
};

#ifndef _DEBUG  // JTHImageProView.cpp�� ����� ����
inline CJTHImageProDoc* CJTHImageProView::GetDocument() const
   { return reinterpret_cast<CJTHImageProDoc*>(m_pDocument); }
#endif

