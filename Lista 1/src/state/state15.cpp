#ifndef STATE16_H
#define STATE16_H

#include <memory>
#include <utility>
#include <vector>
#include "node.cpp"

class State15 {
public:
    State15(std::vector<int> state);

    std::shared_ptr<Node> init();

    static bool is_goal(std::shared_ptr<Node> node);

    std::vector<std::shared_ptr<Node>> succ(std::shared_ptr<Node>);

    int cost(int action);

    int h_value(std::shared_ptr<Node> node);

    std::vector<std::shared_ptr<Node>> extract_path(std::shared_ptr<Node> node);

private:
    std::vector<int> s_state;

    bool is_parent_state(std::vector<int> n_state, std::shared_ptr<Node> node);

};

State15::State15(std::vector<int> state) {
    this->s_state = std::move(state);
}

std::shared_ptr<Node> State15::init() {
    return std::make_shared<Node>(s_state, nullptr, 0, -1);
}

bool State15::is_goal(std::shared_ptr<Node> node) {
    for (int i = 0; i < 16; ++i) {
        if (node->state[i] != i) return false;
    }
    return true;
}

std::vector<std::shared_ptr<Node>> State15::succ(std::shared_ptr<Node> node) {
    std::vector<std::shared_ptr<Node>> ret;
    int idx = -1;
    for (long unsigned int i = 0; i < node->state.size(); ++i) {
        if (node->state[i] == 0) idx = (int) i;
    }

    if (idx / 4 > 0) {
        std::vector<int> n_state = node->state;
        n_state[idx] = n_state[idx - 4];
        n_state[idx - 4] = 0;
        if (!is_parent_state(n_state, node))
            ret.emplace_back(std::make_shared<Node>(n_state,
                                                    node,
                                                    node->path_cost + cost(Action::up),
                                                    Action::up));
    }

    if (idx % 4 > 0) {
        std::vector<int> n_state = node->state;
        n_state[idx] = n_state[idx - 1];
        n_state[idx - 1] = 0;
        if (!is_parent_state(n_state, node))
            ret.emplace_back(std::make_shared<Node>(n_state,
                                                    node,
                                                    node->path_cost + cost(Action::left),
                                                    Action::left));
    }

    if (idx % 4 < 3) {
        std::vector<int> n_state = node->state;
        n_state[idx] = n_state[idx + 1];
        n_state[idx + 1] = 0;
        if (!is_parent_state(n_state, node))
            ret.emplace_back(std::make_shared<Node>(n_state,
                                                    node,
                                                    node->path_cost + cost(Action::right),
                                                    Action::right));
    }

    if (idx / 4 < 3) {
        std::vector<int> n_state = node->state;
        n_state[idx] = n_state[idx + 4];
        n_state[idx + 4] = 0;
        if (!is_parent_state(n_state, node))
            ret.emplace_back(std::make_shared<Node>(n_state,
                                                    node,
                                                    node->path_cost + cost(Action::down),
                                                    Action::down));
    }

    return ret;
}

bool State15::is_parent_state(std::vector<int> n_state, std::shared_ptr<Node> node) {
    if (node->parent == nullptr) return false;
    for (unsigned int i = 0; i < n_state.size(); ++i) {
        if (n_state[i] != node->parent->state[i]) return false;
    }
    return true;
}

int State15::cost(int action) {
    return 1;
}

int State15::h_value(std::shared_ptr<Node> node) {
    int h = 0;
    for (int i = 0; i < (int) node->state.size(); ++i) {
        if (node->state[i] == 0) continue;
        int cidx = node->state[i];
        h += abs(i % 4 - cidx % 4) + abs(i / 4 - cidx / 4);
    }
    return h;
}

std::vector<std::shared_ptr<Node>> State15::extract_path(std::shared_ptr<Node> node) {
    std::vector<std::shared_ptr<Node>> ret;
    while (true) {
        ret.push_back(node);
        if (node->parent == nullptr)
            break;
        else
            node = node->parent;
    }
    return ret;
}

#endif
