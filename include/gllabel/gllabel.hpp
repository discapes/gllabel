#include <string_view>
#include <stdint.h>
#include <utility>
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "glwrappers.hpp"

template <typename T>
class Range
{
	T _begin;
	T _end;
public:
	class iterator
	{
	public:
		iterator(T index) : index(index) {}
		iterator operator++()
		{
			++index;
			return *this;
		}
		T operator*() const { return index; }
		bool operator!=(const iterator& other) { return index != other.index; }
	private:
		T index;
	};
	iterator begin() const { return iterator(_begin); }
	iterator end() const { return iterator(_end); }
};

class GLLFace {
public:
	GLLFace(TypeFace& face, const std::vector<Range<uint16_t>>& ranges);

private:
	Texture<GL_TEXTURE_1D> bezierAtlas;
	Texture<GL_TEXTURE_1D> gridAtlas;
	int beziersPerCell = 4;
	TypeFace& face;
};

namespace GLL {
	void bind();
}

class GLLabel {
public:
	void render(glm::mat4);
	GLLabel(GLLFace&& face, std::string_view text);

	void setText(std::string_view);
	void insertText(uint32_t i, std::string_view);
	void appendText(std::string_view);
	void removeText(uint32_t i, uint32_t n);
	std::string_view getText();

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
	GLLFace face;
	struct Glyph {
		uint16_t width, height;
	};
	std::string text;
	std::vector<Glyph> glyphs;
	Range range = Range::Left;
	bool showCaret = false;
	uint32_t caretPos = 0;
	VertexBuffer VBO;
};
