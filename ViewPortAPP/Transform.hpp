/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Transform.hpp
 * Author: lucampelli
 *
 * Created on 17 de Março de 2017, 15:53
 */

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Ponto.hpp"


class Transform{
    
private:
    float m11,m13,m22,m23;
    float t11,t12,t13,t21,t22,t23,t31 = 0,t32 = 0,t33 = 1;
    
    Transform(){
        
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
    
    void set_2D_rotation_matrix(int degrees, Ponto* shapePos){ // shapePos should be shape center for all matrixes 
        
    }
    
    void set_2D_move_matrix(float Dx, float Dy, Ponto* shapePos){
        
    }
    
    void set_2D_scale_matrix(float scale, Ponto* shapePos){
        
    }
    
    void concatenate_matrix(){
        //multiply matrixes's effects for linear operations,
        //something like concatenate_matrix(set_2D_move_matrix(), set_2D_scale_matrix());
        //it makes the transform matrix turn into that... or something i dont know....
    }
    
    Ponto* T(Ponto* p){
        return new Ponto(p->getX() * m11 + m13 , p->getY() * m22 + m23 );
    }
    
    Ponto* iT(Ponto* p){
        return new Ponto((p->getX()- m13 ) / m11, (p->getY() - m23 ) / m22 );
    }
    
    
};

#endif /* TRANSFORM_HPP */

