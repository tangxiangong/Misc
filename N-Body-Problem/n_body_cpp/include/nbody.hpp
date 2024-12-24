#include <cmath>
#include <array>
#include <vector>
#include <numbers>
#include <utility>

using std::array;
using std::pair;
using std::vector;
using std::numbers::pi;

struct Planet {
    array<double, 3> position;
    array<double, 3> velocity;
    double mass;
};

void offset_momentum(vector<Planet>&);

double energe(const vector<Planet>&);

void advance(vector<Planet>&, double);

pair<double, double> nbody(size_t);