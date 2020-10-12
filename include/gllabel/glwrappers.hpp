#pragma once
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define log(format, ...) printf("GL: " format "\n", ##__VA_ARGS__)

class TypeFace
{
	FT_Face face = nullptr;
public:
	FT_Error error = 0;
	TypeFace(FT_Library library, const FT_Byte* file_base, FT_Long file_size, FT_Long face_index)
	{
		error = FT_New_Memory_Face(library, file_base, file_size, face_index, &face);
	}
	~TypeFace() { if (face != nullptr) { log("Deleted typeface '%s %s'", face->family_name, face->style_name); FT_Done_Face(face); } }
	TypeFace(TypeFace&& other) noexcept : face(other.face) { other.face = nullptr; }

	FT_Error setPixelSizes(FT_UInt pixel_width, FT_UInt pixel_height) { return error = FT_Set_Pixel_Sizes(face, pixel_width, pixel_height); }
	FT_Error loadChar(FT_ULong char_code, FT_Int32 load_flags) { return error = FT_Load_Char(face, char_code, load_flags); }
	FT_Error loadGlyph(FT_ULong glyph_index, FT_Int32 load_flags) { return error = FT_Load_Glyph(face, glyph_index, load_flags); }
	FT_Face operator->() { return face; }
};

template<GLenum target>
class Texture
{
	GLuint texture;
public:
	Texture() { glCreateTextures(target, 1, &texture); }
	~Texture() { if (texture != 0) { glDeleteTextures(1, &texture); log("Deleted texture %d", texture); } }
	Texture(Texture&& other) noexcept : texture(other.texture) { other.texture = 0; }

	void bind() { glActiveTexture(GL_TEXTURE0); return glBindTexture(target, texture); }
	void generateMipmap() { return glGenerateTextureMipmap(texture); }
	void storage2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
	{
		return glTextureStorage2D(texture, levels, internalformat, width, height);
	}
	void subImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width,
		GLsizei height, GLenum format, GLenum type, const void* pixels)
	{
		return glTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels);
	}
	void parameteri(GLenum pname, GLint param) { return glTextureParameteri(texture, pname, param); }
};

class VertexBuffer
{
	GLuint vertexBuffer;
public:
	VertexBuffer() { glCreateBuffers(1, &vertexBuffer); }
	~VertexBuffer() { if (vertexBuffer != 0) { glDeleteBuffers(1, &vertexBuffer); log("Deleted vertex buffer %d", vertexBuffer); } }
	VertexBuffer(VertexBuffer&& other) noexcept : vertexBuffer(other.vertexBuffer) { other.vertexBuffer = 0; }

	void bind(GLuint bindingindex, GLintptr offset, GLsizei stride) { return glBindVertexBuffer(bindingindex, vertexBuffer, offset, stride); }
	void data(GLsizeiptr size, const void* data, GLenum usage) { return glNamedBufferData(vertexBuffer, size, data, usage); }
};

class VertexArray
{
	GLuint vertexArray;
public:
	VertexArray() { glCreateVertexArrays(1, &vertexArray); }
	~VertexArray() { if (vertexArray != 0) { glDeleteVertexArrays(1, &vertexArray); log("Deleted vertex array %d", vertexArray); } }
	VertexArray(VertexArray&& other) noexcept : vertexArray(other.vertexArray) { other.vertexArray = 0; }

	void bind() { return glBindVertexArray(vertexArray); }
};

class Program;

class Shader
{
	GLuint shader;
	GLuint id() { return shader; }
	friend Program;
public:
	Shader(GLenum shaderType) : shader(glCreateShader(shaderType)) {}
	~Shader() { if (shader != 0) { glDeleteShader(shader); log("Deleted shader %d", shader); } }
	Shader(Shader&& other) noexcept : shader(other.shader) { other.shader = 0; }

	void compile() { return glCompileShader(shader); }
	void copySource(GLsizei count, const GLchar** string, const GLint* length) { return glShaderSource(shader, count, string, length); }
	void getiv(GLenum pname, GLint* params) { return glGetShaderiv(shader, pname, params); }
	void getInfoLog(GLsizei maxLength, GLsizei* length, GLchar* infoLog) { return glGetShaderInfoLog(shader, maxLength, length, infoLog); }
};

class Program
{
	GLuint program;
public:
	Program() : program(glCreateProgram()) {}
	~Program() { if (program != 0) { glDeleteProgram(program); log("Deleted program %d", program); } }
	Program(Program&& other) noexcept : program(other.program) { other.program = 0; }

	void attachShader(Shader& shader) { return glAttachShader(program, shader.id()); }
	void detachShader(Shader& shader) { return glDetachShader(program, shader.id()); }
	void link() { return glLinkProgram(program); }
	void use() { return glUseProgram(program); }
	void getiv(GLenum pname, GLint* params) { return glGetProgramiv(program, pname, params); }
	void getInfoLog(GLsizei maxLength, GLsizei* length, GLchar* infoLog) { return glGetProgramInfoLog(program, maxLength, length, infoLog); }
};
