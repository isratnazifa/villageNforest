#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include "home.cpp"

bool dayMode = true;
bool showHomeScreen = true;
float boatAngle = 0.0f;
bool boatReverseX = false;
float fighterJetX = 56.0f;
float fighterJetY = 40.0f;
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
float boatProgress = 0.0f;
bool boatMovingForward = true;
float treeLeafOffsetX = 0.0f;
bool treeLeafMovingRight = true;
float fireflyTwinklePhase = 0.0f;
float birdXOffset = 0.0f;
float birdYOffset = 0.0f;
bool birdsMovingToPeak = true;
float birdWingAngleDeg = 60.0f;
bool birdWingOpening = true;
float moonX = -8.0f;
float moonPathX = -10.0f;
float p = -10.0f;
void fillCircle(float cx, float cy, float r);
void drawGrass(float x, float y, float scale);
void drawBush(float x, float y, float scale);
void drawHomeScreen();

void setRawColor(float r, float g, float b)
{
    ::glColor3f(r, g, b);
}

void setModeColor(float r, float g, float b)
{
    if (dayMode)
    {
        setRawColor(r, g, b);
    }
    else
    {
        // Night mode tint: darker and cooler for all objects.
        setRawColor(r * 0.35f, g * 0.35f, b * 0.50f);
    }
}

#define glColor3f(r, g, b) setModeColor((r), (g), (b))

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
    if (dayMode)
    {
        setRawColor(0.53f, 0.81f, 0.98f); // day sky blue
    }
    else
    {
        setRawColor(0.04f, 0.07f, 0.18f); // night sky dark navy
    }
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 28.0f);
    glVertex2f(50.0f, 28.0f);
    glVertex2f(50.0f, 50.0f);
    glVertex2f(0.0f, 50.0f);
    glEnd();
}
void drawHalfMoon(){
    if(dayMode)
    return;

    const float moonStartX = -8.0f;
    const float moonEndX = 56.0f;
    const float moonY = 41.0f;
    const float moonR = 1.8f;
    const float moonStep = 0.01f;

    if (moonX <= moonEndX)
        moonX += moonStep;
    else
        moonX = moonStartX;

    setRawColor(0.96f, 0.97f, 1.0f);
    fillCircle(moonX, moonY, moonR);

    // subtle crescent effect
    setRawColor(0.04f, 0.07f, 0.18f);
    fillCircle(moonX + 0.65f, moonY + 0.15f, moonR * 0.82f);
}

void drawFullMoon(float cx, float cy, float r)
{
    if (dayMode)
        return;

    const float xStart = -10.0f;
    const float xEnd = 53.0f;
    const float xStep = 0.025f;
    const float yAmplitude = 8.0f;

    if (moonPathX <= xEnd)
    {
        moonPathX += xStep;
    }
    else
    {
        dayMode = true;
        moonPathX = xStart;
        p = xStart;
        return;
    }


    float progress = (moonPathX - xStart) / (xEnd - xStart);
    float yOffset = yAmplitude * sinf(progress * 3.1416f);
    float moonCx = cx + moonPathX;
    float moonCy = cy + yOffset;

    // Moon halo effect (same style as sun: 2 outer glow circles).
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glColor4f(0.88f, 0.92f, 1.0f, 0.28f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(moonCx + (r * 1.22f) * cosf(angle),
                   moonCy + (r * 1.22f) * sinf(angle));
    }
    glEnd();

    ::glColor4f(0.82f, 0.88f, 1.0f, 0.18f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(moonCx + (r * 1.52f) * cosf(angle),
                   moonCy + (r * 1.52f) * sinf(angle));
    }
    glEnd();
    glDisable(GL_BLEND);

    setRawColor(0.96f, 0.97f, 1.0f);
    fillCircle(moonCx, moonCy, r);

    // Moon spots (kolonk)
    setRawColor(0.78f, 0.80f, 0.86f);
    fillCircle(moonCx - 0.45f * r, moonCy + 0.20f * r, 0.20f * r);
    fillCircle(moonCx + 0.28f * r, moonCy + 0.08f * r, 0.16f * r);
    fillCircle(moonCx - 0.05f * r, moonCy - 0.30f * r, 0.22f * r);

    setRawColor(0.70f, 0.73f, 0.80f);
    fillCircle(moonCx + 0.40f * r, moonCy - 0.18f * r, 0.12f * r);
}


void drawStars(float x1, float y1, float x2, float y2, int count)
{
    if (dayMode)
        return;

    // Fixed seed so stars stay at the same random positions each frame.
    srand(42);

    setRawColor(1.0f, 0.84f, 0.0f); // golden color
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < count; i++)
    {
        float rx = (float)rand() / (float)RAND_MAX;
        float ry = (float)rand() / (float)RAND_MAX;
        float sx = x1 + rx * (x2 - x1);
        float sy = y1 + ry * (y2 - y1);
        glVertex2f(sx, sy);
    }
    glEnd();
    glPointSize(1.0f);
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
    drawBush(24.5f, 3.8f, 0.92f);
    drawBush(30.0f, 5.8f, 1.2f);
    drawBush(35.2f, 7.9f, 0.98f);
    drawBush(41.0f, 10.8f, 1.1f);
    drawBush(45.7f, 12.9f, 0.93f);
    drawBush(50.0f, 15.6f, 0.95f);
    drawBush(53.2f, 18.2f, 0.88f);
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
    if (!dayMode)
        return;

    glColor3f(0.12f, 0.12f, 0.12f);

    auto drawBird = [](float x, float y, float wing, float angleDeg)
    {
        const float pi = 3.14159265358979323846f;
        const float halfAngleRad = (angleDeg * 0.5f) * (pi / 180.0f);
        const float dx = wing * cosf(halfAngleRad);
        const float dy = wing * sinf(halfAngleRad);

        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x - dx, y + dy);
        glVertex2f(x, y);
        glVertex2f(x + dx, y + dy);
        glEnd();
    };

    glLineWidth(2.0f);
    glPushMatrix();
    glTranslatef(birdXOffset, birdYOffset, 0.0f);
    drawBird(31.0f, 24.8f, 0.7f, birdWingAngleDeg);
    drawBird(32.8f, 25.4f, 0.55f, birdWingAngleDeg);
    drawBird(34.3f, 24.9f, 0.65f, birdWingAngleDeg);
    drawBird(36.0f, 25.7f, 0.6f, birdWingAngleDeg);
    drawBird(37.5f, 25.1f, 0.5f, birdWingAngleDeg);
    glPopMatrix();
    glLineWidth(1.0f);
}

// draw some stones
void drawStones()
{
//1st stone
    glColor3f(0.38f, 0.40f, 0.42f);
    drawEllipse(31.5f, 5.8f, 1.5f, 2.50f);
    glColor3f(0.50f, 0.52f, 0.54f);
    fillCircle(32.0f, 7.0f, 0.60f);

//2nd stone
    glColor3f(0.46f, 0.46f, 0.48f);
    drawEllipse(32.5f, 4.8f, 1.5f, 2.50f);
    glColor3f(0.56f, 0.56f, 0.58f);
    fillCircle(33.0f, 6.0f, 0.60f);

//3rd stone
    glColor3f(0.40f, 0.42f, 0.44f);
    drawEllipse(30.5f, 4.8f, 1.5f, 2.50f);
    glColor3f(0.52f, 0.52f, 0.54f);
    fillCircle(31.0f, 6.0f, 0.60f);

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

    // connect lines to the Circumference of the Ellipse
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

void drawWaves()
{
    const float phase = fireflyTwinklePhase;
    const float bandGap = 2.3f; // equal spacing between ripple lines

    glColor3f(0.78f, 0.90f, 1.0f);
    glLineWidth(1.0f);

    // 4 ripple lines with uniform distance between bands.
    // Build ripples along river center-flow so V-shapes stay parallel to river direction.
    const float centerSlope = 0.5f * ((24.0f / 46.0f) + (19.0f / 57.0f)); // avg of both river banks
    const float tx = 1.0f;
    const float ty = centerSlope;
    const float tLen = sqrtf(tx * tx + ty * ty);
    const float ux = tx / tLen; // unit tangent (river flow direction)
    const float uy = ty / tLen;
    const float nx = -uy;       // unit normal (across river)
    const float ny = ux;

    for (int band = 0; band < 4; ++band)
    {
        float bandOffset = (band - 1.5f) * bandGap;
        for (float x = 0.0f; x <= 50.0f; x += 3.2f)
        {
            float xShift = 0.36f * cosf(phase * 1.3f + x * 0.10f + band * 0.55f);
            float yFlow = 0.22f * sinf(phase * 1.6f + x * 0.22f + band * 0.40f);
            float yMid = 3.13f + centerSlope * x + bandOffset + yFlow;

            // Shift ripple center slightly along normal to keep band spacing perpendicular to flow.
            float cx = x + xShift + nx * bandOffset * 0.35f;
            float cy = yMid + ny * bandOffset * 0.35f;

            // V-shape arms: mostly along flow (tangent) with slight normal spread.
            const float armLen = 0.45f;
            const float spread = 0.20f;
            drawLine(cx - (ux * armLen + nx * spread), cy - (uy * armLen + ny * spread), cx, cy, 1.0f);
            drawLine(cx, cy, cx + (ux * armLen - nx * spread), cy + (uy * armLen - ny * spread), 1.0f);
        }
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
    if (!dayMode)
        return;
    const float xStart = -10.0f;
    const float xEnd = 53.0f;
    const float xStep = 0.0025f;
    const float yAmplitude = 8.0f;

    if (p <= xEnd)
    {
        p = p + xStep;
    }
    else
    {
        dayMode = false;
        p = xStart;
        moonPathX = xStart;
        return;
    }


    float progress = (p - xStart) / (xEnd - xStart);
    float yOffset = yAmplitude * sinf(progress * 3.1416f);
    float sunCx = cx + p;
    float sunCy = cy + yOffset;

    glutPostRedisplay();

    int i;

    // Soft outer glow behind the sun.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glColor4f(1.0f, 1.f, 1.f, 0.52f);
    glBegin(GL_POLYGON);
    for (i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(sunCx + (r * 1.2f) * cos(angle),
                   sunCy + (r * 1.2f) * sin(angle));
    }
    glEnd();
    glDisable(GL_BLEND);


    // Soft outer glow behind the sun.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glColor4f(1.0f, 1.f, 1.f, 0.32f);
    glBegin(GL_POLYGON);
    for (i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(sunCx + (r * 1.5f) * cos(angle),
                   sunCy + (r * 1.5f) * sin(angle));
    }
    glEnd();
    glDisable(GL_BLEND);

    // Main sun circle
    glColor3f(1.0f, 0.84f, 0.25f);
    glBegin(GL_POLYGON);
    for (i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(sunCx + r * cos(angle),
                   sunCy + r * sin(angle));
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

    // Leaf sway animation: +1 (right) then -1 (left) on X axis.
    glPushMatrix();
    glTranslatef(treeLeafOffsetX, 0.0f, 0.0f);

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
    glPopMatrix();
}

float cl = -10.0f;
float cl2 = 55.0f;
void drawClouds()
{
    if (!dayMode)
        return;
    if (cl <= 53)
        cl = cl + .005;
    else
        cl = -10.0f;

    if (cl2 >= -10.0f)
        cl2 -= 0.005f;
    else
        cl2 = 55.0f;

    glutPostRedisplay();

    // Cloud group 1 (left sky)
    glColor3f(0.94f, 0.98f, 1.0f);
    fillCircle(cl + 8.0f, 36.0f, 1.7f);
    fillCircle(cl + 9.8f, 36.4f, 1.5f);
    fillCircle(cl + 11.5f, 36.0f, 1.4f);
    fillCircle(cl + 9.2f, 35.3f, 1.5f);

    //fillCircle(cl2 + 33.0f, 35.5f, 1.5f);
    //fillCircle(cl2 + 34.7f, 35.9f, 1.3f);
    //fillCircle(cl2 + 36.2f, 35.5f, 1.2f);
    //fillCircle(cl2 + 34.2f, 34.9f, 1.2f);

    // Cloud group 2 (right sky)
    glColor3f(0.94f, 0.97f, 1.0f);
    fillCircle(cl2, 35.5f, 1.5f);
    fillCircle(cl2+1.7, 35.9f, 1.3f);
    fillCircle(cl2+3.2, 35.5f, 1.2f);
    fillCircle(cl2+1.2, 34.9f, 1.2f);
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

    // window color changes by mode
    if (dayMode)
        glColor3f(0.4f, 0.28f, 0.12f);
    else
        glColor3f(1.0f/ 0.35f, 1.f/ 0.35f, 0.0f/0.35f);

    // window
    glBegin(GL_QUADS);
    glVertex2d(37, 13.7);
    glVertex2d(40, 13.0);
    glVertex2d(40, 15.30);
    glVertex2d(37, 16.0);
    glEnd();

    if (dayMode)
        glColor3f(0.22f, 0.14f, 0.08f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);

        glVertex2d(37, 16.0);
        glVertex2d(37, 13.7);
        glVertex2d(40, 13.0);
        glVertex2d(40, 15.30);

        glEnd();
        glLineWidth(1.0f);

    if (!dayMode){
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);

        glVertex2d(40, 13.00);
        glVertex2d(37, 13.70);
        glVertex2d(35.5, 6.70);
        glVertex2d(41, 5.0);
        glEnd();
    }

    // window grills
    glColor3f(0.22f, 0.14f, 0.08f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    // vertical grill

    glVertex2f(37.5f, 13.55f);
    glVertex2f(37.5f, 15.85f);

    glVertex2f(38.5f, 13.35f);
    glVertex2f(38.5f, 15.65f);

    glVertex2f(38.f, 13.45f);
    glVertex2f(38.f, 15.75f);

    glVertex2f(39.f, 13.25f);
    glVertex2f(39.f, 15.55f);

    glVertex2f(39.5f, 13.15f);
    glVertex2f(39.5f, 15.45f);



    glEnd();
    glLineWidth(1.0f);

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

void drawFirefliesInForestTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int count)
{
    if (dayMode)
        return;

    // Fixed seed keeps random fireflies stable across frames.
    srand(1337);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < count; i++)
    {
        float r1 = (float)rand() / (float)RAND_MAX;
        float r2 = (float)rand() / (float)RAND_MAX;
        float sr1 = sqrtf(r1);
        float pulse = (sinf(fireflyTwinklePhase + i * 1.3f) + 1.0f) * 0.5f;

        // Uniform random point inside triangle via barycentric coordinates.
        float px = (1.0f - sr1) * x1 + sr1 * (1.0f - r2) * x2 + sr1 * r2 * x3;
        float py = (1.0f - sr1) * y1 + sr1 * (1.0f - r2) * y2 + sr1 * r2 * y3;
        float driftX = 0.20f * sinf(fireflyTwinklePhase * 0.9f + i * 0.73f);
        float driftY = 0.12f * cosf(fireflyTwinklePhase * 1.1f + i * 0.51f);
        px += driftX;
        py += driftY;

        // Glowing aura (pulsing)
        ::glColor4f(0.75f, 0.95f, 0.25f, 0.15f + 0.25f * pulse);
        fillCircle(px, py, 0.05f + 0.10f * pulse);
    }
    glDisable(GL_BLEND);

    // Bright core
    srand(1337);
    setRawColor(0.85f, 1.0f, 0.40f);
    glPointSize(1.50f);
    glBegin(GL_POINTS);
    for (int i = 0; i < count; i++)
    {
        float r1 = (float)rand() / (float)RAND_MAX;
        float r2 = (float)rand() / (float)RAND_MAX;
        float sr1 = sqrtf(r1);

        float px = (1.0f - sr1) * x1 + sr1 * (1.0f - r2) * x2 + sr1 * r2 * x3;
        float py = (1.0f - sr1) * y1 + sr1 * (1.0f - r2) * y2 + sr1 * r2 * y3;
        float driftX = 0.20f * sinf(fireflyTwinklePhase * 0.9f + i * 0.73f);
        float driftY = 0.12f * cosf(fireflyTwinklePhase * 1.1f + i * 0.51f);
        px += driftX;
        py += driftY;
        glVertex2f(px, py);
    }
    glEnd();
    glPointSize(1.0f);
}

void forest()
{
    glColor3f(0.18f, 0.42f, 0.20f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 12.0);
    glVertex2f(57.0, 31.0);
    glVertex2f(0.0, 28.0);
    glEnd();
    drawFirefliesInForestTriangle(0.0f, 12.0f, 57.0f, 31.0f, 0.0f, 28.0f, 200);

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
    // Irregular/random-looking extra grass tufts.
    drawGrass(1.3f, 12.8f, 0.72f);
    drawGrass(3.7f, 15.0f, 0.64f);
    drawGrass(6.4f, 14.4f, 0.79f);
    drawGrass(9.6f, 16.9f, 0.68f);
    drawGrass(11.4f, 15.8f, 0.73f);
    drawGrass(15.3f, 19.5f, 0.66f);
    drawGrass(18.7f, 19.7f, 0.83f);
    drawGrass(22.1f, 21.9f, 0.69f);
    drawGrass(25.6f, 23.3f, 0.75f);
    drawGrass(29.3f, 22.8f, 0.62f);
    drawGrass(31.1f, 25.1f, 0.71f);
    drawGrass(34.8f, 24.4f, 0.67f);
    drawGrass(38.9f, 27.4f, 0.74f);
    drawGrass(42.6f, 26.8f, 0.63f);
    drawGrass(46.2f, 28.8f, 0.69f);
    drawGrass(49.1f, 30.0f, 0.61f);
    drawGrass(53.4f, 30.1f, 0.66f);
    // Wider scatter across the full forest triangle.
    drawGrass(0.8f, 20.9f, 0.58f);
    drawGrass(2.6f, 23.7f, 0.62f);
    drawGrass(4.2f, 25.2f, 0.55f);
    drawGrass(5.9f, 19.6f, 0.60f);
    drawGrass(7.6f, 21.8f, 0.57f);
    drawGrass(9.1f, 24.6f, 0.63f);
    drawGrass(10.8f, 26.5f, 0.56f);
    drawGrass(13.4f, 20.6f, 0.61f);
    drawGrass(16.1f, 22.5f, 0.58f);
    drawGrass(18.2f, 24.1f, 0.64f);
    drawGrass(21.4f, 26.0f, 0.59f);
    drawGrass(23.7f, 27.3f, 0.55f);
    drawGrass(26.4f, 24.8f, 0.66f);
    drawGrass(28.7f, 26.7f, 0.60f);
    drawGrass(30.9f, 28.4f, 0.57f);
    drawGrass(33.6f, 26.1f, 0.63f);
    drawGrass(35.8f, 27.9f, 0.56f);
    drawGrass(38.1f, 29.4f, 0.61f);
    drawGrass(40.7f, 28.1f, 0.58f);
    drawGrass(43.2f, 29.8f, 0.64f);
    drawGrass(45.6f, 30.7f, 0.55f);
    drawGrass(47.9f, 30.1f, 0.59f);
    drawGrass(50.2f, 31.2f, 0.57f);
    drawGrass(52.6f, 31.6f, 0.60f);
    drawGrass(54.3f, 31.0f, 0.54f);

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

    drawFirefliesInForestTriangle(0.0f, 12.0f, 57.0f, 31.0f, 0.0f, 28.0f, 70);

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
    glVertex2d(10, 15);

    glVertex2d(10.5, 7.5);
    glVertex2d(10.5, 15);

    glEnd();

    glColor3f(0.95f, 0.90f, 0.80f);
    glBegin(GL_TRIANGLES);
    glVertex2d(9.5, 7.5);
    glVertex2d(9.5, 13);
    glVertex2d(0, 8);
    glEnd();

    /* / (majhi) sitting on the boat
    glColor3f(0.96f, 0.80f, 0.64f);
    fillCircle(12.6f, 10.4f, 0.45f); // head

    glColor3f(0.20f, 0.30f, 0.72f);
    glBegin(GL_POLYGON); // body
    glVertex2f(12.0f, 9.8f);
    glVertex2f(13.2f, 9.8f);
    glVertex2f(13.0f, 8.7f);
    glVertex2f(12.2f, 8.7f);
    glEnd();

    glColor3f(0.10f, 0.10f, 0.10f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(12.3f, 8.8f);
    glVertex2f(11.7f, 8.0f); // left leg
    glVertex2f(12.9f, 8.8f);
    glVertex2f(13.6f, 8.1f); // right leg
    glVertex2f(13.0f, 9.5f);
    glVertex2f(14.4f, 9.0f); // hand to hold oar
    glEnd();

    // Oar
    glColor3f(0.56f, 0.34f, 0.14f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(14.1f, 9.1f);
    glVertex2f(18.4f, 6.9f);
    glEnd();
    glColor3f(0.45f, 0.27f, 0.10f);
    glBegin(GL_TRIANGLES); // oar paddle
    glVertex2f(18.4f, 6.9f);
    glVertex2f(19.1f, 6.6f);
    glVertex2f(18.7f, 7.4f);
    glEnd();
    glLineWidth(1.0f);*/
}

void rotateboat(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    if (showHomeScreen)
    {
        if (key == ' ')
        {
            showHomeScreen = false;
            glutPostRedisplay();
        }
        return;
    }

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
        boatMovingForward = !boatMovingForward;
    }
    else if (key == 'm' || key == 'M')
    {
        dayMode = !dayMode;
    }
    else if (key == 27) // ESC key
    {
        exit(0);
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
    if (!dayMode)
        return;
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
    if (showHomeScreen)
    {
        glutPostRedisplay();
        glutTimerFunc(30, updateFighterJet, 0);
        return;
    }
    const float birdTargetXOffset = -30.0f;
    const float birdBaseYOffset = 0.0f;
    const float birdTargetYOffset = 50.0f - 25.7f; // y -> ymax for top-most bird
    const float minWingAngleDeg = 0.0f;
    const float maxWingAngleDeg = 120.0f;

    fireflyTwinklePhase += 0.05f;
    if (fireflyTwinklePhase > 2.0f * 3.1416f)
    {
        fireflyTwinklePhase -= 2.0f * 3.1416f;
    }

    if (birdsMovingToPeak)
    {
        birdXOffset -= 0.05f;
        birdYOffset += 0.07f;
        if (birdXOffset <= birdTargetXOffset || birdYOffset >= birdTargetYOffset)
        {
            birdXOffset = birdTargetXOffset;
            birdYOffset = birdTargetYOffset;
            birdsMovingToPeak = false;
        }
    }
    else
    {
        birdXOffset += 0.05f;
        birdYOffset -= 0.07f;
        if (birdXOffset >= 0.0f || birdYOffset <= birdBaseYOffset)
        {
            birdXOffset = 0.0f;
            birdYOffset = birdBaseYOffset;
            birdsMovingToPeak = true;
        }
    }

    if (birdWingOpening)
    {
        birdWingAngleDeg += 4.0f;
        if (birdWingAngleDeg >= maxWingAngleDeg)
        {
            birdWingAngleDeg = maxWingAngleDeg;
            birdWingOpening = false;
        }
    }
    else
    {
        birdWingAngleDeg -= 4.0f;
        if (birdWingAngleDeg <= minWingAngleDeg)
        {
            birdWingAngleDeg = minWingAngleDeg;
            birdWingOpening = true;
        }
    }

    if (treeLeafMovingRight)
    {
        treeLeafOffsetX += 0.01f;
        if (treeLeafOffsetX >= .20f)
        {
            treeLeafOffsetX = .20f;
            treeLeafMovingRight = false;
        }
    }
    else
    {
        treeLeafOffsetX -= 0.01f;
        if (treeLeafOffsetX <= -.20f)
        {
            treeLeafOffsetX = -.20f;
            treeLeafMovingRight = true;
        }
    }

    if (boatMovingForward)
    {
        boatProgress += 0.0015f;
        if (boatProgress >= 1.0f)
        {
            boatProgress = 1.0f;
            boatMovingForward = false; // reach right, then reverse
            boatReverseX = !boatReverseX;

        }
    }
    else
    {
        boatProgress -= 0.0015f;
        if (boatProgress <= 0.0f)
        {
            boatProgress = 0.0f;
            boatMovingForward = true; // reach left, move forward again
            boatReverseX = !boatReverseX;
        }
    }

    // Day mode controls jet movement and possible new collisions.
    if (dayMode)
    {
        if (!jetCollision)
        {
            fighterJetX -= 0.35f;  // right -> left
            fighterJet2X += 0.30f; // left -> right

            // Accidental collision in the sky (day mode only).
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

        if (fighterJetX < -10.0f)
        {
            fighterJetX = 56.0f;
        }
        if (fighterJet2X > 56.0f)
        {
            fighterJet2X = -8.0f;
        }
    }

    // Once collision starts, explosion should complete even if night begins.
    if (jetCollision)
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

    // Parachute keeps moving regardless of day/night.
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
    if (showHomeScreen)
    {
        drawHomeScreen();
        return;
    }

    if (dayMode)
    {
        glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // day background
    }
    else
    {
        glClearColor(0.02f, 0.03f, 0.10f, 1.0f); // night background
    }
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky();
    drawStars(0.0f, 28.0f, 50.0f, 49.0f, 200);
    //drawHalfMoon();
    drawFullMoon(0.0f, 33.0f, 1.8f);

    glColor3f(146.0f / 255.0f, 116.0f / 255.0f, 91.0f / 255.0f);
    drawVillageLand();
    drawStones();
    glColor3f(0.45f, 0.45f, 0.47f);



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




    const float boatMinX = -10.0f;
    const float boatMaxX = 50.0f;
    const float riverSlope = 19.0f / 57.0f;

    float boatX = -20 + (boatMaxX - boatMinX) * boatProgress; // -10 -> 50
    float boatY = -2.0f + (boatX - boatMinX) * riverSlope;         // নদীর ঢাল ধরে Y




    float boatScale = 1.0f - 0.4f * boatProgress; // 100% -> 60%
    float baseScaleX = 0.75f;
    float baseScaleY = 0.75f;

    glPushMatrix();
    glTranslatef(boatX, boatY, 0.0f);
    glTranslatef(10.0f, 8.0f, 0.0f); // scale around boat area
    glRotatef(boatAngle, 0.0f, 0.0f, 1.0f);//rotate around z axis

    glScalef(boatReverseX ? -(baseScaleX * boatScale) : (baseScaleX * boatScale),
             baseScaleY * boatScale, 1.0f);
    glTranslatef(-10.0f, -8.0f, 0.0f);
    drawboat();
    glPopMatrix();
    drawTree(43.5f, 22.0f, 1.55f);

    drawHouse();

    drawTree(48.5f, 22.0f, 1.55f);
    drawTree(49.5f, 16.0f, 1.55f);

    glPushMatrix();

    glTranslatef(12.0f, -5.0f, 0.0f);
    drawHouse();
    glTranslatef(-12.0f, 05.0f, 0.0f);
    glPopMatrix();

    drawFirefliesInForestTriangle(11.0f, 0.0f, 57.0f, 0.0f, 57.0f, 28.0f, 40);


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
