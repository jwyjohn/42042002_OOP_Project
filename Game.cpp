#define _CRT_SECURE_NO_WARNINGS
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<iomanip>
#include<queue>
#include <conio.h>
#include <Windows.h>
#include "cmd_console_tools.h"

#include "Game.h"
#include"tui.h"


using namespace std;

/***************************************************************************
  功    能：查找地图中对应位置宝石在arr中的位置
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int Game::get_gem(int x, int y) const
{
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr[i].x == x && arr[i].y == y)
			return i;
	}
	return -1;
}

/***************************************************************************
  功    能：移动宝石位置并渲染
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int Game::move_gem(int x1, int y1, int x2, int y2)
{
	int ret1 = this->get_gem(x1, y1);
	int ret2 = this->get_gem(x2, y2);
	if (ret1 < 0)
	{
		//clog << "[Failed] source (" << x1 << ", " << y1 << ") has no gem." << endl;
		Beep(400, 300);
		return -1;
	}
	if (ret2 >= 0)
	{
		//clog << "[Failed] target (" << x2 << ", " << y2 << ") has gem." << endl;
		Beep(400, 300);
		return -2;
	}
	vector<pair<int, int>> path;
	path_finder(x1, y1, x2, y2, path);
	if (path.size() == 0)
	{
		//clog << "[Failed] no path " << "(" << x1 << ", " << y1 << ")" << " -> " << "(" << x2 << ", " << y2 << ")" << " available." << endl;
		Beep(400, 300);
		return -3;
	}
	Beep(750, 300);
	for (auto& i : path)
	{
		arr[ret1].x = i.first;
		arr[ret1].y = i.second;
		this->render();
	}
	return 0;
}

/***************************************************************************
  功    能：移除一个宝石并渲染
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int Game::elim_gem(int x, int y)
{
	int i = this->get_gem(x, y);
	if (i >= 0)
	{
		arr.erase(arr.begin() + i);
		Beep(950, 300);
		this->render();
		return ELIM_SCROE;
	}
	return 0;

}

/***************************************************************************
  功    能：初始化游戏
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Game::init()
{
	score = 0;
	steps = 0;
	srand(seed);
	focus_x = 0, focus_y = 0;
	selected_x = 0, selected_y = 0;
	arr.clear();
	while (arr.size() < START_GEMS)
	{
		Gem new_gem;
		//clog << new_gem.x << new_gem.y << endl;
		if (this->get_gem(new_gem.x, new_gem.y) < 0)
			arr.push_back(new_gem);
	}
	color_buf.clear();
	for (int i = 0; i < STEP_GEMS; i++)
	{
		color_buf.push_back(rand() % COLORS);
	}
}

/***************************************************************************
  功    能：判断坐标是否合法
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool inmap(int x, int y)
{
	return 0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE;
}

/***************************************************************************
  功    能：用于 BFS 的节点
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
struct node
{
	int x;
	int y;
	int id;
	int parent = 0;
	node(int a, int b, int c)
	{
		y = a;
		x = b;
		id = c;
	}
};

/***************************************************************************
  功    能：寻找地图上的最短路
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Game::path_finder(int x1, int y1, int x2, int y2, vector<pair<int, int>>& res)
{
	const int dx[] = { 1,0,-1,0 };
	const int dy[] = { 0,1,0,-1 };
	bool vis[BOARD_SIZE][BOARD_SIZE];
	int mp[BOARD_SIZE][BOARD_SIZE];
	int path[BOARD_SIZE * BOARD_SIZE + 10] = { 0 };
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			vis[i][j] = false;
			mp[i][j] = i * BOARD_SIZE + j + 1;
			int ret = this->get_gem(i, j);
			if (ret >= 0)
			{
				mp[i][j] = 0;
			}
			//clog << setw(3) << mp[i][j];
		}
		//clog << endl;
	}
	queue<node> q;
	node v = node(x1, y1, x1 * BOARD_SIZE + y1 + 1);
	vis[x1][y1] = true;
	q.push(v);
	while (!q.empty())
	{
		node u = q.front();
		q.pop();
		path[u.id] = u.parent;
		for (int i = 0; i < 4; i++)
		{
			int tx = u.x + dx[i];
			int ty = u.y + dy[i];
			if (inmap(tx, ty) && mp[ty][tx] && !vis[ty][tx])
			{
				vis[ty][tx] = true;
				node v = node(ty, tx, mp[ty][tx]);
				v.parent = u.id;
				q.push(v);
			}
		}
	}
	int target = x2 * BOARD_SIZE + y2 + 1;
	if (path[target] == 0)
		return;
	while (target != 0)
	{
		res.push_back({ (target - 1) / BOARD_SIZE,(target - 1) % BOARD_SIZE });
		target = path[target];
	}
	reverse(res.begin(), res.end());
}

/***************************************************************************
  功    能：构造函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Game::Game()
{
	seed = (int)time(0);
	this->init();
}

/***************************************************************************
  功    能：传入种子的构造函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Game::Game(int mseed) : seed(mseed)
{
	this->init();
}

/***************************************************************************
  功    能：渲染游戏地图
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Game::render() const
{
	//clog << "[debug] game.render() called. \n";
	tui& tui0 = tui::get_instance();
	tui0.reset_buf();
	//for (int i = 0; i < BOARD_SIZE; i++)
	//{
	//	for (int j = 0; j < BOARD_SIZE; j++)
	//	{
	//		tui0.add_shadow_to_buf(i, j);
	//	}
	//}
	for (auto& i : arr)
	{
		tui0.add_gem_to_buf(i.x, i.y, i.v);
	}
	char buf[TUI_BAR_W - 1] = { 0 };
	sprintf(buf, "%d", min(score, 99999));
	for (int i = 0; i < TUI_BAR_W - 1; i++)
	{
		if (buf[i] != 0)
		{
			tui0.frame_buf[TUI_BAR_Y + 1][TUI_BAR_X + i + 1].ch = buf[i];
		}
		else
		{
			tui0.frame_buf[TUI_BAR_Y + 1][TUI_BAR_X + i + 1].ch = ' ';
		}
	}
	tui0.add_focus_to_buf(focus_x, focus_y);
	if (inmap(selected_x, selected_y))
	{
		tui0.add_hilite_to_buf(selected_x, selected_y);
	}
	tui0.render();
	Sleep(50);
}

bool Game::is_end() const
{
	if (arr.size() >= BOARD_SIZE * BOARD_SIZE)
		return true;
	return false;
}

/***************************************************************************
  功    能：进行一次移动
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int Game::operation(int x1, int y1, int x2, int y2)
{
	int ret = this->move_gem(x1, y1, x2, y2);
	int k = 0;
	vector<pair<int, int>> elim;
	this->mark_elim(elim);
	for (auto& i : elim)
	{
		score += this->elim_gem(i.first, i.second);
	}
	if (elim.size() <= 0)
	{
		while (k < STEP_GEMS && arr.size() < BOARD_SIZE * BOARD_SIZE)
		{
			Gem new_gem;
			//clog << new_gem.x << new_gem.y << endl;
			if (this->get_gem(new_gem.x, new_gem.y) < 0)
			{
				new_gem.v = color_buf[k];
				arr.push_back(new_gem);
				k++;
				this->render();
				Sleep(100);
			}
		}
		color_buf.clear();
		for (int i = 0; i < STEP_GEMS; i++)
		{
			color_buf.push_back(rand() % COLORS);
		}
		this->render();
	}
	elim.clear();
	this->mark_elim(elim);
	for (auto& i : elim)
	{
		score += this->elim_gem(i.first, i.second);
	}
	this->render();
	return 0;
}

/***************************************************************************
  功    能：进入游戏播放的音频
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void enter_song()
{
	const int _oC = 277, _oD = 311, _oF = 370, _oG = 415, _oA = 466;
	const int _C = 262, _D = 294, _E = 330, _F = 349, _G = 392, _A = 440, _B = 494;
	const int oC = 554, oD = 622, oF = 740, oG = 831, oA = 932;
	const int C = 523, D = 578, E = 659, F = 698, G = 784, A = 880, B = 988;
	const int C_ = 1047, D_ = 1175, E_ = 1319, F_ = 1397, G_ = 1568, A_ = 1760, B_ = 1976;
	const int oC_ = 1109, oD_ = 1245, oF_ = 1480, oG_ = 1661, oA_ = 1865;

	const int T = 300; //一拍的长度
	const int Stop = 600; //一拍休止符的长度

	Beep(C, T);
	Beep(D, T);
	Beep(E, T);
	Beep(F, T);
	Beep(G, T);
}

/***************************************************************************
  功    能：主循环
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Game::session()
{
	while (true)
	{
		tui& tui0 = tui::get_instance();
		tui0.main_menu();
		tui0.render(0);
		while (true)
		{
			int sx, sy, op, k1, k2;
			cct_read_keyboard_and_mouse(sx, sy, op, k1, k2);
			if (k1 == ' ')
			{
				enter_song();
				Sleep(100);
				break;
			}
			if (k1 == 27)
			{
				return;
			}
		}
		this->init();
		this->render();
		selected_x = -1, selected_y = -1;
		while (true)
		{
			if (this->is_end())
			{
				tui0.add_rect_to_buf(TUI_W / 3, TUI_H / 3 + 6, TUI_W / 3, TUI_H / 3 - 6, "GAME OVER", true);
				char buf[128];
				sprintf(buf, "Your SCORE is: %d", min(score, 99999));
				tui0.add_text_to_buf(TUI_W / 3 + 4, TUI_H / 3 * 2 - 6, buf);
				tui0.add_text_to_buf(TUI_W / 3 + 4, TUI_H / 3 * 2 - 2, "Press SPACE to continue...");
				tui0.render();
				while (true)
				{
					int sx, sy, op, k1, k2;
					cct_read_keyboard_and_mouse(sx, sy, op, k1, k2);
					if (k1 == ' ')
					{
						break;
					}
				}
				break;
			}
			int sx, sy, op, k1, k2;
			cct_read_keyboard_and_mouse(sx, sy, op, k1, k2);
			if (k1 == 27)
			{
				tui0.add_rect_to_buf(TUI_W / 3, TUI_H / 3 + 6, TUI_W / 3, TUI_H / 3 - 6, "GAME OVER", true);
				char buf[128];
				sprintf(buf, "Your SCORE is: %d", min(score, 99999));
				tui0.add_text_to_buf(TUI_W / 3 + 4, TUI_H / 3 * 2 - 6, buf);
				tui0.add_text_to_buf(TUI_W / 3 + 4, TUI_H / 3 * 2 - 2, "Press SPACE to continue...");
				tui0.render();
				while (true)
				{
					int sx, sy, op, k1, k2;
					cct_read_keyboard_and_mouse(sx, sy, op, k1, k2);
					if (k1 == ' ')
					{
						break;
					}
				}
				break;
			}
			cct_gotoxy(TUI_BAR_X + 1, TUI_BAR_Y + TUI_BAR_H + 5);
			for (int i = 0; i < STEP_GEMS; i++)
			{
				cct_setcolor(0, color_buf[i] + 1);
				cout << " " << (char)('1' + color_buf[i]);
			}
			cct_setcolor();
			this->render();

			if (sx / 10 < BOARD_SIZE && sy / 5 < BOARD_SIZE)
			{
				if (focus_x != sx / 10 || focus_y != sy / 5)
				{
					focus_x = sx / 10;
					focus_y = sy / 5;
					this->render();
				}
				if (op == MOUSE_LEFT_BUTTON_CLICK)
				{
					if (inmap(selected_x, selected_y))
					{
						this->operation(selected_x, selected_y, focus_x, focus_y);
						selected_x = -1, selected_y = -1;
					}
					else
					{
						if (get_gem(sx / 10, sy / 5) >= 0)
						{
							selected_x = sx / 10;
							selected_y = sy / 5;
							Beep(750, 300);
						}

					}
				}
				else if (op == MOUSE_RIGHT_BUTTON_CLICK)
				{
					if (inmap(selected_x, selected_y))
					{
						selected_x = -1, selected_y = -1;
						Beep(350, 300);
					}
				}
			}
		}
	}
}

/***************************************************************************
  功    能：找出可以消除的宝石
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Game::mark_elim(std::vector<std::pair<int, int>>& res)
{
	const int dx[] = { 1,0,1,1 };
	const int dy[] = { 0,1,1,-1 };

	res.clear();
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			int ret = this->get_gem(i, j);
			if (ret >= 0)
			{
				bool flag = false;
				for (int k = 0; k < 4; k++)
				{
					int cnt = 1;
					int x = i + dx[k], y = j + dy[k];
					while (this->get_gem(x, y) >= 0 && arr[this->get_gem(x, y)].v == arr[ret].v)
					{
						cnt++;
						x = x + dx[k], y = y + dy[k];
					}
					x = i - dx[k], y = j - dy[k];
					while (this->get_gem(x, y) >= 0 && arr[this->get_gem(x, y)].v == arr[ret].v)
					{
						cnt++;
						x = x - dx[k], y = y - dy[k];
					}
					if (cnt >= ELIM_GEMS)
					{
						flag = true;
						break;
					}
				}
				if (flag)
				{
					res.push_back({ i,j });
				}
			}
		}
	}
}
