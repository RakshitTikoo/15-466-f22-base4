/*
* ===================REFERENCES===================
* Majority of the text rendering logic referenced from - https://learnopengl.com/In-Practice/Text-Rendering
* Majority of Blender Assets taken from - https://polyhaven.com/
* The font is taken from - https://fonts.google.com/
* Sound Assets taken from - https://freepd.com/
*/

#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"


#define WIDTH 1280
#define HEIGHT 720


#include <glm/gtc/type_ptr.hpp>

#include <random>


#include "GL.hpp"

#include <string>

//compiles+links an OpenGL shader program from source.
// throws on compilation error.
GLuint gl_compile_program(
	std::string const &vertex_shader_source,
	std::string const &fragment_shader_source);


GLuint Game4_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > Game4_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("Game4.pnct"));
	Game4_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > Game4_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("Game4.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = Game4_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = Game4_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});



Load< Sound::Sample > Main_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Main.opus"));
});



// Load Transforms
void PlayMode::get_transforms() {
	//get pointers to leg for convenience:
	for (auto &transform : scene.transforms) {
		if (transform.name == "background") background = &transform;
		else if (transform.name == "baseball") baseball = &transform;
		else if (transform.name == "bed") bed = &transform;
		else if (transform.name == "bin") bin = &transform;
		else if (transform.name == "brass_pot") brass_pot = &transform;
		else if (transform.name == "clock") clock = &transform;
		else if (transform.name == "enamel_pot") enamel_pot = &transform;
		else if (transform.name == "hammer") hammer = &transform;
		else if (transform.name == "photo_camera") photo_camera = &transform;
		else if (transform.name == "Trophy") Trophy = &transform;
		else if (transform.name == "weight") weight = &transform;
		else if (transform.name == "wine") wine = &transform;
		
	}
	if (background == nullptr) throw std::runtime_error("background not found.");
	if (baseball == nullptr) throw std::runtime_error("baseball not found.");
	if (bed == nullptr) throw std::runtime_error("bed not found.");
	if (bin == nullptr) throw std::runtime_error("bin not found.");
	if (brass_pot == nullptr) throw std::runtime_error("brass_pot not found.");
	if (clock == nullptr) throw std::runtime_error("clock not found.");
	if (enamel_pot == nullptr) throw std::runtime_error("enamel_pot not found.");
	if (hammer == nullptr) throw std::runtime_error("hammer not found.");
	if (photo_camera == nullptr) throw std::runtime_error("photo_camera not found.");
	if (Trophy == nullptr) throw std::runtime_error("Trophy not found.");
	if (weight == nullptr) throw std::runtime_error("weight not found.");
	if (wine == nullptr) throw std::runtime_error("wine not found.");

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();

	zpos = baseball->position.z;
}

void PlayMode::render_object(Scene::Transform *top_obj, Scene::Transform *bottom_obj) {
	// hide objects 
	baseball->position.x = -300.0f;
	bed->position.x = -300.0f;
	bin->position.x = -300.0f;
	brass_pot ->position.x = -300.0f;
	clock->position.x = -300.0f;
	enamel_pot->position.x = -300.0f;
	hammer  ->position.x = -300.0f;
	photo_camera ->position.x = -300.0f;
	Trophy->position.x = -300.0f;
	weight->position.x = -300.0f;
	wine->position.x = -300.0f;

	baseball->position.z = zpos;
	bed->position.z = zpos;
	bin->position.z = zpos;
	brass_pot ->position.z = zpos;
	clock->position.z = zpos;
	enamel_pot->position.z = zpos;
	hammer  ->position.z = zpos;
	photo_camera ->position.z = zpos;
	Trophy->position.z = zpos;
	weight->position.z = zpos;
	wine->position.z = zpos;
	
	
	if(top_obj != nullptr && bottom_obj != nullptr) {
		top_obj->position.x = camera->transform->position.x;
		bottom_obj->position.x = camera->transform->position.x;

		top_obj->position.z += 2.0f;
		bottom_obj->position.z -= 2.0f;

	}
}

// uint32_t options[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // 0 - bed, 1 - clock , 2 - baseball, 3 - bin, 4 - brass_pot, 5 - enamel_pot, 6 - hammer, 7 - camera, 8 - weight, 9 - wine
void PlayMode::render_level(uint8_t top_option, uint8_t bottom_option) {
	//Scene::Transform *top_obj, *bottom_obj;

	// Top - Left, Bottom - Right
	if (options[top_option] == 0) {render_object(bed, weight);} 
	if (options[top_option] == 1) {render_object(clock, weight);} 
	if (options[top_option] == 2) {render_object(baseball, weight);} 
	if (options[top_option] == 3) {render_object(bin, weight);} 
	if (options[top_option] == 4) {render_object(brass_pot, weight);} 
	if (options[top_option] == 5) {render_object(enamel_pot, weight);} 
	if (options[top_option] == 6) {render_object(hammer, weight);} 
	if (options[top_option] == 7) {render_object(photo_camera, weight);} 
	//if (options[top_option] == 8) {render_object(weight, weight);} 
	if (options[top_option] == 9) {render_object(wine, weight);} 

	if (options[bottom_option] == 0) {render_object(weight, bed);} 
	if (options[bottom_option] == 1) {render_object(weight, clock);} 
	if (options[bottom_option] == 2) {render_object(weight, baseball);} 
	if (options[bottom_option] == 3) {render_object(weight, bin);} 
	if (options[bottom_option] == 4) {render_object(weight, brass_pot);} 
	if (options[bottom_option] == 5) {render_object(weight, enamel_pot);} 
	if (options[bottom_option] == 6) {render_object(weight, hammer);} 
	if (options[bottom_option] == 7) {render_object(weight, photo_camera);} 
	//if (options[bottom_option] == 8) {render_object(weight, weight);} 
	if (options[bottom_option] == 9) {render_object(weight, wine);} 



	if (options[top_option] == 0) left_str = "You are tired. \nGo to Bed"; 
	if (options[top_option] == 1) left_str = "Ah I am tired, \nsnooze the alarm"; 
	if (options[top_option] == 2) left_str = "Need to attend my \nkid's baseball match"; 
	if (options[top_option] == 3) left_str = "Today is garbage day.. \nneed to throw the \ntrash"; 
	if (options[top_option] == 4) left_str = "Need to do \nthe dishes"; 
	if (options[top_option] == 5) left_str = "Its my work to \ncook today"; 
	if (options[top_option] == 6) left_str = "Need to fix the \nleak in the \nroof"; 
	if (options[top_option] == 7) left_str = "The wife is asking\nto go on a trip"; 
	if (options[top_option] == 8) left_str = "Nah, I will \nwork out! Yeah!"; 
	if (options[top_option] == 9) left_str = "The boys are \ncalling for drinks."; 

	if (options[bottom_option] == 0) right_str = "You are tired. \nGo to Bed"; 
	if (options[bottom_option] == 1) right_str = "Ah I am tired, \nsnooze the alarm"; 
	if (options[bottom_option] == 2) right_str = "Need to attend my \nkid's baseball match"; 
	if (options[bottom_option] == 3) right_str = "Today is garbage day.. \nneed to throw the \ntrash"; 
	if (options[bottom_option] == 4) right_str = "Need to do \nthe dishes"; 
	if (options[bottom_option] == 5) right_str = "Its my work to \ncook today"; 
	if (options[bottom_option] == 6) right_str = "Need to fix the \nleak in the \nroof"; 
	if (options[bottom_option] == 7) right_str = "The wife is asking\nto go on a trip"; 
	if (options[bottom_option] == 8) right_str = "Nah, I will \nwork out! Yeah!"; 
	if (options[bottom_option] == 9) right_str = "The boys are \ncalling for drinks."; 

}
// uint32_t options[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // 0 - bed, 1 - clock , 2 - baseball, 3 - bin, 4 - brass_pot, 5 - enamel_pot, 6 - hammer, 7 - camera, 8 - weight, 9 - wine
void PlayMode::update_time(uint8_t option_selected) {
	// Update time left
	if (options[option_selected] == 0) time_left -= sleep_cost;
	if (options[option_selected] == 1) time_left -= sleep_cost;
	if (options[option_selected] == 2) time_left -= home_cost;
	if (options[option_selected] == 3) time_left -= home_cost;
	if (options[option_selected] == 4) time_left -= home_cost;
	if (options[option_selected] == 5) time_left -= home_cost;
	if (options[option_selected] == 6) time_left -= home_cost;
	if (options[option_selected] == 7) time_left -= home_cost;
	if (options[option_selected] == 8) time_left -= gym_cost;
	if (options[option_selected] == 9) time_left -= friends_cost;

	// Update total time spent 
	if (options[option_selected] == 0) sleep_win_cnt += sleep_cost;
	if (options[option_selected] == 1) sleep_win_cnt += sleep_cost;
	if (options[option_selected] == 2) home_win_cnt += home_cost;
	if (options[option_selected] == 3) home_win_cnt += home_cost;
	if (options[option_selected] == 4) home_win_cnt += home_cost;
	if (options[option_selected] == 5) home_win_cnt += home_cost;
	if (options[option_selected] == 6) home_win_cnt += home_cost;
	if (options[option_selected] == 7) home_win_cnt += home_cost;
	if (options[option_selected] == 8) gym_win_cnt += gym_cost;
	if (options[option_selected] == 9) friends_win_cnt += friends_cost;


	// Call end 
	if(time_left <= 0) {
		
		if(home_win_cnt >= home_win && sleep_win_cnt >= sleep_win && gym_win_cnt >= gym_win && friends_win_cnt >= friends_win) { // True End
			init_str = "So you won huh?\nYou took care of the family.\nTook care of your health\nYou also kept your social life intact.\nGood Job!! You win!\nPress Space to Replay.";
			render_object(Trophy, weight);
		}
		else if(home_win_cnt >= home_win && sleep_win_cnt >= sleep_win && gym_win_cnt < gym_win && friends_win_cnt >= friends_win) { // No win but friends and fam end
			init_str = "So you didn't win?\nWho cares?!?\nYou have a good family.\nYou have the boys to hang with.\nYou don't care about the win, right? RIGHT?\nPress Space to Replay.";
			render_object(nullptr, nullptr);
		}
		else if(home_win_cnt <= home_win && sleep_win_cnt >= sleep_win && gym_win_cnt >= gym_win && friends_win_cnt >= friends_win) { // No fam end
			init_str = "YOU WON!!\nAnd the boys are here to PARTY\nIgnore the fact your wife and kids\nleft you and took all the prize money...\nPress Space to Replay.";
			render_object(Trophy, weight);
		}
		else if(sleep_win_cnt <= sleep_win) { // No sleep end
			init_str = "You had to be sent to the ER\ndue to severe sleep deprivation.\nYour friends and family care for you...\nPress Space to Replay.";
			render_object(nullptr, nullptr);
		}
		else if(home_win_cnt >= home_win && sleep_win_cnt >= sleep_win && gym_win_cnt >= gym_win && friends_win_cnt <= friends_win) { // No boys end
			init_str = "Congrats you won!\nYour family is with you.\nUnfortunately you neglected your friends\nand they now think\nthe win has gone to your head\nand you are a know-it-all.\nPress Space to Replay.";
			render_object(Trophy, weight);
		}
		else if(home_win_cnt <= home_win && sleep_win_cnt >= sleep_win && gym_win_cnt >= gym_win && friends_win_cnt <= friends_win) { // All gym end
			init_str = "You win..\nBut you stand alone.\nThe lonely warrior.\nWas it worth it?\nPress Space to Replay.";
			render_object(Trophy, weight);
		}
		else if(home_win_cnt <= home_win && sleep_win_cnt >= sleep_win && gym_win_cnt <= gym_win && friends_win_cnt <= friends_win) { // All sleep end
			init_str = "All you really did was sleep?\nHow? Why?\nPress Space to Replay.";
			render_object(nullptr, nullptr);;
		}
		else if(home_win_cnt <= home_win && sleep_win_cnt >= sleep_win && gym_win_cnt <= gym_win && friends_win_cnt >= friends_win) { // All party end
			init_str = "Family? Trophy?\nWho Cares?\nAll time PARTY with the boys!\nPress Space to Replay.";
			render_object(nullptr, nullptr);
		}
		else if(home_win_cnt >= home_win && sleep_win_cnt >= sleep_win && gym_win_cnt <= gym_win && friends_win_cnt <= friends_win) { // All family end
			init_str = "You took responsibility\nand took care of your\nfamily, at the expense\nof your dreams and social life.\nPress Space to Replay.";
			render_object(nullptr, nullptr);
		}
		
		init(1);
	}
}

void PlayMode::init(int val) {
	if(val == 0) { // Only on first run
		get_transforms();
		camera->transform->rotation.x = 0.688107f; camera->transform->rotation.y = 0.008520f; camera->transform->rotation.z = 0.005893f;
		camera->transform->position.x = 33.039566f; camera->transform->position.y = -4.836715f;  camera->transform->position.z = 8.327226f;
		init_str = "Press Space to Begin";
		left_str = "";
		right_str = "";
		//end_str = "";
		render_object(nullptr, nullptr);
	}
	// Set initial values
	init_flg = 1;
	time_left = 200;
	
	gym_cost = 5;
	home_cost = 5;
	friends_cost = 10;
	sleep_cost = 10;

	sleep_win = 50;
	home_win = 70;
	friends_win = 30;
	gym_win = 50;

	home_win_cnt = 0;
	friends_win_cnt = 0;
	gym_win_cnt = 0;
	sleep_win_cnt = 0;


	top_opt = 0;
	bottom_opt = 0;
	do_render = 1;

	left_str = "";
	right_str = "";
	do_something_else_str = "";

	

}

PlayMode::PlayMode() : scene(*Game4_scene) {
	init(0);

	//start music loop playing:
	MainLoop = Sound::loop(*Main_sample, 0.75f, 0.0f);

	// =============================================
	// Get all the char textures and set up shaders
	// =============================================
	load_char("NotoSansMono_Condensed-Regular.ttf");
}

PlayMode::~PlayMode() {
}


// Function code referenced from Game_0 PPU code and https://learnopengl.com/In-Practice/Text-Rendering

void PlayMode::load_char(char *fontfile) {
	program = gl_compile_program(
		//vertex shader:
		"#version 330 core\n"
		"layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
    	"gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    	"TexCoords = vertex.zw;\n"
		"}\n"
	,
		//fragment shader:
		"#version 330 core\n"
		"in vec2 TexCoords;\n"
		"out vec4 color;\n"
		"uniform sampler2D text;\n"
		"uniform vec3 textColor;\n"
		"void main()\n"
		"{\n"
		"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
		"color = vec4(textColor, 1.0) * sampled;\n"
		"}\n"
	);

	/* Initialize FreeType and create FreeType font face. */
	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // compile and setup the shader
    // ----------------------------
    glUseProgram(program);
    projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

		
	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, fontfile, 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

// Function code referenced from https://learnopengl.com/In-Practice/Text-Rendering

void PlayMode::draw_msg(std::string text, float x, float y, float scale, glm::uvec2 const &drawable_size)
{
	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "textColor"), 1.0, 1.0, 1.0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
	float x_ori = x;
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
		if(*c == '\n') { // new line logic
			y = y - 50.0f*scale;
			x = x_ori;
			continue;
		}
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
		//printf("xpos %f ypos %f\n", xpos, ypos);
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a && left.debounce_flg == 0) {
			left.debounce_flg = 1;
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d && right.debounce_flg == 0) {
			right.debounce_flg = 1;
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE && space.debounce_flg == 0) {
			space.debounce_flg = 1;
			space.downs += 1;
			space.pressed = true;
			return true;
		}

	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.debounce_flg = 0;
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.debounce_flg = 0;
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.debounce_flg = 0;
			space.pressed = false;
			return true;
		}
		}
	return false;
}

void PlayMode::update(float elapsed) {


	if(!init_flg) { // If not init
		// run game
		if(do_render) { // If objects not rendered
			do_render = 0;
			// uint32_t options[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // 0 - bed, 1 - clock , 2 - baseball, 3 - bin, 4 - brass_pot, 6 - enamel_pot, 7 - hammer, 8 - camera, 9 - weight, 10 - wine
			// Randomly select choices and left right location
			if (std::rand() % 2 == 0) {// set top as weight 
				top_opt = 8;
				bottom_opt = std::rand() % 10;
				while(bottom_opt == top_opt) bottom_opt = std::rand() % 10;
			}
			else {
				bottom_opt = 8;
				top_opt = std::rand() % 10;
				while(top_opt == bottom_opt) top_opt = std::rand() % 10;
			}

			render_level(top_opt, bottom_opt); // Render level based on top/left and bottom/right options

		}

		if(left.pressed && left.debounce_flg == 1) { // Left option selected
			left.debounce_flg = 2;
			do_render = 1; 
			update_time(top_opt);
		}

		else if(right.pressed && right.debounce_flg == 1) { // Right option selected
			right.debounce_flg = 2;
			do_render = 1;
			update_time(bottom_opt);
		}
		else if(space.pressed && space.debounce_flg == 1) { // Do something else
			space.debounce_flg = 2;
			do_render = 1;
		}

	}
	else {		
		if(space.pressed && space.debounce_flg == 1) { // Start game
			space.debounce_flg = 2;
			init_flg = 0;
			init_str = "";
			do_something_else_str = "Do Something Else";
		}
	}
	//printf("Needed - sleep_win = %d, home_win = %d, gym_win = %d, friends_win = %d\n", sleep_win, home_win, gym_win, friends_win);
	//printf("time_left = %d, sleep_win = %d, home_win = %d, gym_win = %d, friends_win = %d\n", time_left, sleep_win_cnt, home_win_cnt, gym_win_cnt, friends_win_cnt);


	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	space.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);
	
	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);
	if(init_flg) { // If init or game over
		draw_msg(init_str, 450.0f, 650.0f, 0.75f, drawable_size);
	}
	else { // Runtime values
		draw_msg(std::string("Time Left:") + std::to_string(time_left), 450.0f, 650.0f, 1.0f, drawable_size);
		draw_msg(left_str, 50.0f, 400.0f, 0.5f, drawable_size);
		draw_msg(right_str, 1000.0f, 400.0f, 0.5f, drawable_size);
		draw_msg(do_something_else_str, 450.0f, 30.0f, 1.0f, drawable_size);
	}
	

	
}
