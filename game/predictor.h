#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include<algorithm>
#include<vector>
#include"game.h"

class Predictor
{
private:
	Game &game;
	Board have;
	
public:
	Predictor(Game &_game, const Board &_have): game(_game), have(_have)
	{
		assert(game.getn() == have.getn());
		assert(game.getm() == have.getm());
		assert(game.getshown().fit(have));
		have.upd(game.getshown());
	}
	Predictor(Game &_game): Predictor(_game, _game.getshown()) {}
	
	bool click(Cell c)
	{
		if(have.get(c) != unknow_cell) return 0;
		game.click(c);
		have.upd(game.getshown());
		return 1;
	}
	bool setflag(Cell c, bool ingame = 1)
	{
		if(have.get(c) != unknow_cell) return 0;
		if(ingame)
			game.setflag(c);
		have.set(c, flag_cell);
		return 1;
	}
	Board gethave(void) const
	{
		return have;
	}
	void upd(void)
	{
		have.upd(game.getshown());
	}
	
	bool move(void)
	{
		if(game.getstatus() != 0) return 0;
		
		for(auto c: have.getallcell())
		{
			if(!c.gettype().isnumber()) continue;
			
			int k = c.gettype().get();
			
			int x = have.countnei(c, flag_cell);
			int y = have.countnei(c, unknow_cell);
			if(y == 0) continue;
			
			if(x + y == k)
			{
				for(Cell t: have.getnei(c))
					if(t.gettype() == unknow_cell)
					{
						setflag(t);
						return 1;
					}
			}
			if(x == k)
			{
				for(Cell t: have.getnei(c))
					if(t.gettype() == unknow_cell)
					{
						click(t);
						return 1;
					}
			}
		}
		return 0;
	}
	bool randmove(void)
	{
		if(game.getstatus() != 0) return 0;
		
		int n = have.getn();
		int m = have.getm();
		
		std :: vector<Cell> p;
		for(int i=0; i<n; ++i)
			for(int j=0; j<m; ++j)
				if(have.get(Cell(i,j)) == unknow_cell)
					p.emplace_back(i, j);
		
		if(!p.size()) return 0;
		
		Cell c = p[Rand :: rand(0, (int)p.size() - 1)];
		click(c);
		return 1;
	}
};

#endif
