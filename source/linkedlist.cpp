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


#include "linkedlist.h"

template <class T>
ListIterator<T>::ListIterator(ListElem<T> *n)
{
	curr = n;
}

template <class T>
ListIterator* ListIterator<T>::operator++()
{
	curr = curr->next;
	return this;
}

template <class T>
ListIterator* ListIterator<T>::operator--()
{
	curr = curr->previous;
	return this;
}

template <class T>
T& ListIterator<T>::operator->()
{
	return curr->e;
}

template <class T>
bool ListIterator<T>::out() const
{
	return curr==NULL;
}

template <class T>
bool ListIterator<T>::in() const
{
	return curr!=NULL;
}

template <class T>
ListElem<T>::ListElem(T& elem);
{
	e = elem;
}

template <class T>
List<T>::List()
{
	first = null;
	last = null;
}

template <class T>
List<T>::~List()
{
	clear();
}


template <class T>
void List<T>::addItem(T& item)
{
	last->next = new ListElem<T> (item);
	last = last->next;
}

template <class T>
void List<T>::removeItem(ListElem<T>* elem)
{
	if(elem->previous == NULL) //elem is the first one
		first = elem->next;
	else
		elem->previous->next = elem->next;
		
	if(elem->next == NULL) //elem is the last one
		last = elem->previous;
	else
		elem->next->previous = elem->previous;
	
	elem->previous = NULL;
	elem->next = NULL;
	
	delete elem;
}

template <class T>
ListElem<T>* List<T>::firstElem() const
{
	return first;
}

template <class T>
ListElem<T>* List<T>::lastElem() const
{
	return last;
}

template <class T>
void List<T>::clear()
{
	while(first != NULL)
	{
		ListElem<T> next = first->next;
		delete first;
		first = next;
	}
	
	last = NULL;
}


template <class T>
ListIterator<T> List<T>::begin() const
{
	return ListIterator<T>(firstElem());
}

template <class T>
ListIterator<T> List<T>::end() const
{
	return ListIterator<T>(lastElem());
}