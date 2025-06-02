#include "sprite.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QPainter>
#include <QTransform>  // Para transformar y voltear pixmaps

Sprite::Sprite()
    : m_frameIndex(0)
    , m_timeAccumulator(0.0f)
    , m_secondsPerFrame(1.0f / 12.0f)
    , m_state(SpriteState::Idle)
    , m_drawSize(64, 64)
    , m_pos(0, 0)
{
    // Constructor vacío: no cargar nada aquí.
    // Las animaciones se llenan con loadFrames(...) externamente.
}

void Sprite::loadFrames(SpriteState state, const QString &prefix, int count)
{
    // 1) Asegurarnos de vaciar cualquier frame previo para ese estado:
    m_frames[state].clear();
    m_frameIndex = 0;
    m_timeAccumulator = 0.0f;

    // 2) Comprobar si prefix apunta a recurso embebido
    const bool esRecurso = prefix.startsWith(":/");

    // 3) Si no es recurso, obtenemos projectRoot basado en la ubicación del ejecutable
    QString exeDir, projectRoot;
    if (!esRecurso) {
        exeDir      = QCoreApplication::applicationDirPath();
        projectRoot = QDir(exeDir).absoluteFilePath("../..");
    }

    // 4) Para cada índice de frame, construimos la ruta y cargamos el QPixmap
    for (int i = 0; i < count; ++i) {
        // Convertimos i en "000", "001", "002", ...
        QString number = QString("%1").arg(i, 3, 10, QChar('0'));

        QString path;
        if (esRecurso) {
            // Si es recurso, simplemente concatenamos:
            //    prefix = ":/resources/0_Blood_Demon_Idle_"
            // → path = ":/resources/0_Blood_Demon_Idle_000.png"
            path = prefix + number + ".png";
        } else {
            // Modo disco: prefix = "Sprites/.../0_Blood_Demon_Idle_"
            //    relPath = prefix + number + ".png"
            //    fullPath = projectRoot + "/" + relPath
            QString relPath = prefix + number + ".png";
            path = QDir(projectRoot).absoluteFilePath(relPath);
        }

        // Cargamos el pixmap
        QPixmap pix(path);
        if (pix.isNull()) {
            qWarning() << "[loadFrames] NO pudo cargar:" << path;
        } else {
            qDebug() << "[loadFrames] Cargó:" << path << "size" << pix.size();
        }

        // Incluso si pix es nulo, lo añadimos (para preservar el índice)
        m_frames[state].append(pix);
    }
}

void Sprite::generateMirroredFrames(SpriteState srcState, SpriteState dstState)
{
    // 1) Vaciamos cualquier frame previo en dstState
    m_frames[dstState].clear();

    // 2) Si no hay frames cargados en srcState o está vacío, salimos:
    if (!m_frames.contains(srcState) || m_frames[srcState].isEmpty()) {
        qWarning() << "[Sprite] Aviso: no hay frames en srcState"
                   << static_cast<int>(srcState)
                   << "para generarlos espejados.";
        return;
    }

    // 3) Por cada QPixmap en el estado origen, generamos el espejo
    for (const QPixmap &orig : m_frames[srcState]) {
        if (orig.isNull()) {
            // Si el original era inválido, volvemos a añadir un QPixmap nulo
            m_frames[dstState].append(QPixmap());
        } else {
            // Transformación que voltea horizontalmente
            QPixmap flipped = orig.transformed(QTransform().scale(-1, 1));
            m_frames[dstState].append(flipped);
        }
    }

    qDebug() << "[Sprite] Generados" << m_frames[dstState].size()
             << "frames espejados de estado" << static_cast<int>(srcState)
             << "hacia" << static_cast<int>(dstState);
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
    // Solo reiniciamos si realmente cambió el estado
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
    // 1) Obtenemos el vector de frames para el estado actual
    const QVector<QPixmap> &currentFrames = m_frames.value(m_state);
    if (currentFrames.isEmpty())
        return;

    // 2) Acumulamos tiempo y avanzamos el índice de frame si superamos secondsPerFrame
    m_timeAccumulator += dt;
    while (m_timeAccumulator >= m_secondsPerFrame) {
        m_timeAccumulator -= m_secondsPerFrame;
        m_frameIndex = (m_frameIndex + 1) % currentFrames.size();
    }
}

void Sprite::draw(QPainter &painter) const
{
    // 1) Obtenemos el vector de frames para el estado actual
    const QVector<QPixmap> &currentFrames = m_frames.value(m_state);
    if (currentFrames.isEmpty())
        return;

    // 2) Tomamos el QPixmap en la posición m_frameIndex
    const QPixmap &orig = currentFrames.at(m_frameIndex);
    if (orig.isNull())
        return;

    // 3) Escalamos al tamaño deseado (m_drawSize), manteniendo aspecto
    QPixmap scaledPix = orig.scaled(
        m_drawSize.width(),
        m_drawSize.height(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );

    // 4) Dibujamos en la posición (m_pos.x(), m_pos.y())
    painter.drawPixmap(m_pos.x(), m_pos.y(), scaledPix);
}
