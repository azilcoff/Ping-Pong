#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cmath>
#include <vector>
#include <array>
#include "Wall.hpp"

class Ball{
    private:
        GLbyte wallColliderNum = -1;
        inline static const GLfloat RADIUS = 20.0f;
        inline static const float SPEED = 2.3f;
        std::vector<GLfloat> &vertices;
        std::array<Wall, 2> &walls;
    public:
        float momentumX = SPEED;
        float momentumY = 0;
        inline static const GLubyte TRIANGLE_COUNT = 30u;
        inline static const GLsizeiptr SIZE = TRIANGLE_COUNT * 2 * sizeof(GLfloat);
        GLfloat centerX, centerY;
        bool collision()
        {
            GLfloat backSide = centerX - RADIUS;
            GLfloat frontSide = centerX + RADIUS;
            GLfloat downSide = centerY - RADIUS;
            GLfloat upSide = centerY + RADIUS;
            if (frontSide > walls[0].x && backSide < walls[0].x + Wall::WIDTH && upSide > walls[0].y && downSide < walls[0].y + Wall::HEIGHT){
                this->wallColliderNum = 0;
                return true;
            }
            else if(backSide <= walls[1].x + Wall::WIDTH && frontSide >= walls[1].x && upSide >= walls[1].y && downSide <= walls[1].y + Wall::HEIGHT){
                this->wallColliderNum = 1;
                return true;
            }
            return false;
        }
        void appendPosition()
        {
            if (this->collision()){
                this->momentumX *= -1;
                this->momentumY *= -1;
            }
            
            this->centerX += momentumX;
            if (wallColliderNum != -1){
                this->momentumY = 2.0f * walls[wallColliderNum].momentum - this->momentumY ;
                wallColliderNum = -1;
            }
            this->centerY += momentumY * 0.1f;

            vertices.clear();
            for (int i = 0; i < TRIANGLE_COUNT; i++){
                GLfloat angle = glm::radians(360.0f) * static_cast<float>(i) / static_cast<float>(TRIANGLE_COUNT);

                GLfloat currentX = this->centerX + RADIUS * std::cos(angle);
                GLfloat currentY = this->centerY + RADIUS * std::sin(angle);
                vertices.push_back(currentX);
                vertices.push_back(currentY);
            }
            glBufferSubData(GL_ARRAY_BUFFER, 0, SIZE, vertices.data());
        }

        Ball(GLfloat centerX, GLfloat centerY, std::vector<GLfloat>& vertices, std::array<Wall, 2> &walls): centerX(centerX), centerY(centerY), vertices(vertices), walls(walls)
        {

        }
};