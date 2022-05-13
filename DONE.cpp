#include "pch.h"
#include "Platform.h"
#include "DONE.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(DONE, CDialogEx)

DONE::DONE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

DONE::~DONE()
{
}

void DONE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DONE, CDialogEx)
END_MESSAGE_MAP()


