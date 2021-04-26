#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <iterator>
#include <fstream>
//
//
namespace tools {
    template<bool B, typename T, typename F>
    struct cond_t {
        typedef F type;
    };

    template<typename T, typename F>
    struct cond_t<true, T, F> {
        typedef T type;
    };
}

template <size_t ChunkSize>
class FixedAllocator {
    struct Chunk {
        Chunk* next = nullptr;
        explicit Chunk(Chunk* next): next(next) {};
    };
    Chunk* first_free = nullptr;
    size_t chunk_size = ChunkSize;
    std::vector<Chunk*> blocks;
    int pointer_counter = 0;
    const static size_t chunks_per_block = 8;

    void new_block() {
        size_t block_size = chunk_size * chunks_per_block;
        auto* new_block = reinterpret_cast<Chunk*>(operator new(block_size));
        blocks.push_back(new_block);
        Chunk* chunk_iter = new_block;
        for (size_t i = 0; i < chunks_per_block - 1; ++i) {
            new(chunk_iter) Chunk(reinterpret_cast<Chunk*>(reinterpret_cast<uint8_t*>(chunk_iter) + chunk_size));
            chunk_iter = chunk_iter->next;
        }
        new(chunk_iter) Chunk(nullptr);
        first_free = new_block;
    }

public:
    FixedAllocator() {
        if (ChunkSize < 8)
            chunk_size = 8;
    }

    void increase_counter() {
        ++pointer_counter;
    }

    void decrease_counter() {
        --pointer_counter;
        if (pointer_counter == 0) {
            delete this;
        }
    }

    void* allocate() {
        if (!first_free)
            new_block();
        Chunk* copy = first_free;
        first_free = first_free->next;
        return copy;
    }

    void deallocate(void* chunk) {
        auto* new_chunk = reinterpret_cast<Chunk*>(chunk);
        new_chunk->next = first_free;
        first_free = new_chunk;
    }

    ~FixedAllocator() {
        for (auto it: blocks) {
            operator delete(it);
        }
    }

};


template <typename T>
class FastAllocator {
    FixedAllocator<sizeof(T)>* alloc;
public:
    using value_type = T;
    FastAllocator() {
        alloc = new FixedAllocator<sizeof(T)>();
        alloc->increase_counter();
    }
    FastAllocator(const FastAllocator<T>& another) {
        alloc = another.alloc;
        alloc->increase_counter();
    }
    template<typename U>
    explicit FastAllocator(const FastAllocator<U>& other): FastAllocator() {
        other.help();
    }

    T* allocate(int num) {
        if (num > 1)
            return reinterpret_cast<T*>(operator new(num * sizeof(T)));
        return reinterpret_cast<T*>(alloc->allocate());
    }
    void deallocate(T* ptr, int num) {
        if (num > 1)
            operator delete[](ptr);
        else
            alloc->deallocate(ptr);
    }
    int help() const{
        return 2;
    }
    bool operator==(const FastAllocator<T>& another) const {
        return alloc == another.alloc;
    }
    bool operator !=(const FastAllocator<T>& another) const {
        return alloc != another.alloc;
    }
    FastAllocator& operator=(const FastAllocator<T>& another) {
        if (this == &another) {
            return *this;
        }
        alloc->decrease_counter();
        alloc = another.alloc;
        alloc->increase_counter();
        return *this;
    }
    ~FastAllocator() {
        alloc->decrease_counter();
    }
};


template <typename T, typename Alloc=std::allocator<T>>
class List {

    struct EmptyNode {
        EmptyNode* prev;
        EmptyNode* next;
        EmptyNode(EmptyNode* prev, EmptyNode* next): prev(prev), next(next) { };
    };

    struct FilledNode: EmptyNode {
        T content;
        template <typename ...Args>
        FilledNode(EmptyNode* prev, EmptyNode* next, Args&&... args): EmptyNode(prev, next), content(std::forward<Args>(args)...) { };
    };

    using AllocTraitT = std::allocator_traits<Alloc>;
    using EmptyNodeAlloc = typename AllocTraitT::template rebind_alloc<EmptyNode>;
    using FilledNodeAlloc = typename AllocTraitT::template rebind_alloc<FilledNode>;
    using AllocTraitEmptyNode = std::allocator_traits<EmptyNodeAlloc>;
    using AllocTraitFilledNode = std::allocator_traits<FilledNodeAlloc>;

    EmptyNode* first_el = nullptr;
    EmptyNode* last_el = nullptr;
    size_t els = 0;

    EmptyNodeAlloc empty_node_allocator;
    FilledNodeAlloc filled_node_allocator;
    Alloc T_allocator;

    void copy(const List& another, bool copy_alloc_empty, bool copy_alloc_filled, bool copy_T_alloc) {
        if (copy_alloc_empty) {
            empty_node_allocator = another.empty_node_allocator;
        }
        if (copy_alloc_filled) {
            filled_node_allocator = another.filled_node_allocator;
        }
        if (copy_T_alloc) {
            T_allocator = another.T_allocator;
        }
        auto* cur_el = reinterpret_cast<FilledNode*>(another.first_el);
        for (size_t i = 0; i < another.els; ++i) {
            push_back(cur_el->content);
            if (cur_el->next->next) // to avoid UB
                cur_el = reinterpret_cast<FilledNode*>(cur_el->next);
        }
    }

    void initialize() {
        EmptyNode* fake_node = AllocTraitEmptyNode::allocate(empty_node_allocator, 1);
        AllocTraitEmptyNode::construct(empty_node_allocator, fake_node, nullptr, nullptr);
        first_el = fake_node;
        last_el = fake_node;
    }

    template <typename... Args>
    EmptyNode* create_node(Args&&... args) {
        FilledNode* new_node = AllocTraitFilledNode::allocate(filled_node_allocator, 1);
        AllocTraitFilledNode::construct(filled_node_allocator, new_node, nullptr, nullptr, std::forward<Args>(args)...);
        return new_node;
    }

    void delete_node(EmptyNode* ptr) {
        AllocTraitEmptyNode::destroy(empty_node_allocator, ptr);
    }

    void delete_node(FilledNode* ptr) {
        AllocTraitFilledNode::destroy(filled_node_allocator, ptr);
        AllocTraitFilledNode::deallocate(filled_node_allocator, ptr, 1);
    }

public:

    explicit List(size_t count, const Alloc& allocator = Alloc()):
    empty_node_allocator(allocator),filled_node_allocator(allocator), T_allocator(allocator) {
        initialize();
        for (;count;--count)
            emplace_back();
    }

    explicit List(size_t count, const T& value, const Alloc& allocator = Alloc()):
    empty_node_allocator(allocator), filled_node_allocator(allocator), T_allocator(allocator) {
        initialize();
        for (size_t i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    explicit List(const Alloc& alloc = Alloc()): empty_node_allocator(alloc), filled_node_allocator(alloc),
    T_allocator(alloc) {
        initialize();
    }

    List(const List<T, Alloc>& another):
    empty_node_allocator(AllocTraitEmptyNode::select_on_container_copy_construction(another.empty_node_allocator)),
    filled_node_allocator(AllocTraitFilledNode::select_on_container_copy_construction(another.filled_node_allocator)),
    T_allocator(AllocTraitT::select_on_container_copy_construction(another.T_allocator)) {
        initialize();
        copy(another, false, false, false);
    }

    List& operator=(const List<T, Alloc>& another) {
        if (this == &another)
            return *this;
        clear();
        copy(another, AllocTraitEmptyNode::propagate_on_container_copy_assignment::value,
             AllocTraitFilledNode::propagate_on_container_copy_assignment::value,
             AllocTraitT::propagate_on_container_copy_assignment::value);
        return *this;
    }

    void clear() {
        while (els > 0)
            pop_back();
    }

    size_t size() const noexcept{
        return els;
    }

    void push_back(const T& el) {
        insert(end(), el);
    }

    void push_front(const T& el) {
        insert(begin(), el);
    }

    void pop_back() {
        erase(end() - 1);
    }

    void pop_front() {
        erase(begin());
    }

    template <bool IsConst>
    class common_iterator {
    public:

        using difference_type = int;
        using value_type = T;
        using pointer = typename tools::cond_t<IsConst, const T*, T*>::type;
        using reference = typename tools::cond_t<IsConst, const T&, T&>::type;
        using iterator_category = std::bidirectional_iterator_tag;

    private:
        friend List;

        EmptyNode* cur_el = nullptr;

        explicit common_iterator(EmptyNode* ptr): cur_el(ptr) {};

    public:

        common_iterator() = default;

        common_iterator(const common_iterator& a): cur_el(a.cur_el) { };

        common_iterator& operator++() {
            cur_el = cur_el->next;
            return *this;
        }

        common_iterator operator++(int) {
            common_iterator copy = *this;
            ++(*this);
            return copy;
        }

        common_iterator& operator +=(int val) {
            for (int i = 0; i < val; ++i)
                ++(*this);
            return *this;
        }

        common_iterator& operator-=(int val) {
            for (int i = 0; i < val; ++i) {
                --(*this);
            }
            return *this;
        }

        common_iterator operator +(int val) {
            common_iterator copy = *this;
            copy += val;
            return copy;
        }

        common_iterator operator -(int val) {
            common_iterator copy = *this;
            copy -= val;
            return copy;
        }

        common_iterator& operator--() {
            cur_el = cur_el->prev;
            return *this;
        }

        common_iterator operator--(int) {
            common_iterator copy = *this;
            --(*this);
            return copy;
        }

        operator common_iterator<true>() const {
            return common_iterator<true>(cur_el);
        }

        bool operator<=(const common_iterator& op) const {
            return cur_el <= op.cur_el;
        }

        bool operator>=(const common_iterator& op) const {
            return cur_el >= op.cur_el;
        }

        bool operator<(const common_iterator& op) const {
            return cur_el < op.cur_el;
        }

        bool operator>(const common_iterator& op) const {
            return cur_el > op.cur_el;
        }

        bool operator==(const common_iterator& op) const {
            return (cur_el == op.cur_el);
        }

        bool operator!=(const common_iterator& op) const {
            return (cur_el != op.cur_el);
        }

        reference operator*() {
            return reinterpret_cast<FilledNode*>(cur_el)->content;
        }

        pointer operator->() {
            return &reinterpret_cast<FilledNode*>(cur_el)->content;
        }

        const T& operator*() const {
            return reinterpret_cast<FilledNode*>(cur_el)->content;;
        }

        const T* operator->() const {
            return &reinterpret_cast<FilledNode*>(cur_el)->content;
        }
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return iterator(first_el);
    }

    iterator end() {
        return iterator(last_el);
    }

    const_iterator cbegin() const {
        return const_iterator(first_el);
    }

    const_iterator cend() const {
        return const_iterator(last_el);
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(crbegin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(crend());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    Alloc get_allocator() {
        return T_allocator;
    }

    ~List() {
        clear();
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        insert(end(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void insert(const_iterator it, Args&&... args) {
        EmptyNode* prev_node = it.cur_el->prev;
        EmptyNode* new_node = create_node(std::forward<Args>(args)...);
        new_node->prev = prev_node;
        new_node->next = it.cur_el;
        it.cur_el->prev = new_node;
        if (prev_node)
            prev_node->next = new_node;
        else
            first_el = new_node;
        ++els;
    }

    void erase(const_iterator it) {
        EmptyNode* node_ptr = it.cur_el;
        EmptyNode* prev = node_ptr->prev;
        EmptyNode* next = node_ptr->next;
        if (prev)
            prev->next = next;
        else
            first_el = next;
        next->prev = prev;
        delete_node(reinterpret_cast<FilledNode*>(node_ptr));
        --els;
    }

};
