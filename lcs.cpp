#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int lcs(string a, string b) {
    int n = a.size();
    int m = b.size();
    
    vector<vector<int> > v(n+1,vector<int>(m+1,0));
   
    for(int i = 1; i<= n; i++){
        for(int j = 1; j <= m; j++){
            if(a[i-1] == b[j-1])
                v[i][j] = v[i-1][j-1] + 1; 
            else if (a[i-1] != b[i-1])
                v[i][j] = max(v[i][j-1], v[i-1][j]);
            
        }
    }
    return v[n][m];
}
string get_lcs(string a, string b){
    int n = a.size();
    int m = b.size();
    
    vector<vector<int> > v(n+1,vector<int>(m+1,0));
    for(int i = 1; i<= n; i++){
        for(int j = 1; j <= m; j++){
            if(a[i-1] == b[j-1]){
                v[i][j] = v[i-1][j-1] + 1; 
            }
            else if (v[i-1][j] > v[i][j-1]){
                v[i][j] += v[i-1][j]; 
            }else{
                v[i][j] += v[i][j-1]; 
            }
        }
    }
    string res = "";
    int i = a.size();
    int j = b.size();
    while(i > 0 && j > 0){
        if(a[i-1] == b[i-1]){
            res += a[i-1];
            i--;
            j--;
        }
        else if(v[i-1][j] > v[i][j-1]){
            i--;
        }
        else{
            j--;
        }
    }
    
    return res;
}
int main() {
    string a,b;
    cin>>a;
    cin>>b;
    cout<<get_lcs(a,b)<<endl;
    return 0;
};