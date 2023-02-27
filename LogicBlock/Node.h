//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_NODE_H
#define LOGIC_SIM_NODE_H

class Node{
public:
    void ChangeState();
    [[nodiscard]] bool State() const;
protected:
    bool _state;
};

#endif //LOGIC_SIM_NODE_H
