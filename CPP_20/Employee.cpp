//
// Created by 小雨 on 2024/11/23.
//

#include "Employee.hpp"
#include <iostream>
#include <format>

using namespace std;


namespace Records {
    Employee::Employee(string firstName, string lastName) : m_firstName { std::move(firstName) },
                                                            m_lastName { std::move(lastName) } {}

    void Employee::promote(int raiseAmount) { setSalary(getSalary() + raiseAmount); }

    void Employee::demote(int demeritAmount) { setSalary(getSalary() - demeritAmount); }

    void Employee::hire() { m_hired = true; }

    void Employee::fire() { m_hired = false; }

    void Employee::display() const {
        cout << format("Employee {}, {}", getLastName(), getFirstName()) << endl;
        cout << "-----------------------------" << endl;
        cout << (isHired() ? "Current Employee" : "Former Employee") << endl;
        cout << format("Employee Number: {}", getEmployeeNumber()) << endl;
        cout << format("Salary: ${}", getSalary()) << endl;
        cout << endl;
    }

    void Employee::setFirstName(const string& firstName) { m_firstName = firstName; }

    const string& Employee::getFirstName() const { return m_firstName; }

    void Employee::setLastName(const string& lastName) { m_lastName = lastName; }

    const string& Employee::getLastName() const { return m_lastName; }

    void Employee::setEmployeeNumber(int employeeNumber) { m_employeeNumber = employeeNumber; }

    int Employee::getEmployeeNumber() const { return m_employeeNumber; }

    void Employee::setSalary(int salary) { m_salary = salary; }

    int Employee::getSalary() const { return m_salary; };

    bool Employee::isHired() const { return m_hired; }



}
