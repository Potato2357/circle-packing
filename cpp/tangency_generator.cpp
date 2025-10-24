#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <chrono>
#include <thread>
#include <limits.h>
#include <math.h>
#include "Node.cpp"

using namespace std;
using namespace std::chrono;

set<string> tangencies;
set<string> tuples;

double inner_product(double *c_1, double *c_2){
    return -0.5*c_1[0]*c_2[1]-0.5*c_1[1]*c_2[0]+c_1[2]*c_2[2]+c_1[3]*c_2[3];
}

void reflect(double *base, double *refl, double *result){
    double ip = inner_product(base, refl);
    result[0] = base[0] - 2*ip*refl[0];
    result[1] = base[1] - 2*ip*refl[1];
    result[2] = base[2] - 2*ip*refl[2];
    result[3] = base[3] - 2*ip*refl[3];
}

void find_reflections_rec(Node *root, double **dual_config, int dual_size, int rec_lim, int max_num, double scale1, double scale2, double scale3, double scale4){

    if (rec_lim == 0){
        return;
    }

    for (int i=0;i<dual_size;i++){
        //inner_product(root->coords,dual_config[i])<-0.000001
        if (true){
            double ref[4];
            reflect(root->coords, dual_config[i],ref);
            //avoid precision issues
            ref[0] = round(ref[0] / scale1 * 10) * scale1 / 10;
            ref[1] = round(ref[1] / scale2 * 10) * scale2 / 10;
            ref[2] = round(ref[2] / scale3 * 10) * scale3 / 10;
            ref[3] = round(ref[3] / scale4 * 10) * scale4 / 10;
            if (ref[1] <= max_num){
                Node *new_child = new Node(ref[0],ref[1],ref[2],ref[3],dual_size);
                root->add_child(new_child,i);
                find_reflections_rec(root->children[i], dual_config, dual_size, rec_lim-1, max_num, scale1, scale2, scale3, scale4);
            }
        }
    }

}

void find_reflections(Node *root, double **base_config, int base_size, double **dual_config, int dual_size, int rec_lim, int max_num, double scale1, double scale2, double scale3, double scale4){

    for (int i=0;i<base_size;i++){
        Node *new_child = new Node(base_config[i][0],base_config[i][1],base_config[i][2],base_config[i][3],dual_size);
        root->add_child(new_child,i);
    }

    thread **threads = new thread *[base_size];
    
    for (int i=0;i<base_size;i++){
        threads[i] = new thread(find_reflections_rec, root->children[i], dual_config, dual_size, rec_lim, max_num, scale1, scale2, scale3, scale4);
    }

    for (int i=0;i<base_size;i++){
        threads[i]->join();
        delete threads[i];
    }

    delete [] threads;

}

void find_pair_reflections(Node *root_1, Node *root_2, double scale){

    int curv_1 = round(root_1->get_curvature() / scale);
    int curv_2 = round(root_2->get_curvature() / scale);
    tangencies.insert(to_string(curv_1)+" "+to_string(curv_2));
    tangencies.insert(to_string(curv_2)+" "+to_string(curv_1));

    if (root_1->is_leaf() || root_2->is_leaf()){
        return;
    }
    else{
        for (int i=0;i<root_1->branch_num;i++){
            if (root_1->child_status[i] && root_2->child_status[i]){
                find_pair_reflections(root_1->children[i], root_2->children[i], scale);
            }
        }
    }

}

void find_tuples(Node **roots, int tuple_num, double scale){

    vector<int> tuple_vec;
    bool has_leaf = false;
    for (int i=0;i<tuple_num;i++){
        int curv_temp = (int)round(roots[i]->get_curvature() / scale);
        int mod_num = 16; //change
        if (curv_temp < 0){
            curv_temp += mod_num;
        }
        curv_temp = curv_temp % mod_num;
        if (roots[i]->is_leaf()){
            has_leaf = true;
        }
        tuple_vec.push_back(curv_temp);
    }

    sort(tuple_vec.begin(), tuple_vec.end());
    string tuple = "";
    for (int i=0;i<tuple_num;i++){
        tuple = tuple + to_string(tuple_vec[i])+" ";
    }
    tuples.insert(tuple);

    if (has_leaf){
        return;
    }
    else{
        for (int i=0;i<roots[0]->branch_num;i++){
            Node **new_roots = new Node *[tuple_num];
            for (int j=0;j<tuple_num;j++){
                if (roots[j]->child_status[i]){
                    new_roots[j] = roots[j]->children[i];
                }
                else{
                    delete [] new_roots;
                    return;
                }
            }
            find_tuples(new_roots, tuple_num, scale);
            delete [] new_roots;
        }
    }
}

int main(int argc, char *argv[]){ 

    if (argc != 3){
        cout <<"Input error: please use the following format:"<< endl;
        cout <<"./curv_gen <type> <index>"<< endl;
        return 1;
    }

    //read in data

    string type = argv[1];
    string index = argv[2];

    ifstream f_base("../configs/"+type+"_"+index+"_base.txt");
    ifstream f_dual("../configs/"+type+"_"+index+"_dual.txt");

    if (!f_base.is_open() || !f_dual.is_open()){
        cout <<"Error: File cannot be opened."<< endl;
        return 1;
    }

    string in_str;
    int base_size, dual_size;

    f_base >> in_str;
    base_size = stoi(in_str);
    double **base_config = new double *[base_size];
    for (int i=0;i<base_size;i++){
        base_config[i] = new double[4];
        for (int j=0;j<4;j++){
            f_base >> in_str;
            if (in_str.find("r") != std::string::npos){
                double rat = stod(in_str);
                int rad = stoi(in_str.substr(in_str.find("r")+1));
                base_config[i][j] = rat*sqrt(rad);
            }
            else{
                base_config[i][j] = stod(in_str);
            }
        }
    }
    
    f_dual >> in_str;
    dual_size = stoi(in_str);
    double **dual_config = new double *[dual_size];
    for (int i=0;i<dual_size;i++){
        dual_config[i] = new double[4];
        for (int j=0;j<4;j++){
            f_dual >> in_str;
            if (in_str.find("r") != std::string::npos){
                double rat = stod(in_str);
                int rad = stoi(in_str.substr(in_str.find("r")+1));
                dual_config[i][j] = rat*sqrt(rad);
            }
            else{
                dual_config[i][j] = stod(in_str);
            }
        }
    }

    f_base.close();
    f_dual.close();

    double scale1, scale2, scale3, scale4;
    if (!type.compare("cube")){
        scale1 = sqrt(2);
        scale2 = sqrt(2);
        scale3 = 1;
        scale4 = sqrt(2);
    }
    else if (!type.compare("oct")){
        scale1 = 1;
        scale2 = 1;
        scale3 = sqrt(2);
        scale4 = 1;
    }
    else if (!type.compare("cubeoct")){
        scale1 = 1;
        scale2 = 1;
        scale3 = sqrt(6);
        scale4 = 1;
    }
    else if (!type.compare("square")){
        scale1 = 1;
        scale2 = 1;
        scale3 = 1;
        scale4 = 1;
    }
    else if (!type.compare("triangle")){
        scale1 = 1;
        scale2 = 1;
        scale3 = 1;
        scale4 = sqrt(3);
    }
    else if (!type.compare("hexagon")){
        scale1 = sqrt(3);
        scale2 = sqrt(3);
        scale3 = sqrt(3);
        scale4 = 1;
    }
    else{
        cout <<"Type unrecognised; enter scale factors manually:"<<endl;
        cout <<"b bar scale";
        cin >> scale1;
        cout <<"b scale";
        cin >> scale2;
        cout <<"h_1 scale";
        cin >> scale3;
        cout <<"h_2 scale";
        cin >> scale4;
    }

    int rec_lim = 100;
    int max_num = INT_MAX;

    cout <<"Mode selection:"<<endl;
    cout <<" - Recursion limited (r)"<<endl;
    cout <<" - Max curvature limited (c)"<<endl;
    cout <<"Enter the mode (r/c): ";
    cin >> in_str;
    if (in_str.compare("r") == 0){
        cout <<"How many iterations: ";
        cin >> rec_lim;
    }
    else if (in_str.compare("c") == 0){
        cout <<"Find curvatures up to: ";
        cin >> max_num;
    }
    else{
        cout <<"Input error"<<endl;
        for (int i=0;i<base_size;i++){
            delete [] base_config[i];
        }
        for (int i=0;i<dual_size;i++){
            delete [] dual_config[i];
        }
        delete [] base_config;
        delete [] dual_config;
        return 1;
    }

    bool find_tan = false;
    bool find_tuple = false;

    cout <<"Find tangencies? (y/n)";
    cin >> in_str;
    if (in_str.compare("y") == 0){
        find_tan = true;
    }
    cout <<"Find tuples? (y/n)";
    cin >> in_str;
    if (in_str.compare("y") == 0){
        find_tuple = true;
    }
    
    //calculations

    auto start = high_resolution_clock::now();
    
    Node *root_r = new Node(0,0,0,0,base_size);

    find_reflections(root_r, base_config, base_size, dual_config, dual_size, rec_lim, max_num, scale1, scale2, scale3, scale4);
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Calculation time: " << duration.count() * 0.000001 << " s" << endl;

    //output

    start = high_resolution_clock::now();

    if (find_tan){
        if (!type.compare("cube")){
            //cube: 0-1,0-2,0-4,1-3,1-5,2-3,2-6,3-7,4-5,4-6,5-7,6-7
            find_pair_reflections(root_r->children[0], root_r->children[1],sqrt(2));
            find_pair_reflections(root_r->children[0], root_r->children[2],sqrt(2));
            find_pair_reflections(root_r->children[0], root_r->children[4],sqrt(2));
            find_pair_reflections(root_r->children[1], root_r->children[3],sqrt(2));
            find_pair_reflections(root_r->children[1], root_r->children[5],sqrt(2));
            find_pair_reflections(root_r->children[2], root_r->children[3],sqrt(2));
            find_pair_reflections(root_r->children[2], root_r->children[6],sqrt(2));
            find_pair_reflections(root_r->children[3], root_r->children[7],sqrt(2));
            find_pair_reflections(root_r->children[4], root_r->children[5],sqrt(2));
            find_pair_reflections(root_r->children[4], root_r->children[6],sqrt(2));
            find_pair_reflections(root_r->children[5], root_r->children[7],sqrt(2));
            find_pair_reflections(root_r->children[6], root_r->children[7],sqrt(2));
        }
        else if (!type.compare("oct")){
            //oct: 0-2,0-3,0-4,0-5,1-2,1-3,1-4,1-5,2-3,2-4,3-5,4-5
            find_pair_reflections(root_r->children[0], root_r->children[2],1);
            find_pair_reflections(root_r->children[0], root_r->children[3],1);
            find_pair_reflections(root_r->children[0], root_r->children[4],1);
            find_pair_reflections(root_r->children[0], root_r->children[5],1);
            find_pair_reflections(root_r->children[1], root_r->children[2],1);
            find_pair_reflections(root_r->children[1], root_r->children[3],1);
            find_pair_reflections(root_r->children[1], root_r->children[4],1);
            find_pair_reflections(root_r->children[1], root_r->children[5],1);
            find_pair_reflections(root_r->children[2], root_r->children[3],1);
            find_pair_reflections(root_r->children[2], root_r->children[4],1);
            find_pair_reflections(root_r->children[3], root_r->children[5],1);
            find_pair_reflections(root_r->children[4], root_r->children[5],1);
        }
        else if (!type.compare("cubeoct")){
            //cubeoct: 0-1,0-2,0-5,0-10,1-2,1-8,1-11,2-3,2-4,3-4,3-5,3-6,4-7,4-8,5-6,5-10,6-7,6-9,7-8,7-9,8-11,9-10,9-11,10-11
            find_pair_reflections(root_r->children[0], root_r->children[1],1);
            find_pair_reflections(root_r->children[0], root_r->children[2],1);
            find_pair_reflections(root_r->children[0], root_r->children[5],1);
            find_pair_reflections(root_r->children[0], root_r->children[10],1);
            find_pair_reflections(root_r->children[1], root_r->children[2],1);
            find_pair_reflections(root_r->children[1], root_r->children[8],1);
            find_pair_reflections(root_r->children[1], root_r->children[11],1);
            find_pair_reflections(root_r->children[2], root_r->children[3],1);
            find_pair_reflections(root_r->children[2], root_r->children[4],1);
            find_pair_reflections(root_r->children[3], root_r->children[4],1);
            find_pair_reflections(root_r->children[3], root_r->children[5],1);
            find_pair_reflections(root_r->children[3], root_r->children[6],1);
            find_pair_reflections(root_r->children[4], root_r->children[7],1);
            find_pair_reflections(root_r->children[4], root_r->children[8],1);
            find_pair_reflections(root_r->children[5], root_r->children[6],1);
            find_pair_reflections(root_r->children[5], root_r->children[10],1);
            find_pair_reflections(root_r->children[6], root_r->children[7],1);
            find_pair_reflections(root_r->children[6], root_r->children[9],1);
            find_pair_reflections(root_r->children[7], root_r->children[8],1);
            find_pair_reflections(root_r->children[7], root_r->children[9],1);
            find_pair_reflections(root_r->children[8], root_r->children[11],1);
            find_pair_reflections(root_r->children[9], root_r->children[10],1);
            find_pair_reflections(root_r->children[9], root_r->children[11],1);
            find_pair_reflections(root_r->children[10], root_r->children[11],1);
        }
        else if (!type.compare("square")){
            //TODO
        }
        else if (!type.compare("triangle")){
            //TODO
        }
        else if (!type.compare("hexagon")){
            //hexagon: 0-2,2-4,4-1,1-5,5-3,3-0
            find_pair_reflections(root_r->children[0], root_r->children[2],sqrt(3));
            find_pair_reflections(root_r->children[2], root_r->children[4],sqrt(3));
            find_pair_reflections(root_r->children[4], root_r->children[1],sqrt(3));
            find_pair_reflections(root_r->children[1], root_r->children[5],sqrt(3));
            find_pair_reflections(root_r->children[5], root_r->children[3],sqrt(3));
            find_pair_reflections(root_r->children[3], root_r->children[0],sqrt(3));
        }
        else{
            //TODO
        }

        ofstream f_tan("tangencies.txt");
        for (auto i : tangencies){
            f_tan <<i<<endl;
        }
        f_tan.close();

    }
    if (find_tuple){
        if (!type.compare("cube")){
            find_tuples(root_r->children, 8, sqrt(2));
        }
        else if (!type.compare("oct")){
            find_tuples(root_r->children, 6, 1);
        }
        else if (!type.compare("cubeoct")){
            find_tuples(root_r->children, 12, 1);
        }
        else if (!type.compare("square")){
            //TODO
        }
        else if (!type.compare("triangle")){
            //TODO
        }
        else if (!type.compare("hexagon")){
            //TODO
        }
        else{
            //TODO
        }

        ofstream f_tuple("tuples.txt");
        for (auto i : tuples){
            f_tuple <<i<<endl;
        }
        f_tuple.close();

    }

    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Write time: " << duration.count() * 0.000001 << " s" << endl;

    //clean up

    start = high_resolution_clock::now();

    for (int i=0;i<base_size;i++){
        delete [] base_config[i];
    }
    for (int i=0;i<dual_size;i++){
        delete [] dual_config[i];
    }
    delete [] base_config;
    delete [] dual_config;

    delete root_r;

    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Clean up time: " << duration.count() * 0.000001 << " s" << endl;

    return 0;
}