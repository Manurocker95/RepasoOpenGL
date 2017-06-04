/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#ifndef OGL_VERTEX_SHADER_HEADER
#define OGL_VERTEX_SHADER_HEADER

    #include "Shader.hpp"

    namespace example
    {

        class Vertex_Shader : public Shader
        {
        public:

            Vertex_Shader(const Source_Code & source_code)
            :
                Shader(source_code, GL_VERTEX_SHADER)
            {
            }

        };

    }

#endif
