#ifndef SPRITE_DATA_PROVIDER_HPP
#define SPRITE_DATA_PROVIDER_HPP

#include <map>
#include <string>

class Sprite;

class SpriteDataProvider{
public:
	static SpriteDataProvider *get_instance();
	
	/*	allocates an instance of the sprite on the first call,
		instance is stored in std::map<std::string, Sprite*> sprites
		next call with same filename will return the same instance stored.
	*/
	Sprite *get_sprite_by_name(std::string filename);
	
	/*	frees data allocated into sprites
	*/
	void free_data();
private:
	static SpriteDataProvider *singleton;
	std::map<std::string, Sprite*> sprites;
};

#endif