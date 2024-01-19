#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fdd;
	fdd = open("/dev/func", O_RDWR);
	if (fdd < 0)
		printf("ERROR");
	char tmp[6];	
	read(fdd,tmp,6);
	for(int i=0;i<5;i++){
	    printf("%c", tmp[i]); 
	}
	close(fdd);	
	return 0;
}
