#pragma once
#include<vector>
#include"Gem.h"

#define START_GEMS 3
#define STEP_GEMS 3
#define ELIM_GEMS 5
#define ELIM_SCROE 2

class Game
{
private:
	int seed;
	int score;
	int steps;
	std::vector<Gem> arr;
	std::vector<int> color_buf;
	int focus_x, focus_y;
	int selected_x, selected_y;
	int get_gem(int x, int y) const;
	void path_finder(int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>>& res);
	int move_gem(int x1, int y1, int x2, int y2);
	int elim_gem(int x, int y);
	void mark_elim(std::vector<std::pair<int, int>>& res);
	void init();
public:
	Game();
	Game(int seed);
	void render() const;
	bool is_end() const;
	//int get_score() const;
	int operation(int x1, int y1, int x2, int y2);
	void session();
};

