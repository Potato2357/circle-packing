#include <string>
using namespace std;

class Node {
public:
    double coords[4];
    int branch_num;
    Node **children;
    bool *child_status;

    Node(double b_bar, double b, double h_1, double h_2, int branch_num);
    ~Node();
    void add_child(Node *child, int branch_no);
    string print_data();
    double get_curvature();
    bool is_leaf();
};