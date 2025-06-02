#ifndef SPRITE_H
#define SPRITE_H

#include <QPixmap>
#include <QVector>
#include <QPainter>
#include <QString>
#include <QPoint>


enum class SpriteState {
    Idle,
    Walking,
    Attacking,
   // Faltan mas estados (Este metodo funciona para varios Sprites para todos, faltan un par de modificaciones con las rutas)
};

class Sprite
{
public:
    Sprite();

    // Fija la posición donde se dibujará
    void setPosition(int x, int y);

    // Ajusta el tamaño al que queremos escalar el sprite
    void setSize(int ancho, int alto);

    // Llama a este método cada frame para avanzar la animación
    void update(float dt);

    // Dibuja el frame actual en la posición interna (m_pos)
    void draw(QPainter &painter) const;

    void loadFrames(const QString &prefix, int count);
    \

    void setState(SpriteState s);
    void setFPS(int framesPerSecond);
    QSize getSize() const { return m_drawSize; }
     QPoint getPosition() const { return m_pos; }

private:
    QVector<QPixmap> m_walkingFrames;
    QVector<QPixmap> m_idleFrames;    // frames originales (sin escalar)
    int               m_frameIndex; // índice actual en la animación
    QPoint            m_pos;        // posición X,Y donde se dibuja (será la esquina superior del sprite)
    float             m_timeAccumulator; // acumulador de tiempo para avanzar frames
    float             m_secondsPerFrame; // 1 / FPS

    SpriteState       m_state;      // estado de animación (Idle, Walking, etc.)

    QSize             m_drawSize;   // tamañ // estado actual si manejas varias animaciones
};

#endif // SPRITE_H
