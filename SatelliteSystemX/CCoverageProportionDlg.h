#pragma once
#include "CTChart_CoverageProportion.h"
#include "CCircle.h"

// CInstantaneousCoverage 对话框

class CCoverageProportionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCoverageProportionDlg)

public:
	CCoverageProportionDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCoverageProportionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COVERAGE_PROPORTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 圆区域
	CCircle** circle = nullptr;
	afx_msg void OnBnClickedCurveCoverage();
private:
	// 瞬时覆盖率图像绘制进度条
	CProgressCtrl m_progress1;
	// 覆盖率数值进度条
	CProgressCtrl m_progress2;
	//TeeChart控件变量
	CTChart_CoverageProportion m_chart;
	// 进度字符串
	CString progressNum1;
public:
	afx_msg void OnBnClickedLoadCoverage();
private:
	// 小时
	UINT _hour;
	// 分钟
	UINT _minute;
	// 秒
	UINT _second;
	// 累计覆盖率
	CString ans1;
	// 瞬时覆盖率
	CString ans2;
public:
	afx_msg void OnBnClickedInfo();
	afx_msg void OnBnClickedIn();
};