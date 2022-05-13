
#pragma once


class CPlatformDlg : public CDialogEx
{

public:
	CPlatformDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLATFORM_DIALOG };
	
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCHECKMoTest();
	afx_msg void OnBUTTONInputSingle();
	afx_msg void OnBUTTONParConfig();
	afx_msg void OnBUTTONSaveRes();
	afx_msg void OnBUTTONStartExp();
	afx_msg void OnBUTTONStartSeg();
	afx_msg void OnBUTTONInputALot();
	afx_msg void OnCheckFsim();
	afx_msg void OnCheckFv();
	afx_msg void OnCheckRt();
	afx_msg void OnCheckSd();
	afx_msg void OnChangeEDITRepTime();
	afx_msg void OnCloseupCOMBOAlgSelect();
	afx_msg void OnCloseupCOMBOSegAlgName();
	afx_msg void OnChangeEDITThresholdNum();
	DECLARE_MESSAGE_MAP()
public:
	CButton	m_BT_Generate_;
	CButton	m_BT_StartExp_;
	CButton	m_BT_StartSeg_;
	CStatic	m_WD_RT;
	CStatic	m_WD_Path;
	CStatic	m_WD_FV;
	CButton	m_Ck_SD_;
	CStatic	m_WD_FSIM;
	CButton	m_GP_Index;
	CButton	m_GP_Result;
	CButton	m_GP_InputData;
	CComboBox	m_CB_AlgSelect_;
	CComboBox	m_CB_SegAlgName_;
	CStatic	m_WD_ThresNum;
	CStatic	m_WD_SegTime;
	CStatic	m_WD_SegAlgName;
	CStatic	m_WD_SegProg;
	CStatic	m_WD_RepTime;
	CStatic	m_WD_ExpTime;
	CStatic	m_WD_ExpProg;
	CStatic	m_WD_AlgSelect;
	CProgressCtrl	m_PG_Seg;
	CProgressCtrl	m_PG_Exp;
	CButton	m_GP_ThreAlgChos;
	CButton	m_GP_TestConfig;
	CButton	m_GP_ModeChos;
	CButton	m_GP_IndexChos;
	CButton	m_GP_FunChos;
	CButton	m_GP_AlgChos;
	CEdit	m_ET_ThresholdNum_;
	CEdit	m_ET_SegTime_;
	CEdit	m_ET_RepTime_;
	CEdit	m_ET_Path_;
	CEdit	m_ET_FVShow_;
	CEdit	m_ET_ExpTime_;
	CEdit	m_ET_FSIMShow_;
	CButton	m_Ck_RT_;
	CButton	m_Ck_MoTest_;
	CButton	m_Ck_MoApl_;
	CButton	m_Ck_FV_;
	CButton	m_Ck_FSIM_;
	CButton	m_BT_SaveRes;
	CButton	m_BT_ParConfig;
	CButton	m_BT_InputSingle;
	CButton	m_BT_InputCode;
	CButton	m_BT_InputALot;
	CButton	m_BT_GenFig2;
	CButton	m_BT_GenFig1;
	BOOL	m_Ck_MoTest;
	BOOL	m_Ck_RT;
	BOOL	m_Ck_MoApl;
	BOOL	m_Ck_FV;
	BOOL	m_Ck_FSIM;
	CString	m_ET_FSIMShow;
	CString	m_ET_ExpTime;
	CString	m_ET_FVShow;
	CString	m_ET_Path;
	CString	m_ET_RepTime;
	CString	m_ET_RTShow;
	CString	m_ET_SegTime;
	CString	m_ET_ThresholdNum;
	CString	m_CB_SegAlgName;
	CString	m_CB_AlgSelect;
	BOOL	m_Ck_SD;
	CString	m_ImageNm;
	int m_BT_StartSeg;
	int m_BT_StartExp;
	int m_BT_Generate;
	afx_msg void OnBnClickedButtonGenfig2();
	afx_msg void OnBnClickedButtonGenfig1();
	CButton m_BT_GenFig3_;
	afx_msg void OnBnClickedGenfig3();
	CEdit m_ET_Iteration_;
	CString m_ET_Iteration;
	CEdit m_ET_Population_;
	CString m_ET_Population;
	afx_msg void OnEnChangeEtIteration();
	afx_msg void OnEnChangeEtPopulation();
	afx_msg void OnEnChangeEditPath();
	CStatic m_WD_Population;
	CStatic m_WD_Iteration;
	CButton m_BT_ShowImg;
	afx_msg void OnBnClickedBtShowimg();
	CEdit m_ET_RTShow_;
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnCHECKMoApl();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
