// CInstantaneousCoverage.cpp: 实现文件
//

#include "pch.h"
#include "SatelliteSystemX.h"
#include "CCoverageProportionDlg.h"
#include "afxdialogex.h"
#include <vector>
#include "CArea.h"
#include <queue>
#include <string>
#include "CSeries.h"

// CInstantaneousCoverage 对话框

IMPLEMENT_DYNAMIC(CCoverageProportionDlg, CDialogEx)

CCoverageProportionDlg::CCoverageProportionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COVERAGE_PROPORTION, pParent)
	, progressNum1(_T("0.00%")), _hour(0), _minute(0), _second(0), ans1(_T("")), ans2(_T("")) {

}

CCoverageProportionDlg::~CCoverageProportionDlg()
{
	circle = nullptr;
}

void CCoverageProportionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RES, m_chart);
	DDX_Control(pDX, IDC_PROGRESS_COVER, m_progress1);
	DDX_Text(pDX, IDC_PROGRESS_NUM, progressNum1);
	DDX_Control(pDX, IDC_PROGRESS_RATIO, m_progress2);
	DDX_Text(pDX, IDC_EDIT_HOUR, _hour);
	DDX_Text(pDX, IDC_EDIT_MINUTE, _minute);
	DDX_Text(pDX, IDC_EDIT_SECOND, _second);
	DDX_Text(pDX, IDC_ANS1, ans1);
	DDX_Text(pDX, IDC_ANS2, ans2);
}


BEGIN_MESSAGE_MAP(CCoverageProportionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CURVE_COVERAGE, &CCoverageProportionDlg::OnBnClickedCurveCoverage)
	ON_BN_CLICKED(IDC_LOAD_COVERAGE, &CCoverageProportionDlg::OnBnClickedLoadCoverage)
	ON_BN_CLICKED(IDC_INFO, &CCoverageProportionDlg::OnBnClickedInfo)
	ON_BN_CLICKED(IDC_IN, &CCoverageProportionDlg::OnBnClickedIn)
	ON_BN_CLICKED(IDC_IN, &CCoverageProportionDlg::OnBnClickedIn)
END_MESSAGE_MAP()


// CInstantaneousCoverage 消息处理程序


void CCoverageProportionDlg::OnBnClickedCurveCoverage() {
	// TODO: 在此添加控件通知处理程序代码
	CSeries series = (CSeries)m_chart.Series(0);
	series.Clear();
	m_progress1.SetRange(0, 28800);
	double* ans = new double[86401];
	for (int t = 0; t <= 86400; t++) {
		//队列 四叉树BFS
		std::queue<CArea>q;
		//覆盖方格
		std::vector<CArea>covered;
		//非覆盖方格
		std::vector<CArea>none_covered;
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
				if ((totArea - ok) / totArea <= 0.01) {
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
			if ((totArea - ok) / totArea <= 0.01) {
				//未确定面积小 退出
				break;
			}
		}
		m_progress1.SetPos((t + 1) / 3);
		ans[t] = yes / totArea * 100;
		progressNum1.Format(_T("%.2lf%%"), (t + 1) / 3 / 288.0);
		UpdateData(FALSE);
		if (t % 3200 == 0 && t) {
			for (int i = t - 3200; i <= t; i++) {
				auto toTraditionalTime = [](int timeValue) {
					std::string res;
					int hour = timeValue / 3600;
					int minute = timeValue % 3600 / 60;
					int second = timeValue % 3600 % 60;
					if (hour < 10) {
						res += '0';
					}
					res += std::to_string(hour);
					res += ':';
					if (minute < 10) {
						res += '0';
					}
					res += std::to_string(minute);
					res += ':';
					if (second < 10) {
						res += '0';
					}
					res += std::to_string(second);
					return res;
				};
				series.Add(ans[i], CString(toTraditionalTime(i).c_str()), RGB(255, 0, 0));
			}
			UpdateWindow();
		}
	}
	delete[]ans;
	ans = nullptr;
}

void CCoverageProportionDlg::OnBnClickedLoadCoverage() {
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (_hour > 23 || _minute > 59 || _second > 59) {
		MessageBox(_T("输入时刻非法"));
		return;
	}
	int T = 3600 * _hour + 60 * _minute + _second;
	m_progress2.SetRange(0, 10000);
	int pos = 1;
	m_progress2.SetPos(0);
	//代码段1 瞬时覆盖率
	{
		std::queue<CArea>q;
		//覆盖方格
		std::vector<CArea>covered;
		//非覆盖方格
		std::vector<CArea>none_covered;
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
				if ((totArea - ok) / totArea <= 0.01) {
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
			if ((totArea - ok) / totArea <= 0.01) {
				//未确定面积小 退出
				break;
			}
		}
		ans2.Format(_T("%.4lf%%"), yes / totArea);
	}
	m_progress2.SetPos(1);
	//代码段2 累计覆盖率
	{
		//队列 四叉树BFS
		std::queue<CArea>q;
		//覆盖方格
		std::vector<CArea>covered;
		//非覆盖方格
		std::vector<CArea>none_covered;
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
			m_progress2.SetPos(++pos);
			if (pos == 10000) {
				m_progress2.SetPos(0);
				pos = 0;
			}
			//无覆盖
			if (flag == 0) {
				//加入无覆盖集合
				none_covered.emplace_back(area);
				ok += (area.getLatitudet() - area.getLatitudeb()) * (area.getLongituder() - area.getLongitudel());
			} else if (flag == 2) {
				//加入覆盖集合
				covered.emplace_back(area);
				ok += (area.getLatitudet() - area.getLatitudeb()) * (area.getLongituder() - area.getLongitudel());
				yes += (area.getLatitudet() - area.getLatitudeb()) * (area.getLongituder() - area.getLongitudel());
				//std::cout << yes << std::endl;
			} else {
				if ((totArea - ok) / totArea <= 0.01) {
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
			if ((totArea - ok) / totArea <= 0.01) {
				//未确定面积小 退出
				break;
			}
		}
		ans1.Format(_T("%.4lf%%"), yes / totArea * 100);
	}
	m_progress2.SetPos(10000);
	UpdateData(FALSE);
}

void CCoverageProportionDlg::OnBnClickedInfo() {
	// TODO: 在此添加控件通知处理程序代码
	system("start infoCoverageProportion.txt");
}

void CCoverageProportionDlg::OnBnClickedIn() {
	// TODO: 在此添加控件通知处理程序代码
	CSeries series = (CSeries)m_chart.Series(0);
	series.Clear();
	FILE* f = fopen("CoverArea/ratio.txt", "r");
	m_progress1.SetRange32(0, 86401);
	double* ans = new double[86401];
	for (int t = 0; t <= 86400; t++) {
		fscanf(f, "%lf", &ans[t]);
	}
	fclose(f);
	f = nullptr;
	for (int t = 1599; t <= 86399; t+=1600) {
		for (int i = t - 1599; i <= t; i++) {
			auto toTraditionalTime = [](int timeValue) {
				std::string res;
				int hour = timeValue / 3600;
				int minute = timeValue % 3600 / 60;
				int second = timeValue % 3600 % 60;
				if (hour < 10) {
					res += '0';
				}
				res += std::to_string(hour);
				res += ':';
				if (minute < 10) {
					res += '0';
				}
				res += std::to_string(minute);
				res += ':';
				if (second < 10) {
					res += '0';
				}
				res += std::to_string(second);
				return res;
			};
			series.Add(ans[i], CString(toTraditionalTime(i).c_str()), RGB(255, 0, 0));
		}
		progressNum1.Format(_T("%.2lf%%"), (double)(t + 1) / 864.01);
		m_progress1.SetPos(t + 1);
		UpdateData(FALSE);
		UpdateWindow();
	}
}
