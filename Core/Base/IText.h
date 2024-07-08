#pragma once

#include <map>
#include"./Types.h"
#include"./Transformation.hpp"

struct Character {
    uint TextureID; // ID handle of the glyph texture
    iVec2   Size;      // Size of glyph
    iVec2   Bearing;   // Offset from baseline to left/top of glyph
    uint Advance;   // Horizontal offset to advance to next glyph
};

namespace Text
{
    typedef enum TextAlignment
    {
        LEFT = 0,
        CENTER = 1,
        RIGHT = 2,
    }TextAlignment;

    typedef struct TextProperties
    {
        Color color;
        TextAlignment alignment;
    }TextProperties;

    typedef struct TextFont
    {
        String family;
        float size;
    }TextFont;
}

namespace Interface
{
    // This is applicable for Glyph based text rendering like freetype..
	// - Char as key and Character (struct containg text info) as value // Map from stl std::map<char, Character> 
	using CharMap = std::map<char, Character>;	
    using namespace Text;
	class IText
	{
		public:
		IText(String text, Transformation::Transform textTrans, TextProperties textProp, TextFont textFont) : m_text(text),m_textTransform(textTrans), m_textProperties(textProp), m_textFont(textFont) { m_enable = true; }
        virtual ~IText()    {   }

		void SetAligment(TextAlignment align)	{	m_textProperties.alignment = align;	}
        TextAlignment GetAlignment() const {    return m_textProperties.alignment; }
        void SetFontFamily(String fontFamily)   {   m_textFont.family = fontFamily;  }
        String GetFontFamily()  const { return m_textFont.family;    }
        void SetFontSize(uint fontSize) {   m_textFont.size = fontSize;  }
        uint GetFontSize() const {   return m_textFont.size;  }
        fVec3 GetTextPosition() const { return m_textTransform.position;    }
        CharMap* GetActiveCharMap() {   return m_activeCharacters;  }
        void SetEnable(bool status) {   m_enable = status;  }
        void SetTextProperties(TextProperties textProp)  {   m_textProperties = textProp; }
        const TextProperties& GetTextProperties()   {   return m_textProperties;  }
        const String& GetText() const {   return m_text;  }
        void SetText(String text) 
        {   
            m_text = text;
            cacheOffset();
        }
		fVec2 GetAlignmentOffset()
		{
            return (m_textProperties.alignment == Text::CENTER) ? fVec2(m_cacheStringOffset.x/2.0, m_cacheStringOffset.y) :
                    (m_textProperties.alignment == Text::LEFT) ? fVec2(0, m_cacheStringOffset.y) :
                    fVec2(m_cacheStringOffset.x, m_cacheStringOffset.y);
		}

        protected:
        virtual void setActiveCharactersMap(CharMap *charMap = nullptr) = 0;
        // Horizontal
		int getXGlyphSize()
		{
			int size = 0;
			char letter;
			for(uint i=0;i<(uint)m_text.size();++i)
			{
				letter = m_text[i];
				size += (((*m_activeCharacters)[letter].Size.x + (*m_activeCharacters)[letter].Advance)>>6);
			}

			return size;
		}

        // Vertical
		int getYGlyphSize()
		{
			// Here we are asuming a random character offset of y and taking it for granted..
			return (*m_activeCharacters)['A'].Bearing.y/2.0;
		}

        void cacheOffset()
        {
            m_cacheStringOffset.x = getXGlyphSize(); 
            m_cacheStringOffset.y = getYGlyphSize();
        }

		String m_text;
		CharMap *m_activeCharacters;
        fVec2 m_cacheStringOffset;
        bool m_enable;
        Transformation::Transform m_textTransform;
        TextProperties m_textProperties;
        TextFont m_textFont;
	};
}