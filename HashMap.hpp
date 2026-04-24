#ifndef HashMap_HPP
#define HashMap_HPP

#include <iostream>
#include <string>
#include <vector>
#include <forward_list>
#include <functional>
#include <limits>

class HashMap {
    private:
        std::vector<std::forward_list<std::pair<const std::string, int>>> bucket;
        size_t element_count;
        float max_load_factor;
        std::hash<std::string> hasher;

        size_t getIdx(const std::string& key) const {
            return hasher(key) % bucket.size();
        }
    public:
        class Iter {
            private:
                std::forward_list<std::pair<const std::string, int>>::iterator list_it;                
                size_t idx;
                HashMap* set;
            public:
                friend class HashMap;    

                Iter(std::forward_list<std::pair<const std::string, int>>::iterator list_it = {}, size_t idx = 0, HashMap* h = nullptr) : list_it{list_it}, idx{idx}, set{h} {}
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
                
                std::pair<const std::string, int>& operator*() const {
                    return *list_it;
                }
                
                std::pair<const std::string, int>* operator->() const {
                    return list_it.operator->();
                }
        };

        class constIter {
            private:
                std::forward_list<std::pair<const std::string, int>>::const_iterator list_it;                
                size_t idx;
                const HashMap* set;
            public:
                friend class HashMap;

                constIter(std::forward_list<std::pair<const std::string, int>>::const_iterator list_it = {}, size_t idx = 0, const HashMap* h = nullptr) : list_it{list_it}, idx{idx}, set{h} {}
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
                
                const std::pair<const std::string, int>& operator*() const {
                    return *list_it;
                }
                
                const std::pair<const std::string, int>* operator->() const {
                    return list_it.operator->();
                }
        };

        HashMap() : element_count{0}, max_load_factor{1.0} { bucket.resize(20) ; }
        explicit HashMap(size_t bucket_count) : element_count{0}, max_load_factor{1.0} { bucket.resize(bucket_count); }
        template<typename InputIt>
        HashMap(InputIt first, InputIt last, size_t bucket_count);
        HashMap(const HashMap& other) : element_count{other.element_count}, max_load_factor{other.max_load_factor}, bucket{other.bucket} {}
        HashMap(HashMap&& other)noexcept : element_count{std::move(other.element_count)}, max_load_factor{std::move(other.max_load_factor)}, bucket{std::move(other.bucket)} {}
        HashMap(std::initializer_list<std::pair<const std::string, int>> init, size_t bucket_count);
        ~HashMap() = default;

        HashMap& operator=(const HashMap& other);
        HashMap& operator=(HashMap&& other) noexcept;
        HashMap& operator=(std::initializer_list<std::pair<const std::string, int>> ilist);

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
        std::pair<Iter, bool> insert(const std::pair<const std::string, int>& value);
        std::pair<Iter, bool> insert(std::pair<const std::string, int>&& value);
        template<typename P>
        std::pair<Iter, bool> insert(P&& value) { return emplace(std::forward<P>(value)); }
        Iter insert(constIter hint, const std::pair<const std::string, int>& value);
        Iter insert(constIter hint, std::pair<const std::string, int>&& value);
        template<typename P>
        Iter insert(constIter hint, P&& value) { return emplace_hint(hint, std::forward<P>(value)); }
        template<typename InputIt>
        void insert(InputIt first, InputIt last);
        void insert(std::initializer_list<std::pair<const std::string, int>> ilist);
        template<typename M>
        std::pair<Iter, bool> insert_or_assign(const std::string& k, M&& obj);
        template<typename M>
        std::pair<Iter, bool> insert_or_assign(std::string&& k, M&& obj);
        template<typename K, typename M>
        std::pair<Iter, bool> insert_or_assign(K&& k, M&& obj);
        template<typename M>
        Iter insert_or_assign(constIter hint, const std::string& k, M&& obj);
        template<typename M>
        Iter insert_or_assign(constIter hint, std::string&& k, M&& obj);
        template<typename K, typename M>
        Iter insert_or_assign(constIter hint, K&& k, M&& obj);
        template<typename... Args >
        std::pair<Iter, bool> emplace(Args&&... args);
        template<typename... Args>
        Iter emplace_hint(constIter hint, Args&&... args);
        Iter erase(Iter pos);
        Iter erase(constIter pos);
        Iter erase(constIter first, constIter last);
        size_t erase(const std::string& key);
        void swap(HashMap& other) noexcept { std::swap(bucket, other.bucket); }
        void merge(HashMap& other);

        int& at(const std::string& key );
        const int& at(const std::string& key) const;
        template<typename K>
        int& at(const K& x);
        template<typename K>
        const int& at(const K& x) const;
        int& operator[](const std::string& key);
        int& operator[](std::string&& key);
        template<typename K>
        int& operator[](K&& x);
        size_t count(const std::string& key) const;
        Iter find(const std::string& key);
        constIter find(const std::string& key) const;
        bool contains(const std::string& key) const { return !(find(key) == end()); }
        std::pair<Iter, Iter> equal_range(const std::string& key);
        std::pair<constIter, constIter> equal_range(const std::string& key) const;

        std::forward_list<std::pair<const std::string, int>>::iterator begin(size_t n) { return bucket[n].begin();}
        std::forward_list<std::pair<const std::string, int>>::const_iterator begin(size_t n) const { return bucket[n].cbegin(); }
        std::forward_list<std::pair<const std::string, int>>::const_iterator cbegin(size_t n) const { return bucket[n].cbegin(); }
        
        std::forward_list<std::pair<const std::string, int>>::iterator end(size_t n) { return bucket[n].end();}
        std::forward_list<std::pair<const std::string, int>>::const_iterator end(size_t n) const { return bucket[n].cend(); }
        std::forward_list<std::pair<const std::string, int>>::const_iterator cend(size_t n) const { return bucket[n].cend(); }
         
        size_t bucket_count() const { return bucket.size(); }
        size_t max_bucket_count() const { return bucket.max_size(); }
        size_t bucket_size(size_t n) const;
        size_t _bucket(const std::string& key) const { return getIdx(key); }

        float load_factor() const;
        float _max_load_factor() { return max_load_factor; }
        void _max_load_factor(float ml) { max_load_factor = ml; }
        void rehash(size_t count);
        void reserve(size_t count);

        friend bool operator==(const HashMap& lhs, const HashMap& rhs);
        friend bool operator!=(const HashMap& lhs, const HashMap& rhs) { return !(lhs == rhs); }
}; 

template<typename InputIt>
HashMap::HashMap(InputIt first, InputIt last, size_t bucket_count) {
    max_load_factor = 1.0;
    element_count = 0;

    if (bucket_count <= std::distance(first,last)) bucket.resize(std::distance(first,last));
    else bucket.resize(bucket_count);
    
    for (; first != last; ++first) {
        insert(*first);
    }
}

HashMap::HashMap(std::initializer_list<std::pair<const std::string, int>> init, size_t bucket_count) {
    bucket.resize(std::max(init.size(), bucket_count));

    for (const auto& x : init) {
        insert(x);
    }
    
}

HashMap& HashMap::operator=(const HashMap& other) {
    if (this != &other) {
        HashMap tmp(other);

        std::swap(element_count, tmp.element_count);
        std::swap(max_load_factor, tmp.max_load_factor);
        std::swap(bucket, tmp.bucket);
    }
    return *this;
} 

HashMap& HashMap::operator=(HashMap&& other) noexcept {
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

HashMap& HashMap::operator=(std::initializer_list<std::pair<const std::string, int>> ilist) {
    *this = HashMap(ilist, ilist.size());
    return *this;
}

typename HashMap::Iter HashMap::begin() noexcept {
    for (size_t i{}; i < bucket.size(); ++i) {
        if (!bucket[i].empty()) return Iter(bucket[i].begin(), i, this);
    }

    return end();
}

typename HashMap::constIter HashMap::begin() const noexcept {
    return cbegin();
}

typename HashMap::constIter HashMap::cbegin() const noexcept {
    for (size_t i{}; i < bucket.size(); ++i) {
        if (!bucket[i].empty()) return constIter(bucket[i].cbegin(), i , this);
    }

    return cend();
}

typename HashMap::Iter HashMap::end() noexcept {
    return Iter(std::forward_list<std::pair<const std::string, int>>::iterator(), bucket.size(), this);
}

typename HashMap::constIter HashMap::end() const noexcept {
    return cend();
}

typename HashMap::constIter HashMap::cend() const noexcept {
    return constIter(std::forward_list<std::pair<const std::string, int>>::const_iterator(), bucket.size(), this);
}

void HashMap::clear() noexcept {
    for (int i{}; i < bucket.size(); ++i) bucket[i].clear();
    element_count = 0;
}

std::pair<typename HashMap::Iter, bool> HashMap::insert(const std::pair<const std::string, int>& value) {
    auto it = find(value.first);

    if (it != end()) return {it, false};
    if (static_cast<float>(element_count + 1) / bucket_count() >= max_load_factor) rehash(bucket.size() * 2);

    int idx = getIdx(value.first);
    bucket[idx].push_front(value);
    ++element_count;
    
    return {Iter(bucket[idx].begin(), idx, this), true};
}

std::pair<typename HashMap::Iter, bool> HashMap::insert(std::pair<const std::string, int>&& value) {
    auto it = find(value.first);

    if (it != end()) return {it, false};
    if (static_cast<float>(element_count + 1) / bucket_count() >= max_load_factor) rehash(bucket.size() * 2);

    int idx = getIdx(value.first);
    bucket[idx].push_front(std::move(value));
    ++element_count;
    
    return {Iter(bucket[idx].begin(), idx, this), true};
}

typename HashMap::Iter HashMap::insert(constIter hint, const std::pair<const std::string, int>& value) {
    int idx = getIdx(value.first);
    if (idx == hint.idx) {
        for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
            if (*it == value) { return Iter(it, idx, this); } 
        }
    }
    auto p = insert(value);
    return p.first;
}

typename HashMap::Iter HashMap::insert(constIter hint, std::pair<const std::string, int>&& value) {
    int idx = getIdx(value.first);
    if (idx == hint.idx) {
        for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
            if (*it == value) { return Iter(it, idx, this); } 
        }
    }
    auto p = insert(std::move(value));
    return p.first;
}

template<typename InputIt>
void HashMap::insert(InputIt first, InputIt last) {
    for (; first != last; ++first) insert(*first);
}

void HashMap::insert(std::initializer_list<std::pair<const std::string, int>> ilist) {
    for (const auto& x : ilist) insert(x);
}

template<typename M>
std::pair<HashMap::Iter, bool> HashMap::insert_or_assign(const std::string& k, M&& obj) {
    auto it = find(k);
    if (it != end()) {
        it->second = std::forward<M>(obj);
        return {it, false};
    }
    return insert({k, std::forward<M>(obj)});
}

template<typename M>
std::pair<HashMap::Iter, bool> HashMap::insert_or_assign(std::string&& k, M&& obj) {
    auto it = find(k);
    if (it != end()) {
        it->second = std::forward<M>(obj);
        return {it, false};
    }
    return insert({std::move(k), std::forward<M>(obj)});
}

template<typename K, typename M>
std::pair<HashMap::Iter, bool> HashMap::insert_or_assign(K&& k, M&& obj) {
    auto it = find(k);
    if (it != end()) {
        it->second = std::forward<M>(obj);
        return {it, false};
    }
    return insert({std::forward<K>(k), std::forward<M>(obj)});
}


template<typename M>
HashMap::Iter HashMap::insert_or_assign(constIter hint, const std::string& k, M&& obj) {
    auto it = find(k);
    if (it != end()) {
        it->second = std::forward<M>(obj);
        return it;
    }
    return insert(hint, {k, std::forward<M>(obj)}).first;
}

template<typename M>
HashMap::Iter HashMap::insert_or_assign(constIter hint, std::string&& k, M&& obj) {
    auto it = find(k);
    if (it != end()) {
        it->second = std::forward<M>(obj);
        return it;
    }
    return insert(hint, {std::move(k), std::forward<M>(obj)}).first;
}

template<typename K, typename M>
HashMap::Iter HashMap::insert_or_assign(constIter hint, K&& k, M&& obj) {
    auto it = find(k);
    if (it != end()) {
        it->second = std::forward<M>(obj);
        return it;
    }
    return insert(hint, {std::forward<K>(k), std::forward<M>(obj)}).first;
}

template<typename... Args >
std::pair<typename HashMap::Iter, bool> HashMap::emplace(Args&&... args) {
    std::pair<const std::string, int> value{std::forward<Args>(args)...};
    auto it = find(value.first);

    if (it != end()) return {it, false};
    if (static_cast<float>(element_count + 1) / bucket_count() >= max_load_factor) rehash(bucket.size() * 2);

    size_t idx = getIdx(value.first);
    bucket[idx].emplace_front(std::move(value));
    ++element_count;
    
    return {Iter(bucket[idx].begin(), idx, this), true};
}

template<typename... Args>
typename HashMap::Iter HashMap::emplace_hint(constIter hint, Args&&... args) {
    std::pair<const std::string, int>  value{std::forward<Args>(args)...};
    
    size_t idx = getIdx(value.first);
    if (idx == hint.idx) {
        for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
            if (it->first == value.first) { return Iter(it, idx, this); } 
        }
    }
    auto p = emplace(std::move(value));
    return p.first;
}

typename HashMap::Iter HashMap::erase(Iter pos) {
    std::forward_list<std::pair<const std::string, int>>::iterator next_it{bucket[pos.idx].end()};
    
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

typename HashMap::Iter HashMap::erase(constIter pos) {
    std::forward_list<std::pair<const std::string, int>>::iterator next_it{bucket[pos.idx].end()};
    
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

typename HashMap::Iter HashMap::erase(constIter first, constIter last) {    
    Iter it = end();
    while (first != last) {
        auto tmp = first;
        ++first;
        it = erase(tmp);
    }
    return it;
}

size_t HashMap::erase(const std::string& key) {
    size_t idx = getIdx(key);
    auto prev = bucket[idx].before_begin();

    for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it, ++prev) {
        if (it->first == key) {
            bucket[idx].erase_after(prev);
            --element_count;
            return 1;
        }
    }
    return 0;
}

void HashMap::merge(HashMap& other) {
    for (int i{}; i < other.bucket.size(); ++i) {
        auto& srcbucket = other.bucket[i];
        auto prev = srcbucket.before_begin();
        
        while (std::next(prev) != srcbucket.end()) {
            const std::string val = std::next(prev)->first;
            if (!this->contains(val)) {
                size_t idx = getIdx(val);
                bucket[idx].splice_after(bucket[idx].before_begin(), srcbucket, prev);
            } else ++prev;
        }
    }
}

int& HashMap::at(const std::string& key) {
    auto it = find(key); 
    if (it != end()) return it->second;
    else throw std::out_of_range("Key not found.");
}

const int& HashMap::at(const std::string& key) const {
    auto it = find(key); 
    if (it != end()) return it->second;
    else throw std::out_of_range("Key not found.");
}

template<typename K>
int& HashMap::at(const K& x) {
    auto it = find(x); 
    if (it != end()) return it->second;
    else throw std::out_of_range("Key not found.");
}

template<typename K>
const int& HashMap::at(const K& x) const {
    auto it = find(x); 
    if (it != end()) return it->second;
    else throw std::out_of_range("Key not found.");
}

int& HashMap::operator[](const std::string& key) {
    return insert({key, int{}}).first->second;    
}

int& HashMap::operator[](std::string&& key) {
    return insert({std::move(key), int{}}).first->second;    
}

template<typename K>
int& HashMap::operator[](K&& x) {
    return insert({std::forward<K>(x), int{}}).first->second;
}


size_t HashMap::count(const std::string& key) const {
    if (this->contains(key)) return 1;
    return 0;
}

typename HashMap::Iter HashMap::find(const std::string& key) {
    size_t idx = getIdx(key);
    
    for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
        if (it->first == key) return Iter{it, idx, this};
    }

    return end();
}
        
typename HashMap::constIter HashMap::find(const std::string& key) const {
    size_t idx = getIdx(key);
    
    for (auto it = bucket[idx].begin(); it != bucket[idx].end(); ++it) {
        if (it->first == key) return constIter{it, idx, this};
    }

    return cend();
}

std::pair<typename HashMap::Iter, typename HashMap::Iter> HashMap::equal_range(const std::string& key) {
    auto it = find(key);
    
    if (it != end()) { 
        auto first = it;
        auto second = ++it;
        return {first, second};
    } else return {end(), end()};
}

std::pair<typename HashMap::constIter, typename HashMap::constIter> HashMap::equal_range(const std::string& key) const {
    auto it = find(key);
    
    if (it != cend()) { 
        auto first = it;
        auto second = ++it;
        return {first, second};
    } else return {cend(), cend()};
}

size_t HashMap::bucket_size(size_t n) const {
    size_t count = 0;  
    for (auto it = begin(n); it != end(n); ++it) ++count;
    return count;
}

float HashMap::load_factor() const { 
    if (bucket_count() == 0) return 0.0f; 
    return static_cast<float>(element_count) / bucket_count();
}

void HashMap::rehash(size_t count) {
    std::vector<std::forward_list<std::pair<const std::string, int>>> tmp(std::move(bucket));
    bucket.assign(count, std::forward_list<std::pair<const std::string, int>>{});

    for (auto& x : tmp) {
        for (auto& y : x) {
            size_t idx = getIdx(y.first);
            bucket[idx].push_front(std::move(y));
        }
    }
}

void HashMap::reserve(size_t count) {   
    if (bucket.size() == 0) { 
        if (count) rehash(count);
        return;
    }
    if ((static_cast<float>(count)/ bucket.size()) >= max_load_factor) { 
        rehash(static_cast<int>(static_cast<float>(count) / max_load_factor) + 1);
    }
}

bool operator==(const HashMap& lhs, const HashMap& rhs) {
    if (lhs.size() != rhs.size()) return false;

    for (auto& x : lhs.bucket) {
        for (auto& kv : x) {
            if (!rhs.contains(kv.first)) return false;
        }
    }

    return true;
}


#endif