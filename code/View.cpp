/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#include "View.hpp"
#include <iostream>
#include <cassert>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
#include "Settings.h"

namespace example
{
    using namespace std;

	const GLuint View::indices_triangle[] =
	{
		1,  0,  3,              // front
		1,  3,  2
	};

#pragma region Shaders -  Cubos, Esfera, PostPro

	//=====================================================
	// SHADER DE LOS CUBOS. TIENE: LUCES, NIEBLA, TEXTURAS
	//=====================================================

	const string View::vertex_shader_code =

		"#version 330\n"
		""
		"struct Light"
		"{"
		"    vec4 position;"
		"    vec3 color;"
		"};"
		""
		"uniform Light light;"
		"uniform float ambient_intensity;"
		"uniform float diffuse_intensity;"
		""
		"uniform float fog_near;"
		"uniform float fog_far;"
		"uniform vec3  fog_color;"
		""
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		"uniform mat4     normal_matrix;"
		""
		"uniform vec3 material_color;"
		""
		"layout (location = 0) in vec3 vertex_coordinates;"
		"layout (location = 1) in vec3 vertex_normal;"
		"layout (location = 2) in vec2 vertex_texture_uv;"
		""
		"out vec2 texture_uv;"
		"out vec3 front_color;"
		""
		"void main()"
		"{"
		"    vec4  normal   = normal_matrix * vec4(vertex_normal, 1.0);"
		"    vec4  position = model_view_matrix * vec4(vertex_coordinates, 1.0);"
		"    gl_Position = projection_matrix * position;"
		""
		"    vec4  light_direction = light.position - position;"
		"    float light_intensity = diffuse_intensity * max (dot (normalize (normal.xyz), normalize (light_direction.xyz)), 0.0);"
		""
		"	float fog_intensity = 1.0 - clamp((fog_far + vertex_coordinates.z) / (fog_far - fog_near), 0.0, 1.0); "
		""
		"   texture_uv  = vertex_texture_uv;"
		"   front_color = ambient_intensity * material_color + diffuse_intensity * light_intensity * light.color * material_color;"
		"   front_color = mix (front_color, fog_color, fog_intensity);"
		"}";

	const string View::fragment_shader_code =

		"#version 330\n"
		""
		"uniform sampler2D sampler2d;"
		""
		"in  vec3    front_color;"
		"in  vec2 texture_uv;"
		"out vec4 fragment_color;"
		""
		"void main()"
		"{"	 
		// Desplazamos las uvs en el shader, pero deberíamos hacerlas para que vayan desde -1 a 1. La condición es el tamaño
		// Rombos en el fragment shader
		//"fragment_color = abs(texture_uv.s-0.5) + abs(texture_uv.t-0.5) < 0.2 ? vec4(1.0) : vec4(0.0, 0.0, 1.0, 1.0);"
		
		// Circulos en el fragment shader ---> WOWOOWOWOWOWOOWOWOWOWOWO
		//"fragment_color = (((texture_uv.s-0.5) * (texture_uv.s-0.5)) + ((texture_uv.t-0.5) * (texture_uv.t-0.5))) < 0.05 ? vec4(1.0) : vec4(0.0, 0.0, 1.0, 1.0); "
		
		// Meter más fórmulas :D
		//"fragment_color = texture_uv.s + texture_uv.t < 0.5 ? vec4(1.0) : vec4(0.0, 0.0, 1.0, 1.0); "

		// Texturas cargadas
		 "    fragment_color = vec4(front_color, 1.0)*vec4(texture (sampler2d, texture_uv.st).rgb, 1.0);"
		"}";

	//=====================================================
	// SHADER DE LA ESFERA. TIENE SUPER RESPIRACION
	//=====================================================

	const string View::vertex_sphere_breath_shader_code =

		"#version 330\n"
		""
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		""
		"uniform float t;"
		"uniform float k;"
		"uniform float radius;"
		"uniform vec3  material_color;"
		""
		"layout (location = 0) in vec2 vertex_coordinates;"
		""
		"out vec3 front_color;"
		""
		"void main()"
		"{"
		"    float d = radius + cos (vertex_coordinates.x * t) * k + sin (vertex_coordinates.y * t) * k;"
		""
		"	 vec4 position;"
		"	 position.x = d * sin (vertex_coordinates.y) * cos (vertex_coordinates.x);"
		"	 position.y = d * sin (vertex_coordinates.y) * sin (vertex_coordinates.x);"
		"	 position.z = d * cos (vertex_coordinates.y);"
		"	 position.w = 1.0;"
		""
		"    front_color = material_color;"
		"    gl_Position = projection_matrix * model_view_matrix * position;"
		"}";

	const string View::fragment_sphere_breath_shader_code =

		"#version 330\n"
		""
		"in  vec3    front_color;"
		"out vec4 fragment_color;"
		""
		"void main()"
		"{"
		"    fragment_color = vec4(0.0, 0.0, 1.0, 1.0);"
		"}";

	//=====================================================
	// SHADER DEL EFECTO POSTPRO DE RECORTE
	//=====================================================

	const string View::effect_vertex_shader_code =

		"#version 330\n"
		""
		"layout (location = 0) in vec3 vertex_coordinates;"
		"layout (location = 1) in vec2 vertex_texture_uv;"
		""
		"out vec2 texture_uv;"
		""
		"void main()"
		"{"
		"   gl_Position = vec4(vertex_coordinates, 1.0);"
		"   texture_uv  = vertex_texture_uv;"
		"}";

	const string View::effect_fragment_shader_code =

		"#version 330\n"
		""
		"uniform sampler2D sampler2d;"
		""
		"in  vec2 texture_uv;"
		"out vec4 fragment_color;"
		""
		"void main()"
		"{"
		"    vec3 color = texture (sampler2d, texture_uv.st).rgb;"
		"    float i = (color.r + color.g + color.b) * 0.3333333333;"
		"    fragment_color = vec4(vec3(i, i, i) * vec3(1.0, 0.75, 0.5), 1.0);"
		"}";

#pragma endregion

#pragma region Metodos - Constructor, Update, Render, Resize
    View::View(int width, int height)
    :
        angle(0),					// Ángulo en el que giran los cubos
		angle_sphere(0),			// Ángulo en el que gira la esfera
		width(width),				// Ancho de la pantalla
		height(height)				// Alto de la pantalla
    {
		// Creamos la cámara

		camera = new Camera();
		skybox = new Skybox(default_skybox);

        // Se establece la configuración básica:

        glEnable     (GL_CULL_FACE);			// Activamos el face culling
        glClearColor (0.f, 0.f, 0.f, 1.f);


		// Se crea la textura y se dibuja algo en ella:
		int e;
		build_framebuffer();
		effect_program_id = compile_shaders(effect_vertex_shader_code, effect_fragment_shader_code);
		e = glGetError();

		// Creamos las cajas y les mandamos una textura

		cube = new Cube(default_cube_texture);
		cube2 = new Cube(default_cube2_texture);

        // Se compilan y se activan los shaders del cubo:

        program_id = compile_shaders (vertex_shader_code, fragment_shader_code);

		// Primero usamos el program id del cubo

        glUseProgram (program_id);

		// Establecemos los ID según el shader del cubo:

        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");
            normal_matrix_id = glGetUniformLocation (program_id,     "normal_matrix");
			
		// El shader usa luces, materiales y niebla por lo que tendremos que configurarlas:

        configure_material (program_id);
        configure_light    (program_id);
		configure_fog	   (program_id);

		// Esfera

		sphere = new Polar_Sphere(2.f, 30, 30);

		// Se compilan y se activan los shaders de la esfera:

		program_sphere_id = compile_shaders(vertex_sphere_breath_shader_code, fragment_sphere_breath_shader_code);

		// Usamos el program id del cubo

		glUseProgram(program_sphere_id);

		// Establecemos los ID según el shader de la esfera:

		model2_view_matrix_id = glGetUniformLocation(program_sphere_id, "model_view_matrix");
		projection2_matrix_id = glGetUniformLocation(program_sphere_id, "projection_matrix");
		radius_id = glGetUniformLocation(program_sphere_id, "radius");
		t_id = glGetUniformLocation(program_sphere_id, "t");
		k_id = glGetUniformLocation(program_sphere_id, "k");
		glPointSize(3.f);		//Tamaño de los puntacos

		// Reescalamos la pantalla al tamaño para que las matrices se ajusten a ese tamaño

        resize (width, height);

		// Variables usadas por la cámara

		angle_around_x = angle_delta_x = 0.0;
		angle_around_y = angle_delta_y = 0.0;
		pointer_pressed = false;

		e = glGetError();
    }

    void View::update ()
    {
		if (cubes_rotating)
			angle += 0.5f;
		
		if (sphere_rotating)
			angle_sphere += 0.5f;

		angle_around_x += angle_delta_x;
		angle_around_y += angle_delta_y;

		if (angle_around_x < -1.5)
		{
			angle_around_x = -1.5;
		}
		else
			if (angle_around_x > +1.5)
			{
				angle_around_x = +1.5;
			}

		glm::mat4 camera_rotation;

		camera_rotation = glm::rotate(camera_rotation, glm::degrees(angle_around_y), glm::vec3(0.f, 1.f, 0.f));
		camera_rotation = glm::rotate(camera_rotation, glm::degrees(angle_around_x), glm::vec3(1.f, 0.f, 0.f));

		camera->set_target(0, 0, -1);
		camera->rotate(camera_rotation);
    }

    void View::render ()
    {
		//Postpro
		
		if (postprocessing)
		{
			glViewport(0, 0, framebuffer_width, framebuffer_height);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);     // Se activa el framebuffer de la textura
		}		

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//===============================
		//			CUBOS
		//===============================
		
		glEnable(GL_DEPTH_TEST);			// Activamos la profundidad
		skybox->render(*camera);

		int e = glGetError();

		if (showing_cubes)
		{	
			glUseProgram(program_id);

			// Se rota el cubo y se empuja hacia el fondo:

			glm::mat4 model_view_matrix = camera->get_model_view();
			glm::mat4 projection_matrix = camera->get_projection();

			glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
			glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

			model_view_matrix = glm::translate(model_view_matrix, glm::vec3(0.f, 0.f, -10.f));
			model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(0.f, 5.f, 0.f));

			glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

			glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));

			glUniformMatrix4fv(normal_matrix_id, 1, GL_FALSE, glm::value_ptr(normal_matrix));

			// Se dibuja el cubo:

			cube->render();

			model_view_matrix = glm::translate(model_view_matrix, glm::vec3(5.f, 0.f, 0.f));
			model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(0.f, 5.f, 0.f));

			glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
			normal_matrix = glm::transpose(glm::inverse(model_view_matrix));
			glUniformMatrix4fv(normal_matrix_id, 1, GL_FALSE, glm::value_ptr(normal_matrix));

			cube2->render();
		}
		e = glGetError();
		// ==========================
		//			ESFERA
		// ==========================

		if (showing_sphere)
		{
			glUseProgram(program_sphere_id);

			static float t = 0.f;
			static float k = 0.15f;

			t += 0.01f;

			// Se rota la esfera y se empuja hacia el fondo:

			glm::mat4 model2_view_matrix = camera->get_model_view();

			model2_view_matrix = glm::translate(model2_view_matrix, glm::vec3(0.f, 0.f, -10.f));
			model2_view_matrix = glm::rotate(model2_view_matrix, angle_sphere, glm::vec3(1.f, 2.f, 1.f));

			glUniformMatrix4fv(model2_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model2_view_matrix));

			glUniform1f(t_id, t);
			glUniform1f(k_id, k);
			glUniform1f(radius_id, 2.f);

			// Se dibuja la esfera:
			sphere->render();
		}
		
		// Pintamos lo que tenga el framebuffer de postpro
		if (postprocessing)
			render_framebuffer();

    }

    void View::resize (int width, int height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 1.f, 50.f);
		glm::mat4 projection2_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 50.f);

        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glUniformMatrix4fv(projection2_matrix_id, 1, GL_FALSE, glm::value_ptr(projection2_matrix));
		
		camera->set_ratio(float(width) / height);

        glViewport (0, 0, width, height);
    }

#pragma endregion

#pragma region Metodos de Compilacion y Errores de Shaders
    GLuint View::compile_shaders ()
    {
        GLint succeeded = GL_FALSE;

        // Se crean objetos para los shaders:

        GLuint   vertex_shader_id = glCreateShader (GL_VERTEX_SHADER  );
        GLuint fragment_shader_id = glCreateShader (GL_FRAGMENT_SHADER);

        // Se carga el código de los shaders:

        const char *   vertex_shaders_code[] = {          vertex_shader_code.c_str () };
        const char * fragment_shaders_code[] = {        fragment_shader_code.c_str () };
        const GLint    vertex_shaders_size[] = { (GLint)  vertex_shader_code.size  () };
        const GLint  fragment_shaders_size[] = { (GLint)fragment_shader_code.size  () };

        glShaderSource  (  vertex_shader_id, 1,   vertex_shaders_code,   vertex_shaders_size);
        glShaderSource  (fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Se compilan los shaders:

        glCompileShader (  vertex_shader_id);
        glCompileShader (fragment_shader_id);

        // Se comprueba que si la compilación ha tenido éxito:

        glGetShaderiv   (  vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (  vertex_shader_id);

        glGetShaderiv   (fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (fragment_shader_id);

        // Se crea un objeto para un programa:

        GLuint program_id = glCreateProgram ();

        // Se cargan los shaders compilados en el programa:

        glAttachShader  (program_id,   vertex_shader_id);
        glAttachShader  (program_id, fragment_shader_id);

        // Se linkan los shaders:

        glLinkProgram   (program_id);

        // Se comprueba si el linkage ha tenido éxito:

        glGetProgramiv  (program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error (program_id);

        // Se liberan los shaders compilados una vez se han linkado:

        glDeleteShader (  vertex_shader_id);
        glDeleteShader (fragment_shader_id);

        return (program_id);
    }

	GLuint View::compile_shaders(const std::string & vertex_shader_code, const std::string & fragment_shader_code)
	{
		GLint succeeded = GL_FALSE;

		// Se crean objetos para los shaders:

		GLuint   vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

		// Se carga el código de los shaders:

		const char *   vertex_shaders_code[] = { vertex_shader_code.c_str() };
		const char * fragment_shaders_code[] = { fragment_shader_code.c_str() };
		const GLint    vertex_shaders_size[] = { (int)vertex_shader_code.size() };
		const GLint  fragment_shaders_size[] = { (int)fragment_shader_code.size() };

		glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
		glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

		// Se compilan los shaders:

		glCompileShader(vertex_shader_id);
		glCompileShader(fragment_shader_id);

		// Se comprueba que si la compilación ha tenido éxito:

		glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
		if (!succeeded) show_compilation_error(vertex_shader_id);

		glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
		if (!succeeded) show_compilation_error(fragment_shader_id);

		// Se crea un objeto para un programa:

		GLuint new_program_id = glCreateProgram();

		// Se cargan los shaders compilados en el programa:

		glAttachShader(new_program_id, vertex_shader_id);
		glAttachShader(new_program_id, fragment_shader_id);

		// Se linkan los shaders:

		glLinkProgram(new_program_id);

		// Se comprueba si el linkage ha tenido éxito:

		glGetProgramiv(new_program_id, GL_LINK_STATUS, &succeeded);
		if (!succeeded) show_linkage_error(new_program_id);

		// Se liberan los shaders compilados una vez se han linkado:

		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

		return (new_program_id);
	}

    void View::show_compilation_error (GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetShaderInfoLog (shader_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;

        #ifdef _MSC_VER
            OutputDebugStringA (info_log.c_str ());
        #endif

        assert(false);
    }

    void View::show_linkage_error (GLuint program_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetProgramInfoLog (program_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;

        #ifdef _MSC_VER
            OutputDebugStringA (info_log.c_str ());
        #endif

        assert(false);
    }

#pragma endregion

#pragma region Configuracion de Materiales, Luces y Niebla
    void View::configure_material (GLuint program_id)
    {
        GLint material_color = glGetUniformLocation (program_id, "material_color");

        glUniform3f (material_color, 1.f, 1.f, 1.f);
    }

    void View::configure_light (GLuint program_id)
    {
        GLint light_position    = glGetUniformLocation (program_id, "light.position");
        GLint light_color       = glGetUniformLocation (program_id, "light.color"   );
        GLint ambient_intensity = glGetUniformLocation (program_id, "ambient_intensity");
        GLint diffuse_intensity = glGetUniformLocation (program_id, "diffuse_intensity");

        glUniform4f (light_position, 10.f, 10.f, 10.f, 1.f);
        glUniform3f (light_color, 1.f, 1.f, 1.f);
        glUniform1f (ambient_intensity, 0.2f);
        glUniform1f (diffuse_intensity, 0.8f);
    }

	void View::configure_fog(GLuint program_id)
	{
		// Se configura la niebla:

		GLint fog_near = glGetUniformLocation(program_id, "fog_near");
		GLint fog_far = glGetUniformLocation(program_id, "fog_far");
		GLint fog_color = glGetUniformLocation(program_id, "fog_color");

		glUniform1f(fog_near, 1.f);
		glUniform1f(fog_far, 50.f);
		glUniform3f(fog_color, 1.f, 1.f, 1.f);
	}

#pragma endregion

#pragma region Metodos de FrameBuffer para PostProcesing
	void View::build_framebuffer()
	{
		// Se crea un framebuffer en el que poder renderizar:
		{
			glGenFramebuffers(1, &framebuffer_id);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		}

		// Se crea una textura que será el búfer de color vinculado al framebuffer:
		{
			glGenTextures(1, &out_texture_id);
			glBindTexture(GL_TEXTURE_2D, out_texture_id);

			// El búfer de color tendrá formato RGB:

			glTexImage2D
			(
				GL_TEXTURE_2D,
				0,
				GL_RGB,
				framebuffer_width,
				framebuffer_height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				0
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		// Se crea un Z-Buffer para usarlo en combinación con el framebuffer:
		{
			glGenRenderbuffers(1, &depthbuffer_id);
			glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer_id);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebuffer_width, framebuffer_height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer_id);
		}

		// Se configura el framebuffer:
		{
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, out_texture_id, 0);

			const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;

			glDrawBuffers(1, &draw_buffer);
		}

		// Se comprueba que el framebuffer está listo:

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		// Se desvincula el framebuffer:

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Se crea el quad para hacer el render del framebuffer:

		static const GLfloat triangle_positions[] =
		{

			/*
			
			-1,1		 1,1				
				   0,0
			-1-1         1,-1

			*/

			+1.0f, -1.0f, 0.0f,				
			+1.0f, +1.0f, 0.0f,
			-1.0f, +1.0f, 0.0f
		};

		static const GLfloat triangle_texture_uv[] =
		{
			+1.0f,  0.0f,
			+1.0f, +1.0f,
			0.0f, +1.0f
		};

		int e;

		glGenBuffers(1, &triangle_vbo0);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_positions), triangle_positions, GL_STATIC_DRAW);

		e = glGetError();

		glGenBuffers(1, &triangle_vbo1);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_texture_uv), triangle_texture_uv, GL_STATIC_DRAW);

		e = glGetError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_vbo2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangle), indices_triangle, GL_STATIC_DRAW);

		e = glGetError();

		glBindVertexArray(0);

	}

	void View::render_framebuffer()
	{
		glViewport(0, 0, width, height);

		// Se activa el framebuffer de la ventana:

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(effect_program_id);
		glDisable(GL_DEPTH_TEST);	

		// Se activa la textura del framebuffer:

		glBindTexture(GL_TEXTURE_2D, out_texture_id);

		int e = glGetError();

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		e = glGetError();
		
	}

#pragma endregion

#pragma region Metodos de control de la camara
	void View::on_key(int key_code)
	{
		// Teclado
		switch (key_code)
		{
		case sf::Keyboard::W:
			camera->move(glm::vec3(0, 0, -default_camera_speed));
			break;
		case sf::Keyboard::S:
			camera->move(glm::vec3(0, 0, default_camera_speed));
			break;
		case sf::Keyboard::Up:
			camera->move(glm::vec3(0, 0, -default_camera_speed));
			break;
		case sf::Keyboard::Down:
			camera->move(glm::vec3(0, 0, default_camera_speed));
			break;
		case sf::Keyboard::R:
			camera->reset(60.f, 0.1f, 100.f, 1);
			break;
		}
	}

	void View::on_drag(int pointer_x, int pointer_y)
	{
		if (pointer_pressed)
		{
			angle_delta_x = 0.025f * float(last_pointer_y - pointer_y) / float(height);
			angle_delta_y = 0.025f * float(last_pointer_x - pointer_x) / float(width);
		}
	}

	void View::on_click(int pointer_x, int pointer_y, bool down)
	{
		if ((pointer_pressed = down) == true)
		{
			last_pointer_x = pointer_x;
			last_pointer_y = pointer_y;
		}
		else
		{
			angle_delta_x = angle_delta_y = 0.0;
		}
	}
#pragma endregion

}
