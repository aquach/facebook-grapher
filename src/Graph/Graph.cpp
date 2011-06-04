#include "Graph.h"
#include "Gfx/GfxBase.h"
#include "CGrapher.h"

GLUquadric* quadric = gluNewQuadric();
const uint32 c_nodeSides = 8;
const real32 c_nodeSize = 0.1f;

const real32 c_edgeAnimateSpeed = 10.0f;

CGrapher* SNode::grapher = NULL;

//-------------------------------------------------------------------------------------------------------
void SEdge::Render () {
    real32 t = percent * 0.01f;
    if (t < 0) t = 0; if (t > 1.0f) t = 1.0f;

    SNode* end = GetOtherNode(start);
    if (end == NULL)
        return;

    C3Vector pt = start->position * (1 - t) + end->position * t;
    glLineWidth(1.0f);

    glColor3f(0.3f, 0.3f, 0.9f);
    glBegin(GL_LINES);
        glVertex3fv((GLfloat*)&start->position);
        glVertex3fv((GLfloat*)&pt);
    glEnd();

    glColor3f(0.9f, 0.3f, 0.3f);
    glBegin(GL_LINES);
        glVertex3fv((GLfloat*)&pt);
        glVertex3fv((GLfloat*)&end->position);
    glEnd();
}

//-------------------------------------------------------------------------------------------------------
void SEdge::Update (uint32 elapsedTicks) {
    if (!animating)
        return;

    percent += c_edgeAnimateSpeed * elapsedTicks * 0.001f;
    if (percent > 100.0f) {
        animating = false;

        // Propagate animation
        SNode* end = GetOtherNode(start);
        if (end != NULL) {
            for (uint i = 0; i < end->edges.size(); i++) {
                SEdge* edge = end->edges[i];
                if (edge->percent == 0.0f) {
                    edge->animating = true;
                    edge->start = end;
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------
void SNode::Render () {
    if (gender == e_genderMale)
        glColor3f(0.1f, 0.5f, 0.5f);
    else if (gender == e_genderFemale)
        glColor3f(0.5f, 0.1f, 0.5f);
    else
        glColor3f(0.6f, 0.6f, 0.6f);

    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    //uint32 numNeighbors = neighbors.size();
    //real32 ratio = pow(numNeighbors / 10.0f, 0.1f);
    //glScalef(ratio, ratio, ratio);
    gluSphere(quadric, c_nodeSize, c_nodeSides, c_nodeSides);
    glPopMatrix();
}
