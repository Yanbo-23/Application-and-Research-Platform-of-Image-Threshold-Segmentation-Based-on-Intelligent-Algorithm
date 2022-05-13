#pragma once

class DONE : public CDialogEx
{
	DECLARE_DYNAMIC(DONE)

public:
	DONE(CWnd* pParent = nullptr); 
	virtual ~DONE();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
