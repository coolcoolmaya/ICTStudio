// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// ICTStudioView.cpp : CICTStudioView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CICTStudioView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_ZOOM_SLIDER, &CICTStudioView::OnUpdateZoomSlider)
	ON_COMMAND(ID_ZOOM_SLIDER, &CICTStudioView::OnZoomSlider)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CICTStudioView 构造/析构

CICTStudioView::CICTStudioView()
{
	// TODO: 在此处添加构造代码

}

CICTStudioView::~CICTStudioView()
{
}

BOOL CICTStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CICTStudioView 绘制

void CICTStudioView::OnDraw(CDC* pDC)
{
	CICTStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	
	auto gray = pDoc->GetGrayImage();
	Gdiplus::Bitmap bitmap(gray.cols, gray.rows, gray.cols*gray.channels(), PixelFormat24bppRGB, gray.data);

	CRect rc;
	GetClientRect(rc);
	Gdiplus::Bitmap bmp(int(rc.right), int(rc.bottom));

	Gdiplus::Graphics bmpGraphics(&bmp);
	// 刷新背景
	Gdiplus::SolidBrush bkBrush(Gdiplus::Color(255, 255, 255));
	bmpGraphics.FillRectangle(&bkBrush, 0, 0, rc.right, rc.bottom);
	// 绘制图像
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
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CICTStudioView 打印


void CICTStudioView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CICTStudioView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CICTStudioView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CICTStudioView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CICTStudioView 诊断

#ifdef _DEBUG
void CICTStudioView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CICTStudioView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CICTStudioDoc* CICTStudioView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CICTStudioDoc)));
	return (CICTStudioDoc*)m_pDocument;
}
#endif //_DEBUG


// CICTStudioView 消息处理程序


void CICTStudioView::OnUpdateZoomSlider(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable();
}

void CICTStudioView::OnZoomSlider()
{
	// TODO: 在此添加命令处理程序代码
	TRACE0("on slider");

}


BOOL CICTStudioView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}
