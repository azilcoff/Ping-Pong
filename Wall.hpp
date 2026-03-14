#pragma once

#include <glad/glad.h>
#include <vector>
#include "Config.h"

class Wall{
    private:
        inline static const GLubyte SPEED = 2;
        std::vector<GLfloat> &vertices;
        std::vector<GLushort> &indices;
    public:
        bool up = false, down = false;
        float momentum = SPEED;
        inline static const GLfloat WIDTH = 40.0f;
        inline static const GLfloat HEIGHT = 170.0f;
        const GLfloat x;
        GLfloat y;
        void appendPosition(bool clear)
        {   
            if (up){
                this->momentum = -SPEED;
            }
            else if(down){
                this->momentum = SPEED;
            }
            else{
                this->momentum = 0;
            }
            if (this->y + momentum >= 0 && this->y + momentum + HEIGHT <= WINDOW_HEIGHT)
                this->y += momentum;
            if (clear)
                vertices.clear();
            GLushort i1, i2, i3, i4;
            vertices.push_back(this->x);
            vertices.push_back(this->y);
            i1 = vertices.size() / 2 - 1;
            vertices.push_back(this->x + WIDTH);
            vertices.push_back(this->y);
            i2 = i1 + 1;
            vertices.push_back(this->x + WIDTH);
            vertices.push_back(this->y + HEIGHT);
            i3 = i2 + 1;
            vertices.push_back(this->x);
            vertices.push_back(this->y + HEIGHT);
            i4 = i3 + 1;

            indices.push_back(i4);
            indices.push_back(i2);
            indices.push_back(i1);
            indices.push_back(i4);
            indices.push_back(i3);
            indices.push_back(i2);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLushort), indices.data());
        }
        Wall(const GLfloat x, GLfloat y, std::vector<GLfloat> &vertices, std::vector<GLushort> &indices): x(x), y(y), vertices(vertices), indices(indices)
        {

        }
};