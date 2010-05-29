
#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

template <class T>
class ListElem
{
	ListElem<T> *previous, *next;
	
	public:
	T e;
	ListElem(T& elem);
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
	
	void clear();
};


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

#endif