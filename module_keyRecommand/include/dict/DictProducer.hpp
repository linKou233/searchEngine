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
#include <cstring>

using std::string, std::map, std::vector, std::set, std::pair;
using std::ifstream, std::istringstream;
using std::ofstream;
using std::cout;

class DictProducer
{
private:
    vector<string> _files;
    //vector<pair<string, int>> _cnDict;
    vector<pair<string, int>> _dict;
   // map<string, set<int>> _cnIndex;
    map<string, set<int>> _index;
    SplitToolJieba _cuttor;

public:
    //初始化字典生成类
    DictProducer();     //string：路径        tool:基类指针

    void getFiles(string dir);

    void buildEnDict();
    void createEnIndex(string dir);

    void buildCnDict();
    void createCnIndex(string dir);

    void store(string dir);

    void showFiles();   //测试用，打印文件路径 
    void showDict();    //测试用，打印词典内容

    size_t getByteNum_UTF8(const char byte);
};

DictProducer::DictProducer()
{}; // string：路径        tool:基类指针


size_t DictProducer::getByteNum_UTF8(const char byte) {
    int byteNum = 0;
    for(size_t i=0; i<6; ++i) {
        if(byte & (1<<(7-i) ))
            ++byteNum;
        else
            break;
    }

    return byteNum == 0 ? 1:byteNum;
};

void DictProducer::getFiles(string dir)
{
    DIR *pdir = opendir(dir.c_str());
    if (!pdir)
    {
        perror("opendir");
        cout << dir << "\n";
        return;
    }
    dirent *ptr;
    while ((ptr = readdir(pdir)) != 0)
    {
        // 排除.和..文件
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
        {
            _files.push_back(dir + "/" + ptr->d_name);
        }
    }
    closedir(pdir);
};

void DictProducer::buildEnDict()
{
    map<string, int> _preDict;
    _dict.reserve(5000);

    for (auto &files : _files)
    {
        ifstream ifs(files);
        string line;
        while (getline(ifs, line))
        {
            for (auto &ch : line)
            {
                if (!isalpha(ch))
                    ch = ' ';
                else
                    ch = tolower(ch);
            }
            istringstream iss(line);
            string word;
            while (iss >> word)
                ++_preDict[word];
        }
        ifs.close();
    }
    _dict = {_preDict.begin(), _preDict.end()};
};

void DictProducer::createEnIndex(string dir)    //dir为要生成的词典路径
{
    for (int i = 0; i < _dict.size(); ++i)
    {
        for (char c : _dict[i].first)
        {
            string s{c};
            _index[s].insert(i);
        }
    }
    ofstream  index_out{dir};
    for(const auto &it:_index)
    {
        index_out << it.first << " ";
        for(int value: it.second)
        {
            index_out << value <<" ";
        }
        index_out << "\n";
    }
};


void DictProducer::buildCnDict()
{
    map<string, int> _preDict;
    _dict.reserve(5000);

    for (auto &files : _files)
    {
        ifstream ifs(files);
        string line;
        while (getline(ifs, line))
        {
            for (auto &ch : line)
            {
                if(isalpha(ch) || ispunct(ch) || isalnum(ch)) //如果是英文或者标点符号
                {
                    ch = ' ';
                }
            }
            vector<string> words;
            words = _cuttor.cut(line);
            for(auto word:words) {
                if(word != " ")
                    ++_preDict[word];
            }
        }
        ifs.close();
    }

    _dict = {_preDict.begin(), _preDict.end()};
};

void DictProducer::createCnIndex(string dir) {
    int i=0;
    for(auto ch: _dict) {
        string word = ch.first; //获取当前词条
        size_t charNums = word.size();  //获取当前词条的字符数

        for(size_t idx=0; idx!=charNums; ) {
            //获取当前字符在utf-8编码中所占字节数
            size_t charlen = getByteNum_UTF8(word[idx]);
            
            //提取当前字符及其后续字符串组成的字串
            string subword = word.substr(idx, charlen);

            //将当前字串添加到索引中
            _index[subword].insert(i);
            idx += charlen;
        }
        ++i;
    }

    ofstream  index_out{dir};
    for(const auto &it:_index)
    {
        index_out << it.first << " ";
        for(int value: it.second)
        {
            index_out << value <<" ";
        }
        index_out << "\n";
    }
};

void DictProducer::store(string dir)
{
    ofstream dict_out{dir};
    for (auto it : _dict)
    {
        dict_out << it.first << " " << it.second << "\n";
    }
    dict_out.close();
};

void DictProducer::showFiles()
{ // 测试用。打印语料库路径
    for (auto it : _files)
    {
        std::cout << it.data() << "\n";
    }
}

void DictProducer::showDict()
{ // 测试用，打印字典内容
    for (auto it : _dict)
    {
        cout << it.first << " : " << it.second << "\n";
    }
    cout << _dict.size() << "\n";
    cout << _dict.capacity() << "\n";

    cout << _dict[0].first << " " << _dict[0].second << "\n";
}

#endif // __DICTPRODUCER_H__
