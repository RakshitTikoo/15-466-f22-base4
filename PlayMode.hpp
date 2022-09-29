#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>


#include <vector>
#include <deque>
#include <map>


// To draw text
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <hb.h>
#include <hb-ft.h>


struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	

	// Helper Functions
	void get_transforms();
	void init(int val);
	void render_object(Scene::Transform *top_obj, Scene::Transform *bottom_obj);

	void render_level(uint8_t top_option, uint8_t bottom_option);
	void update_time(uint8_t option_selected);

	// Draw function
	void load_char(char *fontfile);
	void draw_msg(std::string text, float x, float y, float scale, glm::uvec2 const &drawable_size);
	void get_color(glm::uvec2 const &drawable_size);

	//----- game state -----
	// Text Features
	struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
	};

    std::map <char, Character> Characters;
	unsigned int VAO, VBO;
	GLuint program = 0;
	GLuint textColor = 0;
	glm::mat4 projection; 
	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
		uint8_t debounce_flg = 0;
	} left, right, space;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	// Transforms
	Scene::Transform *background = nullptr;
	Scene::Transform *baseball = nullptr;
	Scene::Transform *bed = nullptr;
	Scene::Transform *bin = nullptr;
	Scene::Transform *brass_pot = nullptr;
	Scene::Transform *clock = nullptr;
	Scene::Transform *enamel_pot = nullptr;
	Scene::Transform *hammer = nullptr;
	Scene::Transform *photo_camera = nullptr;
	Scene::Transform *Trophy = nullptr;
	Scene::Transform *weight = nullptr;
	Scene::Transform *wine = nullptr;

	float wobble = 0.0f;

	// Game Control
	uint32_t options[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // 0 - bed, 1 - clock , 2 - baseball, 3 - bin, 4 - brass_pot, 5 - enamel_pot, 6 - hammer, 7 - camera, 8 - weight, 9 - wine
	
	int time_left;
	int gym_cost;
	int home_cost;
	int friends_cost;
	int sleep_cost;

	int home_win;
	int friends_win;
	int gym_win;
	int sleep_win;

	int home_win_cnt;
	int friends_win_cnt;
	int gym_win_cnt;
	int sleep_win_cnt;


	float zpos;

	// flags 
	uint8_t init_flg;
	uint8_t top_opt;
	uint8_t bottom_opt;
	uint8_t do_render;

	// Strings
	std::string init_str;
	std::string left_str;
	std::string right_str;
	std::string do_something_else_str;

	//music coming from the tip of the leg (as a demonstration):
	std::shared_ptr< Sound::PlayingSample > MainLoop;
	
	//camera:
	Scene::Camera *camera = nullptr;

};
