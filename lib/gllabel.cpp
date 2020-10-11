#include <gllabel/gllabel.hpp>
#include <stdio.h>
using namespace std;
using namespace glm;

#define todo(format, ...) fprintf(stderr, "TODO: " format "\n", ##__VA_ARGS__)

void GLLabel::render(mat4) 
{
	static nullptr_t x = [](){ todo("void GLLabel::render(mat4)"); return nullptr; }();
}
GLLabel::GLLabel(const string& text)
{
	if (text.length())
		appendText(text);
}

void GLLabel::setText(const string&)
{
	todo("void GLLabel::setText(const string&)");
}
void GLLabel::insertText(uint32_t i, const string&)
{
	todo("void GLLabel::insertText(uint32_t i, const string&)");
}
void GLLabel::appendText(const string&)
{
	todo("void GLLabel::appendText(const string&)");
}
void GLLabel::removeText(uint32_t i, uint32_t n)
{
	todo("void GLLabel::removeText(uint32_t i, uint32_t n)");
}
const string& GLLabel::getText() 
{
	return this->text;
}

void GLLabel::setRange(Range) 
{
	todo("void GLLabel::setRange(Range) ");
}
GLLabel::Range GLLabel::getRange()
{
	return this->range;
}

void GLLabel::setShowCaret(bool)
{
	todo("void GLLabel::setShowCaret(bool)");
}
bool GLLabel::getShowCaret() 
{
	return this->showCaret;
}
void GLLabel::setCaretPos(uint32_t)
{
	todo("void GLLabel::setCaretPos(uint32_t)");
}
uint32_t GLLabel::getCaretPos()
{
	return caretPos;
}
