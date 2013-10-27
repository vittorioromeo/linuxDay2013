#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};
constexpr float	raggioPallina{10.f}, velocitàPallina{8.f};
constexpr float lunghezzaGiocatore{60.f}, altezzaGiocatore{20.f}, 
				velocitàGiocatore{6.f};
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

// Sistemando un pò il codice ed astraendo i metodi
// comuni sia al blocco che al giocatore
// il nostro codice finale è di esattamente 170 righe
struct Rettangolo
{
	RectangleShape forma;
	float x() { return forma.getPosition().x; }
	float y() { return forma.getPosition().y; }
	float sx() { return x() - forma.getSize().x / 2.f; }
	float dx() { return x() + forma.getSize().x / 2.f; }
	float sopra() { return y() - forma.getSize().y / 2.f; }
	float sotto() { return y() + forma.getSize().y / 2.f; }
};

struct Giocatore : public Rettangolo
{
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
};

struct Blocco : public Rettangolo
{
	bool distrutto{false};

	Blocco(float mX, float mY) 
	{ 
		forma.setPosition(mX, mY);
		forma.setSize({lunghezzaBlocco, altezzaBlocco});
		forma.setFillColor(Color::Yellow);
		forma.setOrigin(lunghezzaBlocco / 2.f, altezzaBlocco / 2.f);
	}
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

void collisioneTra(Blocco& mBlocco, Pallina& mPallina)
{
	if(!intersezione(mBlocco, mPallina)) return;
	mBlocco.distrutto = true;

	float intersezioneDaSx{mPallina.dx() - mBlocco.sx()};
	float intersezioneDaDx{mBlocco.dx() - mPallina.sx()};
	float intersezioneDaSopra{mPallina.sotto() - mBlocco.sopra()};
	float intersezioneDaSotto{mBlocco.sotto() - mPallina.sopra()};

	bool pallinaDaSx(abs(intersezioneDaSx) < abs(intersezioneDaDx));
	bool pallinaDaSopra(abs(intersezioneDaSopra) < abs(intersezioneDaSotto));

	float minIntersezioneX{pallinaDaSx ? intersezioneDaSx : intersezioneDaDx};
	float minIntersezioneY{pallinaDaSopra ? intersezioneDaSopra : intersezioneDaSotto};

	if(abs(minIntersezioneX) < abs(minIntersezioneY))
		mPallina.velocità.x = pallinaDaSx ? -velocitàPallina : velocitàPallina;
	else
		mPallina.velocità.y = pallinaDaSopra ? -velocitàPallina : velocitàPallina;	
}

int main() 
{
	Pallina pallina{lunghezzaFinestra / 2, altezzaFinestra / 2};
	Giocatore giocatore{lunghezzaFinestra / 2, altezzaFinestra - 50};
	vector<Blocco> blocchi;

	for(int iX{0}; iX < numeroBlocchiX; ++iX)	
		for(int iY{0}; iY < numeroBlocchiY; ++iY)		
			blocchi.emplace_back((iX + 1) * (lunghezzaBlocco + 3) + 22, 
								(iY + 2) * (altezzaBlocco + 3));	

	RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 9"};
	finestra.setFramerateLimit(60);
	
	while(true)
	{
		finestra.clear(Color::Black);

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		pallina.update();
		giocatore.update();
		collisioneTra(giocatore, pallina);
		for(auto& blocco : blocchi) collisioneTra(blocco, pallina);
		blocchi.erase(remove_if(begin(blocchi), end(blocchi), 
			[](const Blocco& mBlocco){ return mBlocco.distrutto; }), 
			end(blocchi));

		finestra.draw(pallina.forma);
		finestra.draw(giocatore.forma);
		for(auto& blocco : blocchi) finestra.draw(blocco.forma);
		finestra.display();
	}	

	return 0;
}

// Adesso implementare ciò che manca è molto semplice!
// Come implementeresti... 
// * 	Un restart dopo la distruzione di tutti i mattoncini?
// * 	Una variabile `punteggio` che viene aumentata ad ogni
//	 	mattoncino distrutto?
// *	Più palline anziché una sola?
// *	Blocchi che richiedono più rimbalzi per essere distrutti?
