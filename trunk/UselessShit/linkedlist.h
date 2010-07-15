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

template <typename T>
class ListElem
{
	
	public:
	ListElem<T> *previous, *next;
	T e;
	ListElem<T>(const T& elem);
};


template <typename T>
class ListIterator
{
	ListElem<T> *curr;
	public: 
	ListIterator(ListElem<T> *n);
	ListIterator operator++();
	ListIterator operator--();
	T* operator->() const;
	T& operator*() const;
	
	bool out() const;
	bool in() const;
};

template <typename T>
class List
{
	
	public: 
	ListElem<T>* first, *last;
	List();
	~List();
	void addItem(const T& item);
	void removeItem(ListElem<T>* elem);
	ListElem<T>* firstElem() const;
	ListElem<T>* lastElem() const;
	ListIterator<T> begin() const;
	ListIterator<T> end() const;
	
	void clear();
};


///===================
/// LINKEDLIST.CPP
///===================

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

template <typename T>
ListIterator<T>::ListIterator(ListElem<T> *n)
{
	curr = n;
}

template <typename T>
ListIterator<T> ListIterator<T>::operator++()
{
	curr = curr->next;
	return *this;
}

template <typename T>
ListIterator<T> ListIterator<T>::operator--()
{
	curr = curr->previous;
	return *this;
}

template <typename T>
T* ListIterator<T>::operator->() const
{
	return &(curr->e);
}

template <typename T>
T& ListIterator<T>::operator*() const
{
	return curr->e;
}

template <typename T>
bool ListIterator<T>::out() const
{
	return curr==NULL;
}

template <typename T>
bool ListIterator<T>::in() const
{
	return curr!=NULL;
}

template <typename T>
ListElem<T>::ListElem(const T& elem)
{
	e = elem;
}

template <typename T>
List<T>::List()
{
	first = NULL;
	last = NULL;
}

template <typename T>
List<T>::~List()
{
	clear();
}


template <typename T>
void List<T>::addItem(const T& item)
{
	last->next = new ListElem<T> (item);
	last->next->previous = last;
	last = last->next;
	if(first == NULL)
		first = last;
}

template <typename T>
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

template <typename T>
ListElem<T>* List<T>::firstElem() const
{
	return first;
}

template <typename T>
ListElem<T>* List<T>::lastElem() const
{
	return last;
}

template <typename T>
void List<T>::clear()
{
	while(first != NULL)
	{
		ListElem<T> *next = first->next;
		delete first;
		first = next;
	}
	
	last = NULL;
}


template <typename T>
ListIterator<T> List<T>::begin() const
{
	return ListIterator<T>(firstElem());
}

template <typename T>
ListIterator<T> List<T>::end() const
{
	return ListIterator<T>(lastElem());
}

#endif
