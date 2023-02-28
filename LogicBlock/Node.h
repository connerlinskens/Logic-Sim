//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_NODE_H
#define LOGIC_SIM_NODE_H

class Node{
public:
    explicit Node(bool manuallyOverridable = false);
    void ChangeState();
    [[nodiscard]] bool State() const;

    [[nodiscard]] bool IsManuallyOverridable() const;
    void SetManuallyOverridableState(bool state);
protected:
    bool _state;
    bool _manuallyOverridable;
};

#endif //LOGIC_SIM_NODE_H
