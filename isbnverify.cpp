#include <iostream>
#include <string>
using std::string;
using std::cin;
using std::cout;
using std::endl;

int main()
{
    string s;
    cin >> s;
    int len = s.length();
    char v = s.back();
    if(v == 'x') v = 10;
    else v -= '0';
    v = 10-v;
    int ans = 0;
    int cnt = 0;
    int p = 10;
    for(int i=0;i<len-1;++i) {
        if('0' <= s[i] && s[i] <= '9') {
            if(cnt) ans = (ans+s[i]-'0')%p;
            else ans = (ans+(s[i]-'0')*3)%p;
            cnt ^= 1;
        }
    }
    if(ans == (int)v)cout << "true" << endl;
    else {
        cout << "false" << endl;
        cout << "ans: " << ans << " verifynum: " << (int)v << endl;
    }
    return 0;
}