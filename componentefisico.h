#ifndef COMPONENTEFISICO_H
#define COMPONENTEFISICO_H
#include "tranformacion.h"
class componentefisico{

public:
    explicit componentefisico(tranformacion *t);
    componentefisico(const componentefisico &o);
    void setSpeed(float pxPerSecond);
    void actualizar(float dt);
    QPointF velocity() const;
     void setDirection(int dir);
    // … otros métodos que necesites
private:
    tranformacion *m_transform;
    QPointF         m_velocity;
    float           m_speed;
    int             m_direction;
    // … demás variables internas
};


#endif // COMPONENTEFISICO_H
