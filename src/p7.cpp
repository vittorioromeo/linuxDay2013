#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};
constexpr float	raggioPallina{10.f}, velocitàPallina{8.f};
constexpr float lunghezzaGiocatore{60.f}, altezzaGiocatore{20.f}, 
				velocitàGiocatore{6.f};

// Aggiungiamo nuove costanti per i mattoncini da distruggere
constexpr float lunghezzaBlocco{60.f}, altezzaBlocco{20.f};
constexpr int numeroBlocchiX{11}, numeroBlocchiY{4};

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

// Creiamo una classe per i mattoncini, `Blocco`
struct Blocco
{
	RectangleShape forma;

	// Questo valore booleano determina se il
	// blocco è stato colpito o no
	bool distrutto{false};

	// Il costruttore è praticamente identico
	// a quello per il giocatore
	Blocco(float mX, float mY) 
	{ 
		forma.setPosition(mX, mY);
		forma.setSize({lunghezzaBlocco, altezzaBlocco});
		forma.setFillColor(Color::Yellow);
		forma.setOrigin(lunghezzaBlocco / 2.f, altezzaBlocco / 2.f);
	}

	float x() { return forma.getPosition().x; }
	float y() { return forma.getPosition().y; }
	float sx() { return x() - forma.getSize().x / 2.f; }
	float dx() { return x() + forma.getSize().x / 2.f; }
	float sopra() { return y() - forma.getSize().y / 2.f; }
	float sotto() { return y() + forma.getSize().y / 2.f; }
};

template<class T1, class T2> bool intersezione(T1& mForma1, T2& mForma2)
{
	return mForma1.dx() >= mForma2.sx() && 
			mForma1.sx() <= mForma2.dx() &&
			mForma1.sotto() >= mForma2.sopra() && 
			mForma1.sopra() <= mForma2.sotto();
}

void collisioneTra(Giocatore& mGiocatore, Pallina& mPallina)
{
	if(!intersezione(mGiocatore, mPallina)) return;

	mPallina.velocità.y = -velocitàPallina;
	if(mPallina.x() < mGiocatore.x()) mPallina.velocità.x = -velocitàPallina;
	else mPallina.velocità.x = velocitàPallina;
}

int main() 
{
	Pallina pallina{lunghezzaFinestra / 2, altezzaFinestra / 2};
	Giocatore giocatore{lunghezzaFinestra / 2, altezzaFinestra - 50};
	
	// Usiamo un `vector`, classe della libreria standard del C++
	// per contenere un numero qualsiasi di istanze di blocchi
	vector<Blocco> blocchi;

	// "Riempiamo" il nostro vector di blocchi con un for loop 
	// bidimensionale, posizionando i blocchi "a griglia"
	for(int iX{0}; iX < numeroBlocchiX; ++iX)	
		for(int iY{0}; iY < numeroBlocchiY; ++iY)		
			blocchi.emplace_back((iX + 1) * (lunghezzaBlocco + 3) + 22, 
								(iY + 2) * (altezzaBlocco + 3));	

	RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 7"};
	finestra.setFramerateLimit(60);
	
	while(true)
	{
		finestra.clear(Color::Black);

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		pallina.update();
		giocatore.update();
		collisioneTra(giocatore, pallina);

		finestra.draw(pallina.forma);
		finestra.draw(giocatore.forma);

		// Bisogna disegnare sulla finestra ogni singolo blocco!
		// Utilizziamo un for-loop moderno di C++11, che ci permette
		// di definire intuitivamente: 
		// "per ogni blocco `blocco` della collezione blocchi, 
		// disegna `blocco.forma`"
		for(auto& blocco : blocchi) finestra.draw(blocco.forma);
		finestra.display();
	}	

	return 0;
}