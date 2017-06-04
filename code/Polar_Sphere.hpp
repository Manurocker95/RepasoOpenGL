/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#include <GL/glew.h>

#pragma once

namespace example
{

	class Polar_Sphere
	{
		struct Polar_Coordinate
		{
			float alpha, beta;
		};

		enum
		{
			COORDINATES_VBO,
			NORMALS_VBO,
			TEX_COORDS_VBO,
			INDICES_IBO,
			VBO_COUNT
		};

	private:

		GLuint  vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
		GLuint  vao_id;                  // Id del VAO del cubo

		GLsizei number_of_vertices;

		float   radius;

	public:

		Polar_Sphere(float radius, int number_of_bands, int number_of_slices);

		~Polar_Sphere() 
		{
			glDeleteVertexArrays(1, &vao_id);
			glDeleteBuffers(VBO_COUNT, vbo_ids);
		}

		void render();

	};


}
