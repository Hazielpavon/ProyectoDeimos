#include "componentefisico.h"

componentefisico::componentefisico(tranformacion *t)
    : m_transform(t),
    m_velocity(0, 0),
    m_direction(0),
    m_speed(0.0f)
{}

void componentefisico::setSpeed(float pxPerSecond)
{
    m_speed = pxPerSecond;
}

void componentefisico::setDirection(int dir)
{
    // dir debe ser -1, 0 o +1
    m_direction = dir;
    // Actualizamos directamente la componente X de la velocidad
    m_velocity.setX(m_direction * m_speed);
    // (en este ejemplo no usamos componente Y)
}

QPointF componentefisico::velocity() const
{
    return m_velocity;
}

void componentefisico::actualizar(float dt)
{
    if (!m_transform) return;

    m_velocity.setX(m_direction * m_speed);

    QPoint pos = m_transform->getPosition();
    float dx  = m_velocity.x() * dt;
    pos.setX(pos.x() + int(dx));
    m_transform->setPosition(pos.x(), pos.y());
}
