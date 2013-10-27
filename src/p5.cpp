#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};
constexpr float	raggioPallina{10.f}, velocitàPallina{8.f};

// Aggiungiamo delle costanti per il giocatore
constexpr float lunghezzaGiocatore{60.f}, altezzaGiocatore{20.f}, 
				velocitàGiocatore{6.f};

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

		if(sx() < 0) velocità.x = velocitàPallina;
		else if(dx() > lunghezzaFinestra) velocità.x = -velocitàPallina;

		if(sopra() < 0) velocità.y = velocitàPallina;
		else if(sotto() > altezzaFinestra) velocità.y = -velocitàPallina;
	}

	float x() { return forma.getPosition().x; }
	float y() { return forma.getPosition().y; }
	float sx() { return x() - forma.getRadius(); }
	float dx() { return x() + forma.getRadius(); }
	float sopra() { return y() - forma.getRadius(); }
	float sotto() { return y() + forma.getRadius(); }
};

// Creiamo una classe per il giocatore
// simile a quella per la pallina
struct Giocatore
{
	// RectangleShape è una classe di SFML che
	// definisce una forma rettangolare
	RectangleShape forma;
	Vector2f velocità;

	// Come per la pallina, costruiamo il giocatore
	// con le coordinate iniziali e passiamo i
	// parametri alla RectangleShape
	Giocatore(float mX, float mY) 
	{ 
		forma.setPosition(mX, mY);
		forma.setSize({lunghezzaGiocatore, altezzaGiocatore});
		forma.setFillColor(Color::Red);
		forma.setOrigin(lunghezzaGiocatore / 2.f, altezzaGiocatore / 2.f);
	}

	// Come per la pallina, definiamo il metodo `muovi`
	void muovi(Vector2f mVelocità) { forma.move(mVelocità); }
	void update() 
	{ 
		muovi(velocità); 
		

		// Per muovere il giocatore, controlliamo se
		// l'utente sta pressando la freccia a sx o la
		// freccia a dx: se sì, modifichiamo la velocità

		// Per evitare che il giocatore finisca
		// fuori dalla finestra, applichiamo la velocità
		// soltanto se la posizione del giocatore è all'interno
		// della finestra 
		if(Keyboard::isKeyPressed(Keyboard::Key::Left) && 
			sx() > 0) velocità.x = -velocitàGiocatore;
		else if(Keyboard::isKeyPressed(Keyboard::Key::Right) && 
				dx() < lunghezzaFinestra) velocità.x = velocitàGiocatore;

		// Se l'utente non sta pressando niente
		// settiamo la velocità a 0
		else velocità.x = 0;
	}	

	float x() { return forma.getPosition().x; }
	float y() { return forma.getPosition().y; }
	float sx() { return x() - forma.getSize().x / 2.f; }
	float dx() { return x() + forma.getSize().x / 2.f; }
	float sopra() { return y() - forma.getSize().y / 2.f; }
	float sotto() { return y() + forma.getSize().y / 2.f; }
};

int main() 
{
	Pallina pallina{lunghezzaFinestra / 2, altezzaFinestra / 2};

	// Definiamo un'istanza del giocatore
	Giocatore giocatore{lunghezzaFinestra / 2, altezzaFinestra - 50};

	RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 5"};
	finestra.setFramerateLimit(60);
	
	while(true)
	{
		finestra.clear(Color::Black);

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		pallina.update();

		// Aggiorniamo il giocatore nel game loop...
		giocatore.update();

		finestra.draw(pallina.forma);

		// ...e disegnamo la sua forma sulla finestra
 		finestra.draw(giocatore.forma);
		finestra.display();
	}	

	return 0;
}