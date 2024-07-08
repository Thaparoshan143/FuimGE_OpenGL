#ifndef OpenGL_Text_H
#define OpenGL_Text_H

#pragma once

#include"../../Core/Base/IText.h"
#include"./OpenGL_Window.h"
#include"./OpenGL_Shader.h"
#include"./OpenGL_Buffer.h"

#include "../../includes/freetypeinc/ft2build.h"
#include "../../includes/freetypeinc/freetype/freetype.h"

namespace OpenGL
{
    // This is currently specifically tied to the openGL, might have to move elsewhere later
    typedef struct FreetypeTextProp
    {
        Transformation::Transform m_textTransform;
        Text::TextProperties m_textProperties;
        Text::TextFont m_textFont;
    }FreetypeTextProp;

    // class TextRenderer;
    using FontMap = std::map<String, Interface::CharMap>;
    class FreetypeText : public Interface::IText
    {
        public:
        FreetypeText(String text, FreetypeTextProp textProp);
        ~FreetypeText() override {   }

        FreetypeTextProp GetFreetypeTextProp()  {   return FreetypeTextProp({m_textTransform, m_textProperties, m_textFont});    }

        friend class TextRenderer;

        private:
        void initFreetypeFont();
        void setActiveCharactersMap(Interface::CharMap *charMap = nullptr) override;
    };

}

#endif