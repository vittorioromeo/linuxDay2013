#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};
constexpr float raggioPallina{10.f};

// Creiamo una classe per la pallina
// `struct` == `class` in C++
struct Pallina
{
    // CircleShape è una classe di SFML
    // che definisce un cerchio
    CircleShape forma;

    // Costruttore della pallina
    // parametro mX = coordinata x iniziale
    // parametro mY = coordinata y iniziale
    Pallina(float mX, float mY)
    {
        // Applichiamo posizione, raggio, colore e origine
        // alla CircleShape `forma`
        forma.setPosition(mX, mY);
        forma.setRadius(raggioPallina);
        forma.setFillColor(Color::Red);
        forma.setOrigin(raggioPallina, raggioPallina);
    }
};

int main()
{
    // Definiamo un'istanza di Pallina al centro
    // della finestra
    Pallina pallina{lunghezzaFinestra / 2, altezzaFinestra / 2};

    RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 2"};
    finestra.setFramerateLimit(60);

    while(true)
    {
        finestra.clear(Color::Black);

        if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

        // Disegnamo la forma della pallina sulla finestra
        finestra.draw(pallina.forma);
        finestra.display();
    }

    return 0;
}
