#pragma once

/***************************************************************************
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
#define BOARD_SIZE 9
#define COLORS 7

class Gem
{
private:
	int x;
	int y;
	int v;
	int score;
	friend class Game;
public:
	Gem();
	Gem(int x, int y, int v);
	bool operator==(const Gem& b) const;
};

