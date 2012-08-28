// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "snake.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_bmCells.LoadBitmap( IDB_CELLS );
	
 //m_width= rc.Width() /4;
//	m_height = rc.Height() /4;
	
	m_height=20;
	m_width=30;
	
	for( int row = 0; row < m_height; row ++ )
		for( int col = 0; col < m_width; col ++ )
		{
			m_board[row][col].state=BLANK;
			m_board[row][col].m_position.x =row;
			m_board[row][col].m_position.y =col;
		}
	m_board[9][10].state=SNAKE;
	m_board[9][9].state=SNAKE;
	m_head.x=9;
	m_head.y=10;
	m_tail.x=9;
	m_tail.y=9;
	m_board[m_tail.x][m_tail.y].m_next.x=9;
	m_board[m_tail.x][m_tail.y].m_next.y=10;
	m_default=EAST;
	m_board[12][12].state =FOOD;
	m_start=FALSE;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_GAME_NEW, OnGameNew)
	ON_COMMAND(ID_MOVE_EAST, OnMoveEast)
	ON_COMMAND(ID_MOVE_NORTH, OnMoveNorth)
	ON_COMMAND(ID_MOVE_SOUTH, OnMoveSouth)
	ON_COMMAND(ID_MOVE_WEST, OnMoveWest)
	ON_UPDATE_COMMAND_UI(ID_GAME_NEW, OnUpdateGameNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawBoard(&dc);
	AfxGetMainWnd()->SetWindowPos( NULL, 0, 0, 14+8*m_width, 
		51+8*m_height, SWP_NOMOVE | SWP_NOZORDER );

}


void CChildView::DrawBoard(CDC *pDC)
{

	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	CBitmap *pbmOld = (CBitmap *)(dcMem.SelectObject(&m_bmCells));
	
	for( int row = 0; row < m_height; row ++ )
		for( int col = 0; col < m_width; col ++ )
		{
			pDC->BitBlt( col*8, row*8, 8, 8, &dcMem, 
				0, m_board[row][col].state *8, SRCCOPY );
		}
	dcMem.SelectObject( pbmOld );

}

void CChildView::move(int directery,CDC *pDC)
{
		switch(directery)
		{
		case EAST:
		
			m_board[m_head.x][m_head.y].m_next.y=m_head.y+1;
			m_board[m_head.x][m_head.y].m_next.x=m_head.x ;
			m_head.y++;
			m_default=EAST;
			break;
		case SOUTH:
			m_board[m_head.x][m_head.y].m_next.y=m_head.y;
			m_board[m_head.x][m_head.y].m_next.x=m_head.x+1 ;

			m_head.x++;
			m_default=SOUTH;
			break;
		case WEST:
			m_board[m_head.x][m_head.y].m_next.y=m_head.y-1;
			m_board[m_head.x][m_head.y].m_next.x=m_head.x ;

			m_head.y--;
			m_default=WEST;
			break;
		case NORTH:
			m_board[m_head.x][m_head.y].m_next.y=m_head.y;
			m_board[m_head.x][m_head.y].m_next.x=m_head.x-1 ;

			m_head.x--;
			m_default=NORTH;
			break;
		}

		if (!InBound(m_head.x,m_head.y))
		{	
			GameOver(m_head.x,m_head.y);
		}
	
		if (m_board[m_head.x][m_head.y].state==FOOD)
		{
			int row,col;
			Grow(m_head.x,m_head.y,pDC);
			do{
			 row = rand() % m_height;
			 col = rand() % m_width;
			}while(m_board[row][col].state!=BLANK);
			m_board[row][col].state=FOOD;
			DrawCell(row,col,pDC);
		}
		else if (m_board[m_head.x][m_head.y].state==SNAKE)
		{
			GameOver(m_head.x,m_head.y);
		}
		else if (m_board[m_head.x][m_head.y].state ==BLANK)
		{
			Grow(m_head.x,m_head.y ,pDC);
			DeleteTail(pDC);
		}
		m_olddirect=m_default;

}

void CChildView::OnTimer(UINT nIDEvent) 
{
	
	CClientDC dc(this);
	move(m_default,&dc);
	CWnd ::OnTimer(nIDEvent);
}

void CChildView::OnGameNew() 
{
	CClientDC dc(this);
	SetTimer( 1, 200, NULL );
	m_start=!m_start;
}


void CChildView::DeleteTail(CDC *pDC)
{
	
	m_board[m_tail.x][m_tail.y].state =BLANK;
	DrawCell(m_tail.x,m_tail.y,pDC);
	int tmp_x =m_board[m_tail.x][m_tail.y].m_next.x;
	int tmp_y =m_board[m_tail.x][m_tail.y].m_next.y;
	m_tail.x =tmp_x;
	m_tail.y =tmp_y;
}

void CChildView::DrawCell(int row, int col, CDC *pDC)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	CBitmap *pbmOld = (CBitmap *)(dcMem.SelectObject(&m_bmCells));
	pDC->BitBlt( col*8, row*8, 8, 8, &dcMem, 
			0, m_board[row][col].state *8, SRCCOPY );
	dcMem.SelectObject( pbmOld );
}

void CChildView::OnMoveEast() 
{
	if(m_default==WEST||m_olddirect!=m_default)
		return;
	m_default=EAST;
}

void CChildView::OnMoveNorth() 
{
	if (m_default==SOUTH||m_olddirect!=m_default)
		return;
	m_default=NORTH;
}

void CChildView::OnMoveSouth() 
{
	if(m_default==NORTH||m_olddirect!=m_default)
		return;
	m_default=SOUTH;
}

void CChildView::OnMoveWest() 
{
	if(m_default==EAST||m_olddirect!=m_default)
		return;
	m_default=WEST;
}

void CChildView::Grow(int row,int col ,CDC *pDC)
{
	m_board[row][col].state =SNAKE;
	DrawCell(row,col,pDC);
}

BOOL CChildView::InBound(int row, int col)
{
	return (row >= 0 && row < m_height &&
			col >= 0 && col < m_width);
}

void CChildView::GameOver(int row, int col)
{
	KillTimer( 1 );
	AfxMessageBox ("game over",MB_OK,0);
}

void CChildView::OnUpdateGameNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_start);
}
