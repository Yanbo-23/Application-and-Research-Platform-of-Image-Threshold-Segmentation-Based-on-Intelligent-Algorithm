#pragma once

class STA_Config : public CDialogEx
{
	DECLARE_DYNAMIC(STA_Config)

public:
	STA_Config(CWnd* pParent = nullptr);   
	virtual ~STA_Config();
	STA_Config* m_pTipDlg;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	CString m_beta;
	CString m_alpha_min;
	CString m_delta;
	CString m_alpha_max;
	CString m_gamma;
	CString m_fc;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangealphamax();
	afx_msg void OnEnChangegamma();
	afx_msg void OnEnChangefc();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
