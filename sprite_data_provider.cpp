#include "console_graphics.hpp"
#include "error_report.hpp"
#include "sprite_data_provider.hpp"

#include <fstream>

SpriteDataProvider *SpriteDataProvider::singleton = 0;

SpriteDataProvider *SpriteDataProvider::get_instance(){
	if(!singleton){
		singleton = new SpriteDataProvider;
	}
	return singleton;
}

Sprite *SpriteDataProvider::get_sprite_by_name(std::string filename){
	Sprite *sprite = sprites[filename];
	
	if(!sprite){
		std::ifstream in;
		in.open(filename.c_str(), std::ios::binary);
		
		if(in.fail()){
			std::string error = "SpriteDataProvider failed to open sprite file with filename " + filename;
			log_error(error.c_str());
			return 0;
		}
		
		sprite = new Sprite;
		sprite->read_from_file(in);
		in.close();
		
		sprites[filename] = sprite;
	}
	
	return sprite;
}

void SpriteDataProvider::free_data(){
}