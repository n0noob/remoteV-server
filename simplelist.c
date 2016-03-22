#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include "simplelist.h"

#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define LIST_PATH "./mediafile.list"


char *m_files[] = {".mp4", ".mkv", ".mp3", NULL};

bool mediafile_check(char const *file)
{
	int ext_len = 0;
	char **temp = m_files;
	int file_len = strlen(file);
	while (*temp)
	{
		ext_len = strlen(*temp);
		if (file_len > ext_len)
		{
			if(strcmp(file + file_len - ext_len, *temp) == 0)
				return true;
		}
		temp++;
	}
	return false;
}

int pwdir(char * path)
{
	DIR * d;
	FILE *fp;
	fp = fopen(LIST_PATH, "w");
	d = opendir(path);
	if(!d)
	{
		fprintf(stderr, "Cannot open directory %s\n Error: %s", path, strerror(errno));
		return -1;
	}
	while (1)
	{
		struct dirent * node;
		char * str;
		node = readdir(d);
		if(!node)
		{
			break;
		}
		str = node->d_name;
		
		if(!(str[0] == '.'))
		{
			if((node->d_type & DT_REG) && (mediafile_check(str)))
			{
				if(*(path + strlen(path) - 1) != '/')
					fprintf(fp, " %s/%s\n", path, str);
					//printf(" %s%s/%s%s\n", ANSI_COLOR_CYAN, path, str, ANSI_COLOR_RESET);
				else
					fprintf(fp, " %s%s\n", path, str);
					//printf(" %s%s%s%s\n", ANSI_COLOR_CYAN, path, str, ANSI_COLOR_RESET);
				
			}

			else if(node->d_type & DT_DIR)
			{
				char new_path[PATH_MAX];
				int path_len = 0;
				if(*(path + strlen(path) - 1) != '/')
					path_len = snprintf(new_path, PATH_MAX, "%s/%s", path, str);
				else
					path_len = snprintf(new_path, PATH_MAX, "%s%s", path, str);

				if(path_len > PATH_MAX)
				{
					fprintf(stderr, "Path length exceeded for directory: %s\n Error: %s", str, strerror(errno));	
					break;
				}
				pwdir(new_path);
			}
		}
	}
}


int main()
{
	pwdir("/home/anoop");
	return 0;
}