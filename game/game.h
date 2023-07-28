#ifndef _GAME_H_
#define _GAME_H_

#include<cassert>
#include<algorithm>
#include<vector>
#include<chrono>
#include<random>

namespace Rand
{
	std :: mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	
	int rand(int l,int r)
	{
		return std :: uniform_int_distribution<int>(l,r)(gen);
	}
}

class Cell_Type
{
private:
	int k;// -4 = uninit, -3 = unknow, -2 = flag, -1 = mine, 0 ~ 8 = number
	
public:
	explicit Cell_Type(int _k = -4): k(_k)
	{
		assert(-4 <= k && k <= 8);
	}
	
	int get(void) const
	{
		return k;
	}
	void set(int _k)
	{
		k = _k;
	}
	
	bool isuninit(void) const { return k == -4;}
	bool isunknow(void) const { return k == -3;}
	bool isflag(void)   const { return k == -2;}
	bool ismine(void)   const { return k == -1;}
	bool isempty(void)  const { return k == 0; }
	
	bool isnumber(bool include0 = 1) const
	{
		if(include0) return 0 <= k && k <= 8;
		else return 1 <= k && k <= 8;
	}
	bool operator == (Cell_Type oth) const
	{
		return k == oth.k;
	}
	bool operator != (Cell_Type oth) const
	{
		return k != oth.k;
	}
};
const Cell_Type uninit_cell(-4), unknow_cell(-3), flag_cell(-2), mine_cell(-1), empty_cell(0);

class Cell
{
private:
	int x,y;
	Cell_Type type;
	
public:
	Cell(void): x(-1), y(-1), type() {}
	Cell(int _x,int _y): x(_x), y(_y), type() {}
	Cell(int _x,int _y,Cell_Type _type): x(_x), y(_y), type(_type) {}
	Cell(int _x,int _y,int _type): x(_x), y(_y), type(_type) {}
	
	int getx(void) const { return x;}
	int gety(void) const { return y;}
	std :: pair<int,int> getxy(void) const { return {x,y};}
	Cell_Type gettype(void) const { return type;}
	
	void setx(int _x){ x = _x;}
	void sety(int _y){ y = _y;}
	void setxy(std :: pair<int,int> xy){ x = xy.first; y = xy.second;}
	void settype(Cell_Type _type){ type = _type;}
	void settype(int _type){ type = Cell_Type(_type);}
};

class Board
{
private:
	int n, m;
	std :: vector< std :: vector< Cell_Type > > a;

public:
	Board(void): n(0), m(0), a() {}
	Board(const decltype(a) &_a)
	{
		a = _a;
		n = (int)a.size(); m = a.size()? (int)a[0].size(): 0;
	}
	Board(int _n,int _m,Cell_Type k = uninit_cell)
	{
		n = _n; m = _m;
		a.resize(n);
		for(int i=0; i<n; ++i)
			a[i].resize(m, k);
	}
	
	bool inx(int x) const
	{
		return 0 <= x && x < n;
	}
	bool iny(int y) const
	{
		return 0 <= y && y < m;
	}
	bool in(Cell c) const
	{
		return inx(c.getx()) && iny(c.gety());
	}
	
	int getn(void) const
	{
		return n;
	}
	int getm(void) const
	{
		return m;
	}
	
	Cell_Type get(Cell c) const
	{
		assert(in(c));
		return a[c.getx()][c.gety()];
	}
	void set(Cell c, Cell_Type k)
	{
		assert(in(c));
		a[c.getx()][c.gety()] = k;
	}
	void set(Cell c)
	{
		set(c, c.gettype());
	}
	
	std :: vector<Cell> getallcell(void) const
	{
		std :: vector<Cell> res;
		for(int i=0; i<n; ++i)
			for(int j=0; j<m; ++j)
				res.emplace_back(i, j, a[i][j]);
		return res;
	}
	std :: vector<Cell> getnei(Cell c) const
	{
		int x = c.getx(), y = c.gety();
		std :: vector<Cell> res;
		for(int i=-1; i<=1; ++i) if(inx(x+i))
			for(int j=-1; j<=1; ++j) if(iny(y+j))
			{
				if(i == 0 && j == 0) continue;
				res.emplace_back(x+i, y+j, a[x+i][y+j]);
			}
		return res;
	}
	
	int countnei(Cell c,Cell_Type type) const
	{
		int res = 0;
		for(auto t: getnei(c))
			if(t.gettype() == type)
				++res;
		return res;
	}
	
	bool isvalid(void) const
	{
		for(int i=0; i<n; ++i)
			for(int j=0; j<m; ++j)
			{
				if(!a[i][j].isnumber()) continue;
				if(countnei(Cell(i,j), flag_cell) > a[i][j].get())
					return 0;
			}
		return 1;
	}
	int countflag(void) const
	{
		int res = 0;
		for(int i=0; i<n; ++i)
			for(int j=0; j<m; ++j)
				if(a[i][j].isflag())
					++res;
		return res;
	}
	
	bool fit(const Board &oth) const
	{
		if(n != oth.getn()) return 0;
		if(m != oth.getm()) return 0;
		
		for(int i=0; i<n; ++i)
			for(int j=0; j<m; ++j)
			{
				auto x = a[i][j], y = oth.get(Cell(i,j));
				if(x.isunknow() || y.isunknow()) continue;
				if(x != y) return 0;
			}
		return 1;
	}
	void upd(const Board &oth)
	{
		assert(fit(oth));
		
		for(int i=0; i<n; ++i)
			for(int j=0; j<m; ++j)
				if(a[i][j].isunknow())
					a[i][j] = oth.get(Cell(i,j));
	}
};

class Game
{
public:
	virtual int getn(void) const = 0;
	virtual int getm(void) const = 0;
	virtual bool in(Cell c) const = 0;
	virtual Cell getcell(Cell c) const = 0;
	virtual Board getshown(void) const = 0;
	virtual Board getreal(void) const = 0;
	virtual int getstatus(void) const = 0;
	virtual int click(Cell c) = 0;
	virtual bool setflag(Cell c) = 0;
};

#endif
