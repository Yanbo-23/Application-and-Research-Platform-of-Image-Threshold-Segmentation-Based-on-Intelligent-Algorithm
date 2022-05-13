#include "pch.h"
#include "Platform.h"
#include "STA_Config.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(STA_Config, CDialogEx)

STA_Config::STA_Config(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_beta(_T("1"))
	, m_alpha_min(_T("1"))
	, m_delta(_T("1"))
	, m_alpha_max(_T("20"))
	, m_gamma(_T("1"))
	, m_fc(_T("2"))
{

}

STA_Config::~STA_Config()
{
	if ( m_pTipDlg != NULL) {

		delete  m_pTipDlg;

		m_pTipDlg = NULL;

	}
}

void STA_Config::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_beta);
	DDX_Text(pDX, IDC_EDIT2, m_alpha_min);
	DDX_Text(pDX, IDC_EDIT3, m_delta);
	DDX_Text(pDX, IDC_alpha_max, m_alpha_max);
	DDX_Text(pDX, IDC_gamma, m_gamma);
	DDX_Text(pDX, IDC_fc, m_fc);
}


BEGIN_MESSAGE_MAP(STA_Config, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &STA_Config::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &STA_Config::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &STA_Config::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_alpha_max, &STA_Config::OnEnChangealphamax)
	ON_EN_CHANGE(IDC_gamma, &STA_Config::OnEnChangegamma)
	ON_EN_CHANGE(IDC_fc, &STA_Config::OnEnChangefc)
	ON_BN_CLICKED(IDOK, &STA_Config::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &STA_Config::OnBnClickedButton1)
END_MESSAGE_MAP()


void STA_Config::OnEnChangeEdit1()
{
	UpdateData();
	UpdateData(false);
}


void STA_Config::OnEnChangeEdit2()
{
	UpdateData();
	UpdateData(false);
}


void STA_Config::OnEnChangeEdit3()
{
	UpdateData();
	UpdateData(false);
}


void STA_Config::OnEnChangealphamax()
{
	UpdateData();
	UpdateData(false);
}


void STA_Config::OnEnChangegamma()
{
	UpdateData();
	UpdateData(false);
}


void STA_Config::OnEnChangefc()
{
	UpdateData();
	UpdateData(false);

}


void STA_Config::OnBnClickedOk()
{
	ShowWindow(false);
}


void STA_Config::OnBnClickedButton1()
{
	ShowWindow(false);
}
