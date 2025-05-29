//
// Created by 小雨 on 2024/11/24.
//

#ifndef CPP_20_DATABASE_HPP
#define CPP_20_DATABASE_HPP
#include <string>
#include <vector>
#include "Employee.hpp"

namespace Records {
    const int FirstEmployeeNumber {1000};
    class Database {
    public:
        Employee& addEmployee(const std::string&, const std::string&);
        Employee& getEmployee(int);
        Employee& getEmployee(const std::string&, const std::string&);

        void displayAll() const;
        void displayCurrent() const;
        void displayFormer() const;
    private:
        std::vector<Employee> m_employees;
        int m_nextEmployeeNumber { FirstEmployeeNumber };
    };
}

#endif //CPP_20_DATABASE_HPP
