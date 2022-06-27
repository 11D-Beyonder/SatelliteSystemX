// CTimeWindowDlg.cpp: 实现文件
//

#include "pch.h"
#include "SatelliteSystemX.h"
#include "CTimeWindowDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"
#include "CGanttSeries.h"
#include <string>
#include <algorithm>

// CTimeWindowDlg 对话框

IMPLEMENT_DYNAMIC(CTimeWindowDlg, CDialogEx)

CTimeWindowDlg::CTimeWindowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIMEWINDOW, pParent)
	, longitude(0.0), latitude(0.0), maxTimeWindow(_T("")), minTimeWindow(_T("")), totTimeWindow(_T("")), maxTimeInterval(_T("")), minTimeInterval(_T("")), totTimeInterval(_T("")) {

}

CTimeWindowDlg::~CTimeWindowDlg()
{
}

void CTimeWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_GANTT, m_chart);
	DDX_Text(pDX, IDC_EDIT_LONGITUDE, longitude);
	DDX_Text(pDX, IDC_EDIT_LATITUDE, latitude);
	DDX_Text(pDX, IDC_MAX_WINDOW, maxTimeWindow);
	DDX_Text(pDX, IDC_MIN_WINDOW, minTimeWindow);
	DDX_Text(pDX, IDC_TOT_WINDOW, totTimeWindow);
	DDX_Text(pDX, IDC_MAX_INTERVAL, maxTimeInterval);
	DDX_Text(pDX, IDC_MIN_INTERVAL, minTimeInterval);
	DDX_Text(pDX, IDC_TOT_INTERVAL, totTimeInterval);
}


BEGIN_MESSAGE_MAP(CTimeWindowDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LOAD_TIMEWINDOW, &CTimeWindowDlg::OnBnClickedLoadTimewindow)
	ON_BN_CLICKED(IDC_TIMEWINDOW_DETAIL, &CTimeWindowDlg::OnBnClickedTimewindowDetail)
	ON_BN_CLICKED(IDC_BUTTON_INFO, &CTimeWindowDlg::OnBnClickedButtonInfo)
	ON_BN_CLICKED(IDC_IN, &CTimeWindowDlg::OnBnClickedIn)
END_MESSAGE_MAP()


// CTimeWindowDlg 消息处理程序
void CTimeWindowDlg::getTimeWindow(double longitude, double latitude) {
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

void CTimeWindowDlg::OnBnClickedLoadTimewindow()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	
	getTimeWindow(longitude, latitude);
	
	FILE* f = fopen("TimeWindow.txt", "w");
	fprintf(f, "经度:%.8lf 纬度：%.8lf\n", longitude, latitude);
	fprintf(f, "\n时间窗口\n");
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
	for (int i = 0; i < timeWindow.size(); i++) {
		fprintf(f, "[%s,%s]\n", toTraditionalTime(timeWindow[i].first).c_str(), toTraditionalTime(timeWindow[i].second).c_str());
	}
	fprintf(f, "\n时间间隙\n");
	fprintf(f, "[%s,%s]\n", toTraditionalTime(0).c_str(), toTraditionalTime(timeWindow.front().first - 1).c_str());
	for (int i = 1; i < timeWindow.size(); i++) {
		fprintf(f,"[%s,%s]\n", toTraditionalTime(timeWindow[i - 1].second + 1).c_str(), toTraditionalTime(timeWindow[i].first-1).c_str());
	}
	fprintf(f, "[%s,%s]\n", toTraditionalTime(timeWindow.back().second+1).c_str(), toTraditionalTime(86400).c_str());
	fclose(f);

	// 计算最大最小值
	int _minTimeWindow = INT_MAX;
	int _maxTimeWindow = INT_MIN;
	int _totTimeWindow = 0;
	for (const auto& window:timeWindow) {
		_minTimeWindow = (std::min)(_minTimeWindow, window.second - window.first + 1);
		_maxTimeWindow = (std::max)(_maxTimeWindow, window.second - window.first + 1);
		_totTimeWindow += window.second - window.first + 1;
	}
	minTimeWindow.Format(_T("%d秒"), _minTimeWindow);
	maxTimeWindow.Format(_T("%d秒"), _maxTimeWindow);
	totTimeWindow.Format(_T("%d秒"), _totTimeWindow);
	int _minTimeInterval = timeWindow.front().first + 86401 - timeWindow.back().second;
	int _maxTimeInterval = timeWindow.front().first + 86401 - timeWindow.back().second;
	for (int i = 1; i < timeWindow.size(); i++) {
		_maxTimeInterval = (std::max)(_maxTimeInterval, timeWindow[i].first - timeWindow[i - 1].second - 1);
		_minTimeInterval = (std::min)(_minTimeInterval, timeWindow[i].first - timeWindow[i - 1].second - 1);
	}
	minTimeInterval.Format(_T("%d秒"), _minTimeInterval);
	maxTimeInterval.Format(_T("%d秒"), _maxTimeInterval);
	totTimeInterval.Format(_T("%d秒"), 86401 - _totTimeWindow);
	// 将参数传递到控件
	UpdateData(FALSE);

	//绘制甘特图
	CSeries series0 = (CSeries)m_chart.get_aSeries(0);
	series0.Clear();
	CGanttSeries ganttSeries = (CGanttSeries)series0.get_asGantt();
	CString str;
	for (int i = 0; i < timeWindow.size(); i++) {
		str.Format(_T("%d"), i + 1);
		ganttSeries.AddGanttColor((double)timeWindow[i].first/86400.0, (double)timeWindow[i].second/86400.0, i*10+5, str, RGB(255, 0, 0));
	}
	ganttSeries.AddGanttColor(0.0, (double)timeWindow[0].first / 86400.0, 5, _T(" "), RGB(0, 0, 255));
	for (int i = 1; i < timeWindow.size(); i++) {
		ganttSeries.AddGanttColor(timeWindow[i - 1].second / 86400.0, timeWindow[i].first / 86400.0, i * 10 + 5, _T(" "), RGB(0, 0, 255));
	}
	str.Format(_T("%d"), timeWindow.size() + 1);
	ganttSeries.AddGanttColor(timeWindow.back().second / 86400.0, 1.0, timeWindow.size() * 10 + 5, _T(" "), RGB(0, 0, 255));
}

void CTimeWindowDlg::OnBnClickedTimewindowDetail() {
	// TODO: 在此添加控件通知处理程序代码
	system("start TimeWindow.txt");
}


void CTimeWindowDlg::OnBnClickedButtonInfo() {
	// TODO: 在此添加控件通知处理程序代码
	system("start infoTimeWindow.txt");
}


void CTimeWindowDlg::OnBnClickedIn() {
	// TODO: 在此添加控件通知处理程序代码
	double cityPos[13][2] = {
		80.14, 26.27,
		-94.33,39.02,
		120.27, 23.03,
		67.02, 24.51,
		85.19, 27.42,
		23.54, 54.54,
		139.43, 35.32,
		49.1, 55.45,
		32.36, 15.34,
		102.5, 16.25,
		89.34, 22.49,
		30.05, -1.59,
		167.58, -29.03
	};
	CString cityName[13] = {
		_T("坎普尔"),
		_T("堪萨斯城"),
		_T("高雄"),
		_T("卡拉奇"),
		_T("加德满都"),
		_T("考纳斯"),
		_T("川崎"),
		_T("喀山"),
		_T("喀士穆"),
		_T("孔敬"),
		_T("库尔纳"),
		_T("基加利"),
		_T("京斯敦")
	};
	CSeries series0 = (CSeries)m_chart.get_aSeries(0);
	series0.Clear();
	CGanttSeries ganttSeries = (CGanttSeries)series0.get_asGantt();
	for (int i = 0; i < 13; i++) {
		getTimeWindow(cityPos[i][0], cityPos[i][1]);
		for (int j = 0; j < timeWindow.size(); j++) {
			ganttSeries.AddGanttColor((double)timeWindow[j].first / 86400.0, (double)timeWindow[j].second / 86400.0, i * 10 + 5, cityName[i], RGB(255, 0, 0));
		}
		
		ganttSeries.AddGanttColor(0.0, (double)timeWindow[0].first / 86400.0, i * 10 + 5, cityName[i], RGB(0, 0, 255));
		for (int j = 1; j < timeWindow.size(); j++) {
			ganttSeries.AddGanttColor(timeWindow[j - 1].second / 86400.0, timeWindow[j].first / 86400.0, i * 10 + 5, cityName[i], RGB(0, 0, 255));
		}
		ganttSeries.AddGanttColor(timeWindow.back().second / 86400.0, 1.0, i * 10 + 5, cityName[i], RGB(0, 0, 255));
	}
}