#include "mainwindow.h"
#include "menuopciones.h"
#include "pantallainicio.h"
#include "PantallaCarga.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    pantallaActual(nullptr),
    pantallaInicio(nullptr),
    menuOpciones(nullptr),
    pantallaCarga(nullptr)
{
    setFixedSize(950, 650);

    pantallaInicio = new PantallaInicio(this);

    connect(pantallaInicio, &PantallaInicio::iniciarJuegoPresionado, this, [=]() {
        qDebug() << "✅ Cambiando a MenuOpciones";

        if (!menuOpciones) {
            menuOpciones = new MenuOpciones(this);

            connect(menuOpciones, &MenuOpciones::nuevaPartida, this, [=]() {
                qDebug() << "🕹️ NUEVA PARTIDA presionada";

                pantallaCarga = new PantallaCarga(this);

                connect(pantallaCarga, &PantallaCarga::cargaCompletada, this, [=]() {
                    qDebug() << "✅ Carga finalizada (por ahora no se carga el nivel)";
                    pantallaCarga->close(); // Solo cerramos por ahora
                });

                mostrarPantalla(pantallaCarga);
            });

            // Aquí puedes conectar continuarJuego y salirDelJuego más adelante
        }

        mostrarPantalla(menuOpciones);
    });

    mostrarPantalla(pantallaInicio);
}

MainWindow::~MainWindow() {}

void MainWindow::mostrarPantalla(QWidget *pantalla)
{
    if (pantallaActual)
        pantallaActual->hide();

    pantallaActual = pantalla;
    setCentralWidget(pantallaActual);
    pantallaActual->show();
}
