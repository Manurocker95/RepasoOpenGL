/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#pragma once
static const int window_width = 800;							// Ancho de Pantalla
static const int window_height = 600;							// Alto de Pantalla
static const float pi = 3.1415926536f;							// PI
static const float pi_x_2 = 6.2831853072f;						// PI * 2
static const float default_camera_speed = 0.01f;				// Velocidad a la que se mueve la cámara
static const bool postprocessing = false;						// Si estamos aplicando postprocessing o no
static const bool showing_cubes = true; 						// Si renderizamos los cubos 
static const bool showing_sphere = true;						// Si renderizamos la esfera
static const bool cubes_rotating = true;						// Si rotamos los cubos
static const bool sphere_rotating = true;						// Si rotamos la esfera
static const char * default_skybox = "..\\..\\assets\\sky-cube-map-";				// Skybox por defecto
static const char * default_cube_texture = "..\\..\\assets\\uv-checker-3.tga";		// Textura del cubo 1 por defecto
static const char * default_cube2_texture = "..\\..\\assets\\wood-crate-1.tga";		// Textura del cubo 2 por defecto
