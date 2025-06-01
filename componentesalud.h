#ifndef COMPONENTESALUD_H
#define COMPONENTESALUD_H

class componentesalud
{
public:
    componentesalud();
    void setHP(int hp);
    int  currentHP() const;
    int  maxHP() const;
    void aplicarDano(int dmg);
    void actualizar(float dt);
    // …
private:
    int m_currentHP;
    int m_maxHP;
    // … otras variables (regeneración, estados, etc.)
};


#endif // COMPONENTESALUD_H
