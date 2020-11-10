#include<bits/stdc++.h>
using namespace std;

const int N = 100;
vector<int> g[N], correct_tsvs, faulty_tsvs, spare_tsvs;
int edge_capacity[N][N];
vector<vector<int> > repair_paths;

int get_index(int i,int j, int rows,int cols) {
    int ind = i*(cols) + j + 1;
    return ind;
}

int find_shortest_path(int source,int sink, vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);

    parent[source] = -2;
    queue<int> q;
    q.push(source);

    while(!q.empty()) {
        int cur = q.front();
        q.pop();

        for(auto next : g[cur]) {
            if(parent[next] == -1 && edge_capacity[cur][next] == 1) {
                parent[next] = cur;
                if(next == sink) return 1;
                q.push(next);
            }
        }
    }

    return 0;
}


int main() {
    int rows, cols, length_constraint;

    //read input
    
        cout<<"Enter the number of rows in TSV Grid"<<endl;
        cin>>rows;
        cout<<"Enter the number of columns in TSV Grid"<<endl;
        cin>>cols;
        int tsv_grid[rows][cols];
        cout<<"Enter TSV grid \n 0 for faulty tsvs  \n 1 for correct tsvs \n 2 for spare TSVs \n 3 for an empty position \n \n";
        for(int i=0;i<rows;i++) {
            for(int j=0;j<cols;j++) {
                cin>>tsv_grid[i][j];
            }
        }    
        cout<<endl;
        cout<<"Enter length constraint"<<endl;
        cin>>length_constraint;
        cout<<endl;
    
    
    for(int i=0;i<rows;i++) {
        for(int j=0;j<cols;j++) {

            int ind = get_index(i,j,rows,cols);
            if(tsv_grid[i][j] == 1) {
                correct_tsvs.push_back(ind);
            }
            else if(tsv_grid[i][j] == 0) {
                faulty_tsvs.push_back(ind);
            }
            else if(tsv_grid[i][j] == 2) {
                spare_tsvs.push_back(ind);
            }
        }
    }    

    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            edge_capacity[i][j] = 1;
        }
    }


    // cout<<"Indexes of faulty tsvs are"<<endl;
    // for(auto e: faulty_tsvs) cout<<e<<" ";
    // cout<<endl<<endl;

    // cout<<"Indexes of correct tsvs are"<<endl;
    // for(auto e: correct_tsvs) cout<<e<<" ";
    // cout<<endl<<endl;


    // cout<<"Indexes of spare tsvs"<<endl;
    // for(auto e: spare_tsvs) cout<<e<<" ";
    // cout<<endl<<endl;

    // construct directed graph
    for(int i=0;i<rows;i++) {
        for(int j=0;j<cols;j++) {
            if(tsv_grid[i][j] == 3) continue;
            
            int ind = get_index(i,j,rows,cols);

            if(j != cols-1) {
                int ind_right = get_index(i,j+1,rows,cols);
                g[ind].push_back(ind_right);
            }
            
            if(i != rows-1) {
                int ind_down = get_index(i+1,j,rows,cols);
                g[ind].push_back(ind_down);
            }
            
            if(j != 0) {
                int ind_left = get_index(i,j-1,rows,cols);
                g[ind].push_back(ind_left);
            }
                
            if(i != 0) {
                int ind_up = get_index(i+1,j,rows,cols);
                g[ind].push_back(ind_up);
            }
                 
        }
    }
    
    int source = 0, sink = (rows)*(cols)+1;

    for(int i=0;i<faulty_tsvs.size();i++) {
        g[source].push_back(faulty_tsvs[i]);
    }

    for(int i=0;i<spare_tsvs.size();i++) {
        g[spare_tsvs[i]].push_back(sink);
        
    }

    int total_no_of_paths = 0;
   
    vector<int> parent(N);
    while(1) {
        int path_exists = find_shortest_path(source, sink, parent);
        if(path_exists == 0) break;
        total_no_of_paths += 1;

        vector<int> cur_repair_path;
        int cur = sink;
        while(cur != source) {
            int prev = parent[cur];
            edge_capacity[prev][cur] -= 1;
            edge_capacity[cur][prev] += 1;
            cur = prev;
            if(cur != source)
            cur_repair_path.push_back(cur);
        }
        reverse(cur_repair_path.begin(), cur_repair_path.end());
        if(cur_repair_path.size() > length_constraint + 1) {
            cout<<"The given TSV Grid is not repairable under given length constraint \n Try increasing the length constraint\n"<<endl;
            return 0;
        }
        repair_paths.push_back(cur_repair_path);
    }

    int no_of_faulty_tsvs = faulty_tsvs.size();
    if(total_no_of_paths == no_of_faulty_tsvs) {
        cout<<"The given TSV Grid is repairable under given length constraint"<<endl<<endl;
        cout<<"Repair paths are"<<endl<<endl;
        for(auto repair_path : repair_paths) {
            for(int i=0;i<repair_path.size();i++)  {
                cout<<repair_path[i];
                if(i != repair_path.size()-1) cout<<" -> ";
            }
            cout<<endl<<endl;
        }
    }
    else {
         cout<<"The given TSV Grid is not repairable under given length constraint \n Try increasing the number of correct TSVs \n OR \n  decreasing the number of faulty TSVs\n"<<endl;;
    }
}