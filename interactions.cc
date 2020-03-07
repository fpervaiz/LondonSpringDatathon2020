#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <string>

using namespace std;

using MatrixInt = vector<vector<int> >;

const int NTA_NUM = 54; // Number of NTA's for which we have bike stations (including the -1 ones)

map<int, int> load_station_to_nta(string filepath) {
    map<int, int> m;
    ifstream data(filepath);
    if (data.is_open()) {
        string s;
        getline(data, s); // Omit header line

        while (getline(data, s)) {
            istringstream iss(s);
            string key, value;
            getline(iss, key, ',');
            getline(iss, value, ',');
            m[stoi(key)] = stoi(value);   
        }

        return m;
    } else {
        cout << "Could not load " << filepath << endl;
    }
}

MatrixInt neighbourhood_interation(string filepath, map<int, int> relation) {
    MatrixInt M(NTA_NUM, vector<int>(NTA_NUM, 0));
    ifstream data(filepath);
    if (data.is_open()) {
        string s;
        getline(data, s); // Omit header line
        
        // vector<string> relevant_variables = {"tripduration", "starttime", "stoptime", "start_station_id", "end_station_id"};
        string tripduration, starttime, stoptime, start_station_id, end_station_id;
        int count = 0;
        while (getline(data, s)){
            if (count % 100000 == 0) cout << count << endl;
            count += 1;
            istringstream iss(s);
            getline(iss, tripduration, ',');
            getline(iss, starttime, ',');
            getline(iss, stoptime, ',');
            getline(iss, start_station_id, ',');
            getline(iss, end_station_id, ',');
            int start_nta, end_nta;
            
            if (start_station_id.size() > 0 and end_station_id.size() > 0) {
                start_nta = relation[stoi(start_station_id)];
                end_nta = relation[stoi(end_station_id)];
                M[start_nta][end_nta] += 1;
            }
        }

        return M;
    } else {
        cout << "Could not load " << filepath << endl;
    }    
}

void write_matrix(const MatrixInt& M, string filepath) {
    ofstream outfile(filepath);
    for (int i = 0; i < M.size(); ++i) {
        for (int j = 0; j < M[i].size(); ++j) {
            if (j != 0) outfile << ",";
            outfile << M[i][j];
        }
        outfile << endl;
    }
    outfile.close();
}

int main() {
    map<int, int> id_to_nta = load_station_to_nta("Data/id-to-nta.csv");
    MatrixInt M = neighbourhood_interation("Data/nyc_bikeshare.csv", id_to_nta);
    write_matrix(M, "nta_interactions.csv");
}