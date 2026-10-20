#include "SortedList.h"
#include "Task.h"
#include "Person.h"
#include "TaskManager.h"

#include <climits>
#include <sstream>

#include <iostream>
#include <stdexcept>

struct Tracked {
    static int live;
    static int copiesUntilThrow;
    static bool throwOnCompare;

    int value;

    explicit Tracked(int number) : value(number) {
        ++live;
    }

    Tracked(const Tracked& other) : value(other.value) {
        if (copiesUntilThrow == 0) {
            throw std::runtime_error("copy failed");
        }
        if (copiesUntilThrow > 0) {
            --copiesUntilThrow;
        }
        ++live;
    }

    ~Tracked() {
        --live;
    }

    bool operator>(const Tracked& other) const {
        if (throwOnCompare) {
            throw std::runtime_error("comparison failed");
        }
        return value > other.value;
    }
};

int Tracked::live = 0;
int Tracked::copiesUntilThrow = -1;
bool Tracked::throwOnCompare = false;

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void testCopyFailure() {
    mtm::SortedList<Tracked> list;
    list.insert(Tracked(1));
    list.insert(Tracked(2));
    require(Tracked::live == 2, "unexpected live element count");

    Tracked::copiesUntilThrow = 1;
    bool caught = false;
    try {
        mtm::SortedList<Tracked> copy(list);
    } catch (const std::runtime_error&) {
        caught = true;
    }
    Tracked::copiesUntilThrow = -1;

    require(caught, "copy should have thrown");
    require(Tracked::live == 2, "copy failure leaked an element");
    require(list.length() == 2, "source list changed after copy failure");
}

void testComparisonFailure() {
    mtm::SortedList<Tracked> list;
    list.insert(Tracked(1));
    list.insert(Tracked(2));
    require(Tracked::live == 2, "unexpected live element count");

    Tracked::throwOnCompare = true;
    bool caught = false;
    try {
        list.insert(Tracked(3));
    } catch (const std::runtime_error&) {
        caught = true;
    }
    Tracked::throwOnCompare = false;

    require(caught, "comparison should have thrown");
    require(Tracked::live == 2, "comparison failure leaked an element");
    require(list.length() == 2, "list changed after comparison failure");
}

void testUnassignedTask() {
    Task task(5, TaskType::General, "Example");
    require(task.getId() == -1, "unassigned task must have ID -1");
}

void testSortedOrder() {
    mtm::SortedList<int> list;
    list.insert(3);
    list.insert(1);
    list.insert(5);
    list.insert(3);

    const int expected[] = {5, 3, 3, 1};
    int index = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        require(index < 4 && *it == expected[index], "list is not sorted");
        ++index;
    }
    require(index == 4 && list.length() == 4, "unexpected list length");

    list.remove(list.begin());
    auto middle = list.begin();
    ++middle;
    list.remove(middle);
    require(list.length() == 2, "removal failed");
    require(*list.begin() == 3, "wrong element after removal");
}

void testTaskPriority() {
    Person person("Alice");
    Task low(20, TaskType::General, "Low priority");
    Task high(80, TaskType::General, "High priority");
    low.setId(1);
    high.setId(2);

    person.assignTask(low);
    person.assignTask(high);
    require(person.getHighestPriorityTask().getId() == 2, "wrong first task");
    require(person.completeTask() == 2, "high-priority task was not completed first");
    require(person.completeTask() == 1, "low-priority task was not completed second");
}

void testLargePriorityBump() {
    TaskManager manager;
    manager.assignTask("Alice", Task(50, TaskType::General, "Example"));
    manager.bumpPriorityByType(TaskType::General, INT_MAX);

    std::ostringstream output;
    std::streambuf* original = std::cout.rdbuf(output.rdbuf());
    try {
        manager.printAllTasks();
    } catch (...) {
        std::cout.rdbuf(original);
        throw;
    }
    std::cout.rdbuf(original);
    require(output.str().find("Priority: 100") != std::string::npos,
            "large priority increase was not capped at 100");
}

int main() {
    try {
        testCopyFailure();
        require(Tracked::live == 0, "element leak after copy test");
        testComparisonFailure();
        require(Tracked::live == 0, "element leak after comparison test");
        testUnassignedTask();
        testSortedOrder();
        testTaskPriority();
        testLargePriorityBump();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    std::cout << "Regression tests passed\n";
    return 0;
}
