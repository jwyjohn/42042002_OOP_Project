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
  ��    �ܣ�������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main()
{
	cct_setcursor(CURSOR_INVISIBLE);
	Game g;
	//Beep(750, 300);
	g.session();

	return 0;
}
