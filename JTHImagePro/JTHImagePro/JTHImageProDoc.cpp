
// JTHImageProDoc.cpp : CJTHImageProDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CJTHImageProDoc ����/�Ҹ�

CJTHImageProDoc::CJTHImageProDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CJTHImageProDoc::~CJTHImageProDoc()
{
}

BOOL CJTHImageProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CJTHImageProDoc serialization

void CJTHImageProDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		ar.Write(resultImg, 256 * 256);
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		CFile *fp = ar.GetFile();
		if(fp->GetLength() == 256 * 256) ar.Read(inputImg, 256 * 256);
		else AfxMessageBox("256X256 ũ���� ���ϸ� ��밡���մϴ�.");
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CJTHImageProDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CJTHImageProDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CJTHImageProDoc ����

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


// CJTHImageProDoc ���


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
