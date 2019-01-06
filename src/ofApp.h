#pragma once

#include "ofMain.h"

class Tensegrity
{
public:
    Tensegrity()
    {
        n_particle = 12;
        m_particle = 0.2f;
        delta_t = 0.01f;
        r_particle = 5.0f;
        friction = 0.80f;

        /*/video1
        pos_init = ofVec3f(-400.0f, 200.0f, 250.0f); 
        vel_init = ofVec3f(-0.7f, 0.80f, 0.50f);
        rot_init = ofVec3f(-0.5f, 0.00f, 0.00f);
        k_spring = 20.0f;
        b_spring = 5.0f;
        gravity = ofVec3f(0.0f, 0.0f, -9.8f);
        //*/

        /*/video2
        pos_init = ofVec3f(-500.0f, 800.0f, 250.0f);  
        vel_init = ofVec3f(-0.3f, 1.00f, 0.40f); 
        rot_init = ofVec3f(-1.3f, 1.00f, 0.40f); 
        k_spring = 20.0f;
        b_spring = 5.0f;
        gravity = ofVec3f(0.0f, 0.0f, -9.8f);
        //*/

        /*/video3
        pos_init = ofVec3f(-500.0f, 800.0f, 250.0f);  
        vel_init = ofVec3f(-0.3f, 1.00f, 0.40f); 
        rot_init = ofVec3f(-1.3f, 1.00f, 0.40f); 
        gravity = ofVec3f(0.0f, 0.0f, -0.98f);
        k_spring = 20.0f;
        b_spring = 5.0f;
        //*/

        //*/video4
        pos_init = ofVec3f(-500.0f, 800.0f, 250.0f);
        vel_init = ofVec3f(-0.3f, 1.00f, 0.40f);
        rot_init = ofVec3f(-1.3f, 1.00f, 0.40f);
        gravity = ofVec3f(0.0f, 0.0f, -0.98f);
        k_spring = 1.1f;
        b_spring = 0.6f;
        //*/


        std::cout << "init pos" << std::endl;
        pos = {
            ofVec3f(1.0f, std::sqrt(3.0f), (-3.0f - std::sqrt(5.0f)) / 2.0f),                                                                //Ａ：(1, √3, (-3-√5)/2)
            ofVec3f(-2.0f, 0.0f, (-3.0f - std::sqrt(5.0f)) / 2.0f),                                                                          //Ｂ：(-2, 0, (-3-√5)/2)
            ofVec3f(1.0f, -std::sqrt(3.0f), (-3.0f - std::sqrt(5.0f)) / 2.0f),                                                               //Ｃ：(1, -√3, (-3-√5)/2)
            ofVec3f((-1.0f - std::sqrt(5.0f)) / 2.0f, (-1.0f - std::sqrt(5.0f)) * std::sqrt(3.0f) / 2.0f, (1.0f - std::sqrt(5.0f)) / 2.0f),  //Ｄ：(-(1+√5)/2, -(1+√5)√3/2, (1-√5)/2)
            ofVec3f(1.0f + std::sqrt(5.0f), 0.0f, (1.0f - std::sqrt(5.0f)) / 2.0f),                                                          //Ｅ：(1+√5, 0, (1-√5)/2)
            ofVec3f((-1.0f - std::sqrt(5.0f)) / 2.0f, (1.0f + std::sqrt(5.0f)) * std::sqrt(3.0f) / 2.0f, (1.0f - std::sqrt(5.0f)) / 2.0f),   //Ｆ：(-(1+√5)/2, (1+√5)√3/2, (1-√5)/2)
            ofVec3f((1.0f + std::sqrt(5.0f)) / 2.0f, (1.0f + std::sqrt(5.0f)) * std::sqrt(3.0f) / 2.0f, (-1.0f + std::sqrt(5.0f)) / 2.0f),   //Ｇ：((1+√5)/2, (1+√5)√3/2, (√5-1)/2)
            ofVec3f(-1.0f - std::sqrt(5.0f), 0.0f, (-1.0f + std::sqrt(5.0f)) / 2.0f),                                                        //Ｈ：(-1-√5, 0, (√5-1)/2)
            ofVec3f((1.0f + std::sqrt(5.0f)) / 2.0f, (-1.0f - std::sqrt(5.0f)) * std::sqrt(3.0f) / 2.0f, (-1.0f + std::sqrt(5.0f)) / 2.0f),  //Ｉ：((1+√5)/2, -(1+√5)√3/2, (√5-1)/2)
            ofVec3f(-1.0f, -std::sqrt(3.0f), (3.0f + std::sqrt(5.0f)) / 2.0f),                                                               //Ｊ：(-1, -√3, (3+√5)/2)
            ofVec3f(2.0f, 0.0f, (3.0f + std::sqrt(5.0f)) / 2.0f),                                                                            //Ｋ：(2, 0, (3+√5)/2)
            ofVec3f(-1.0f, std::sqrt(3.0f), (3.0f + std::sqrt(5.0f)) / 2.0f)                                                                 //Ｌ：(-1, √3, (3+√5)/2)
        };

        for (auto& pi : pos) {
            pi *= 40.0f;
            pi += pos_init;
            std::cout << pi << std::endl;
        }
        // std::cout << "init ppos" << std::endl;
        ppos = pos;
        for (auto& ppi : ppos) {
            ppi += vel_init;
            if (ppi.z > pos_init.z) {
                ppi += rot_init;
            }
        }

        std::cout << std::endl
                  << "init spring and stick" << std::endl;
        // + 2: stick     AD BL CK EF GJ HI
        // o 1: spring    each have 4 spring
        // x 0: nothing   AG BC DJ EI HF LK
        //
        // 0123456789
        // ABCDEFGHIJKL
        //A-oo+oox-----
        //B -xo-o-o---+
        //C  -oo---o-+-
        //D   ----oox--
        //E    -+o-x-o-
        //F     -ox---o
        //G      ---+oo
        //H       -+o-o
        //I        -oo-
        //J         -oo
        //K          -x
        //L           -

        spring = {
            {0, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 2},
            {0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 2, 0},
            {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

        d_spring = pos[0].distance(pos[1]);
        std::cout << "spring length " << d_spring << std::endl;
        d_stick = pos[0].distance(pos[3]);
        std::cout << "stick length " << d_stick << std::endl;

        std::cout << "init f" << std::endl;
        f = {
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f),
            ofVec3f(0.0f, 0.0f, 0.0f)};

        std::cout << std::endl
                  << "finish init" << std::endl;
    }

    void update_initf();
    void update_spring_f();
    void update_particle();
    void update_stick();
    void update_collision();

    void draw_ground();
    void draw_particle();
    void draw_stick();
    void draw_spring();

    ofVec3f velocity(int i);
    ofVec3f centor_of_tensegrity();

    //std::vector<ofVec3f> pos(12, ofVec3f(0, 0, 0));
    //std::vector<ofVec3f> ppos(12, ofVec3f(0, 0, 0));
    //std::vector<ofVec3f> f(12, ofVec3f(0, 0, 0));

    // class variable
    std::vector<ofVec3f> pos;
    std::vector<ofVec3f> ppos;
    std::vector<ofVec3f> f;

    // const value
    int n_particle;
    float r_particle;
    float m_particle;
    float d_spring;
    float k_spring;
    float b_spring;
    float d_stick;
    ofVec3f gravity;
    float delta_t;
    float friction;

    ofVec3f pos_init;
    ofVec3f vel_init;
    ofVec3f rot_init;

    // temp variable
    float length;
    ofVec3f pos_temp;
    ofVec3f direction_norm;
    ofVec3f diff;

    //std::vector<std::vector<int>> spring(12, std::vector<int>(12, 0));
    std::vector<std::vector<int>> spring;
};


class ofApp : public ofBaseApp
{
public:
    //base
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    //func
    void savepic();
    int count = 0;

    //var
    Tensegrity te{};

    ofImage sequence;
    ofEasyCam cam;
    ofLight light;
};
