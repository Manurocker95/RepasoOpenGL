/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include "Cube.hpp"
#include <SFML/OpenGL.hpp>
#include "Color_Buffer_Rgba8888.hpp"
#include "Settings.h"

extern "C"
{
#include <targa.h>
}

namespace example
{

    const GLfloat Cube::coordinates[] =
    {
        -1.f, +1.f, +1.f,
        +1.f, +1.f, +1.f,
        +1.f, -1.f, +1.f,
        -1.f, -1.f, +1.f,
        -1.f, +1.f, -1.f,
        -1.f, +1.f, +1.f,
        -1.f, -1.f, +1.f,
        -1.f, -1.f, -1.f,
        +1.f, +1.f, -1.f,
        -1.f, +1.f, -1.f,
        -1.f, -1.f, -1.f,
        +1.f, -1.f, -1.f,
        +1.f, +1.f, +1.f,
        +1.f, +1.f, -1.f,
        +1.f, -1.f, -1.f,
        +1.f, -1.f, +1.f,
        -1.f, -1.f, +1.f,
        +1.f, -1.f, +1.f,
        +1.f, -1.f, -1.f,
        -1.f, -1.f, -1.f,
        -1.f, +1.f, -1.f,
        +1.f, +1.f, -1.f,
        +1.f, +1.f, +1.f,
        -1.f, +1.f, +1.f,
    };

	const GLfloat Cube::texture_uvs[] =
	{
		0.f, 0.f,             // 0
		1.f, 0.f,             // 1
		1.f, 1.f,             // 2
		0.f, 1.f,             // 3
		0.f, 0.f,             // 4
		1.f, 0.f,             // 5
		1.f, 1.f,             // 6
		0.f, 1.f,             // 7
		0.f, 0.f,             // 8
		1.f, 0.f,             // 9
		1.f, 1.f,             // 10
		0.f, 1.f,             // 11
		0.f, 0.f,             // 12
		1.f, 0.f,             // 13
		1.f, 1.f,             // 14
		0.f, 1.f,             // 15
		0.f, 0.f,             // 16
		1.f, 0.f,             // 17
		1.f, 1.f,             // 18
		0.f, 1.f,             // 19
		0.f, 0.f,             // 20
		1.f, 0.f,             // 21
		1.f, 1.f,             // 22
		0.f, 1.f,             // 23
	};
	const GLfloat Cube::normals[] =
	{
		0.f,  0.f, +1.f,
		0.f,  0.f, +1.f,
		0.f,  0.f, +1.f,
		0.f,  0.f, +1.f,
		-1.f,  0.f,  0.f,
		-1.f,  0.f,  0.f,
		-1.f,  0.f,  0.f,
		-1.f,  0.f,  0.f,
		0.f,  0.f, -1.f,
		0.f,  0.f, -1.f,
		0.f,  0.f, -1.f,
		0.f,  0.f, -1.f,
		+1.f,  0.f,  0.f,
		+1.f,  0.f,  0.f,
		+1.f,  0.f,  0.f,
		+1.f,  0.f,  0.f,
		0.f, -1.f,  0.f,
		0.f, -1.f,  0.f,
		0.f, -1.f,  0.f,
		0.f, -1.f,  0.f,
		0.f, +1.f,  0.f,
		0.f, +1.f,  0.f,
		0.f, +1.f,  0.f,
		0.f, +1.f,  0.f,
	};

    const GLubyte Cube::indices[] =
    {
        1,  0,  3,              // front
        1,  3,  2,
        5,  4,  7,              // left
        5,  7,  6,
        9,  8,  11,             // back
        9,  11, 10,
        13, 12, 15,             // right
        13, 15, 14,
        17, 16, 19,             // bottom
        17, 19, 18,
        20, 23, 22,             // top
        20, 22, 21,
    };

    Cube::Cube(const char * texture_path)
		:
		has_texture(false)
	{
        // Se generan índices para los VBOs del cubo:

        glGenBuffers (VBO_COUNT, vbo_ids);
        glGenVertexArrays (1, &vao_id);

        // Se activa el VAO del cubo para configurarlo:

        glBindVertexArray (vao_id);

        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de normales y se vinculan al VAO:

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de color y se vinculan al VAO:

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texture_uvs), texture_uvs, GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un IBO los datos de índices:

        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray (0);

		// Se carga la textura si es necesario:

		if (texture_path != 0)
		{
			std::auto_ptr< Texture > texture = load_texture(texture_path);

			has_texture = texture.get() != 0;

			if (has_texture)
			{
				// Se habilitan las texturas, se genera un id para un búfer de textura,
				// se selecciona el búfer de textura creado y se configuran algunos de
				// sus parámetros:

				glEnable(GL_TEXTURE_2D);
				glGenTextures(1, &texture_id);
				glBindTexture(GL_TEXTURE_2D, texture_id);

				// Desde OpenGL 3.0:
				{
					glTexImage2D
					(
						GL_TEXTURE_2D,
						0,
						GL_RGBA,
						texture->get_width(),
						texture->get_height(),
						0,
						GL_RGBA,
						GL_UNSIGNED_BYTE,
						texture->colors()
					);

					glGenerateMipmap(GL_TEXTURE_2D);
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
		}
    }

    Cube::~Cube()
    {
		if (has_texture)
		{
			glDeleteTextures(1, &texture_id);
		}

        // Se liberan los VBOs y el VAO usados:

        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (VBO_COUNT, vbo_ids);
    }

    void Cube::render ()
    {

		if (has_texture)
		{
			glBindTexture(GL_TEXTURE_2D, texture_id);
		}

        // Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos:

        glBindVertexArray (vao_id);
        glDrawElements    (GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray (0);
    }


	std::auto_ptr< Texture > Cube::load_texture(const char * texture_file_path)
	{
		std::auto_ptr< Texture > texture;
		tga_image                loaded_image;

		if (tga_read(&loaded_image, texture_file_path) == TGA_NOERR)
		{
			// Si se ha podido cargar la imagen desde el archivo, se crea un búfer para una textura:

			texture.reset(new Texture(loaded_image.width, loaded_image.height));

			// Se convierte el formato de píxel de la imagen cargada a RGBA8888:

			tga_convert_depth(&loaded_image, texture->bits_per_color());
			tga_swap_red_blue(&loaded_image);

			// Se copian los pixels del búfer de la imagen cargada al búfer de la textura:

			Texture::Color * loaded_image_pixels = reinterpret_cast< Texture::Color * >(loaded_image.image_data);
			Texture::Color * loaded_image_pixels_end = loaded_image_pixels + loaded_image.width * loaded_image.height;
			Texture::Color * image_buffer_pixels = texture->colors();

			while (loaded_image_pixels <  loaded_image_pixels_end)
			{
				*image_buffer_pixels++ = *loaded_image_pixels++;
			}

			tga_free_buffers(&loaded_image);
		}

		return (texture);
	}

}
