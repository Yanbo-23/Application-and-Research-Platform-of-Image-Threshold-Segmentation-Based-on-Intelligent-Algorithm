// GA_Config.cpp: 实现文件
//

#include "pch.h"
#include "Platform.h"
#include "GA_Config.h"
#include "afxdialogex.h"


// GA_Config 对话框

IMPLEMENT_DYNAMIC(GA_Config, CDialogEx)

GA_Config::GA_Config(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_etac(_T("20"))
	, m_etam(_T("20"))
	, m_Pc(_T("0.8"))
	, m_Pm(_T("0.2"))
{

}

GA_Config::~GA_Config()
{
}

void GA_Config::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_etac);
	DDX_Text(pDX, IDC_EDIT2, m_etam);
	DDX_Text(pDX, IDC_EDIT3, m_Pc);
	DDX_Text(pDX, IDC_EDIT4, m_Pm);
}


BEGIN_MESSAGE_MAP(GA_Config, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &GA_Config::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &GA_Config::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &GA_Config::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &GA_Config::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON1, &GA_Config::OnBnClickedButton1)
END_MESSAGE_MAP()


// GA_Config 消息处理程序


void GA_Config::OnEnChangeEdit1()
{
	UpdateData();
	UpdateData(false);
}


void GA_Config::OnEnChangeEdit2()
{
	UpdateData();
	UpdateData(false);
}


void GA_Config::OnEnChangeEdit3()
{
	UpdateData();
	UpdateData(false);
}


void GA_Config::OnEnChangeEdit4()
{
	UpdateData();
	UpdateData(false);
}


void GA_Config::OnBnClickedButton1()
{
	ShowWindow(false);

}
