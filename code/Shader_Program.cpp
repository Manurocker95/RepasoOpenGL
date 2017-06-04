/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#include "Shader_Program.hpp"

namespace example
{

    bool Shader_Program::link ()
    {
        glLinkProgram (program_object_id);

        // Se comprueba si el linkage ha tenido éxito:

        GLint succeeded = GL_FALSE;

        glGetProgramiv (program_object_id, GL_LINK_STATUS, &succeeded);

        if (!succeeded)
        {
            // Se guarda el log del error que ha ocurrido:

            GLint log_length;

            glGetShaderiv (program_object_id, GL_INFO_LOG_LENGTH, &log_length);

            if (log_length > 0)
            {
                log_string.resize  (log_length);

                glGetShaderInfoLog (program_object_id, log_length, NULL, &log_string.front ());
            }

            assert(false);
        }

        return (link_completed = (succeeded != GL_FALSE));
    }

}
