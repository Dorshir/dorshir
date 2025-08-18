#include "hash_table.hpp"

#include <functional>
#include <optional>
#include <vector>
#include <list>
#include <mutex>
#include <stdexcept>


namespace mt {


template <typename K, typename V>
HashTable<K, V>::HashTable(size_t capacity, HashFunction const& hash_func, EqualityFunction const& eq_func)
: m_backets(capacity)
, m_capacity{capacity}
, m_num_of_items{0}
, m_hash_func{hash_func}
, m_eq_func{eq_func}
, m_mutex{}
, m_locker{m_mutex}
{}


template <typename K, typename V>
V& HashTable<K, V>::look_up(K const& key) const
{
    size_t hash_index = m_hash_func(key) % m_capacity;
    Locker lock(m_mutex);

    for (auto const& kv : m_backets[hash_index]) {
        if (m_eq_func(kv.key, key)) {
            return kv.value;
        }
    }

    throw std::runtime_error("Key not found");
}


template <typename K, typename V>
TableResult HashTable<K, V>::insert(K const& key, V const& value)
{
    size_t hash_index = m_hash_func(key) % m_capacity;

    Locker lock(m_mutex);

    for (auto const& kv : m_backets[hash_index]) {
        if (m_eq_func(kv.key, key)) {
            return MapResult::MAP_DUP_KEY;
        }
    }

    m_backets[hash_index].emplace_back(key, value);
    ++m_num_of_items;


    return MapResult::MAP_SUCCESS;
}


template <typename K, typename V>
std::optional<KeyValue<K, V>> HashTable<K, V>::remove(K const& key)
{
    size_t hash_index = m_hash_func(key) % m_capacity;
    auto& list = m_backets[hash_index];

    std::optional<KeyValue<K, V>> removed_kv = std::nullopt;
    
    Locker lock(m_mutex);

    auto const begin = list.begin();
    auto const end = list.end();

    for (auto itr = begin; itr != end; ++itr) {
        if (m_eq_func(itr->key, key)) {
            removed_kv = *itr;
            list.erase(itr);

            --m_num_of_items;
            break;
        }
    }

    return removed_kv;
}

} // namespace mt