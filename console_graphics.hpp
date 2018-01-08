#ifndef CONSOLE_GRAPHICS_H
#define CONSOLE_GRAPHICS_H

#include <iosfwd>

class ScreenLayer;
class Sprite;

void set_framerate(int n);
void set_screen_dimensions(int width, int height);
void set_background(int val);
void render(void);
void draw_text(const char* str, int x, int y, int width, int height, int frame = 0);
void draw_str(const char* str, int x, int y, int width = 100, int height = 100);
void draw_area(int val, int x, int y, int width, int height);
void draw_sqr(int val, int x, int y, int width, int height);
void draw_line(int val, int x, int y, int x2, int y2);
void draw_image(Sprite* s, int x, int y, int frame = 0);
void draw_image_clip(Sprite* s, int x, int y, int x_from_sprite, int y_from_sprite, int width, int height, int frame = 0);
void draw_int_left_end(int num, int x, int y);
void draw_int_right_end(int num, int x, int y);
void draw_layer(ScreenLayer* s, int x, int y);

int current_frame();
const char* last_render();

class ScreenLayer{
public:
	ScreenLayer(int width, int height);
	~ScreenLayer();
	
	void draw_text(const char* str, int x, int y, int width, int height, int frame = 0);
	void draw_str(const char* str, int x, int y, int width = 100, int height = 100);
	void draw_area(int val, int x, int y, int width, int height);
	void draw_sqr(int val, int x, int y, int width, int height);
	void draw_line(int val, int x, int y, int x2, int y2);
	void draw_image_clip(Sprite* s, int x, int y, int x_from_sprite, int y_from_sprite, int width, int height, int frame = 0);
	void draw_image(Sprite* s, int x, int y, int frame = 0);
	void draw_int_left_end(int num, int x, int y);
	void draw_int_right_end(int num, int x, int y);
	void clear();
	
	int get_width();
	int get_height();
	const char* get_screen();
private:
	int screen_width_;
	int screen_height_;
	char* screen_;
};

class Sprite{
public:
	Sprite();
	Sprite(int id, int width, int height, const char* sprite = 0, bool is_animation = false,
	bool is_loop = false, int frame_duration = 1, int animation_size = 1);
	~Sprite();
	
	int get_id();
	int get_width();
	int get_height();
	const char* get_sprite(int frame = 0);
	bool is_animation();
	bool is_loop();
	int get_frame_duration();
	int get_animation_size();
	
	std::ifstream& read_from_file(std::ifstream& in);
	void read_from_file(const char* filename);
	std::ofstream& write_to_file(std::ofstream& out);
private:
	int id_;
	int width_;
	int height_;
	bool is_animation_;
	bool is_loop_;
	int frame_duration_;
	int animation_size_;
	unsigned char* sprite_;
};

#endif