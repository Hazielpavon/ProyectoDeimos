#include "Sprite.h"

Sprite::Sprite()
    : m_frameIndex(0),
    m_direction(Direction::Idle),
    m_pos(0, 0),
    m_speed(4)   // por defecto 4 píxeles/tick; puedes cambiarlo con setSpeed()
{
    // En el constructor no cargamos nada;
    // llamaremos a loadFrames(...) desde el widget principal.
}

void Sprite::loadFrames(const QString &prefixRight,const QString &prefixLeft, int count)
{
    m_framesRight.clear();
    m_framesLeft.clear();

    // Por ejemplo, si prefixRight = ":/sprites/char_right_"
    // y count = 3, cargará ":/sprites/char_right_0.png", ":/sprites/char_right_1.png", ":/sprites/char_right_2.png"
    for (int i = 0; i < count; ++i) {
        QString rightPath = QString("%1%2.png").arg(prefixRight).arg(i);
        QString leftPath  = QString("%1%2.png").arg(prefixLeft).arg(i);

        QPixmap pR(rightPath);
        QPixmap pL(leftPath);

        // Puedes verificar si se cargó correctamente, por ejemplo:
        if (pR.isNull() || pL.isNull()) {
            qWarning("Sprite::loadFrames: no se pudo cargar algún frame en '%s' o '%s'",
                     qPrintable(rightPath), qPrintable(leftPath));
        }

        m_framesRight.append(pR);
        m_framesLeft.append(pL);
    }
}

void Sprite::setPosition(int x, int y)
{
    m_pos.setX(x);
    m_pos.setY(y);
}

QPoint Sprite::position() const
{
    return m_pos;
}

void Sprite::setDirection(Direction d)
{
    m_direction = d;
    // Si pasamos de Idle a Left/Right, dejamos que update() avance frames.
    // Si configuramos Idle, el update() reseteará el frame a 0.
}

Direction Sprite::direction() const
{
    return m_direction;
}

void Sprite::setSpeed(int pixels)
{
    m_speed = pixels;
}

int Sprite::speed() const
{
    return m_speed;
}

void Sprite::update()
{
    // Si está en Idle (quieto), forzamos index a 0 (frame inicial del “quieto”)
    if (m_direction == Direction::Idle) {
        m_frameIndex = 0;
        return;
    }

    // 1) Avanzar el índice de animación
    //    Usamos modulo en caso de que count > 1
    int frameCount = m_framesRight.size();
    if (frameCount == 0)
        return; // nada que hacer si no hay frames

    m_frameIndex = (m_frameIndex + 1) % frameCount;

    // 2) Mover la posición en X
    if (m_direction == Direction::Right) {
        m_pos.setX(m_pos.x() + m_speed);
    }
    else if (m_direction == Direction::Left) {
        m_pos.setX(m_pos.x() - m_speed);
    }
    // No tocamos el eje Y aquí (podrías agregar salto, gravedad, etc. en otra parte)
}

void Sprite::draw(QPainter &painter) const
{
    if (m_framesRight.isEmpty() || m_framesLeft.isEmpty())
        return;

    QPixmap currentPixmap;
    int frameCount = m_framesRight.size();

    if (m_direction == Direction::Right) {
        currentPixmap = m_framesRight[m_frameIndex % frameCount];
    }
    else if (m_direction == Direction::Left) {
        currentPixmap = m_framesLeft[m_frameIndex % frameCount];
    }
    else {
        // Quieto: dibujar el primer frame de la derecha (o podrías elegir otra imagen)
        currentPixmap = m_framesRight[0];
    }

    // Dibuja el pixmap en (x, y)
    painter.drawPixmap(m_pos.x(), m_pos.y(), currentPixmap);
}

void Sprite::resetAnimation()
{
    m_frameIndex = 0;
}

int Sprite::width() const
{
    if (!m_framesRight.isEmpty())
        return m_framesRight.first().width();
    return 0;
}

int Sprite::height() const
{
    if (!m_framesRight.isEmpty())
        return m_framesRight.first().height();
    return 0;
}
