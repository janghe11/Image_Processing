
// JTHImageProDoc.h : CJTHImageProDoc Ŭ������ �������̽�
//


#pragma once


class CJTHImageProDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CJTHImageProDoc();
	DECLARE_DYNCREATE(CJTHImageProDoc)

// Ư���Դϴ�.
public:
	unsigned char **inputImg;
	unsigned char **inputImg2;
	unsigned char **resultImg;
	int imageWidth;
	int imageHeight;
	int depth;

	int gImageWidth;
	int gImageHeight;
	unsigned char **gResultImg;
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CJTHImageProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void PixelAdd(void);
	void PixelSub(void);
	void PixelHistoEq(void);
	void PixelTwoImageAdd(void);
	void LoadTwoImages(void);
	void LoadImageFile(CArchive& ar);
	void LoadSecondImageFile(CArchive& ar);
	void RegionSharpening(void);
	void Convolve(unsigned char ** inputImg, unsigned char ** resultImg, int cols, int rows, float mask[][3], int bias, int depth);
	void RegionSobel(void);
	void RegionMedian(void);
	void Erosion(void);
	void Dilation(void);
	void Opening(void);
	void CopyResultToInput(void);
	void Closing(void);
	void GeometryZoominPixelCopy(void);
	void GeometryZoominInterpolation(void);
};
