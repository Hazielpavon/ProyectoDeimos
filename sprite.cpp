// sprite.cpp

#include "sprite.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

Sprite::Sprite()
    : m_frameIndex(0)
    , m_timeAccumulator(0.0f)
    , m_secondsPerFrame(1.0f / 12.0f)
    , m_state(SpriteState::Idle)
    , m_drawSize(64, 64)
    , m_pos(0, 0)
{
}

void Sprite::loadFrames(const QString &prefix, int count)
{
    m_idleFrames.clear();
    m_frameIndex = 0;
    m_timeAccumulator = 0.0f;

    QString exeDir = QCoreApplication::applicationDirPath();
    QString projectRoot = QDir(exeDir).absoluteFilePath("../..");

    for (int i = 0; i < count; ++i) {
        QString number = QString("%1").arg(i, 3, 10, QChar('0'));
        // prefix por ejemplo "Sprites/PersonajePrincipal/PNG Sequences/Idle/0_Blood_Demon_Idle_"
        QString relPath = prefix + number + ".png";
        QString fullPath = QDir(projectRoot).absoluteFilePath(relPath);

        QPixmap pix(fullPath);
        if (pix.isNull()) {
            qWarning() << "Idle ❌ NO pudo cargar:" << fullPath;
        } else {
            qDebug() << "Idle ✅ Cargó:" << fullPath << "size" << pix.size();
        }
        m_idleFrames.append(pix);
    }
}

void Sprite::setPosition(int x, int y)
{
    m_pos.setX(x);
    m_pos.setY(y);
}

void Sprite::setSize(int w, int h)
{
    m_drawSize = QSize(w, h);
}

void Sprite::setState(SpriteState newState)
{
    if (m_state != newState) {
        m_state = newState;
        m_frameIndex = 0;
        m_timeAccumulator = 0.0f;
    }
}

void Sprite::setFPS(int framesPerSecond)
{
    if (framesPerSecond > 0) {
        m_secondsPerFrame = 1.0f / float(framesPerSecond);
    }
}

void Sprite::update(float dt)
{
    const QVector<QPixmap> *currentFrames = nullptr;
    if (m_state == SpriteState::Walking) {
        currentFrames = &m_walkingFrames;
    } else {
        currentFrames = &m_idleFrames;
    }

    if (!currentFrames || currentFrames->isEmpty())
        return;

    m_timeAccumulator += dt;
    while (m_timeAccumulator >= m_secondsPerFrame) {
        m_timeAccumulator -= m_secondsPerFrame;
        m_frameIndex = (m_frameIndex + 1) % currentFrames->size();
    }
}

void Sprite::draw(QPainter &painter) const
{
    const QVector<QPixmap> *currentFrames = nullptr;
    if (m_state == SpriteState::Walking) {
        currentFrames = &m_walkingFrames;
    } else {
        currentFrames = &m_idleFrames;
    }

    if (!currentFrames || currentFrames->isEmpty())
        return;

    const QPixmap &orig = currentFrames->at(m_frameIndex);
    if (orig.isNull()) return;

    // Escalamos a m_drawSize
    QPixmap scaledPix = orig.scaled(
        m_drawSize.width(),
        m_drawSize.height(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    painter.drawPixmap(m_pos.x(), m_pos.y(), scaledPix);
}
