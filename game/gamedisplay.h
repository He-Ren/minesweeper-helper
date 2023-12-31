#ifndef _GAMEDISPLAY_H_
#define _GAMEDISPLAY_H_

#include<string>
#include"game.h"

class Game_Display: public Game
{
private:
	Game &game;
	Cell pos;
public:
	Game_Display(Game &_game): game(_game), pos(0,0) {}
	
	int getn(void)       const { return game.getn();}
	int getm(void)       const { return game.getm();}
	int getd(void)       const { return game.getd();}
	bool in(Cell c)      const { return game.in(c); }
	Cell getcell(Cell c) const { return game.getcell(c);}
	Board getshown(void) const { return game.getshown();}
	Board getreal(void)  const { return game.getreal();}
	int getstatus(void)  const { return game.getstatus();}
	int getremain(void)  const { return game.getremain();}
	
	int click(Cell c)
	{
		if(!game.in(c)) return 0;
		pos = c;
		return click();
	}
	bool setflag(Cell c)
	{
		if(!game.in(c)) return 0;
		pos = c;
		return setflag();
	}
	
	int click(void)
	{
		return game.click(pos);
	}
	bool setflag(void)
	{
		return game.setflag(pos);
	}
	bool move(int dx,int dy)
	{
		Cell npos(pos.getx() + dx, pos.gety() + dy);
		if(game.in(npos)){ pos = npos; return 1;}
		else return 0;
	}
	std :: string show(void) const
	{
		auto a = game.getshown();
		int n = a.getn(), m = a.getm();
		
		std :: vector< std :: string > s(2 * n + 1);
		for(int i=0; i<(int)s.size(); ++i)
			s[i].resize(2 * m + 1, ' ');
		for(int i=0; i<n; ++i)
			for(int j=0; j<m; ++j)
			{
				char c;
				auto t = a.get(Cell(i,j));
				
				if(t == uninit_cell)
					c = '/';
				else if(t == unknow_cell)
					c = 'U';
				else if(t == flag_cell)
					c = 'F';
				else if(t == mine_cell)
					c = 'X';
				else if(t == empty_cell)
					c = ' ';
				else if(t.isnumber())
					c = t.get() + '0';
				else
					c = 'E';
				
				s[i*2+1][j*2+1] = c;
			}
		
		{
			int x = pos.getx() * 2 + 1;
			int y = pos.gety() * 2 + 1;
			
			s[x][y - 1] = '|';
			s[x][y + 1] = '|';
			s[x - 1][y] = '-';
			s[x + 1][y] = '-';
			s[x - 1][y - 1] = '+';
			s[x - 1][y + 1] = '+';
			s[x + 1][y - 1] = '+';
			s[x + 1][y + 1] = '+';
		}
		
		std::string res;
		for(const auto &t: s)
			res += t + '\n';
		return res;
	}
};

#endif
