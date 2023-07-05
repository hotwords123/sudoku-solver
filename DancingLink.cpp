#include "DancingLink.h"
#include <iostream>

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

void DancingLink::addNode(int col, bool new_row) {
    Node *node;
    if (new_row) {
        node = new Node(row_heads.size(), col);
        node->init(&Node::h);
        row_heads.push_back(node);
    } else {
        Node *head = row_heads.back();
        node = new Node(head->row, col);
        node->insertTo(&Node::h, head);
    }

    node->insertTo(&Node::v, col_heads[col]);
    col_sizes[col]++;
}

void DancingLink::destroy() {
    for (Node *head : row_heads) {
        for (Node *node = head->h.next; node != head; ) {
            Node *next = node->h.next;
            delete node;
            node = next;
        }
        delete head;
    }

    for (Node *head : col_heads) {
        delete head;
    }

    pivot->init(&Node::h);
    row_heads.clear();
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

void DancingLink::removeRow(node_ptr row) {
    for (Node *cell : cells(row)) {
        cell->removeFrom(&Node::v);
        col_sizes[cell->col]--;
    }

    row->removeFrom(&Node::v);
    col_sizes[row->col]--;
}

void DancingLink::restoreRow(node_ptr row) {
    col_sizes[row->col]++;
    row->restoreTo(&Node::v);

    for (Node *cell : cells<kBackward>(row)) {
        col_sizes[cell->col]++;
        cell->restoreTo(&Node::v);
    }
}
