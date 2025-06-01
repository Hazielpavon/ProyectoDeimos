#include "componentesalud.h"

componentesalud::componentesalud()
    : m_currentHP(100),
    m_maxHP(100)
{
}

void componentesalud::setHP(int hp)
{
    m_currentHP = hp;
    m_maxHP     = hp;
}

int componentesalud::currentHP() const
{
    return m_currentHP;
}

int componentesalud::maxHP() const
{
    return m_maxHP;
}

void componentesalud::actualizar(float /*dt*/)
{
    // Por ahora no hacemos nada (sin regeneración, sin estados).
}

void componentesalud::aplicarDano(int dmg)
{
    m_currentHP -= dmg;
    if (m_currentHP < 0) m_currentHP = 0;
}
