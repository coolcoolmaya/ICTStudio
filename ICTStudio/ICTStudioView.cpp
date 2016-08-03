// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// ICTStudioView.cpp : CICTStudioView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ICTStudio.h"
#endif

#include "ICTStudioDoc.h"
#include "ICTStudioView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CICTStudioView

IMPLEMENT_DYNCREATE(CICTStudioView, CScrollView)

BEGIN_MESSAGE_MAP(CICTStudioView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CICTStudioView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_ZOOM_SLIDER, &CICTStudioView::OnUpdateZoomSlider)
	ON_COMMAND(ID_ZOOM_SLIDER, &CICTStudioView::OnZoomSlider)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CICTStudioView ����/����

CICTStudioView::CICTStudioView()
{
	// TODO: �ڴ˴���ӹ������

}

CICTStudioView::~CICTStudioView()
{
}

BOOL CICTStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CICTStudioView ����

void CICTStudioView::OnDraw(CDC* pDC)
{
	CICTStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	
	auto gray = pDoc->GetGrayImage();
	int lineBytes = (gray.step*8 + 31) / 32 * 4;
	int depth = gray.depth();
	int nElemSize = gray.elemSize();
	int nStep = gray.step;
	int nChannels = gray.channels();

	CPoint scrollPt = GetScrollPosition();
	int nScrollH = GetScrollPos(SB_HORZ);
	int nScrollV = GetScrollPos(SB_VERT);
	// ��ȡ��ͼ����
	CRect rc;
	GetClientRect(rc);

	// ������ͼ����bitmap
	Gdiplus::Bitmap bkgBmp(int(rc.right), int(rc.bottom));
	Gdiplus::Graphics bmpGraphics(&bkgBmp);
	
	if (CV_8U == gray.depth())
	{
		Gdiplus::Bitmap bitmap(gray.cols, gray.rows, lineBytes, PixelFormat8bppIndexed, gray.data);

		// �޸ĵ�ɫ��Ϊ256���ҽ׵�ɫ��
		int nPaletteSize = bitmap.GetPaletteSize();
		Gdiplus::ColorPalette* pPalette = reinterpret_cast<Gdiplus::ColorPalette*>(new BYTE[nPaletteSize]);
		bitmap.GetPalette(pPalette, nPaletteSize);
		for (UINT i = 0; i < pPalette->Count; ++i)
			pPalette->Entries[i] = Gdiplus::Color::MakeARGB(255, i, i, i);
		pPalette->Flags = Gdiplus::PaletteFlagsGrayScale;
		bitmap.SetPalette(pPalette);
		delete pPalette;

		// ����ͼ��
		bmpGraphics.DrawImage(&bitmap, 0, 0, scrollPt.x, scrollPt.y
			, __min(bitmap.GetWidth() - scrollPt.x, rc.Width())
			, __min(bitmap.GetHeight() - scrollPt.y, rc.Height())
			, Gdiplus::UnitPixel);
	
	}
	/*Important! Create a CacheBitmap object for quick drawing*/
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	Gdiplus::CachedBitmap cachedBmp(&bkgBmp, &graphics);
	graphics.DrawCachedBitmap(&cachedBmp, scrollPt.x, scrollPt.y);
}

void CICTStudioView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal(100, 100);
	// TODO: �������ͼ�ĺϼƴ�С
	
	CICTStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc)
		sizeTotal = pDoc->GetDocSize();
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CICTStudioView ��ӡ


void CICTStudioView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CICTStudioView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CICTStudioView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CICTStudioView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CICTStudioView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CICTStudioView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CICTStudioView ���

#ifdef _DEBUG
void CICTStudioView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CICTStudioView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CICTStudioDoc* CICTStudioView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CICTStudioDoc)));
	return (CICTStudioDoc*)m_pDocument;
}
#endif //_DEBUG


// CICTStudioView ��Ϣ�������


void CICTStudioView::OnUpdateZoomSlider(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable();
}

void CICTStudioView::OnZoomSlider()
{
	// TODO: �ڴ���������������
	TRACE0("on slider");

}


BOOL CICTStudioView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBrush br(GetSysColor(COLOR_WINDOW));
	FillOutsideRect(pDC, &br);
	return TRUE;
}


void CICTStudioView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: �ڴ����ר�ô����/����û���
	CICTStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc)
	{
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		ResizeParentToFit(FALSE);
	}

}
