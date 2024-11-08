#include <unordered_map>
#include <list>
#include <mutex>
#include <string>

class DNS_Cache
{
public:
    explicit DNS_Cache(size_t capacity) : capacity_(capacity) {} 

    void update(const std::string &name, const std::string &ip); // Cредняя сложность O(1) для unordered_map и O(1) для list, итого O(1)
    std::string resolve(const std::string &name) const; // Сложность O(1)

private:
    struct CacheNode
    {
        std::string name;
        std::string ip;
        std::list<CacheNode>::iterator iterator;
        CacheNode(const std::string &n, const std::string &i) : name(n), ip(i) {}
    };

    size_t capacity_;
    mutable std::mutex mutex_;
    std::list<CacheNode> cache_list_;
    std::unordered_map<std::string, CacheNode *> cache_map_;
};