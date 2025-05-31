#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pantallainicio.h"
#include "menuopciones.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *pantallaActual;
    PantallaInicio *pantallaInicio;
    MenuOpciones *menuOpciones;

    void mostrarPantalla(QWidget *pantalla);
};

#endif // MAINWINDOW_H
