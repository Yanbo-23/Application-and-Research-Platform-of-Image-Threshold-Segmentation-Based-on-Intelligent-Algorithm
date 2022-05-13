#pragma once

class GA_Config : public CDialogEx
{
	DECLARE_DYNAMIC(GA_Config)

public:
	GA_Config(CWnd* pParent = nullptr);   
	virtual ~GA_Config();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	CString m_etac;
	CString m_etam;
	CString m_Pc;
	CString m_Pm;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedButton1();
};
