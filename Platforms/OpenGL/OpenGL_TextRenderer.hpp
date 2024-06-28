#pragma once

#include"../../Core/Base/Types.h"
#include"../../Core/Base/IText.h"
#include"../../Core/Base/IWindow.h"
#include"../../Core/Base/TSingleton.hpp"
#include"../../Core/Base/Log.hpp"

#include"./OpenGL_Shader.h"
#include"./OpenGL_Buffer.h"
#include"./OpenGL_Window.h"
#include"./OpenGL_Text.h"

#include <map>

#define DEFAULT_TEXT_SHADER_PATH "../res/Shaders/Text/"
#define DEFAULT_FONT_PATH "../res/Fonts/"
#define BASE_FONT_SIZE 1/3.0f

namespace OpenGL
{
    using FontMap = std::map<String, Interface::CharMap>;

    class TextRenderer : public Singleton::TISingleton<TextRenderer>
    {
        public:
        TextRenderer() : TISingleton(this), m_textShader(DEFAULT_TEXT_SHADER_PATH, "ver.shader", "frag.shader")  {  initRenderer(); }

        void RenderText()
        {
            for(uint i=0;i<(uint)m_renderQueue.size();++i)
            {
                renderText(m_renderQueue[i], false);
            }
        }

        void AddToQueue(String text, FreetypeTextProp textProp)
        {
            FreetypeText *temp = new FreetypeText(text, textProp);
            m_renderQueue.push_back(temp);
            temp->setActiveCharactersMap(&GetCharMapFromList(temp->GetFontFamily()));
            temp->cacheOffset();
        }

        static void SetRenderTarget(Interface::IWindow* target)  {   m_target = target;  }
        Interface::CharMap& GetCharMapFromList(String fontFamily)
        {
            if(m_fontFamilyList.count(fontFamily) == 0)
            {
                initFreetype(fontFamily);
            }

            return m_fontFamilyList[fontFamily];
        }

        void AddFontList(String fontFamily)
        {
            initFreetype(fontFamily);
        }

        void SetQueueText(String newText, uint index)
        {
            if(m_renderQueue.size() <= index)
            {
                std::cout << "Out of index access to queue string!!" << std::endl;
                return;
            }
            else
            {
                m_renderQueue[index]->SetText(newText);
            }
        }

        private:
        void initRenderer()
        {
            m_textShader.CompileProgram();

            // Freetype is also initialize here for now as it holds map for required font family
            initFreetype();
        }

        void initFreetype(String fontFamily = "Roboto")
        {
            if(m_fontFamilyList.count(fontFamily) == 0)
            {
                FT_Library ft;
                if (FT_Init_FreeType(&ft))
                {
                    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
                    exit(1);
                }

                FT_Face face;
                String fontFamilyPath = DEFAULT_FONT_PATH + fontFamily + ".ttf";

                if (FT_New_Face(ft, fontFamilyPath.data(), 0, &face)) 
                {
                    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
                    exit(1);
                }
                else 
                {
                    FT_Set_Pixel_Sizes(face, 0, 48);

                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    std::map<char, Character> charMap;
                    for (unsigned char c = 0; c < 128; c++)
                    {
                        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                        {
                            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                            continue;
                        }

                        unsigned int texture;
                        glGenTextures(1, &texture);
                        glBindTexture(GL_TEXTURE_2D, texture);
                        glTexImage2D(
                            GL_TEXTURE_2D,
                            0,
                            GL_RED,
                            face->glyph->bitmap.width,
                            face->glyph->bitmap.rows,
                            0,
                            GL_RED,
                            GL_UNSIGNED_BYTE,
                            face->glyph->bitmap.buffer
                        );

                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                        Character character = {
                            texture,
                            iVec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                            iVec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                            static_cast<uint>(face->glyph->advance.x)
                        };
                        charMap.insert(std::pair<char, Character>(c, character));
                    }
                    m_fontFamilyList.insert({fontFamily, charMap});
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
                FT_Done_Face(face);
                FT_Done_FreeType(ft);
            }
            else
            {
                std::cout << "Font familt exist on the list!!" << std::endl;
            }

            m_VAO.Bind();
            m_VBO.Bind();
            // For text rendering we require only position (x,y) and texture (u,v).. for 6*4 (i.e using two triangle method to draw quad to draw text on top)
            m_VBO.ReserveBuffer(6*4, GL_DYNAMIC_DRAW);
            m_VAO.AppendBufferLink(&m_VBO, nullptr);
            // Here 4 is the total vertex buffer size i.e two position and two texture data 
            m_VAO.EnableVertexAttrMan(4);
            // unbinding is not manditory, optional
            m_VAO.Unbind();
            m_VBO.Unbind();
        }

        void renderText(FreetypeText *textTarget, bool normalize = true)
        {
            FreetypeTextProp texProp = textTarget->GetFreetypeTextProp();
            texProp.m_textFont.size *= BASE_FONT_SIZE;

            if(m_target == nullptr)
            {
                std::cout << "Window target not updated to render" << std::endl;
                return; 
            }
            
            fVec2 winDim = m_target->GetWindowDim();

            m_textShader.UseProgram();
            m_textShader.SetUniformVec4("textColor", texProp.m_textProperties.color);

            m_VAO.Bind();
            glm::mat4 proj = glm::ortho<float>(0, winDim.x, 0, winDim.y);
            m_textShader.SetUniformMat4("projection", proj);
            glActiveTexture(GL_TEXTURE0);

            float x = texProp.m_textTransform.position.x, y = texProp.m_textTransform.position.y;
            if(!normalize)
            {
                x = x * (winDim.x/2.0) + winDim.x/2.0;
                y = -1 * y * (winDim.y/2.0) + winDim.y/2.0;
            }
            // the position based are different in projection and OpenGL working so adjusting for y
            y = winDim.y - y;
            // For now adjusting the x and y alignment 
            fVec2 alignmentOffset = textTarget->GetAlignmentOffset();
            x = x - (alignmentOffset.x * texProp.m_textFont.size);
            y = y - (alignmentOffset.y * texProp.m_textFont.size);

            // iterate through all characters
            const String &textRef = textTarget->GetText();
            Interface::CharMap &activeCharMap = GetCharMapFromList(textTarget->GetFontFamily());

            // std::cout << x << " , " << y << std::endl;
            // std::cout << textRef << std::endl;
            std::string::const_iterator c;
            for (c = textRef.begin(); c != textRef.end(); c++) 
            {
                Character ch = activeCharMap[*c];
                float xpos = x + ch.Bearing.x * texProp.m_textFont.size;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * texProp.m_textFont.size;

                float w = ch.Size.x * texProp.m_textFont.size;
                float h = ch.Size.y * texProp.m_textFont.size;

                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },            
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                // update content of m_VBO memory
                m_VBO.LoadSubBuffer(6*4, *vertices);
                m_VBO.Unbind();
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += ((ch.Advance >> 6) * texProp.m_textFont.size); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }
            m_VAO.Unbind();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // Later query from Shader Manager and have just reference 
        OpenGL_Shader m_textShader;
        VertexArrayObject m_VAO;
        VertexBufferObject m_VBO;
        static FontMap m_fontFamilyList;
        std::vector<FreetypeText*> m_renderQueue;
        static Interface::IWindow *m_target;
    };

    FontMap TextRenderer::m_fontFamilyList;
    Interface::IWindow* TextRenderer::m_target;
}   