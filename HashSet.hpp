#ifndef HashSet_HPP
#define HashSet_HPP

#include <iostream>
#include <vector>
#include <forward_list>
#include <functional>
#include <limits>

class HashSet {
    private:
        std::vector<std::forward_list<int>> bucket;
        size_t element_count;
        float max_load_factor;
        std::hash<int> hasher;

        size_t getIdx(int value) const {
            return hasher(value) % bucket.size();
        }
    public:
        class Iter {
            private:
                std::forward_list<int>::iterator list_it;                
                size_t idx;
                HashSet* set;
            public:
                friend class HashSet;

                using iterator_category = std::forward_iterator_tag;
                using value_type = int;
                using difference_type  = std::ptrdiff_t;
                using pointer = int*;
                using reference = int&;     

                Iter(std::forward_list<int>::iterator list_it = {}, size_t idx = 0, HashSet* h = nullptr) : list_it{list_it}, idx{idx}, set{h} {}
                Iter(const Iter& other) : list_it{other.list_it}, idx{other.idx}, set{other.set} {}

                bool operator==(Iter other) const { return list_it == other.list_it; }
                bool operator!=(Iter other) const { return list_it != other.list_it; }

                Iter& operator++() { 
                    ++list_it;
                    if (list_it == set->bucket[idx].end()) {
                        for (size_t i{idx + 1}; i < set->bucket.size(); ++i) {
                            if (!set->bucket[i].empty()) {
                                list_it = set->bucket[i].begin();
                                idx = i;
                                return *this;
                            }
                        }
                 
                        list_it = {};
                        idx = set->bucket.size();
                    }
                    return *this;
                }

                Iter operator++(int) {
                    Iter tmp(*this);
                    ++(*this);
                    return tmp;
                }
                
                int& operator*() const {
                    return *list_it;
                }
                
                int* operator->() const {
                    return list_it.operator->();
                }
        };

        class constIter {
            private:
                std::forward_list<int>::const_iterator list_it;                
                size_t idx;
                const HashSet* set;
            public:
                friend class HashSet;

                using iterator_category = std::forward_iterator_tag;
                using value_type = int;
                using difference_type  = std::ptrdiff_t;
                using pointer = const int*;
                using reference = const int&;       

                constIter(std::forward_list<int>::const_iterator list_it = {}, size_t idx = 0, const HashSet* h = nullptr) : list_it{list_it}, idx{idx}, set{h} {}
                constIter(const constIter& other) : list_it{other.list_it}, idx{other.idx}, set{other.set} {}

                bool operator==(constIter other) const { return list_it == other.list_it; }
                bool operator!=(constIter other) const { return list_it != other.list_it; }

                constIter& operator++() { 
                    ++list_it;
                    if (list_it == set->bucket[idx].end()) {
                        for (size_t i{idx + 1}; i < set->bucket.size(); ++i) {
                            if (!set->bucket[i].empty()) {
                                list_it = set->bucket[i].begin();
                                idx = i;
                                return *this;
                            }
                        }
                       list_it = {};
                       idx = set->bucket.size();
                    }
                    return *this;
                }

                constIter operator++(int) {
                    constIter tmp(*this);
                    ++(*this);
                    return tmp;
                }
                
                const int& operator*() const {
                    return *list_it;
                }
                
                const int* operator->() const {
                    return list_it.operator->();
                }
        };

        HashSet() : element_count{0}, max_load_factor{1.0} { bucket.resize(20) ; }
        explicit HashSet(size_t bucket_count) : element_count{0}, max_load_factor{1.0} { bucket.resize(bucket_count); }
        template<typename InputIt>
        HashSet(InputIt first, InputIt last, size_t bucket_count);
        HashSet(const HashSet& other) : element_count{other.element_count}, max_load_factor{other.max_load_factor}, bucket{other.bucket} {}
        HashSet(HashSet&& other)noexcept : element_count{std::move(other.element_count)}, max_load_factor{std::move(other.max_load_factor)}, bucket{std::move(other.bucket)} {}
        HashSet(std::initializer_list<int> init, size_t bucket_count);
        ~HashSet() = default;

        HashSet& operator=(const HashSet& other);
        HashSet& operator=(HashSet&& other) noexcept;
        HashSet& operator=(std::initializer_list<int> ilist);

        Iter begin() noexcept;
        constIter begin() const noexcept;
        constIter cbegin() const noexcept;

        Iter end() noexcept;
        constIter end() const noexcept;
        constIter cend() const noexcept;
  
        bool empty() const noexcept { return bucket.empty(); }
        size_t size() const noexcept { return element_count; } 
        size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }
        
        void clear() noexcept;
        std::pair<Iter, bool> insert(const int& value);
        std::pair<Iter, bool> insert(int&& value);
        Iter insert(constIter hint, const int& value);
        Iter insert(constIter hint, int&& value);
        template<typename InputIt>
        void insert(InputIt first, InputIt last);
        void insert(std::initializer_list<int> ilist);
        template<typename... Args >
        std::pair<Iter, bool> emplace(Args&&... args);
        template<typename... Args>
        Iter emplace_hint(constIter hint, Args&&... args);
        Iter erase(Iter pos);
        Iter erase(constIter pos);
        Iter erase(constIter first, constIter last);
        size_t erase(const int& key);
        void swap(HashSet& other) noexcept { std::swap(bucket, other.bucket); }
        void merge(HashSet& other);

        size_t count(const int& key) const;
        Iter find(const int& key);
        constIter find(const int& key) const;
        bool contains(const int& key) const { return !(find(key) == end()); }
        std::pair<Iter, Iter> equal_range(const int& key);
        std::pair<constIter, constIter> equal_range(const int& key) const;


        std::forward_list<int>::iterator begin(size_t n) { return bucket[n].begin();}
        std::forward_list<int>::const_iterator begin(size_t n) const { return bucket[n].cbegin(); }
        std::forward_list<int>::const_iterator cbegin(size_t n) const { return bucket[n].cbegin(); }
        
        std::forward_list<int>::iterator end(size_t n) { return bucket[n].end();}
        std::forward_list<int>::const_iterator end(size_t n) const { return bucket[n].cend(); }
        std::forward_list<int>::const_iterator cend(size_t n) const { return bucket[n].cend(); }
         
        size_t bucket_count() const { return bucket.size(); }
        size_t max_bucket_count() const { return bucket.max_size(); }
        size_t bucket_size(size_t n) const;
        size_t _bucket(const int& key) const { return getIdx(key); }

        float load_factor() const;
        float _max_load_factor() { return max_load_factor; }
        void _max_load_factor(float ml) { max_load_factor = ml; }
        void rehash(size_t count);
        void reserve(size_t count);

        friend bool operator==(const HashSet& lhs, const HashSet& rhs);
        friend bool operator!=(const HashSet& lhs, const HashSet& rhs) { return !(lhs == rhs); }
}; 

template<typename InputIt>
HashSet::HashSet(InputIt first, InputIt last, size_t bucket_count) {
    max_load_factor = 1.0;
    element_count = 0;

    if (bucket_count <= std::distance(first,last)) bucket.resize(std::distance(first,last));
    else bucket.resize(bucket_count);
    
    for (; first != last; ++first) {
        insert(*first);
    }
}

HashSet::HashSet(std::initializer_list<int> init, size_t bucket_count) {
    bucket.resize(std::max(init.size(), bucket_count));

    for (const auto& x : init) {
        insert(x);
    }
    
}

HashSet& HashSet::operator=(const HashSet& other) {
    if (this != &other) {
        HashSet tmp(other);

        std::swap(element_count, tmp.element_count);
        std::swap(max_load_factor, tmp.max_load_factor);
        std::swap(bucket, tmp.bucket);
    }
    return *this;
} 

HashSet& HashSet::operator=(HashSet&& other) noexcept {
    if (this != &other) {
        clear();
        element_count = std::move(other.element_count);
        max_load_factor = std::move(other.max_load_factor);
        bucket = std::move(other.bucket);

        other.element_count = 0;
        other.max_load_factor = 1.0;
        other.bucket = {};
    }
    return *this;
}      

HashSet& HashSet::operator=(std::initializer_list<int> ilist) {
    *this = HashSet(ilist, ilist.size());
    return *this;
}

typename HashSet::Iter HashSet::begin() noexcept {
    for (size_t i{}; i < bucket.size(); ++i) {
        if (!bucket[i].empty()) return Iter(bucket[i].begin(), i, this);
    }

    return end();
}

typename HashSet::constIter HashSet::begin() const noexcept {
    return cbegin();
}

typename HashSet::constIter HashSet::cbegin() const noexcept {
    for (size_t i{}; i < bucket.size(); ++i) {
        if (!bucket[i].empty()) return constIter(bucket[i].cbegin(), i , this);
    }

    return cend();
}

typename HashSet::Iter HashSet::end() noexcept {
    return Iter(std::forward_list<int>::iterator(), bucket.size(), this);
}

typename HashSet::constIter HashSet::end() const noexcept {
    return cend();
}

typename HashSet::constIter HashSet::cend() const noexcept {
    return constIter(std::forward_list<int>::const_iterator(), bucket.size(), this);
}

void HashSet::clear() noexcept {
    for (int i{}; i < bucket.size(); ++i) bucket[i].clear();
    element_count = 0;
}

std::pair<typename HashSet::Iter, bool> HashSet::insert(const int& value) {
    auto it = find(value);

    if (it != end()) return {it, false};
    if (static_cast<float>(element_count + 1) / bucket_count() >= max_load_factor) rehash(bucket.size() * 2);

    int idx = getIdx(value);
    bucket[idx].push_front(value);
    ++element_count;
    
    return {Iter(bucket[idx].begin(), idx, this), true};
}

std::pair<typename HashSet::Iter, bool> HashSet::insert(int&& value) {
    auto it = find(value);

    if (it != end()) return {it, false};
    if (static_cast<float>(element_count + 1) / bucket_count() >= max_load_factor) rehash(bucket.size() * 2);

    int idx = getIdx(value);
    bucket[idx].push_front(std::move(value));
    ++element_count;
    
    return {Iter(bucket[idx].begin(), idx, this), true};
}

typename HashSet::Iter HashSet::insert(constIter hint, const int& value) {
    int idx = getIdx(value);
    if (idx == hint.idx) {
        for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
            if (*it == value) { return Iter(it, idx, this); } 
        }
    }
    auto p = insert(value);
    return p.first;
}

typename HashSet::Iter HashSet::insert(constIter hint, int&& value) {
    int idx = getIdx(value);
    if (idx == hint.idx) {
        for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
            if (*it == value) { return Iter(it, idx, this); } 
        }
    }
    auto p = insert(std::move(value));
    return p.first;
}

template<typename InputIt>
void HashSet::insert(InputIt first, InputIt last) {
    for (; first != last; ++first) insert(*first);
}

void HashSet::insert(std::initializer_list<int> ilist) {
    for (const auto& x : ilist) insert(x);
}

template<typename... Args >
std::pair<typename HashSet::Iter, bool> HashSet::emplace(Args&&... args) {
    int value{std::forward<Args>(args)...};
    auto it = find(value);

    if (it != end()) return {it, false};
    if (static_cast<float>(element_count + 1) / bucket_count() >= max_load_factor) rehash(bucket.size() * 2);

    size_t idx = getIdx(value);
    bucket[idx].emplace_front(std::move(value));
    ++element_count;
    
    return {Iter(bucket[idx].begin(), idx, this), true};
}

template<typename... Args>
typename HashSet::Iter HashSet::emplace_hint(constIter hint, Args&&... args) {
    int value{std::forward<Args>(args)...};
    
    size_t idx = getIdx(value);
    if (idx == hint.idx) {
        for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
            if (*it == value) { return Iter(it, idx, this); } 
        }
    }
    auto p = emplace(std::move(value));
    return p.first;
}

typename HashSet::Iter HashSet::erase(Iter pos) {
    std::forward_list<int>::iterator next_it{bucket[pos.idx].end()};
    
    if (pos.list_it == bucket[pos.idx].begin()) { 
        bucket[pos.idx].pop_front();
        --element_count;
        next_it = bucket[pos.idx].begin();
    }

    else {
        auto tmp = bucket[pos.idx].begin();
        auto it = tmp;
        ++it;

        while (it != bucket[pos.idx].end()) {
            if (it == pos.list_it) {
                next_it = bucket[pos.idx].erase_after(tmp);
                --element_count;
                break;
            }
            ++tmp;
            ++it;
        }
    }

    if (next_it == bucket[pos.idx].end()) {
        for (size_t i{pos.idx + 1}; i < bucket.size(); ++i) {
            if (!bucket[i].empty()) {
                return Iter{bucket[i].begin(), i, this};
            }
        }
        return end();
    }
    
    return Iter{next_it, pos.idx, this};
} 

typename HashSet::Iter HashSet::erase(constIter pos) {
    std::forward_list<int>::iterator next_it{bucket[pos.idx].end()};
    
    if (pos.list_it == bucket[pos.idx].begin()) { 
        bucket[pos.idx].pop_front();
        --element_count;
        next_it = bucket[pos.idx].begin();
    }

    else {
        auto tmp = bucket[pos.idx].begin();
        auto it = tmp;
        ++it;

        while (it != bucket[pos.idx].end()) {
            if (it == pos.list_it) {
                next_it = bucket[pos.idx].erase_after(tmp);
                --element_count;
                break;
            }
            ++tmp;
            ++it;
        }
    }

    if (next_it == bucket[pos.idx].end()) {
        for (size_t i{pos.idx + 1}; i < bucket.size(); ++i) {
            if (!bucket[i].empty()) {
                return Iter{bucket[i].begin(), i, this};
            }
        }
        return end();
    }
    
    return Iter{next_it, pos.idx, this};
} 

typename HashSet::Iter HashSet::erase(constIter first, constIter last) {    
    Iter it = end();
    while (first != last) {
        auto tmp = first;
        ++first;
        it = erase(tmp);
    }
    return it;
}

size_t HashSet::erase(const int& key) {
    size_t idx = getIdx(key);
    auto prev = bucket[idx].before_begin();

    for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it, ++prev) {
        if (*it == key) {
            bucket[idx].erase_after(prev);
            --element_count;
            return 1;
        }
    }
    return 0;
}

void HashSet::merge(HashSet& other) {
    for (int i{}; i < other.bucket.size(); ++i) {
        auto& srcbucket = other.bucket[i];
        auto prev = srcbucket.before_begin();
        
        while (std::next(prev) != srcbucket.end()) {
            int val = *(std::next(prev));
            if (!this->contains(val)) {
                size_t idx = getIdx(val);
                bucket[idx].splice_after(bucket[idx].before_begin(), srcbucket, prev);
            } else ++prev;
        }
    }
}

size_t HashSet::count(const int& key) const {
    if (this->contains(key)) return 1;
    return 0;
}

typename HashSet::Iter HashSet::find(const int& key) {
    size_t idx = getIdx(key);
    
    for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
        if (*it == key) return Iter{it, idx, this};
    }

    return end();
}
        
typename HashSet::constIter HashSet::find(const int& key) const {
    size_t idx = getIdx(key);
    
    for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
        if (*it == key) return constIter{it, idx, this};
    }

    return cend();
}

std::pair<typename HashSet::Iter, typename HashSet::Iter> HashSet::equal_range(const int& key) {
    auto it = find(key);
    
    if (it != end()) { 
        auto first = it;
        auto second = ++it;
        return {first, second};
    } else return {end(), end()};
}

std::pair<typename HashSet::constIter, typename HashSet::constIter> HashSet::equal_range(const int& key) const {
    auto it = find(key);
    
    if (it != cend()) { 
        auto first = it;
        auto second = ++it;
        return {first, second};
    } else return {cend(), cend()};
}

size_t HashSet::bucket_size(size_t n) const {
    size_t count = 0;  
    for (auto it = begin(n); it != end(n); ++it) ++count;
    return count;
}

float HashSet::load_factor() const { 
    if (bucket_count() == 0) return 0.0f; 
    return static_cast<float>(element_count) / bucket_count();
}

void HashSet::rehash(size_t count) {
    std::vector<std::forward_list<int>> tmp(std::move(bucket));
    bucket.assign(count, std::forward_list<int>{});

    for (auto& x : tmp) {
        for (auto& y : x) {
            size_t idx = getIdx(y);
            bucket[idx].push_front(std::move(y));
        }
    }
}

void HashSet::reserve(size_t count) {   
    if (bucket.size() == 0) { 
        if (count) rehash(count);
        return;
    }
    if ((static_cast<float>(count)/ bucket.size()) >= max_load_factor) { 
        rehash(static_cast<int>(static_cast<float>(count) / max_load_factor) + 1);
    }
}

bool operator==(const HashSet& lhs, const HashSet& rhs) {
    if (lhs.size() != rhs.size()) return false;

    for (auto& x : lhs.bucket) {
        for (auto& key : x) {
            if (!rhs.contains(key)) return false;
        }
    }

    return true;
}


#endif