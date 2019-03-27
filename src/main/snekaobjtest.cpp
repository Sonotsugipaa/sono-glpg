#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <map>
#include <exception>

#include <cstdlib>
#include <cstdio>

#include "sneka/snekaruntime.hpp"
#include "sneka/renderobject.hpp"
#include "sneka/direction.hpp"
#include "sneka/gridobject.hpp"
#include "sneka/levelrenderer.hpp"
#include "sneka/levelobject.hpp"
#include "sneka/amscript_ext.hpp"
#include "sneka/chunk.hpp"

#include "runtime.hpp"
#include "shader.hpp"
#include "globject.hpp"
#include "transition.hpp"
#include "utils.tpp"
#include "read_utils.hpp"
#include "trace.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <SDL2/SDL.h>

using std::cout;
using std::endl;
using std::size_t;
using glm::vec3;
using namespace sneka;
using namespace gla;

#define SCREEN_WIDTH         (800)
#define WINDOW_SIZE_FACTOR   (0.7)
#define FPS                  (60.0)
#define FRAMERATE            (1000.0/FPS)
#define STEPS                (10)
#define FRAMES               (500)
#define CAM_DISTANCE         (1.0)
#define CAM_HEIGHT           (2.0)
#define CAM_PITCH            (1.1)
#define CURVATURE            (0.04)
#define TILES                (30)
#define OBJECTS              (250)
#define DRUGS                (0.0)
#define WORLD_MIN_Z          (0.2)
#define WORLD_MAX_Z          (100.0)



void main_body(SnekaRuntime&, Chunk**, std::size_t, LevelRenderer&);

int main(int argn, char** argv) {
/* ------ INIT -------------------------------------------------------------- */
	int W = SCREEN_WIDTH, H = SCREEN_WIDTH * 9 / 16;

	if(argn > 1) {
		int read;
		read_dec_int(argv[1], &read);
		if(read > 0) {
			W = read;
			H = W * 9 / 16;
		}
	}
	if(argn > 2) {
		int read;
		read_dec_int(argv[2], &read);
		if(read > 0) {
			H = read;
		}
	}

	trace_sigaction_init();  TRACE;


	SnekaRuntime runtime = SnekaRuntime(
			"sneka level render test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			W, H, true, true );  TRACE;

	amscript::Amscript level_ams;
	{
		std::ifstream is = std::ifstream("assets/lvl/1.lvl");
		level_ams = amscript::Amscript(is);
	}


	MeshLoader* mesh_loader = new MeshLoader();
	//FileChunkLoader chunk_loader;
	LevelObjectLoader* lvl_loader = new LevelObjectLoader(*mesh_loader);
	std::map<char, std::string> obj_map;
	{
		std::string path_to_obj = level_ams.resolveSymbol("$object_path");
		if(path_to_obj[path_to_obj.size()-1] != '/')
			path_to_obj.push_back('/');
		std::vector<std::string> obj_enum = level_ams.compute("$object_enum", "");
		std::size_t off = 0;
		for(std::size_t i=0; i < obj_enum.size(); ++i) {
			std::string obj_name = path_to_obj + obj_enum[i];
			if(! obj_enum[i].empty()) {
				char digit = digit_to_ch(i-off);
				obj_map[digit] = obj_name;
				cout << obj_name << " --> " << digit << endl;
			} else {
				++off;
			}
		}
	}
	FolderChunkLoader* chunk_loader = new FolderChunkLoader(
			*lvl_loader, obj_map,
			"assets/lvl" );
	Chunk* chunk_0_0 = &chunk_loader->get(0, 0);
	Chunk* chunk_0_m1 = &chunk_loader->get(0, -1);
	Chunk** chunks = new Chunk*[2] {
		chunk_0_0, chunk_0_m1
	};
	bool swapped = chunk_0_0->swap(glm::ivec2(0, 0), glm::ivec2(0, -5), *chunk_0_m1);
	std::cout << "inter-chunk swap " << (swapped? "successful" : "failed") << std::endl;

	FloorObject floor = FloorObject(mesh_loader->get("assets/tile_caved.mesh"), 40);
	floor.setColor(glm::vec4(0.4f, 0.7f, 0.4f, 0.5f));

	LevelRenderer* renderer = new LevelRenderer(
			runtime,
			floor, TILES,
			CURVATURE, DRUGS );  TRACE;

	std::cout << "chunk size: " << chunk_0_0->getSize() << std::endl;
	renderer->setView(
			vec3(
				-static_cast<GLfloat>(chunk_0_0->getSize() / 2),
				-2.0f,
				-static_cast<GLfloat>(chunk_0_0->getSize())
			),
			0.0f,
			0.75f );
	renderer->setPerspective(
			90.0f,
			(GLfloat) WORLD_MIN_Z,
			(GLfloat) WORLD_MAX_Z );

	renderer->addLight(glm::vec3(2.0f, 1.0f, 1.0f));

/* ------ BODY ------------------------------------------------------------- */
	try {
		TRACE;
		main_body(runtime, chunks, 2, *renderer);
		TRACE;
	} catch(std::runtime_error ex) {
		std::cerr
				<< "Error: " << ex.what()
				<< " (\"" << gla_trace_file << "\", " << gla_trace_line << ")" << endl;
	}

/* ------ END -------------------------------------------------------------- */
	TRACE;
	delete chunks; TRACE; // just an array of pointers
	delete renderer; TRACE;

	// Exact order
	//delete chunk_0_0; TRACE; // Don't delete this, it has been new'd by chunk_loader
	delete chunk_loader; TRACE;
	delete lvl_loader; TRACE;
	delete mesh_loader; TRACE;

	return EXIT_SUCCESS;
}


void main_body(
		SnekaRuntime& runtime,
		Chunk** chunks,  std::size_t chunks_n,
		LevelRenderer& renderer
) {
	(void) runtime;
	TRACE;

	/*
	LevelObject* obj1;
	LevelObject* obj2;
	obj1 = new LevelObject(lvl_loader.get("assets/obj/bloc.obj"));  TRACE;
	obj1->setGridPosition(-1, -3);  TRACE;
	renderer.putObject(*obj1);  TRACE;
	obj2 = new LevelObject(lvl_loader.get("assets/obj/pyr.obj"));  TRACE;
	obj2->setGridPosition(1, -3);  TRACE;
	renderer.putObject(*obj2);  TRACE;
	*/
	for(std::size_t i=0; i < chunks_n; ++i) {
		auto iter = chunks[i]->getObjectMap().begin();
		auto end = chunks[i]->getObjectMap().end();
		while(iter != end) {
			renderer.putObject(*(iter->second));
			++iter;
		}
	}

	TRACE;
	while(runtime.pollEvents()) {
		std::this_thread::sleep_for(std::chrono::milliseconds((int) FRAMERATE));
		renderer.renderFrame();
	}
	TRACE;
}
