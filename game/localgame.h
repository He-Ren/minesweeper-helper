#ifndef _LOCALGAME_H_
#define _LOCALGAME_H_

#include<cassert>
#include<algorithm>
#include<vector>
#include<chrono>
#include"game.h"

class Local_Game: Game
{
private:
	int n, m, d;
	Board board, shown;
	bool inited;
	int status;// -1 = failed, 0 = playing, 1 = succeed
	int showncnt;
	
	void init(Cell fob)
	{
		std :: mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
		
		inited = 1;
		status = 0;
		showncnt = 0;
		board = Board(n, m, uninit_cell);
		shown = Board(n, m, unknow_cell);
		
		std :: vector<Cell> p;
		for(int i=0; i<n; ++i)
			for(int j=0; j<m; ++j)
			{
				if(i == fob.getx() && j == fob.gety())
					continue;
				p.emplace_back(i, j);
			}
		shuffle(p.begin(), p.end(), gen);
		p.push_back(fob);
		
		for(int i=0; i<d; ++i)
			board.set(p[i], mine_cell);
		for(int i=d; i<(int)p.size(); ++i)
			board.set(p[i], Cell_Type( board.countnei(p[i], mine_cell) ));
	}
	
public:
	Local_Game(void)
		: n(0), m(0), d(0), board(), shown(), status(0), showncnt(0)
	{}
	
	Local_Game(int _n,int _m,int _d):
		n(_n), m(_m), d(_d), board(), shown(n, m, unknow_cell), status(0), showncnt(0)
	{
		assert(n >= 0 && m >= 0);
		assert(0 <= d && d < n * m);
	}
	
	Cell getcell(int x,int y) const
	{
		return Cell(x, y, shown.get(x,y));
	}
	Cell getcell(Cell c) const
	{
		return getcell(c.getx(), c.gety());
	}
	Board getshown(void) const
	{
		return shown;
	}
	Board getreal(void) const
	{
		if(status == 0)
			return Board(n, m, uninit_cell);
		else
			return board;
	}
	int getstatus(void) const
	{
		return status;
	}
	
	int click(int x,int y)
	{
		if(!inited) init(Cell(x, y));
		
		if(status != 0) return 0;
		if(!board.in(x,y)) return 0;
		if(shown.get(x,y) != unknow_cell) return 0;
		
		if(board.get(x,y) == mine_cell)
		{
			status = -1;
			shown.set(x,y,mine_cell);
			return -1;
		}
		
		shown.set(x, y, board.get(x,y));
		if(board.get(x,y).isempty())
		{
			for(auto t: board.getnei(x,y))
				if(t.gettype().ismine() == 0)
					click(t.getx(), t.gety());
		}
		
		++showncnt;
		if(showncnt == n * m - d)
			status = 1;
		
		return 1;
	}
};

#endif
