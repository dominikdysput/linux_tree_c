//implementation command "tree"from linux in C
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>

#define KATALOG_COLOR "\033[94m"
#define FILE_COLOR "\x1B[32m" 
#define RESET "\033[0m"

int flagd = 0; int flagf = 0; 
static int how_much_files = 0;
static int how_much_directory = 0;
static int which_level = 0;
void make_space(int which_level)
{
	for (int i = 0; i < which_level; i++)
	{
		printf(" ");
	}
}
int listdir(const char *name, int which_level)
{
    DIR *dir;
    struct dirent *entry;
	struct stat fileStat;
    if (!(dir = opendir(name)))
        return which_level;
	
    while ((entry = readdir(dir)) != NULL) 
	{
		if (flagd == 1)
		{
			if (which_level>0)
			{
				make_space(which_level);
			}
			if (entry->d_type == DT_DIR) 
			{
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
					continue;
				char path[PATH_MAX];				
				snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);				
				if(flagf == 1)
				{
					printf("|--%s%s\n",KATALOG_COLOR, path);
				}
				else if(flagf==0)
				{
					printf("|--%s%s\n",KATALOG_COLOR, entry->d_name);
				}
				printf("%s", RESET);				
				how_much_directory++;
				listdir(path, which_level + 1);					
			} 
		}
		else if(flagd==0)
		{
			if (which_level>0)
			{
				make_space(which_level);
			}
			if (entry->d_type == DT_DIR) 
			{

				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
					continue;
				char path[PATH_MAX];				
				snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);				
				if(flagf == 1)
				{
					
					printf("|--%s%s\n",KATALOG_COLOR, path);
				}
				else if(flagf==0)
				{
					printf("|--%s%s\n",KATALOG_COLOR, entry->d_name);
				}
				printf("%s", RESET);
				how_much_directory++;
				listdir(path, which_level + 1);					
			} 
			else if (fileStat.st_mode & S_IXUSR)	
			{
				if(flagf==1)
				{
					char path[PATH_MAX];
					snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
					printf("|--%s%s\n",FILE_COLOR, path);
				}
				else if(flagf==0)
				{
					printf("|--%s%s\n",FILE_COLOR, entry->d_name);
				}
				printf("%s", RESET);
				how_much_files++;					
			}
			else 
			{
				if(flagf==1)
				{
					char path[PATH_MAX];
					snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
					printf("|--%s\n",path);
				}
				else if(flagf==0)
				{	
					printf("|--%s\n",entry->d_name);
			
				}
				how_much_files++;				
			}
		}
    }
    closedir(dir);
	return which_level;
}
int main(int argc,char **argv)
 {
	int ret, index;
	char *cvalue = NULL; char *foldername=NULL;
	opterr = 0; //no default "invalid option" info
	while ((ret = getopt (argc, argv, "dfL")) != -1)
		switch (ret) {
			case 'd': flagd = 1; break;
			case 'f': flagf = 1; break;
			case '?':
				fprintf (stderr, "Unknown option `-%c'.\n",optopt);
				return 1;
			default: abort ();
		}
	for (index = optind; index < argc; index++)
		{
			foldername = argv[index]; break;
		}

	if(foldername==NULL)
		foldername=".";


	listdir(foldername, which_level);
	

	(flagd == 0) ? printf("\n\n%d directories, %d files\n", how_much_directory, how_much_files) : 
	 printf("\n\n%d directories\n", how_much_directory);		

    return 0;
}
