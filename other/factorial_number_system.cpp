
class Fuctorial_Number_System{
public:
    vector<int> f;
    int size;

    Fuctorial_Number_System(){}

    Fuctorial_Number_System(vector<int>& f_){
        size = f_.size();
        f = f_;
    }

    Fuctorial_Number_System(long long x){
        long long k = 1;
        do{
            f.push_back(x%k);
            x/=k;
            k++;
        }while(x>0);

        size = f.size();
    }

    Fuctorial_Number_System operator = (Fuctorial_Number_System& x){
        f = x.f;
        size = x.size;
        return *this;
    }

    int& operator[](int pos){
        return f[pos];
    }

    Fuctorial_Number_System operator + (const Fuctorial_Number_System& x){
        vector<int> a = this->f;
        vector<int> b = x.f;
        a.resize(max(size, x.size), 0);
        b.resize(max(size, x.size), 0);
        int carry = 0;
        for(int i=0; i<a.size(); i++){
            a[i] += b[i] + carry;
            carry = 0;
            if(a[i] > i){
                carry = a[i]/(i+1);
                a[i] %= (i+1);
            }
        }
        if(carry != 0){
            a.push_back(carry);
        }
        return Fuctorial_Number_System(a);
    }
};