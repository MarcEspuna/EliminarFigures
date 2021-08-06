#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct ControlLines
{   //WE WILL DRAW IN THE RENDERER USING GL_QUADS OR TRIANGLES

    ControlLines(size_t difficulty)
        :m_Dificulty(difficulty)
    {

    }

    ControlLines()
        :m_Dificulty(1)
    {

    }

    float horsontalLine[8] =
    {
        //  Horsontal Line:
        -640.0f,  20.0f,    //0
         640.0f,  20.0f,    //1
         640.0f, -20.0f,    //2
        -640.0f, -20.0f,    //3
    };



    float verticalLine[8] =
    {
        //  Vertical Line:
         -20.0f,  360.0f,   //0
          20.0f,  360.0f,   //1
          20.0f, -360.0f,   //2
         -20.0f, -360.0f,   //3
    };

    float centralQuad[8] =
    {
        //  Central Quad:
          20.0f,  20.0f,    //0
          20.0f, -20.0f,    //1
         -20.0f, -20.0f,    //2
         -20.0f,  20.0f     //3
    };

    unsigned int indexHorzontalLine[6] =
    {
        //  Horsontal Line:
             0,1,3,
             3,2,1,
    };

    unsigned int indexVerticalLine[6] =
    {
        //  Vertical Line:
             0,1,3,
             3,2,1,
    };

    unsigned int indexCentralQuad[6] =
    {
        //  Central Quad:
             0,1,2,
             0,2,3
    };

    glm::mat4 modelH = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 modelV = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 modelC = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));



    inline float* GetPositionsH() { return horsontalLine; }
    inline float* GetPositionsV() { return verticalLine; }
    inline float* GetPositionsC() { return centralQuad; }

    inline unsigned int* GetIndexH() { return indexHorzontalLine; }
    inline unsigned int* GetIndexV() { return indexVerticalLine; }
    inline unsigned int* GetIndexC() { return indexCentralQuad; }

    inline size_t GetCountPositions() { return 8; }
    inline size_t GetCountIndexes() { return 6; }

private:
    size_t m_Dificulty;


};


struct Figures
{

    float Square[8] =
    {
        -300.0f, -300.0f,       //0
        -300.0f, -200.0f,       //1
        -200.0f, -200.0f,       //2
        -200.0f, -300.0f        //3
    };

    float Triangle[3] =
    {

    };

    float Star[16] =
    {
        174.0f, 200.0f,             //0
        72.0f,  228.0f,             //1
        174.0f, 252.0f,             //2
        226.0f, 200.0f,             //3
        203.0f, 111.0f,             //4
        230.0f, 252.0f,             //5
        318.0f, 224.0f,             //6
        200.0f, 350.0f              //7

    };

    float V[6] =
    {

    };

    float TreeWay[16] =
    {

    };

    unsigned int indexQuad[6] =
    {
        0,1,2,
        2,3,0
    };

    unsigned int indexStar[18] =
    {
        0,1,2,
        0,2,3,
        0,3,4,
        3,5,6,
        2,5,7,
        2,3,5
    };

};