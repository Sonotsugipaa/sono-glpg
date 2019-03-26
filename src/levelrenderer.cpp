#include "sneka/levelrenderer.hpp"

#include "sneka/renderobject.hpp"
#include "sneka/mesh.hpp"
//#include "sneka/pool.hpp"

#include "utils.tpp"
#include "trace.hpp"

#include <glm/gtc/matrix_transform.hpp>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>



namespace {

	glm::mat4 get_view(glm::vec3 pos, GLfloat yaw, GLfloat pitch) {
		glm::mat4 retn = glm::mat4(1.0f);

		// pitch
		retn = glm::rotate(
				retn,
				pitch,
				glm::vec3(1.0f, 0.0f, 0.0f) );

		// yaw
		retn = glm::rotate(
				retn,
				yaw,
				glm::vec3(0.0f, 1.0f, 0.0f) );

		return glm::translate(retn, pos);
	}

	void gl_features_enable() {
		/*
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.05f);
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		*/

		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
	}

	void gl_features_disable() {
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	}

}



namespace sneka {

	void LevelRenderer::lights_cache_compute() {
		if(! lights_cache_changed) return;

		for(std::size_t i=0; i < lights.size(); i+=1) {
			for(std::size_t j=0; j < 3; j+=1) {
				lights_cache[(i*3)+j] = glm::normalize(lights[i])[j];
			}
		}
		lights_cache_changed = false;
	}


	LevelRenderer::LevelRenderer(
			const SnekaRuntime & runtime,
			FloorObject& floor, GLuint repeat_stride,
			GLfloat curv, GLfloat drugs
	):
			Renderer(runtime),
			lights_cache_changed(true),
			floor(&floor),
			floor_tiles(floor.side_length), floor_tiles_half(floor_tiles / 2.0f),
			floor_tile_size(1.0f),
			repeat_stride(repeat_stride),
			curvature(curv),
			drugs(drugs),
			light_color(glm::vec3(1.0f, 1.0f, 1.0f)),
			clear_color(glm::vec3(0.05f, 0.05f, 0.05f)),
			view_pos(glm::vec3(0.0f, 0.0f, 0.0f)),
			view_yaw(0.0f), view_pitch(0.0f),
			proj_fov_y(90.0f), proj_z_near(0.2f), proj_z_far(100.0f),
			fog_intensity(0.0f)
	{
		TRACE;
	}

	LevelRenderer::~LevelRenderer() { TRACE; }


	RenderObject* LevelRenderer::popObject() {
		TRACE;
		auto iter = objects.begin();
		if(iter == objects.end())
			return nullptr;
		return iter->second;
		//std::cout << "now rendering " << obj.uid << std::endl;
	}

	RenderObject* LevelRenderer::getObject(uid_t uid) {
		TRACE;
		auto iter = objects.find(uid);
		if(iter == objects.end())
			return nullptr;
		return iter->second;
		//std::cout << "now rendering " << obj.uid << std::endl;
	}

	// this NEEDS to be tested, the getObject(uid_t) call could be ambiguous
	const RenderObject * LevelRenderer::getObject(uid_t uid) const {
		return const_cast<const RenderObject *>(getObject(uid));
	}

	void LevelRenderer::putObject(RenderObject& obj) {
		TRACE;
		objects[obj.getUid()] = &obj;
		//std::cout << "now rendering " << obj.uid << std::endl;
	}

	void LevelRenderer::removeObject(RenderObject& obj) {
		TRACE;
		removeObject(obj.getUid());
		//std::cout << "not rendering " << obj.uid << std::endl;
	}

	void LevelRenderer::removeObject(uid_t uid) {
		TRACE;
		auto iter = objects.find(uid);
		if(iter != objects.end())
			objects.erase(iter);
	}

	std::size_t LevelRenderer::getObjectsCount() const {
		TRACE;
		return objects.size();
	}


	FloorObject & LevelRenderer::getFloorObject() {
		TRACE;
		return *floor;
	}


	bool LevelRenderer::addLight(glm::vec3 dir) {
		if(lights.size() >= max_lights) return false;
		lights_cache_changed = true;

		lights.push_back(dir);
		return true;
	}

	// this might need some proper testing
	bool LevelRenderer::setLight(std::size_t i, glm::vec3 dir) {
		lights_cache_changed = true;

		std::size_t size = lights.size();
		if(i < size) {
			lights[i] = dir;
			return true;
		}
		if(i == size) {
			return addLight(dir);
		}
		return false;
	}

	void LevelRenderer::clearLights() {
		lights_cache_changed = true;
		lights.resize(0);
	}

	std::vector<glm::vec3> LevelRenderer::getLights() const {
		return lights;
	}

	std::size_t LevelRenderer::getLightsCount() const {
		return lights.size();
	}


	glm::vec3 LevelRenderer::getLightColor() const {
		return light_color;
	}

	void LevelRenderer::setLightColor(glm::vec3 col) {
		light_color = col;
	}

	glm::vec3 LevelRenderer::getClearColor() const {
		return clear_color;
	}

	void LevelRenderer::setClearColor(glm::vec3 col) {
		clear_color = col;
	}



	void LevelRenderer::setView(glm::vec3 pos, GLfloat yaw, GLfloat pitch) {
		TRACE;
		view_pos = pos;
		view_yaw = yaw;
		view_pitch = pitch;
	}


	void LevelRenderer::setPerspective(
			GLfloat fov_y,
			GLfloat zNear, GLfloat zFar
	) {
		proj_fov_y = fov_y;
		proj_z_near = zNear;
		proj_z_far = zFar;
	}


	// Watch out, we got a big one over here!
	void LevelRenderer::renderFrame() {
		using namespace shader::level;

		TRACE;

		runtime.getWorldShader().use();
		gl_features_enable();

		glClearColor(clear_color[0], clear_color[1], clear_color[2], 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			glm::vec4 fog = glm::vec4(clear_color, fog_intensity);
			glm::mat4 proj_mat = glm::perspective(
					proj_fov_y,
					runtime.getViewportRatio(),
					proj_z_near,
					proj_z_far );

			glUniform4fv(uniform_fog, 1, &fog[0]);
			glUniform1f(uniform_time, time.millis() / 1000.0f);
			glUniform1f(uniform_curvature, curvature);
			glUniform1f(uniform_drugs, drugs);
			glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, &proj_mat[0][0]);
		}

		glm::mat4 mat_view = get_view(
				glm::vec3(
					view_pos[0],
					view_pos[1],
					view_pos[2]
				),
				view_yaw, view_pitch );

		/*
		glm::vec3 view_dir =
				glm::rotate(
					glm::rotate(
						glm::mat4(1.0f),
						-view_yaw,
						glm::vec3(0.0f, 1.0f, 0.0f)
					),
					-view_pitch,
					glm::vec3(1.0f, 0.0f, 0.0f)
				) *
				glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		view_dir = glm::normalize(view_dir);
		*/

		lights_cache_compute();
		glUniform3fv(uniform_light_color, 1, &light_color[0]);
		glUniform3fv(uniform_light_dir, lights.size(), lights_cache);
		glUniform1i(uniform_light_count, lights.size());

		// draw the floor with a matrix that translates back every tile
		glm::mat4 mat_view_tr = glm::translate(
				mat_view,
				glm::vec3(
					-(floor_tiles_half + floor_tile_size * glm::floor(view_pos[0] / floor_tile_size)),
					0.0f,
					-(floor_tiles_half + floor_tile_size * glm::floor(view_pos[2] / floor_tile_size))
				) );
		glm::vec3 pos = mat_view_tr * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glUniform3fv(uniform_view_pos, 1, &pos[0]);
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, &mat_view_tr[0][0]);

		TRACE;
		floor->draw();

		/* can be optimized: everything is drawn 9 times, but a
		 * maximum of 4 iterations can theoretically suffice */
		for(int x=-1; x<=1; x+=1) {
			for(int z=-1; z<=1; z+=1) {
				mat_view_tr = glm::translate(
						mat_view,
						glm::vec3(
							static_cast<GLfloat>(x) * repeat_stride,
							0.0f,
							static_cast<GLfloat>(z) * repeat_stride
						) );
				glm::vec3 pos = - (mat_view_tr * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				glUniform3fv(uniform_view_pos, 1, &pos[0]);
				glUniformMatrix4fv(uniform_view, 1, GL_FALSE, &mat_view_tr[0][0]);

				auto iter = objects.begin();
				while(iter != objects.end()) {
					TRACE;
					iter->second->draw();
					++iter;
				}
			}
		}

		SDL_GL_SwapWindow(runtime.window);

		gl_features_disable();
	}

}
