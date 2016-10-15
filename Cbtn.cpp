#include "stdafx.h"
#include "Cbtn.h"


Cbtn::Cbtn(int flags)
	:CButton(),
	m_color(RGB(0xCD, 0xE7, 0xE1)),
	m_crForeGround(RGB(0x0F, 0x1E, 0x48)),
	m_MouseOnButton(FALSE),
	FLAGS(flags)
	, m_bTracking(false)
{
	m_color_static = m_color;
	m_crForeGround_static = m_crForeGround;
}


Cbtn::~Cbtn()
{
}


// change color
void Cbtn::ChangeColor(COLORREF color, COLORREF txtcolor)
{
	CRect rect;

	m_color = color;
	m_color_static = color;
	m_crForeGround = txtcolor;
	m_crForeGround_static = txtcolor;

	GetClientRect(&rect);
}


void Cbtn::ChangeColor(COLORREF color)
{
	CRect rect;

	BYTE r = GetRValue(color);
	BYTE g = GetGValue(color);
	BYTE b = GetBValue(color);

	int rr = r > 0x80 ? 0 : 0xff;
	int gg = g > 0x80 ? 0 : 0xff;
	int bb = b > 0x80 ? 0 : 0xff;

	COLORREF txtcolor = RGB(rr, gg, bb);

	m_color = color;
	m_color_static = color;
	m_crForeGround = txtcolor;
	m_crForeGround_static = txtcolor;

	GetClientRect(&rect);
}


// ���صĺ���
void Cbtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC); //�õ����Ƶ��豸����CDC

	CString strCaption; // ��ť����

	// �õ���ť���⣬�Ա���������������DC���ػ���ť����
	GetWindowText(strCaption);

	// ���ñ���ģʽΪ͸���������ڻ���ť����ʱ������ı����Ჿ�ֵظ��ǵ���ť�ı���
	dc.SetBkMode(TRANSPARENT);

	VERIFY(lpDrawItemStruct->CtlType == ODT_BUTTON);

	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)

	{
		//�ػ���������
		CBrush brush(m_color);

		dc.FillRect(&(lpDrawItemStruct->rcItem), &brush);// ���İ�ť�ı���ɫ��ֻҪ�����DC��Ϳ������Ҫ�ı���ɫ����
		dc.SetTextColor(m_crForeGround);      // ����ǰ��ɫ
		dc.DrawText(strCaption, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	int act = lpDrawItemStruct->itemAction;
	int sta = lpDrawItemStruct->itemState;

	//ѡ��
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brushfill(rgbmin(m_color,50));
		dc.FillRect(&(lpDrawItemStruct->rcItem), &brushfill);//��ť����

		dc.SetTextColor(rgbadd(m_crForeGround,180));
		dc.DrawText(strCaption, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);//����ɫ�ı�
	}

	//����
	if ((lpDrawItemStruct->itemState & ODS_FOCUS) && (lpDrawItemStruct->itemAction & (ODA_FOCUS | ODA_DRAWENTIRE)) && ((FLAGS&FOCUS_FRAM)==FOCUS_FRAM))
	{

		COLORREF fc =
			//RGB(255, 255, 255);
			RGB(255 - GetRValue(m_color), 255 - GetGValue(m_color), 255 - GetBValue(m_color));
		CBrush brush(fc);
		dc.FrameRect(&(lpDrawItemStruct->rcItem), &brush);//�����߿�

		dc.SetTextColor(rgbadd(m_crForeGround,50));
		dc.DrawText(strCaption, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);//����ɫ�ı�
	}

	//disable
	if ((lpDrawItemStruct->itemState & ODS_DISABLED) && (lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE)))
	{
		//gray
		CBrush brushfill(RGB(0xDD,0xDD,0xDD));
		dc.FillRect(&(lpDrawItemStruct->rcItem), &brushfill);//��ť���

		dc.SetTextColor(RGB(0x88, 0x88, 0x88));
		dc.DrawText(strCaption, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);//��Ȼ��û����
	}

	//????no use
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		CBrush brush(m_color);
		dc.FillRect(&(lpDrawItemStruct->rcItem), &brush);
	}


	dc.Detach();

}

COLORREF Cbtn::rgbmin(COLORREF ref, int mi)
{
	int Cr = GetRValue(ref);
	int Cg = GetGValue(ref);
	int Cb = GetBValue(ref);
	
	Cr -= mi;
	if (Cr < 0)
		Cr = 0;
	
	Cg -= mi;
	if (Cg < 0)
		Cg = 0;
	
	Cb -= mi;
	if (Cb < 0)
		Cb = 0;
	return RGB(Cr, Cg, Cb);
}


COLORREF Cbtn::rgbadd(COLORREF ref, int ad)
{
	int Cr = GetRValue(ref);
	int Cg = GetGValue(ref);
	int Cb = GetBValue(ref);

	Cr += ad;
	if (Cr > 255)
		Cr = 255;

	Cg += ad;
	if (Cg > 255)
		Cg = 255;

	Cb += ad;
	if (Cb > 255)
		Cb = 255;
	return RGB(Cr, Cg, Cb);
}

BEGIN_MESSAGE_MAP(Cbtn, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()


void Cbtn::OnMouseMove(UINT nFlags, CPoint point)
{

	CButton::OnMouseMove(nFlags, point);

	//CWnd* pWnd;  // Finestra attiva
	//CWnd* pParent; // Finestra che contiene il bottone

	// If the mouse enter the button with the left button pressed
	// then do nothing
	if (nFlags & MK_LBUTTON && m_MouseOnButton == FALSE) return;

	//if (!m_bTracking)
	if ((FLAGS & (ON_DEEPER)) && !m_bTracking)
	{
		//������봰ʱ������WM_MOUSEHOVER��WM_MOUSELEAVE ��Ϣ  
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
}


void Cbtn::CCol(COLORREF BG, COLORREF FG)
{
	CRect rect;

	m_color = BG;
	m_crForeGround = FG;

	GetClientRect(&rect);
}


void Cbtn::OnMouseLeave()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bTracking = FALSE;
	m_MouseOnButton = FALSE;
	CCol(m_color_static, m_crForeGround_static);
	Invalidate();

	CButton::OnMouseLeave();
}


void Cbtn::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (((FLAGS & ON_DEEPER) == ON_DEEPER) && (m_MouseOnButton == FALSE)){
		CCol(rgbmin(m_color, 30), rgbadd(m_crForeGround));
		Invalidate();
	}

	m_MouseOnButton = TRUE;
	
	CButton::OnMouseHover(nFlags, point);
}


void Cbtn::SetOnDeeper(bool onDeep)
{
	if (onDeep)
	{
		FLAGS |= ON_DEEPER;
	}
	else
	{
		FLAGS &= ON_NO_DEEPER;
	}
}


void Cbtn::SetFocusFram(bool focusFram)
{
	if (focusFram)
	{
		FLAGS |= FOCUS_FRAM;
	}
	else
	{
		FLAGS &= FOCUS_NO_FRAM;
	}
}
