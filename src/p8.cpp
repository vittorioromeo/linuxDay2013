#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int lunghezzaFinestra{800}, altezzaFinestra{600};
constexpr float raggioPallina{10.f}, velocitàPallina{8.f};
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

        if(sx() < 0)
            velocità.x = velocitàPallina;
        else if(dx() > lunghezzaFinestra)
            velocità.x = -velocitàPallina;

        if(sopra() < 0)
            velocità.y = velocitàPallina;
        else if(sotto() > altezzaFinestra)
            velocità.y = -velocitàPallina;
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

        if(Keyboard::isKeyPressed(Keyboard::Key::Left) && sx() > 0)
            velocità.x = -velocitàGiocatore;
        else if(Keyboard::isKeyPressed(Keyboard::Key::Right) &&
                dx() < lunghezzaFinestra)
            velocità.x = velocitàGiocatore;
        else
            velocità.x = 0;
    }

    float x() { return forma.getPosition().x; }
    float y() { return forma.getPosition().y; }
    float sx() { return x() - forma.getSize().x / 2.f; }
    float dx() { return x() + forma.getSize().x / 2.f; }
    float sopra() { return y() - forma.getSize().y / 2.f; }
    float sotto() { return y() + forma.getSize().y / 2.f; }
};

struct Blocco
{
    RectangleShape forma;
    bool distrutto{false};

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

template <class T1, class T2>
bool intersezione(T1& mForma1, T2& mForma2)
{
    return mForma1.dx() >= mForma2.sx() && mForma1.sx() <= mForma2.dx() &&
           mForma1.sotto() >= mForma2.sopra() &&
           mForma1.sopra() <= mForma2.sotto();
}

void collisioneTra(Giocatore& mGiocatore, Pallina& mPallina)
{
    if(!intersezione(mGiocatore, mPallina)) return;

    mPallina.velocità.y = -velocitàPallina;
    if(mPallina.x() < mGiocatore.x())
        mPallina.velocità.x = -velocitàPallina;
    else
        mPallina.velocità.x = velocitàPallina;
}

// Ecco probabilmente la parte più complessa
// del nostro gioco: l'effetto della collisione
// tra un blocco e la pallina
void collisioneTra(Blocco& mBlocco, Pallina& mPallina)
{
    // Se non c'è intersezione, usciamo dalla funzione
    if(!intersezione(mBlocco, mPallina)) return;

    // Altrimenti, segnamo il blocco come distrutto
    mBlocco.distrutto = true;

    // Calcoliamo le varie intersezioni della pallina
    // con il blocco, in base alla direzione
    float intersezioneDaSx{mPallina.dx() - mBlocco.sx()};
    float intersezioneDaDx{mBlocco.dx() - mPallina.sx()};
    float intersezioneDaSopra{mPallina.sotto() - mBlocco.sopra()};
    float intersezioneDaSotto{mBlocco.sotto() - mPallina.sopra()};

    // Se l'intersezione da sx è più piccola di quella da dx
    // allora possiamo affermare che la pallina ha colpito il
    // blocco da sx
    bool pallinaDaSx(abs(intersezioneDaSx) < abs(intersezioneDaDx));

    // Usiamo lo stesso concetto per sopra/sotto
    bool pallinaDaSopra(abs(intersezioneDaSopra) < abs(intersezioneDaSotto));

    // "Conserviamo" l'intersezione più piccola basandoci sulle
    // premesse appena calcolate:
    // se la pallina ha colpito il blocco da sx, allora il valore
    // che ci interessa è l'intersezione da sx
    float minIntersezioneX{pallinaDaSx ? intersezioneDaSx : intersezioneDaDx};
    float minIntersezioneY{
        pallinaDaSopra ? intersezioneDaSopra : intersezioneDaSotto};

    // Se l'intensità (valore assoluto) dell'intersezione orizzontale
    // è minore dell'intensità dell'intersezione verticale
    // possiamo affermare con certezza che la pallina ha colpito il blocco
    // orizzontalmente; altrimenti, la pallina ha colpito il blocco
    // verticalmente

    // In base alle nostre considerazioni modifichiamo la velocità orizzontale
    // o la velocità verticale della pallina, ottenendo un rimbalzo "valido"
    if(abs(minIntersezioneX) < abs(minIntersezioneY))
        mPallina.velocità.x = pallinaDaSx ? -velocitàPallina : velocitàPallina;
    else
        mPallina.velocità.y =
            pallinaDaSopra ? -velocitàPallina : velocitàPallina;
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

    RenderWindow finestra{{lunghezzaFinestra, altezzaFinestra}, "Arkanoid - 8"};
    finestra.setFramerateLimit(60);

    while(true)
    {
        finestra.clear(Color::Black);

        if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

        pallina.update();
        giocatore.update();
        collisioneTra(giocatore, pallina);

        // Utilizziamo un altro for-loop di C++11 per controllare
        // le collisioni tra la pallina ed OGNI blocco
        for(auto& blocco : blocchi) collisioneTra(blocco, pallina);

        // Ed infine, utilizziamo un algoritmo ed un'espressione lambda di C++11
        // per rimuovere dal nostro vector di blocchi i blocchi che sono stati
        // distrutti durante questa iterazione del game loop
        blocchi.erase(remove_if(begin(blocchi), end(blocchi),
                          [](const Blocco& mBlocco)
                          {
                              return mBlocco.distrutto;
                          }),
            end(blocchi));

        finestra.draw(pallina.forma);
        finestra.draw(giocatore.forma);
        for(auto& blocco : blocchi) finestra.draw(blocco.forma);
        finestra.display();
    }

    return 0;
}