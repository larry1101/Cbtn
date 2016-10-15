#pragma once
class Cbtn :
	public CButton
{
public:
	Cbtn(int FLAGS = 0x0001);
	~Cbtn();
public:
	void ChangeColor(COLORREF color, COLORREF txtcolor);
	void ChangeColor(COLORREF color);
	void SetOnDeeper(bool onDeep = true);
	void SetFocusFram(bool focusFram = true);
public:
	const static int ON_NO_DEEPER = 0xFFFE;
	const static int ON_DEEPER = 0x0001;
	const static int FOCUS_FRAM = 0x0010;
	const static int FOCUS_NO_FRAM = 0xFFEF;
private:
	COLORREF m_color;
	COLORREF m_crForeGround;
	COLORREF m_color_static;
	COLORREF m_crForeGround_static;
	COLORREF rgbmin(COLORREF ref, int mi=20);
	COLORREF rgbadd(COLORREF ref, int ad=20);
	BOOL m_MouseOnButton;
	bool m_bTracking;
	int FLAGS;
	void CCol(COLORREF BG, COLORREF FG);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};

