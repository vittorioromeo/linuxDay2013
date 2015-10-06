#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};

// Aggungiamo una costante per la velocità della pallina
constexpr float raggioPallina{10.f}, velocitàPallina{8.f};

struct Pallina
{
    CircleShape forma;

    // Velocità della pallina (vettore bidimensionale)
    Vector2f velocità{-velocitàPallina, -velocitàPallina};

    Pallina(float mX, float mY)
    {
        forma.setPosition(mX, mY);
        forma.setRadius(raggioPallina);
        forma.setFillColor(Color::Red);
        forma.setOrigin(raggioPallina, raggioPallina);
    }

    // Metodo `muovi`: sposta la pallina con
    // il parametro vettore velocità mVelocità
    void muovi(Vector2f mVelocità) { forma.move(mVelocità); }

    // "Aggiorniamo" la pallina muovendola
    // con la sua velocità
    void update() { muovi(velocità); }
};

int main()
{
    Pallina pallina{lunghezzaFinestra / 2, altezzaFinestra / 2};

    RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 3"};
    finestra.setFramerateLimit(60);

    while(true)
    {
        finestra.clear(Color::Black);

        if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

        // Ogni iterazione del game loop
        // aggiorniamo la pallina
        pallina.update();

        finestra.draw(pallina.forma);
        finestra.display();
    }

    return 0;
}