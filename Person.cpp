
#include "Person.h"
using std::endl;

Person::Person(const string &name) : m_name(name) {}

string Person::getName() const {
    return m_name;
}

const SortedList<Task>& Person::getTasks() const {
    return m_tasks;
}

void Person::setTasks(const SortedList<Task>& tasks) {
    m_tasks = tasks;
}

void Person::assignTask(const Task& task) {
    m_tasks.insert(task);
}


int Person::completeTask() {
    if (m_tasks.length() == 0) {
        throw std::runtime_error("No tasks assigned to this person.");
    }
    int taskId = (*m_tasks.begin()).getId();
    m_tasks.remove(m_tasks.begin());
    return taskId;
}

const Task& Person::getHighestPriorityTask() const {
    if (m_tasks.length() == 0) {
        throw std::runtime_error("No tasks assigned to this person.");
    }
    return (*m_tasks.begin());
}

ostream& operator<<(ostream& os, const Person& person) {
    os << "Person: " << person.m_name << endl;
    for (const Task& t: person.m_tasks) {
        os << t << endl;
    }
    return os;
}
