// CCoverageArea.cpp: 实现文件
//

#include "pch.h"
#include "SatelliteSystemX.h"
#include "CCoverageAreaDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <queue>
#include "CArea.h"
// CCoverageArea 对话框

IMPLEMENT_DYNAMIC(CCoverageAreaDlg, CDialogEx)

CCoverageAreaDlg::CCoverageAreaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COVERAGE_AREA, pParent)
	, _hour(0), _minute(0), _second(0), ans(_T("")) {
	drawOK = false;
	moving = false;
	magnification = 100;
}

CCoverageAreaDlg::~CCoverageAreaDlg()
{
	memDC.DeleteDC();
	circle = nullptr;
}

void CCoverageAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOUR, _hour);
	DDX_Text(pDX, IDC_EDIT_MINUTE, _minute);
	DDX_Text(pDX, IDC_EDIT_SECOND, _second);
	DDX_Control(pDX, IDC_CHOICE, m_cb);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Text(pDX, IDC_STATIC_RATIO, ans);
}


BEGIN_MESSAGE_MAP(CCoverageAreaDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_LOAD_AREA1, &CCoverageAreaDlg::OnBnClickedLoadArea)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_INFO, &CCoverageAreaDlg::OnBnClickedButtonInfo)
	ON_EN_CHANGE(IDC_EDIT_HOUR, &CCoverageAreaDlg::OnEnChangeEditHour)
	ON_EN_CHANGE(IDC_EDIT_MINUTE, &CCoverageAreaDlg::OnEnChangeEditMinute)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CCoverageArea 消息处理程序


BOOL CCoverageAreaDlg::OnEraseBkgnd(CDC* pDC) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CDialogEx::OnEraseBkgnd(pDC);
	return FALSE;
}


void CCoverageAreaDlg::OnPaint() {
	CPaintDC dc(this); 
	// device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	m_cb.AddString(_T("瞬时覆盖区域"));
	m_cb.AddString(_T("累计覆盖区域"));
	m_cb.AddString(_T("前一小时累计覆盖区域（快速）"));
	m_cb.AddString(_T("前二小时累计覆盖区域（快速）"));
	m_cb.AddString(_T("前三小时累计覆盖区域（快速）"));
	m_cb.SetCurSel(0);
}


void CCoverageAreaDlg::OnBnClickedLoadArea() {
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (_hour > 23 || _minute > 59 || _second > 59) {
		MessageBox(_T("输入时刻非法"));
		return;
	}
	//覆盖方格
	std::vector<CArea>covered;
	//非覆盖方格
	std::vector<CArea>none_covered;
	int T = 3600 * _hour + 60 * _minute + _second;
	if (m_cb.GetCurSel() == 1) {
		m_progress.SetRange(0, 10000);
		int pos = 0;
		//队列 四叉树BFS
		std::queue<CArea>q;
		//最初的方格入队列
		q.emplace(CArea(75, 0, 135, 55));
		while ((q.front().getLatitudet() - q.front().getLatitudeb()) * (q.front().getLongituder() - q.front().getLongitudel()) > 25) {
			CArea area = q.front();
			q.pop();
			//分割成四个区域
			q.emplace(CArea(area.getLongitudel(), area.getLatitudet(), (area.getLongitudel() + area.getLongituder()) / 2.0, (area.getLatitudeb() + area.getLatitudet()) / 2.0));
			q.emplace(CArea((area.getLongitudel() + area.getLongituder()) / 2.0, area.getLatitudet(), area.getLongituder(), (area.getLatitudeb() + area.getLatitudet()) / 2.0));
			q.emplace(CArea(area.getLongitudel(), (area.getLatitudeb() + area.getLatitudet()) / 2.0, (area.getLongitudel() + area.getLongituder()) / 2.0, area.getLatitudeb()));
			q.emplace(CArea((area.getLongitudel() + area.getLongituder()) / 2.0, (area.getLatitudeb() + area.getLatitudet()) / 2.0, area.getLongituder(), area.getLatitudeb()));
		}
		//总面积
		double totArea = 60.0 * 55.0;
		//已经确定覆盖或不覆盖的面积
		double ok = 0;
		//覆盖面积
		double yes = 0;
		while (!q.empty()) {
			//出队
			CArea area = q.front();
			q.pop();
			//遍历前t个小时
			//只要有一个时刻覆盖就算覆盖
			//4个二进制位表示四个顶点
			//0000 四个顶点都没有覆盖
			//1111 四个点都覆盖
			short flag = 0;
			for (int t = 0; t <= T; t++) {
				//在某个时刻区域被覆盖 就说明累计覆盖
				int cover = 0;
				//=============================================================================================
				//遍历九个卫星 判断顶点是否被覆盖
				//加上自己的判断覆盖函数即可
				for (int i = 0; i <= 8; i++) {
					if (circle[t][i].atSphericalCrown(CLocation(area.getLongitudel(), area.getLatitudet()))) {
						cover |= 1;
					}
					if (circle[t][i].atSphericalCrown(CLocation(area.getLongituder(), area.getLatitudet()))) {
						cover |= 2;
					}
					if (circle[t][i].atSphericalCrown(CLocation(area.getLongitudel(), area.getLatitudeb()))) {
						cover |= 4;
					}
					if (circle[t][i].atSphericalCrown(CLocation(area.getLongituder(), area.getLatitudeb()))) {
						cover |= 8;
					}
					if (cover == 15) {
						//全覆盖 判断完毕
						break;
					}
				}
				//==============================================================================================
				if (cover == 15) {
					flag = 2;
					break;
				} else if (cover > 0) {
					flag = 1;
				}
			}
			m_progress.SetPos(++pos);
			if (pos == 10000) {
				m_progress.SetPos(0);
				pos = 0;
			}
			//无覆盖
			if (flag == 0) {
				//加入无覆盖集合
				none_covered.emplace_back(area);
				ok += (area.getLatitudeb() - area.getLatitudet()) * (area.getLongituder() - area.getLongitudel());
			} else if (flag == 2) {
				//加入覆盖集合
				covered.emplace_back(area);
				ok += (area.getLatitudeb() - area.getLatitudet()) * (area.getLongituder() - area.getLongitudel());
				yes += (area.getLatitudeb() - area.getLatitudet()) * (area.getLongituder() - area.getLongitudel());
				//std::cout << yes << std::endl;
			} else {
				if ((totArea - ok) / totArea <= 0.001) {
					//未确定面积小 退出
					break;
				} else {
					//分割成四个区域
					q.emplace(CArea(area.getLongitudel(), area.getLatitudet(), (area.getLongitudel() + area.getLongituder()) / 2.0, (area.getLatitudeb() + area.getLatitudet()) / 2.0));
					q.emplace(CArea((area.getLongitudel() + area.getLongituder()) / 2.0, area.getLatitudet(), area.getLongituder(), (area.getLatitudeb() + area.getLatitudet()) / 2.0));
					q.emplace(CArea(area.getLongitudel(), (area.getLatitudeb() + area.getLatitudet()) / 2.0, (area.getLongitudel() + area.getLongituder()) / 2.0, area.getLatitudeb()));
					q.emplace(CArea((area.getLongitudel() + area.getLongituder()) / 2.0, (area.getLatitudeb() + area.getLatitudet()) / 2.0, area.getLongituder(), area.getLatitudeb()));
				}
			}
			if ((totArea - ok) / totArea <= 0.001) {
				//未确定面积小 退出
				break;
			}
		}
		ans.Format(_T("覆盖率:%.4lf%%"), yes / totArea * 100);
		m_progress.SetPos(10000);
	} else if(m_cb.GetCurSel()==0){
		//队列 四叉树BFS
		std::queue<CArea>q;
		//最初的方格入队列
		q.emplace(CArea(75, 55, 135, 0));
		while ((q.front().getLatitudet() - q.front().getLatitudeb()) * (q.front().getLongituder() - q.front().getLongitudel()) > 25) {
			CArea area = q.front();
			q.pop();
			//分割成四个区域
			q.emplace(CArea(area.getLongitudel(), area.getLatitudet(), (area.getLongitudel() + area.getLongituder()) / 2.0, (area.getLatitudeb() + area.getLatitudet()) / 2.0));
			q.emplace(CArea((area.getLongitudel() + area.getLongituder()) / 2.0, area.getLatitudet(), area.getLongituder(), (area.getLatitudeb() + area.getLatitudet()) / 2.0));
			q.emplace(CArea(area.getLongitudel(), (area.getLatitudeb() + area.getLatitudet()) / 2.0, (area.getLongitudel() + area.getLongituder()) / 2.0, area.getLatitudeb()));
			q.emplace(CArea((area.getLongitudel() + area.getLongituder()) / 2.0, (area.getLatitudeb() + area.getLatitudet()) / 2.0, area.getLongituder(), area.getLatitudeb()));
		}
		//总面积
		double totArea = 60.0 * 55.0;
		//已经确定覆盖或不覆盖的面积
		double ok = 0;
		//覆盖面积
		double yes = 0;
		while (!q.empty()) {
			//出队
			CArea area = q.front();
			q.pop();
			//4个二进制位表示四个顶点
			//0000 四个顶点都没有覆盖
			//1111 四个点都覆盖
			int cover = 0;
			//=============================================================================================
			//遍历九个卫星 判断顶点是否被覆盖
			for (int i = 0; i <= 8; i++) {
				if (circle[T][i].atSphericalCrown(CLocation(area.getLongitudel(), area.getLatitudet()))) {
					cover |= 1;
				}
				if (circle[T][i].atSphericalCrown(CLocation(area.getLongituder(), area.getLatitudet()))) {
					cover |= 2;
				}
				if (circle[T][i].atSphericalCrown(CLocation(area.getLongitudel(), area.getLatitudeb()))) {
					cover |= 4;
				}
				if (circle[T][i].atSphericalCrown(CLocation(area.getLongituder(), area.getLatitudeb()))) {
					cover |= 8;
				}
				if (cover == 15) {
					break;
				}
			}
			//==============================================================================================
			//无覆盖
			if (cover == 0) {
				//加入无覆盖集合
				none_covered.emplace_back(area);
				ok += (area.getLatitudet() - area.getLatitudeb()) * (area.getLongituder() - area.getLongitudel());
			} else if (cover == 15) {
				//加入覆盖集合
				covered.emplace_back(area);
				ok += (area.getLatitudet() - area.getLatitudeb()) * (area.getLongituder() - area.getLongitudel());
				yes += (area.getLatitudet() - area.getLatitudeb()) * (area.getLongituder() - area.getLongitudel());
			} else {
				if ((totArea - ok) / totArea <= 0.001) {
					//未确定面积小 退出
					break;
				} else {
					//分割成四个区域
					q.emplace(CArea(area.getLongitudel(), area.getLatitudet(), (area.getLongitudel() + area.getLongituder()) / 2.0, (area.getLatitudeb() + area.getLatitudet()) / 2.0));
					q.emplace(CArea((area.getLongitudel() + area.getLongituder()) / 2.0, area.getLatitudet(), area.getLongituder(), (area.getLatitudeb() + area.getLatitudet()) / 2.0));
					q.emplace(CArea(area.getLongitudel(), (area.getLatitudeb() + area.getLatitudet()) / 2.0, (area.getLongitudel() + area.getLongituder()) / 2.0, area.getLatitudeb()));
					q.emplace(CArea((area.getLongitudel() + area.getLongituder()) / 2.0, (area.getLatitudeb() + area.getLatitudet()) / 2.0, area.getLongituder(), area.getLatitudeb()));
				}
			}
			if ((totArea - ok) / totArea <= 0.001) {
				//未确定面积小 退出
				break;
			}
		}
		ans.Format(_T("覆盖率:%.4lf%%"), yes / totArea * 100);
		m_progress.SetRange(0, 1);
		m_progress.SetPos(1);
	} else if (m_cb.GetCurSel() == 2) {
		FILE* f = fopen("CoverArea/1.1.txt", "r");
		int n;
		double ratio = 0.0;
		fscanf(f, "%d %lf", &n, &ratio);
		while (n--) {
			double a, b, c, d;
			fscanf(f, "%lf%lf%lf%lf", &a, &b, &c, &d);
			covered.emplace_back(CArea(a, b, c, d));
		}
		fclose(f);
		f = fopen("CoverArea/1.2.txt", "r");
		fscanf(f, "%d", &n);
		while (n--) {
			double a, b, c, d;
			fscanf(f, "%lf%lf%lf%lf", &a, &b, &c, &d);
			none_covered.emplace_back(CArea(a, b, c, d));
		}
		fclose(f);
		f = nullptr;
		ans.Format(_T("覆盖率:%.4lf%%"), ratio * 100);
		m_progress.SetRange(0, 1);
		m_progress.SetPos(1);
	} else if (m_cb.GetCurSel() == 3) {
		FILE* f = fopen("CoverArea/2.1.txt", "r");
		int n;
		double ratio = 0.0;
		fscanf(f, "%d %lf", &n, &ratio);
		while (n--) {
			double a, b, c, d;
			fscanf(f, "%lf%lf%lf%lf", &a, &b, &c, &d);
			covered.emplace_back(CArea(a, b, c, d));
		}
		fclose(f);
		f = fopen("CoverArea/2.2.txt", "r");
		fscanf(f, "%d", &n);
		while (n--) {
			double a, b, c, d;
			fscanf(f, "%lf%lf%lf%lf", &a, &b, &c, &d);
			none_covered.emplace_back(CArea(a, b, c, d));
		}
		fclose(f);
		f = nullptr;
		ans.Format(_T("覆盖率:%.4lf%%"), ratio * 100);
		m_progress.SetRange(0, 1);
		m_progress.SetPos(1);
	} else {
		FILE* f = fopen("CoverArea/3.1.txt", "r");
		int n;
		double ratio = 0.0;
		fscanf(f, "%d %lf", &n, &ratio);
		while (n--) {
			double a, b, c, d;
			fscanf(f, "%lf%lf%lf%lf", &a, &b, &c, &d);
			covered.emplace_back(CArea(a, b, c, d));
		}
		fclose(f);
		f = fopen("CoverArea/3.2.txt", "r");
		fscanf(f, "%d", &n);
		while (n--) {
			double a, b, c, d;
			fscanf(f, "%lf%lf%lf%lf", &a, &b, &c, &d);
			none_covered.emplace_back(CArea(a, b, c, d));
		}
		fclose(f);
		f = nullptr;
		ans.Format(_T("覆盖率:%.4lf%%"), ratio * 100);
		m_progress.SetRange(0, 1);
		m_progress.SetPos(1);
	}

	CDC* pDC = GetDC();
	//建立与屏幕设备描述表（前端缓冲区）兼容的内存设备描述表句柄（后备缓冲区）
	memDC.CreateCompatibleDC(NULL);
	//这时还不能绘图，因为没有位图的设备描述表是不能绘图的
	CBitmap memBitmap;
	//下面建立一个与屏幕设备描述表（或者内存设备描述表）兼容的位图
	memBitmap.CreateCompatibleBitmap(pDC, 3000, 2750);
	//将位图选入到内存设备描述表
	//只有选入了位图的设备描述表才有地方绘图，画到指定的位图上
	memDC.SelectObject(&memBitmap);
	memDC.FillSolidRect(0, 0, 3000, 2750, RGB(255, 255, 255));
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 0, 0));
	memDC.SelectObject(&pen);
	memDC.SelectObject(&brush);
	//绘图 
	for (const CArea& area : covered) {
		memDC.Rectangle((int)((area.getLongitudel() - 75) * 50), (int)(area.getLatitudet() * 50), (int)((area.getLongituder() - 75) * 50), (int)(area.getLatitudeb() * 50));
	}
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(0, 0, 255));
	memDC.SelectObject(&brush);
	for (const CArea& area : none_covered) {
		memDC.Rectangle((int)((area.getLongitudel() - 75) * 50), (int)(area.getLatitudet() * 50), (int)((area.getLongituder() - 75) * 50), (int)(area.getLatitudeb() * 50));
	}
	// 将后备缓冲区中的图形拷贝到前端缓冲区
	// 等比例图
	pDC->BitBlt(0, 0, 600, 600, &memDC, 0, 0, SRCCOPY);
	// 缩略图
	pDC->StretchBlt(1272, 405, 216, 198, &memDC, 0, 0, 3000, 2750, SRCCOPY);
	// 缩放图
	pDC->StretchBlt(620, 0, 600, 600, &memDC, 0, 0, magnification << 1, magnification << 1, SRCCOPY);
	// 绘图完成后的清理
	pen.DeleteObject();
	brush.DeleteObject();
	pDC->DeleteDC();
	// 标记内存绘图完成
	drawOK = true;
	picturePos = CPoint(0, 0);
	UpdateData(FALSE);
}


void CCoverageAreaDlg::OnMouseMove(UINT nFlags, CPoint point) {
	if (point.x > 600 || point.y > 600 || !drawOK ) {
		return;
	}
	//拖动时，鼠标按住的位置始终光标
	if (!moving) {
		CDC* pDC = GetDC();
		//位图左上角位置加上客户区位移为当前鼠标在位图的位置
		//获取位图窗口左上角
		int nx = picturePos.x + point.x - magnification;
		nx = (std::max)(0, nx);
		int ny = point.y + picturePos.y - magnification;
		ny = (std::max)(0, ny);
		//绘制缩放区位图
		pDC->StretchBlt(620, 0, 600, 600, &memDC, nx, ny, magnification << 1, magnification << 1, SRCCOPY);
		pDC->DeleteDC();
	} else {
		CDC* pDC = GetDC();
		int nx = memPos.x - point.x;
		nx = (std::max)(0, nx);
		nx = (std::min)(2400, nx);
		int ny = memPos.y - point.y;
		ny = (std::max)(0, ny);
		ny = (std::min)(2150, ny);
		//绘制拖曳区位图
		pDC->BitBlt(0, 0, 600, 600, &memDC, nx, ny, SRCCOPY);
		//绘图完成后的清理
		pDC->DeleteDC();
		picturePos.x = nx;
		picturePos.y = ny;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CCoverageAreaDlg::OnLButtonUp(UINT nFlags, CPoint point) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	moving = false;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CCoverageAreaDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (point.x > 600 || point.y > 600) {
		return;
	}
	moving = true;
	//确定选中的位图位置
	memPos = point + picturePos;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCoverageAreaDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt) {
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (moving) {
		return;
	}
	if (zDelta > 0) {
		magnification -= 2;
	} else {
		magnification += 2;
	}
	magnification = (std::max)(1, magnification);
	CDC* pDC = GetDC();
	//位图左上角位置加上客户区位移为当前鼠标在位图的位置
	//获取位图窗口左上角
	int nx = picturePos.x + pt.x - magnification;
	nx = (std::max)(0, nx);
	int ny = pt.y + picturePos.y - magnification;
	ny = (std::max)(0, ny);
	pDC->StretchBlt(620, 0, 600, 600, &memDC, nx, ny, magnification << 1, magnification << 1, SRCCOPY);
	pDC->DeleteDC();
	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CCoverageAreaDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (moving) {
		return FALSE;
	}
	if (zDelta > 0) {
		magnification -= 2;
	} else {
		magnification += 2;
	}
	magnification = (std::max)(1, magnification);
	CDC* pDC = GetDC();
	//位图左上角位置加上客户区位移为当前鼠标在位图的位置
	//获取位图窗口左上角
	int nx = picturePos.x + pt.x - magnification;
	nx = (std::max)(0, nx);
	int ny = pt.y + picturePos.y - magnification;
	ny = (std::max)(0, ny);
	pDC->StretchBlt(620, 0, 600, 600, &memDC, nx, ny, magnification << 1, magnification << 1, SRCCOPY);
	pDC->DeleteDC();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CCoverageAreaDlg::OnBnClickedButtonInfo() {
	// TODO: 在此添加控件通知处理程序代码
	system("start infoCoverageArea.txt");
}


void CCoverageAreaDlg::OnEnChangeEditHour() {
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CCoverageAreaDlg::OnEnChangeEditMinute() {
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


int CCoverageAreaDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
}
