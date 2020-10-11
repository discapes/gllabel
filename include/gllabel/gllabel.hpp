#include <string>
#include <stdint.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "glwrappers.hpp"

class GLLabel {
public:
	void render(glm::mat4);
	GLLabel(const std::string& text); 	

	void setText(const std::string&);
	void insertText(uint32_t i, const std::string&);
	void appendText(const std::string&);
	void removeText(uint32_t i, uint32_t n);
	const std::string& getText();

	enum class Range {
		Left,
		Right,
		Centered,
		Justified,
	};
	void setRange(Range);
	Range getRange();

	void setShowCaret(bool);
	bool getShowCaret();
	void setCaretPos(uint32_t);
	uint32_t getCaretPos();
private:
	struct Glyph {
		uint16_t width, height;
	};
	std::string text;
	std::vector<Glyph> glyphs;
	Range range = Range::Left;
	bool showCaret = false;
	uint32_t caretPos = 0;
	VertexArray VAO;
	VertexBuffer VBO;
};
