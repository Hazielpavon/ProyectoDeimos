#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menuopciones.h"
#include "pantallainicio.h"
#include "PantallaCarga.h"
#include <QPainter>
#include <QDebug>
#include "videointro.h"

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
    // Tamaño fijo para el área de juego
    setFixedSize(950, 650);
     ui->setupUi(this);

    connect(pantallaCarga, &PantallaCarga::cargaCompletada, this, [=]() {

        // 1) Creamos la entidad/jugador:
        m_player = new entidad();

        // Cargamos Walking (24 frames)

         m_player->sprite().loadFrames(SpriteState::Walking, "Sprites/PersonajePrincipal/PNG Sequences/Walking/0_Blood_Demon_Walking_",24);
        // Cargar animación Idle:
         m_player->sprite().loadFrames(SpriteState::Idle,"Sprites/PersonajePrincipal/PNG Sequences/Idle/0_Blood_Demon_Idle_", 16);

         m_player->sprite().loadFrames(SpriteState::IdleLeft,"Sprites/PersonajePrincipal/PNG Sequences/Idle Left/0_Blood_Demon_IdleL_", 16);

         m_player->sprite().loadFrames(SpriteState::WalkingLeft,"Sprites/PersonajePrincipal/PNG Sequences/Walking Left/0_Blood_Demon_WalkingL_", 24);


         m_player->sprite().setSize(128, 128);

        // Posiciónalo en el centro
        int spriteW = 128;
        int spriteH = 128;
        int x = (width() - spriteW) / 2;
        int y = (height() - spriteH) / 2;
        m_player->sprite().setPosition(x, y);
        m_player->sprite().setState(SpriteState::Idle);
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &MainWindow::onGameLoop);
        m_timer->start(int(m_dt * 1000));
        setFocusPolicy(Qt::StrongFocus);
        setFocus();
        QWidget *temp = new QWidget(this);
        setCentralWidget(temp);
        temp->show();
        delete temp;
    });
    setFocusPolicy(Qt::NoFocus);
    pantallaInicio = new PantallaInicio(this);
    connect(pantallaInicio, &PantallaInicio::iniciarJuegoPresionado, this, [=]() {
        qDebug() << "✅ Cambiando a MenuOpciones";
        if (!menuOpciones) {
            menuOpciones = new MenuOpciones(this);
            connect(menuOpciones, &MenuOpciones::nuevaPartida, this, [=]() {
                qDebug() << "🕹️ NUEVA PARTIDA presionada";
                pantallaCarga = new PantallaCarga(this);
                connect(pantallaCarga, &PantallaCarga::cargaCompletada, this, [=]() {
                    qDebug() << "✅ Carga finalizada. Iniciando la partida real.";
                    pantallaCarga->close();
                    m_player = new entidad();
                    m_player->transform().setPosition(width()/2 - 32, height()/2 - 32);
                    m_timer = new QTimer(this);
                    connect(m_timer, &QTimer::timeout, this, &MainWindow::onGameLoop);
                    m_timer->start(int(m_dt * 1000));
                    setFocusPolicy(Qt::StrongFocus);
                    setFocus();
                    QWidget *temp = new QWidget(this);
                    setCentralWidget(temp);
                    temp->show();
                    delete temp;

                });
                mostrarPantalla(pantallaCarga);
            });
        }
        mostrarPantalla(menuOpciones);
    });
    mostrarPantalla(pantallaInicio);
}
MainWindow::~MainWindow() {
    if (m_timer) {
        m_timer->stop();
        delete m_timer;
    }
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

void MainWindow::onGameLoop()
{
    if (!m_player)
        return;
    processInput();
    m_player->actualizar(m_dt);
    update();
}

void MainWindow::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(200, 200, 200));

    if (m_player) {
        m_player->sprite().draw(painter);
        QPoint posSprite = m_player->sprite().getPosition();
        m_player->salud().dibujar(painter, posSprite);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
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
    if (!m_player) return;

    const float moveSpeed = 160.0f;
    float vx = 0.0f, vy = 0.0f;

    // 1) Primero, calcula únciamente un botón a la vez:
    if (m_leftPressed && !m_rightPressed) {
        vx = -moveSpeed;
    }
    else if (m_rightPressed && !m_leftPressed) {
        vx = +moveSpeed;
    }
    else {
        vx = 0.0f;
    }

    if (m_upPressed && !m_downPressed) {
        vy = -moveSpeed;
    }
    else if (m_downPressed && !m_upPressed) {
        vy = +moveSpeed;
    }
    else {
        vy = 0.0f;
    }
    m_player->fisica().setVelocity(vx, vy);
    if (vx < 0.0f) {
        m_player->sprite().setState(SpriteState::WalkingLeft);
    }
    else if (vx > 0.0f) {
        m_player->sprite().setState(SpriteState::Walking);
    }
    else {
        if (vy == 0.0f) {
            if (m_player->facingleft()) {
                m_player->sprite().setState(SpriteState::IdleLeft);
            } else {
                m_player->sprite().setState(SpriteState::Idle);
            }
        }
        else {
            m_player->sprite().setState(SpriteState::Walking);
        }
    }
}


