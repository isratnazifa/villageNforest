#include <GL/gl.h>
#include <GL/glut.h>

static void drawFilledRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

static void drawRectOutline(float x1, float y1, float x2, float y2, float width)
{
    glLineWidth(width);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
    glLineWidth(1.0f);
}

static void drawHLine(float x1, float x2, float y, float width)
{
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(x1, y);
    glVertex2f(x2, y);
    glEnd();
    glLineWidth(1.0f);
}

static void drawBitmapText(const char *text, float x, float y, void *font)
{
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
}

static int bitmapTextWidthPx(const char *text, void *font)
{
    int w = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        w += glutBitmapWidth(font, text[i]);
    }
    return w;
}

// Convert pixel width -> world units (window=500x300, ortho=50x45 -> 10 px/X-unit).
static float pxToWorldX(int px)
{
    return px / 10.0f;
}

static void drawCenteredText(const char *text, float cx, float y, void *font)
{
    float w = pxToWorldX(bitmapTextWidthPx(text, font));
    drawBitmapText(text, cx - w * 0.5f, y, font);
}

void drawHomeScreen()
{
    glClearColor(0.03f, 0.05f, 0.09f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const float panelLeft = 4.0f;
    const float panelRight = 46.0f;
    const float panelBottom = 3.0f;
    const float panelTop = 42.0f;
    const float centerX = (panelLeft + panelRight) * 0.5f;

    const float dividerTitle = 38.0f;
    const float dividerNames = 22.5f;
    const float dividerFooter = 6.5f;

    // Terminal-style panel body.
    ::glColor3f(0.06f, 0.09f, 0.14f);
    drawFilledRect(panelLeft, panelBottom, panelRight, panelTop);

    // Frame + section lines (ASCII box feel).
    ::glColor3f(0.33f, 0.76f, 0.58f);
    drawRectOutline(panelLeft, panelBottom, panelRight, panelTop, 2.0f);
    drawHLine(panelLeft, panelRight, dividerTitle, 1.6f);
    drawHLine(panelLeft, panelRight, dividerNames, 1.6f);
    drawHLine(panelLeft, panelRight, dividerFooter, 1.4f);

    // Title
    ::glColor3f(0.68f, 1.0f, 0.82f);
    drawCenteredText("VillageNForest", centerX, 39.3f, GLUT_BITMAP_HELVETICA_18);

    // Presented by block
    ::glColor3f(0.82f, 0.95f, 0.90f);
    drawCenteredText("Presented By", centerX, 34.8f, GLUT_BITMAP_HELVETICA_18);

    ::glColor3f(0.40f, 0.84f, 0.66f);
    drawCenteredText("------------------", centerX, 33.2f, GLUT_BITMAP_9_BY_15);

    ::glColor3f(0.90f, 0.98f, 0.95f);
    drawCenteredText("Raisul Islam Nahid", centerX, 30.5f, GLUT_BITMAP_HELVETICA_18);
    drawCenteredText("&", centerX, 27.5f, GLUT_BITMAP_HELVETICA_18);
    drawCenteredText("Nazifa Israt Soya", centerX, 24.5f, GLUT_BITMAP_HELVETICA_18);

    // Instruction heading
    ::glColor3f(0.82f, 0.95f, 0.90f);
    drawCenteredText("Instructions", centerX, 20.5f, GLUT_BITMAP_HELVETICA_18);

    ::glColor3f(0.40f, 0.84f, 0.66f);
    drawCenteredText("------------------", centerX, 18.9f, GLUT_BITMAP_9_BY_15);

    // Instruction list (left-aligned)
    ::glColor3f(0.88f, 0.97f, 0.93f);
    const float listX = 6.0f;
    drawBitmapText("L / R     -> Rotate boat left / right",      listX, 16.2f, GLUT_BITMAP_9_BY_15);
    drawBitmapText("O         -> Toggle boat forward / reverse", listX, 14.2f, GLUT_BITMAP_9_BY_15);
    drawBitmapText("M         -> Toggle Day / Night",            listX, 12.2f, GLUT_BITMAP_9_BY_15);
    drawBitmapText("Up / Down -> Move fighter jet up / down",    listX, 10.2f, GLUT_BITMAP_9_BY_15);

    // Footer call-to-action
    ::glColor3f(1.0f, 0.93f, 0.54f);
    drawCenteredText("Press SPACEBAR to Start", centerX, 4.2f, GLUT_BITMAP_HELVETICA_18);

    glFlush();
}
