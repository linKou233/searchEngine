#include <workflow/WFFacilities.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/Workflow.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <vector>

using std::cin, std::cout, std::cerr;
using std::vector, std::string;

static WFFacilities::WaitGroup waitGroup(1);

void sighandler(int signo) {
    cout << "done! \n."; 
    waitGroup.done();
}

//编写任务的回调函数

int main() {
    signal(SIGINT, sighandler);
//创建任务
//启动任务
    waitGroup.wait();
    cout << "finished!\n";
    return 0;
}