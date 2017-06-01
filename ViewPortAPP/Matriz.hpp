/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Matriz.hpp
 * Author: Grupo10 - Luca Campelli e Jonatan Guesser
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
    std::vector<double> valores;


public:

    Matriz(int linhas, int colunas) {
        num_lin = linhas;
        num_col = colunas;
        valores.assign(num_lin*num_col, 0.0);
    }

    /*Cada linha inteira possui [nº colunas] valores, então o índice
     é calculado pelo número de 'linhas cheias' no vetor mais o número
     da coluna desejada. O valor é subtraido em 1 pois o vector conta
     as posições a partir de 0.*/
    double get(int l, int c) {
        return valores[(l) * num_col + c];
    }

    double set(int l, int c, double dado) {
        valores[(l) * num_col + c] = dado;
    }

    int lin() {
        return num_lin;
    }

    int col() {
        return num_col;
    }

    Matriz* multiply(Matriz* m) {

        Matriz* m3 = new Matriz(num_lin, m->col());

        for (int i = 0; i < num_lin; ++i)
            for (int j = 0; j < m->col(); ++j)
                for (int k = 0; k < num_col; ++k) {
                    m3->set(i, j, m3->get(i, j) + (this->get(i, k) * m->get(k, j)));
                }

        return m3;

    }

    Matriz* multiply(double esc) {
        Matriz* m3 = new Matriz(num_lin, num_col);

        for (int i = 0; i < num_lin; ++i)
            for (int j = 0; j < num_col; ++j) {
                m3->set(i, j, this->get(i, j) * esc);
            }

        return m3;
    }

    void print() {
        for (int i = 0; i < num_lin; i++) {
            for (int j = 0; j < num_col; j++) {
                cout << get(i, j) << " ";
            }
            cout << endl;
        }
    }

    Matriz* copy() {
        Matriz* r = new Matriz(num_lin, num_col);
        for (int i = 0; i < num_lin; i++) {
            for (int j = 0; j < num_col; j++) {
                r->set(i,j, get(i, j));
            }
        }
        
        return r;
    }
    
    Matriz* transpose(){
        Matriz* r = new Matriz(num_lin, num_col);
        for (int i = 0; i < num_lin; i++) {
            for (int j = 0; j < num_col; j++) {
                r->set(j,i, get(i, j));
            }
        }
        
        return r;
    }

};

#endif /* MATRIZ_HPP */

