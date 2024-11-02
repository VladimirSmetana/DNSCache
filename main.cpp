#include <iostream>
#include <unordered_map>
#include <list>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <string>

#include "dns_cache.h"

std::mutex cout_mutex;

void test_DNS_Cache(DNS_Cache& dns_cache, const std::string& name, const std::string& ip) 
{
    dns_cache.update(name, ip);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock(cout_mutex); //  для наглядного вывода 
    if (dns_cache.resolve(name).empty())
    {
        std::cout << "Добавлено " << name << ": " << "значение заменено" << std::endl;
    } 
    else
    {
        std::cout << "Добавлено " << name << ": " << dns_cache.resolve(name) << std::endl;
    }
}

int main() {
    const size_t capacity = 3; // максимальное количество выводимых значений
    DNS_Cache dns_cache(capacity);
    std::unordered_map<std::string, std::string> test_map;
    std::vector<std::thread> tr;

    // тестовая выборка
    test_map["first.com"] = "93.184.216.34";
    test_map["second.org"] = "93.184.216.35";
    test_map["third.net"] = "93.184.216.36";
    test_map["fourth.com"] = "93.184.216.34";
    test_map["fifth.org"] = "93.184.216.35";
    test_map["sexth.net"] = "93.184.216.36";

    for (auto it = test_map.cbegin(); it != test_map.cend(); ++it)
    {
        tr.emplace_back(test_DNS_Cache, std::ref(dns_cache), it->first, it->second);
    }

    for (auto& tr_:tr)
    {
        tr_.join();
    }

    return 0;
}
