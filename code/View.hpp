/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include <GL/glew.h>            // Debe incluirse antes que gl.h
    #include "Cube.hpp"
	#include "Polar_Sphere.hpp"
    #include <string>
	#include "Camera.hpp"
	#include <memory>
	#include "Skybox.hpp"

    namespace example
    {

        class View
        {
        private:

			// Shaders
            static const std::string   vertex_shader_code;
            static const std::string fragment_shader_code;
			static const std::string   vertex_sphere_breath_shader_code;
			static const std::string fragment_sphere_breath_shader_code;
			static const std::string effect_vertex_shader_code;
			static const std::string effect_fragment_shader_code;

			// Postpro
			static const GLsizei framebuffer_width = 512;
			static const GLsizei framebuffer_height = 512;
			GLuint framebuffer_id;
			GLuint depthbuffer_id;
			GLuint out_texture_id;
			GLuint effect_program_id;
			GLuint triangle_vbo0;
			GLuint triangle_vbo1;
			GLuint triangle_vbo2;
			static const GLuint indices_triangle[];

			// Cubes
			Cube *   cube, *cube2;
            GLint  model_view_matrix_id;
            GLint  projection_matrix_id;
            GLint      normal_matrix_id;
			GLuint program_id;
			float  angle;

			//Spheres
			Polar_Sphere * sphere;
			GLint  model2_view_matrix_id;
			GLint  projection2_matrix_id;
			GLint             radius_id;
			GLint				   t_id;
			GLint				   k_id;
			GLuint program_sphere_id;
            float angle_sphere;

			// Camera
			Camera * camera;
			Skybox * skybox;

			float  angle_around_x;
			float  angle_around_y;
			float  angle_delta_x;
			float  angle_delta_y;

			bool   pointer_pressed;
			int    last_pointer_x;
			int    last_pointer_y;

			int	   width;
			int    height;

        public:

            View(int width, int height);

            void   update ();
            void   render ();
            void   resize (int width, int height);
			void   on_key(int key_code);
			void   on_drag(int pointer_x, int pointer_y);
			void   on_click(int pointer_x, int pointer_y, bool down);

        private:

            GLuint compile_shaders        ();
			GLuint compile_shaders(const std::string & vertex_shader_code, const std::string & fragment_shader_code);
            void   show_compilation_error (GLuint  shader_id);
            void   show_linkage_error     (GLuint program_id);

            void   configure_material     (GLuint program_id);
            void   configure_light        (GLuint program_id);
			void   configure_fog          (GLuint program_id);

			void   build_framebuffer();
			void   render_framebuffer();

        };

    }

#endif
