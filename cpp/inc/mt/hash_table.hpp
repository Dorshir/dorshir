#ifndef MT_HASH_TABLE_HPP
#define MT_HASH_TABLE_HPP

#include <functional>
#include <optional>
#include <vector>
#include <list>
#include <mutex>

namespace mt {

enum class TableResult {
  MAP_DUP_KEY,
  MAP_SUCCESS
};

struct Locker {
  
public:

  Locker(std::mutex& mu)
  : mutex{mu}
  {
      mutex.lock();
  }

  ~Locker()
  {
      mutex.unlock();
  }

private:

  std::mutex& mutex;
};

template <typename K, typename V>
struct KeyValue {
  K const key;
  V value;

  KeyValue(K const& key, V const& value)
  : key(key)
  , value(value)
  {}
};

template <typename K, typename V>
class HashTable {

public:

  using HashFunction = std::function<size_t(K const&)>;
  using EqualityFunction = std::function<bool(K const&, K const&)>;
  

  explicit HashTable(size_t capacity, HashFunction const& hash_func, EqualityFunction const& eq_func);
  ~HashTable() = default;
  HashTable(HashTable const&) = delete;
  HashTable& operator=(HashTable const&) = delete;
  HashTable(HashTable&&) noexcept = delete;
  HashTable& operator=(HashTable&&) noexcept = delete;

public:

  V& look_up(K const& key) const;
  TableResult insert(K const& key, V const& value);
  std::optional<KeyValue<K, V>> remove(K const& key);

private:

  std::vector<std::list<KeyValue<K, V>>> m_backets;
  size_t m_capacity;
  size_t m_num_of_items;
  HashFunction m_hash_func;
  EqualityFunction m_eq_func;
  mutable std::mutex m_mutex;

};


}  // namespace mt

#include "hash_table.txx"

#endif  // MT_HASH_TABLE_HPP