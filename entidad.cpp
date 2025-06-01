#include "entidad.h"
#include <QDebug>

entidad::entidad()
    : m_transformacion()
    , m_sprite()
    , m_componenteFisico(&m_transformacion)  // ← aquí espera un constructor ComponenteFisico(Transformacion*)
    , m_componenteSalud()
      {

    // 1) Posición inicial de la entidad (por ejemplo 50,50)
    m_transformacion.setPosition(50, 50);

    // 2) Cargar los frames del sprite (asegúrate de que resources.qrc tiene estas rutas)
    //    Por ejemplo, si tienes en resources.qrc:
    //      sprites/char_right_0.png
    //      sprites/char_right_1.png
    //      sprites/char_right_2.png
    //      sprites/char_left_0.png
    //      sprites/char_left_1.png
    //      sprites/char_left_2.png
    m_sprite.loadFrames(":/sprites/char_right_", ":/sprites/char_left_", 3);

    // 3) Sincronizar la posición del Sprite con la de Transformacion
    QPoint posInicial = m_transformacion.getPosition();
    m_sprite.setPosition(posInicial.x(), posInicial.y());

    // 4) Configurar velocidad del componente físico (120 px/s, por ejemplo)
    m_componenteFisico.setSpeed(120.0f);

    // 5) Configurar salud inicial (por ejemplo 100 HP)
    m_componenteSalud.setHP(100);
}

entidad::~entidad()
{
    // Si algún componente tuviera memoria dinámica, liberarla aquí.
    // En este ejemplo, no es necesario porque usamos solo objetos que se destruyen solos.
}

void entidad::actualizar(float dt)
{
    // --------------------------------------------------------
    // 1) Actualizar la física (esto modifica la Transformación interna)
    // --------------------------------------------------------
    m_componenteFisico.actualizar(dt);

    // --------------------------------------------------------
    // 2) Recuperar la nueva posición de Transformación y pasársela al Sprite
    // --------------------------------------------------------
    QPoint nuevaPos = m_transformacion.getPosition();
    m_sprite.setPosition(nuevaPos.x(), nuevaPos.y());

    // --------------------------------------------------------
    // 3) Determinar la dirección del Sprite según la velocidad física
    // --------------------------------------------------------
    float vx = m_componenteFisico.velocity().x();
    if      (vx >  +0.1f) m_sprite.setDirection(Direction::Right);
    else if (vx <  -0.1f) m_sprite.setDirection(Direction::Left);
    else                  m_sprite.setDirection(Direction::Idle);

    // --------------------------------------------------------
    // 4) Avanzar la animación del Sprite (cambia frameInterno, etc.)
    // --------------------------------------------------------
    m_sprite.update();

    // --------------------------------------------------------
    // 5) Actualizar componente de salud (regeneraciones, estados, etc.)
    // --------------------------------------------------------
    m_componenteSalud.actualizar(dt);

    // --------------------------------------------------------
    // 6) Aquí podrías agregar colisiones, IA u otra lógica extra
    // --------------------------------------------------------
}

void entidad::dibujar(QPainter &painter)
{
    // El sprite ya está sincronizado en posición y frame;
    // basta con llamar a draw() para que se pinte.
    m_sprite.draw(painter);

    // Opcional: si quieres dibujar una barra de vida sobre la cabeza:
    // int hpActual = m_componenteSalud.currentHP();
    // int hpMax    = m_componenteSalud.maxHP();
    // if (hpActual > 0 && hpMax > 0) {
    //     float frac = float(hpActual) / float(hpMax);
    //     int anchoBarra = 40;
    //     int altoBarra  = 5;
    //     int anchoHealth = int(anchoBarra * frac);
    //     QPoint pos = m_transformacion.getPosition();
    //     painter.setBrush(Qt::red);
    //     painter.drawRect(pos.x(), pos.y() - 10, anchoBarra, altoBarra);
    //     painter.setBrush(Qt::green);
    //     painter.drawRect(pos.x(), pos.y() - 10, anchoHealth, altoBarra);
    //     painter.setBrush(Qt::NoBrush);
    // }
}
