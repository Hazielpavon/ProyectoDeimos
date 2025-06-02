// entidad.cpp
#include "entidad.h"
#include <QDebug>

entidad::entidad()
    : m_transformacion()
    , m_sprite()
    , m_componenteFisico(&m_transformacion)
    , m_componenteSalud()
    , m_isJumping(false)
    , m_verticalVelocity(0.0f)
    , m_groundY(0.0f)    // no importa el valor aquí; lo volveremos a fijar en startJump()
    , m_facingLeft(false)
{
    // Cargamos las animaciones horizontales (Idle, Walking, IdleLeft, WalkingLeft):
    m_sprite.loadFrames(SpriteState::Walking,":/resources/0_Blood_Demon_Walking_",24);

    // Cargar animación Idle:
    m_sprite.loadFrames(SpriteState::Idle,":/resources/0_Blood_Demon_Idle_",16);

    m_sprite.loadFrames(SpriteState::IdleLeft, ":/resources/0_Blood_Demon_IdleL_",16);

    m_sprite.loadFrames(SpriteState::WalkingLeft,":/resources/0_Blood_Demon_WalkingL_",24);

    // Para salto (JumpStart) hay solo 6:
   m_sprite.loadFrames(SpriteState::Jump, ":/resources/0_Blood_Demon_Jump Loop_",6);

    m_sprite.generateMirroredFrames(SpriteState::Jump,SpriteState::JumpLeft);

    // Configuraciones generales:
    m_sprite.setFPS(12);
    m_sprite.setSize(128, 128);

    // No fijamos aquí m_groundY, lo haremos en startJump()
    m_sprite.setState(SpriteState::Idle);
    m_componenteSalud.setHP(100);
}

entidad::~entidad(){

}

void entidad::startJump()
{
    if (m_isJumping) return; // ya está en el aire → ignoramos doble salto

    // 1) Antes de nada, tomamos m_groundY de la posición actual del transform
    m_groundY = m_transformacion.getPosition().y();

    // 2) Disparamos la velocidad vertical hacia arriba:
    m_verticalVelocity = -JUMP_VELOCITY;
    m_isJumping = true;

    // 3) Cambiamos a la animación de salto (Jump o JumpLeft según facingleft):
    if (m_facingLeft) {
        m_sprite.setState(SpriteState::JumpLeft);
    } else {
        m_sprite.setState(SpriteState::Jump);
    }
}

void entidad::actualizar(float dt)
{
    // Si está en salto, procesamos primero la parte vertical:
    if (m_isJumping) {
        actualizarSalto(dt);
    }

    // Luego actualizamos la física de X (ComponenteFisico usa setVelocity(vx,0))
    m_componenteFisico.actualizar(dt);

    // Ajustamos la posición del sprite basándonos en m_transformacion:
    QPointF posF = m_transformacion.getPosition();
    int spriteW = m_sprite.getSize().width();
    int spriteH = m_sprite.getSize().height();
    int drawX = int(posF.x() - (spriteW * 0.5f));
    int drawY = int(posF.y() - (spriteH * 0.5f));
    m_sprite.setPosition(drawX, drawY);

    // Si no está saltando, actualizamos animación horizontal/idle:
    if (!m_isJumping) {
        actualizarAnimacion(dt);
    } else {
        // Si está saltando, solo progresamos frames de salto:
        m_sprite.update(dt);
    }

    // Salud/otros…
    m_componenteSalud.actualizar(dt);
}

void entidad::actualizarSalto(float dt)
{
    // 1) Subimos o bajamos según la velocidad vertical
    QPointF pos = m_transformacion.getPosition();
    pos.setY(pos.y() + m_verticalVelocity * dt);

    // 2) Aplicamos gravedad a la velocidad vertical
    m_verticalVelocity += GRAVITY * dt;

    // 3) Chequeamos si ya llegó al suelo (pos.y >= m_groundY)
    if (pos.y() >= m_groundY) {
        pos.setY(m_groundY);          // ajustamos justo al nivel del suelo
        m_verticalVelocity = 0.0f;
        m_isJumping = false;

        // Tras aterrizar, regresamos a Idle o IdleLeft segun m_facingLeft
        if (m_facingLeft) {
            m_sprite.setState(SpriteState::IdleLeft);
        } else {
            m_sprite.setState(SpriteState::Idle);
        }
    }

    // 4) Asignamos la nueva posición (X no cambia aquí)
    m_transformacion.setPosition(pos.x(), pos.y());
}

void entidad::actualizarAnimacion(float dt)
{
    float vx = m_componenteFisico.velocity().x();

    if (qFuzzyCompare(vx, 0.0f)) {
        // Idle
        if (m_facingLeft) {
            m_sprite.setState(SpriteState::IdleLeft);
        } else {
            m_sprite.setState(SpriteState::Idle);
        }
    }
    else if (vx < 0.0f) {
        // Caminando a la izquierda
        m_facingLeft = true;
        m_sprite.setState(SpriteState::WalkingLeft);
    }
    else {
        // Caminando a la derecha
        m_facingLeft = false;
        m_sprite.setState(SpriteState::Walking);
    }

    m_sprite.update(dt);
}
