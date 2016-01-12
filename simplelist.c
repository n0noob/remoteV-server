#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int pwdir(char * path)
{
	DIR * d;
	d = opendir(path);
	if(!d)
	{
		fprintf(stderr, "Cannot open directory %s\n Error: %s", path, strerror(errno));
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
		if(!(node->d_type & DT_DIR))
		{
			printf("%s\n", str);
		}
	}
}


int main()
{
	pwdir("/home/anoop/");
	return 0;
}