#if !defined(__DICTPRODUCER_H__)
#define __DICTPRODUCER_H__

#include "SplitTool.hpp"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <dirent.h>
#include <fstream>
#include <sstream>

using std::string, std::map, std::vector, std::set, std::pair;
using std::ifstream, std::istringstream;
using std::ofstream;
using std::cout;

class DictProducer
{
private:
    vector<string> _files;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index;
    SplitTool* _cuttor;


public:
    //初始化字典生成类
    DictProducer()
    :_files{}
    ,_dict{}
    ,_index{}
    ,_cuttor{nullptr}
    {};     //string：路径        tool:基类指针

    void getFiles(string dir) {
        DIR *pdir = opendir(dir.c_str() );
        if(!pdir) {
            perror("opendir");
            cout << dir << "\n";
            return;
        }
        dirent *ptr;
        while ((ptr = readdir(pdir) )!=0 ) {
            // 排除.和..文件
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
            {
                _files.push_back(dir + "/" + ptr->d_name);
            }
        }
        closedir(pdir);
    };

    void buildEnDict() {
        map<string, int> _preDict;
        _dict.resize(5000);
        
        for(auto & files: _files) {
            ifstream ifs(files);
            string line;
            while(getline(ifs, line) ) {
                for(auto& ch :line) {
                    if(!isalpha(ch) ) {
                        ch = ' ';
                    } else {
                        ch = tolower(ch);
                    }
                }
                istringstream iss(line);
                string word;
                while (iss >> word) {
                    ++_preDict[word];
                }
            }
            ifs.close();
        }

        _dict = {_preDict.begin(), _preDict.end()};
    };

    void buildCnDict();
    void createIndex();

    void store(string dir) {
        ofstream dict_out{dir};
        for(auto it:_dict) {
            dict_out << it.first << " " << it.second << "\n";
        }
        dict_out.close();
    };

    void showFiles() {   //测试用。打印语料库路径
        for(auto it : _files) {
            std::cout << it.data() << "\n";
        }
    }
    void showDict() {   //测试用，打印字典内容
        for(auto it:_dict) {
            cout << it.first << " : " <<it.second << "\n";
        }
        cout << _dict.size() << "\n";
        cout << _dict.capacity() << "\n";

        cout << _dict[0].first << " " << _dict[0].second << "\n";
    }
};

#endif // __DICTPRODUCER_H__
