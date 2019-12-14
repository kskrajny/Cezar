#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]){

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    cout << N << endl << M << endl;
    for(int i=0;i<N;i++){
        int los =rand()%2;
        if(los == 0){
            cout << 'R' << endl;
        } else {
            cout << 'G' << endl;
        }
    }
    for(int i=0;i<M;i++){
        int los =rand()%2;
        if(los == 0){
            cout << '?' << endl;
        } else {
            cout << 'O' << endl;
        }
        if(i == M/2) {cout << N/2 << endl << N/2+1 << endl;} else
        {cout << N/2-i/4 << endl << N-i/2-los << endl;}
    }

    return 0;
}
