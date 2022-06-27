
// SatelliteSystemXDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SatelliteSystemX.h"
#include "SatelliteSystemXDlg.h"
#include "afxdialogex.h"
#include "CProgressDlg.h"
#include "CTimeWindowDlg.h"
#include <string>
#include "GreatFinderDlg.h"
#include "CCoverageProportionDlg.h"
#include "CCoverageAreaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSatelliteSystemXDlg 对话框



CSatelliteSystemXDlg::CSatelliteSystemXDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SATELLITESYSTEMX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_HEAD);
}

void CSatelliteSystemXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSatelliteSystemXDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TO_TIMEWINDOW, &CSatelliteSystemXDlg::OnBnClickedToTimewindow)
	ON_WM_CLOSE()
//	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TO_GREAT_FINDER, &CSatelliteSystemXDlg::OnBnClickedToGreatFinder)
	ON_BN_CLICKED(IDC_To_COVERAGE_PROPORTION, &CSatelliteSystemXDlg::OnBnClickedToCoverageProportion)
	ON_BN_CLICKED(IDC_TO_COVERAGE_AREA, &CSatelliteSystemXDlg::OnBnClickedToCoverageArea)
END_MESSAGE_MAP()


// CSatelliteSystemXDlg 消息处理程序

BOOL CSatelliteSystemXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	getCircles();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSatelliteSystemXDlg::getCircles() {
	circle = new CCircle*[86401];
	for (int t = 0; t <= 86400; t++) {
		circle[t] = new CCircle[9];
	}
	// 创建进度条窗口
	CProgressDlg* progressDlg = new CProgressDlg;
	progressDlg->Create(IDD_PROGRESS_INIT, this);
	progressDlg->ShowWindow(SW_SHOW);
	progressDlg->m_progress.SetRange(0, 28800);
	progressDlg->m_progress.SetPos(0);
	progressDlg->UpdateWindow();
	progressDlg->opt.Format(_T("打开文件……"));
	progressDlg->UpdateData(FALSE);
	// 读取文件
	FILE* p_file[9] = { nullptr };
	for (int i = 0; i <= 8; i++) {
		std::string _FileName = "SatCoverInfo_";
		_FileName.push_back(i + '0');
		_FileName += "x.txt";
		p_file[i] = fopen(_FileName.c_str(), "r");
	}
	progressDlg->opt.Format(_T("读取圆周坐标，处理区域圆……"));
	progressDlg->UpdateData(FALSE);
	for (int t = 0; t <= 86400; t++) {
		for (int i = 0; i <= 8; i++) {
			CLocation* point = new CLocation[3];
			for (int _ = 0; _ <= 2; _++) {
				//读三行 确定一个圆
				double longitude = 0.0, latitude = 0.0;
				fscanf(p_file[i], "%lf %lf", &longitude, &latitude);
				point[_] = CLocation(longitude, latitude);
			}
			circle[t][i] = CCircle(point[0], point[1], point[2]);
			delete[]point;
			point = nullptr;
		}
		progressDlg->num_progress.Format(_T("%.1lf%%"), (t + 1) / 3 / 288.0);
		progressDlg->UpdateData(FALSE);
		progressDlg->m_progress.SetPos((t + 1) / 3);
	}
	progressDlg->opt.Format(_T("关闭文件……"));
	progressDlg->UpdateWindow();

	for (int i = 0; i <= 8; i++) {
		fclose(p_file[i]);
	}
	progressDlg->DestroyWindow();
}

void CSatelliteSystemXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSatelliteSystemXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	//使用CImage的Load、Draw函数显示图像
	CImage image;
	image.Load(_T("satellite.png"));

	//获取控件的矩形
	CRect rectControl;  //控件矩形对象
	CWnd* pWnd = GetDlgItem(IDC_IMAGE); //Picture Control的ID为IDC_IMAGE
	pWnd->GetClientRect(&rectControl);

	//以控件为画布，在其上画图
	CDC* pDc = GetDlgItem(IDC_IMAGE)->GetDC();
	//绘图前必须调用此函数（设置缩放模式），否则失真严重
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);

	//将图片绘制到Picture控件表示的矩形区域
	image.Draw(pDc->m_hDC, rectControl);   
	image.Destroy();
	pWnd->ReleaseDC(pDc);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSatelliteSystemXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSatelliteSystemXDlg::OnBnClickedToTimewindow()
{
	// TODO: 在此添加控件通知处理程序代码
	CTimeWindowDlg dlg;
	dlg.circle = circle;
	dlg.DoModal();
}


void CSatelliteSystemXDlg::OnClose() {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 释放内存
	for (int t = 0; t <= 86400; t++) {
		delete[]circle[t];
		circle[t] = nullptr;
	}
	delete[]circle;
	circle = nullptr;
	CDialogEx::OnClose();
}


//int CSatelliteSystemXDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) {
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  在此添加您专用的创建代码
//	return 0;
//}


void CSatelliteSystemXDlg::OnBnClickedToGreatFinder() {
	// TODO: 在此添加控件通知处理程序代码
	GreatFinderDlg dlg;
	dlg.circle = circle;
	dlg.DoModal();
}


void CSatelliteSystemXDlg::OnBnClickedToCoverageProportion() {
	// TODO: 在此添加控件通知处理程序代码
	CCoverageProportionDlg dlg;
	dlg.circle = circle;
	dlg.DoModal();
}

void CSatelliteSystemXDlg::OnBnClickedToCoverageArea() {
	// TODO: 在此添加控件通知处理程序代码
	CCoverageAreaDlg dlg;
	dlg.circle = circle;
	dlg.DoModal();
}
