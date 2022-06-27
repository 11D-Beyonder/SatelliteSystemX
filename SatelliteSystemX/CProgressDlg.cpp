// CProgressDlg.cpp: 实现文件
//

#include "pch.h"
#include "SatelliteSystemX.h"
#include "CProgressDlg.h"
#include "afxdialogex.h"


// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROGRESS_INIT, pParent)
	, num_progress(_T(""))
	, opt(_T(""))
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_INIT, m_progress);
	DDX_Text(pDX, IDC_PROGRESS_NUM, num_progress);
	DDX_Text(pDX, IDC_PROGRESS_OPT, opt);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()


// CProgressDlg 消息处理程序
