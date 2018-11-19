// Coefficient.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PyroSoftM.h"
#include "CoefficientDlg.h"
#include "afxdialogex.h"


// CCoefficient ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCoefficientDlg, CDialogEx)

CCoefficientDlg::CCoefficientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COEFFICIENT_SET, pParent)
{

}

CCoefficientDlg::~CCoefficientDlg()
{
}

void CCoefficientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THIRD_COEFF, m_Third_Coeff);
	DDX_Control(pDX, IDC_SECOND_COEFF, m_Second_Coeff);
	DDX_Control(pDX, IDC_FIRST_COEFF, m_First_Coeff);
	DDX_Control(pDX, IDC_CONSTANT, m_Constant);
}


BEGIN_MESSAGE_MAP(CCoefficientDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ACCEPT, &CCoefficientDlg::OnBnClickedAccept)
	ON_BN_CLICKED(IDC_CANCEL, &CCoefficientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CCoefficient �޽��� ó�����Դϴ�.
void CCoefficientDlg::OnBnClickedAccept()
{
	CString str;
	float tempFirst = 0.0f, tempSecond = 0.0f, tempThird = 0.0f, tempIntercept = 0.0f;

	m_First_Coeff.GetWindowTextA(str);
	tempFirst = _ttof(str);
	CheckEmptyValue(str);

	m_Second_Coeff.GetWindowTextA(str);
	tempSecond = _ttof(str);
	CheckEmptyValue(str);

	m_Third_Coeff.GetWindowTextA(str);
	tempThird = _ttof(str);
	CheckEmptyValue(str);

	m_Constant.GetWindowTextA(str);
	tempIntercept = _ttof(str);
	CheckEmptyValue(str);


	if (m_EmptyStringFlag >= 4)
	{
		AfxMessageBox("�������� �ʴ� ���� �����մϴ�.");	// -�����ӹ�
		m_EmptyStringFlag = 0;
	}
	else
	{
		pDoc->m_FirstCeoff = tempFirst;
		pDoc->m_SecondCeoff = tempSecond;
		pDoc->m_ThirdCeoff = tempThird;
		pDoc->m_Intercept = tempIntercept;

		AfxMessageBox("���� ���� �Ǿ����ϴ�.");
		OnBnClickedCancel();
	}
}

void CCoefficientDlg::CheckEmptyValue(CString str)
{
	if (str == "" || str == "0")
		m_EmptyStringFlag++;
}


void CCoefficientDlg::OnBnClickedCancel()
{
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}


BOOL CCoefficientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_EmptyStringFlag = 0;
	PrintZoneInf();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CCoefficientDlg::PreTranslateMessage(MSG* pMsg)
{
	//����Ű�� ������ UPdate�� �� �ֵ��� 
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData(TRUE);

		OnBnClickedAccept();

		pDoc->UpdateAllViews(NULL, 1);
		pDoc->UpdateAllViews(NULL, 7);
		return FALSE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
		return FALSE;
	}
	else
		return CDialogEx::PreTranslateMessage(pMsg);

}

void CCoefficientDlg::PrintZoneInf()
{
	CString str;

	str.Format(_T("%.4f"), pDoc->m_ThirdCeoff);
	m_Third_Coeff.SetWindowText(str);

	str.Format(_T("%.4f"), pDoc->m_SecondCeoff);
	m_Second_Coeff.SetWindowText(str);

	str.Format(_T("%.4f"), pDoc->m_FirstCeoff);
	m_First_Coeff.SetWindowText(str);

	str.Format(_T("%d"), pDoc->m_Intercept);
	m_Constant.SetWindowText(str);
}
