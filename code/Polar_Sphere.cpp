/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#include <vector>
#include "Polar_Sphere.hpp"
#include "Settings.h"

namespace example
{

	Polar_Sphere::Polar_Sphere(float radius, int number_of_bands, int number_of_slices)
	:
		radius(radius)
	{
		std::vector< Polar_Coordinate > coordinates;

		float delta = pi / number_of_bands;
		float beta  = delta;

		for (int index = 0, end = number_of_bands - 1; index < end; index++, beta += delta)
		{
			float alpha = 0.f;
			float delta = pi_x_2 / number_of_slices;

			for (int index = 0, end = number_of_slices; index < end; index++, alpha += delta)
			{
				coordinates.push_back ({ alpha, beta });
			}
		}

		// Se generan índices para los VBOs del cubo:

		glGenBuffers(VBO_COUNT, vbo_ids);
		glGenVertexArrays(1, &vao_id);

		// Se activa el VAO del cubo para configurarlo:

		glBindVertexArray(vao_id);

		// Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
		glBufferData(GL_ARRAY_BUFFER, coordinates.size () * sizeof(Polar_Coordinate), coordinates.data (), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		number_of_vertices = coordinates.size();
	}

	void Polar_Sphere::render()
	{
		glBindVertexArray(vao_id);
		//glDrawElements (GL_POINTS, sizeof(indices), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_POINTS, 0, number_of_vertices);
	}

}
