#include "list.hpp"

namespace ds {

template <typename T>
struct List<T>::Node
{

public:

    explicit Node(T data, Node* prev = nullptr, Node* next = nullptr)
    : data{data}
    , prev{prev}
    , next{next}
    {
        assert(prev);
        assert(next);
    }

    ~Node() = default;
    Node(Node const& other) = default;
    Node& operator=(Node const& other) = default;
    
    T data;
    Node* prev;
    Node* next;
    
};


template <typename T>
List<T>::List()
: m_head{0, &m_head, &m_tail}
, m_tail{0, &m_head, &m_tail}
, m_size{}
{}


template <typename T>
List<T>::~List()
{
    Node* curr = m_head.next;
    Node* prev = curr;
    
    while(curr != &m_tail){
        curr = curr->next;
        delete prev;
        prev = curr;
    }
}


template <typename T>
List<T>::List(List const &other)
: List{}
{
    Node* curr = other.m_head.next;
    while (curr != &other.m_tail)
    {
        push_tail(curr->data);
        curr = curr->next;
    }
}


template <typename T>
List<T> &List<T>::operator=(List const &other)
{
    if (this != &other) {
        List temp{other}; 
        swap(temp);      
    }
    return *this;
}


template <typename T>
void List<T>::push_head(T const &data)
{   
    Node* real_head = m_head.next;

    Node* new_node = new Node{data, &m_head, real_head};

    real_head->prev = new_node;
    m_head.next = new_node;

    ++m_size;
}


template <typename T>
void List<T>::push_tail(T const & data)
{
    Node* real_tail = m_tail.prev;

    Node* new_node = new Node{data, real_tail, &m_tail};

    real_tail->next = new_node;
    m_tail.prev = new_node;

    ++m_size;
}


template <typename T>
bool List<T>::pop_head(T &r_value)
{
    Node* real_head = m_head.next;
    if(real_head == &m_tail){
        return false;
    }
    
    r_value = real_head->data;

    real_head->next->prev = &m_head;
    m_head.next = real_head->next;

    delete real_head;

    --m_size;
    return true;
}


template <typename T>
bool List<T>::pop_tail(T &r_value)
{
    Node* real_tail = m_tail.prev;
    if(real_tail == &m_head){
        return false;
    }
    
    r_value = real_tail->data;

    m_tail.prev = real_tail->prev;
    real_tail->prev->next = &m_tail;

    delete real_tail;

    --m_size;
    return true;
}


template <typename T>
size_t List<T>::size() const
{
    return m_size;
}


template <typename T>
bool List<T>::empty() const
{
    return m_size == 0;
}


template <typename T>
void List<T>::clear()
{
    Node* curr = m_head.next;
    Node* prev = curr;
    
    while(curr != &m_tail){
        curr = curr->next;
        delete prev;
        prev = curr;
    }

    m_head.next = &m_tail;
    m_tail.prev = &m_head;
    m_size = 0;
}


template <typename T>
void List<T>::swap(List &other)
{

    std::swap(m_size, other.m_size);
    std::swap(m_head.next, other.m_head.next);
    std::swap(m_tail.prev, other.m_tail.prev);

    m_head.next->prev = &m_head;
    m_tail.prev->next = &m_tail;

    other.m_head.next->prev = &other.m_head;
    other.m_tail.prev->next = &other.m_tail;
    
}



template <typename T>
template <typename A>
void List<T>::for_each(A action)
{
    Node* curr = m_head.next;
    while(curr != &m_tail){
        action(curr->data);
        curr = curr->next;
    }
}

}