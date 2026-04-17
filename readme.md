# `main.cpp` Function-by-Function Guide

This file builds a full animated village scene using OpenGL/GLUT (day/night cycle, river, birds, forest, boats, fighter jets, parachute, moon/sun transitions, and effects).

Below, every function is explained with:
- **What it does**
- **Algorithm/strategy**
- **How it works in the scene**

---

## 1) Color and Utility Infrastructure

### `setRawColor(float r, float g, float b)`
- **What it does:** Sets OpenGL color directly without day/night tint logic.
- **Algorithm/strategy:** Thin wrapper over `::glColor3f(...)` (global namespace call).
- **How it works:** Used when a color must remain exact (for glow/effects), bypassing the macro-based mode tinting.

### `setModeColor(float r, float g, float b)`
- **What it does:** Applies day/night-aware tinting.
- **Algorithm/strategy:**  
  - If `dayMode == true`, keep original color.  
  - Else darken and cool the color (`r*0.35`, `g*0.35`, `b*0.50`).
- **How it works:** Most scene objects use this automatically through the `glColor3f` macro.

### `moveAny(void (*drawFunc)(), float tx, float ty)`
- **What it does:** Moves any no-argument draw function by translation.
- **Algorithm/strategy:** `glPushMatrix -> glTranslatef -> drawFunc() -> glPopMatrix`.
- **How it works:** Reusable transform helper to avoid repeating matrix boilerplate.

### `scaleAny(void (*drawFunc)(), float sx, float sy)`
- **What it does:** Scales any no-argument draw function.
- **Algorithm/strategy:** `glPushMatrix -> glScalef -> drawFunc() -> glPopMatrix`.
- **How it works:** Reuses a base draw routine at different sizes.

---

## 2) Primitive Shape Helpers

### `drawCircle(float cx, float cy)`
- **What it does:** Draws a filled circle centered at `(cx, cy)`.
- **Algorithm/strategy:** Approximates a circle with a 100-vertex polygon using `cos/sin`.
- **How it works:** Generic circular shape helper.

### `drawEllipse(float cx, float cy, float rx, float ry)`
- **What it does:** Draws a filled ellipse.
- **Algorithm/strategy:** 100-point polygon, x/y radii scaled separately.
- **How it works:** Used for stones, clouds, jet cockpit, etc.

### `drawhalfCircle(float cx, float cy, float r)`
- **What it does:** Draws a half-circle (upper arc).
- **Algorithm/strategy:** Iterates half-angle range and emits polygon points.
- **How it works:** Used in parachute canopy design.

### `fillCircle(float cx, float cy, float r)`
- **What it does:** Draws a filled circle with configurable radius.
- **Algorithm/strategy:** Same trig polygon approach as `drawCircle`, but parameterized by `r`.
- **How it works:** Core primitive used by moon spots, firefly glow, tree leaves, explosions, etc.

### `drawCircleR(float cx, float cy, float r)`
- **What it does:** Draws a circle variant with explicit radius argument.
- **Algorithm/strategy:** Polygon approximation with trigonometric points.
- **How it works:** Additional reusable circle helper.

### `fillRect(float x, float y, float w, float h)`
- **What it does:** Draws a filled axis-aligned rectangle.
- **Algorithm/strategy:** Emits 4 vertices as a quad.
- **How it works:** Used for trunks, windows, and structural blocks.

### `drawLine(float x1, float y1, float x2, float y2, float width)`
- **What it does:** Draws a single line segment with custom width.
- **Algorithm/strategy:** Sets `glLineWidth(width)` then emits two line vertices.
- **How it works:** Reused for grasses, ripples, legs/arms, fence rails, etc.

---

## 3) Sky, Sun, Moon, Stars

### `drawSky()`
- **What it does:** Draws full sky rectangle background.
- **Algorithm/strategy:** Fills a quad covering the top scene area; color depends on `dayMode`.
- **How it works:** First backdrop layer in each frame.

### `drawHalfMoon()`
- **What it does:** Draws a moving crescent moon (legacy/alternate moon).
- **Algorithm/strategy:**  
  - Horizontal motion from `moonStartX` to `moonEndX`, then reset.  
  - Draw base moon circle, then overlay dark circle for crescent cut.
- **How it works:** Night-only moon variant.

### `drawFullMoon(float cx, float cy, float r)`
- **What it does:** Draws animated full moon with halos and crater spots.
- **Algorithm/strategy:**  
  - Moves moon along an arc using sinusoidal Y offset and linear X progress (`moonPathX`).  
  - When moon completes path, toggles back to day mode.  
  - Renders two blended halo circles, then main moon disk and spots.
- **How it works:** Primary night celestial animation counterpart to the sun.

### `drawSun(float cx, float cy, float r)`
- **What it does:** Draws animated sun with glow.
- **Algorithm/strategy:**  
  - Moves sun along an arc (`p` progress + sine wave for Y).  
  - On end of path, switches to night mode and resets moon path.  
  - Draws two alpha-blended glow circles plus main sun circle.
- **How it works:** Drives day-to-night transition timing.

### `drawStars(float x1, float y1, float x2, float y2, int count)`
- **What it does:** Draws random stars inside a rectangle (night only).
- **Algorithm/strategy:**  
  - Uses fixed random seed (`srand(42)`) so stars are stable frame-to-frame.  
  - Uniform random points in rectangular bounds.
- **How it works:** Adds static starfield without flicker/jitter.

---

## 4) Terrain, River, and Waves

### `drawVillageLand()`
- **What it does:** Draws right-side village ground and its local vegetation.
- **Algorithm/strategy:**  
  - Draws triangular ground patch.  
  - Places grass tufts and bush clusters manually at tuned coordinates.
- **How it works:** Builds habitable land mass beside river.

### `drawVillageRoads()`
- **What it does:** Placeholder for road drawing.
- **Algorithm/strategy:** Currently empty.
- **How it works:** Reserved extension point.

### `drawRiver()`
- **What it does:** Draws river polygon crossing the scene.
- **Algorithm/strategy:** Single 5-vertex polygon describing river shape.
- **How it works:** Main water body between land and forest.

### `drawWaves()`
- **What it does:** Draws animated V-shaped ripples aligned with river flow.
- **Algorithm/strategy:**  
  - Uses time phase (`fireflyTwinklePhase`) for motion.  
  - Computes river flow tangent/normal vectors.  
  - Creates 4 ripple bands at equal spacing (`bandGap`).  
  - For each band and x-step, computes shifted center and draws two segments as a V.
- **How it works:** Produces dynamic surface texture parallel to river direction.

---

## 5) Fence System

### `fenceBase(float x, bool upperBank)`
- **What it does:** Returns river-bank baseline y-value for given `x`.
- **Algorithm/strategy:** Uses one of two linear equations (upper or lower bank).
- **How it works:** Core geometry helper to keep fence posts attached to bank slope.

### `drawFenceLine(...)`
- **What it does:** Draws one bank’s fence posts and rails.
- **Algorithm/strategy:**  
  - Iterates along x with fixed spacing.  
  - Skips center gap (`gapLeft`, `gapRight`) as a passage.  
  - For each post: draws vertical stem + cap + two rails.
- **How it works:** Reusable procedural fence strip on sloped terrain.

### `drawFence()`
- **What it does:** Draws both fence lines (lower and upper bank).
- **Algorithm/strategy:** Calls `drawFenceLine` twice with tuned constants.
- **How it works:** Frames the river path visually.

### `drawFench()`
- **What it does:** Compatibility wrapper for typo name.
- **Algorithm/strategy:** Calls `drawFence()`.
- **How it works:** Prevents breakage if old calls use `drawFench`.

---

## 6) Vegetation and Forest

### `drawGrass(float x, float y, float scale)`
- **What it does:** Draws one grass tuft at position/scale.
- **Algorithm/strategy:**  
  - Applies transform.  
  - Draws multiple narrow blade lines in two green shades.
- **How it works:** Fundamental small vegetation element.

### `drawGrass()`
- **What it does:** Draws a predefined small patch of grass.
- **Algorithm/strategy:** Calls scaled/positioned `drawGrass(x,y,scale)` multiple times.
- **How it works:** Convenience grouped grass.

### `drawBush(float x, float y, float scale)`
- **What it does:** Draws one bush cluster.
- **Algorithm/strategy:** Composes overlapping circles in layered green tones.
- **How it works:** Used on village land for denser low vegetation.

### `drawTreeShadow(float x, float y, float sx, float sy)`
- **What it does:** Draws one tree shadow blob.
- **Algorithm/strategy:** Scaled dark circle under tree location.
- **How it works:** Adds grounding/depth under trees.

### `drawTreeShadow()`
- **What it does:** Draws multiple preset shadows.
- **Algorithm/strategy:** Calls overloaded shadow helper with tuned positions.
- **How it works:** Scene-level shadow decoration.

### `drawTreeTrunk(float x, float y, float scale)`
- **What it does:** Draws a standalone trunk.
- **Algorithm/strategy:** Rectangular trunk + shadow strip using `fillRect`.
- **How it works:** Reusable trunk primitive.

### `drawTreeTrunk()`
- **What it does:** Draws one trunk at a default location.
- **Algorithm/strategy:** Wrapper calling parametric version.
- **How it works:** Quick placement helper.

### `drawTree(float x, float y, float scale)`
- **What it does:** Draws a full stylized tree with animated canopy.
- **Algorithm/strategy (clear step-by-step):**
  - **Step 1: Local transform setup**  
    The function starts with `glPushMatrix()`, then `glTranslatef(x, y, 0)` and `glScalef(scale, scale, 1)`.  
    This means the tree is authored once in local coordinates, and can be reused anywhere with any size.
  - **Step 2: Trunk construction**  
    It draws the trunk with two filled rectangles: one base brown rectangle and one darker side strip.  
    Strategy: fake volume by adding a shadow strip on one side instead of using real lighting.
  - **Step 3: Bark texture detail**  
    A few vertical lines are drawn on the trunk to simulate bark grooves.  
    Strategy: very low-cost detail lines that visually break the flat surface.
  - **Step 4: Root bumps at base**  
    Small circles near the bottom edges create root-like bulges.  
    Strategy: simple geometric accents that make trunk-ground contact feel natural.
  - **Step 5: Canopy sway transform**  
    Before leaves are drawn, the code pushes another matrix and applies `glTranslatef(treeLeafOffsetX, 0, 0)`.  
    Strategy: animate only the leaves, not the trunk, to mimic wind sway.
  - **Step 6: Canopy volume layers**  
    Multiple overlapping circles form the leaf mass (center, left, right, top).  
    Strategy: circle overlap creates a clustered organic silhouette instead of a single round blob.
  - **Step 7: Light/shadow color passes**  
    The canopy is drawn in darker green first, then lighter highlight patches, then extra darker blobs.  
    Strategy: manual color layering gives depth and direction without dynamic lights.
  - **Step 8: Matrix cleanup**  
    It pops the canopy matrix, then pops the full tree matrix to restore global transform state.
- **How it works in scene animation:**  
  `updateFighterJet()` changes `treeLeafOffsetX` over time (left-right oscillation).  
  Because only canopy drawing is inside that leaf-offset transform, the trunk remains fixed while leaves sway.
- **Why this design is effective:**  
  - Reusable: one function draws many trees with different `(x, y, scale)`.  
  - Cheap: only primitive shapes, no heavy geometry.  
  - Expressive: layered colors + selective animation make trees look alive with minimal code.

### `forest()`
- **What it does:** Draws full forest region and details.
- **Algorithm/strategy:**  
  - Draws forest triangle background.  
  - Adds many grass tufts (including irregular scatter).  
  - Places layered trees across slope.  
  - Draws fireflies (dense + sparse pass).
- **How it works:** Constructs the complete left/upper forest ecosystem.

### `drawFirefliesInForestTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int count)`
- **What it does:** Draws animated glowing fireflies inside a triangle.
- **Algorithm/strategy:**  
  - Uses barycentric random sampling for uniform point distribution in triangle.  
  - Adds sinusoidal pulse and small drift per point.  
  - Two-pass render: translucent aura circles + bright point cores.
- **How it works:** Night-only ambient motion/atmosphere in forest.

---

## 7) Clouds, Hills, and House

### `drawClouds()`
- **What it does:** Draws cloud groups in sky.
- **Algorithm/strategy:** Composes each cloud from overlapping circles/ellipses with soft highlights.
- **How it works:** Adds day-sky depth and softness.

### `drawHill(float x, float y, float sx, float sy, float r, float g, float b)`
- **What it does:** Draws one hill blob with configurable size/color.
- **Algorithm/strategy:** Uses transformed circular/elliptic geometry.
- **How it works:** Reusable terrain macro-shape.

### `drawHills()`
- **What it does:** Draws multiple layered hills.
- **Algorithm/strategy:** Calls `drawHill` with different scales, positions, and shades.
- **How it works:** Creates depth via front/mid/back hill layering.

### `drawHouse()`
- **What it does:** Draws detailed house structure and window lighting behavior.
- **Algorithm/strategy:**  
  - Uses polygons/quads for body, roof, details.  
  - Window color and light beam change in night mode.
- **How it works:** Main architectural focal object on village side.

---

## 8) Boats, Birds, Stones, and Parachute

### `drawboat()`
- **What it does:** Draws boat body and mast assembly.
- **Algorithm/strategy:** Polygon hull + line outlines + mast/parts.
- **How it works:** Base drawable for moving boat transformation.

### `drawBirds()`
- **What it does:** Draws flock of birds with animated wing angle and positional offsets.
- **Algorithm/strategy:**  
  - Defines local lambda to draw V-shaped wings from angle.  
  - Applies flock translation (`birdXOffset`, `birdYOffset`).  
  - Uses shared animated wing opening angle.
- **How it works:** Day-time flock animation over sky/river region.

### `drawStones()`
- **What it does:** Draws multiple foreground stones.
- **Algorithm/strategy:** Ellipses + circles in gray palettes for base/highlight.
- **How it works:** Adds ground detail near village/river.

### `drawParachute(float x, float y, float scale)`
- **What it does:** Draws parachute canopy + hanging body + ropes.
- **Algorithm/strategy:** Transformed local composition of half-circle, ellipse, rectangle, and lines.
- **How it works:** Triggered after jet collision event.

---

## 9) Fighter Jets and Explosion

### `drawFighterJet(float x, float y, float scale, bool faceLeft)`
- **What it does:** Draws one fighter jet with orientation support.
- **Algorithm/strategy:**  
  - Local transform at `(x,y)`.  
  - Horizontal flip via negative scale when needed.  
  - Multi-part polygon composition (body, wings, tail, cockpit).
- **How it works:** Two jets fly opposite directions and can collide.

### `drawExplosion(float x, float y, float r)`
- **What it does:** Draws layered explosion blast.
- **Algorithm/strategy:** Overlapping circles in yellow/orange shades with varying radii/offsets.
- **How it works:** Rendered during collision event progression.

---

## 10) Input and Animation Controllers

### `rotateboat(unsigned char key, int x, int y)`
- **What it does:** Handles normal keyboard controls.
- **Algorithm/strategy:**  
  - `l/r` rotate boat angle.  
  - `o` toggles boat direction/reverse state.  
  - `m` toggles day/night manually.  
  - number keys also control stone view angles (if enabled in your current version).
- **How it works:** Updates global state and requests redraw.

### `controlJetY(int key, int x, int y)`
- **What it does:** Handles special key input for jet vertical control.
- **Algorithm/strategy:** Up/Down arrows move jet Y within clamped bounds.
- **How it works:** Lets user influence jet altitude.

### `updateFighterJet(int value)`
- **What it does:** Master per-frame update routine (timer callback).
- **Algorithm/strategy:** Updates all animated state:
  - firefly phase
  - bird motion path + wing flapping
  - tree sway
  - boat forward/reverse interpolation
  - jet movement and collision detection
  - explosion radius growth/lifecycle
  - parachute descent + drift
  - schedules next timer tick with `glutTimerFunc(30, ...)`
- **How it works:** Central simulation loop driving scene animation.

---

## 11) Scene Composition and Program Lifecycle

### `display(void)`
- **What it does:** Draws one complete frame.
- **Algorithm/strategy:** Ordered painter-style rendering:
  - clear/background
  - sky objects (stars/moon/sun/clouds)
  - land/river/fence/waves
  - dynamic actors (boat, birds, jets, parachute, explosion)
  - forest, house, details, fireflies
- **How it works:** Called by GLUT whenever frame redraw is needed.

### `init(void)`
- **What it does:** Initializes rendering projection setup.
- **Algorithm/strategy:** Sets clear color, chooses projection matrix, and applies orthographic view bounds.
- **How it works:** Prepares 2D scene coordinate system (`glOrtho` world).

### `main(int argc, char **argv)`
- **What it does:** Program entry point and GLUT wiring.
- **Algorithm/strategy:**  
  - Initializes GLUT/window.  
  - Calls `init()`.  
  - Registers callbacks (`display`, keyboard, special keys, timer).  
  - Starts event loop (`glutMainLoop`).
- **How it works:** Hands control to GLUT runtime.

---

## 12) Forward Declarations at Top

The top-of-file lines:
- `void fillCircle(float cx, float cy, float r);`
- `void drawGrass(float x, float y, float scale);`
- `void drawBush(float x, float y, float scale);`

are **function declarations** (not separate implementations). They allow earlier code to call these functions before their full definitions appear later in the file.
