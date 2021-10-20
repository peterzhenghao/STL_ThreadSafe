#pragma once

#include <set>
#include <mutex>


template <
    typename Key, 
    typename Compare = std::less<Key>, 
    typename Allocator = std::allocator<Key>>
class CSafeMultiset final
{      
public:
    using SafeMultisetType = std::multiset<Key, Compare, Allocator>;
    using size_type = typename SafeMultisetType::size_type;
    using value_type = Key;
    using iterator = typename SafeMultisetType::iterator;
    using const_iterator = typename SafeMultisetType::const_iterator         ;
    
public:
    CSafeMultiset() : SafeMultiset() {}
    ~CSafeMultiset() { clear(); }// 调用容器的clear还是自己封装的clear函数
    CSafeMultiset(const SafeMultisetType& __s) : SafeMultiset(__s) {}
    CSafeMultiset(SafeMultisetType&& __s) : SafeMultiset(std::move(__s)) {}
    
    CSafeMultiset(const CSafeMultiset&) = delete;
    CSafeMultiset& operator=(const CSafeMultiset&) = delete;
    CSafeMultiset(CSafeMultiset&&) = delete;
    CSafeMultiset& operator=(CSafeMultiset&&) = delete;

    bool empty() const
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.empty();
    }
    
    size_type size() const
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.size();
    }

    size_type count(const Key& key) const
    { 
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.count(key); 
    }
    
    void operator=(const SafeMultisetType& __v)
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        SafeMultiset = __v;
    }

    // 迭代器相关
    iterator begin() noexcept
    { 
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.begin(); 
    }

    iterator end() noexcept
    { 
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.end();
    }
    
    //增加一个元素
    void insert(const Key& __v)
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        SafeMultiset.insert(__v);
    }
    
    //删除全部元素
    void clear()
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        SafeMultiset.clear();
    }
    
    //删除多个值为key的元素
    size_type erase(const Key& key)
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.erase(key);
    }

    //删除第一个值为*ite的元素
    iterator erase(const_iterator inputIterator) noexcept
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.erase(inputIterator);
    }


    //查 只会查找到第一个
    iterator find(const Key& key)
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.find(key);
    }

    iterator lower_bound(const Key& key)       
    { 
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.lower_bound(key); 
    }

    iterator upper_bound(const Key& key)       
    { 
        std::lock_guard<std::mutex> lock_guard(m_lock);
        return SafeMultiset.upper_bound(key); 
    }

private: 
    SafeMultisetType SafeMultiset;
    mutable std::mutex m_lock;   
};
