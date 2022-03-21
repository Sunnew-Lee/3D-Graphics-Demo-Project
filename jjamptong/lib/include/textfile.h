/******************************************************************************/
/*!
\file   textfile.hpp
\par    Purpose: Text file read functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    email: sang.le@digipen.edu
\par    DigiPen login: sang.le
\date   28/10/2014
*/
/******************************************************************************/
#include <stdio.h>


/******************************************************************************/
/*!
\fn     ReadTextFile
\brief
        Read the content of a text file.
\param  const char fn[]
        Text filename.
\return
        The content of the given text file.
*/
/******************************************************************************/
const char *ReadTextFile(const char fn[]) 
{
	FILE *fp;
	char *content = NULL;
	int count=0;

	if (fn != NULL) 
    {
		fp = fopen(fn,"rt");
		if (fp != NULL) 
        {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

			if (count > 0) 
            {
				content = new char[count + 1];
				count = static_cast<int>(fread(content,sizeof(char),count,fp));
				content[count] = '\0';
			}

			fclose(fp);
		}
	}
	return content;
}
