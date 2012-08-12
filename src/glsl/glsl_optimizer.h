#pragma once
#ifndef GLSL_OPTIMIZER_H
#define GLSL_OPTIMIZER_H

#import <stdbool.h>

/*
 Main GLSL optimizer interface.
 See ../../README.md for more instructions.

 General usage:

 ctx = glslopt_initialize();
 for (lots of shaders) {
   shader = glslopt_optimize (ctx, shaderType, shaderSource, options);
   if (glslopt_get_status (shader)) {
     newSource = glslopt_get_output (shader);
   } else {
     errorLog = glslopt_get_log (shader);
   }
   glslopt_shader_delete (shader);
 }
 glslopt_cleanup (ctx);
*/

struct glslopt_shader;
struct glslopt_ctx;

enum glslopt_shader_type {
	kGlslOptShaderVertex = 0,
	kGlslOptShaderFragment,
};

// Options flags for glsl_optimize
enum glslopt_options {
	kGlslOptionSkipPreprocessor = (1<<0), // Skip preprocessing shader source. Saves some time if you know you don't need it.
	kGlslOptionNotFullShader = (1<<1), // Passed shader is not the full shader source. This makes some optimizations weaker.
};

struct glslopt_ctx* glslopt_initialize (bool openglES);
void glslopt_cleanup (struct glslopt_ctx* ctx);

struct glslopt_shader* glslopt_optimize (struct glslopt_ctx* ctx, enum glslopt_shader_type type, const char* shaderSource, unsigned options);
bool glslopt_get_status (struct glslopt_shader* shader);
const char* glslopt_get_output (struct glslopt_shader* shader);
const char* glslopt_get_raw_output (struct glslopt_shader* shader);
const char* glslopt_get_log (struct glslopt_shader* shader);
void glslopt_shader_delete (struct glslopt_shader* shader);


#endif /* GLSL_OPTIMIZER_H */
