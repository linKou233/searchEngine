#if !defined(__SPLITTOOL_H__)
#define __SPLITTOOL_H__

#include "../cppjieba/Jieba.hpp"

#include <vector>
#include <string>


using std::vector, std::string;
using cppjieba::Jieba;

class Configuration;    //前向声明

class SplitTool
{ 
public:
    virtual ~SplitTool() {};
    virtual vector<string> cut(const string& sentence) = 0 ;
};


#endif // __SPLITTOOL_H__
