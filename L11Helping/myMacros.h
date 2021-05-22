#ifndef MYMACROS_H_
#define MYMACROS_H_

#define PRINT_INFO(MSG) \
        ( { printf("[PID: %d] %s\n", getpid(), MSG ); } )

#define FAIL_IF(EXP, MSG_OK, MSG_FAIL) \
        ( { if((EXP) == -1) { printf("[PID: %d] %s. Reason: %s\n", getpid(), MSG_FAIL, strerror(errno)); exit(EXIT_FAILURE); }\
            else{ printf("[PID: %d] %s\n", getpid(), MSG_OK);} } )

#endif