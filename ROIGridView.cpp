// StatusView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PyroSoftM.h"
#include "ROIGridView.h"


// CStatusView

IMPLEMENT_DYNCREATE(CROIGridView, CView)

CROIGridView::CROIGridView()
{
}

CROIGridView::~CROIGridView()
{
}

BEGIN_MESSAGE_MAP(CROIGridView, CView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CStatusView �׸����Դϴ�.

void CROIGridView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

}


// CStatusView �����Դϴ�.

#ifdef _DEBUG
void CROIGridView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CROIGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStatusView �޽��� ó�����Դϴ�.

int CROIGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (m_ROIGridDlg.GetSafeHwnd() == 0)
	{
		if (!m_ROIGridDlg.Create(IDD_STATUS_INF, this))
			return FALSE;

		m_ROIGridDlg.ModifyStyle(0, WS_VISIBLE, 0);

		AdjustLayout();
	}

	return 0;
}


void CROIGridView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int dlgH = 0;

	if (theApp.adjust_count > 4)
		theApp.nStatusDlgHeight = rectClient.Width();

	m_ROIGridDlg.SetWindowPos(NULL,
		rectClient.left,
		rectClient.top + dlgH,
		rectClient.Width(),
		rectClient.Height() - dlgH,
		SWP_NOACTIVATE | SWP_NOZORDER);
	//SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE);
}


void CROIGridView::InitStatusData()
{
	m_ROIGridDlg.m_ROI_GridView.Clear();

	m_ROIGridDlg.m_ROI_GridView.put_AllowUserResizing(3);					// Allow column, row mouse resizing
	m_ROIGridDlg.m_ROI_GridView.put_Row(1);										// trace row
	m_ROIGridDlg.m_ROI_GridView.put_Cols(pDoc->m_ROICount + 1);		// trace column

																	// row #1 naming
	for (int i = 0; i < pDoc->m_ROICount + 1; i++) {
		CString dummy = _T("");
		dummy.Format("%d", i);
		m_ROIGridDlg.m_ROI_GridView.put_ColAlignment(i, 4);					// flexAlignCenterCenter
		m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(0, i, (CString)"ROI #" + dummy);
		m_ROIGridDlg.m_ROI_GridView.put_ColWidth(i, m_ROIGridDlg.controlWidth*1.8);
	}

	// column #1 naming
	m_ROIGridDlg.m_ROI_GridView.put_Rows(4);
	m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(0, 0, _T(""));
	m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(1, 0, _T("Min Temp"));
	m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(2, 0, _T("Max Temp"));
	m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(3, 0, _T("Avg Temp"));

	// height adjust
	for (int i = 1; i < 4; i++) {
		m_ROIGridDlg.m_ROI_GridView.put_RowHeight(i, 380);
	}

	// �߾� ������ column, row�� ������ ������ cell�� �⺻ ǥ�� ������� ����
	for (int i = 1; i < 4; i++) {
		m_ROIGridDlg.m_ROI_GridView.put_Row(i);
		for (int j = 1; j < pDoc->m_ROICount + 1; j++) {
			m_ROIGridDlg.m_ROI_GridView.put_Col(j);
			m_ROIGridDlg.m_ROI_GridView.put_CellAlignment(9);	// flexAlignGeneral
		}
	}
}

void CROIGridView::SetFont()
{

}

void CROIGridView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint<2) return;

	if (lHint < 4) {
		//���� Updata�� ���۽�Ű�� �κ�
		UpdateStatusData();
	}
}


void CROIGridView::UpdateStatusData()
{
	UpdateGridControlSet();

	CString str;

	int indexDummy = pDoc->m_POI_count + 1;

	// 171211 VERIFYING RESULTDATA ELEMENTS =================================
	int arrayElementCnt = sizeof(pDoc->m_ResultData.TMin) / sizeof(pDoc->m_ResultData.TMin[0]);

	if (pDoc->m_ROICount != arrayElementCnt) {
		for (int i = pDoc->m_ROI_loop_count; i < arrayElementCnt; i++) {
			pDoc->m_ResultData.TMin[i] = 0.0f;
			pDoc->m_ResultData.TMax[i] = 0.0f;
			pDoc->m_ResultData.TAvg[i] = 0.0f;
		}
	}
	// ================================= 171211 VERIFYING RESULTDATA ELEMENTS

	// Point Of Interest str
	// POI�� 1 �̻��̸� POI + ROI �µ����� ��� �޾ƿͼ� grid�� �ѷ��ش�
	if (pDoc->m_POI_count >= 1) {
		for (int i = 0; i < pDoc->m_POI_count; i++) {
			if (pDoc->POI_TemperatureArray[i] > 0 && pDoc->POI_TemperatureArray[i] <= 2000)
				str.Format("%.1f", pDoc->POI_TemperatureArray[i]);
			else
				str.Format("%.1f", 0.0f);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(1, i + 1, str);
		}

		for (int i = 0; i < pDoc->m_ROICount; i++) {
			if (pDoc->m_ResultData.TMin[i] > 0 && pDoc->m_ResultData.TMin[i] <= 2000)
				str.Format("%.1f", pDoc->m_ResultData.TMin[i]);
			else
				str.Format("%.1f", 0.0f);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(2, indexDummy, str);
			indexDummy++;
		}

		indexDummy = pDoc->m_POI_count + 1;
		for (int i = 0; i < pDoc->m_ROICount; i++) {
			if (pDoc->m_ResultData.TMax[i] > 0 && pDoc->m_ResultData.TMax[i] <= 2000)
				str.Format("%.1f", pDoc->m_ResultData.TMax[i]);
			else
				str.Format("%.1f", 0.0f);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(3, indexDummy, str);
			indexDummy++;
		}

		indexDummy = pDoc->m_POI_count + 1;
		for (int i = 0; i < pDoc->m_ROICount; i++) {
			if (pDoc->m_ResultData.TAvg[i] > 0 && pDoc->m_ResultData.TAvg[i] <= 2000)
				str.Format("%.1f", pDoc->m_ResultData.TAvg[i]);
			else
				str.Format("%.1f", 0.0f);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(4, indexDummy, str);
			indexDummy++;
		}
	}
	else {		// POI�� ������ ROI �µ����� �����ָ� �ȴ�
		for (int i = pDoc->m_POI_count; i < pDoc->m_ROICount; i++) {
			if (pDoc->m_ResultData.TMin[i] > 0 && pDoc->m_ResultData.TMin[i] <= 2000)
				str.Format("%.1f", pDoc->m_ResultData.TMin[i]);
			else
				str.Format("%.1f", 0.0f);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(1, i + 1, str);
		}

		for (int i = pDoc->m_POI_count; i < pDoc->m_ROICount; i++) {
			if (pDoc->m_ResultData.TMax[i] > 0 && pDoc->m_ResultData.TMax[i] <= 2000)
				str.Format("%.1f", pDoc->m_ResultData.TMax[i]);
			else
				str.Format("%.1f", 0.0f);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(2, i + 1, str);
		}

		for (int i = pDoc->m_POI_count; i < pDoc->m_ROICount; i++) {
			if (pDoc->m_ResultData.TAvg[i] > 0 && pDoc->m_ResultData.TAvg[i] <= 2000)
				str.Format("%.1f", pDoc->m_ResultData.TAvg[i]);
			else
				str.Format("%.1f", 0.0f);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(3, i + 1, str);
		}
	}
}


void CROIGridView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CROIGridView::OnSize(UINT nType, int cx, int cy)
{
	if (GetSafeHwnd() == NULL) return;

	CView::OnSize(nType, cx, cy);

	AdjustLayout();
}

void CROIGridView::UpdateGridControlSet() {
	if (pDoc->columnUpdateFlag == true) {

		// for redraw of grid control
		m_ROIGridDlg.m_ROI_GridView.Clear();
		m_ROIGridDlg.m_ROI_GridView.put_Row(1);
		int columnTemp = (pDoc->m_POI_count) + (pDoc->m_ROICount) + 1;

		// 171128 IMPLEMENTED ========================================================
		// POI�� �ϳ� �̻��� ��쿡�� ����,
		if (pDoc->m_POI_count >= 1) {
			m_ROIGridDlg.m_ROI_GridView.put_Cols(columnTemp);			// trace column
																		//for (int i = 0; i < pDoc->m_POI_count; i++) {
			for (int i = 0; i < pDoc->RunningIndex.GetCount(); i++) {
				CString dummy = _T("");
				int indexDummy = pDoc->RunningIndex[i] + 1;
				dummy.Format("POI #%d", indexDummy);
				m_ROIGridDlg.m_ROI_GridView.put_ColAlignment(i + 1, 4);
				m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(0, i + 1, dummy);
				m_ROIGridDlg.m_ROI_GridView.put_ColWidth(i + 1, m_ROIGridDlg.controlWidth*1.8);
			}
			int indexDummy = pDoc->m_POI_count + 1;
			for (int i = 1; i < pDoc->m_ROICount + 1; i++) {
				CString dummy = _T("");
				dummy.Format("%d", i);
				m_ROIGridDlg.m_ROI_GridView.put_ColAlignment(indexDummy, 4);
				m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(0, indexDummy, (CString)"ROI #" + dummy);
				m_ROIGridDlg.m_ROI_GridView.put_ColWidth(indexDummy, m_ROIGridDlg.controlWidth*1.8);
				indexDummy++;
			}
			for (int i = 1; i < pDoc->m_POI_count + 1; i++) {
				m_ROIGridDlg.m_ROI_GridView.put_Row(1);
				m_ROIGridDlg.m_ROI_GridView.put_Col(i);
				// POI �µ��� ���� �κ��� ���� ���� gray
				m_ROIGridDlg.m_ROI_GridView.put_CellBackColor(0xCECECE);	//0xBBGGRR
			}
		}
		else {
			m_ROIGridDlg.m_ROI_GridView.put_Cols(pDoc->m_ROICount + 1);	// trace column for roiCount
			for (int i = 1; i < pDoc->m_ROICount + 1; i++) {
				CString dummy = _T("");
				dummy.Format("%d", i);
				m_ROIGridDlg.m_ROI_GridView.put_ColAlignment(i, 4);
				m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(0, i, (CString)"ROI #" + dummy);
				m_ROIGridDlg.m_ROI_GridView.put_ColWidth(i, m_ROIGridDlg.controlWidth*1.8);
			}
		}
		// ======================================================== 171129 IMPLEMENTED

		// column #1 naming
		if (pDoc->m_POI_count >= 1) {
			m_ROIGridDlg.m_ROI_GridView.put_Rows(5);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(0, 0, _T(""));
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(1, 0, _T("POI Temp"));
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(2, 0, _T("Min Temp"));
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(3, 0, _T("Max Temp"));
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(4, 0, _T("Avg Temp"));

			for (int i = 1; i < 5; i++) {
				m_ROIGridDlg.m_ROI_GridView.put_RowHeight(i, 380);
			}

			// ù��° column, row���� text alignment ����
			for (int i = 1; i < 5; i++) {
				m_ROIGridDlg.m_ROI_GridView.put_Row(i);
				for (int j = 1; j < columnTemp; j++) {
					m_ROIGridDlg.m_ROI_GridView.put_Col(j);
					m_ROIGridDlg.m_ROI_GridView.put_CellAlignment(9);
				}
			}
		}
		else {
			m_ROIGridDlg.m_ROI_GridView.put_Rows(4);
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(0, 0, _T(""));
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(1, 0, _T("Min Temp"));
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(2, 0, _T("Max Temp"));
			m_ROIGridDlg.m_ROI_GridView.put_TextMatrix(3, 0, _T("Avg Temp"));

			for (int i = 1; i < 4; i++) {
				m_ROIGridDlg.m_ROI_GridView.put_RowHeight(i, 380);
			}

			// ù��° column, row���� text alignment ����
			for (int i = 1; i < 4; i++) {
				m_ROIGridDlg.m_ROI_GridView.put_Row(i);
				for (int j = 1; j < columnTemp; j++) {
					m_ROIGridDlg.m_ROI_GridView.put_Col(j);
					m_ROIGridDlg.m_ROI_GridView.put_CellAlignment(9);
				}
			}
		}
	}
	// control redraw end
	pDoc->columnUpdateFlag = false;
}
