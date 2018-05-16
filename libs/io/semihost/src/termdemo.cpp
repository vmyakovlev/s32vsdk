
#include "string.h"
#include "t32term.h"

void _sys_exit (void)
{
	while (1);
}


int main()
{
	static char hmessage[] = "\r\nHello World!\r\n";
	static char mmessage[] = "\r\nr read file\r\nw write file\r\np performance test\r\n";
	static char pmessage[] = "Performance Test Text         \r\n";
	char buffer[4096];
	int i, ch, handle;

	T32_Term_Puts( hmessage );

	while (1)
	{
		T32_Term_Puts( mmessage );

		ch = T32_Term_Getchar();

		if (ch == 'r' || ch == 'R')
		{
			handle = T32_Term_OpenFile("test1",T32_TERM_O_RDONLY);
			if (handle == -1) {
				T32_Term_Puts( "failed to open file\r\n" );
				continue;
			}
			i = T32_Term_ReadFile(handle,buffer,sizeof(buffer));
			if (i == -1) {
				T32_Term_Puts( "failed to read file\r\n" );
				continue;
			}
			buffer[i] = '\0';
			T32_Term_Puts(buffer);

			i = T32_Term_CloseFile(handle);
			if (i == -1) {
				T32_Term_Puts( "failed to close file\r\n" );
				continue;
			}
			continue;
		}

		if (ch == 'w' || ch == 'W')
		{
			handle = T32_Term_OpenFile("test2",T32_TERM_O_RDWR|T32_TERM_O_CREATE_TRUNC);
			if (handle == -1) {
				T32_Term_Puts( "failed to create file\r\n" );
				continue;
			}
			i = T32_Term_WriteFile(handle,hmessage,strlen(hmessage));
			if (i == -1) {
				T32_Term_Puts( "failed to write file\r\n" );
				continue;
			}

			i = T32_Term_CloseFile(handle);
			if (i == -1) {
				T32_Term_Puts( "failed to close file\r\n" );
				continue;
			}
			continue;
		}

		if (ch == 'p' || ch == 'P')
		{
			for ( i = 0 ; i < 32 ; i++ )
				T32_Term_PutBlock( (unsigned char *) pmessage, 32 );
			continue;
		}
	}
}

