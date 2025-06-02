#include "entidad.h"
#include <QDebug>

entidad::entidad()
    : m_transformacion()
    , m_sprite()
    , m_componenteFisico(&m_transformacion)
    , m_componenteSalud()
{
    // 1) Cargar las animaciones en el constructor:
    // El personaje se crea idle
    m_sprite.setState(SpriteState::Idle);

    m_sprite.loadFrames(SpriteState::Idle,"Sprites/PersonajePrincipal/PNG Sequences/Idle/0_Blood_Demon_Idle_", 16);

    m_sprite.loadFrames(SpriteState::Walking,"Sprites/PersonajePrincipal/PNG Sequences/Walking/0_Blood_Demon_Walking_", 24);

    m_sprite.loadFrames(SpriteState::IdleLeft,"Sprites/PersonajePrincipal/PNG Sequences/Idle Left/0_Blood_Demon_IdleL_", 16);

    m_sprite.loadFrames(SpriteState::WalkingLeft,"Sprites/PersonajePrincipal/PNG Sequences/Walking Left/0_Blood_Demon_WalkingL_", 24);
    // 1.3) Ajustar velocidad de fotogramas (opcional):
    m_sprite.setFPS(12);
    // 1.4) Tamaño inicial del sprite
    m_sprite.setSize(128, 128);
    // 1.6) Colocar posición inicial (por ejemplo, en el centro de la pantalla):
    m_transformacion.setPosition(0.0f, 0.0f);
    // Salud
    m_componenteSalud.setHP(100);
}

entidad::~entidad()
{

}

void entidad::actualizar(float dt)
{
    m_componenteFisico.actualizar(dt);
    QPointF posF = m_transformacion.getPosition();
    actualizarAnimacion(dt);
    int spriteW = m_sprite.getSize().width();
    int spriteH = m_sprite.getSize().height();
    int drawX = int(posF.x() - (spriteW * 0.5f));
    int drawY = int(posF.y() - (spriteH * 0.5f));
    m_sprite.setPosition(drawX, drawY);
    m_componenteSalud.actualizar(dt);
}
void entidad::actualizarAnimacion(float dt)
{
    float vx = m_componenteFisico.velocity().x();
    float vy = m_componenteFisico.velocity().y();

    if (qFuzzyCompare(vx, 0.0f) && qFuzzyCompare(vy, 0.0f)) {
        // —> No se mueve en ninguna dirección: elegimos Idle o IdleLeft según último facing
        if (m_facingLeft) {
            m_sprite.setState(SpriteState::IdleLeft);
        } else {
            m_sprite.setState(SpriteState::Idle);
        }
    }
    else if (vx < 0.0f) {
        // —> Se mueve a la izquierda
        m_facingLeft = true;
        m_sprite.setState(SpriteState::WalkingLeft);
    }
    else {
        // —> Se mueve a la derecha o en vertical; lo consideramos “caminando hacia la derecha”
        m_facingLeft = false;
        m_sprite.setState(SpriteState::Walking);
    }

    m_sprite.update(dt);
}

