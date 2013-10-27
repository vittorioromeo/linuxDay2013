#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};
constexpr float	raggioPallina{10.f}, velocitàPallina{8.f};

struct Pallina
{
	CircleShape forma;
	Vector2f velocità{-velocitàPallina, -velocitàPallina};

	Pallina(float mX, float mY) 
	{
		forma.setPosition(mX, mY);
		forma.setRadius(raggioPallina);
		forma.setFillColor(Color::Red);
		forma.setOrigin(raggioPallina, raggioPallina);
	}	

	void muovi(Vector2f mVelocità) { forma.move(mVelocità); }
	void update() 
	{ 
		muovi(velocità); 
		
		// Evitiamo che la pallina esca fuori dallo schermo

		// Se sta uscendo a sx, settiamo la velocità 
		// orizzontale ad un valore positivo (verso dx)
		if(sx() < 0) velocità.x = velocitàPallina;

		// Altrimenti, se sta uscendo a dx, settiamo la
		// velocità orizzontale ad un valore negativo (verso sx)
		else if(dx() > lunghezzaFinestra) velocità.x = -velocitàPallina;

		// Applichiamo lo stesso concetto per sopra/sotto
		if(sopra() < 0) velocità.y = velocitàPallina;
		else if(sotto() > altezzaFinestra) velocità.y = -velocitàPallina;
	}

	// Metodi `proprietà` o `getter`
	// per ottenere valori comuni
	float x() { return forma.getPosition().x; }
	float y() { return forma.getPosition().y; }
	float sx() { return x() - forma.getRadius(); }
	float dx() { return x() + forma.getRadius(); }
	float sopra() { return y() - forma.getRadius(); }
	float sotto() { return y() + forma.getRadius(); }
};

int main() 
{
	Pallina pallina{lunghezzaFinestra / 2, altezzaFinestra / 2};

	RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 4"};
	finestra.setFramerateLimit(60);
	
	while(true)
	{
		finestra.clear(Color::Black);

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		pallina.update();

		finestra.draw(pallina.forma);
		finestra.display();
	}	

	return 0;
}