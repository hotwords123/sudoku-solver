#include "DancingLink.h"

using node_ptr = DancingLink::node_ptr;

DancingLink::DancingLink() {
    pivot = new Node(-1, -1);
    pivot->init(&Node::h);
}

DancingLink::~DancingLink() {
    destroy();
    delete pivot;
}

void DancingLink::build(int n_cols) {
    destroy();

    col_heads.reserve(n_cols);
    for (int i = 0; i < n_cols; i++) {
        Node *head = new Node(-1, i);
        head->init(&Node::v);
        head->insertTo(&Node::h, pivot);
        col_heads.push_back(head);
    }

    col_sizes.assign(n_cols, 0);
}

node_ptr DancingLink::addNode(node_ptr where, int col) {
    Node *node;
    if (where == nullptr) {
        node = new Node(n_rows++, col);
        node->init(&Node::h);
    } else {
        node = new Node(where->row, col);
        node->insertTo(&Node::h, where);
    }

    node->insertTo(&Node::v, col_heads[col]);
    col_sizes[col]++;
    return node;
}

void DancingLink::destroy() {
    for (Node *head : col_heads) {
        for (Node *node = head->v.next; node != head; ) {
            Node *next = node->v.next;
            delete node;
            node = next;
        }
        delete head;
    }

    n_rows = 0;
    pivot->init(&Node::h);
    col_heads.clear();
    col_sizes.clear();
}

void DancingLink::cover(int col) {
    col_heads[col]->removeFrom(&Node::h);

    for (Node *row : rows(col)) {
        for (Node *cell : cells(row)) {
            cell->removeFrom(&Node::v);
            col_sizes[cell->col]--;
        }
    }
}

void DancingLink::uncover(int col) {
    for (Node *row : rows<kBackward>(col)) {
        for (Node *cell : cells<kBackward>(row)) {
            col_sizes[cell->col]++;
            cell->restoreTo(&Node::v);
        }
    }

    col_heads[col]->restoreTo(&Node::h);
}

void DancingLink::coverRow(node_ptr row) {
    for (Node *cell : cells(row)) {
        cell->removeFrom(&Node::v);
        col_sizes[cell->col]--;
    }

    row->removeFrom(&Node::v);
    col_sizes[row->col]--;
}

void DancingLink::uncoverRow(node_ptr row) {
    col_sizes[row->col]++;
    row->restoreTo(&Node::v);

    for (Node *cell : cells<kBackward>(row)) {
        col_sizes[cell->col]++;
        cell->restoreTo(&Node::v);
    }
}
