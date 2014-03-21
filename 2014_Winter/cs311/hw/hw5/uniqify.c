/*
*  Name: Matt Schreiber
*  Email: schreibm@onid.oregonstate.edu
*  Class: CS311-400
*  Assignment: 5
*/

/* Needed in order to access tdestroy() */
#define _GNU_SOURCE
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <search.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFLEN      200
#define MSG_SZ     (sizeof(struct mbuf) - sizeof(long))
#define MSG_PERMS   S_IRUSR | S_IWUSR

/* I have limited the number of processes to 26 because
 * the sorting  of words depends on the subprocesses
 * receiving only words in a certain alphabetical
 * range (e.g., if there are 3 children, child 1 gets
 * words starting with the letters 'a' through 'i'),
 * and the function that generates the msgtyp that enables
 * this works only on first letters of words.  There's
 * indubitably a better way to do this. */
#define MAX_PROCS   26

typedef struct mbuf {
    long mtype;
    char mtext[BUFLEN];
    int count;
} msg_t;

/* Globals */

/* Identifier of parent queue */
static int parent_queue = -1;
/* Identifier of child queue */
static int child_queue = -1;
/* Array of queues */
static int *queues = NULL;
/* Number of queues */
static int num_queues = -1;
/* Array of pids */
static pid_t *pids = NULL;
/* Parent pid */
static pid_t parent_pid = -1;
/* Child pid */
static pid_t child_pid = -1;
/* Array to hold queue keys */
key_t *keys;

static inline char * get_word(FILE *file, size_t maxlen);
static int compar_mbuf(const void *lhs, const void *rhs);
static int msgsnd_str(int msqid, size_t mtext_len, int msgflg, long mtype, char *str, int count);
static int process_queue(int msqid, size_t maxmsgsz, long msgtyp, int msgflg,
        int(*compar)(const void *, const void *),
        void(*action)(const void *nodep, const VISIT value, const int level));
static int print_msgs(int msqid, size_t maxmsgsz, long msgtyp, int msgflg);
static inline int keygen(key_t **keys, int num_keys);
static inline int msqid_gen(key_t key);
static inline int queue_init(int **queuep, key_t *keys, int num_keys);
static void clean_up_procs(int *queuep, pid_t *pidp, int num_queuep);
static void send_msg_nodes(const void *nodep, const VISIT value, const int level);
static void sig_handler(int sig);
static void subproc_exec(int *queuep, key_t *keys, int num_procs);
static void usage(FILE *stream, const char *prog_name, const char *err, int exit_code);
static void * xmalloc(unsigned n);
static inline void msgtyp_idx_gen(int **idxp, int num_procs);
static inline int remove_queue(int msqid);

int main(int argc, char *argv[])
{
    /* Index */
    int i;
    /* Flags to msgsnd */
    int msgsnd_flag = 0;
    /* Flags to msgrcv */
    int msgrcv_flg = 0;
    /* Number of processes to fork() */
    int num_procs = 1;
    /* String read from input */
    char *input = NULL;
    /* Placeholder for message type identifier */
    long msgtyp_id;
    /* Indices of  */
    int msgtyp_idxs[26];
    int *msgtyp_idxs_p = msgtyp_idxs;

    /* Struct for use with GNU getopt_long() */
    struct option longopts[] = {
        { "number", required_argument,  0,  'n' },
        { 0, 0, 0, 0 }
    };

    /* Variable to hold flag currently
     * being ready by getopt() */
    int c;

    /* Index of option currently
     * being considered */
    int opt_index = 0;

    if(argc != 1 && argc != 3)
        usage(stderr, argv[0], "incorrect number of options", EXIT_FAILURE);


    /*
     * Loop adapted from example given at
     * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     */
    while((c = getopt_long(argc, argv, "n:",
                    longopts, &opt_index)) != -1) {
        switch(c) {
            case 'n':
                num_procs = atoi(optarg);
                break;
            case('?'):
            default:
                usage(stderr, argv[0], "unrecognized option", EXIT_FAILURE);
                break;
        }
    }

    if(num_procs > MAX_PROCS)
        usage(stderr, argv[0], "invalid number of processes", EXIT_FAILURE);

    /* One queue each for parent and children */
    num_queues = num_procs + 1;

    /* Calculate which queue/process a word
     * should be assigned to based on its
     * first letter */
    msgtyp_idx_gen(&msgtyp_idxs_p, num_procs);

    /* Allocate space to store pids of
     * parent and children */
    pids = xmalloc(sizeof(pid_t) * num_queues);
    /* Store parent pid */
    pids[0] = parent_pid = getpid();

    /* Create one key each for parent
     * and child processes */
    keygen(&keys, num_queues);

    /* Create one queue each for parent
     * and child processes */
    queue_init(&queues, keys, num_queues);

    /* Parent queue is first in array */
    parent_queue = queues[0];

    /* Set up handler for SIGCHLD,
     * SIGINT, and SIGTERM */
    signal(SIGCHLD, &sig_handler);
    signal(SIGINT, &sig_handler);
    signal(SIGTERM, &sig_handler);

    /* Start subprocesses */
    subproc_exec(queues, keys, num_procs);

    /* Assign string 'input' a message type that is
     * the pid of one of the subprocesses (that's why there's
     * a '+ 1' in the array index -- the parent pid is pid[0]). */
    while((input = get_word(stdin, BUFLEN)) != NULL) {
        msgtyp_id = (long)pids[msgtyp_idxs[input[0] - 97] + 1];
        /* Create msg_t from 'input' string and
         * place it in parent queue */
        if(msgsnd_str(parent_queue, strlen(input) + 1, msgsnd_flag, msgtyp_id, input, 1) == -1) {
            perror("main: msgsnd");
            clean_up_procs(queues, pids, num_queues);
            exit(EXIT_FAILURE);
        }
        free(input);
    }

    /* Send terminating message to each child */
    for(i = 1; i < num_queues; i++) {
        msgtyp_id = (long)pids[i];
        if(msgsnd_str(parent_queue, 1, msgsnd_flag, msgtyp_id, "", -1) == -1) {
            perror("msgsnd");
            clean_up_procs(queues, pids, num_queues);
            exit(EXIT_FAILURE);
        }
    }

    /* Print messages received from children
     * in alphabetical order */
    for(i = 1; i <= num_procs; i++) {
        if(print_msgs(queues[i], MSG_SZ, pids[i], msgrcv_flg) == -1) {
            perror("main: msgrcv");
            clean_up_procs(queues, pids, num_queues);
            exit(EXIT_FAILURE);
        }
    }

    /* Wait for children to terminate */
    for (i = 0; i < num_procs; i++)
        wait(NULL);

    /* Free memory and destroy queues */
    clean_up_procs(queues, pids, num_queues);
    return 0;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_word
 *  Description:  Reads one character at a time from 'file', stopping when it
 *                encounters a non-letter character.
 * =====================================================================================
 */
static inline char *
get_word(FILE *file, size_t maxlen)
{
    int len = 0;
    char c;
    char *word = NULL;

    word = xmalloc(maxlen);

    while( (c = fgetc(file)) != EOF) {
        if((c >= 'a' && c <= 'z')) {
            word[len] = c;
            len++;
        } else if(c >= 'A' && c <= 'Z') {
            word[len] = tolower(c);
            len++;
        } else if (len > 0) {
            break;
        }
    }

    word[len] = '\0';

    if(c == EOF) {
        free(word);
        return NULL;
    }

    return word;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  compar_mbuf
 *  Description:  Compares two mbuf structs based on a strcmp() of their mtext
 *                fields.
 * =====================================================================================
 */
static int
compar_mbuf(const void *lhs, const void *rhs)
{
    return strcmp(((const msg_t *)lhs)->mtext, ((const msg_t *)rhs)->mtext);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  msgsnd_str
 *  Description:  Converts a string into an mbuf struct with mtype field 'str',
 *                count field 'count', and mtype field 'mtype', then places it
 *                in specified queue.  Returns the status of the call to
 *                msgsnd().
 * =====================================================================================
 */
static int
msgsnd_str(int msqid, size_t mtext_len, int msgflg, long mtype, char *str, int count)
{
    msg_t msg;
    /* To stop Valgrind from complaining... */
    memset(&msg, 0, sizeof(msg));
    msg.mtype = mtype;
    msg.count = count;
    memcpy(msg.mtext, str, mtext_len);
    return msgsnd(msqid, &msg, MSG_SZ, msgflg);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  process_queue
 *  Description:  Reads messages from queue and places them in a search tree
 *                using the tsearch() function.  When all messages have been read, traverses
 *                the tree, sending messages based upon the alphabetical order of the mtext
 *                field.
 * =====================================================================================
 */
static int
process_queue(int msqid, size_t maxmsgsz, long msgtyp, int msgflg,
        int(*compar)(const void *, const void *),
        void (*action)(const void *nodep, const VISIT value, const int level))
{
    int ret;
    msg_t *msg = NULL, *msg_node = NULL;
    void *root = NULL, *node = NULL;
    int msgsnd_flag = 0;

    while(1) {
        /* Each node of the tree must be separately
         * allocated. */
        msg = xmalloc(sizeof(msg_t));
        ret = msgrcv(msqid, msg, maxmsgsz, msgtyp, msgflg);
        if(ret == -1) {
            perror("process_queue: msgrcv");
            _exit(EXIT_FAILURE);
        }
        /* If msg's count field == -1, there are
         * no more messages to process. */
        if(msg->count == -1) {
            free(msg);
            break;
        }
        node = tsearch(msg, &root, compar);
        /* If 'node' is NULL, tsearch failed to insert
         * the message into the tree. */
        if(node == NULL) {
            perror("tsearch");
            _exit(EXIT_FAILURE);
        } else {
            msg_node = *(msg_t **)node;
            /* Check whether the newly-inserted node
             * points to the same place as 'msg'.  If it
             * doesn't, increment the node's count field
             * and free msg. */
            if(msg_node != msg) {
                msg_node->count++;
                free(msg);
            }
        }
    }

    /* Place messages in queue in
     * alphabetical order */
    twalk(root, action);

    /* Free the memory associated
     * with the tree */
    tdestroy(root, &free);

    /* Send terminating message to parent */
    if(msgsnd_str(child_queue, 1, msgsnd_flag, msgtyp, "", -1) == -1) {
        perror("msgsnd_str");
        _exit(EXIT_FAILURE);
    }

    return ret;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_msgs
 *  Description:  Reads messages from queue and prints the 'count' and 'mtext'
 *                field in a pretty-ish format
 * =====================================================================================
 */
static int
print_msgs(int msqid, size_t maxmsgsz, long msgtyp, int msgflg)
{
    msg_t msg;
    int ret;

    while(1) {
        ret = msgrcv(msqid, &msg, maxmsgsz, msgtyp, msgflg);
        if(msg.count == -1)
            break;
        printf("%7d %s\n", msg.count, msg.mtext);
    }

    return ret;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  keygen
 *  Description:  Allocates space for 'num_keys' key_t variables, assigning
 *                IPC_PRIVATE as the value of each key.
 * =====================================================================================
 */
static inline int
keygen(key_t **keyp, int num_keys)
{
    int i;
    *keyp = xmalloc(num_keys * sizeof(key_t));
    for(i = 0; i < num_keys; i++)
        (*keyp)[i] = IPC_PRIVATE;
    return i + 1;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  msqid_gen
 *  Description:  Wrapper for msgget().  Returns an msqid based on the 'key' argument.
 * =====================================================================================
 */
static inline int
msqid_gen(key_t key)
{
    int msqid = msgget(key, MSG_PERMS | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    return msqid;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_init
 *  Description:  Allocates space for 'num_keys' message queues, generating
 *                queue ids for each.
 * =====================================================================================
 */
static inline int
queue_init(int **queuep, key_t *keyp, int num_keys)
{
    int i;
    *queuep = xmalloc(sizeof(int) * num_keys);

    for(i = 0; i < num_keys; i++)
        (*queuep)[i] = msqid_gen(keyp[i]);

    return i + 1;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  clean_up_procs
 *  Description:  Handles terminating children, destroys message queues, and
 *                releases dynamically-allocated global arrays.
 * =====================================================================================
 */
static void
clean_up_procs(int *queuep, pid_t *pidp, int num_queuep)
{
    struct msqid_ds ds_buf;
    int i;
    if(child_pid == -1) {
        for(i = 1; i < num_queuep; i++)
            kill(pidp[i], SIGTERM);
        for(i = 0; i < num_queuep; i++) {
            /* Workaround for checking whether queue exists;
             * I would use msgget, but since the queues were
             * created with IPC_PRIVATE, it seems that that
             * method won't work. */
            if(msgctl(queuep[i], IPC_STAT, &ds_buf) == 0) {
                if(remove_queue(queuep[i]) == -1 && errno != EIDRM) {
                    fprintf(stderr, "Error removing queue %d, process %d:\n", queuep[i], i);
                    perror("msgctl");
                }
            }
        }
    }
    free(keys);
    free(queues);
    free(pids);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  send_msg_nodes
 *  Description:  To be used with twalk() function.  Places the nodes of the
 *                tree in 'child_queue' in alphabetical order of the mtext
 *                field.
 * =====================================================================================
 */
static void
send_msg_nodes(const void *nodep, const VISIT value, const int level)
{
    msg_t *msg;
    size_t len;
    int msgsnd_flag = 0;

    switch (value) {
        case leaf:
        case postorder:
            msg = *(msg_t **) nodep;
            msg->mtype = child_pid;
            len = strlen(msg->mtext) + 1;
            msgsnd(child_queue, msg, MSG_SZ, msgsnd_flag);
        default:
            break;
    }
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_handler
 *  Description:  Signal handler function.  Cleans up the processes and queues
 *                on receipt of SIGINT and SIGTERM, as well as SIGCHLD so long
 *                as errno does not indicate successful child exit.
 * =====================================================================================
 */
static void
sig_handler(int sig)
{
    switch(sig) {
        case SIGCHLD:
            if(errno == 0)
                break;
        case SIGINT:
        case SIGTERM:
            clean_up_procs(queues, pids, num_queues);
            exit(EXIT_FAILURE);
            break;
        default:
            break;
    }
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  subproc_exec
 *  Description:  fork()s 'num_procs' children, each of which gets its own
 *                message queue.  Children process messages through the
 *                process_queue() function, then exit.
 * =====================================================================================
 */
static void
subproc_exec(int *queuep, key_t *keyp, int num_procs)
{
    int i;
    int msgrcv_flg = 0;
    key_t parent_key, child_key;

    for(i = 1; i <= num_procs; i++) {
        switch(pids[i] = fork()) {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
                break;
            case 0:
                /* parent_queue and child_queue
                 * are global variables, as are
                 * the '_key' and '_pid' variables
                 * and 'keys' and 'pids' arrays. */
                parent_queue = queuep[0];
                child_queue = queuep[i];
                parent_key = keyp[0];
                child_key = keyp[i];
                /* Since pid[i] is 0 in child,
                 * call getpid() to get the real one. */
                child_pid = pids[i] = getpid();
                process_queue(parent_queue, MSG_SZ, child_pid, msgrcv_flg, &compar_mbuf, &send_msg_nodes);
                /* Each child has its own copy of these
                 * global arrays, and each must free them. */
                free(keys);
                free(queues);
                free(pids);
                _exit(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  usage
 *  Description:  Prints a usage message and exits
 * =====================================================================================
 */
static void
usage(FILE *stream, const char *prog_name, const char *err, int exit_code)
{
    fprintf(stream, "Error: %s\n", err);
    fprintf(stream, "Usage: %s -n NUMBER\n", prog_name);
    fprintf(stream, "   -n  --number     Number of sorter subprocesses\n");
    exit(exit_code);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  xmalloc
 *  Description:  From the tsearch(3) Linux man page.  Wraps malloc(), retuning
 *                a void pointer on success and exiting on failure.
 * =====================================================================================
 */
static void *
xmalloc(unsigned n)
{
    void *p = NULL;
    if((p = malloc(n)))
        return p;
    perror("xmalloc");
    exit(EXIT_FAILURE);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  msgtyp_idx_gen
 *  Description:  Splits a 26-member array into 'num_procs' groups, assigning
 *                one 0-based index to each group in ascending order.  Used for
 *                determining which queue to place a message in based upon the
 *                first letter of the mtext field.
 * =====================================================================================
 */
static inline void
msgtyp_idx_gen(int **idxp, int num_procs)
{
    int i, j;

    /* 26 -- not a magic number, but
     * the number of letters in the
     * alphabet */
    int interval = 26 / num_procs;
    int remainder, placeholder, alpha_idx;
    remainder = 26 % num_procs;

    if(interval == 0)
        interval++;

    for(i = 0; i < 26; i++) {
        alpha_idx = interval;
        placeholder = remainder;
        for(j = 0; j < num_procs; j++, alpha_idx += interval) {
            if(placeholder > 0) {
                placeholder--;
                alpha_idx++;
            }
            if(i + 1 <= alpha_idx)
                break;
        }
        (*idxp)[i] = j < num_procs ? j : --j;
    }
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  remove_queue
 *  Description:  Wrapper for msgctl(); destroys queue with given msqid
 * =====================================================================================
 */
static inline int
remove_queue(int msqid)
{
    return msgctl(msqid, IPC_RMID, NULL);
}
