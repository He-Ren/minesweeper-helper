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
	int n = 0, m = 0, d = 0;
	bool needinput = 1;
	while(1)
	{
		system("cls");
		
		if(needinput)
			cin >> n >> m >> d;
		
		if(n < 1 || m < 1 || d < 0 || d >= n*m)
			continue;
		
		Local_Game game(n, m, d);
		Game_Display game_display(game);
		Predictor predictor(game_display);
		
		while(game.getstatus() == 0)
		{
			system("cls");
			cout << "Remaining: " << game_display.getremain() << endl;
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
			else if(c == 'f')
			{
				game_display.setflag();
			}
			else if(c == 'g')
			{
				int x,y;
				if(ss >> x >> y)
				{
					game_display.move(x, y);
				}
			}
			else if(c == 'w')
			{
				game_display.move(-1, 0);
			}
			else if(c == 's')
			{
				game_display.move(1, 0);
			}
			else if(c == 'a')
			{
				game_display.move(0, -1);
			}
			else if(c == 'd')
			{
				game_display.move(0, 1);
			}
			else if(c == 'q')
			{
				cout << "Sure? (no/yes)" << endl;
				
				getline(cin, op);
				if(op == "yes")
				{
					system("pause");
					exit(0);
				}
			}
			else if(c == 'b')
			{
				predictor.upd();
				
				bool flag = predictor.move();
				if(flag == 0)
				{
					printf("Auto move failed.\n");
					getline(cin, op);
				}
			}
			else if(c == 'n')
			{
				predictor.upd();
				
				bool flag = predictor.randmove();
				if(flag == 0)
				{
					printf("Random move failed.\n");
					getline(cin, op);
				}
			}
			else if(c == 'm')
			{
				predictor.upd();
				
				int cnt = 0;
				while(1)
				{
					bool flag = predictor.move();
					if(flag == 0) break;
					++cnt;
					
					system("cls");
					cout << "Remaining: " << game_display.getremain() << endl;
					cout << game_display.show();
					printf("cnt = %d.\n",cnt);
					
					Sleep(300);
				}
				
				system("cls");
				cout << "Remaining: " << game_display.getremain() << endl;
				cout << game_display.show();
				printf("cnt = %d, finished.\n",cnt);
				
				getline(cin, op);
			}
		}
		
		system("cls");
		cout << "Remaining: " << game_display.getremain() << endl;
		cout << game_display.show();
		
		if(game.getstatus() == 1)
			printf("Succeed.\n");
		else
			printf("Failed.\n");
		
		
		printf("Continue? (yes/no/again)\n");
		
		while(1)
		{
			printf(": ");
			
			string op;
			getline(cin, op);
			stringstream ss(op);
			
			ss >> op;
			if(op == "yes")
			{
				needinput = 1;
				break;
			}
			if(op == "no")
			{
				system("pause");
				return 0;
			}
			if(op == "again")
			{
				needinput = 0;
				break;
			}
		}
	}
	system("pause");
	return 0;
}
