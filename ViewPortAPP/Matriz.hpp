/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Matriz.hpp
 * Author: lucampelli
 *
 * Created on 17 de Março de 2017, 16:17
 */

#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#include <vector>

using namespace std;

class Matriz {
private:
    int num_lin, num_col;
    //Os valores da matriz são armazeados linearmente em um único vetor
    std::vector<float> valores;
    

public:

    Matriz(int linhas, int colunas){
        num_lin = linhas;
        num_col = colunas;
        valores->assign(num_lin*num_col, 0.0);
    }
    
    /*Cada linha inteira possui [nº colunas] valores, então o índice
     é calculado pelo número de 'linhas cheias' no vetor mais o número
     da coluna desejada. O valor é subtraido em 1 pois o vector conta
     as posições a partir de 0.*/
    float get(int l, int c){
        return valores[(l-1)*num_col + c-1];
    }
    
    float set(int l, int c, float dado){
        valores[(l-1)*num_col + c-1] = dado;
    }
    
    int lin(){
        return num_lin;
    }
    
    int col(){
        return num_col;
    }
    
    Matriz* operator*(Matriz* m){
        Matriz res = new Matriz(num_lin, m->col());
        for(int l = 1; l =< res->lin(); l++){
            for(int c = 1; c =< res->col(); c++){
                float dado_novo = 0.0;
                for(int i = 0; i < num_lin; i++){
                    dado_novo += this->get(l, c+i) * m->get(l+i, c); //wrong
                }
                res->set(l, c, dado_novo);
            }
            
        }
        return res;
    }

};

#endif /* MATRIZ_HPP */

