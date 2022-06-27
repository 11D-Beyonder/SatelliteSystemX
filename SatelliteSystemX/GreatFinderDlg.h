#pragma once
#include "CTChart_GreatFinder.h"
#include "CCircle.h"
#include <vector>

// GreatFinderDlg 对话框

class GreatFinderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GreatFinderDlg)

public:
	GreatFinderDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~GreatFinderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GREAT_FINDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// TeeChart 控件
	CTChart_GreatFinder m_chart;
	afx_msg void OnBnClickedMaxGreat();
public:
	// 圆区域
	CCircle** circle = nullptr;
private:
	// 时间窗口
	std::vector<std::pair<int, int>>timeWindow;
	/// <summary>
	/// 获取时间窗口
	/// </summary>
	/// <param name="longitude">经度</param>
	/// <param name="latitude">纬度</param>
	void getTimeWindow(double longitude, double latitude);
	/// <summary>
	/// 获取最大时间间隙，
	/// 先获得时间窗口，
	/// 再遍历所有窗口得到时间间隙最大值。
	/// </summary>
	/// <param name="longitude">经度</param>
	/// <param name="latitude">纬度</param>
	/// <returns>最大时间间隙</returns>
	long long getMaxTimeInterval(double longitude, double latitude);
	/// <summary>
	/// 获取最大时间间隙平方和，
	/// 先获得时间窗口，
	/// 再遍历所有窗口得到时间间隙最大值，
	/// 最后遍历所有窗口得到时间间隙最大值平方和。
	/// </summary>
	/// <param name="longitude">经度</param>
	/// <param name="latitude">纬度</param>
	/// <returns>最大时间间隙</returns>
	long long getTimeIntervalSquareSum(double longitude, double latitude);
	/// <summary>
	/// 用RAND_MAX得到0~1随机数
	/// </summary>
	/// <returns>0~1随机数</returns>
	double rand0();
public:
	// 进度条控件
	CProgressCtrl m_progress;
	afx_msg void OnBnClickedSquareSumGreat();
private:
	// 结果
	CString res;
public:
	// 选项
	CString opt;
	afx_msg void OnBnClickedButtonInfo();
};
