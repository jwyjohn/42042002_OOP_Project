#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <Windows.h>
#include "cmd_console_tools.h"
#include "tui.h"
#include "Game.h"

using namespace std;

/***************************************************************************
  功    能：获取单例
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
tui& tui::get_instance()
{
	//clog << "[debug] tui.get_instance() called. \n";
	static tui instance;
	return instance;
}

/***************************************************************************
  功    能：增量渲染 TUI 界面
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::render(int mode)
{
	//clog << "[debug] tui.render() called. \n";
	cct_setcursor(CURSOR_INVISIBLE);
	for (int i = 0; i < TUI_H; i++)
	{
		for (int j = 0; j < TUI_W; j++)
		{
			if (frame[i][j] == frame_buf[i][j])
			{
				continue;
			}
			else
			{
				frame[i][j] = frame_buf[i][j];
				cct_showch(j, i, frame[i][j].ch, 0, frame[i][j].fg);
			}
		}
	}
	this->reset_buf(mode);
	//Sleep(100);
	//cct_setcursor(CURSOR_VISIBLE_NORMAL);
}

/***************************************************************************
  功    能：向界面中加入宝石
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::add_gem_to_buf(int x, int y, int c)
{
	int bx = 10 * x + 1, by = 5 * y + 1;
	const int dx[] = { 4,2,4,6,4,-1 };
	const int dy[] = { 1,2,2,2,3,-1 };
	int i = 0;
	while (dx[i] > 0)
	{
		frame_buf[by + dy[i]][bx + dx[i]] = { (char)('1' + c),c + 1 };
		i++;
	}
}

/***************************************************************************
  功    能：向界面中加入位置提示
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::add_shadow_to_buf(int x, int y)
{
	int bx = 10 * x + 1, by = 5 * y + 1;
	frame_buf[by + 0][bx + 0] = { (char)('0' + y) ,7 };
	frame_buf[by + 0][bx + 1] = { (char)('0' + x) ,7 };
}

/***************************************************************************
  功    能：向界面中加入一个方框
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::add_rect_to_buf(int x, int y, int w, int h, bool clear)
{
	const int kx[] = { 0,1,0,1 };
	const int ky[] = { 0,0,1,1 };

	if (clear)
	{
		for (int i = 0; i < h + 1; i++)
		{
			for (int j = 0; j < w + 1; i++)
			{
				frame_buf[i + y][j + x] = { ' ', 7 };
			}
		}
	}
	for (int i = 0; i < h + 1; i++)
	{
		frame_buf[i + y][x] = { '|', 7 };
		frame_buf[i + y][x + w + 1] = { '|', 7 };
	}
	for (int i = 0; i < w + 1; i++)
	{
		frame_buf[y][i + x] = { '-', 7 };
		frame_buf[y + h + 1][i + x] = { '-', 7 };
	}
	for (int k = 0; k < 4; k++)
	{
		frame_buf[y + ky[k] * (h + 1)][x + kx[k] * (w + 1)] = { '+', 7 };
	}
}

/***************************************************************************
  功    能：向界面中加入带标题方框
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::add_rect_to_buf(int x, int y, int w, int h, const char* s, bool clear)
{
	this->add_rect_to_buf(x, y, w, h);
	for (int i = 0; i < min((int)strlen(s), w - 1); i++)
	{
		frame_buf[y][i + x] = { s[i], 7 };
	}

}

/***************************************************************************
  功    能：向界面中加入焦点
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::add_focus_to_buf(int x, int y)
{
	const int kx[] = { 0,1,0,1 };
	const int ky[] = { 0,0,1,1 };
	x = x * 10 + 2, y = y * 5 + 1;
	int h = 3, w = 5;
	for (int k = 0; k < 4; k++)
	{
		frame_buf[y + ky[k] * (h + 1)][x + kx[k] * (w + 1)] = { '.', 7 };
	}
}

/***************************************************************************
  功    能：向界面中加入高亮焦点
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::add_hilite_to_buf(int x, int y)
{
	const int kx[] = { 0,1,0,1 };
	const int ky[] = { 0,0,1,1 };
	x = x * 10 + 2, y = y * 5 + 1;
	int h = 3, w = 5;
	for (int k = 0; k < 4; k++)
	{
		frame_buf[y + ky[k] * (h + 1)][x + kx[k] * (w + 1)] = { '+', COLOR_HWHITE };
	}
}

/***************************************************************************
  功    能：向界面中加入文字
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::add_text_to_buf(int x, int y, const char* s)
{
	for (int i = 0; i < strlen(s); i++)
	{
		frame_buf[y][i + x] = { s[i], 7 };
	}
}

/***************************************************************************
  功    能：重置界面
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::reset_buf(int mode)
{
	for (int i = 0; i < TUI_H; i++)
	{
		for (int j = 0; j < TUI_W; j++)
		{
			frame_buf[i][j] = { ' ', 7 };
		}
	}
	if (mode == 1)
	{
		this->add_rect_to_buf(TUI_BOARD_X, TUI_BOARD_Y, TUI_BOARD_W, TUI_BOARD_H);
		this->add_rect_to_buf(TUI_BAR_X, TUI_BAR_Y, TUI_BAR_W, TUI_BAR_H, "Score");
		this->add_rect_to_buf(TUI_BAR_X, TUI_BAR_Y + TUI_BAR_H + 4, TUI_BAR_W, TUI_BAR_H, "Next");
		this->add_text_to_buf(TUI_BAR_X, TUI_BAR_H * 44, "[ESC] Back");
	}
	if (mode == 0)
	{
		const char str1[] = "* Press SPACE to start... *";
		this->add_text_to_buf((TUI_W - (int)strlen(str1)) / 2, TUI_H / 3 * 2, str1);

		const char str2[] = "* Press ESC to exit... *";
		this->add_text_to_buf((TUI_W - (int)strlen(str2)) / 2, TUI_H / 3 * 2 + 2, str2);
	}
}

/***************************************************************************
  功    能：将主界面加入缓冲区
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void tui::main_menu()
{
	for (int i = 0; i < TUI_H; i++)
	{
		for (int j = 0; j < TUI_W; j++)
		{
			frame_buf[i][j] = { ' ', 7 };
		}
	}
	cct_setcolor();
	cct_gotoxy(TUI_BAR_X + 1, TUI_BAR_Y + TUI_BAR_H + 5);
	for (int i = 0; i < STEP_GEMS; i++)
	{
		cout << "  ";
	}
	this->add_text_to_buf(26 ,10+1, " _   _ _       _   _            ____                      ");
	this->add_text_to_buf(26, 10+2, "| \\ | (_)_ __ | |_| |__        / ___| __ _ _ __ ___   ___ ");
	this->add_text_to_buf(26, 10+3, "|  \\| | | '_ \\| __| '_ \\ _____| |  _ / _` | '_ ` _ \\ / _ \\");
	this->add_text_to_buf(26, 10+4, "| |\\  | | | | | |_| | | |_____| |_| | (_| | | | | | |  __/");
	this->add_text_to_buf(26, 10+5, "|_| \\_|_|_| |_|\\__|_| |_|      \\____|\\__,_|_| |_| |_|\\___|");
	const char str1[] = "* Press SPACE to start... *";
	this->add_text_to_buf((TUI_W - (int)strlen(str1)) / 2, TUI_H / 3 * 2, str1);

	const char str2[] = "* Press ESC to exit... *";
	this->add_text_to_buf((TUI_W - (int)strlen(str2)) / 2, TUI_H / 3 * 2 + 2, str2);
}


/***************************************************************************
  功    能：TUI界面的构造函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
tui::tui()
{
	//clog << "[debug] tui.tui() called. \n";
	cct_setconsoleborder(TUI_W, TUI_H);
	cct_enable_mouse();
	cct_setfontsize("fixedsys", 16);
	for (int i = 0; i < TUI_H; i++)
	{
		for (int j = 0; j < TUI_W; j++)
		{
			frame_buf[i][j] = { ' ', 7 };
			frame[i][j] = { ' ', 7 };
		}
	}
	this->render();
}
