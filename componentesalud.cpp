#include "componentesalud.h"
#include <QPainter>
#include <QRect>

componentesalud::componentesalud()
    : m_currentHP(100),  // por defecto 100 puntos de vida
    m_maxHP(100)
{
}

void componentesalud::setHP(int hp)
{
    m_currentHP = qBound(0, hp, m_maxHP);
}

int componentesalud::currentHP() const
{
    return m_currentHP;
}

int componentesalud::maxHP() const
{
    return m_maxHP;
}

void componentesalud::aplicarDano(int dmg)
{
    m_currentHP = qMax(0, m_currentHP - dmg);
}

void componentesalud::actualizar(float /*dt*/)
{
    // Aquí podrías regenerar vida, temporizadores, etc.
}

// Implementación de dibujar: pinta una barra sobre posSprite
void componentesalud::dibujar(QPainter &painter, const QPoint &posSprite)
{
    // 1) Tamaño de la barra en píxeles
    const int barraAncho  = 64;   // ancho total de la barra
    const int barraAlto   = 8;    // grosor de la barra
    const int margenY     = 10;   // distancia por encima del sprite

    // 2) Calculamos proporción de vida actual
    float fraccionVida = 0.0f;
    if (m_maxHP > 0)
        fraccionVida = float(m_currentHP) / float(m_maxHP);

    // 3) Determinamos rectángulo completo (borde) y rectángulo interior (vida llena)
    //    Centremos la barra horizontalmente sobre posSprite.x().
    int xBarra = posSprite.x() + 0 - (barraAncho / 2) + 32;
    // ‣ “+32” asume que tu sprite mide 64 píxeles de ancho. Ajusta si cambias el tamaño.
    int yBarra = posSprite.y() - margenY;

    QRect rectBorde(xBarra, yBarra, barraAncho, barraAlto);
    QRect rectVida (xBarra + 1, yBarra + 1,
                   int((barraAncho - 2) * fraccionVida),
                   barraAlto - 2);

    // 4) Dibujamos el borde de la barra (color negro/oscuro)
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rectBorde);

    // 5) Dibujamos la parte “vacía” (fondo gris)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(80, 80, 80));
    painter.drawRect(rectBorde.adjusted(1, 1, -1, -1));

    // 6) Dibujamos la parte “llena” (color verde)
    painter.setBrush(QColor(50, 205, 50));  // verde “lime”
    painter.drawRect(rectVida);

    // 7) (Opcional) Si quieres mostrar texto de vida "HP: X/Y" encima:
    /*
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 8));
    QString texto = QString("%1/%2").arg(m_currentHP).arg(m_maxHP);
    painter.drawText(xBarra, yBarra - 2, texto);
    */
}
