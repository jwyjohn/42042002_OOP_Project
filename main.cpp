#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <Windows.h>
#include "cmd_console_tools.h"

#include"Game.h"
using namespace std;

/***************************************************************************
  功    能：主函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main()
{
	cct_setcursor(CURSOR_INVISIBLE);
	Game g;
	//Beep(750, 300);
	g.session();

	return 0;
}
