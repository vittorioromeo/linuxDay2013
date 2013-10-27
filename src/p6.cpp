#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};
constexpr float	raggioPallina{10.f}, velocitàPallina{8.f};
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

struct Giocatore
{
	RectangleShape forma;
	Vector2f velocità;

	Giocatore(float mX, float mY) 
	{ 
		forma.setPosition(mX, mY);
		forma.setSize({lunghezzaGiocatore, altezzaGiocatore});
		forma.setFillColor(Color::Red);
		forma.setOrigin(lunghezzaGiocatore / 2.f, altezzaGiocatore / 2.f);
	}

	void muovi(Vector2f mVelocità) { forma.move(mVelocità); }
	void update() 
	{ 
		muovi(velocità); 
		
		if(Keyboard::isKeyPressed(Keyboard::Key::Left) && 
			sx() > 0) velocità.x = -velocitàGiocatore;
		else if(Keyboard::isKeyPressed(Keyboard::Key::Right) && 
				dx() < lunghezzaFinestra) velocità.x = velocitàGiocatore;
		else velocità.x = 0;
	}	

	float x() { return forma.getPosition().x; }
	float y() { return forma.getPosition().y; }
	float sx() { return x() - forma.getSize().x / 2.f; }
	float dx() { return x() + forma.getSize().x / 2.f; }
	float sopra() { return y() - forma.getSize().y / 2.f; }
	float sotto() { return y() + forma.getSize().y / 2.f; }
};

// Iniziamo a gestire le collisioni: definiamo
// una funzione generica che ci permette di
// verificare se due oggetti collidono
template<class T1, class T2> bool intersezione(T1& mForma1, T2& mForma2)
{
	return 	mForma1.dx() >= mForma2.sx() && 
			mForma1.sx() <= mForma2.dx() &&
			mForma1.sotto() >= mForma2.sopra() && 
			mForma1.sopra() <= mForma2.sotto();
}

// Creiamo anche una funzione che determina
// gli effetti della collisione tra giocatore
// e pallina
void collisioneTra(Giocatore& mGiocatore, Pallina& mPallina)
{
	// Se non c'è intersezione, usciamo dalla funzione
	if(!intersezione(mGiocatore, mPallina)) return;

	// Altrimenti "spingiamo" la pallina verso l'alto
	mPallina.velocità.y = -velocitàPallina;

	// E direzioniamola in base al lato in cui è 
	// stata colpita dal giocatore
	if(mPallina.x() < mGiocatore.x()) mPallina.velocità.x = -velocitàPallina;
	else mPallina.velocità.x = velocitàPallina;
}

int main() 
{
	Pallina pallina{lunghezzaFinestra / 2, altezzaFinestra / 2};
	Giocatore giocatore{lunghezzaFinestra / 2, altezzaFinestra - 50};

	RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 6"};
	finestra.setFramerateLimit(60);
	
	while(true)
	{
		finestra.clear(Color::Black);

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		pallina.update();
		giocatore.update();

		// Non dimentichiamo di verificare la collisione
		// tra pallina e giocatore all'interno del game loop!
		collisioneTra(giocatore, pallina);

		finestra.draw(pallina.forma);
		finestra.draw(giocatore.forma);
		finestra.display();
	}	

	return 0;
}