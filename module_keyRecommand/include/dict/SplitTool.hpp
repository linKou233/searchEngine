#if !defined(__SPLITTOOL_H__)
#define __SPLITTOOL_H__

#include "../cppjieba/Jieba.hpp"

#include <vector>
#include <string>

using std::vector, std::string;
using cppjieba::Jieba;

class SplitTool
{ 
public:
    virtual ~SplitTool() {};
    virtual vector<string> cut(const string& sentence) = 0 ;
};


class SplitToolJieba : public SplitTool
{
public:
    SplitToolJieba();
    ~SplitToolJieba();
    
    vector<string> cut(const string &sentence) override;

private:
    static Jieba* jieba;
};

Jieba *SplitToolJieba::jieba = nullptr;

SplitToolJieba::SplitToolJieba()
{   
    //Jieba初始化
    const char *const DICT_PATH = "../data/dict/jieba.dict.utf8";
    const char *const HMM_PATH = "../data/dict/hmm_model.utf8";
    const char *const USER_DICT_PATH = "../data/dict/user.dict.utf8";
    const char *const IDF_PATH = "../data/dict/idf.utf8";
    const char *const STOP_WORD_PATH = "../data/dict/stop_words.utf8";
    
    jieba = new Jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);
};

SplitToolJieba::~SplitToolJieba()
{
    if(jieba)
    {
        delete jieba;
        jieba = nullptr;
    }
}

vector<string> SplitToolJieba::cut(const string &sentence) 
{
    vector<string> words;
    jieba->Cut(sentence, words, true);
    return words;
};


#endif // __SPLITTOOL_H__
