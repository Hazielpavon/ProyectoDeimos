#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class PantallaInicio;
class MenuOpciones;
class PantallaCarga;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *pantallaActual;
    PantallaInicio *pantallaInicio;
    MenuOpciones *menuOpciones;
    PantallaCarga *pantallaCarga;

    void mostrarPantalla(QWidget *pantalla);
};

#endif // MAINWINDOW_H
