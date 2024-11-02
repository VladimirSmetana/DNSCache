#include "dns_cache.h"

void DNS_Cache::update(const std::string &name, const std::string &ip)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = cache_map_.find(name);
    if (it != cache_map_.end())
    {
        it->second->ip = ip;
        cache_list_.splice(cache_list_.end(), cache_list_, it->second->iterator);
        it->second->iterator = std::prev(cache_list_.end());
        return;
    }

    if (cache_map_.size() >= capacity_)
    {
        auto oldest = cache_list_.front();
        cache_map_.erase(oldest.name);
        cache_list_.pop_front();
    }

    cache_list_.emplace_back(name, ip);
    auto new_node = std::prev(cache_list_.end());
    cache_map_[name] = &(*new_node);
    cache_map_[name]->iterator = new_node;
}

std::string DNS_Cache::resolve(const std::string &name) const
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = cache_map_.find(name);
    if (it != cache_map_.end())
    {
        return it->second->ip;
    }
    return std::string();
}
