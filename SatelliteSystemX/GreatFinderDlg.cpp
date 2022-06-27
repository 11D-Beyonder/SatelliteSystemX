// GreatFinderDlg.cpp: 实现文件
//

#include "pch.h"
#include "SatelliteSystemX.h"
#include "GreatFinderDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"
#include <algorithm>

// GreatFinderDlg 对话框

IMPLEMENT_DYNAMIC(GreatFinderDlg, CDialogEx)

GreatFinderDlg::GreatFinderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GREAT_FINDER, pParent)
	, res(_T("")), opt(_T("")) {

}

GreatFinderDlg::~GreatFinderDlg()
{
	circle = nullptr;
}

void GreatFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_POINTS, m_chart);
	DDX_Control(pDX, IDC_PROGRESS_CAL, m_progress);
	DDX_Text(pDX, IDC_RES1, res);
	DDX_Text(pDX, IDC_OPT, opt);
}


BEGIN_MESSAGE_MAP(GreatFinderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MAX_GREAT, &GreatFinderDlg::OnBnClickedMaxGreat)
	ON_BN_CLICKED(IDC_SQUARE_SUM_GREAT, &GreatFinderDlg::OnBnClickedSquareSumGreat)
	ON_BN_CLICKED(IDC_BUTTON_INFO, &GreatFinderDlg::OnBnClickedButtonInfo)
END_MESSAGE_MAP()


// GreatFinderDlg 消息处理程序


void GreatFinderDlg::OnBnClickedMaxGreat() {
	// TODO: 在此添加控件通知处理程序代码
	CSeries serices = (CSeries)m_chart.Series(0);
	m_progress.SetRange(0, 200);
	m_progress.SetPos(0);
	srand(time(nullptr));
	//全局最优解
	long long bestFunc = LONGLONG_MAX;
	std::pair<double, double>x[21];
	std::pair<double, double>v[21];
	std::pair<double, double>gBest;
	std::pair<double, double>pBest[21];
	long long f[21] = { 0 };
	const int n = 20;
	auto drawPoints = [&]() {
		serices.Clear();
		for (int i = 1; i <= n; i++) {
			serices.AddXY(x[i].first, x[i].second, _T(""), RGB(0, 0, 255));
		}
		serices.AddXY(gBest.first, gBest.second, _T(""), RGB(255, 0, 0));
		UpdateWindow();
	};
	const double c = 0.2;
	for (int i = 1; i <= n; i++) {
		//75~135
		x[i].first = rand() % 61 + 75;
		//0~55
		x[i].second = rand() % 56;
		//-60~60
		v[i].first = rand() % 121 - 60;
		//-55~55
		v[i].second = rand() % 111 - 55;
		f[i] = getMaxTimeInterval(x[i].first, x[i].second);
		pBest[i] = x[i];
		if (bestFunc > f[i]) {
			bestFunc = f[i];
			gBest = x[i];
		}
	}
	// 画点
	drawPoints();

	double w = 0.7;
	// 结束标志
	for (int _ = 1; _<=100; _++) {
		for (int i = 1; i <= n; i++) {
			//v x 移动
			v[i].first = w * v[i].first + c * rand0() * (pBest[i].first - x[i].first) + c * rand0() * (gBest.first - x[i].first);
			v[i].second = w * v[i].second + c * rand0() * (pBest[i].second - x[i].second) + c * rand0() * (gBest.second - x[i].second);
			x[i].first += v[i].first;
			x[i].second += v[i].second;
			// 边界检测
			if (x[i].first < 75) {
				x[i].first = 75;
			} else if (x[i].first > 135) {
				x[i].first = 135;
			}
			if (x[i].second < 0) {
				x[i].second = 0;
			} else if (x[i].second > 55) {
				x[i].second = 55;
			}
			long long temp = getMaxTimeInterval(x[i].first, x[i].second);
			// 更新部分最优和全局最优
			if (f[i] > temp) {
				f[i] = temp;
				pBest[i] = x[i];
			}
			if (bestFunc > temp) {
				bestFunc = temp;
				gBest = x[i];

			}
		}
		drawPoints();
		m_progress.SetPos(_);
	}
	w = 0.2;
	for (int _ = 101; _<=200; _++) {
		for (int i = 1; i <= n; i++) {
			//v x 移动
			v[i].first = w * v[i].first + c * rand0() * (pBest[i].first - x[i].first) + c * rand0() * (gBest.first - x[i].first);
			v[i].second = w * v[i].second + c * rand0() * (pBest[i].second - x[i].second) + c * rand0() * (gBest.second - x[i].second);
			x[i].first += v[i].first;
			x[i].second += v[i].second;
			//边界检测
			if (x[i].first < 75) {
				x[i].first = 75;
			} else if (x[i].first > 135) {
				x[i].first = 135;
			}
			if (x[i].second < 0) {
				x[i].second = 0;
			} else if (x[i].second > 55) {
				x[i].second = 55;
			}
			long long temp = getMaxTimeInterval(x[i].first, x[i].second);
			//更新部分最优和全局最优
			if (f[i] > temp) {
				f[i] = temp;
				pBest[i] = x[i];
			}
			if (bestFunc > temp) {
				bestFunc = temp;
				gBest = x[i];

			}
		}
		drawPoints();
		m_progress.SetPos(_);
	}
	res.Format(_T("经度：%.8lf\r\n纬度：%.8lf\r\n时间间隙最大值：%lld"), gBest.first, gBest.second, bestFunc);
	opt.Format(_T("时间间隙最大值最小点"));
	UpdateData(FALSE);
}

void GreatFinderDlg::getTimeWindow(double longitude, double latitude) {
	CLocation tar(longitude, latitude);
	//1/1 00:00:00~1/2 00:00:00
	//从第0秒到第86401秒
	std::vector<int>times;
	for (int t = 0; t <= 86400; t++) {
		//找到9个圆 依次检验是否在球缺上
		/*check*/
		for (int i = 0; i <= 8; i++) {
			if (circle[t][i].atSphericalCrown(tar)) {
				//std::cout << i << ' ';
				times.emplace_back(t);
				break;
			}
		}
	}
	int l = 0;
	int r = 0;
	//时间窗口
	timeWindow.clear();
	while (l < times.size() && r < times.size()) {
		while (r + 1 < times.size() && times[r + 1] == times[r] + 1) {
			++r;
		}
		timeWindow.emplace_back(std::pair<int, int>(times[l], times[r]));
		++r;
		l = r;
	}
}


long long GreatFinderDlg::getMaxTimeInterval(double longitude, double latitude) {
	getTimeWindow(longitude, latitude);
	int maxTimeInterval = timeWindow.front().first + 86401 - timeWindow.back().second;
	for (int i = 1; i < timeWindow.size(); i++) {
		maxTimeInterval = (std::max)(maxTimeInterval, timeWindow[i].first - timeWindow[i - 1].second - 1);
	}
	return maxTimeInterval;
}

long long GreatFinderDlg::getTimeIntervalSquareSum(double longitude, double latitude) {
	getTimeWindow(longitude, latitude);
	long long ans = (long long)(timeWindow.front().first + 86401 - timeWindow.back().second) * (long long)(timeWindow.front().first + 86401 - timeWindow.back().second);
	for (int i = 1; i < timeWindow.size(); i++) {
		ans += (long long)(timeWindow[i].first - timeWindow[i - 1].second - 1) * (long long)(timeWindow[i].first - timeWindow[i - 1].second - 1);
	}
	return ans;
}

double GreatFinderDlg::rand0() {
	return (double)rand() / RAND_MAX;
}

void GreatFinderDlg::OnBnClickedSquareSumGreat() {
	// TODO: 在此添加控件通知处理程序代码
	CSeries serices = (CSeries)m_chart.Series(0);
	m_progress.SetRange(0, 200);
	m_progress.SetPos(0);
	srand(time(nullptr));
	//全局最优解
	long long bestFunc = LONGLONG_MAX;
	std::pair<double, double>x[21];
	std::pair<double, double>v[21];
	std::pair<double, double>gBest;
	std::pair<double, double>pBest[21];
	long long f[21] = { 0 };
	const int n = 20;
	auto drawPoints = [&]() {
		serices.Clear();
		for (int i = 1; i <= n; i++) {
			serices.AddXY(x[i].first, x[i].second, _T(""), RGB(0, 0, 255));
		}
		serices.AddXY(gBest.first, gBest.second, _T(""), RGB(255, 0, 0));
		UpdateWindow();
	};
	const double c = 0.2;
	for (int i = 1; i <= n; i++) {
		//75~135
		x[i].first = rand() % 61 + 75;
		//0~55
		x[i].second = rand() % 56;
		//-60~60
		v[i].first = rand() % 121 - 60;
		//-55~55
		v[i].second = rand() % 111 - 55;
		f[i] = getTimeIntervalSquareSum(x[i].first, x[i].second);
		pBest[i] = x[i];
		if (bestFunc > f[i]) {
			bestFunc = f[i];
			gBest = x[i];
		}
	}
	drawPoints();
	double w = 0.7;
	//结束标志
	for (int _ = 1; _ <= 100; _++) {
		for (int i = 1; i <= n; i++) {
			//v x 移动
			v[i].first = w * v[i].first + c * rand0() * (pBest[i].first - x[i].first) + c * rand0() * (gBest.first - x[i].first);
			v[i].second = w * v[i].second + c * rand0() * (pBest[i].second - x[i].second) + c * rand0() * (gBest.second - x[i].second);
			x[i].first += v[i].first;
			x[i].second += v[i].second;
			//边界检测
			if (x[i].first < 75) {
				x[i].first = 75;
			} else if (x[i].first > 135) {
				x[i].first = 135;
			}
			if (x[i].second < 0) {
				x[i].second = 0;
			} else if (x[i].second > 55) {
				x[i].second = 55;
			}
			long long temp = getTimeIntervalSquareSum(x[i].first, x[i].second);
			//更新部分最优和全局最优
			if (f[i] > temp) {
				f[i] = temp;
				pBest[i] = x[i];
			}
			if (bestFunc > temp) {
				bestFunc = temp;
				gBest = x[i];

			}
		}
		m_progress.SetPos(_);
		drawPoints();
	}
	w = 0.2;
	for (int _ = 101; _ <= 200; _++) {
		for (int i = 1; i <= n; i++) {
			//v x 移动
			v[i].first = w * v[i].first + c * rand0() * (pBest[i].first - x[i].first) + c * rand0() * (gBest.first - x[i].first);
			v[i].second = w * v[i].second + c * rand0() * (pBest[i].second - x[i].second) + c * rand0() * (gBest.second - x[i].second);
			x[i].first += v[i].first;
			x[i].second += v[i].second;
			//边界检测
			if (x[i].first < 75) {
				x[i].first = 75;
			} else if (x[i].first > 135) {
				x[i].first = 135;
			}
			if (x[i].second < 0) {
				x[i].second = 0;
			} else if (x[i].second > 55) {
				x[i].second = 55;
			}
			long long temp = getTimeIntervalSquareSum(x[i].first, x[i].second);
			//更新部分最优和全局最优
			if (f[i] > temp) {
				f[i] = temp;
				pBest[i] = x[i];
			}
			if (bestFunc > temp) {
				bestFunc = temp;
				gBest = x[i];

			}
		}
		m_progress.SetPos(_);
		drawPoints();
	}
	opt.Format(_T("时间间隙平方和最小点"));
	res.Format(_T("经度：%.8lf\r\n纬度：%.8lf\r\n时间间隙平方和：%lld"), gBest.first, gBest.second, bestFunc);
	UpdateData(FALSE);
}


void GreatFinderDlg::OnBnClickedButtonInfo() {
	// TODO: 在此添加控件通知处理程序代码
	system("start infoGreatPos.txt");
}
