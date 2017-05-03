/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Transform.hpp
 * Author: Grupo10 - Luca Campelli e Jonatan Guesser
 *
 * Created on 17 de Março de 2017, 15:53
 */

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Ponto.hpp"
#include "Matriz.hpp"
#include "ListaEnc.hpp"
#include <math.h>


class Transform{
    
private:
    double m11,m13,m22,m23;
    Matriz* dc = new Matriz(3,3);
    Matriz* idc = new Matriz(3,3);
    Matriz* t = NULL;
    Matriz* t3D = NULL;
    Matriz* w = new Matriz(3,3);
    Ponto* camPos;
    Ponto* wmin, *wmax, *vpmin, *vpmax;
    
    
    Transform(){
        w->set(0,0,1);
        w->set(1,1,1);
        w->set(2,2,1);
    }
    
public:    
    static Transform* getInstance(){
        static Transform* t;
        if(!t){
            t = new Transform();
        }
        return t;
    }
    
    void calculate_viewport_transform(Ponto* vpmin, Ponto* vpmax, Ponto* wmin, Ponto* wmax, Ponto* pos){

        camPos = pos;
        
        this->vpmin = vpmin;
        this->vpmax = vpmax;
        this->wmin = wmin;
        this->wmax = wmax;
        
    }
    
    Matriz* set_2D_rotation_matrix(int degrees){
        Matriz* m = new Matriz(3,3);
        double rad = (degrees * M_PI)/180;        
        m->set(0,0,cosf(rad));
        m->set(0,1,-sinf(rad));
        m->set(1,0,sinf(rad));
        m->set(1,1,cosf(rad));
        m->set(2,2,1);
        return m;
    }
    
    Ponto* rotationT2D(int degrees, Ponto* p, Ponto* c){
        double rad = (degrees * M_PI)/180;
        //test purposes only
        double x = p->getX() - c->getX();
        double y = p->getY() - c->getY();
        Ponto* r = new Ponto(x * cosf(rad) - y * sinf(rad), x * sinf(rad) + y* cosf(rad));
        r->move_to(r->getX() + c->getX(), r->getY() + c->getY());
        return r;
    }
    
    Matriz* set_2D_move_matrix(double Dx, double Dy){
        Matriz* m = new Matriz(3,3);
        m->set(0,0,1);
        m->set(1,1,1);
        m->set(2,0,Dx);
        m->set(2,1,Dy);
        m->set(2,2,1);
        return m;
    }
    
    Matriz* set_2D_scale_matrix(double scaleX,double scaleY){
        Matriz* m = new Matriz(3,3);
        m->set(0,0,scaleX);
        m->set(1,1,scaleY);
        m->set(2,2,1);
        return m;
    }
    
    Matriz* set_3Dx_rotation_matrix(int degrees){
        Matriz* m = new Matriz(4,4);
        double rad = (degrees * M_PI)/180; 
        m->set(0,0,1);
        m->set(1,1,cosf(rad));
        m->set(2,1,sinf(rad));
        m->set(1,2,-sinf(rad));
        m->set(2,2,cosf(rad));
        m->set(3,3,1);
        return m;
    }
    
    Matriz* set_3Dy_rotation_matrix(int degrees){
        Matriz* m = new Matriz(4,4);
        double rad = (degrees * M_PI)/180;
        m->set(1,1,1);
        m->set(0,0,cosf(rad));
        m->set(0,2,-sinf(rad));
        m->set(2,0,sinf(rad));
        m->set(2,2,cosf(rad));
        m->set(3,3,1);
        return m;
    }
    
    Matriz* set_3Dz_rotation_matrix(int degrees){
        Matriz* m = new Matriz(4,4);
        double rad = (degrees * M_PI)/180;        
        m->set(0,0,cosf(rad));
        m->set(0,1,-sinf(rad));
        m->set(1,0,sinf(rad));
        m->set(1,1,cosf(rad));
        m->set(2,2,1);
        m->set(3,3,1);
        return m;
    }
    
    Matriz* set_3D_move_matrix(double Dx, double Dy, double Dz){
        Matriz* m = new Matriz(4,4);
        m->set(0,0,1);
        m->set(1,1,1);
        m->set(2,2,1);
        m->set(3,0,Dx);
        m->set(3,1,Dy);
        m->set(3,2,Dz);
        m->set(3,3,1);
        return m;
    }
    
    Matriz* set_3D_scale_matrix(double scaleX,double scaleY, double scaleZ){
        Matriz* m = new Matriz(4,4);
        m->set(0,0,scaleX);
        m->set(1,1,scaleY);
        m->set(2,2,scaleZ);
        m->set(3,3,1);
        return m;
    }
    
    float findAngle(Ponto* x, Ponto* y){
        float divs = (x->getX() * y->getX() + x->getY() * y->getY() + x->getZ() * y->getZ());
        float divd1 = sqrtf(x->getX()) + sqrtf(x->getY()) + sqrtf(x->getZ());
        float divd2 = sqrtf(y->getX()) + sqrtf(y->getY()) + sqrtf(y->getZ());
        float divd = divd1 * divd2;
        
        return acosf(divs/divd);
    }
    
    void setT(Matriz* m){
        t = m;
    }
    
    void setT3D(Matriz* m){
        t3D = m;
    }
    
    ListaEnc<Ponto*>* getWindowStuff(){
        ListaEnc<Ponto*>* l = new ListaEnc<Ponto*>();
        l->adiciona(wmin);
        l->adiciona(wmax);
        return l;
    }
    
    void concatenate_matrix(Matriz* m){
        t = t->multiply(m);
    }
    
    void concatenate_matrix_3D(Matriz* m){
        t3D = t3D->multiply(m);
    }
    
    Ponto* transform(Ponto* p){
        Matriz* m1 = new Matriz(1,3);
        m1->set(0,0,p->getX());
        m1->set(0,1,p->getY());
        m1->set(0,2,p->getZ());
        
        Matriz* r = m1->multiply(t);
        Ponto* t = new Ponto(r->get(0,0), r->get(0,1), r->get(0,2));
        
        return t;
    }
    
    Ponto* transform3D(Ponto* p){
        Matriz* m1 = new Matriz(1,4);
        m1->set(0,0,p->getX());
        m1->set(0,1,p->getY());
        m1->set(0,2,p->getZ());
        m1->set(0,3,p->getW());
        
        Matriz* r = m1->multiply(t3D);
        Ponto* t = new Ponto(r->get(0,0), r->get(0,1), r->get(0,2));
        
        return t;
    }
    
    Ponto* dT(Ponto* p){
        
        double xvp, yvp;
        xvp = ((p->getX() - wmin->getX())/(wmax->getX() -wmin->getX())) * (vpmax->getX() - vpmin->getX());
        yvp = (1 - ((p->getY() - wmin->getY())/(wmax->getY() -wmin->getY()))) * (vpmax->getY() - vpmin->getY());
        return new Ponto(xvp,yvp);
        
    }
    
    Ponto* cT(Ponto* p, double zoom, double rot){
        
        double xw, yw;
        
        xw = ((p->getX() / (vpmax->getX() - vpmin->getX())) * (wmax->getX() - wmin->getX())) + (wmin->getX());
        yw = ((1 - (p->getY()/ (vpmax->getY() - vpmin->getY()))) * (wmax->getY() - wmin->getY())) + wmin->getY();
        
        setT(set_2D_rotation_matrix(rot));
        
        Ponto* r = new Ponto(xw,yw);
        
        return (transform(r));
    }
    
};

#endif /* TRANSFORM_HPP */

