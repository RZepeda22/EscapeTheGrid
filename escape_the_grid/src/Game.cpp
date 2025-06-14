#include "Game.hpp" // Incluimos la definición de nuestra clase Game

// Constructor: Se llama automáticamente cuando se crea un objeto Game.
// Aquí inicializamos los miembros de la clase.
Game::Game() :
    mWindow(sf::VideoMode({200, 200}), "SFML works!"), // Inicializamos la ventana con tamaño y título.
    mPlayer(100.f) // Inicializamos el círculo (que ahora llamamos mPlayer) con un radio de 100.f.
{
    // Configuramos el color del círculo. Esto se hace en el constructor
    // porque es una configuración inicial que solo necesita hacerse una vez.
    mPlayer.setFillColor(sf::Color::Green);
}

// El método principal que contiene el bucle del juego.
void Game::run()
{
    while (mWindow.isOpen()) // Mientras la ventana esté abierta...
    {
        processEvents(); // Procesamos los eventos de entrada (teclado, ratón, cerrar ventana, etc.).
        update();        // Actualizamos la lógica del juego (movimiento de objetos, colisiones, etc.).
        render();        // Dibujamos los elementos en la ventana.
    }
}

// Procesa los eventos que ocurren en la ventana.
void Game::processEvents()
{
    // Iteramos sobre todos los eventos pendientes en la cola.
    while (const std::optional event = mWindow.pollEvent())
    {
        // Si el evento es de tipo sf::Event::Closed (el usuario hace clic en la 'X' para cerrar la ventana).
        if (event->is<sf::Event::Closed>())
        {
            mWindow.close(); // Cerramos la ventana.
        }
        // Aquí podrías añadir más lógica para otros tipos de eventos, como:
        // if (event->is<sf::Event::KeyPressed>()) {
        //     // Lógica para cuando se presiona una tecla.
        // }
    }
}

// Actualiza la lógica del juego. En este ejemplo simple, no hay mucho que actualizar,
// pero aquí iría el código para mover el jugador, actualizar puntuaciones, etc.
void Game::update()
{
    // Por ahora, no hay lógica de actualización específica aquí.
    // Si quisieras que el círculo se moviera o cambiara de tamaño con el tiempo,
    // ese código iría aquí.
}

// Dibuja los elementos en la ventana.
void Game::render()
{
    mWindow.clear();       // Limpia la ventana con el color por defecto (negro).
    mWindow.draw(mPlayer); // Dibuja nuestro círculo (mPlayer) en la ventana.
    mWindow.display();     // Muestra en pantalla todo lo que se ha dibujado.
}