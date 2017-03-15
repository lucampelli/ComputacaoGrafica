/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Elemento.hpp
 * Author: lucampelli
 *
 * Created on 8 de Março de 2017, 14:05
 */

#ifndef ELEMENTO_HPP
#define ELEMENTO_HPP

template<typename T>
class Elemento {
private:
	T *info;
	Elemento<T>* _next;

 public:
 	Elemento(const T& info, Elemento<T>* next) : info(new T(info)), _next(next) {}
 
 	~Elemento()
 	{
 		delete info;
 	}
 
 	Elemento<T>* getProximo() const
 	{
 		return _next;
 	}
 
 	T getInfo() const
 	{
 		return *info;
 	}
 
 	void setProximo(Elemento<T>* next)
 	{
		_next = next;
 	}
 };

#endif /* ELEMENTO_HPP */

