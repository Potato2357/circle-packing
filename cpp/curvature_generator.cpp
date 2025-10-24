#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <chrono>
#include <thread>
#include <limits.h>
#include <math.h>

using namespace std;
using namespace std::chrono;

set<int> curvatures;
set<string> circles;

/**
 * Finds the inner product of two circles
 */
double inner_product(double *c_1, double *c_2){
    return -0.5*c_1[0]*c_2[1]-0.5*c_1[1]*c_2[0]+c_1[2]*c_2[2]+c_1[3]*c_2[3];
}

/**
 * Finds the coordinates after reflecting one circle across another
 * @param base the circle to be reflected
 * @param refl the circle reflected across
 * @param result return result via pointer
 */
void reflect(double *base, double *refl, double *result){
    double ip = inner_product(base, refl);
    result[0] = base[0] - 2*ip*refl[0];
    result[1] = base[1] - 2*ip*refl[1];
    result[2] = base[2] - 2*ip*refl[2];
    result[3] = base[3] - 2*ip*refl[3];
}

/**
 * Returns a string with the coordinates of the circle
 */
string circ_to_str(double *circle){
    return to_string(circle[0])+" "+to_string(circle[1])+" "+to_string(circle[2])+" "+to_string(circle[3]);
}

/**
 * Recursively finds reflections of a circle across dual circles
 * @param curvs set to store curvatures
 * @param circs vector to store reflected circles
 * @param base_circle the circle to reflect
 * @param dual_config the dual configuration
 * @param dual_size the number of circles in the dual configuration
 * @param rec_lim the maximum depth of recursions
 * @param max_num the maximum curvature to search to
 * @param scale1 scale for inversive coordinates
 * @param scale2 scale for inversive coordinates
 * @param scale3 scale for inversive coordinates
 * @param scale4 scale for inversive coordinates
 * @param print_circ option to save the circles
 */
void find_reflections_rec(set<int> *curvs, set<string> *circs, double *base_circle, double **dual_config, int dual_size, int rec_lim, int max_num, double scale1, double scale2, double scale3, double scale4, bool print_circ){

    curvs->insert(round(base_circle[1] / scale2));
    if (print_circ){
        circs->insert(circ_to_str(base_circle));
    }

    if (rec_lim == 0){
        if (max_num < INT_MAX){
            cout << "Error: recursion limit reached" << endl;
        }
        return;
    }

    for (int i=0;i<dual_size;i++){
        if (inner_product(base_circle,dual_config[i]) < -0.01){
            double *ref = new double[4];
            reflect(base_circle, dual_config[i], ref);
            //avoid precision issues
            // ref[0] = round(ref[0] / scale1 * 10) * scale1 / 10;
            // ref[1] = round(ref[1] / scale2 * 10) * scale2 / 10;
            // ref[2] = round(ref[2] / scale3 * 10) * scale3 / 10;
            // ref[3] = round(ref[3] / scale4 * 10) * scale4 / 10;
            if (ref[1] <= max_num){
                find_reflections_rec(curvs, circs, ref, dual_config, dual_size, rec_lim-1, max_num, scale1, scale2, scale3, scale4, print_circ);
            }
            delete [] ref;
        }
    }

}

/**
 * Thread handler
 */
void thread_func(set<int> *curvs, set<string> *circs, double *base_circle, double **dual_config, int dual_size, int rec_lim, int max_num, double scale1, double scale2, double scale3, double scale4, bool print_circ){
    find_reflections_rec(curvs, circs, base_circle, dual_config, dual_size, rec_lim, max_num, scale1, scale2, scale3, scale4, print_circ);
}

/**
 * Main function to handle finding reflections,
 * spawns off threads to do calculations and organises the results
 * @param base_config the base configuration
 * @param base_size the number of circles in the base configuration
 * @param dual_config the dual configuration
 * @param dual_size the number of circles in the dual configuration
 * @param rec_lim the maximum depth of recursions
 * @param max_num the maximum curvature to search to
 * @param scale1 scale for inversive coordinates
 * @param scale2 scale for inversive coordinates
 * @param scale3 scale for inversive coordinates
 * @param scale4 scale for inversive coordinates
 * @param thread_num the number of threads (currently defaults to the base size)
 * @param print_circ option to save the circles
 */
void find_reflections(double **base_config, int base_size, double **dual_config, int dual_size, int rec_lim, int max_num, double scale1, double scale2, double scale3, double scale4, int thread_num, bool print_circ){
    
    thread **threads = new thread *[thread_num];
    set<int> *curvs = new set<int>[thread_num];
    set<string> *circs = new set<string>[thread_num];
    
    int load_num = ceil((double) thread_num / 8);
    int s = 0;
    int t = 0;

    for (int i=0;i<load_num;i++){
        for (int j=0;j<8;j++){
            threads[s] = new thread(thread_func, &curvs[s], &circs[s], base_config[s], dual_config, dual_size, rec_lim, max_num, scale1, scale2, scale3, scale4, print_circ);
            s++;
            if (s >= thread_num){
                break;
            }
        }

        for (int j=0;j<8;j++){
            threads[t]->join();
            delete threads[t];
            curvatures.insert(curvs[t].begin(), curvs[t].end());
            circles.insert(circs[t].begin(), circs[t].end());
            t++;
            if (t >= thread_num){
                break;
            }
        }
    }
    
    
    delete [] threads;
    delete [] curvs;
    delete [] circs;

}

string curvs_mod(int n){
    set<int> curvs_mod;
    for (auto i : curvatures){
        int temp = i;
        while (temp < 0){
            temp += n;
        }
        curvs_mod.insert(temp%n);
    }
    string ans = "";
    for (auto i : curvs_mod){
        ans = ans + to_string(i) + " ";
    }
    ans += "\nMissing: ";
    for (int i=0;i<n;i++){
        auto j = curvs_mod.find(i);
        if (j == curvs_mod.end()){
            ans = ans + to_string(i)+ " ";
        }
    }
    return ans;
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

    int rec_lim = 200;
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

    bool print_circ = false;
    bool print_curv = false;
    bool print_mod = false;

    cout <<"Print circles? (y/n)";
    cin >> in_str;
    if (in_str.compare("y") == 0){
        print_circ = true;
    }
    cout <<"Print curvatures? (y/n)";
    cin >> in_str;
    if (in_str.compare("y") == 0){
        print_curv = true;
    }
    cout <<"Print modulo n? (y/n)";
    cin >> in_str;
    if (in_str.compare("y") == 0){
        print_mod = true;
    }
    
    //calculations

    auto start = high_resolution_clock::now();
    
    find_reflections(base_config, base_size, dual_config, dual_size, rec_lim, max_num, scale1, scale2, scale3, scale4, base_size, print_circ);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Calculation time: " << duration.count() * 0.000001 << " s" << endl;

    //output

    start = high_resolution_clock::now();

    if (print_curv){
        ofstream f_curv("../packings/"+type+"_"+index+"_curvatures.txt");
        for (auto i : curvatures){
            f_curv <<i<<endl;
        }
        f_curv.close();
    }

    if (print_circ){
        ofstream f_circ("circle_packing.txt");
        for (auto i : circles){
            f_circ << i <<endl;
        }
        f_circ << "END" <<endl;
        f_circ.close();
        ofstream f_circ_d("circle_packing_dual.txt");
        for (int i=0;i<dual_size;i++){
            f_circ_d <<dual_config[i][0]<<" "<<dual_config[i][1]<<" "<<dual_config[i][2]<<" "<<dual_config[i][3]<<" "<<endl;
        }
        f_circ_d << "END" <<endl;
        f_circ_d.close();
    }

    if (print_mod){
        ofstream f_mod("curvatures_mod.txt");
        f_mod <<"mod 2: "<<curvs_mod(2)<<endl<<endl;
        f_mod <<"mod 3: "<<curvs_mod(3)<<endl<<endl;
        f_mod <<"mod 4: "<<curvs_mod(4)<<endl<<endl;
        f_mod <<"mod 5: "<<curvs_mod(5)<<endl<<endl;
        f_mod <<"mod 6: "<<curvs_mod(6)<<endl<<endl;
        f_mod <<"mod 7: "<<curvs_mod(7)<<endl<<endl;
        f_mod <<"mod 8: "<<curvs_mod(8)<<endl<<endl;
        f_mod <<"mod 9: "<<curvs_mod(9)<<endl<<endl;
        f_mod <<"mod 11: "<<curvs_mod(11)<<endl<<endl;
        f_mod <<"mod 12: "<<curvs_mod(12)<<endl<<endl;
        f_mod <<"mod 13: "<<curvs_mod(13)<<endl<<endl;
        f_mod <<"mod 16: "<<curvs_mod(16)<<endl<<endl;
        f_mod <<"mod 24: "<<curvs_mod(24)<<endl<<endl;
        f_mod <<"mod 32: "<<curvs_mod(32)<<endl<<endl;
        f_mod <<"mod 48: "<<curvs_mod(48)<<endl<<endl;
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Write time: " << duration.count() * 0.000001 << " s" << endl;

    //clean up

    for (int i=0;i<base_size;i++){
        delete [] base_config[i];
    }
    for (int i=0;i<dual_size;i++){
        delete [] dual_config[i];
    }
    delete [] base_config;
    delete [] dual_config;

    return 0;
}