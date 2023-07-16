#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <algorithm>
// добавьте неоходимые include-директивы сюда

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
public:
  SingleLinkedList() {}

   
  //=============================================================== добавили класс из лекции =========
  template <typename ValueType>
    class BasicIterator {   
                friend class SingleLinkedList;
        explicit BasicIterator(Node* node) {
        node_ = node;   
        }
   public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
        
    BasicIterator () = default;
    
//============================================================================== 

// Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
   BasicIterator(const BasicIterator<Type>& other) noexcept {
         node_= other.node_;
            // Реализуйте конструктор самостоятельно
        }
        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
   BasicIterator& operator=(const BasicIterator& rhs) = default;

       // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
        return node_ == rhs.node_;
        } 

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
       [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
           return node_ != rhs.node_;
        } 

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
           return node_ == rhs.node_;
        }
  
        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
              return  node_ != rhs.node_;
        }
//=============================================================================================

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
         assert(node_ != nullptr);
           node_ = node_->next_node;  
           return *this;
        } 

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
         assert(node_ != nullptr);
         auto old_value(*this); // Сохраняем прежнее значение объекта для последующего возврата
        ++(*this); // используем логику префиксной формы инкремента
        return old_value;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
         assert(node_ != nullptr);
         return node_->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
         assert(node_ != nullptr);
         return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
//========================================= добавим методы ======================

   template <typename Iterator>                           // можно в приват закиунть метод
    void Init(Iterator begin, Iterator end) {
        Node* node = &head_;
        for (Iterator i= begin;i!=end; ++i) {
            ++size_;
            node->next_node = new Node(*i, nullptr);
            node = node->next_node;
        }
    }

// новый ==========================================================================================
   // Конструктор с использованием диапазона итераторов
    template <typename Iterator>
    SingleLinkedList(Iterator begin, Iterator end) : head_(), size_(0) {
        Init(begin, end);
    }
    //=============================================================================================
    SingleLinkedList(std::initializer_list<Type> values) : head_(), size_(0) {
        Init(values.begin(), values.end());
    }
    SingleLinkedList(const SingleLinkedList& other) : head_(), size_(0) {
        Init(other.begin(), other.end());
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
       if (this != &rhs) {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }
//================================================================================================

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
          return Iterator{head_.next_node};
        //return {};
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
       // return {};
        return Iterator{nullptr};
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
      return ConstIterator {head_.next_node};
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
          return ConstIterator {nullptr};
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
       return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
//======================= добавили по заданию текущему - финалка ====================================================    

// Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению

[[nodiscard]] Iterator before_begin() noexcept {
    return Iterator{&head_}; // Создаем итератор, указывающий на фиктивный узел
}

// Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению

[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
    return before_begin(); // Возвращаем результат вызова before_begin()
}

// Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
[[nodiscard]] ConstIterator before_begin() const noexcept {
    return ConstIterator{const_cast<Node*>(&head_)}; // Создаем константный итератор, указывающий на фиктивный узел
} 
 
 //======================================================================================================
    
// Возвращает количество элементов в списке за время O(1)
   [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
// Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        if (!size_) {
        return true;
        }
        return false;
    }
   // Вставка элемента в начало списка
    void PushFront(const Type& value) {
       Node* new_node = new Node(value, head_.next_node);
        head_.next_node = new_node;
        ++size_;   
    }      
 //======================= добавили по заданию текущему финалке =======================================

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
 Iterator InsertAfter(ConstIterator pos, const Type& value) {
     Node* pos_node = pos.node_; // Получаем указатель на узел, на который указывает pos
    Node* new_node = new Node(value, pos_node->next_node); // Создаем новый узел с нужными значениями
    pos_node->next_node = new_node; // Вставляем новый узел после pos_node
    ++size_; // Увеличиваем размер списка
    return Iterator{new_node}; // Возвращаем итератор на вставленный элемент
}

    void PopFront() noexcept {
      if (head_.next_node) {
        Node* first_node = head_.next_node;
        head_.next_node = first_node->next_node; // Перемещаем указатель на следующий элемент после фиктивного узла
        delete first_node; // Удаляем первый элемент
        --size_; // Уменьшаем размер списка
    }
}

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
     Node* pos_node = pos.node_; // Получаем указатель на узел, на который указывает pos
    Node* node_to_remove = pos_node->next_node; // Получаем указатель на узел, который нужно удалить
    if (node_to_remove) {
        pos_node->next_node = node_to_remove->next_node; // Перемещаем указатель на следующий узел после pos_node
        delete node_to_remove; // Удаляем узел
        --size_; // Уменьшаем размер списка
    }
    return Iterator{pos_node->next_node}; // Возвращаем итератор на элемент, следующий за удалённым
}

//===============================================================================================
    // Очистка списка
    void Clear() noexcept {
        Node* current = head_.next_node;
        while (current) {
            Node* next_node = current->next_node;
            delete current;
            current = next_node;
        }
        head_.next_node = nullptr;
        size_ = 0;
    }
     
  ~SingleLinkedList() {
        Clear();
    }
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

//===============================  добавляем методы =====================================================

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {   // из лекции 
   lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
       return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
return !(lhs < rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
return !(lhs < rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
return !(lhs < rhs);
} 
