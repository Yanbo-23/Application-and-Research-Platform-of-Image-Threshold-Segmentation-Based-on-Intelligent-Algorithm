#include "pch.h"
#include "Platform.h"
#include "PSO_Config.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(PSO_Config, CDialogEx)

PSO_Config::PSO_Config(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_w(_T("1"))
	, m_wdamp(_T("0.99"))
	, w_c1(_T("1.5"))
	, w_c2(_T("2.0"))
{

}

PSO_Config::~PSO_Config()
{
}

void PSO_Config::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_w);
	DDX_Text(pDX, IDC_EDIT5, m_wdamp);
	DDX_Text(pDX, IDC_EDIT6, w_c1);
	DDX_Text(pDX, IDC_EDIT7, w_c2);
}


BEGIN_MESSAGE_MAP(PSO_Config, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &PSO_Config::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT5, &PSO_Config::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &PSO_Config::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &PSO_Config::OnEnChangeEdit7)
	ON_BN_CLICKED(IDC_BUTTON1, &PSO_Config::OnBnClickedButton1)
END_MESSAGE_MAP()


void PSO_Config::OnEnChangeEdit1()
{
	UpdateData();
	UpdateData(false);

}


void PSO_Config::OnEnChangeEdit5()
{
	UpdateData();
	UpdateData(false);

}


void PSO_Config::OnEnChangeEdit6()
{
	UpdateData();
	UpdateData(false);

}


void PSO_Config::OnEnChangeEdit7()
{
	UpdateData();
	UpdateData(false);

}


void PSO_Config::OnBnClickedButton1()
{
	ShowWindow(false);
}
