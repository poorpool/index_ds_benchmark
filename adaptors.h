#ifndef POORPOOL_ADAPTORS_H
#define POORPOOL_ADAPTORS_H
#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

template <typename K, typename V> class IndexDS {
public:
  virtual void Clear() = 0;
  virtual void Put(const K &key, const V &value) = 0;
  virtual void Erase(const K &key) = 0;
  virtual bool Get(const K &key, V &value) = 0;
  virtual bool IsSupportScan() = 0;
  virtual string GetName() = 0;
  virtual vector<pair<K, V>> Scan(const K &startKey, const K &endKey) {
    printf("This data structure does not support scan!\n");
    exit(-1);
  }
  virtual ~IndexDS() {}
};

template <typename K, typename V> class MapAdaptor : public IndexDS<K, V> {
private:
  map<K, V> mp;

public:
  virtual void Clear() { mp.clear(); }
  virtual void Put(const K &key, const V &value) { mp[key] = value; }
  virtual bool Get(const K &key, V &value) {
    auto it = mp.find(key);
    if (it == mp.end()) {
      return false;
    }
    value = it->second;
    return true;
  }
  virtual void Erase(const K &key) { mp.erase(key); }
  virtual bool IsSupportScan() { return true; }
  virtual string GetName() { return "std::map"; }
  virtual vector<pair<K, V>> Scan(const K &startKey, const K &endKey) {
    vector<pair<K, V>> ans;
    for (auto it = mp.find(startKey); it != mp.end() && it->first < endKey;
         it++) {
      ans.push_back({it->first, it->second});
    }
    return ans;
  }
  virtual ~MapAdaptor() {}
};

template <typename K, typename V>
class UnorderedMapAdaptor : public IndexDS<K, V> {
private:
  unordered_map<K, V> mp;

public:
  virtual void Clear() { mp.clear(); }
  virtual void Put(const K &key, const V &value) { mp[key] = value; }
  virtual bool Get(const K &key, V &value) {
    auto it = mp.find(key);
    if (it == mp.end()) {
      return false;
    }
    value = it->second;
    return true;
  }
  virtual void Erase(const K &key) { mp.erase(key); }
  virtual bool IsSupportScan() { return false; }
  virtual string GetName() { return "std::unordered_map"; }
  virtual ~UnorderedMapAdaptor() {}
};

#endif // POORPOOL_ADAPTORS_H