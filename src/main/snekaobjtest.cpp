#include <iostream>
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



namespace {

	class TestObject : public LevelObject {
	private:
		static LevelObjectCounter counter_test;
	public:
		TestObject(const sneka::SnekaRuntime & rt):
				LevelObject::LevelObject(counter_test, rt, "assets/arrow.mesh")
		{
			TRACE;
			cout << "Created test object" << endl;
		}

		virtual ~TestObject() {
			TRACE;
			cout << "Destroyed test object" << endl;
		}
	};

	LevelObjectCounter TestObject::counter_test;

}


void main_body(SnekaRuntime&, LevelRenderer&);

int main(int argn, char** argv) {
	using namespace sneka;
	using namespace gla;

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
			W, H, true, true);  TRACE;


	FloorObject floor = FloorObject(runtime, "assets/tile_caved.mesh", 40);
	floor.setColor(glm::vec4(0.4f, 0.7f, 0.4f, 1.0f));
	//floor.shade = (float) FLOOR_SHADE;
	//floor.reflect = (float) FLOOR_REFLECT;
	//floor.reflect_falloff = (float) FLOOR_REFLECT_FO;

	LevelRenderer* renderer = new LevelRenderer(
			runtime,
			floor, TILES,
			CURVATURE, DRUGS );  TRACE;

	renderer->setView(vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f);
	renderer->setPerspective(
					90.0f,
					(GLfloat) WORLD_MIN_Z,
					(GLfloat) WORLD_MAX_Z);

	renderer->addLight(glm::vec3(2.0f, 1.0f, 1.0f));

/* ------ BODY ------------------------------------------------------------- */
	try {
		TRACE;
		main_body(runtime, *renderer);
		TRACE;
	} catch(std::runtime_error ex) {
		std::cerr
			<< "Error: " << ex.what()
			<< " (\"" << gla_trace_file << "\", " << gla_trace_line << ")" << endl;
	}

/* ------ END -------------------------------------------------------------- */
	TRACE;
	delete renderer;  TRACE;

	return EXIT_SUCCESS;
}


void main_body(
		SnekaRuntime& runtime,
		LevelRenderer& renderer
) {
	TRACE;
	TestObject obj1 = TestObject(runtime);  TRACE;
	obj1.setGridPosition(-1, -5);  TRACE;
	renderer.putObject(obj1);
	TestObject obj2 = TestObject(runtime);  TRACE;
	obj2.setGridPosition(1, -5);  TRACE;
	renderer.putObject(obj2);

	Timer timer;

	TRACE;
	while(runtime.pollEvents()) {
		std::this_thread::sleep_for(std::chrono::milliseconds((int) FRAMERATE));
		renderer.renderFrame();
	}
}
