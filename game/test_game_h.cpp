#include<bits/stdc++.h>
#include"localgame.h"
#include"gamedisplay.h"
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

int main(void)
{
	int n = 10, m = 10, d = 5;
	
	Local_Game game(n, m, d);
	Game_Display game_display(game);
	
	while(game.getstatus() == 0)
	{
		system("cls");
		cout << game_display.show();
		cout << ": ";
		
		string op;
		getline(cin, op);
		stringstream ss(op);
		
		char c;
		if(!(ss >> c)) continue;
		
		if(c == 'c')
		{
			int x,y;
			if(ss >> x >> y)
			{
				game_display.click(Cell(x,y));
			}
			else
			{
				game_display.click();
			}
		}
		else if(c == 'm')
		{
			int x,y;
			if(ss >> x >> y)
			{
				game_display.move(x, y);
			}
		}
		else if(c == 'u')
		{
			game_display.move(-1, 0);
		}
		else if(c == 'd')
		{
			game_display.move(1, 0);
		}
		else if(c == 'l')
		{
			game_display.move(0, -1);
		}
		else if(c == 'r')
		{
			game_display.move(0, 1);
		}
		else if(c == 'q')
		{
			system("pause");
			exit(0);
		}
	}
	system("cls");
	cout << game_display.show();
	return 0;
}
