

#ifndef MULTISTACKLIB_MULTISTACK_H
#define MULTISTACKLIB_MULTISTACK_H

#include <stdexcept>
#include <fstream>
#include <cmath>
#include "Stack.h"
#include <iostream>

using namespace std;

template<class T>
class TMultiStack {
public:
    T *arr = nullptr;
    int size;
    int stack_count;
    TStack<T> *stacks;
    T** old_data;
    void relocate();
public:
    TMultiStack(int size = 1, int stack_count = 1);
    TMultiStack(const TMultiStack<T> &stack);
    ~TMultiStack();
    TMultiStack<T> &operator=(const TMultiStack<T> &stack);
    void push(T item, int index);
    T pop(int index);
    bool is_empty(int i);
    bool is_full(int i);
    int get_stack_with_least_elements();
    template<class T1>
    friend istream &operator>>(istream &istream, TMultiStack<T1> &stack);
    template<class T1>
    friend ostream &operator<<(ostream &ostream, const TMultiStack<T1> &stack);
    static TMultiStack<T> &load(const char *name);
    void save(const char *name);
};

template<class T>
TMultiStack<T>::TMultiStack(int size, int stack_count) {
    if ((size <= 0) || (stack_count <= 0))
        throw std::range_error("Wrong multi-stack size");
    this->size = size;
    this->stack_count = stack_count;
    this->arr = new T[size];
    for (int i = 0; i < this->size; ++i) {
        this->arr[i] = 0;
    }
    this->stacks = new TStack<T>[stack_count];
    this->old_data = new T*[stack_count];

    int count = int(floor(double(size) / stack_count));
    int *sizes = new int[this->stack_count];
    for (int j = 0; j < this->stack_count - 1; ++j) {
        sizes[j] = count;
    }
    sizes[this->stack_count - 1] = size - (count * (stack_count - 1));

    int q = 0;
    for (int k = 0; k < stack_count; ++k) {
        this->stacks[k].set_data(&(this->arr[q]), sizes[k], -1);
        this->old_data[k] = &(this->arr[q]);
        q = q + sizes[k];
    }
}

template<class T>
TMultiStack<T>::TMultiStack(const TMultiStack<T> &stack) {
    if (this->size != stack.size) {
        delete[] this->arr;
        this->size = stack.size;
        this->arr = new T [stack.size];
    }
    this->size = stack.size;
    this->stack_count = stack.stack_count;
    this->stacks = new TStack<T>[this->stack_count];
    this->old_data = new T*[stack_count];
    for (int i = 0; i < this->size; ++i) {
        this->arr[i] = stack.arr[i];
    }
    for (int j = 0; j < this->stack_count; ++j) {
        this->stacks[j] = stack.stacks[j];
        this->old_data[j] = stack.old_data[j];
    }
}

template<class T>
void TMultiStack<T>::push(T item, int index) {
    if ((index < 0) || (index>=this->stack_count)) {
        throw std::overflow_error("Stack index error");
    }
    if(this->stacks[index].is_full()){
        this->relocate();
    }
    this->stacks[index].push(item);

}

template<class T>
T TMultiStack<T>::pop(int index) {
  if ((index < 0) || (index >= this->stack_count)) {
    throw std::overflow_error("Stack index error");
  }
  if (this->stacks[index].is_empty()) {
    throw std::overflow_error("Stack is empty");
  }
  T res = this->stacks[index].pop();
  return res;
}


template<class T>
TMultiStack<T>& TMultiStack<T>::operator=(const TMultiStack<T>& stack) {
  if (this == &stack)
    return *this;
  this->size = stack.size;
  delete[] this->arr;
  delete[] this->stacks;
  this->arr = new T[this->size];
  for (int i = 0; i < this->size; ++i) {
    this->arr[i] = stack.arr[i];
  }
  this->stacks = new TStack<T>[this->stack_count];
  for (int j = 0; j < this->stack_count; ++j) {
    this->stacks[j] = stack.stacks[j];
  }
  return *this;
}

template<class T>
TMultiStack<T>::~TMultiStack() {
    delete[] this->arr;
    delete[] this->stacks;
    this->stacks = nullptr;
    this->arr = nullptr;
}

template<class T>
bool TMultiStack<T>::is_empty(int index) {
    if ((index < 0) || (index>=this->stack_count)) {
        throw std::overflow_error("Stack index error");
    }
    return this->stacks[index].is_empty();
}

template<class T>
bool TMultiStack<T>::is_full(int index) {
    if ((index < 0) || (index>=this->stack_count)) {
        throw std::overflow_error("Stack index error");
    }
    return this->stacks[index].is_full();
}

template<class T>
int TMultiStack<T>::get_stack_with_least_elements() {
    int index = 0;
    if(this->stacks[0].is_empty())
        return index;
    int item_count = this->stacks[0].get_item_count();
    for (int j = 0; j < this->stack_count; ++j) {
        if(this->stacks[j].is_empty())
            return j;
        if(this->stacks[j].get_item_count() < item_count){
            index = j;
        }
    }
    return index;
}

template<class T1>
istream &operator>>(istream &istream, TMultiStack<T1> &stack) {
    int stack_count = 0;
    istream >> stack_count;
    int size = 0;
    istream >> size;
    stack.resize(size, stack_count);
    for (int i = 0; i <= stack.front; ++i) {
        istream >> stack.stacks[i];
    }
    return istream;
}

template<class T1>
ostream &operator<<(ostream &ostream, const TMultiStack<T1> &stack) {
    for (int i = 0; i < stack.stack_count; ++i) {
        ostream << stack.stacks[i];
    }
    ostream << "\n";
    return ostream;
}

template<class T>
TMultiStack<T> &TMultiStack<T>::load(const char *name) {
    std::ifstream file;
    file.open(name, std::fstream::in);
    if (!file.is_open())
        throw "Cant open such file";
    int size;
    file >> size;
    TMultiStack *stack = new TMultiStack(size);
    file >> stack->front;
    file >> *stack;
    file.close();
    return *stack;
}

template<class T>
void TMultiStack<T>::save(const char *name) {
    fstream file;
    file.open(name, fstream::out);
    if (!file.is_open())
        throw "Cant open such file";
    file << this->size << " " << this->stack_count << "\n" << *this;
    file.close();
}

template<class T>
void TMultiStack<T>::relocate() {
    int free_s = 0;
    for (int i = 0; i < this->stack_count; ++i) {
        free_s += this->stacks[i].get_size() - this->stacks[i].get_item_count();
    }
    int count = int(floor(double(free_s) / this->stack_count));
    if(count == 0){
        throw std::overflow_error("Stack overflow");
    }
    int *sizes = new int[this->stack_count];
    for (int j = 0; j < this->stack_count - 1; ++j) {
        sizes[j] = this->stacks[j].get_item_count() + count;
    }
    sizes[this->stack_count - 1] = this->stacks[this->stack_count - 1].get_item_count() +
            free_s - (count * (this->stack_count-1));

    T** new_data = new T*[this->stack_count];
    int q = 0;
    for (int k = 0; k < this->stack_count; ++k) {
        new_data[k] = &(this->arr[q]);
        q = q + sizes[k];
    }

    for (int l = 0; l < this->stack_count; ++l) {
        if(new_data[l] == this->old_data[l]){
            this->stacks[l].set_data(new_data[l], sizes[l], this->stacks[l].get_item_count()-1);
        }else if(new_data[l] < this->old_data[l]){
            for (int i = 0; i < stacks[l].get_item_count(); ++i) {
                new_data[l][i] = this->old_data[l][i];
            }
            this->stacks[l].set_data(new_data[l], sizes[l], this->stacks[l].get_item_count()-1);
        }else if(new_data[l] > this->old_data[l]){
            int k = l;
            for (; k < this->stack_count; ++k) {
                if(new_data[k] <= this->old_data[k])
                    break;
            }
            k--;
            for (int i = k; i >= l; --i) {
                for (int j = this->stacks[i].get_item_count() - 1; j >= 0; j--) {
                    new_data[i][j] = this->old_data[i][j];
                }
                this->stacks[i].set_data(new_data[i], sizes[i], this->stacks[i].get_item_count()-1);
            }
        }
    }
    T** buffer = this->old_data;
    this->old_data = new_data;
    delete[] buffer;
    delete[] sizes;
}


#endif //MULTISTACKLIB_MULTISTACK_H
