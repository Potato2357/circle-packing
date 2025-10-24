#include "Node.h"

Node::Node(double b_bar, double b, double h_1, double h_2, int branch_num){
    coords[0] = b_bar;
    coords[1] = b;
    coords[2] = h_1;
    coords[3] = h_2;
    this->branch_num = branch_num;
    children = new Node *[branch_num];
    child_status = new bool[branch_num];
    for (int i=0;i<branch_num;i++){
        child_status[i] = false;
    }
}
Node::~Node(){
    for (int i=0;i<branch_num;i++){
        if (child_status[i]){
            delete children[i];
            children[i] = nullptr;
        }
    }
    delete [] children;
    delete [] child_status;
    children = nullptr;
}
void Node::add_child(Node *child, int branch_no){
    children[branch_no] = child;
    child_status[branch_no] = true;
}
string Node::print_data(){
    return to_string(coords[0]).substr(0,5)+" "+to_string(coords[1]).substr(0,5)+" "+to_string(coords[2]).substr(0,5)+" "+to_string(coords[3]).substr(0,5);
}

double Node::get_curvature(){
    return coords[1];
}

bool Node::is_leaf(){
    for (int i=0;i<branch_num;i++){
        if (child_status[i]){
            return false;
        }
    }
    return true;
}
