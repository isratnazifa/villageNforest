#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

float boatAngle = 0.0f;
bool boatReverseX = false;
float fighterJetX = 56.0f;
float fighterJetY = 38.0f;
float fighterJet2X = -8.0f;
float fighterJet2Y = 37.5f;
bool jetCollision = false;
float explosionX = 0.0f;
float explosionY = 0.0f;
float explosionRadius = 0.0f;
bool parachuteActive = false;
bool parachuteLanded = false;
float parachuteX = 0.0f;
float parachuteY = 0.0f;
void fillCircle(float cx, float cy, float r);
void drawGrass(float x, float y, float scale);
void drawBush(float x, float y, float scale);

// move anything with the function.
void moveAny(void (*drawFunc)(), float tx, float ty)
{
    glPushMatrix();
    glTranslatef(tx, ty, 0.0f);
    drawFunc();
    glPopMatrix();
}

void drawSky()
{
    glColor3f(0.53f, 0.81f, 0.98f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 28.0f);
    glVertex2f(50.0f, 28.0f);
    glVertex2f(50.0f, 50.0f);
    glVertex2f(0.0f, 50.0f);
    glEnd();
}
void drawVillageLand()
{
    // Village ground patch on the right side of the river.
    glColor3f(0.60f, 0.48f, 0.34f);
    glBegin(GL_TRIANGLES);
    glVertex2f(11.0f, 0.0f);
    glVertex2f(57.0f, 0.0f);
    glVertex2f(57.0f, 24.0f);
    glEnd();

    // Grass tufts on village land.
    drawGrass(14.0f, 1.2f, 0.95f);
    drawGrass(18.0f, 2.4f, 0.90f);
    drawGrass(23.0f, 3.5f, 1.00f);
    drawGrass(28.0f, 5.1f, 0.92f);
    drawGrass(33.0f, 7.3f, 1.05f);
    drawGrass(39.0f, 10.6f, 0.96f);
    drawGrass(45.0f, 13.8f, 0.98f);
    drawGrass(51.0f, 16.9f, 0.90f);

    // Bush clusters on village land.
    drawBush(20.0f, 2.2f, 1.0f);
    drawBush(30.0f, 5.8f, 1.2f);
    drawBush(41.0f, 10.8f, 1.1f);
    drawBush(50.0f, 15.6f, 0.95f);
}

void drawVillageRoads()
{
}

void drawCircle(float cx, float cy)
{
    int i;
    float r = 5.0;
    glBegin(GL_POLYGON);
    for (i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(cx + r * cos(angle),
                   cy + r * sin(angle));
    }
    glEnd();
}

void drawEllipse(float cx, float cy, float rx, float ry)
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(cx + rx * cos(angle),
                   cy + ry * sin(angle));
    }
    glEnd();
}

void drawhalfCircle(float cx, float cy, float r)
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i < 50; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(cx + r * cos(angle),
                   cy + r * sin(angle));
    }
    glEnd();
}

void fillCircle(float cx, float cy, float r)
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(cx + r * cos(angle),
                   cy + r * sin(angle));
    }
    glEnd();
}

void drawBush(float x, float y, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    glColor3f(0.12f, 0.44f, 0.14f);
    fillCircle(0.0f, 0.0f, 0.9f);
    fillCircle(-0.85f, 0.10f, 0.65f);
    fillCircle(0.85f, 0.12f, 0.62f);
    glColor3f(0.18f, 0.58f, 0.20f);
    fillCircle(-0.28f, 0.45f, 0.45f);
    fillCircle(0.32f, 0.42f, 0.40f);

    glPopMatrix();
}

void drawCircleR(float cx, float cy, float r)
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(cx + r * cos(angle),
                   cy + r * sin(angle));
    }
    glEnd();
}

void fillRect(float x, float y, float w, float h)
{
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2, float width)
{
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1.0f);
}

void drawRiver()
{
    glColor3f(0.16f, 0.45f, 0.78f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0, 0.0);
    glVertex2f(11.0, 0.0);
    glVertex2d(57, 24);
    glVertex2d(57, 31);
    glVertex2d(0, 12);
    glEnd();
}

float fenceBase(float x, bool upperBank)
{
    if (upperBank)
    {
        // Upper river edge: (0,12) -> (57,31)
        return 12.0f + (19.0f / 57.0f) * x;
    }

    // Bottom river edge: (11,0) -> (57,24)
    return (12.0f / 23.0f) * (x - 11.0f);
}

void drawFenceLine(float xStart, float xEnd, float spacing, float postHW, float postH,
                   float railHi, float railLo, float gapLeft, float gapRight, bool upperBank)
{
    // Draw post bodies
    for (float px = xStart; px <= xEnd + 0.1f; px += spacing)
    {
        if (px > gapLeft && px < gapRight)
            continue; // gate gap

        float py = fenceBase(px, upperBank);
        glColor3f(0.42f, 0.24f, 0.07f);
        fillRect(px - postHW, py, postHW * 2.0f, postH);
        glColor3f(0.28f, 0.14f, 0.03f);
        fillRect(px + postHW * 0.42f, py, postHW * 0.58f, postH);
    }

    // Left rail segment
    glColor3f(0.60f, 0.38f, 0.14f);
    drawLine(xStart, fenceBase(xStart, upperBank) + railHi, gapLeft, fenceBase(gapLeft, upperBank) + railHi, 4.0f);
    drawLine(xStart, fenceBase(xStart, upperBank) + railLo, gapLeft, fenceBase(gapLeft, upperBank) + railLo, 4.0f);
    glColor3f(0.78f, 0.58f, 0.28f);
    drawLine(xStart, fenceBase(xStart, upperBank) + railHi + 0.15f, gapLeft, fenceBase(gapLeft, upperBank) + railHi + 0.15f, 1.2f);
    drawLine(xStart, fenceBase(xStart, upperBank) + railLo + 0.15f, gapLeft, fenceBase(gapLeft, upperBank) + railLo + 0.15f, 1.2f);
    glColor3f(0.28f, 0.14f, 0.04f);
    drawLine(xStart, fenceBase(xStart, upperBank) + railHi - 0.15f, gapLeft, fenceBase(gapLeft, upperBank) + railHi - 0.15f, 1.2f);
    drawLine(xStart, fenceBase(xStart, upperBank) + railLo - 0.15f, gapLeft, fenceBase(gapLeft, upperBank) + railLo - 0.15f, 1.2f);

    // Right rail segment
    glColor3f(0.60f, 0.38f, 0.14f);
    drawLine(gapRight, fenceBase(gapRight, upperBank) + railHi, xEnd, fenceBase(xEnd, upperBank) + railHi, 4.0f);
    drawLine(gapRight, fenceBase(gapRight, upperBank) + railLo, xEnd, fenceBase(xEnd, upperBank) + railLo, 4.0f);
    glColor3f(0.78f, 0.58f, 0.28f);
    drawLine(gapRight, fenceBase(gapRight, upperBank) + railHi + 0.15f, xEnd, fenceBase(xEnd, upperBank) + railHi + 0.15f, 1.2f);
    drawLine(gapRight, fenceBase(gapRight, upperBank) + railLo + 0.15f, xEnd, fenceBase(xEnd, upperBank) + railLo + 0.15f, 1.2f);
    glColor3f(0.28f, 0.14f, 0.04f);
    drawLine(gapRight, fenceBase(gapRight, upperBank) + railHi - 0.15f, xEnd, fenceBase(xEnd, upperBank) + railHi - 0.15f, 1.2f);
    drawLine(gapRight, fenceBase(gapRight, upperBank) + railLo - 0.15f, xEnd, fenceBase(xEnd, upperBank) + railLo - 0.15f, 1.2f);

    // Post edge details and top caps
    glColor3f(0.25f, 0.12f, 0.02f);
    for (float px = xStart; px <= xEnd + 0.1f; px += spacing)
    {
        if (px > gapLeft && px < gapRight)
            continue;
        float py = fenceBase(px, upperBank);
        float top = py + postH;
        drawLine(px - postHW, py, px - postHW, top, 1.0f);
        drawLine(px - postHW, top, px + postHW, top, 1.0f);
    }
}

// Draw fence beside both river banks.
void drawFence()
{
    const float postHW = 0.18f;
    const float postH = 1.25f;
    const float spacing = 1.9f;
    const float railHi = postH * 0.74f;
    const float railLo = postH * 0.40f;
    const float gapLeft = 23.0f;
    const float gapRight = 27.0f;

    // Bottom bank segment follows (11,0) -> (57,24)
    drawFenceLine(11.2f, 49.0f, spacing, postHW, postH, railHi, railLo, gapLeft, gapRight, false);

    // Upper bank segment follows (0,12) -> (57,31)
    drawFenceLine(0.5f, 49.0f, spacing, postHW, postH, railHi, railLo, gapLeft, gapRight, true);
}

// Backward compatible name (typo kept for existing calls if any).
void drawFench()
{
    drawFence();
}

// draw birds
void drawBirds()
{
    glColor3f(0.12f, 0.12f, 0.12f);

    auto drawBird = [](float x, float y, float wing)
    {
        glBegin(GL_LINE_STRIP);
        glVertex2f(x - wing, y);
        glVertex2f(x, y + wing * 0.55f);
        glVertex2f(x + wing, y);
        glEnd();
    };

    glLineWidth(2.0f);
    drawBird(21.0f, 34.8f, 0.7f);
    drawBird(22.8f, 35.4f, 0.55f);
    drawBird(24.3f, 34.9f, 0.65f);
    drawBird(26.0f, 35.7f, 0.6f);
    drawBird(27.5f, 35.1f, 0.5f);
    glLineWidth(1.0f);
}

// draw some stones
void drawStones()
{
}

void drawParachute(float x, float y, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    glColor3f(1.0f, 0.30f, 0.30f);
    drawhalfCircle(0.0f, 0.0f, 3.0f);
    glColor3f(0.80f, 0.30f, 0.60f);
    drawEllipse(0.0f, 0.0f, 3.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glRectd(-1.0f, -2.0f, 1.0f, -3.0f);

    //connect lines to the Circumference of the Ellipse
    glBegin(GL_LINES);
    glVertex2f(-1.8f, -0.8f);
    glVertex2f(-1.0f, -2.0f);

    glVertex2f(-0.7f, -1.0f);
    glVertex2f(-0.3f, -2.0f);

    glVertex2f(0.7f, -1.0f);
    glVertex2f(0.3f, -2.0f);

    glVertex2f(1.8f, -0.8f);
    glVertex2f(1.0f, -2.0f);

    glEnd();
    glPopMatrix();
}



// draw grass
void drawGrass(float x, float y, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    glColor3f(0.12f, 0.54f, 0.16f);
    drawLine(0.0f, 0.0f, -0.25f, 1.0f, 1.0f);
    drawLine(0.0f, 0.0f, 0.0f, 1.25f, 1.0f);
    drawLine(0.0f, 0.0f, 0.25f, 1.05f, 1.0f);

    glColor3f(0.18f, 0.64f, 0.22f);
    drawLine(0.08f, 0.0f, 0.45f, 0.85f, 1.0f);
    drawLine(-0.08f, 0.0f, -0.45f, 0.80f, 1.0f);

    glPopMatrix();
}

void drawGrass()
{
    drawGrass(3.0f, 11.8f, 1.1f);
    drawGrass(5.5f, 12.3f, 0.9f);
    drawGrass(9.0f, 13.1f, 1.0f);
}

void drawWaves(){
    glColor3f(0.78f, 0.90f, 1.0f);
    glLineWidth(1.5f);

    // Small diagonal ripples that follow river flow.
    for (float x = 8.0f; x <= 50.0f; x += 6.0f)
    {
        float yMid = 0.52f * (x - 11.0f) + 2.4f; // around lower river band
        drawLine(x - 1.0f, yMid - 0.25f, x, yMid + 0.10f, 1.2f);
        drawLine(x, yMid + 0.10f, x + 1.0f, yMid - 0.25f, 1.2f);
    }

    for (float x = 5.0f; x <= 47.0f; x += 7.0f)
    {
        float yMid = 12.0f + (19.0f / 57.0f) * x - 2.4f; // around upper river band
        drawLine(x - 0.9f, yMid - 0.2f, x, yMid + 0.15f, 1.2f);
        drawLine(x, yMid + 0.15f, x + 0.9f, yMid - 0.2f, 1.2f);
    }

    glLineWidth(1.0f);
}

// draw tree shadow
void drawTreeShadow(float x, float y, float sx, float sy)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(sx, sy, 1.0f);

    glColor3f(0.09f, 0.22f, 0.10f);
    fillCircle(0.0f, 0.0f, 1.0f);

    glPopMatrix();
}

void drawTreeShadow()
{
    drawTreeShadow(6.0f, 16.5f, 1.9f, 0.55f);
    drawTreeShadow(9.0f, 17.3f, 1.7f, 0.50f);
}

// draw tree trunk
void drawTreeTrunk(float x, float y, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    glColor3f(0.42f, 0.24f, 0.07f);
    fillRect(-0.7f, -4.5f, 1.4f, 4.8f);
    glColor3f(0.28f, 0.14f, 0.03f);
    fillRect(0.1f, -4.5f, 0.6f, 4.8f);

    glPopMatrix();
}

void drawTreeTrunk()
{
    drawTreeTrunk(12.0f, 22.0f, 0.95f);
}

void drawSun(float cx, float cy, float r)
{
    int i;
    glColor3f(1.0f, 0.84f, 0.25f);

    glBegin(GL_POLYGON);
    for (i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(cx + r * cos(angle),
                   cy + r * sin(angle));
    }
    glEnd();
}

void drawTree(float x, float y, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    // Trunk
    glColor3f(0.42f, 0.24f, 0.07f);
    fillRect(-0.7f, -4.5f, 1.4f, 6.8f);
    glColor3f(0.28f, 0.14f, 0.03f);
    fillRect(0.1f, -4.5f, 0.6f, 6.8f); // shadow strip on right

    // Bark texture lines
    glColor3f(0.22f, 0.11f, 0.02f);
    drawLine(-0.45f, -4.0f, -0.45f, -1.8f, 1.0f);
    drawLine(-0.10f, -4.3f, -0.10f, -0.8f, 1.0f);
    drawLine(0.35f, -3.8f, 0.35f, -2.2f, 1.0f);

    // Root bumps
    glColor3f(0.35f, 0.18f, 0.05f);
    fillCircle(-0.55f, -4.5f, 0.28f);
    fillCircle(0.55f, -4.5f, 0.28f);

    // Canopy dark base
    glColor3f(0.06f, 0.32f, 0.05f);
    fillCircle(0.0f, 2.2f, 2.4f);
    fillCircle(-1.7f, 1.7f, 1.9f);
    fillCircle(1.6f, 1.7f, 1.9f);
    fillCircle(0.0f, 3.7f, 1.9f);

    // Canopy highlights
    glColor3f(0.14f, 0.46f, 0.12f);
    fillCircle(-0.6f, 3.2f, 1.2f);
    fillCircle(-2.1f, 2.4f, 0.9f);
    fillCircle(0.0f, 4.6f, 1.0f);

    // Lower-right canopy shadow blobs
    glColor3f(0.03f, 0.20f, 0.03f);
    fillCircle(1.3f, 1.2f, 1.1f);
    fillCircle(0.4f, 1.5f, 0.8f);

    glPopMatrix();
}
void drawClouds()
{
    // Cloud group 1 (left sky)
    glColor3f(0.96f, 0.98f, 1.0f);
    fillCircle(8.0f, 36.0f, 1.7f);
    fillCircle(9.8f, 36.4f, 1.5f);
    fillCircle(11.5f, 36.0f, 1.4f);
    fillCircle(9.2f, 35.3f, 1.5f);

    // Cloud group 2 (right sky)
    glColor3f(0.94f, 0.97f, 1.0f);
    fillCircle(33.0f, 35.5f, 1.5f);
    fillCircle(34.7f, 35.9f, 1.3f);
    fillCircle(36.2f, 35.5f, 1.2f);
    fillCircle(34.2f, 34.9f, 1.2f);
}

void drawHill(float x, float y, float sx, float sy, float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(sx, sy, 1.0f);

    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(-0.92f, 28.02f);
    glVertex2f(-0.17f, 29.88f);
    glVertex2f(0.88f, 31.90f);
    glVertex2f(1.79f, 33.14f);
    glVertex2f(2.75f, 34.26f);
    glVertex2f(3.88f, 35.00f);
    glVertex2f(5.25f, 35.60f);
    glVertex2f(7.08f, 36.22f);
    glVertex2f(8.83f, 36.58f);
    glVertex2f(10.21f, 36.68f);
    glVertex2f(12.71f, 36.72f);
    glVertex2f(14.21f, 36.68f);
    glVertex2f(15.83f, 36.36f);
    glVertex2f(17.42f, 35.72f);
    glVertex2f(18.67f, 35.10f);
    glVertex2f(19.75f, 34.38f);
    glVertex2f(20.88f, 33.70f);
    glVertex2f(21.71f, 32.78f);
    glVertex2f(22.58f, 31.94f);
    glVertex2f(23.29f, 30.84f);
    glVertex2f(23.79f, 29.98f);
    glVertex2f(23.96f, 29.60f);
    glVertex2f(24.04f, 28.00f);
    glEnd();

    glPopMatrix();
}

void drawHills()
{
    // Mixed big and small hills with aligned roots.
    // Root alignment rule: y = 28 * (1 - sy)
    drawHill(-9.0f, 5.6f, 0.500f, 0.90f, 0.38f, 0.66f, 0.32f);
    drawHill(-6.0f, 5.6f, 0.90f, 0.80f, 0.38f, 0.66f, 0.32f);   // big
    drawHill(7.0f, 7.0f, 0.56f, 0.56f, 0.34f, 0.60f, 0.28f);    // small
    drawHill(14.0f, 5.6f, 0.60f, 0.70f, 0.32f, 0.58f, 0.27f);   // medium
    drawHill(22.0f, 5.04f, 0.700f, 0.75f, 0.36f, 0.64f, 0.30f); // big
    drawHill(33.0f, 7.84f, 0.50f, 0.70f, 0.30f, 0.56f, 0.26f);  // small
    drawHill(38.0f, 7.0f, 0.50f, 0.70f, 0.33f, 0.59f, 0.28f);   // small
    drawHill(44.0f, 5.6f, 0.46f, 0.750f, 0.35f, 0.63f, 0.30f);  // big
}

void drawHouse()
{
    // pentagon wall
    glColor3f(0.54f, 0.28f, 0.4f);
    glBegin(GL_POLYGON);
    glVertex2f(35.0, 11.0);
    glVertex2f(42.0, 9.0);
    glVertex2f(42.0, 17.50);
    glVertex2f(38.0, 21.0);
    glVertex2f(35.0, 17.0);
    glEnd();

    // WALL
    glColor3f(0.54f, 0.28f, 0.12f);
    glBegin(GL_POLYGON);
    glVertex2f(42.0, 17.50);
    glVertex2d(42.0, 9.0);
    glVertex2d(48, 12.5);
    glVertex2f(48.0, 19.0);
    glEnd();

    // cieling
    glColor3f(0.54f, 0.8f, 0.12f);
    glBegin(GL_POLYGON);
    glVertex2f(43.0, 16.0);
    glVertex2f(49.0, 19.0);
    glVertex2f(45.0, 23.0);
    glVertex2f(38.0, 21.0);

    glEnd();
    // cieling backside
    glBegin(GL_QUADS);
    glVertex2f(35.0, 17.0);
    glVertex2f(38.20, 20.8);
    glVertex2f(38.0, 21.0);
    glVertex2f(34.00, 16.5);
    glEnd();

    // door
    glColor3f(0.4f, 0.28f, 0.12f);
    glBegin(GL_POLYGON);
    glVertex2d(44, 10.1);
    glVertex2d(46, 11.3);
    glVertex2d(46, 16.5);
    glVertex2d(44, 15.5);
    glEnd();

    // window
    glBegin(GL_QUADS);
    glVertex2d(37, 13.7);
    glVertex2d(40, 13.0);
    glVertex2d(40, 15.30);
    glVertex2d(37, 16.0);
    glEnd();

    // window grills
    glBegin(GL_LINES);

    glEnd();

    // root
    glBegin(GL_QUADS);
    glVertex2d(34, 10);
    glVertex2d(41.8, 7.5);
    glVertex2d(42, 9);
    glVertex2d(35, 11);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(41.8, 7.5);
    glVertex2d(48.8, 11.8);
    glVertex2d(48, 12.5);
    glVertex2d(42, 9);

    glEnd();
}

void scaleAny(void (*drawFunc)(), float sx, float sy)
{
    glPushMatrix();

    glScalef(sx, sy, 1.0f);

    drawFunc(); // draw anything

    glPopMatrix();
}

void forest()
{
    glColor3f(0.18f, 0.42f, 0.20f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 12.0);
    glVertex2f(57.0, 31.0);
    glVertex2f(0.0, 28.0);
    glEnd();

    // Ground details on forest floor using reusable helpers.



    drawGrass(2.0f, 13.2f, 0.9f);
    drawGrass(4.5f, 14.1f, 0.85f);
    drawGrass(8.0f, 15.6f, 0.95f);
    drawGrass(12.5f, 17.0f, 0.8f);
    drawGrass(17.0f, 18.8f, 0.75f);
    // Extra grass along the upper river bank (land side).

    drawGrass(20.0f, 20.0f, 0.70f);
    drawGrass(24.0f, 21.1f, 0.78f);
    drawGrass(28.0f, 22.4f, 0.72f);
    drawGrass(32.0f, 23.8f, 0.80f);
    drawGrass(36.0f, 24.9f, 0.76f);
    drawGrass(40.0f, 26.5f, 0.82f);
    drawGrass(44.0f, 27.9f, 0.74f);
    drawGrass(48.0f, 29.2f, 0.78f);
    drawGrass(52.0f, 30.8f, 0.70f);

    // Layered trees with roots kept inside the triangle area.

    drawTree(3.0f, 28.38f, 0.85f);
    drawTree(0.0f, 28.38f, 0.75f);
    drawTree(6.0f, 28.38f, 0.75f);
    drawTree(8.0f, 25.38f, 0.75f);
    drawTree(0.0f, 21.38f, 0.75f);
    drawTree(1.8f, 22.0f, 0.72f);
    drawTree(14.0f, 23.9f, 0.64f);
    drawTree(20.0f, 24.8f, 0.60f);
    drawTree(27.0f, 27.0f, 0.58f);
    drawTree(34.0f, 29.3f, 0.56f);
    drawTree(41.0f, 31.6f, 0.54f);
    drawTree(48.0f, 31.8f, 0.52f);
}

void drawboat()
{
    glColor3f(0.54f, 0.28f, 0.12f);
    glBegin(GL_POLYGON);
    glVertex2d(4, 8);
    glVertex2f(7.0, 5.5);
    glVertex2d(16, 6);
    glVertex2d(19, 9);

    glEnd();

    glColor3f(0.92f, 0.72f, 0.48f);
    glBegin(GL_LINE_LOOP);
    glVertex2d(4, 8);
    glVertex2f(7.0, 5.5);
    glVertex2d(16, 6);
    glVertex2d(19, 9);
    glVertex2f(15.0, 7.5);
    glVertex2d(8, 7);
    glEnd();

    glColor3f(0.92f, 0.72f, 0.48f);
    glBegin(GL_LINES);

    glVertex2d(19, 9);
    glVertex2d(4, 8);

    glEnd();


    glColor3f(0.92f, 0.72f, 0.48f);
    glLineWidth(5);
    glBegin(GL_LINES);

    glVertex2d(10, 7.5);
    glVertex2d(10,15);

    glEnd();

    glColor3f(0.95f, 0.90f, 0.80f);
    glBegin(GL_TRIANGLES);
    glVertex2d(8, 7);
    glVertex2d(8, 13);
    glVertex2d(0, 8);
    glEnd();
}

void rotateboat(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    if (key == 'l' || key == 'L')
    {
        boatAngle += 10.0f; // left rotation
    }
    else if (key == 'r' || key == 'R')
    {
        boatAngle -= 10.0f; // right rotation
    }
    else if (key == 'o' || key == 'O')
    {
        boatReverseX = !boatReverseX; // toggle reverse by x direction
    }
    else
    {
        return;
    }

    if (boatAngle >= 360.0f)
        boatAngle -= 360.0f;
    else if (boatAngle <= -360.0f)
        boatAngle += 360.0f;

    glutPostRedisplay();
}

void controlJetY(int key, int x, int y)
{
    (void)x;
    (void)y;

    if (key == GLUT_KEY_UP)
    {
        fighterJetY += 0.7f;
        if (fighterJetY > 43.0f)
            fighterJetY = 43.0f;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        fighterJetY -= 0.7f;
        if (fighterJetY < 30.0f)
            fighterJetY = 30.0f;
    }
    else
    {
        return;
    }

    glutPostRedisplay();
}

void drawFighterJet(float x, float y, float scale, bool faceLeft)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(faceLeft ? -scale : scale, scale, 1.0f);

    // Body
    glColor3f(0.72f, 0.75f, 0.80f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.8f, 0.0f);
    glVertex2f(-0.8f, 0.45f);
    glVertex2f(1.1f, 0.45f);
    glVertex2f(1.9f, 0.0f);
    glVertex2f(1.1f, -0.45f);
    glVertex2f(-0.8f, -0.45f);
    glEnd();

    // Nose tip
    glColor3f(0.58f, 0.62f, 0.68f);
    glBegin(GL_TRIANGLES);
    glVertex2f(1.9f, 0.0f);
    glVertex2f(1.2f, 0.24f);
    glVertex2f(1.2f, -0.24f);
    glEnd();

    // Wings
    glColor3f(0.66f, 0.70f, 0.76f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(-1.4f, 1.0f);
    glVertex2f(0.8f, 0.15f);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(-1.4f, -1.0f);
    glVertex2f(0.8f, -0.15f);
    glEnd();

    // Tail fins
    glColor3f(0.60f, 0.64f, 0.70f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.6f, 0.0f);
    glVertex2f(-2.2f, 0.9f);
    glVertex2f(-1.2f, 0.2f);
    glVertex2f(-1.6f, 0.0f);
    glVertex2f(-2.2f, -0.9f);
    glVertex2f(-1.2f, -0.2f);
    glEnd();

    // Cockpit
    glColor3f(0.20f, 0.35f, 0.55f);
    drawEllipse(0.45f, 0.15f, 0.35f, 0.20f);

    glPopMatrix();
}

void drawExplosion(float x, float y, float r)
{
    glColor3f(1.0f, 0.72f, 0.12f);
    fillCircle(x, y, r);
    glColor3f(1.0f, 0.45f, 0.05f);
    fillCircle(x - r * 0.25f, y + r * 0.05f, r * 0.65f);
    glColor3f(1.0f, 0.90f, 0.45f);
    fillCircle(x + r * 0.18f, y - r * 0.08f, r * 0.38f);
}

void updateFighterJet(int value)
{
    (void)value;

    if (!jetCollision)
    {
        fighterJetX -= 0.35f; // right -> left
        fighterJet2X += 0.30f; // left -> right

        // Accidental collision in the sky.
        if (fabsf(fighterJetX - fighterJet2X) < 1.2f && fabsf(fighterJetY - fighterJet2Y) < 1.0f)
        {
            jetCollision = true;
            explosionX = (fighterJetX + fighterJet2X) * 0.5f;
            explosionY = (fighterJetY + fighterJet2Y) * 0.5f;
            explosionRadius = 0.45f;
            parachuteActive = true;
            parachuteLanded = false;
            parachuteX = explosionX;
            parachuteY = explosionY - 0.4f;
        }
    }
    else
    {
        explosionRadius += 0.22f;
        if (explosionRadius > 2.8f)
        {
            jetCollision = false;
            fighterJetX = 56.0f;
            fighterJetY = 38.0f;
            fighterJet2X = -8.0f;
            fighterJet2Y = 37.5f;
            explosionRadius = 0.0f;
        }
    }

    if (fighterJetX < -10.0f)
    {
        fighterJetX = 56.0f;
    }
    if (fighterJet2X > 56.0f)
    {
        fighterJet2X = -8.0f;
    }

    // Parachute: appears after collision, then drifts and lands over forest.
    if (parachuteActive && !parachuteLanded)
    {
        parachuteX -= 0.07f; // wind drift toward forest
        parachuteY -= 0.10f; // gradual descent
        if (parachuteY <= 25.8f)
        {
            parachuteY = 25.8f;
            parachuteLanded = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, updateFighterJet, 0);
}

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    drawSky();


    glColor3f(146.0f/255.0f, 116.0f/255.0f, 91.0f/255.0f);
    drawVillageLand();

    // DRAW SUN
    drawSun(0.0, 33.0, 1.5);
    drawClouds();

    // Draw parachute first so blast can appear in front of it.
    if (parachuteActive)
    {
        drawParachute(parachuteX, parachuteY, 0.35f);
    }

    if (!jetCollision)
    {
        drawFighterJet(fighterJetX, fighterJetY, 0.9f, true);
        drawFighterJet(fighterJet2X, fighterJet2Y, 0.82f, false);
    }
    else
    {
        drawExplosion(explosionX, explosionY, explosionRadius);
    }
    drawBirds();

    drawHills();
    forest();

    drawRiver();
    drawWaves();
    drawFence();

    glPushMatrix();
    glTranslatef(10.0f, 8.0f, 0.0f); // scale around boat area
    glRotatef(boatAngle, 0.0f, 0.0f, 1.0f);
    glScalef(boatReverseX ? -0.75f : 0.75f, 0.75f, 1.0f);
    glTranslatef(-10.0f, -8.0f, 0.0f);
    drawboat();
    glPopMatrix();
    drawTree(43.5f, 22.0f, 1.55f);


    drawHouse();


    drawTree(49.5f, 16.0f, 1.55f);

    glPushMatrix();

    glTranslatef(12.0f, -5.0f, 0.0f);
    drawHouse();
    glTranslatef(-12.0f, 05.0f, 0.0f);
    glPopMatrix();



    glFlush();
}

void init(void)
{
    /* select clearing (background) color */
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 50.0, 0.0, 45.0, -10.0, 10.0);
}
/*
 * Declare initial window size, position, and display mode
 * (single buffer and RGBA). Open window with "hello"
 * in its title bar. Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("hello");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(rotateboat);
    glutSpecialFunc(controlJetY);
    glutTimerFunc(30, updateFighterJet, 0);
    glutMainLoop();
    return 0; /* ISO C requires main to return int. */
}
