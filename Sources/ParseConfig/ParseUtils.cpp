#include "ParseConfig.hpp"

void exitError(std::string message) {
    std::cerr << message << std::endl;
    exit(1);
}

bool isDirectory(std::string path) {
    int			stat_res;
    struct stat	file_stat;
    int fd;

    if (path[0] != '/' || path[path.size() - 1] == '/')
        return (false);
    fd = open(path.c_str(), O_RDONLY);
    stat_res = fstat(fd, &file_stat);
    close(fd);
    if (stat_res == -1)
        return (false);
    if (S_ISDIR(file_stat.st_mode))
        return (true);
    return (false);
}

static int isFile(std::string path) {
    int			stat_res;
    struct stat	file_stat;
    int fd;

    if (path[0] != '/')
        return (-1);
    fd = open(path.c_str(), O_RDONLY);
    stat_res = fstat(fd, &file_stat);
    close(fd);
    if (stat_res == -1)
        return (-1);
    if (S_ISDIR(file_stat.st_mode))
        return (-1);
    return (file_stat.st_mode);
}

bool isFileRead(std::string path) {
    int mode = isFile(path);
    if (mode == -1 || !(mode & S_IRUSR))
        return (false);
    return (true);
}

bool isFileExec(std::string path) {
    int mode = isFile(path);
    if (mode == -1 || !(mode & S_IXUSR))
        return (false);
    return (true);
}