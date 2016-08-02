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
	Gdiplus::Bitmap bitmap(gray.cols, gray.rows, gray.cols*gray.channels(), PixelFormat24bppRGB, gray.data);

	CRect rc;
	GetClientRect(rc);
	Gdiplus::Bitmap bmp(int(rc.right), int(rc.bottom));

	Gdiplus::Graphics bmpGraphics(&bmp);
	// ˢ�±���
	Gdiplus::SolidBrush bkBrush(Gdiplus::Color(255, 255, 255));
	bmpGraphics.FillRectangle(&bkBrush, 0, 0, rc.right, rc.bottom);
	// ����ͼ��
	bmpGraphics.DrawImage(&bitmap, 0, 0);

	/*Important! Create a CacheBitmap object for quick drawing*/
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	Gdiplus::CachedBitmap cachedBmp(&bmp, &graphics);
	graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
}

void CICTStudioView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
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

	return TRUE;
}
