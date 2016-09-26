
// JTHImageProDoc.cpp : CJTHImageProDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "JTHImagePro.h"
#endif

#include "JTHImageProDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CJTHImageProDoc

IMPLEMENT_DYNCREATE(CJTHImageProDoc, CDocument)

BEGIN_MESSAGE_MAP(CJTHImageProDoc, CDocument)
END_MESSAGE_MAP()


// CJTHImageProDoc 생성/소멸

CJTHImageProDoc::CJTHImageProDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CJTHImageProDoc::~CJTHImageProDoc()
{
}

BOOL CJTHImageProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CJTHImageProDoc serialization

void CJTHImageProDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		ar.Write(resultImg, 256 * 256);
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		CFile *fp = ar.GetFile();
		if(fp->GetLength() == 256 * 256) ar.Read(inputImg, 256 * 256);
		else AfxMessageBox("256X256 크기의 파일만 사용가능합니다.");
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CJTHImageProDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CJTHImageProDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CJTHImageProDoc 진단

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


// CJTHImageProDoc 명령


void CJTHImageProDoc::PixelAdd(void)
{
	int value;

	for(int y = 0; y < 256; y++)
		for(int x = 0; x < 256; x++) {
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
	float N = 256 * 256;
	int hist[256], sum[256];
	
	for(k = 0; k < 256; k++) hist[k] = 0;

	for(y = 0; y < 256; y++)
		for(x = 0; x < 256; x++) {
			k = inputImg[y][x];
			hist[k] = hist[k] + 1;
		}

	for(i = 0; i < 256; i++) {
		acc_hist = acc_hist + hist[i];
		sum[i] = acc_hist;
	}

	for(y = 0; y < 256; y++)
		for(x = 0; x < 256; x++) {
			k = inputImg[y][x];
			resultImg[y][x] = (unsigned char)(sum[k] / N * 255);
		}
}


void CJTHImageProDoc::PixelTwoImageAdd(void)
{
	int value;

	LoadTwoImages();

	for(int y = 0; y < 256; y++)
		for(int x = 0; x < 256; x++) {
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
		file.Read(inputImg, 256 * 256);
		file.Close();
	}

	AfxMessageBox("Select the Second Image");

	if(dlg.DoModal()==IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		file.Read(inputImg2, 256 * 256);
		file.Close();
	}
}
