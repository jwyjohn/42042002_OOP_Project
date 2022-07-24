#pragma once
#define TUI_W 112
#define TUI_H 47
#define TUI_BOARD_X 0
#define TUI_BOARD_Y 0
#define TUI_BOARD_W 90
#define TUI_BOARD_H 45
#define TUI_BAR_X 93
#define TUI_BAR_Y 2
#define TUI_BAR_W 16
#define TUI_BAR_H 1

/***************************************************************************
  说    明：渲染单个字符的像素点
***************************************************************************/
struct pixel
{
	char ch;
	int fg;
	bool operator==(const pixel& b)
	{
		return (ch==b.ch) && (fg == b.fg);
	}
};

/***************************************************************************
  说    明：单例模式的TUI
***************************************************************************/
class tui
{
public:
	pixel frame_buf[TUI_H][TUI_W];
	~tui() {};
	tui& operator=(const tui&) = delete;
	static tui& get_instance();
	void render(int mode = 1);
	void add_gem_to_buf(int x, int y, int c);
	void add_shadow_to_buf(int x, int y);
	void add_rect_to_buf(int x, int y, int w, int h, bool clear = false);
	void add_rect_to_buf(int x, int y, int w, int h, const char* s, bool clear = false);
	void add_focus_to_buf(int x, int y);
	void add_hilite_to_buf(int x, int y);
	void add_text_to_buf(int x, int y, const char* s);
	void reset_buf(int mode = 1);
	void main_menu();
private:
	tui(const tui&) = delete;
	tui();
	pixel frame[TUI_H][TUI_W];
};