/*
*   This file is part of NSMB Editor DS
*
*   NSMB Editor DS is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NSMB Editor DS is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NSMB Editor DS.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H


#ifndef NULL
#define NULL 0
#endif

template <class T>
class ListElem
{
	ListElem<T> *previous, *next;
	
	public:
	T e;
	explicit ListElem(T& elem);
};


template <class T>
class ListIterator
{
	ListElem<T> *curr;
	public: 
	ListIterator(ListElem<T> *n);
	ListIterator operator++();
	ListIterator operator--();
	bool out() const;
	bool in() const;
};

template <class T>
class List
{
	ListElem<T>* first, last;
	
	public: 
	List();
	~List();
	void addItem(T& item);
	void removeItem(ListElem<T>* elem);
	ListElem<T>* firstElem() const;
	ListElem<T>* lastElem() const;
	ListIterator<T> begin() const;
	ListIterator<T> end() const;
	
	void clear();
};


#endif