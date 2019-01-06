#include "ofApp.h"
#include <list>
#include <random>

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(60);
    ofBackground(255);

    ofEnableDepthTest();

    ofEnableLighting();
    light.enable();
}

//--------------------------------------------------------------
void ofApp::update()
{
    te.update_initf();
    te.update_spring_f();
    te.update_particle();
    te.update_stick();
    te.update_collision();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cam.begin();
    ofRotateX(-60);
    ofRotateZ(20);
    ofTranslate(100, 100, -200);
    light.setPosition(cam.getPosition());

    //std::cout << "draw_ground" << std::endl;
    te.draw_ground();
    //std::cout << "draw_particle" << std::endl;
    //te.draw_particle();
    //std::cout << "draw_stick" << std::endl;
    te.draw_stick();
    //std::cout << "draw_spring" << std::endl;
    te.draw_spring();
    //std::cout << "centor is " << te.centor_of_tensegrity() << std::endl;
    std::cout << te.centor_of_tensegrity().z << std::endl;

    cam.end();

    savepic();  // to save movie
}

void ofApp::keyPressed(int key) {}
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}

void ofApp::savepic()
{
    sequence.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    std::ostringstream sout;
    sout << std::setfill('0') << std::setw(5) << count;
    std::string s = sout.str();
    string fileName = s + ".png";
    sequence.save(fileName);
    count++;
}

//--------------------------------------------------------------

void Tensegrity::update_initf()
{
    for (int i = 0; i < n_particle; i++) {
        f[i] = gravity * m_particle;
    }
}

void Tensegrity::update_spring_f()
{
    for (int i = 0; i < n_particle; i++) {
        for (int j = 0; j < n_particle; j++) {
            if (spring[i][j] == 1) {  //spring
                //spring
                length = pos[i].distance(pos[j]);
                direction_norm = pos[j] - pos[i];
                direction_norm = direction_norm.normalize();
                f[i] = f[i] + k_spring * (length - d_spring) * direction_norm;
                f[j] = f[j] - k_spring * (length - d_spring) * direction_norm;
                //damper
                f[i] = f[i]
                       + b_spring
                             * (direction_norm.dot(velocity(j)) - direction_norm.dot(velocity(i)))
                             * direction_norm;
                f[j] = f[j]
                       - b_spring
                             * (direction_norm.dot(velocity(j)) - direction_norm.dot(velocity(i)))
                             * direction_norm;
            }
        }
    }
}

void Tensegrity::update_particle()
{
    for (int i = 0; i < n_particle; i++) {
        pos_temp = pos[i];
        pos[i] = pos[i] + 0.999f * (pos[i] - ppos[i]) + f[i] * delta_t * delta_t / m_particle;
        ppos[i] = pos_temp;
    }
}

//fixed length of stick
void Tensegrity::update_stick()
{
    for (int i = 0; i < n_particle; i++) {
        for (int j = 0; j < n_particle; j++) {
            if (spring[i][j] == 2) {  //stick
                direction_norm = pos[j] - pos[i];
                direction_norm = direction_norm.normalize();
                pos_temp = (pos[i] + pos[j]) / 2.0f;  //centor of stick;
                pos[i] = pos_temp - d_stick / 2.0f * direction_norm;
                pos[j] = pos_temp + d_stick / 2.0f * direction_norm;
            }
        }
    }
}

void Tensegrity::update_collision()
{
    for (int i = 0; i < n_particle; i++) {
        if (pos[i].z < r_particle) {
            ppos[i].z = 2.0f * pos[i].z - ppos[i].z;
            float diff = -pos[i].z + r_particle;
            pos[i].z += diff;
            ppos[i].z += diff;

            //friction
            ppos[i].x = ppos[i].x + (pos[i].x - ppos[i].x) * (1.0f - friction);
            ppos[i].y = ppos[i].y + (pos[i].y - ppos[i].y) * (1.0f - friction);
        }
    }
}

//--------------------------------------------------------------

void Tensegrity::draw_ground()
{
    ofSetColor(192, 192, 192);  //silver
    ofDrawBox(ofVec3f(0.0f, 0.0f, -2.0f), 10000.0f, 10000.0f, 1.0f);
}

void Tensegrity::draw_particle()
{
    for (int i = 0; i < n_particle; i++) {
        ofSetColor(249, 37, 0);  //red
        ofDrawSphere(pos[i], r_particle);
    }
}

void Tensegrity::draw_stick()
{
    for (int i = 0; i < n_particle; i++) {
        for (int j = 0; j < n_particle; j++) {
            if (spring[i][j] == 2) {       //stick
                ofSetColor(177, 104, 51);  //brawn
                ofVec3f drawpos = pos[i];
                diff = (pos[j] - pos[i]) / 30.0f;
                for (int k = 0; k < 31; k++) {
                    ofDrawSphere(drawpos, r_particle);
                    drawpos += diff;
                }
            }
        }
    }
}

void Tensegrity::draw_spring()
{
    for (int i = 0; i < n_particle; i++) {
        for (int j = 0; j < n_particle; j++) {
            if (spring[i][j] == 1) {        //spring
                ofSetColor(157, 204, 224);  //light blue
                ofVec3f drawpos = pos[i];
                diff = (pos[j] - pos[i]) / 20.0f;
                for (int k = 0; k < 19; k++) {
                    drawpos += diff;
                    ofDrawSphere(drawpos, r_particle);
                }
            }
        }
    }
}

//--------------------------------------------------------------
ofVec3f Tensegrity::velocity(int i)
{
    return pos[i] - ppos[i];
}

//--------------------------------------------------------------
ofVec3f Tensegrity::centor_of_tensegrity()
{
    pos_temp = ofVec3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < n_particle; i++) {
        pos_temp += pos[i];
    }
    pos_temp /= n_particle;
    return pos_temp;
}
