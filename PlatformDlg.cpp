#include "pch.h"
#include "plot.h"
#include "OptAlg.h"
#include "SegAlg.h"
#include  "TestAlg.h"
#include "framework.h"
#include <fstream>
#include "Platform.h"
#include "PlatformDlg.h"
#include  "PSO_Config.h "
#include  "STA_Config.h "
#include  "GA_Config.h "
#include  "DONE.h "
#include<thread>
#include <numeric>
#include "io.h"
#include "afxdialogex.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include<ctime>
#include<cmath>
#include <random>
typedef float (*pf)(Mat, Mat);
using namespace cv;
using namespace std;
#define CV_SORT_ASCENDING    0
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
vector <Mat> img(99999);
CString algName;
CString segAlgName;
PSO_Config* PSO_Dlg;
GA_Config* GA_Dlg;
STA_Config* STA_Dlg;
DONE* DONE_Dlg;
CString file;
vector<String> filenames(99999);
vector<float> ssim_;
vector<float> sd_;
vector<float> rt_;
vector<float> fv_;
int singleOrALot = 0;
int imgCount;
Mat History;
int Iter;
int STARTEXP = 0;
vector <Mat> img_ori(99999);
float FBest;
int ms = 0;


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CPlatformDlg::
CPlatformDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLATFORM_DIALOG, pParent)
	, m_ET_Iteration(_T("100"))
	, m_ET_Population(_T("100"))
{
	m_Ck_MoTest = FALSE;
	m_Ck_RT = FALSE;
	m_Ck_MoApl = TRUE;
	m_Ck_FV = FALSE;
	m_Ck_FSIM = FALSE;
	m_ET_FSIMShow = _T("");
	m_ET_ExpTime = _T("");
	m_ET_FVShow = _T("");
	m_ET_Path = _T("");
	m_ET_RepTime = _T("");
	m_ET_RTShow = _T("");
	m_ET_SegTime = _T("");
	m_ET_ThresholdNum = _T("");
	m_CB_SegAlgName = _T("");
	m_CB_AlgSelect = _T("");
	m_Ck_SD = FALSE;
	m_ImageNm = _T("");
	m_BT_StartSeg = 0;
	m_BT_StartExp = 0;
	m_BT_Generate = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlatformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_Generate, m_BT_Generate_);
	DDX_Control(pDX, IDC_BUTTON_StartExp, m_BT_StartExp_);
	DDX_Control(pDX, IDC_BUTTON_StartSeg, m_BT_StartSeg_);
	DDX_Control(pDX, IDC_WORD_RT, m_WD_RT);
	DDX_Control(pDX, IDC_WORD_Path, m_WD_Path);
	DDX_Control(pDX, IDC_WORD_FV, m_WD_FV);
	DDX_Control(pDX, IDC_CHECK_SD, m_Ck_SD_);
	DDX_Control(pDX, IDC_WORD_FSIM, m_WD_FSIM);
	DDX_Control(pDX, IDC_GROUP_Index, m_GP_Index);
	DDX_Control(pDX, IDC_GROUP_Result, m_GP_Result);
	DDX_Control(pDX, IDC_GROUP_InputData, m_GP_InputData);
	DDX_Control(pDX, IDC_COMBO_AlgSelect, m_CB_AlgSelect_);
	DDX_Control(pDX, IDC_COMBO_SegAlgName, m_CB_SegAlgName_);
	DDX_Control(pDX, IDC_WORD_ThresNum, m_WD_ThresNum);
	DDX_Control(pDX, IDC_WORD_SegTime, m_WD_SegTime);
	DDX_Control(pDX, IDC_WORD_SegAlgName, m_WD_SegAlgName);
	DDX_Control(pDX, IDC_WORD_SegProg, m_WD_SegProg);
	DDX_Control(pDX, IDC_WORD_RepTime, m_WD_RepTime);
	DDX_Control(pDX, IDC_WORD_ExpTime, m_WD_ExpTime);
	DDX_Control(pDX, IDC_WORD_ExpProg, m_WD_ExpProg);
	DDX_Control(pDX, IDC_WORD_AlgSelect, m_WD_AlgSelect);
	DDX_Control(pDX, IDC_PROGRESS_Seg, m_PG_Seg);
	DDX_Control(pDX, IDC_PROGRESS_Exp, m_PG_Exp);
	DDX_Control(pDX, IDC_GROUP_ThreAlgChos, m_GP_ThreAlgChos);
	DDX_Control(pDX, IDC_GROUP_TestConfig, m_GP_TestConfig);
	DDX_Control(pDX, IDC_GROUP_ModeChos, m_GP_ModeChos);
	DDX_Control(pDX, IDC_GROUP_IndexChos, m_GP_IndexChos);
	DDX_Control(pDX, IDC_GROUP_FunChos, m_GP_FunChos);
	DDX_Control(pDX, IDC_GROUP_AlgChos, m_GP_AlgChos);
	DDX_Control(pDX, IDC_EDIT_ThresholdNum, m_ET_ThresholdNum_);
	DDX_Control(pDX, IDC_EDIT_SegTime, m_ET_SegTime_);
	DDX_Control(pDX, IDC_EDIT_RepTime, m_ET_RepTime_);
	DDX_Control(pDX, IDC_EDIT_Path, m_ET_Path_);
	DDX_Control(pDX, IDC_EDIT_FVShow, m_ET_FVShow_);
	DDX_Control(pDX, IDC_EDIT_ExpTime, m_ET_ExpTime_);
	DDX_Control(pDX, IDC_EDIT_FSIMShow, m_ET_FSIMShow_);
	DDX_Control(pDX, IDC_CHECK_RT, m_Ck_RT_);
	DDX_Control(pDX, IDC_CHECK_MoTest, m_Ck_MoTest_);
	DDX_Control(pDX, IDC_CHECK_MoApl, m_Ck_MoApl_);
	DDX_Control(pDX, IDC_CHECK_FV, m_Ck_FV_);
	DDX_Control(pDX, IDC_CHECK_FSIM, m_Ck_FSIM_);
	DDX_Control(pDX, IDC_BUTTON_SaveRes, m_BT_SaveRes);
	DDX_Control(pDX, IDC_BUTTON_ParConfig, m_BT_ParConfig);
	DDX_Control(pDX, IDC_BUTTON_InputSingle, m_BT_InputSingle);
	DDX_Control(pDX, IDC_BUTTON_InputCode, m_BT_InputCode);
	DDX_Control(pDX, IDC_BUTTON_InputALot, m_BT_InputALot);
	DDX_Control(pDX, IDC_BUTTON_GenFig2, m_BT_GenFig2);
	DDX_Control(pDX, IDC_BUTTON_GenFig1, m_BT_GenFig1);
	DDX_Check(pDX, IDC_CHECK_MoTest, m_Ck_MoTest);
	DDX_Check(pDX, IDC_CHECK_RT, m_Ck_RT);
	DDX_Check(pDX, IDC_CHECK_MoApl, m_Ck_MoApl);
	DDX_Check(pDX, IDC_CHECK_FV, m_Ck_FV);
	DDX_Check(pDX, IDC_CHECK_FSIM, m_Ck_FSIM);
	DDX_Text(pDX, IDC_EDIT_FSIMShow, m_ET_FSIMShow);
	DDX_Text(pDX, IDC_EDIT_ExpTime, m_ET_ExpTime);
	DDX_Text(pDX, IDC_EDIT_FVShow, m_ET_FVShow);
	DDX_Text(pDX, IDC_EDIT_Path, m_ET_Path);
	DDX_Text(pDX, IDC_EDIT_RepTime, m_ET_RepTime);
	DDX_Text(pDX, IDC_EDIT_RTShow, m_ET_RTShow);
	DDX_Text(pDX, IDC_EDIT_SegTime, m_ET_SegTime);
	DDX_Text(pDX, IDC_EDIT_ThresholdNum, m_ET_ThresholdNum);
	DDX_CBString(pDX, IDC_COMBO_SegAlgName, m_CB_SegAlgName);
	DDX_CBString(pDX, IDC_COMBO_AlgSelect, m_CB_AlgSelect);
	DDX_Check(pDX, IDC_CHECK_SD, m_Ck_SD);
	DDX_Control(pDX, IDC_GenFig3, m_BT_GenFig3_);
	DDX_Control(pDX, IDC_ET_Iteration, m_ET_Iteration_);
	DDX_Text(pDX, IDC_ET_Iteration, m_ET_Iteration);
	DDX_Control(pDX, IDC_ET_Population, m_ET_Population_);
	DDX_Text(pDX, IDC_ET_Population, m_ET_Population);
	DDX_Control(pDX, IDC_WD_Population, m_WD_Population);
	DDX_Control(pDX, IDC_WD_Iteration, m_WD_Iteration);
	DDX_Control(pDX, IDC_BT_ShowImg, m_BT_ShowImg);
	DDX_Control(pDX, IDC_EDIT_RTShow, m_ET_RTShow_);
}

BEGIN_MESSAGE_MAP(CPlatformDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_MoTest, OnCHECKMoTest)
	ON_BN_CLICKED(IDC_BUTTON_InputSingle, OnBUTTONInputSingle)
	ON_BN_CLICKED(IDC_BUTTON_ParConfig, OnBUTTONParConfig)
	ON_BN_CLICKED(IDC_BUTTON_SaveRes, OnBUTTONSaveRes)
	ON_BN_CLICKED(IDC_BUTTON_StartExp, OnBUTTONStartExp)
	ON_BN_CLICKED(IDC_BUTTON_StartSeg, OnBUTTONStartSeg)
	ON_BN_CLICKED(IDC_BUTTON_InputALot, OnBUTTONInputALot)
	ON_BN_CLICKED(IDC_CHECK_FSIM, OnCheckFsim)
	ON_BN_CLICKED(IDC_CHECK_FV, OnCheckFv)
	ON_BN_CLICKED(IDC_CHECK_RT, OnCheckRt)
	ON_BN_CLICKED(IDC_CHECK_SD, OnCheckSd)
	ON_EN_CHANGE(IDC_EDIT_RepTime, OnChangeEDITRepTime)
	ON_CBN_CLOSEUP(IDC_COMBO_AlgSelect, OnCloseupCOMBOAlgSelect)
	ON_CBN_CLOSEUP(IDC_COMBO_SegAlgName, OnCloseupCOMBOSegAlgName)
	ON_EN_CHANGE(IDC_EDIT_ThresholdNum, OnChangeEDITThresholdNum)
	ON_BN_CLICKED(IDC_BUTTON_GenFig2, &CPlatformDlg::OnBnClickedButtonGenfig2)
	ON_BN_CLICKED(IDC_BUTTON_GenFig1, &CPlatformDlg::OnBnClickedButtonGenfig1)
	ON_BN_CLICKED(IDC_GenFig3, &CPlatformDlg::OnBnClickedGenfig3)
	ON_EN_CHANGE(IDC_ET_Iteration, &CPlatformDlg::OnEnChangeEtIteration)
	ON_EN_CHANGE(IDC_ET_Population, &CPlatformDlg::OnEnChangeEtPopulation)
	ON_EN_CHANGE(IDC_EDIT_Path, &CPlatformDlg::OnEnChangeEditPath)
	ON_BN_CLICKED(IDC_BT_ShowImg, &CPlatformDlg::OnBnClickedBtShowimg)
	ON_BN_CLICKED(IDC_BUTTON_Generate, &CPlatformDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_CHECK_MoApl, &CPlatformDlg::OnCHECKMoApl)
	ON_WM_TIMER()


END_MESSAGE_MAP()

BOOL CPlatformDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);


	return TRUE;
}

void CPlatformDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}



void CPlatformDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CPlatformDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CPlatformDlg::OnCHECKMoTest()
{
	UpdateData();
	m_Ck_MoApl = FALSE;
	m_GP_TestConfig.EnableWindow(m_Ck_MoTest);
	m_GP_IndexChos.EnableWindow(m_Ck_MoTest);
	m_Ck_FV_.EnableWindow(m_Ck_MoTest);
	m_Ck_RT_.EnableWindow(m_Ck_MoTest);
	m_Ck_SD_.EnableWindow(m_Ck_MoTest);
	m_Ck_FSIM_.EnableWindow(m_Ck_MoTest);
	m_WD_RepTime.EnableWindow(m_Ck_MoTest);
	m_ET_RepTime_.EnableWindow(m_Ck_MoTest);
	if (m_ET_RepTime != _T("") && (m_Ck_FSIM || m_Ck_FV || m_Ck_RT || m_Ck_SD) && m_Ck_MoTest)
		m_BT_StartExp_.EnableWindow(TRUE);
	else
		m_BT_StartExp_.EnableWindow(FALSE);
	m_ET_Population_.EnableWindow(m_Ck_MoTest);
	m_WD_ExpProg.EnableWindow(m_Ck_MoTest);
	m_PG_Exp.EnableWindow(m_Ck_MoTest);
	m_WD_ExpTime.EnableWindow(m_Ck_MoTest);
	m_ET_ExpTime_.EnableWindow(m_Ck_MoTest);
	m_BT_StartSeg_.EnableWindow(m_Ck_MoApl);
	m_WD_SegProg.EnableWindow(m_Ck_MoApl);
	m_PG_Seg.EnableWindow(m_Ck_MoApl);
	m_WD_SegTime.EnableWindow(m_Ck_MoApl);
	m_ET_SegTime_.EnableWindow(m_Ck_MoApl);
	m_WD_FV.EnableWindow(m_Ck_MoApl);
	m_WD_RT.EnableWindow(m_Ck_MoApl);
	m_WD_FSIM.EnableWindow(m_Ck_MoApl);
	m_ET_FVShow_.EnableWindow(m_Ck_MoApl);
	m_ET_RTShow_.EnableWindow(m_Ck_MoApl);
	m_ET_FSIMShow_.EnableWindow(m_Ck_MoApl);
	m_BT_InputALot.EnableWindow(m_Ck_MoApl);
	m_ET_Path = _T("");
	UpdateData(FALSE);

}
void Drawline(Mat img, Point start, Point end)
{
	int thickness = 1;
	int linetype = 8;
	line(img, start, end, Scalar(250, 220, 0), thickness, linetype);
}



void CPlatformDlg::OnCHECKMoApl()
{
	UpdateData();
	m_Ck_MoTest = FALSE;
	m_BT_Generate_.EnableWindow(m_Ck_MoTest);
	m_GP_TestConfig.EnableWindow(m_Ck_MoTest);
	m_GP_IndexChos.EnableWindow(m_Ck_MoTest);
	m_Ck_FV_.EnableWindow(m_Ck_MoTest);
	m_Ck_RT_.EnableWindow(m_Ck_MoTest);
	m_Ck_SD_.EnableWindow(m_Ck_MoTest);
	m_Ck_FSIM_.EnableWindow(m_Ck_MoTest);
	m_WD_RepTime.EnableWindow(m_Ck_MoTest);
	m_ET_RepTime_.EnableWindow(m_Ck_MoTest);
	m_BT_StartExp_.EnableWindow(m_Ck_MoTest);
	m_WD_ExpProg.EnableWindow(m_Ck_MoTest);
	m_PG_Exp.EnableWindow(m_Ck_MoTest);
	m_WD_ExpTime.EnableWindow(m_Ck_MoTest);
	m_ET_ExpTime_.EnableWindow(m_Ck_MoTest);
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoApl) {
		m_BT_StartSeg_.EnableWindow(TRUE);
		m_BT_ShowImg.EnableWindow(TRUE);
	}
	else {
		m_BT_StartSeg_.EnableWindow(FALSE);
		m_BT_ShowImg.EnableWindow(FALSE);
	}
	m_WD_SegProg.EnableWindow(m_Ck_MoApl);
	m_PG_Seg.EnableWindow(m_Ck_MoApl);
	m_WD_SegTime.EnableWindow(m_Ck_MoApl);
	m_ET_SegTime_.EnableWindow(m_Ck_MoApl);
	m_BT_InputALot.EnableWindow(m_Ck_MoApl);
	UpdateData(FALSE);

}
void CPlatformDlg::OnBUTTONInputSingle()
{
	singleOrALot = 1;
	CString filter;
	filter = "JPG Files (*.jpg)|*.jpg|Png Files (*.png)|*.png||";
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter, this);
	UpdateData(TRUE);
	if (fileDlg.DoModal() == IDOK) {
		m_ImageNm = fileDlg.GetPathName();
		m_ET_Path = m_ImageNm;
		UpdateData(FALSE);
	}; 
}

void CPlatformDlg::OnTimer(UINT_PTR nIDEvent)
{

	UpdateData();
	ms += 1;
	CString time;
	time.Format(_T("%.2f"), ms / 60.0);
	m_ET_ExpTime = time;
	UpdateData(false);
	CDialogEx::OnTimer(nIDEvent);

}

void CPlatformDlg::OnBUTTONParConfig()
{
	if (algName == "STA") {
		if (STA_Dlg != NULL)

		{
			STA_Dlg->ShowWindow(true);

		}
		else {
			STA_Dlg = new STA_Config();
			STA_Dlg->Create(IDD_DIALOG3, this);
			STA_Dlg->ShowWindow(SW_SHOW);
		}
	}
	if (algName == "GA") {
		if (GA_Dlg != NULL)

		{
			GA_Dlg->ShowWindow(true);

		}
		else {
			GA_Dlg = new GA_Config();
			GA_Dlg->Create(IDD_DIALOG2, this);
			GA_Dlg->ShowWindow(SW_SHOW);
		}
	}
	if (algName == "PSO") {
		if (PSO_Dlg != NULL)

		{
			PSO_Dlg->ShowWindow(true);

		}
		else {
			PSO_Dlg = new PSO_Config();
			PSO_Dlg->Create(IDD_DIALOG1, this);
			PSO_Dlg->ShowWindow(SW_SHOW);
		}
	}
}

void CPlatformDlg::OnBUTTONSaveRes()
{

	TCHAR szFolderPath[MAX_PATH] = { 0 };
	CString strPath;
	BROWSEINFO sInfo;
	::ZeroMemory(&sInfo, sizeof(sInfo));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("Please read your folder!");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.ulFlags = NULL;
	LPITEMIDLIST lPidBrowse = ::SHBrowseForFolder(&sInfo);
	if (lPidBrowse != nullptr)
	{
		if (::SHGetPathFromIDList(lPidBrowse, szFolderPath))
		{
			strPath = szFolderPath;
		}
	}
	if (lPidBrowse != nullptr)
	{
		::CoTaskMemFree(lPidBrowse);
	}
	if (strPath.IsEmpty())
	{
		AfxMessageBox(_T("Failed to select folder!"));
		return;
	}

	if (!::PathFileExists(strPath))
	{
		AfxMessageBox(_T("Error selecting folder!"));
		return;
	}
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strPath);
	std::list<CString> lstName;
	CString strText;
	while (bWorking)
	{
		bWorking = finder.FindNextFileW();
		if (finder.IsDots())
		{
			continue;
		}
		else
		{
			CString str = finder.GetFileName();
			lstName.push_back(str);
			strText += str + _T("\r\n");
		}
	}
	CString tem;
	int num = imgCount;
	if (singleOrALot == 1)
		num = 1;
	CString strPath_;
	for (int i = 1; i < num + 1; i++) {
		strPath_ = strPath + _T("\\");
		tem.Format(_T("%d"), i);
		strPath_ = strPath_ + tem + _T(".jpg");
		std::string savePath = (CStringA)strPath_;
		imwrite(savePath, img[i - 1]);

	}
	DONE_Dlg = new DONE();
	DONE_Dlg->Create(IDD_DIALOG4, this);
	DONE_Dlg->ShowWindow(SW_SHOW);
}

void CPlatformDlg::OnBUTTONStartExp()
{
	STARTEXP = 1;
	UpdateData();
	m_ET_SegTime = _T("");
	CString timeTotalStr = 0;
	float timeTotal = 0;
	m_WD_FV.EnableWindow(false);
	m_WD_RT.EnableWindow(false);
	m_WD_FSIM.EnableWindow(false);
	m_ET_FVShow_.EnableWindow(false);
	m_ET_RTShow_.EnableWindow(false);
	m_ET_FSIMShow_.EnableWindow(false);
	srand(unsigned(time(NULL)));
	clock_t startTime;
	clock_t endTime;

	const int times = _ttoi(m_ET_RepTime);
	vector<float> ssim(times);
	vector<float> sd(times);
	vector<float> rt(times);
	vector<float> fv(times);
	m_PG_Exp.SetPos(0);
	m_PG_Exp.SetRange(0, times);
	m_PG_Exp.SetStep(1);
	const int SE = _ttoi(m_ET_Population);
	const int Dim = _ttoi(m_ET_ThresholdNum);
	const int Iterations = _ttoi(m_ET_Iteration);
	if (singleOrALot == 1)
		imgCount = 1;
	for (int number = 0; number < times; number++) {
		float low = 0;
		float up = 255;
		string str;
		str = CT2A(m_ImageNm.GetString());
		size_t pos = str.find(".");
		size_t size = str.size();
		string x = str.substr(0, pos);
		string newname = x + ".jpg";
		rename(str.c_str(), newname.c_str());
		str = newname;
		img_ori[number] = imread(str, 0).clone();
		img[number] = imread(str, 0).clone();  
		if (img[number].channels() == 3)
			cvtColor(img[number], img[number], COLOR_RGB2GRAY); 

		Mat h = Mat::zeros(1, 256, CV_32F);
		for (int i = 0; i < img[number].rows; ++i) {
			for (int j = 0; j < img[number].cols; ++j) {
				h.at<float>((int)img[number].at<uchar>(i, j))++;
			}
		}
		pf p;
		if (segAlgName == _T("OTSU I"))
			p = ostu1;
		else if (segAlgName == _T("OTSU II"))
			p = ostu2;
		else
			p = Kapur;
		startTime = clock();
		Mat Best;
		if (algName == _T("STA")) {
			STA sta = STA(p, SE, Dim, Iterations, h, low, up);
			if (STA_Dlg != NULL) {
				sta.alpha_max = _ttof(STA_Dlg->m_alpha_max);
				sta.alpha_min = _ttof(STA_Dlg->m_alpha_min);
				sta.beta = _ttof(STA_Dlg->m_beta);
				sta.gamma = _ttof(STA_Dlg->m_gamma);
				sta.delta = _ttof(STA_Dlg->m_delta);
				sta.fc = _ttof(STA_Dlg->m_fc);

			}
			Iter = sta.Iterations;
			sta.STAMain();
			Best = sta.Best;
			FBest = sta.fBest;
			History = -sta.history.clone();
		}
		if (algName == _T("PSO")) {
			PSO pso(p, SE, Dim, Iterations, h, low, up);
			if (PSO_Dlg != NULL) {
				pso.w = _ttof(PSO_Dlg->m_w);
				pso.wdamp = _ttof(PSO_Dlg->m_wdamp);
				pso.c1 = _ttof(PSO_Dlg->w_c1);
				pso.c2 = _ttof(PSO_Dlg->w_c2);

			}
			Iter = pso.Iterations;
			pso.PSOMain();
			Best = pso.Best;
			FBest = pso.fBest;
			History = -pso.history.clone();
		}
		if (algName == _T("GA")) {
			GA ga(p, SE, Dim, Iterations, h, low, up);
			if (GA_Dlg != NULL) {
				ga.etac = _ttof(GA_Dlg->m_etac);
				ga.etam = _ttof(GA_Dlg->m_etam);
				ga.Pc = _ttof(GA_Dlg->m_Pc);
				ga.Pm = _ttof(GA_Dlg->m_Pm);
			}
			Iter = ga.Iterations;
			ga.GAMain();
			Best = ga.Best;
			FBest = ga.fBest;
			History = -ga.history.clone();
		}
		endTime = clock();
		int len = Best.rows * Best.cols;
		float gray_index = 255 / len;
		cv::sort(Best, Best, SORT_EVERY_ROW + 0);
		for (int i = 0; i < img[number].rows; i++) {
			for (int j = 0; j < img[number].cols * img[number].channels(); j++) {
				if ((float)img[number].at<uchar>(i, j) > 0 && (float)img[number].at<uchar>(i, j) < Best.at<float>(0, 0)) {
					img[number].at<uchar>(i, j) = 0;
				}
				int index;
				for (index = 1; index < len; index++) {
					if ((float)img[number].at<uchar>(i, j) >= Best.at<float>(0, index - 1) && (float)img[number].at<uchar>(i, j) <= Best.at<float>(0, index)) {
						img[number].at<uchar>(i, j) = (uchar)(index);
					}
				}
				if ((float)img[number].at<uchar>(i, j) > Best.at<float>(0, Dim - 1) && (float)img[number].at<uchar>(i, j) <= 255) {

					img[number].at<uchar>(i, j) = (index);
				}
			}
		}
		m_PG_Exp.StepIt();
		img[number] = 255 * (img[number] / Dim);
		ssim[number] = SSIM(img_ori[number], img[number]);
		rt[number] = float(endTime - startTime) / 1000;
		fv[number] = History.at<float>(Iter - 1);
		timeTotal += rt[number];
		timeTotalStr.Format(_T("%4f"), timeTotal);
		m_ET_ExpTime = timeTotalStr + _T(" (s)");
	}
	ssim_ = ssim;
	fv_ = fv;
	rt_ = rt;
	CString FV;
	CString RT;
	CString SS;
	CString S_D;
	FV.Format(_T("%.4f"), SumVector(fv) / times);
	RT.Format(_T("%f"), SumVector(rt) / times);
	SS.Format(_T("%.4f"), SumVector(ssim) / times);
	S_D.Format(_T("%f"), SD(fv));
	RT = RT + _T(" (s)");
	m_ET_FVShow = FV;
	m_ET_RTShow = RT;
	m_ET_FSIMShow = SS;
	m_BT_GenFig1.EnableWindow(true);
	m_BT_GenFig2.EnableWindow(true);
	m_BT_GenFig3_.EnableWindow(true);
	m_BT_SaveRes.EnableWindow(true);
	m_BT_Generate_.EnableWindow(true);
	m_WD_FV.EnableWindow(true);
	m_WD_RT.EnableWindow(true);
	m_WD_FSIM.EnableWindow(true);
	m_ET_FVShow_.EnableWindow(true);
	m_ET_RTShow_.EnableWindow(true);
	m_ET_FSIMShow_.EnableWindow(true);
	UpdateData(false);
	STARTEXP = 0;

}
void CPlatformDlg::OnBUTTONStartSeg()
{
	UpdateData();
	clock_t startTime;
	clock_t endTime;
	m_WD_FV.EnableWindow(false);
	m_WD_RT.EnableWindow(false);
	m_WD_FSIM.EnableWindow(false);
	m_ET_FVShow_.EnableWindow(false);
	m_ET_RTShow_.EnableWindow(false);
	m_ET_FSIMShow_.EnableWindow(false);
	if (singleOrALot == 1)
		imgCount = 1;
	m_PG_Seg.SetPos(0);
	if (imgCount == 1)
		m_PG_Seg.SetRange(0, imgCount + 1);
	else
		m_PG_Seg.SetRange(0, imgCount);
	m_PG_Seg.SetStep(1);
	for (int number = 0; number < imgCount; number++) {
		srand(unsigned(time(NULL)));
		const int SE = _ttoi(m_ET_Population);
		const int Dim = _ttoi(m_ET_ThresholdNum);
		const int Iterations = _ttoi(m_ET_Iteration);
		float low = 0;
		float up = 255;
		string str;
		if (singleOrALot == 1)
			str = CT2A(m_ImageNm.GetString());
		else {
			str = filenames[number];
		}
		size_t pos = str.find(".");
		size_t size = str.size();
		string x = str.substr(0, pos);
		string newname = x + ".jpg";
		rename(str.c_str(), newname.c_str());
		str = newname;
		img_ori[number] = imread(str, 0).clone();
		img[number] = imread(str, 0).clone();
		if (img[number].channels() == 3)
			cvtColor(img[number], img[number], COLOR_RGB2GRAY);

		Mat h = Mat::zeros(1, 256, CV_32F);
		for (int i = 0; i < img[number].rows; ++i) {
			for (int j = 0; j < img[number].cols; ++j) {
				h.at<float>((int)img[number].at<uchar>(i, j))++;
			}
		}
		pf p;
		if (segAlgName == _T("OTSU I"))
			p = ostu1;
		else if (segAlgName == _T("OTSU II"))
			p = ostu2;
		else
			p = Kapur;
		startTime = clock();
		Mat Best;
		if (algName == _T("STA")) {
			STA sta = STA(p, SE, Dim, Iterations, h, low, up);
			if (STA_Dlg != NULL) {
				sta.alpha_max = _ttof(STA_Dlg->m_alpha_max);
				sta.alpha_min = _ttof(STA_Dlg->m_alpha_min);
				sta.beta = _ttof(STA_Dlg->m_beta);
				sta.gamma = _ttof(STA_Dlg->m_gamma);
				sta.delta = _ttof(STA_Dlg->m_delta);
				sta.fc = _ttof(STA_Dlg->m_fc);

			}
			Iter = sta.Iterations;
			sta.STAMain();
			Best = sta.Best;
			FBest = sta.fBest;
			History = -sta.history.clone();
		}
		if (algName == _T("PSO")) {
			PSO pso(p, SE, Dim, Iterations, h, low, up);
			if (PSO_Dlg != NULL) {
				pso.w = _ttof(PSO_Dlg->m_w);
				pso.wdamp = _ttof(PSO_Dlg->m_wdamp);
				pso.c1 = _ttof(PSO_Dlg->w_c1);
				pso.c2 = _ttof(PSO_Dlg->w_c2);

			}
			Iter = pso.Iterations;
			pso.PSOMain();
			Best = pso.Best;
			FBest = pso.fBest;
			History = -pso.history.clone();
		}
		if (algName == _T("GA")) {
			GA ga(p, SE, Dim, Iterations, h, low, up);
			if (GA_Dlg != NULL) {
				ga.etac = _ttof(GA_Dlg->m_etac);
				ga.etam = _ttof(GA_Dlg->m_etam);
				ga.Pc = _ttof(GA_Dlg->m_Pc);
				ga.Pm = _ttof(GA_Dlg->m_Pm);
			}
			Iter = ga.Iterations;
			ga.GAMain();
			Best = ga.Best;
			FBest = ga.fBest;
			History = -ga.history.clone();
		}
		endTime = clock();
		int len = Best.rows * Best.cols;
		float gray_index = 255 / len;
		cv::sort(Best, Best, SORT_EVERY_ROW + 0);
		for (int i = 0; i < img[number].rows; i++) {
			for (int j = 0; j < img[number].cols * img[number].channels(); j++) {
				if ((float)img[number].at<uchar>(i, j) > 0 && (float)img[number].at<uchar>(i, j) < Best.at<float>(0, 0)) {
					img[number].at<uchar>(i, j) = 0;

				}
				int index;
				for (index = 1; index < len; index++) {
					if ((float)img[number].at<uchar>(i, j) >= Best.at<float>(0, index - 1) && (float)img[number].at<uchar>(i, j) <= Best.at<float>(0, index)) {
						img[number].at<uchar>(i, j) = (uchar)(index);
					}
				}
				if ((float)img[number].at<uchar>(i, j) > Best.at<float>(0, Dim - 1) && (float)img[number].at<uchar>(i, j) <= 255) {
					img[number].at<uchar>(i, j) = (index);
				}
			}
		}
		m_PG_Seg.StepIt();
		img[number] = 255 * (img[number] / Dim);
	}
	CString FV;
	CString RT;
	CString SS;
	FV.Format(_T("%f"), History.at<float>(Iter - 1));
	RT.Format(_T("%.4f"), float(endTime - startTime) / 1000);
	SS.Format(_T("%.4f"), SSIM(img_ori[imgCount - 1], img[imgCount - 1]));
	RT = RT + _T(" (s)");
	m_ET_FVShow = FV;
	m_ET_RTShow = RT;
	m_ET_FSIMShow = SS;
	m_BT_GenFig1.EnableWindow(true);
	m_BT_GenFig2.EnableWindow(true);
	m_BT_GenFig3_.EnableWindow(true);
	m_BT_SaveRes.EnableWindow(true);
	m_WD_FV.EnableWindow(true);
	m_WD_RT.EnableWindow(true);
	m_WD_FSIM.EnableWindow(true);
	m_ET_FVShow_.EnableWindow(true);
	m_ET_RTShow_.EnableWindow(true);
	m_ET_FSIMShow_.EnableWindow(true);
	if (imgCount == 1)
		m_PG_Seg.StepIt();
	UpdateData(false);
}

void CPlatformDlg::OnBUTTONInputALot()
{
	singleOrALot = 2;
	TCHAR szFolderPath[MAX_PATH] = { 0 };
	CString strPath;
	BROWSEINFO sInfo;
	::ZeroMemory(&sInfo, sizeof(sInfo));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("Please read the folder ！");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.ulFlags = NULL;
	LPITEMIDLIST lPidBrowse = ::SHBrowseForFolder(&sInfo);
	if (lPidBrowse != nullptr)
	{
		if (::SHGetPathFromIDList(lPidBrowse, szFolderPath))
		{
			strPath = szFolderPath;
		}
	}
	if (lPidBrowse != nullptr)
	{
		::CoTaskMemFree(lPidBrowse);
	}
	if (strPath.IsEmpty())
	{
		AfxMessageBox(_T("Failed to select folder!"));
		return;
	}

	if (!::PathFileExists(strPath))
	{
		AfxMessageBox(_T("Error selecting folder!"));
		return;
	}
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strPath);
	std::list<CString> lstName;
	CString strText;
	while (bWorking)
	{
		bWorking = finder.FindNextFileW();
		if (finder.IsDots())
		{
			continue;
		}
		else
		{
			CString str = finder.GetFileName();
			lstName.push_back(str);
			strText += str + _T("\r\n");
		}
	}
	file = strPath;
	file = file + _T("\\");
	UpdateData();
	m_ET_Path = file;
	UpdateData(false);
	intptr_t handle;
	struct _finddata_t fileinfo;
	imgCount = 1;
	String ddd = file + "*.jpg";
	handle = _findfirst(ddd.data(), &fileinfo);
	if (handle != -1) {
		while (!_findnext(handle, &fileinfo))
		{
			imgCount++;
		}
	}
	handle = _findfirst(ddd.data(), &fileinfo);
	if (handle != -1) {
		filenames[0] = file + fileinfo.name;
		string last_img = fileinfo.name;
		string current_img;
		int i = 1;
		while (!_findnext(handle, &fileinfo))
		{
			current_img = fileinfo.name;
			filenames[i] = file + fileinfo.name;
			last_img = fileinfo.name;
			i++;
		}
		_findclose(handle);
	}

}

void CPlatformDlg::OnCheckFsim()
{
	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoTest &&
		m_ET_RepTime != _T("") && (m_Ck_FSIM || m_Ck_FV || m_Ck_RT || m_Ck_SD))
		m_BT_StartExp_.EnableWindow(TRUE);
	else
		m_BT_StartExp_.EnableWindow(FALSE);
	UpdateData(FALSE);

}

void CPlatformDlg::OnCheckFv()
{
	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoTest &&
		m_ET_RepTime != _T("") && (m_Ck_FSIM || m_Ck_FV || m_Ck_RT || m_Ck_SD))
		m_BT_StartExp_.EnableWindow(TRUE);
	else
		m_BT_StartExp_.EnableWindow(FALSE);
	UpdateData(FALSE);

}

void CPlatformDlg::OnCheckRt()
{
	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoTest &&
		m_ET_RepTime != _T("") && (m_Ck_FSIM || m_Ck_FV || m_Ck_RT || m_Ck_SD))
		m_BT_StartExp_.EnableWindow(TRUE);
	else
		m_BT_StartExp_.EnableWindow(FALSE);
	UpdateData(FALSE);

}

void CPlatformDlg::OnCheckSd()
{
	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoTest &&
		m_ET_RepTime != _T("") && (m_Ck_FSIM || m_Ck_FV || m_Ck_RT || m_Ck_SD))
		m_BT_StartExp_.EnableWindow(TRUE);
	else
		m_BT_StartExp_.EnableWindow(FALSE);
	UpdateData(FALSE);

}

void CPlatformDlg::OnChangeEDITRepTime()
{

	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoTest &&
		m_ET_RepTime != _T("") && (m_Ck_FSIM || m_Ck_FV || m_Ck_RT || m_Ck_SD))
		m_BT_StartExp_.EnableWindow(TRUE);
	else
		m_BT_StartExp_.EnableWindow(FALSE);
	UpdateData(FALSE);


}
void CPlatformDlg::OnEnChangeEtIteration()
{
	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoApl) {
		m_BT_StartSeg_.EnableWindow(TRUE);
		m_BT_ShowImg.EnableWindow(TRUE);
	}
	else {
		m_BT_StartSeg_.EnableWindow(FALSE);
		m_BT_ShowImg.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}


void CPlatformDlg::OnEnChangeEtPopulation()
{
	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoApl) {
		m_BT_StartSeg_.EnableWindow(TRUE);
		m_BT_ShowImg.EnableWindow(TRUE);
	}
	else {
		m_BT_StartSeg_.EnableWindow(FALSE);
		m_BT_ShowImg.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}


void CPlatformDlg::OnCloseupCOMBOAlgSelect()
{
	int index = 0;
	CString str;
	index = m_CB_AlgSelect_.GetCurSel();
	m_CB_AlgSelect_.GetLBText(index, str);
	algName = str;
	m_BT_ParConfig.EnableWindow(TRUE);

	UpdateData();
	m_CB_AlgSelect = str;
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoApl) {
		m_BT_StartSeg_.EnableWindow(TRUE);
		m_BT_ShowImg.EnableWindow(TRUE);
	}
	else {
		m_BT_StartSeg_.EnableWindow(FALSE);
		m_BT_ShowImg.EnableWindow(FALSE);
	}

	UpdateData(FALSE);


}

void CPlatformDlg::OnCloseupCOMBOSegAlgName()
{
	int index = 0;
	CString str;
	index = m_CB_SegAlgName_.GetCurSel();
	m_CB_SegAlgName_.GetLBText(index, str);
	segAlgName = str;
	UpdateData();
	m_CB_SegAlgName = str;
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoApl) {
		m_BT_StartSeg_.EnableWindow(TRUE);
		m_BT_ShowImg.EnableWindow(TRUE);
	}
	else {
		m_BT_StartSeg_.EnableWindow(FALSE);
		m_BT_ShowImg.EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CPlatformDlg::OnEnChangeEditPath()
{
	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoApl) {
		m_BT_StartSeg_.EnableWindow(TRUE);
		m_BT_ShowImg.EnableWindow(TRUE);
	}
	else {
		m_BT_StartSeg_.EnableWindow(FALSE);
		m_BT_ShowImg.EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CPlatformDlg::OnChangeEDITThresholdNum()
{
	UpdateData();
	if (m_CB_AlgSelect != _T("") && m_CB_SegAlgName != _T("") && m_ET_ThresholdNum != _T("")
		&& m_ET_Population != _T("") && m_ET_Iteration != _T("") && m_ET_Path != _T("") && m_Ck_MoApl) {
		m_BT_StartSeg_.EnableWindow(TRUE);
		m_BT_ShowImg.EnableWindow(TRUE);
	}
	else {
		m_BT_StartSeg_.EnableWindow(FALSE);
		m_BT_ShowImg.EnableWindow(FALSE);
	}
	UpdateData(FALSE);

}
void CPlatformDlg::OnBnClickedBtShowimg()
{
	if (singleOrALot == 1)
		imgCount = 1;
	for (int number = 0; number < imgCount; number++) {
		srand(unsigned(time(NULL)));
		const int SE = _ttoi(m_ET_Population);
		const int Dim = _ttoi(m_ET_ThresholdNum);
		const int Iterations = _ttoi(m_ET_Iteration);
		float low = 0;
		float up = 255;
		string str;
		if (singleOrALot == 1)
			str = CT2A(m_ImageNm.GetString());
		else {
			str = filenames[number];
		}
		img_ori[number] = imread(str, 0).clone();
	}
	imgShow(img_ori[imgCount - 1]);
}

void CPlatformDlg::OnBnClickedButtonGenfig1()
{
	int Cnt = Iter;
	int* x = new int[Cnt];
	int* y = new int[Cnt];
	vector<float> X(x, x + Iter);
	vector<float> Y(y, y + Iter);
	for (int i = 0; i < Iter; i++)
	{
		X[i] = (double)i;
		Y[i] = History.at<float>(i);
	}

	Plot plot(float(-1.1), float(Iter), History.at<float>(0), History.at<float>(Iter - 1));
	plot.Color_1 = cvScalar(0, 0, 0);
	plot.Color_2 = cvScalar(255, 255, 255);
	plot.plot(Y, cvScalar(255, 5, 5), '.', true);
	plot.title("Iterative Figure");
	plot.xlabel("Iteration", cvScalar(255, 0, 0));
	plot.ylabel("FitnessValue", cvScalar(255, 0, 0));
	cvShowImage("The FitnessValue Figure", plot.Figure);
	cvWaitKey(0);
}

void CPlatformDlg::OnBnClickedButtonGenfig2()
{
	if (imgCount == 1) {
		imgShow(img[imgCount - 1]);
	}
	else {
		for (int i = 0; i < imgCount; i++) {
			namedWindow("Image");
			imshow("Image", img[i]);
			waitKey(85);
		}
	}

}

void CPlatformDlg::OnBnClickedGenfig3()
{
	showHis(img_ori[imgCount - 1]);
}

void CPlatformDlg::OnBnClickedButtonGenerate()
{
	const int times = _ttoi(m_ET_RepTime);
	CString filter;
	filter = "JPG Files (*.txt)|*.txt|";
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter, this);
	std::string name;
	if (fileDlg.DoModal() == IDOK) {
		name = CT2A(fileDlg.GetPathName().GetString());
		fileEmpty(name);
		fstream output_stream;
		output_stream.open(name, ios::out | ios::app);
		output_stream << "< Optimization Algorithm: " << CT2A(algName.GetString())
			<< "	Segmentation Algorithm: " << CT2A(segAlgName.GetString()) << ">" << endl;
		output_stream << "<Iteration: " << _ttoi(m_ET_Iteration) << "	Population: "
			<< _ttoi(m_ET_Population) << "	Threshold number: " << _ttoi(m_ET_ThresholdNum)
			<< "	Repetition Times: " << _ttoi(m_ET_RepTime) << ">" << endl << endl;
		if (m_Ck_SD && m_Ck_FSIM && m_Ck_RT && m_Ck_FV) {
			output_stream << "       Fitness Value(FV)" << "              Structural Similarity(SSIM)"
				<< "             Running Time(RT)" << endl;
			for (int i = 0; i < times; i++) {
				output_stream << " " << setprecision(3) << i + 1 << fixed << setprecision(4) << "         " << fv_[i]
					<< "                                      " << ssim_[i] << "                                      " << rt_[i] << endl;
			}
			output_stream << endl << fixed << setprecision(4) << "Average Fitness Value(FV): " << CT2A(m_ET_FVShow.GetString())
				<< "	Average Structural Similarity(SSIM): " << CT2A(m_ET_FSIMShow.GetString())
				<< "	Running Time(RT): " << CT2A(m_ET_RTShow.GetString()) << endl;
			output_stream << fixed << setprecision(5) << "Standard Deviation: " << SD(fv_) << endl;
		}
		else {
			if (m_Ck_FV) {
				output_stream << "       Fitness Value(FV)" << endl;
				for (int i = 0; i < times; i++)
					output_stream << " " << setprecision(3) << i + 1 << fixed << setprecision(4) << "         " << fv_[i] << endl;
				output_stream << fixed << setprecision(4) << "Average Fitness Value(FV): " << CT2A(m_ET_FVShow.GetString()) << endl << endl;
			}
			if (m_Ck_FSIM) {
				output_stream << "    Structural Similarity(SSIM)" << endl << endl;
				for (int i = 0; i < times; i++)
					output_stream << " " << setprecision(3) << i + 1 << fixed << setprecision(4) << "         " << ssim_[i] << endl;
				output_stream << fixed << setprecision(4) << "Average Structural Similarity(SSIM): " << CT2A(m_ET_FSIMShow.GetString()) << endl << endl;
			}
			if (m_Ck_RT) {
				output_stream << "        Running Time(RT)" << endl;
				for (int i = 0; i < times; i++)
					output_stream << " " << setprecision(3) << i + 1 << fixed << setprecision(4) << "         " << rt_[i] << endl;
				output_stream << fixed << setprecision(4) << "Average Running Time(RT): " << CT2A(m_ET_RTShow.GetString()) << endl << endl;

			}
			if (m_Ck_SD)
				output_stream << fixed << setprecision(5) << "Standard Deviation: " << SD(fv_) << endl;
		}
		DONE_Dlg = new DONE();
		DONE_Dlg->Create(IDD_DIALOG4, this);
		DONE_Dlg->ShowWindow(SW_SHOW);
	}
}





