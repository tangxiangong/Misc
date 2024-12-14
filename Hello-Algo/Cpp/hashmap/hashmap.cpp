#include <vector>
#include <tuple>

using std::vector;
using std::tuple;

template<typename K, typename V>
class HashMap {
private:
    size_t m_size;
    size_t m_capacity;
    double m_loadThres;
    size_t m_extendRatio;
    vector<vector<tuple<K, V>>> m_buckets;

public:
    HashMap();
    size_t calculate_index(K);
};