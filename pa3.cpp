#include<bits/stdc++.h>
using namespace std;

bool check_antipodal(vector<vector<long long>>&u,vector<vector<long long>>&v,int i,int j){
    long long cmp1=u[i][0]*u[j][1]-u[i][1]*u[j][0];
    cmp1=(cmp1==0?0:cmp1/abs(cmp1));
    long long cmp2=u[i][0]*v[j][1]-u[i][1]*v[j][0];
    cmp2=(cmp2==0?0:cmp2/abs(cmp2));
    if(cmp1*cmp2>=0){
        return 1;
    }
    cmp1=u[j][0]*u[i][1]-u[j][1]*u[i][0];
    cmp1=(cmp1==0?0:cmp1/abs(cmp1));
    cmp2=u[j][0]*v[j][1]-u[j][1]*v[j][0];
    cmp2=(cmp2==0?0:cmp2/abs(cmp2));
    if(cmp1*cmp2>0){
        return 0;
    }
    return 1;
}
int main(){
    ifstream h("a3in.txt");
    int n;
    h>>n;
    vector<vector<long long>> a(n,vector<long long>(2,0)),u(n,vector<long long>(2,0)),v(n,vector<long long>(2,0));
    map<int,map<int,bool>> mp;
    vector<pair<int,int>> res;
    long long x,x1,y,y1;
    cin>>x>>y>>x1>>y1;
    for(auto &i:a) h>>i[0]>>i[1],mp[i[0]][i[1]]=1;
    if(!mp[x][y] || !mp[x1][y1]){
        cout<<"0\n";
        return 0;
    }
    for(int i=0;i<n;i++){
        v[i][0]=a[(i+1)%n][0]-a[i][0];
        v[i][1]=a[(i+1)%n][1]-a[i][1];
        u[i][0]=a[(i+n-1)%n][0]-a[i][0];
        u[i][1]=a[(i+n-1)%n][1]-a[i][1];
    }
    long long ans=0;
    int idx=1,last=1;
    for(int i=0;i<n;i++){
        idx=max(i+1,last);
        bool flag=0;
        while(idx<n){
            bool cmp1=check_antipodal(u,v,i,idx);
            bool cmp2=check_antipodal(v,u,i,idx);
            if(cmp1==cmp2){
                res.push_back({i+1,idx+1});
                ans=min(ans,(a[i][0]-a[idx][0])*(a[i][0]-a[idx][0])+(a[i][1]-a[idx][1])*(a[i][1]-a[idx][1]));
            }
            if(flag && cmp1!=cmp2) break;
            else if(!flag && cmp1==cmp2) last=idx,flag=1;
            idx++;
        }
    }
    if((x-x1)*(x-x1)+(y-y1)*(y-y1)==ans) cout<<"1\n";
    else cout<<"0\n";
    return 0;

}

/*
7
100 0
170 68
169 69
160 77
123 97
100 98
85 98

7
100 0
170 68
169 69
159 77
123 97
100 98
85 98

7
100 0
115 98
100 98
77 98
40 77
31 69
30 68

4
40 0
100 0
70 4
60 3
*/
