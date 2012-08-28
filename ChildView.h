// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__75945A2A_B50C_11D6_97AC_0080C87FF08B__INCLUDED_)
#define AFX_CHILDVIEW_H__75945A2A_B50C_11D6_97AC_0080C87FF08B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum { BLANK,SNAKE,FOOD,NOUSE };
enum DIR { EAST,SOUTH,WEST,NORTH};

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GameOver(int row,int col);
	BOOL InBound(int row,int col);
	void Grow(int row,int col,CDC *pDC);
	void DrawCell(int row,int col,CDC *pDC);
	void DeleteTail(CDC *pDC);
	void move(int directery,CDC *pDC);
	void DrawBoard(CDC *pDC);
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnGameNew();
	afx_msg void OnMoveEast();
	afx_msg void OnMoveNorth();
	afx_msg void OnMoveSouth();
	afx_msg void OnMoveWest();
	afx_msg void OnUpdateGameNew(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bmCells;	
	int m_height;
	int m_width;
	BOOL m_start;

	int m_default;
	int m_olddirect;
	
	struct pos {
		int x;
		int y;
	}m_head,m_tail;
	struct node	{
		int state;
		struct pos m_position;
		struct pos m_next;
		struct pos m_fore;
	} m_board[40][30];
	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__75945A2A_B50C_11D6_97AC_0080C87FF08B__INCLUDED_)
