/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#ifndef CUBE_HEADER
#define CUBE_HEADER
	#include <memory>
	#include <GL/glew.h>
	#include <SFML/OpenGL.hpp>

    namespace example
    {
		class   Color_Buffer_Rgba8888;
		typedef Color_Buffer_Rgba8888 Texture;

        class Cube
        {
        private:

            // Índices para indexar el array vbo_ids:

            enum
            {
                COORDINATES_VBO,
				TEXTURE_UVS_VBO,
				NORMALS_VBO,
                INDICES_IBO,
                VBO_COUNT
            };

            // Arrays de datos del cubo base:

            static const GLfloat coordinates[];
            static const GLfloat normals    [];
			static const GLfloat texture_uvs[];
            static const GLubyte indices    [];
			

        private:

            GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
            GLuint vao_id;                  // Id del VAO del cubo

			bool   has_texture;
			GLuint texture_id;

		public:

			Cube(const char * texture_path = 0);
           ~Cube();

            void render ();
			std::auto_ptr< Texture > load_texture(const char * texture_file_path);

        };

    }

#endif
