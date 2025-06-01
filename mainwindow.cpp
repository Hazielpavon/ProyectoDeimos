#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menuopciones.h"
#include "pantallainicio.h"
#include "PantallaCarga.h"
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    pantallaActual(nullptr),
    pantallaInicio(nullptr),
    menuOpciones(nullptr),
    pantallaCarga(nullptr),
    ui(new Ui::MainWindow),
    m_player(nullptr),
    m_timer(nullptr),
    m_upPressed(false),
    m_downPressed(false),
    m_leftPressed(false),
    m_rightPressed(false)
{
    ui->setupUi(this);

    // Tamaño fijo para el área de juego
    setFixedSize(950, 650);
    connect(pantallaCarga, &PantallaCarga::cargaCompletada, this, [=]() {

        // 1) Creamos la entidad/jugador:
        m_player = new entidad();

        // Cargamos Walking (24 frames)
        m_player->sprite().loadWalkingFrames("Sprites/PersonajePrincipal/PNG Sequences/Walking/0_Blood_Demon_Walking_", 24);

        // Cargamos Idle (por ejemplo 16 frames)
        m_player->sprite().loadIdleFrames("Sprites/PersonajePrincipal/PNG Sequences/Idle/0_Blood_Demon_Idle_",16);


        m_player->sprite().setSize(128, 128);

        // Posiciónalo en el centro
        int spriteW = 128;
        int spriteH = 128;
        int x = (width() - spriteW) / 2;
        int y = (height() - spriteH) / 2;
        m_player->sprite().setPosition(x, y);

        // Estado inicial: Idle
        m_player->sprite().setState(SpriteState::Idle);


        // 5) Iniciamos el timer y damos foco
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &MainWindow::onGameLoop);
        m_timer->start(int(m_dt * 1000));

        setFocusPolicy(Qt::StrongFocus);
        setFocus();

        // 6) Quitamos el widget de carga y dejamos la ventana “vacía”
        QWidget *temp = new QWidget(this);
        setCentralWidget(temp);
        temp->show();
        delete temp;
    });
    // NOTA: No creamos m_player ni arrancamos m_timer aquí.
    //       Esperaremos hasta que el usuario presione “Nueva Partida”
    //       y termine la pantalla de carga.

    setFocusPolicy(Qt::NoFocus);
    // Le quitamos el foco por default:
    // luego, al arrancar la partida, haremos setFocus() para poder leer teclado.

    // Parte de Ana: configuramos la pantalla de inicio y menús
    pantallaInicio = new PantallaInicio(this);

    connect(pantallaInicio, &PantallaInicio::iniciarJuegoPresionado, this, [=]() {
        qDebug() << "✅ Cambiando a MenuOpciones";

        if (!menuOpciones) {
            menuOpciones = new MenuOpciones(this);

            connect(menuOpciones, &MenuOpciones::nuevaPartida, this, [=]() {
                qDebug() << "🕹️ NUEVA PARTIDA presionada";

                // Creamos la pantalla de carga
                pantallaCarga = new PantallaCarga(this);

                connect(pantallaCarga, &PantallaCarga::cargaCompletada, this, [=]() {
                    qDebug() << "✅ Carga finalizada. Iniciando la partida real.";

                    // Cerramos la pantalla de carga
                    pantallaCarga->close();

                    // 1) Creamos la entidad/jugador y la posicionamos
                    m_player = new entidad();
                    m_player->transform().setPosition(width()/2 - 32, height()/2 - 32);
                    // Ajusta -32 si tu sprite no es 64x64

                    // 2) Ahora que el jugador existe, podemos arrancar el timer
                    m_timer = new QTimer(this);
                    connect(m_timer, &QTimer::timeout, this, &MainWindow::onGameLoop);
                    m_timer->start(int(m_dt * 1000)); // 16 ms aprox.

                    // 3) Le damos foco a la ventana para poder leer teclado
                    setFocusPolicy(Qt::StrongFocus);
                    setFocus();

                    // 4) Cambiamos al “modo juego”: quitamos el centralWidget de menús
                    //    y dejamos que paintEvent dibuje el jugador en toda la ventana.
                    //    Para eso, podemos poner el centralWidget a nullptr, o crear
                    //    un widget “vacío” solo para liberar el espacio:
                    QWidget *temp = new QWidget(this);
                    setCentralWidget(temp);
                    temp->show();
                    delete temp;
                    // Si eliminas temp, el centralWidget queda huérfano, pero luego Paint
                    // de MainWindow seguirá dibujando encima. Alternativamente,
                    // simplemente usa setCentralWidget(nullptr) si tu versión lo admite.
                });

                // Mostrar pantalla de carga
                mostrarPantalla(pantallaCarga);
            });

            // Aquí puedes conectar “continuarJuego” y “salirDelJuego” si gustas
        }

        mostrarPantalla(menuOpciones);
    });

    // Al inicio, mostrar la pantalla de inicio
    mostrarPantalla(pantallaInicio);
}

MainWindow::~MainWindow() {
    // Si m_timer está vivo, lo detenemos
    if (m_timer) {
        m_timer->stop();
        delete m_timer;
    }
    // Si existe el jugador, lo borramos
    delete m_player;

    delete ui;
}

void MainWindow::mostrarPantalla(QWidget *pantalla)
{
    if (pantallaActual)
        pantallaActual->hide();

    pantallaActual = pantalla;
    setCentralWidget(pantallaActual);
    pantallaActual->show();
}

// Game loop: solo se llamará una vez que m_player haya sido creado
void MainWindow::onGameLoop()
{
    // Si el jugador no existe aún, no hacemos nada
    if (!m_player)
        return;

    // 1) Procesar input (pone velocidad XY en el ComponenteFisico)
    processInput();

    // 2) Actualizar entidad (move + animación del sprite)
    m_player->actualizar(m_dt);

    // 3) Forzar repintado → paintEvent()
    update();
}

// mainwindow.cpp

void MainWindow::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(200, 200, 200));

    if (m_player) {
        // 1) Dibuja el sprite
        m_player->sprite().draw(painter);

        // 2) Dibuja la barra de vida (directamente en MainWindow):
        //    Supongamos que ComponenteSalud tiene:
        //      void dibujar(QPainter &painter, const QPoint &pos);
        //    y queremos que la barra aparezca en la esquina superior del sprite:
        QPoint posSprite = m_player->sprite().getPosition();
        m_player->salud().dibujar(painter, posSprite);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Solo procesamos teclas si el jugador ya existe
    if (!m_player) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        m_upPressed = true;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        m_downPressed = true;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        m_leftPressed = true;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        m_rightPressed = true;
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // Solo procesamos tecla soltada si el jugador existe
    if (!m_player) {
        QMainWindow::keyReleaseEvent(event);
        return;
    }

    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        m_upPressed = false;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        m_downPressed = false;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        m_leftPressed = false;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        m_rightPressed = false;
        break;
    default:
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::processInput()
{
    // Si el jugador no existe, nada que procesar
    if (!m_player)
        return;

    float vx = 0.0f, vy = 0.0f;
    const float moveSpeed = 120.0f; // px/s

    if (m_upPressed)    vy -= moveSpeed;
    if (m_downPressed)  vy += moveSpeed;
    if (m_leftPressed)  vx -= moveSpeed;
    if (m_rightPressed) vx += moveSpeed;

    // Ajusta ComponenteFisico: ahora sí existe setVelocity(float, float)
    m_player->fisica().setVelocity(vx, vy);

    // Cambiar animación Idle vs Walking según vector
    if (vx == 0.0f && vy == 0.0f) {
        m_player->sprite().setState(SpriteState::Idle);
    } else {
        m_player->sprite().setState(SpriteState::Walking);
    }
}
