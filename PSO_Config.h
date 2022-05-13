#pragma once

class PSO_Config : public CDialogEx
{
	DECLARE_DYNAMIC(PSO_Config)

public:
	PSO_Config(CWnd* pParent = nullptr);   
	virtual ~PSO_Config();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	CString m_w;
	CString m_wdamp;
	CString w_c1;
	CString w_c2;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnBnClickedButton1();
};
