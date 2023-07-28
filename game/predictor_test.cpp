#include<bits/stdc++.h>
#include<windows.h>
#include"localgame.h"
#include"gamedisplay.h"
#include"predictor.h"
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

int main(void)
{
	int T;
	cin >> T;
	
	int n, m, d;
	cin >> n >> m >> d;
	
	int cnt_win = 0, cnt_lose = 0;
	int lstmove = -1;
	int lstround = -1; 
	int combo = 0;
	int mxcombo = 0;
	
	const int preW = 15;
	const int midW = 3;
	
	auto P = [&] (string s)
	{
		cout << setw(preW) << s << ":" << string(midW, ' ');
	};
	auto print_info = [&] (void)
	{
		system("cls");
		
		P("Size"); cout << n << " * " << m << endl;
		P("Mine"); cout << d << endl;
		P("Total"); cout << cnt_win + cnt_lose << endl;
		P("Win"); cout << cnt_win << endl;
		P("Lose"); cout << cnt_lose << endl;
		P("Rate");
		if(cnt_win + cnt_lose == 0)
			cout << "N/A" << endl;
		else
			cout << (double)cnt_win / (cnt_win + cnt_lose) * 100 << "%" <<endl;
		
		P("Combo");
		cout << combo << endl;
		
		P("Max Combo");
		cout << mxcombo << endl;
		
		P("Last Round");
		if(lstround == -1)
			cout << "N/A" << endl;
		else if(lstround == 0)
			cout << "Lose" << endl;
		else
			cout << "Win" << endl;
		
		P("Last Move");
		if(lstmove == -1)
			cout << "N/A" << endl;
		else if(lstmove == 0)
			cout << "Random" << endl;
		else
			cout << "Regular" << endl; 
	};
	
	while(T--)
	{
		Local_Game game(n, m, d);
		Game_Display game_display(game);
		Predictor predictor(game_display);
		
		auto print_game = [&] (void)
		{
			print_info();
			
			P("Remaining");
			cout << game_display.getremain() << endl;
			cout << game_display.show();
		};
		
		lstmove = -1;
		
		while(game.getstatus() == 0)
		{
			print_game();
			Sleep(50);
			
			predictor.upd();
			
			bool flag = predictor.move();
			if(flag == 0)
			{
				lstmove = 0;
				predictor.randmove();
			}
			else
			{
				lstmove = 1;
			}
		}
		
		if(game.getstatus() == 1)
		{
			lstround = 1;
			++cnt_win;
			++combo;
			mxcombo = max(mxcombo, combo);
		}
		else
		{
			lstround = 0;
			++cnt_lose;
			combo = 0;
		}
		
		lstmove = -1;
	}
	
	print_info();
	system("pause");
	return 0;
}
