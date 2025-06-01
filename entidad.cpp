#include "entidad.h"
#include <QDebug>

entidad::entidad()
    : m_transformacion()
    , m_sprite()
    , m_componenteFisico(&m_transformacion)  // Suponiendo que ComponenteFisico requiere puntero a Transformacion
    , m_componenteSalud()
{
    // 1) Cargar las animaciones en el constructor:

    // 1.1) Animación Walking (24 frames):
    //     Prefijo: sin “000.png” al final
    m_sprite.loadWalkingFrames(
        "Sprites/PersonajePrincipal/PNG Sequences/Walking/0_Blood_Demon_Walking_",
        24
        );

    // 1.2) Animación Idle (por ejemplo 16 frames):
    m_sprite.loadIdleFrames(
        "Sprites/PersonajePrincipal/PNG Sequences/Idle/0_Blood_Demon_Idle_",
        16
        );

    // 1.3) Ajustar velocidad de fotogramas (opcional):
    //     Podrías querer Idle a 8 FPS, Walking a 12 FPS.
    //     Sin embargo la implementación que mostramos usa un solo FPS para todo estado.
    m_sprite.setFPS(12);

    // 1.4) Tamaño inicial del sprite (p. ej. 128×128):
    m_sprite.setSize(128, 128);

    // 1.5) Estado inicial a Idle (supongamos que, al nacer la entidad,
    //     no se está moviendo)
    m_sprite.setState(SpriteState::Idle);

    // 1.6) Colocar posición inicial (por ejemplo, en el centro de la pantalla):
    //     Dejamos el valor inicial en (0,0) aquí. Lo ajustará MainWindow más tarde:
    m_transformacion.setPosition(0.0f, 0.0f);

    // 1.7) Configurar el ComponenteSalud (por ejemplo, 100 puntos de vida):
    m_componenteSalud.setHP(100);
}

entidad::~entidad()
{
    // Si alguno de tus componentes requiere limpieza manual, hazla aquí.
    // En este ejemplo, todos los objetos son destruidos automáticamente.
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
        m_sprite.setState(SpriteState::Idle);
    } else {
        m_sprite.setState(SpriteState::Walking);
    }

    m_sprite.update(dt);
}
