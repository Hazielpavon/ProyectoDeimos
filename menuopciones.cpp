#include "menuopciones.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QDebug>
#include <QApplication>

MenuOpciones::MenuOpciones(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(950, 650);

    // Imagen de fondo
    fondo = new QLabel(this);
    fondo->setPixmap(QPixmap(":/resources/menu_Op.png"));
    fondo->setScaledContents(true);
    fondo->resize(950, 650);
    fondo->lower();

    // Botón invisible sobre "NUEVA PARTIDA"
    btnNuevaPartida = new QPushButton(this);
    btnNuevaPartida->setGeometry(355, 300, 240, 50); // Ajustado al texto
    btnNuevaPartida->setStyleSheet("background-color: transparent;");

    // Botón invisible sobre "CONTINUAR"
    btnContinuar = new QPushButton(this);
    btnContinuar->setGeometry(355, 350, 240, 50); // Ajustado al texto
    btnContinuar->setStyleSheet("background-color: transparent;");

    // Botón invisible sobre "SALIR DEL JUEGO"
    btnSalir = new QPushButton(this);
    btnSalir->setGeometry(355, 420, 240, 50); // Ajustado al texto
    btnSalir->setStyleSheet("background-color: transparent;");

    // Conexiones
    connect(btnNuevaPartida, &QPushButton::clicked, this, []() {
        qDebug() << "🎮 NUEVA PARTIDA";
    });

    connect(btnContinuar, &QPushButton::clicked, this, []() {
        qDebug() << "⏩ CONTINUAR";
    });

    connect(btnSalir, &QPushButton::clicked, this, []() {
        qDebug() << "🚪 SALIR DEL JUEGO";
        qApp->exit();
    });
}
