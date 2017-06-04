/*
> ==========================================
>		Repaso para el examen
>	Hecho por Manuel Rodríguez Matesanz
> ==========================================
*/

#include <cassert>
#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include "View.hpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "Settings.h"

using namespace sf;
using namespace example;

int main ()
{
    // Se crea la ventana de SFML, que es donde se creará el contexto de OpenGL:

    Window window(VideoMode(window_width, window_height), "Repaso para el examen: Cubos rotando, Luces, Niebla, Texturas, Camara, Efectos, Esfera de GLPoints", Style::Default, ContextSettings(32));

    // Una vez se ha creado el contexto de OpenGL ya se puede inicializar Glew:

    GLenum glew_initialization =  glewInit ();

    assert(glew_initialization == GLEW_OK);

    // Glew se inicializa antes de crear view porque view ya usa extensiones de OpenGL:

    View view(800, 600);

    window.setVerticalSyncEnabled (true);

    bool running = true;

    do
    {
        Event event;

        while (window.pollEvent (event))
        {
            switch (event.type)
            {
                case Event::Closed:
                {
                    running = false;
                    break;
                }

                case Event::Resized:
                {
                    Vector2u window_size = window.getSize ();

                    view.resize (window_size.x, window_size.y);

                    break;
                }


				case Event::KeyPressed:
				{
					view.on_key(event.key.code);
					break;
				}

				case Event::MouseButtonPressed:
				{
					view.on_click(event.mouseButton.x, event.mouseButton.y, true);
					break;
				}

				case Event::MouseButtonReleased:
				{
					view.on_click(event.mouseButton.x, event.mouseButton.y, false);
					break;
				}

				case Event::MouseMoved:
				{
					view.on_drag(event.mouseMove.x, event.mouseMove.y);
					break;
				}

            }
        }
	
        view.update ();
        view.render ();

        window.display ();
    }

    while (running);

    return (EXIT_SUCCESS);
}
