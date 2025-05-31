#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), pantallaActual(nullptr), menuOpciones(nullptr)
{
    setFixedSize(950, 650);

    pantallaInicio = new PantallaInicio(this);

    connect(pantallaInicio, &PantallaInicio::iniciarJuegoPresionado, this, [=]() {
        qDebug() << "✅ Cambiando a MenuOpciones";
        if (!menuOpciones)
            menuOpciones = new MenuOpciones(this);
        mostrarPantalla(menuOpciones);
    });

    mostrarPantalla(pantallaInicio);
}

MainWindow::~MainWindow() {}

void MainWindow::mostrarPantalla(QWidget *pantalla)
{
    if (pantallaActual) pantallaActual->hide();
    pantallaActual = pantalla;
    setCentralWidget(pantallaActual);
    pantallaActual->show();
}


