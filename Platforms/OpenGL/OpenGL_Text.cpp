#include"./OpenGL_Text.h"

namespace OpenGL
{
    FreetypeText::FreetypeText(String text, FreetypeTextProp textProp) : IText(text, textProp.m_textTransform, textProp.m_textProperties, textProp.m_textFont)
    {
        initFreetypeFont();
    }

    void FreetypeText::initFreetypeFont()
    {
        // subsitute to add to font renderer map and query before making one and later queue system
        // if doesnt exist's in the fontlist map then load the character mapping info.. and add to map

	}

    void FreetypeText::setActiveCharactersMap(Interface::CharMap *charMap)
    {
        m_activeCharacters = charMap;
    }
}