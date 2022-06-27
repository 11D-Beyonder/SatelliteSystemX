#pragma once
#include "CCircle.h"
#include <vector>
#include "CTChart_TimeWindow.h"

// CTimeWindowDlg 对话框

class CTimeWindowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeWindowDlg)

public:
	CTimeWindowDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTimeWindowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIMEWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 圆区域
	CCircle** circle = nullptr;
private:
	// 时间窗口区间
	std::vector<std::pair<int, int>>timeWindow;
	/// <summary>
	/// 获取某一位置的时间窗口
	/// </summary>
	/// <param name="longitude">经度</param>
	/// <param name="latitude">纬度</param>
	void getTimeWindow(double longitude, double latitude);
public:
	// 按钮响应 获取时间窗口
	afx_msg void OnBnClickedLoadTimewindow();
	// TeeChart 控件变量
	CTChart_TimeWindow m_chart;
	// 经度
	double longitude;
	// 纬度
	double latitude;
	// 时间窗口最大值
	CString maxTimeWindow;
	// 时间窗口最小值
	CString minTimeWindow;
	// 时间窗口之和
	CString totTimeWindow;
	// 时间间隔最大值
	CString maxTimeInterval;
	// 时间间隔最小值
	CString minTimeInterval;
	// 时间间隔之和 
	CString totTimeInterval;
	// 显示所有时间窗口
	afx_msg void OnBnClickedTimewindowDetail();
	// 功能说明
	afx_msg void OnBnClickedButtonInfo();
	afx_msg void OnBnClickedIn();
};