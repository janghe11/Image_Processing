
// JTHImageProDoc.h : CJTHImageProDoc 클래스의 인터페이스
//


#pragma once


class CJTHImageProDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CJTHImageProDoc();
	DECLARE_DYNCREATE(CJTHImageProDoc)

// 특성입니다.
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

	unsigned char **morphedImg[10];

	//complex_num **fft_result;
	//complex_num **ifft_result;

	double **dct_result;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CJTHImageProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
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
	void GeometryZoomoutAvg(void);
	void GeometryRotate(void);
	void GeometryMirror(void);
	void GeometryFlip(void);
	void GeometryZoomoutSubsampling(void);
	void GeometryWarping(void);
	void GeometryMorphing(void);
	void DCT(void);
	void FastDCT(void);
	void DCT_1D(double * in, double * out, const int count);
	void FastIDCT(void);
	void IDCT_1D(double * in, double * out, const int count);
	void SaveJpeg(void);
	void WriteSOI(CFile * file);
	void WriteDQT(CFile * file);
	void WriteDHT(CFile * file);
	void WriteSOF(CFile * file, int Width, int Height);
	void WriteSOS(CFile * file);
	void DCT_Zigzag_Quantization(short * ptr, int img_W, int tableNo);
	void Encode(CFile * file, short * ptr, int num, int img_W, int * cnt);
	void WriteCode(CFile * file, unsigned short code, int size, int * cnt);
	void WriteEOI(CFile * file);
	unsigned char CountBits(short V);
};
