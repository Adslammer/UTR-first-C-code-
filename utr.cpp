#include <map>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

struct Automat {

  template<class T, class U>
  void load(basic_istream<T, U>& in) {
    string t;

    getline(in, t); toIds(t); // sva stanja
    getline(in, t); // svi input znakovi

    for (int i = 0; i < t.size(); i+=2)
      inputset.push_back(t[i]);
    
    trans.resize(id.size()+1);
    getline(in, t); konacna = toIds(t);
    getline(in, t); start = toIds(t);

    while (getline(in, t)) {
      char stanje[100], target[100], p;
      sscanf(t.c_str(),"%[^,],%c->%s", stanje, &p, target);
      makeTrans(p, getId(stanje), toIds(string(target)));
     /* free(stanje); free(target);*/
    }
  }

  int solve(const string t) {
    cout << endl;
    reset();
    for (int i = 0; i < t.size(); ++i) {
      cout << "ubacujem znak: " << t[i] << endl;
      push(t[i]);
      while(push('$'));
      debug();
      cout << endl;
    }
    for (int i = 0; i < trenutna.size(); ++i)
      for (int j = 0; j < konacna.size(); ++j) 
        if (trenutna[i] == konacna[j])
          return 1;
    return 0;
  }

  void reset() {
    cout << "postavljen u pocetna stanja." << endl;
    trenutna = start;
    while (push('$'));
  }

  void out() {
    for (int i = 0; i < trans.size(); ++i) {
      cout << i << ':';
      for (int j = 0; j < trans[i].size(); ++j) {
        cout << trans[i][j] << "->" << j << "  ";
      } 
      cout << endl;
    }
  }

  void debug() {
    cout << "trenutna stanja: ";
    for (int i = 0; i < trenutna.size(); ++i) {
      cout << trenutna[i] << ' ';
    }
    cout << endl;
  }

  int push(char t) {
    vector<int> next;
    int sol = 0;

    for (int i = 0; i < trenutna.size(); ++i) {
      vector<string> &from = trans[trenutna[i]];
      bool match = false;
      for (int j = 0; j < from.size(); ++j) {
        if (from[j].find(t) != string::npos) {
          match = (t != '$');
          next.push_back(j);
          printf( t == '$' ?
            "epsilon prijelaz iz %d -> %d.\n" :
            "stanje %d je preslo u %d.\n", trenutna[i], j);
        }
      }
      if (!match) {
        next.push_back(trenutna[i]);
      }
    }

    sort(next.begin(),next.end());
    next.erase(unique(next.begin(), next.end()), next.end());

    sol = (trenutna!=next);
    trenutna = next;

    return sol;
  }

  void makeTrans(char p, int fromId, const vector<int> targs) {
    vector<string> &from = (trans[fromId]);

    for (int i = 0; i < targs.size(); ++i) {
      if (targs[i] >= from.size()) {
        from.resize(targs[i]+1);
      }
      from[targs[i]].push_back(p);
    }
  }

  int getId(const char* t) {return getId( string(t) );}

  int getId(const string &t) {
    return id[t] ? id[t] : id[t] = id.size();
  }

  vector<int> toIds(string t) {
    size_t pos;
    vector<int> ret;

    for (t.push_back(',');
      (pos = t.find(',')) != string::npos;
      t = t.substr(pos+1))
        ret.push_back( getId(t.substr(0,pos)) );
    
    return ret;
  }

  map <string,int> id;
  string inputset;

  vector<int> pocetna, konacna, start, trenutna;
  vector< vector<string> > trans;

} S;

int main() {
  ifstream definicija("nka.txt");
  ifstream ulaz("in.txt");
  string t;
  
  
  S.load(definicija);

  while (getline(ulaz, t)) {
    
    cout <<
      (S.solve(t) ? "automat prhivaca niz " : "automat ne prihvaca niz ")
      << t << endl;
  }
  
  system("PAUSE");	

}

