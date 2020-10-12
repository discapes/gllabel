#include <gllabel/gllabel.hpp>
#include <string_view>
#include <optional>
#include <stdio.h>
using namespace std;
using namespace glm;

#define todo(format, ...) fprintf(stderr, "TODO: " format "\n", ##__VA_ARGS__)

GLLFace::GLLFace(TypeFace& face, const vector<Range<uint16_t>>& ranges) 
: face(face) {
	for (Range range : ranges)
		for (uint16_t i : range) {
			face.loadGlyph(i, FT_LOAD_NO_SCALE);
		}
}

void GLL::bind() {

}

void GLLabel::render(mat4) 
{
	static char h = [](){ todo("void GLLabel::render(mat4)"); return 'h'; }();
}

GLLabel::GLLabel(GLLFace&& face, string_view text)
	: face(std::move(face))
{
	if (text.length())
		appendText(text);
}
void GLLabel::setText(string_view)
{
	todo("void GLLabel::setText(string_view)");
}
void GLLabel::insertText(uint32_t i, string_view)
{
	todo("void GLLabel::insertText(uint32_t i, string_view)");
}
void GLLabel::appendText(string_view)
{
	todo("void GLLabel::appendText(string_view)");
}
void GLLabel::removeText(uint32_t i, uint32_t n)
{
	todo("void GLLabel::removeText(uint32_t i, uint32_t n)");
}
string_view GLLabel::getText() 
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
