# 遍历目录

## 编译

gcc list.c –o list

## 使用

./list [OPTION]... [FILE]...

## 解释

* FILE 
  * 无指定文件：列出当前目录下所有文件（不含点打头的文件，如.profile）
  * 普通文件：列出文件
  * 目录：列出目录下所有文件
* OPTION
  * -r  递归方式列出子目录（每项要含路径，类似find的-print输出风格，需要设计递归程序）
  * -a  列出文件名第一个字符为圆点的普通文件（默认情况下不列出文件名首字符为圆点的文件）
  * -l   后跟一整数，限定文件大小的最小值（字节）
  * -h  后跟一整数，限定文件大小的最大值（字节）
  * -m 后跟一整数n，限定文件的最近修改时间必须在n天内
  * --  显式地终止命令选项分析

## 运行举例

* `./list` 列出当前目录下所有文件（不含点打头的文件，如.profile）
  ![1](img/1.png)
* `./list README.md list list.c test/test1 test/test2` 列出指定的文件名和所有指定目录下的文件
  ![2](img/2.png)
* `./list -r test` 递归的列出test目录下的所有文件
  ![3](img/3.png)
* `./list -a` 列出当前目录下所有文件（包括点打头的文件，如.profile）
  ![4](img/4.png)
* `./list –l 100 –h 5000 /bin /etc`  列出大小在100~5000之间的文件
  ![5](img/5.png)
* `./list –a -r -l 1000 –m 2`       递归式列出当前目录树下大小超1KB且2天内修改过的文件（包括文件名首字符为圆点的文件）
  ![6](img/6.png)
* `./list  --  -l` “--”后的参数不作为选项处理
  ![7](img/7.png)
* `./list  *` “*”展开为当前目录下所有文件（不含点打头的文件，如.profile）
  ![8](img/8.png)

## 代码

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>

#define FILE_SIZE_MAX LONG_MAX        // The maximum size of a file
#define FILE_SIZE_MIN 0               // The minimum size of a file
#define TIME_MAX LONG_MAX             // The maximum time limit
#define PATH_MAX 4096                 // The maximum length of an error path
#define ERROR_MESSAGE_LENGTH PATH_MAX // The maximum length of an error message

#define TRUE 1
#define FALSE 0

typedef char Status;

// Error type
typedef enum Error
{
    SUCCESS,
    OPTION_INVALID,
    ARGUMENT_INVALID,
    ARGUMENT_MISSING,
    ARGUMENT_TOO_LARGE,
    PATH_TOO_LONG,
    PATH_INVALID,
    PATH_UNACCESSIBLE,
} Error;

long min_size = FILE_SIZE_MIN;                // -l option
long max_size = FILE_SIZE_MAX;                // -h option
long time_limit = TIME_MAX / (3600 * 24);     // -m option
char is_all = FALSE;                          // -a option
char is_recursive = FALSE;                    // -r option
int path_amount = 1;                          // amount of path argument
char **paths;                                 // path argument to list
char error_message[ERROR_MESSAGE_LENGTH + 1]; // store error message

// Analyze the arguments
// param:
//     argc - the amount of arguments
//     argv - the arguments
void CmdArgumentAnalyze(const int argc, const char **const argv);

// List files within the given paths
void List();

int main(int argc, char **argv)
{
    CmdArgumentAnalyze(argc, argv);

    List();

    return 0;
}

// Print the error message
// param:
//    error_str - the string to store the error message
//    error - the error code
void ErrorPrint(char *error_str, const Error error)
{
    switch (error)
    {
    case OPTION_INVALID:
        sprintf(error_str, "ls: invalid option '%s'", error_message);
        break;
    case ARGUMENT_INVALID:
        sprintf(error_str, "ls: invalid argument '%s'", error_message);
        break;
    case ARGUMENT_MISSING:
        sprintf(error_str, "ls: option '%s' missing argument", error_message);
        break;
    case ARGUMENT_TOO_LARGE:
        sprintf(error_str, "ls: argument '%s' too large", error_message);
        break;
    case PATH_TOO_LONG:
        sprintf(error_str, "ls: path '%s..' too long", error_message);
        break;
    case PATH_INVALID:
        sprintf(error_str, "ls: invalid path '%s'", error_message);
        break;
    case PATH_UNACCESSIBLE:
        sprintf(error_str, "ls: unaccessible path '%s'", error_message);
        break;
    }
}

// Check if the error is not SUCCESS and print the error message
// param:
//     err - the error code
#define ERROR_CHECK(err)                                  \
    do                                                    \
    {                                                     \
        Error error = err;                                \
        if (error != SUCCESS)                             \
        {                                                 \
            char error_str[2 * ERROR_MESSAGE_LENGTH + 1]; \
            ErrorPrint(error_str, error);                 \
            fprintf(stderr, "%s\n", error_str);           \
            exit(EXIT_FAILURE);                           \
        }                                                 \
    } while (0)

// check the argument is a option
// param:
//     x - the argument to check
#define IS_OPTION(x) ((x)[0] == '-' && (x)[1] != '\0')

// Parse the option argument after the option
// param:
//     val - the variable to store the result
//     index - the index of the option
//     argc - the amount of arguments
//     argv - the argument to parse
#define PARSE_OPTION_ARGUMENT(val, index, argc, argv)      \
    do                                                     \
    {                                                      \
        if ((index) + 1 >= (argc))                         \
        {                                                  \
            strcpy(error_message, (argv)[(index)]);        \
            return ARGUMENT_MISSING;                       \
        }                                                  \
                                                           \
        (index)++;                                         \
        char *end_pointer;                                 \
        (val) = strtol((argv)[(index)], &end_pointer, 10); \
        /* check if the argument is invalid */             \
        if (*end_pointer != '\0' || (val) < 0)             \
        {                                                  \
            strcpy(error_message, (argv)[(index)]);        \
            return ARGUMENT_INVALID;                       \
        }                                                  \
        /* check if the argument is too large */           \
        else if ((val) == LONG_MAX)                        \
        {                                                  \
            strcpy(error_message, (argv)[(index)]);        \
            return ARGUMENT_TOO_LARGE;                     \
        }                                                  \
    } while (0)

// Analyze the options
// param:
//     index - the index of the argument to analyze
//     argc - the amount of arguments
//     argv - the arguments
// return:
//     the error code
const Error CmdOptionAnalyze(int *index, const int argc, const char **const argv)
{
    for (; *index < argc && IS_OPTION(argv[*index]); (*index)++)
    {
        switch (argv[*index][1])
        {
        case 'a':
            is_all = 1;
            break;
        case 'r':
            is_recursive = 1;
            break;
        case 'l':
            PARSE_OPTION_ARGUMENT(min_size, *index, argc, argv);
            break;
        case 'h':
            PARSE_OPTION_ARGUMENT(max_size, *index, argc, argv);
            break;
        case 'm':
            PARSE_OPTION_ARGUMENT(time_limit, *index, argc, argv);
            break;
        case '-':
            (*index)++;
            return SUCCESS;
        default:
            strcpy(error_message, argv[*index]);
            return OPTION_INVALID;
        }
    }
    return SUCCESS;
}

// Analyze the paths
// param:
//     index - the index of the argument to analyze
//     argc - the amount of arguments
//     argv - the arguments
// return:
//     the error code
const Error CmdPathAnalyze(int *index, const int argc, const char **const argv)
{
    if (*index == argc)
    {
        // No path argument
        path_amount = 1;
        paths = (char **)malloc(sizeof(char *));
        paths[0] = ".";
    }
    else
    {
        path_amount = argc - *index;
        paths = argv + *index;
        for (int i = 0; i < path_amount; i++)
        {
            // check if the path is too long
            if (strlen(paths[i]) > PATH_MAX)
            {
                strncpy(error_message, paths[i], ERROR_MESSAGE_LENGTH);
                error_message[ERROR_MESSAGE_LENGTH] = '\0';
                return PATH_TOO_LONG;
            }
        }
    }
    return SUCCESS;
}

// Analyze the arguments
void CmdArgumentAnalyze(const int argc, const char **const argv)
{
    int i = 1;
    // Analyze options
    ERROR_CHECK(CmdOptionAnalyze(&i, argc, argv));
    // Analyze paths
    ERROR_CHECK(CmdPathAnalyze(&i, argc, argv));
}

// Path node information
typedef struct Path
{
    struct stat st;
    char *path_str;
    struct Path *next;
} Path;

// Classify the paths into files and dictionaries
// param:
//    file_paths - the header of the file path list
//    dict_paths - the header of the directory path list
// return:
//    the error code
const Error PathClassify(Path *file_paths, Path *dict_paths)
{
    for (int i = 0; i < path_amount; i++)
    {
        Path *path = (Path *)malloc(sizeof(Path));
        path->path_str = paths[i];
        if (stat(path->path_str, &path->st) == -1)
        {
            strcpy(error_message, path->path_str);
            return PATH_INVALID;
        }
        path->next = NULL;
        // if the path is a directory, add it to the directory list
        // otherwise add it to the file list
        if (S_ISDIR(path->st.st_mode))
        {
            dict_paths->next = path;
            dict_paths = path;
        }
        else
        {
            file_paths->next = path;
            file_paths = path;
        }
    }
    return SUCCESS;
}

// transform the day to second
#define DAY_TO_SECOND(day) ((day)*3600*24)

// Check the file is valid to print
// param:
//     st - the file information
// return:
//     the file is valid or not
#define FILE_CHECK(st) ((time(NULL) - (st).st_mtime) <= DAY_TO_SECOND(time_limit) && (st).st_size <= max_size && (st).st_size >= min_size)

// List file paths
// param:
//     file_paths - the header of the file path list
// return:
//     the error code
const Error FileList(const Path *file_paths)
{
    file_paths = file_paths->next;
    while (file_paths != NULL)
    {
        if (FILE_CHECK(file_paths->st))
        {
            printf("%s ", file_paths->path_str);
        }
        file_paths = file_paths->next;
    }
    return SUCCESS;
}

// List all the files under the dictionary either recursively or not recursively
// param:
//     dic_path - the dictionary path
// return:
//     the error code
const Error DictList(const char *const path)
{
    DIR *dir = opendir(path);
    // Check the dictionary is accessible
    if (!dir)
    {
        strcpy(error_message, path);
        return PATH_UNACCESSIBLE;
    }

    struct dirent *dp;
    dp = readdir(dir);
    while (dp != NULL)
    {
        // Ignore the file starts with '.' if the option -a is not set
        if (dp->d_name[0] == '.' && !is_all)
        {
            dp = readdir(dir);
            continue;
        }

        // Get the subpath
        if (strlen(path) + strlen(dp->d_name) + 1 > PATH_MAX)
        {
            strcpy(error_message, path);
            strcat(error_message, "/");
            strncat(error_message, dp->d_name, ERROR_MESSAGE_LENGTH - strlen(error_message));
            return PATH_TOO_LONG;
        }
        char subpath[PATH_MAX];
        strcpy(subpath, path);
        strcat(subpath, "/");
        strcat(subpath, dp->d_name);

        struct stat st;
        stat(subpath, &st);
        if (S_ISDIR(st.st_mode))
        {
            if (is_recursive)
            {
                if (FILE_CHECK(st))
                {
                    printf("%s\n", subpath);
                }
                // Ignore the '.' and '..' directories
                if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
                {
                    DictList(subpath);
                }
            }
            else if (FILE_CHECK(st))
            {
                printf("%s ", dp->d_name);
            }
        }
        else
        {
            if (FILE_CHECK(st))
            {
                if (is_recursive)
                {
                    printf("%s\n", subpath);
                }
                else
                {
                    printf("%s ", dp->d_name);
                }
            }
        }
        dp = readdir(dir);
    }
    closedir(dir);
    return SUCCESS;
}

// List files
void List()
{
    // classify the paths into files and dictionaries
    Path *file_paths = (Path *)malloc(sizeof(Path));
    Path *dict_paths = (Path *)malloc(sizeof(Path));
    file_paths->next = NULL;
    dict_paths->next = NULL;
    ERROR_CHECK(PathClassify(file_paths, dict_paths));

    // list the files
    if (file_paths->next != NULL)
    {
        ERROR_CHECK(FileList(file_paths));
        printf("\n\n");
    }

    // list all the files under the dict paths
    if (dict_paths->next != NULL)
    {
        if (path_amount == 1)
        {
            ERROR_CHECK(DictList(dict_paths->next->path_str));
            printf("\n\n");
        }
        else
        {
            Path *dict_path = dict_paths->next;
            while (dict_path != NULL)
            {
                printf("%s:\n", dict_path->path_str);
                ERROR_CHECK(DictList(dict_path->path_str));
                dict_path = dict_path->next;
                printf("\n\n");
            }
        }
    }

    // free the malloced memory
    Path *next;
    while (file_paths != NULL)
    {
        next = file_paths->next;
        free(file_paths);
        file_paths = next;
    }

    while (dict_paths != NULL)
    {
        next = dict_paths->next;
        free(dict_paths);
        dict_paths = next;
    }
}
```