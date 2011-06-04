#include "CGrapher.h"
#include "Gfx/GfxBase.h"
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include "Resources/CTexture.h"

#define Error_(x) cerr << x << endl; cout << x << endl;

const real32 c_springConstant = 1.0f;
const real32 c_coulombConstant = 400.0f;
const real32 c_damping = 0.9f;
const real32 c_kineticThreshold = 5.0f;

//-------------------------------------------------------------------------------------------------------
CGrapher::CGrapher () :
    m_focusMode(false),
    m_kineticEnergy(0.0f),
    m_updateTime(0.0f),
    m_paused(false) {
    m_camera.SetPosition(C3Vector(15.0f, 15.0f, 15.0f));

    m_font = new CFont(CTexture::LoadTexture("font", "boli.bmp"), CTexture::LoadTexture("font_mask", "boli_mask.bmp"));
    m_font->SetColor(0.0f, 0.0f, 0.0f);

    SNode::grapher = this;
}

//-------------------------------------------------------------------------------------------------------
void CGrapher::LoadFile (const char* filename) {
    cout << "Loading graph from " << filename << "..." << endl;

    ifstream stream(filename);

    if (stream.fail()) {
        Error_("Could not open file.");
        return;
    }

    map<string, uint> fbIdsToNodeIndices;

    uint nodeIndex = 0;
    char line[256];

    stream.getline(line, 256);

    uint numFriends = atoi(line);

    for (uint i = 0; i < numFriends; i++) {
        stream.getline(line, 256);

        if (stream.fail())
            break;

        SNode* node = new SNode;
        node->fbId = line;

        stream.getline(line, 256);
        node->personName = line;

        stream.getline(line, 256);
        if (strcmp(line, "male") == 0)
            node->gender = e_genderMale;
        else if (strcmp(line, "female") == 0)
            node->gender = e_genderFemale;
        else
            node->gender = e_genderUnknown;

        node->position.Set(
            CMath::RandNumber(-35.0f, 35.0f),
            CMath::RandNumber(-35.0f, 35.0f),
            CMath::RandNumber(-35.0f, 35.0f)
        );
        node->velocity.Set(0.0f, 0.0f, 0.0f);

        m_nodes.push_back(node);

        fbIdsToNodeIndices[node->fbId] = nodeIndex;
        nodeIndex++;
    }

    while (true) {
        stream.getline(line, 256);

        if (stream.fail())
            break;

        stringstream sstream(line);
        string fbIdOne, fbIdTwo;
        sstream >> fbIdOne;
        sstream >> fbIdTwo;

        map<string, uint>::iterator iter = fbIdsToNodeIndices.find(fbIdOne);
        if (iter == fbIdsToNodeIndices.end()) {
            Error_("Found no matching id (" << fbIdOne << ")");
            continue;
        }

        uint nodeIndexOne = iter->second;

        iter = fbIdsToNodeIndices.find(fbIdTwo);
        if (iter == fbIdsToNodeIndices.end()) {
            Error_("Found no matching id (" << fbIdTwo << ")");
            continue;
        }

        uint nodeIndexTwo = iter->second;

        SNode* nodeOne = m_nodes[nodeIndexOne];
        SNode* nodeTwo = m_nodes[nodeIndexTwo];
        nodeOne->neighbors.push_back(nodeTwo);
        nodeTwo->neighbors.push_back(nodeOne);

        SEdge* edge = new SEdge;
        edge->one = nodeOne;
        edge->two = nodeTwo;
        edge->start = nodeOne; // Chosen arbitrarily for now
        m_edges.push_back(edge);
        nodeOne->edges.push_back(edge);
        nodeTwo->edges.push_back(edge);
    }

    cout << "Done loading!" << endl;

    // Experimental, for each node, remove all neighbors who don't have neighbors who know the node
    // This makes cliques easier to see, maybe?
#if 0
    int numRemoved = 0;
    for (uint i = 0; i < m_nodes.size(); i++) {
        SNode* node = m_nodes[i];
        for (uint j = 0; j < node->neighbors.size(); j++) {
            SNode* neighbor = node->neighbors[j];

            bool neighborKnowsNeighborKnowsNode = false;
            for (uint k = 0; k < neighbor->neighbors.size(); k++) {
                SNode* neighborNeighbor = neighbor->neighbors[k];
                for (uint l = 0; l < neighborNeighbor->neighbors.size(); l++) {
                    SNode* neighborNeighborNeighbor = neighborNeighbor->neighbors[l];

                    if (neighborNeighborNeighbor == node) {
                        neighborKnowsNeighborKnowsNode = true;
                        break;
                    }
                }
                if (neighborKnowsNeighborKnowsNode)
                    break;
            }

            // Remove edge then
            if (!neighborKnowsNeighborKnowsNode) {
                node->neighbors.erase(node->neighbors.begin() + j);

                for (int k = 0; k < neighbor->neighbors.size(); k++)
                    if (neighbor->neighbors[k] == node)
                        neighbor->neighbors.erase(neighbor->neighbors.begin() + k);

                numRemoved++;
            }
        }
    }

    cout << "Removed " << numRemoved << endl;
#endif
}

//-------------------------------------------------------------------------------------------------------
void CGrapher::DoPhysicsUpdate (uint32 elapsedTicks) {
    if (elapsedTicks > 100)
        elapsedTicks = 100;

    real32 dt = (real32)elapsedTicks * 0.001f * 0.5f; // Cut timestep in half to avoid oscillation

    real32 kineticEnergy = 0;
    for (uint i = 0; i < m_nodes.size(); i++) {
        SNode* node = m_nodes[i];
        C3Vector force;

        for (uint j = 0; j < m_nodes.size(); j++) {
            SNode* neighbor = m_nodes[j];

            if (neighbor == node)
                continue;

            C3Vector dir = neighbor->position - node->position;
            real64 distSq = dir.squareMagnitude();
            if (distSq < 0.0001f)
                distSq = 0.01f;
            if (distSq > 100.0f)
                continue;

            real64 coulombMagnitude = c_coulombConstant / distSq;
            if (coulombMagnitude > 100.0f)
                coulombMagnitude = 100.0f;
            dir.Normalize();
            force += dir * -coulombMagnitude;
        }

        for (uint j = 0; j < node->neighbors.size(); j++) {
            SNode* neighbor = node->neighbors[j];

            //if (neighbor->personName[0] <= 'L' && node->personName[0] > 'L')
            //    continue;

            //if (neighbor->personName[0] > 'L' && node->personName[0] <= 'L')
            //    continue;

            C3Vector dir = neighbor->position - node->position;
            real64 dist = dir.magnitude();
            if (dist > 100.0f)
                dist = 100.0f;
            real64 springMagnitude = c_springConstant * dist;
            dir /= dist;
            force += dir * springMagnitude;
        }

        // Experimental force towards center based on property
        //real32 dist = node->position.x;
        //real32 ratio;
        //if (node->personName[0] <= 'L')
        //    ratio = -1.0f;
        //else
  //          ratio = 1.0f;

        //ratio *= 20;
        //real32 delta = ratio - dist;
        //force.x += delta * 5.0f;

        node->velocity = (node->velocity + (force * dt)) * c_damping;
    }

    for (uint i = 0; i < m_nodes.size(); i++) {
        SNode* node = m_nodes[i];

        node->position += node->velocity * dt;
        kineticEnergy += node->velocity.squareMagnitude();
    }

    m_kineticEnergy = kineticEnergy;
}

//-------------------------------------------------------------------------------------------------------
void CGrapher::Update (uint32 elapsedTicks) {
    if (!m_paused) {
        CTimer timer;
        timer.Start();
        DoPhysicsUpdate(elapsedTicks);
        m_updateTime = timer.GetTicks();
    }

    m_camera.Update(elapsedTicks);

    for (uint i = 0; i < m_edges.size(); i++) {
         m_edges[i]->Update(elapsedTicks);
    }
}

//-------------------------------------------------------------------------------------------------------
void MultiplyMatrixVector (real32 mat[16], real32 vec[4]) {
    real32 result[4];
    result[0] = mat[0] * vec[0] + mat[4] * vec[1] + mat[8] * vec[2] + mat[12] * vec[3];
    result[1] = mat[1] * vec[0] + mat[5] * vec[1] + mat[9] * vec[2] + mat[13] * vec[3];
    result[2] = mat[2] * vec[0] + mat[6] * vec[1] + mat[10] * vec[2] + mat[14] * vec[3];
    result[3] = mat[3] * vec[0] + mat[7] * vec[1] + mat[11] * vec[2] + mat[15] * vec[3];

    memcpy(vec, result, sizeof(real32) * 4);
}

//-------------------------------------------------------------------------------------------------------
void CGrapher::Render () {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, c_windowWidth, c_windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(c_fov, c_aspectRatio, c_nearPlane, c_farPlane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_camera.ApplyViewTransform();

    glEnable(GL_DEPTH_TEST);

    // Render nodes first
    for (uint i = 0; i < m_nodes.size(); i++) {
        m_nodes[i]->Render();

        m_font->SetColor(0.2f, 0.2f, 0.2f);
        if (m_focusMode) {
            if (m_nodes[i]->personName == m_focusPerson)
                m_font->SetColor(0.3f, 0.3f, 0.9f);
        }
    }

    // Render edges on top of nodes, to blend

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    for (uint i = 0; i < m_edges.size(); i++) {
        if (m_focusMode) {
            if (m_edges[i]->one->personName != m_focusPerson && m_edges[i]->two->personName != m_focusPerson)
                continue;
        }

        m_edges[i]->Render();
    }

    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);

    // Then render all labels

    for (uint i = 0; i < m_nodes.size(); i++) {

        real32 pos[4] = {
            m_nodes[i]->position.x,
            m_nodes[i]->position.y,
            m_nodes[i]->position.z,
            1.0f
        };
        real32 mat[16];

        glGetFloatv(GL_MODELVIEW_MATRIX, mat);
        MultiplyMatrixVector(mat, pos);

        real32 dist = abs(pos[2]);

        glGetFloatv(GL_PROJECTION_MATRIX, mat);
        MultiplyMatrixVector(mat, pos);

        pos[0] /= pos[3];
        pos[1] /= pos[3];
        pos[2] /= pos[3];

        if (pos[2] > -1 && pos[2] < 1 && dist < 50.0f) { // Display only closer ones
            real32 ratio = dist / 50.0f;
            m_font->SetSize(0.25f * (1.5 - ratio));
            const char* name = m_nodes[i]->personName.c_str();
            char label[1024];
            sprintf(label, "%s (%d)", name, m_nodes[i]->neighbors.size());
            real32 width = m_font->GetWidth(label);
            m_font->Print(label, pos[0] - width * 0.5f, pos[1] - 0.025f);
        }
    }

    // Misc HUD Info

    m_font->SetSize(1.0f);
    char buffer[256];
    sprintf(buffer, "%.2f J", m_kineticEnergy);
    m_font->Print(buffer, 1.0f - m_font->GetWidth(buffer), -1.0f);

    sprintf(buffer, "%lu ms", m_updateTime);
    m_font->Print(buffer, -1.0f + 0.025f, -1.0f);

    //glDisable(GL_DEPTH_TEST);
    //glMatrixMode(GL_MODELVIEW);
    //glLineWidth(3.0);
    //const real32 axes[][3] = {
    //    {10.0f, 0.0f, 0.0f},
    //    {0.0f, 10.0f, 0.0f},
    //    {0.0f, 0.0f, 10.0f}
    //};
    //for (int i = 0; i < 3; i++) {
    //    glColor3fv(axes[i]);
    //    glBegin(GL_LINES);
    //    glVertex3f(0.0f, 0.0f, 0.0f);
    //    glVertex3fv(axes[i]);
    //    glEnd();
    //}
    //glEnable(GL_DEPTH_TEST);


    SDL_GL_SwapBuffers();
}

//-------------------------------------------------------------------------------------------------------
void CGrapher::HandleKeyboard (SDL_KeyboardEvent key) {
    m_camera.HandleKeyboard(key);

    switch (key.keysym.sym) {
        case SDLK_ESCAPE:
            if (key.type == SDL_KEYDOWN) {
                SDL_Event* e = new SDL_Event;
                e->type = SDL_QUIT;
                SDL_PushEvent(e);
            }
        break;
        case SDLK_f:
            if (key.type == SDL_KEYDOWN) {
                m_focusMode = !m_focusMode;
            }
        break;
        case SDLK_p:
            if (key.type == SDL_KEYDOWN) {
                m_paused = !m_paused;
            }
        break;
        case SDLK_F1:
            if (key.type == SDL_KEYUP) {
                char line[32];
                cin.getline(line, 32);
                m_focusPerson = line;
            }
        break;
        case SDLK_F2:
            if (key.type == SDL_KEYDOWN) {
                uint numIterations = 0;
                while (m_kineticEnergy > c_kineticThreshold && numIterations++ < 500) {
                    cout << m_kineticEnergy << " ";
                    DoPhysicsUpdate(20);
                }
            }
        break;
        case SDLK_F3:
            if (key.type == SDL_KEYDOWN) {
                m_edges[0]->animating = true;
            }
        break;
    default:
    break;
    }
}

//-------------------------------------------------------------------------------------------------------
void CGrapher::HandleMouseButton (SDL_MouseButtonEvent mouseButton) {
}

//-------------------------------------------------------------------------------------------------------
void CGrapher::HandleMouseMove (SDL_MouseMotionEvent mouse) {
    m_camera.HandleMouseMove(mouse);
}
