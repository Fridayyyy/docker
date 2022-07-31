#include <sys/wait.h>       //waitpid
#include <sys/mount.h>      //mount
#include <fcntl.h>          //open
#include <unistd.h>         //execv,sethostname,chroot,fchdir
#include <sched.h>          //clone

#include <cstring>
#include <string>

#define STACK_SIZE ( 512 * 512 )

namespace docker{
    typedef int proc_statu;
    proc_statu proc_err = -1;
    proc_statu proc_exit = 0;
    proc_statu proc_wait = 1;

    typedef struct container_config{
        std::string host_name;//主机名
        std::string roo_dir;  //容器根目录
    } container_config;

    class container{
    private:
        //
        typedef int process_pid;

        //
        char child_stack[STACK_SIZE];

        //
        container_config config;

        void start_bash(){
            std::string bash = "/bin/bash";
            char *c_bash = new char[bash.length()+1]; //+1 to put '\0'
            strcpy(c_bash,bash.c_str());

            char* const child_args[] = { c_bash,NULL };
            execv(child_args[0],child_args);
            delete []c_bash;

        }

    public:
        container(container_config &config){
            this->config = config;
        }
        void start(){
            auto setup = [](void *args)->int {
                auto _this = reinterpret_cast<container *>(args);
                _this->start_bash();
                return proc_wait;
            };

            process_pid child_pid = clone(setup,child_stack+STACK_SIZE,
                                          SIGCHLD,
                                          this);
            waitpid(child_pid, nullptr,0);
        }
    };
}

