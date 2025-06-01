#ifndef SPRITE_H
#define SPRITE_H

#include <QPixmap>
#include <QVector>
#include <QPainter>
#include <QString>
#include <QPoint>

enum class Direction {
    Idle = 0,
    Left = -1,
    Right = +1
};

class Sprite
{
public:
    Sprite();


    void loadFrames(const QString &prefixRight,const QString &prefixLeft, int count);

    // Posición en pantalla
    void setPosition(int x, int y);
    QPoint position() const;

    // Dirección: Idle (0), Left (-1) o Right (+1)
    void setDirection(Direction d);
    Direction direction() const;

    // Velocidad en pixeles por “tick”
    void setSpeed(int pixels);
    int speed() const;

    // Llamar periódicamente (por ejemplo, desde un QTimer) para:
    //  1) avanzar el frame de animación
    //  2) desplazar la posición en x si direction != Idle
    void update();

    // Dibuja el frame actual en el QPainter en la posición interna (m_pos).
    void draw(QPainter &painter) const;

    // Para resetear animación a frame inicial (por si quieres forzar “quieto”)
    void resetAnimation();

    // Ancho y alto del sprite (basado en el primer frame Right[0]).
    // Útil si quieres comprobar colisiones o bordes.
    int width() const;
    int height() const;

private:
    QVector<QPixmap> m_framesRight;  // frames de caminar a la derecha
    QVector<QPixmap> m_framesLeft;   // frames de caminar a la izquierda

    int         m_frameIndex;      // índice actual dentro de [0..m_framesRight.size()-1]
    Direction   m_direction;       // Idle, Left o Right
    QPoint      m_pos;             // posición (x,y) actual del sprite
    int         m_speed;           // pixeles por “tick”
};

#endif // SPRITE_H
