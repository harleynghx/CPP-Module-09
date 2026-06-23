#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <stack>
#include <list>

class RPN {
private:
    std::stack<int, std::list<int> > _stack;

public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();

    void evaluate(const std::string& expression);
};

#endif