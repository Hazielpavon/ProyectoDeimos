#ifndef COMPONENTESALUD_H
#define COMPONENTESALUD_H

#include <QPainter>
#include <QPoint>

class componentesalud
{
public:
    componentesalud();

    void setHP(int hp);
    int  currentHP() const;
    int  maxHP() const;
    void aplicarDano(int dmg);
    void actualizar(float dt);

    // ← Añadimos esta función pública para poder dibujar la barra de vida.
    //    Recibirá el QPainter y la posición (superior izquierda del sprite).
    void dibujar(QPainter &painter, const QPoint &posSprite);

private:
    int m_currentHP;
    int m_maxHP;
};
#endif // COMPONENTESALUD_H
