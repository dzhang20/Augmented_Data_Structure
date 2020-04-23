#include<iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
using namespace std; 

int main()  
{  
    vector<int> v;
    fstream newfile;
    map<int,int> mapp;
    newfile.open("test.txt",ios::in);
    if (newfile.is_open()){   
        string line;
        int index=0;
        while(getline(newfile, line)){ 
//           cout << line << "\n"; //print the data of the string
            int value = stoi(line);
            std::vector<int>::iterator it = std::find(v.begin(), v.end(), value);
            if(it==v.end()){
                v.push_back(value);
                index++;
                continue;
            }
            int last = v.at(v.size()-1);
            if(value == last){
                index++;
                continue;
            }
            int i = std::distance(v.begin(), it);
            int reuse = v.size() - i-1;
            if(mapp.find(reuse)==mapp.end()){
                mapp[reuse]=1;
            }else{
                mapp[reuse]+=1;
            }
            v.erase(v.begin()+i);
            v.push_back(value);
            index++;
        }
      newfile.close(); //close the file object.
    } 
    for(map<int,int>::iterator it = mapp.begin();it != mapp.end(); ++it){
        std::cout << " " << (*it).first << " " << (*it).second << "\n";
    }
    return 0;  
}