#include "sprite.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QPainter>
Sprite::Sprite()
    : m_frameIndex(0)
    , m_timeAccumulator(0.0f)
    , m_secondsPerFrame(1.0f / 12.0f)
    , m_state(SpriteState::Idle)
    , m_drawSize(64, 64)
    , m_pos(0, 0)
{
    // Nada más en el ctor; las animaciones se cargan con loadFrames(...)
}

void Sprite::loadFrames(SpriteState state, const QString &prefix, int count)
{
    // Borro cualquier frame previo para ese estado:
    m_frames[state].clear();
    m_frameIndex = 0;
    m_timeAccumulator = 0.0f;

    // Para calcular la ruta al proyecto:
    QString exeDir = QCoreApplication::applicationDirPath();
    QString projectRoot = QDir(exeDir).absoluteFilePath("../..");

    for (int i = 0; i < count; ++i) {
        QString number = QString("%1").arg(i, 3, 10, QChar('0'));
        QString relPath = prefix + number + ".png";
        QString fullPath = QDir(projectRoot).absoluteFilePath(relPath);

        QPixmap pix(fullPath);
        if (pix.isNull()) {
            qWarning() << "Sprite ❌ NO pudo cargar (";
        } else {
            qDebug() << "Sprite ✅ Cargó (";
        }
        m_frames[state].append(pix);
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
    // Obtengo el vector de frames para el estado actual:
    const QVector<QPixmap> &currentFrames = m_frames.value(m_state);
    if (currentFrames.isEmpty())
        return;

    m_timeAccumulator += dt;
    while (m_timeAccumulator >= m_secondsPerFrame) {
        m_timeAccumulator -= m_secondsPerFrame;
        m_frameIndex = (m_frameIndex + 1) % currentFrames.size();
    }
}

void Sprite::draw(QPainter &painter) const
{
    // Dibujo el frame según el estado actual:
    const QVector<QPixmap> &currentFrames = m_frames.value(m_state);
    if (currentFrames.isEmpty())
        return;

    const QPixmap &orig = currentFrames.at(m_frameIndex);
    if (orig.isNull())
        return;

    QPixmap scaledPix = orig.scaled(
        m_drawSize.width(),
        m_drawSize.height(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    painter.drawPixmap(m_pos.x(), m_pos.y(), scaledPix);
}
