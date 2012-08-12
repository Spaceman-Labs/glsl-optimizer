//
//  main.c
//  glsl-optimizer
//
//  Created by jkraut on 8/11/12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "glsl_optimizer.h"

char *readFile(FILE *input)
{
	long  lFileLen;               /* Length of file */
	char *cFile;                  /* Dynamically allocated buffer (entire file) */
	
	fseek(input, 0L, SEEK_END);  /* Position to end of file */
	lFileLen = ftell(input);     /* Get file length */
	rewind(input);               /* Back to start of file */
	
	cFile = (char*)calloc(lFileLen + 1, sizeof(char));
	
	if(cFile == NULL )
	{
		printf("\nInsufficient memory to read file.\n");
		exit(1);
	}
	
	fread(cFile, lFileLen, 1, input); /* Read the entire file into cFile */
	return cFile;
}

int main(int argc, const char * argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s *.[vsh|fsh]\n", argv[0]);
		return 0;
	}
	
	struct glslopt_ctx *ctx = glslopt_initialize(true);
	
	for (int i = 1; i < argc; i++)
	{
		enum glslopt_shader_type type;
		size_t filenameLen = strlen(argv[i]);
		if (!strncmp(".vsh", &argv[i][filenameLen - 4], 4))
		{
			type = kGlslOptShaderVertex;
		}
		else if (!strncmp(".fsh", &argv[i][filenameLen - 4], 4))
		{
			type = kGlslOptShaderFragment;
		}
		else
		{
			fprintf(stderr, "Sorry, I can't tell what kind of shader \"%s\" is. Try a file ending in \".vsh\" or \".fsh\".\n", argv[i]);
			return 1;
		}
			
		FILE *file = fopen(argv[i], "r");
		if (!file)
		{
			fprintf(stderr, "Sorry, I couldn't open \"%s\" (error %d).", argv[i], errno);
			return 1;
		}
		
		char *string = readFile(file);
		fclose(file);
		
		struct glslopt_shader *shader = glslopt_optimize(ctx, type, string, 0);
		free(string);
		
		if (glslopt_get_status(shader))
		{
			const char *newSource = glslopt_get_output(shader);
			printf("%s\n", newSource);
		}
		else
		{
			const char *error = glslopt_get_log(shader);
			fprintf(stderr, "%s\n", error);
		}
		glslopt_shader_delete(shader);
	}
	glslopt_cleanup(ctx);
	
	return 0;
}

