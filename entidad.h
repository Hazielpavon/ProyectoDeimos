#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QPainter>
#include "tranformacion.h"
#include "sprite.h"
#include "componentefisico.h"
#include "componentesalud.h"

/**
 * @brief La clase entidad agrupa los siguientes componentes:
 *        - Transformacion m_transformacion
 *        - Sprite       m_sprite
 *        - ComponenteFisico  m_componenteFisico
 *        - ComponenteSalud   m_componenteSalud
 *
 *        Métodos públicos:
 *          - entidad()           : constructor
 *          - ~entidad()          : destructor
 *          - actualizar(float)   : llama a la física, sincroniza con el sprite y avanza animación
 *          - dibujar(QPainter&)  : dibuja el sprite en pantalla
 *
 *        Getters para acceder a los componentes:
 *          - transform(), fisica(), salud(), sprite()
 */
class entidad
{
public:
    entidad();
    ~entidad();

    /**
     * @brief actualizar   Actualiza la física, la animación y otros componentes internos.
     * @param dt           Delta‐time en segundos (por ejemplo ~0.016 para 60 FPS).
     */
    void actualizar(float dt);

    /**
     * @brief dibujar      Dibuja el sprite de la entidad en pantalla.
     * @param painter      QPainter válido donde se va a pintar el sprite.
     */
    void dibujar(QPainter &painter);

    // Getters para acceder a los subcomponentes si se necesitara desde fuera:
    tranformacion   &transform()  { return m_transformacion;   }
    componentefisico &fisica()     { return m_componenteFisico;  }
    componentesalud  &salud()      { return m_componenteSalud;   }
    Sprite           &sprite()     { return m_sprite;           }

private:
    tranformacion    m_transformacion;    ///< Posición, rotación y escala
    Sprite            m_sprite;            ///< Controla la animación (frames) del personaje
    componentefisico   m_componenteFisico;   ///< Actualiza posición/velocidad física
    componentesalud    m_componenteSalud;    ///< Maneja puntos de vida y daño
};

#endif // ENTIDAD_H
