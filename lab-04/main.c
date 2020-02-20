// This code is implemented for fourth lab of EECS 678 Lab - University of Kansas

#define LAB_CODE

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// Recursive file counter
int getNumOfFilesRec(char *path);

// We assume argv[1] is the path to the directory to count
int main(int argc,char* argv[]) {
    int numOfFiles=0;

    if(argc != 2){
        fprintf(stderr, "Usage: %s absolute_path_to_directory\n", argv[0]);
        return EXIT_FAILURE;
    }

#ifdef LAB_CODE
    // Create a pipe,fork, and call the function. The child will write the returned
    // value(number of files) to the parent. Parent should read it and wait for child
    // to finish.
    int pipe_fd[2]; // pipe_fd[0] ~ read , pipe_fd[1] ~ write
    pipe(pipe_fd);
    pid_t id = fork();
     switch (id)
     {
        case -1: {// Error
            perror("Failed to create fork in function 'main'.");
            break;
        }
        case 0: {// Child Process
            int ret = getNumOfFilesRec(argv[1]);
            write(pipe_fd[1], &ret, sizeof(int));
            exit(0);
        }
        default: {// Parent Process ( id > 0 )
            read(pipe_fd[0], &numOfFiles, sizeof(int));
            wait(NULL);
            break;
        }
     }

#else
    numOfFiles = getNumOfFilesRec(argv[1]);
#endif

    if(numOfFiles < 0){
        printf("An error occurred.\n");
        return EXIT_FAILURE;
    }

    printf("Number of files: %d\n", numOfFiles);
    return EXIT_SUCCESS;
}

int getNumOfFilesRec(char path[PATH_MAX]){
    DIR* directoryPtr; /*DIR* of opened directory*/
    struct dirent* direntPtr;/*Reading a directory*/
    struct stat statBuf; /*Get status information of a file*/
    int numOfFiles=0;
    char targetPath[PATH_MAX];
#ifdef LAB_CODE
    // Create a pipe for all children. No need to create an external
    // pipe for each children.
    int fd[2];
    pipe(fd);
    int num_children = 0;
    pid_t pid;
    int ret;

#endif

    printf("Process %u counting files in path: %s\n", getpid(), path);

    /*Open given directory*/
    if((directoryPtr = opendir(path)) == NULL){
        perror("Failed to open file directory");
        return -1;
    }

    // the direntPtr can point to a file or directory
    while((direntPtr = readdir(directoryPtr)) != NULL){
        if(strcmp(".",direntPtr->d_name) == 0 || // skip these directories
           strcmp("..",direntPtr->d_name) == 0)
            continue;

        sprintf(targetPath, "%s/%s", path, direntPtr->d_name);
        if(stat(targetPath, &statBuf) == -1){ /*Get file mode (file or directory)*/
            perror("Failed to get file status");
            goto finish;
        }

        // check whether it is directory or not
        if(S_ISDIR(statBuf.st_mode)){
            /*Directory found*/
#ifdef LAB_CODE
            // Create a child process to get into the directory
            // to count files. Write the returned number of files
            // to parent via pipe
            pid = fork();

            switch (pid)
            {
                case -1: {// ERROR
                    perror("Failed to create fork in function 'getNumOfFilesRec'.");
                    break;
                }
                case 0: {// Child process
                    getNumOfFilesRec(targetPath);
                    read(fd[0], &ret, sizeof(int));
                    break;
                }
                default: {// Parent process
                    ++num_children;
                    break;
                }
            }
#else
            int returnVal = getNumOfFilesRec(targetPath);
            if (returnVal == -1) {
                numOfFiles = -1;
                goto finish;
            }
            numOfFiles += returnVal;
#endif
        }
        else{ /*File found*/
#ifdef DEBUG
            printf("File found: %s\n",targetPath);
#endif
            ++numOfFiles;
        }
    }

#ifdef LAB_CODE
    // read the number of files integer coming from children and
    // wait all of them to finish execution
    //write(fd[1], )
    for(int i = 0; i < num_children; i++) {
        int tmp;
        read(fd[0], &tmp, sizeof(int));
        wait(NULL);
        numOfFiles += tmp;
    }

#endif

finish:
    /*Close directory*/
    if(closedir(directoryPtr) == -1){
        perror("Failed to close directory");
        return -1;
    }

    return numOfFiles;
}
