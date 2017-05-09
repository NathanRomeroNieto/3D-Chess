// textfile.h: interface for reading and writing text files
// www.lighthouse3d.com
//
// simple reading and writing for text files
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*!
   \brief "Lectura del archivos de Texto - retorno de un puntero de char"
   \param "char *fn"
   \pre "Ninguno"
   \post "Ninguno"
   \return "char*"
*/

char *textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}


/*!
   \brief "Escritura de Archivos- Retorna 1 si es correcta la escritura y 0 sino lo es"
   \param "char *fn, char *s"
   \pre "Ninguno"
   \post "Ninguno"
   \return "INT"
*/

int textFileWrite(char *fn, char *s) {

	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}
