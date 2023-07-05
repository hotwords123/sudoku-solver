#pragma once

#include <vector>

class DancingLink {
    class Node {
        struct Link {
            Node *prev, *next;

            Link() : prev(), next() {}
            Link(Node *prev, Node *next) : prev(prev), next(next) {}
        };

        using axis_t = Link Node::*;
        using direction_t = Node *Link::*;

        template <axis_t axis, direction_t dir>
        class AxisIterator {
        public:
            AxisIterator() : node() {}
            explicit AxisIterator(Node *node) : node(node) {}

            Node *operator*() const { return node; }
            Node *operator->() const { return node; }
            AxisIterator &operator++() { node = node->*axis.*dir; return *this; }
            bool operator==(const AxisIterator &other) const { return node == other.node; }
            bool operator!=(const AxisIterator &other) const { return node != other.node; }

        private:
            Node *node;
        };

        template <axis_t axis, direction_t dir>
        class AxisLink {
        public:
            explicit AxisLink(Node *head) : head(head) {}

            using iterator = AxisIterator<axis, dir>;

            iterator begin() const { return iterator(head->*axis.*dir); }
            iterator end() const { return iterator(head); }

        private:
            Node *head;
        };

        friend class DancingLink;

    public:
        int rowIndex() const { return row; }
        int columnIndex() const { return col; }

    private:
        Link h, v;
        int row, col;

        Node(int row, int col) : row(row), col(col) {}

        Node *&prev(axis_t axis) { return (this->*axis).prev; }
        Node *&next(axis_t axis) { return (this->*axis).next; }

        /**
         * Initializes the given axis to a single node.
         * @param axis the axis to initialize
         */
        void init(axis_t axis) {
            this->*axis = Link(this, this);
        }

        /**
         * Inserts the given node to the given axis.
         * @param axis the axis to insert to
         * @param where the node to insert before
         */
        void insertTo(axis_t axis, Node *where) {
            Node *prev = where->prev(axis);
            this->*axis = Link(prev, where);
            prev->next(axis) = where->prev(axis) = this;
        }

        /**
         * Removes this node from the given axis.
         * @param axis the axis to remove from
         */
        void removeFrom(axis_t axis) {
            prev(axis)->next(axis) = next(axis);
            next(axis)->prev(axis) = prev(axis);
        }

        /**
         * Restores this node to the given axis.
         * @param axis the axis to restore to
         */
        void restoreTo(axis_t axis) {
            prev(axis)->next(axis) = next(axis)->prev(axis) = this;
        }

        template <axis_t axis, direction_t dir>
        AxisLink<axis, dir> enumerate() {
            return AxisLink<axis, dir>(this);
        }
    };

public:
    using node_ptr = Node *;

    static constexpr Node::direction_t kForward = &Node::Link::next;
    static constexpr Node::direction_t kBackward = &Node::Link::prev;

    DancingLink();
    ~DancingLink();

    /**
     * Builds the dancing link structure.
     * @param n_cols the number of columns
     */
    void build(int n_cols);

    /**
     * Adds a node to the given row.
     * @param col the column of the new node
     * @param new_row whether to create a new row
     */
    void addNode(int col, bool new_row = false);

    void destroy();

    /**
     * Marks the given column as covered, removing all related rows.
     * @param col the column to cover
     */
    void cover(int col);

    /**
     * Unmarks the given column as covered, restoring all related rows.
     * This operation must be performed in the reverse order of `cover`.
     * @param col the column to uncover
     */
    void uncover(int col);

    /**
     * Marks the given row as removed, removing all related cells.
     * @param row the row to remove
     */
    void removeRow(node_ptr row);

    /**
     * Unmarks the given row as removed, restoring all related cells.
     * This operation must be performed in the reverse order of `removeRow`.
     * @param row the row to restore
     */
    void restoreRow(node_ptr row);

    /**
     * Returns the number of rows in the given column.
     * @param col the column to query
     * @return the number of rows in the column
     */
    int columnSize(int col) const { return col_sizes[col]; }

    /**
     * Enumerates all columns.
     * @return the columns
     */
    template <Node::direction_t dir = kForward>
    Node::AxisLink<&Node::h, dir> columns() {
        return pivot->enumerate<&Node::h, dir>();
    }

    /**
     * Enumerates all rows in the given column.
     * @param col the column to enumerate
     * @return the rows in the column
     */
    template <Node::direction_t dir = kForward>
    Node::AxisLink<&Node::v, dir> rows(int col) {
        return col_heads[col]->enumerate<&Node::v, dir>();
    }

    /**
     * Enumerates all cells in the given row.
     * @param row the row to enumerate
     * @return the cells in the row
     */
    template <Node::direction_t dir = kForward>
    Node::AxisLink<&Node::h, dir> cells(node_ptr row) {
        return row->enumerate<&Node::h, dir>();
    }

private:
    Node *pivot;
    std::vector<Node *> row_heads;
    std::vector<Node *> col_heads;
    std::vector<int> col_sizes;
};
