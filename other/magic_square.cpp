
vector<vector<int>> Magic_Square(int n){

    vector<vector<int>> v(n, vector<int>(n,0));

    if(n&1){	//odd
        int x = n/2;
        int y = 0;
        int cnt = 0;
        v[y][x] = ++cnt;
        while(cnt < n*n){
            x = (x+1)%n;
            y = (y-1+n)%n;
            if(v[y][x] != 0){
                x = (x-1+n)%n;
                y = (y+2)%n;
            }
            v[y][x] = ++cnt;
        }
    }else if(n%4 == 0){	//4k
        int x = 0;
        int y = 0;
        for(int i=0; i<n; i++){
            for(int k=0; k<n/4; k++){
                v[i][(i+k*4)%n] = -1;
                v[i][(3+k*4-i+n)%n] = -1;
            }
        }
        vector<int> tmp(n*n);
        iota(tmp.begin(), tmp.end(), 1);
        set<int> s(tmp.begin(), tmp.end());
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(v[i][j] < 0){
                    v[i][j] = i*n + j + 1;
                    s.erase(v[i][j]);
                }
            }
        }
        for(int i=n-1; i>=0; i--){
            for(int j=n-1; j>=0; j--){
                if(v[i][j] == 0){
                    v[i][j] = *s.begin();
                    s.erase(v[i][j]);
                }
            }
        }

    }else{	//4k+2
        auto u = Magic_Square((n-2)/2 + 1);
        for(int i=0; i<u.size(); i++){
            for(int j=0; j<u.size(); j++){
                u[i][j] = (u[i][j] - 1)*4;
            }
        }
        vector<vector<int>> w((n-2)/2+1, vector<int>((n-2)/2 + 1, 0));
        for(int i=0; i<w.size(); i++){
            w[w.size()/2 + 1][i] = 1;
        }
        swap(w[w.size()/2 + 1][w.size()/2], w[w.size()/2][w.size()/2]);

        for(int i=w.size()/2 + 2; i<w.size(); i++){
            for(int j=0; j<w.size(); j++){
                w[i][j] = 2;
            }
        }

        int z[3][2][2] = {
            {{4,1},{2,3}},
            {{1,4},{2,3}},
            {{1,4},{3,2}}
        };

        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                v[i][j] = u[i/2][j/2];
                int w_ = w[i/2][j/2];
                v[i][j] += z[w_][i&1][j&1];
            }
        }
    }


    return v;
}
