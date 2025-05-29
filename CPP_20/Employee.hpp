//
// Created by 小雨 on 2024/11/23.
//

#ifndef CPP_20_EMPLOYEE_HPP
#define CPP_20_EMPLOYEE_HPP

#include <string>

namespace Records {
    const int DefaultStartingSalary {30000};
    const int DefaultRaiseAndDemeritAmount {1000};
    class Employee {
    public:
//       "pass by value and use std::move" is better than "pass by const reference".
//        Employee(const std::string&, const std::string&);
        Employee(std::string, std::string);
        void promote(int raiseAmount = DefaultRaiseAndDemeritAmount);
        void demote(int demeritAmount = DefaultRaiseAndDemeritAmount);
        void hire();
        void fire();
        void display() const;

        void setFirstName(const std::string&);
        const std::string& getFirstName() const;

        void setLastName(const std::string&);
        const std::string& getLastName() const;

        void setEmployeeNumber(int);
        int getEmployeeNumber() const;

        void setSalary(int);
        int getSalary() const;

        bool isHired() const;

    private:
        std::string m_firstName;
        std::string m_lastName;
        int m_employeeNumber {-1};
        int m_salary {DefaultStartingSalary};
        bool m_hired {false};
    };
}

#endif //CPP_20_EMPLOYEE_HPP
