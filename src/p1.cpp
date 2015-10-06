#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// `constexpr` definisce una costante
constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};

int main()
{
    // Creazione della finestra
    RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 1"};
    finestra.setFramerateLimit(60);

    // Game loop
    while(true)
    {
        // "Ripuliamo" la finestra
        finestra.clear(Color::Black);

        // Quando "ESC" è pressato, usciamo dal loop
        if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

        // Mostriamo la finestra
        finestra.display();
    }

    return 0;
}