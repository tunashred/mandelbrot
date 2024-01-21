#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

#define MAXFILE_SIZE 500

bool copyFile(char *src_path, char *dest_path) {
	struct stat src_properties;
	if(stat(src_path, &src_properties) != 0) {
		printf("Error getting source file's properties.\n");
		return false;
	}

	if(src_properties.st_size > MAXFILE_SIZE) {
		if(symlink(src_path, dest_path) != 0) {
			printf("Error creating symlink.\n");
			return false;
		}
		return true;
	}

	int fd_src, fd_dest;
	char buffer[4096];
	long numRead;

	int dest_flags = O_CREAT | O_WRONLY | O_TRUNC;
	mode_t dest_permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; /* -rw-rw-rw- */

	fd_src = open(src_path, O_RDONLY);
	if(fd_src < 0) {
		printf("Error opening file.\n");
		return false;
	}

	fd_dest = open(dest_path, dest_flags, dest_permissions);
	if(fd_dest < 0) {
		close(fd_src);
		printf("Failed to create destination file.\n");
		return false;
	}

	while( (numRead = read(fd_src, buffer, sizeof(buffer))) > 0 ) {
		if(write(fd_dest, buffer, numRead) != numRead) {
			printf("Partial write or error occured.\n");
			close(fd_src);
			close(fd_dest);
			return false;
		}
	}

	if(chmod(dest_path, S_IRUSR | S_IRGRP | S_IROTH) != 0) { /* -r--r--r-- */
		printf("Error changing permissions.\n");
		return false;
	}

	close(fd_src);
	close(fd_dest);

	return true;
}

void recursiveTraversal(char *src_path, char *dest_path) {
	DIR *src_dir;

	if((src_dir = opendir(src_path)) == NULL) {
		return;
	}

	// properties: st_mode, st_size
	// info: name, 
	struct stat src_properties;
	struct dirent *src_info = readdir(src_dir);

	while((src_info = readdir(src_dir))) {
		if (strncmp(src_info->d_name, ".", 2) == 0 || strncmp(src_info->d_name, "..", 3) == 0) {
            continue;
        }

        char src_full_path[256];
        sprintf(src_full_path, "%s/%s", src_path, src_info->d_name);

		char dest_full_path[256];
		sprintf(dest_full_path, "%s/%s", dest_path, src_info->d_name);

        stat(src_full_path, &src_properties);

        if(S_ISDIR(src_properties.st_mode)) {
			mkdir(dest_full_path, src_properties.st_mode);
            recursiveTraversal(src_full_path, dest_full_path);
        }
		else {
			copyFile(src_full_path, dest_full_path);
		}
	}

	closedir(src_dir);
}

int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("Bad parameters.\n");
		return -1;
	}
	
	recursiveTraversal(argv[1], argv[2]);

	return 0;
}
