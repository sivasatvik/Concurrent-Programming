#include <iostream>
#include <algorithm>
#include "lazylist.h"



LazyList::LazyList(){
		head = new Node;
		tail = new Node;
		head->key = 0;
		head->next = tail;
		head->marked = false;
		tail->key = INT_MAX;
		tail->marked = false;
		tail->next = NULL;
	}

bool LazyList::validate(Node *pred, Node *curr){
	return !pred->marked && !curr->marked && pred->next == curr;
}


bool LazyList::add(my_pair item){
	int key = item.second;
	int i = 0;
	while(1){
		Node * pred = head;
		Node * curr = head->next;
		while(curr->key < key){
			pred = curr; curr = curr->next; i++;
		}
		// pred_mx[i].lock();
		// curr_mx[i+1].lock();
		mx[i].lock();
		mx[i+1].lock();
			
		if(validate(pred, curr)){
			if(curr->key == key){
				return false;
			}
			else{
				Node *node = new Node;
				node->item = item;
				node->key = key;
				node->marked = false;
				node->next = curr;
				pred->next = node;
				return true;
			}
		}
		// curr_mx.unlock();
		// pred_mx.unlock();
		mx[i].unlock();
		mx[i+1].unlock();
	}
}


bool LazyList::remove(my_pair item){
	int key = item.second;
	int i = 0;
	while(1){
		Node *pred = head;
		Node *curr = head->next;
		while(curr->key < key){
			pred = curr; curr = curr->next; i++;
		}
		// pred_mx[i].lock();
		// curr_mx[i+1].lock();
		mx[i].lock();
		mx[i+1].lock();
		if(validate(pred, curr)){
			if(curr->key != key){
				return false;
			}
			else{
				curr->marked = true;
				pred->next = curr->next;
				return true;
			}
		}
		// curr_mx.unlock();
		// pred_mx.unlock();
		mx[i].unlock();
		mx[i+1].unlock();
	}
}


bool LazyList::contains(my_pair item){
	int key = item.second;
	Node *curr = head;
	while(curr->key < key){
		curr = curr->next;
	}
	return curr->key == key && !curr->marked;
}


std::vector<int> LazyList::choose(int id){
	Node * temp = head;
	temp = temp->next;
	for(int i = 0; i<id; i++){
		temp = temp->next;
	}
	return temp->item.first;
}