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
#include <math.h>


class Transform{
    
private:
    float m11,m13,m22,m23;
    Matriz* t = NULL;
    Matriz* w = new Matriz(3,3);
    
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
    
    void calculate_draw_and_click_matrix(Ponto* vpmin, Ponto* vpmax, Ponto* wmin, Ponto* wmax){ //to be substituted
        //it has movemnet and scale
        m11 = (vpmax->getX() - vpmin->getX()) / (wmax->getX() - wmin->getX());
        m22 = (vpmax->getY() - vpmin->getY()) / (wmax->getY() - wmin->getY());
        m13 = -wmin->getX() * (m11) + vpmin->getX();
        m23 = -wmin->getY() * (m22) + vpmin->getY();
    }
    
    Matriz* set_2D_rotation_matrix(int degrees){
        Matriz* m = new Matriz(3,3);
        float rad = (degrees * M_PI)/180;        
        m->set(0,0,cosf(rad));
        m->set(0,1,-sinf(rad));
        m->set(1,0,sinf(rad));
        m->set(1,1,cosf(rad));
        m->set(2,2,1);
        return m;
    }
    
    Ponto* rotationT2D(int degrees, Ponto* p, Ponto* c){
        float rad = (degrees * M_PI)/180;
        //test purposes only
        float x = p->getX() - c->getX();
        float y = p->getY() - c->getY();
        Ponto* r = new Ponto(x * cosf(rad) - y * sinf(rad), x * sinf(rad) + y* cosf(rad));
        r->move_to(r->getX() + c->getX(), r->getY() + c->getY());
        return r;
    }
    
    Matriz* set_2D_move_matrix(float Dx, float Dy){
        Matriz* m = new Matriz(3,3);
        m->set(0,0,1);
        m->set(1,1,1);
        m->set(2,0,Dx);
        m->set(2,1,Dy);
        m->set(2,2,1);
        return m;
    }
    
    Matriz* set_2D_scale_matrix(float scaleX,float scaleY){
        Matriz* m = new Matriz(3,3);
        m->set(0,0,scaleX);
        m->set(1,1,scaleY);
        m->set(2,2,1);
        return m;
    }
    
    void setT(Matriz* m){
        t = m;
    }
    
    void concatenate_matrix(Matriz* m){
        t = t->multiply(m);
    }
    
    Ponto* transform(Ponto* p){
        Matriz* m1 = new Matriz(1,3);
        m1->set(0,0,p->getX());
        m1->set(0,1,p->getY());
        m1->set(0,2,p->getW());
        
        Matriz* r = m1->multiply(t);
        Ponto* t = new Ponto(r->get(0,0), r->get(0,1), r->get(0,2));
        return t;
    }
    
    Ponto* dT(Ponto* p){
        
        return new Ponto(p->getX() * m11 + m13 , p->getY() * m22 + m23 );
    }
    
    Ponto* cT(Ponto* p){
        return new Ponto((p->getX()- m13 ) / m11, (p->getY() - m23 ) / m22 );
    }
    
    
};

#endif /* TRANSFORM_HPP */

