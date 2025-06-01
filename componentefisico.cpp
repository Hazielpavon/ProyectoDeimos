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
componentefisico::componentefisico(const componentefisico &o)
    : m_transform(o.m_transform)
    , m_velocity(o.m_velocity)
    , m_speed(o.m_speed)
    , m_direction(o.m_direction)
{}

QPointF componentefisico::velocity() const
{
    return m_velocity;
}

// Nuevo método que añadimos:
void componentefisico::setVelocity(float vx, float vy)
{
    m_velocity.setX(vx);
    m_velocity.setY(vy);

    // Puedes actualizar “m_speed” y “m_direction” si los necesitas:
    m_speed = std::hypot(vx, vy);
    if (m_speed > 0.0f) {
        // Calcula el ángulo en grados, si necesitas setDirection
        float angRad = std::atan2(vy, vx);
        m_direction = int(angRad * 180.0f / M_PI);
    }
}

void componentefisico::setDirection(int dir)
{
    m_direction = dir;
    // Si cambias sólo la dirección, tal vez quieras recalcular m_velocity
    // usando la magnitud m_speed y el ángulo. Ejemplo:
    float rad = m_direction * M_PI / 180.0f;
    m_velocity.setX(m_speed * std::cos(rad));
    m_velocity.setY(m_speed * std::sin(rad));
}

void componentefisico::actualizar(float dt)
{
    // Simple ejemplo: mueve la transformación según m_velocity * dt
    if (m_transform) {
        QPointF pos = m_transform->getPosition();
        // (asumo que Transformacion tiene un getPositionF() que devuelve QPointF;
        // si no, usa getPosition() y conviértelo a QPointF)
        pos += m_velocity * dt;
        m_transform->setPosition(pos.x(), pos.y());
    }
}

