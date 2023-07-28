#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include"game.h"

namespace Predictor_Algo
{
	using std::vector;
	using std::map;
	using std::set;
	using std::pair;
	using std::max;
	using std::min;
	using pii = pair<int,int>;
	
	int n,m;
	
	int encode(Cell c){ return c.getx() * m + c.gety();}
	Cell decode(int u){ return Cell(u / m, u % m);}
	
	vector<int> calc(const vector<int> &A,const vector<int> &B)
	{
		if(A.size() <= B.size()) return {};
		int j = 0;
		vector<int> res;
		for(int i=0; i<(int)A.size(); ++i)
		{
			if(j<(int)B.size() && A[i] == B[j]) ++j;
			else res.emplace_back(A[i]);
		}
		return j<(int)B.size()? vector<int>(): res;
	}
	
	map< vector<int>, pii > f;
	map< int, vector< vector<int> > > g;
	set< vector<int> > q;
	
	bool hasans = 0;
	Cell ans;
	
	vector<Cell> solve(const Board &have)
	{
		n = have.getn();
		m = have.getm();
		f.clear();
		g.clear();
		q.clear();
		hasans = 0;
		ans = Cell(0,0);
		
		auto upd = [&] (vector<int> u,pii k)
		{
			if(hasans) return;
			if(!u.size()) return;
			
			k.first = max(k.first, 0);
			k.second = min(k.second, (int)u.size());
			
			if(k.first == (int)u.size())
			{
				hasans = 1;
				ans = decode(u[0]);
				ans.settype(mine_cell);
				return;
			}
			if(k.second == 0)
			{
				hasans = 1;
				ans = decode(u[0]);
				ans.settype(empty_cell);
				return;
			}
			
			pii has = {0, (int)u.size()};
			auto it = f.find(u);
			if(it != f.end()) has = it -> second;
			
			k.first = max(k.first, has.first);
			k.second = min(k.second, has.second);
			if(has == k) return;
			
			if(it == f.end())
			{
				for(int t: u)
					g[t].emplace_back(u);
			}
			
			f[u] = k;
			q.emplace(u);
		};
		
		for(auto c: have.getallcell())
			if(c.gettype().isnumber())
			{
				vector<int> u;
				for(auto t: have.getnei(c, unknow_cell))
					u.emplace_back(encode(t));
				
				int rem = c.gettype().get() - have.countnei(c, flag_cell);
				upd(u, {rem, rem});
			}
		
		while(q.size())
		{
			if(hasans) break;
			
			auto u = *q.begin(); q.erase(q.begin());
			pii cur = f[u];
			
			for(int mask=1; mask<(1<<((int)u.size())); ++mask)
			{
				vector<int> v;
				for(int i=0; i<(int)u.size(); ++i)
					if((mask >> i) & 1)
						v.emplace_back(u[i]);
				upd(v, {0, cur.second});
			}
			
			for(int i: u)
				for(auto v: g[i])
				{
					auto w = calc(v, u);
					if(!w.size()) continue;
					
					pii nxt = f[v];
					nxt.first -= cur.second;
					nxt.second -= cur.first;
					upd(w, nxt);
				}
		}
		
		if(hasans) return {ans};
		else return {};
	}
}

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
		if(!have.in(c)) return 0;
		if(have.get(c) != unknow_cell && !have.get(c).isnumber()) return 0;
		game.click(c);
		have.upd(game.getshown());
		return 1;
	}
	bool setflag(Cell c, bool ingame = 1)
	{
		if(!have.in(c)) return 0;
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
				for(Cell t: have.getnei(c, unknow_cell))
				{
					setflag(t);
					return 1;
				}
			}
			if(x == k)
			{
//				click(c);
				for(Cell t: have.getnei(c, unknow_cell))
					if(t.gettype() == unknow_cell)
					{
						click(t);
						return 1;
					}
			}
		}
		
		auto res = Predictor_Algo :: solve(have);
		if(res.size())
		{
			auto u = res[0];
			if(u.gettype() == mine_cell)
				setflag(u);
			else
				click(u);
			return 1;
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
