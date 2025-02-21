#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>

#pragma pack(1)
typedef struct{
	unsigned char B;
	unsigned char G;
	unsigned char R;
}RGB;

#define W 1440
#define H 900

void mzk(RGB (*buf)[W], int pos_x, int pos_y, int len)
{
	RGB color = buf[pos_y][pos_x];//{.R=0xff};	

	int i,j;
	for(i=0; i<H; i++)
	{
		for(j=0; j<W; j++)
		{
			if( 
			(i>=pos_y && i<= (pos_y+len) )
				&&
			 (j>=pos_x && j<= (pos_x+len) )	)
				buf[i][j] = color;
			
		}
	}
}

int main(int argc, char *argv[])
{
	if(2 != argc)
	{
		printf("Usage: %s <file>\n", argv[0]);
		return -1;
	}

	//open source
	int fds = open(argv[1], O_RDONLY);
	if(-1 == fds)
	{
		perror("open src file");
		return -1;
	}

	//create new file
	char destfile[20] = {0};
	strcat(strcat(destfile, "mzk."), argv[1]);
	int fdd = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(-1 == fdd)
	{
		perror("open des file");
		return -1;
	}

	//copy BMP head
	char header[54];
	read(fds, header, sizeof header);
	write(fdd, header, sizeof header);

	//read src
	RGB buf[H][W];
//	lseek(fds, 54, SEEK_SET);
	int ret;
	ret = read(fds, buf, sizeof buf);
	
	mzk(buf, 500, 500, 100);

	write(fdd, buf, ret);
	

	//close src & new
	close(fds);
	close(fdd);
}
