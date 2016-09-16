/**
 * drawing.c
 * Drawing functions
 *
 * @author Dan Foad, Alexander Owen-Meehan
 * @version 0.1.0
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 *
 * IMPORTANT NOTE: Plotting a point to the canvas is [y][x], NOT [x][y]
 *
 */

#include <math.h>
#include <stdio.h>
#include "drawing.h"

/** drawLine
 * Draw a line between two points onto the canvas
 *
 *****************************************************************
 ** Based on "Algorithm for computer control of a digital plotter"
 ** Bresenham, J. E. (1965)
 ** IBM Systems Journal, v. 4, p. 25-30
 *****************************************************************
 *
 * @param Page *page	The Page struct that holds the canvas
 * @param int x1		The bottom-left x-coordinate of the line
 * @param int y1		The bottom-left y-coordinate of the line
 * @param int x2		The top-right x-coordinate of the line
 * @param int y2		The top-right y-coordinate of the line
 * @param int delete	Whether the shape is being deleted or drawn
 */
Error drawLine(Page *page, int x1, int y1, int x2, int y2, int delete) {
	int dx, dy;
	float i, j;
	dx = x2 - x1;
	dy = y2 - y1;
	i = 0.0f;
	j = 0.0f;
	char draw = '*';
	if (delete) {
		draw = '.';
	}

	if (x1 >= page->x || x2 >= page->x) return MAX_WIDTH;
	if (y1 >= page->y || y2 >= page->y) return MAX_HEIGHT;
	if (x1 < 0 || x2 < 0) return MIN_WIDTH;
	if (y1 < 0 || y2 < 0) return MIN_HEIGHT;

	if (abs(dx) > abs(dy)) {
		if (dx > 0) {
			for (i = 0.0f; i <= dx; i++) {
				j = ((i * dy) / dx) + 0.5f;
				page->canvas[(int)(y1 + j)][(int)(x1 + i)] = draw;
			}
		} else {
			for (i = 0.0f; i >= dx; i--) {
				j = ((i * dy) / dx) + 0.5f;
				page->canvas[(int)(y1 + j)][(int)(x1 + i)] = draw;
			}
		}
	} else {
		if (dy > 0) {
			for (j = 0.0f; j <= dy; j++) {
				i = ((j * dx) / dy) + 0.5f;
				page->canvas[(int)(y1 + j)][(int)(x1 + i)] = draw;
			}
		} else {
			for (j = 0.0f; j >= dy; j--) {
				i = ((j * dx) / dy) + 0.5f;
				page->canvas[(int)(y1 + j)][(int)(x1 + i)] = draw;
			}
		}
	}

	return NO_ERROR;
}

/** drawRect
 * Draw a rectangle (unfilled) between two points onto the canvas
 *
 * @param Page *page	The Page struct that holds the canvas
 * @param int x1		The bottom-left x-coordinate of the rectangle
 * @param int y1		The bottom-left y-coordinate of the rectangle
 * @param int x2		The top-right x-coordinate of the rectangle
 * @param int y2		The top-right y-coordinate of the rectangle
 * @param int delete	Whether the shape is being deleted or drawn
 */
Error drawRect(Page *page, int x1, int y1, int x2, int y2, int delete) {
	if (drawLine(page, x1, y1, x1, y2, delete) != NO_ERROR) return drawLine(page, x1, y1, x1, y2, delete);
	if (drawLine(page, x1, y1, x2, y1, delete) != NO_ERROR) return drawLine(page, x1, y1, x2, y1, delete);
	if (drawLine(page, x1, y2, x2, y2, delete) != NO_ERROR) return drawLine(page, x1, y2, x2, y2, delete);
	if (drawLine(page, x2, y1, x2, y2, delete) != NO_ERROR) return drawLine(page, x2, y1, x2, y2, delete);
	return NO_ERROR;
}

/** drawCircle
 * Draw a circle (unfilled) of certain radius onto the canvas
 *
 *********************************************************************************
 ** Based on "Algorithm for drawing ellipses or hyperbolae with a digital plotter"
 ** Pitteway, M. L. V. (1967)
 ** Computer J., v. 10(3), p. 282-289
 *********************************************************************************
 *
 * @param Page *page	The Page struct that holds the canvas
 * @param int x1		The x-coordinate of the circle origin
 * @param int y1		The y-coordinate of the circle origin
 * @param int r		The radius of the circle
 * @param int delete	Whether the shape is being deleted or drawn
 */
Error drawCircle(Page *page, int x, int y, int r, int delete) {
	float theta = 0.0f;
	int deg = 0, i = 0, j = 0;
	const double PI = 3.14159265358979323846;
	char draw = '*';
	if (delete) {
		draw = '.';
	}

	/* First run to check if safe to draw */
	for (deg = 0; deg <= 360; deg++) {
		theta = (deg * PI) / 180.0f;
		i = x + r * cos(theta) + 0.5;
		j = y + r * sin(theta) + 0.5;

		if (i >= page->x) return MAX_WIDTH;
		if (j >= page->y) return MAX_HEIGHT;
		if (i < 0) return MIN_WIDTH;
		if (j < 0) return MIN_HEIGHT;
	}

	/* Only draw once all points confirmed safe */
	for (deg = 0; deg <= 360; deg++) {
		theta = (deg * PI) / 180.0f;
		i = x + r * cos(theta) + 0.5;
		j = y + r * sin(theta) + 0.5;

		page->canvas[j][i] = draw;
	}

	return NO_ERROR;
}

/** fill
 * Fill a region assuming 4-connected neighborhood recursively
 *
 ****************************************
 ** Based on "Digital Picture Processing"
 ** Rosenfeld, A., Kak, A. C. (1982)
 ** Academic Press, Inc.
 ****************************************
 *
 * @param Page *page	The Page struct that holds the canvas
 * @param int x1		The x-coordinate of the seed point
 * @param int y1		The y-coordinate of the seed point
 */
void fill(Page *page, int x, int y) {
	if (page->canvas[y][x] == '.') {
		page->canvas[y][x] = '*';

		if (x - 1 >= 0)			fill(page, x - 1, y);
		if (x < page->x - 1)	fill(page, x + 1, y);
		if (y - 1 >= 0)			fill(page, x, y - 1);
		if (y < page->y - 1)	fill(page, x, y + 1);
	}
}

/** invert
 * Invert the canvas, replacing . with * and vice-versa
 *
 * @param Page *page	The Page struct that holds the canvas
 */
void invert(Page *page) {
	int i, j;
	i = 0;
	j = 0;
	for (i = 0; i < page->y; i++) {
		for (j = 0; j < page->x; j++) {
			if (page->canvas[i][j] == '.') {
				page->canvas[i][j] = '*';
			} else {
				page->canvas[i][j] = '.';
			}
		}
	}
}

/** clear
 * Clear the canvas, replacing all points with .
 *
 * @param Page *page	The Page struct that holds the canvas
 */
void clear(Page *page) {
	int i, j;
	i = 0;
	j = 0;
	for (i = 0; i < page->y; i++) {
		for (j = 0; j < page->x; j++) {
			page->canvas[i][j] = '.';
		}
	}
}

/** r
 * Redraw the canvas to the console
 *
 * @param Page *page	The Page struct that holds the canvas
 */
void r(Page *page) {
	int i, j;
	i = 0;
	j = 0;
	for (i = page->y - 1; i >= 0; i--) {
		for (j = 0; j < page->x; j++) {
			printf("%c ", page->canvas[i][j]);
		}
		printf("\r\n");
	}
}

/** undraw
 * Undraw a given shape to the canvas
 *
 * @param Page *page		The Page struct that holds the canvas
 * @param char *shape		The shape to undraw
 * @param int param1		The first parameter entered with the command by the user
 * @param int param2		The second parameter entered with the command by the user
 * @param int param3		The third parameter entered with the command by the user
 * @param int param4		The fourth parameter entered with the command by the user (only used by line and rect)
 */
void undraw(Page *page, char *shape, int param1, int param2, int param3, int param4) {
	if (strcmp(shape, "line") == 0) {
		drawLine(page, param1, param2, param3, param4, 1);
	}
	if (strcmp(shape, "rect") == 0) {
		drawRect(page, param1, param2, param3, param4, 1);
	}
	if (strcmp(shape, "circle") == 0) {
		drawCircle(page, param1, param2, param3, 1);
	}
}

/** printError
 * Print out the error caused by drawing a polygon
 *
 * @param char *polygon	The shape that failed to draw
 * @param Error err		The nature of the error
 */
void printError(char *polygon, Error err) {
	printf("Error: %s could not be drawn, it exceeds the ", polygon);
	switch (err) {
		case MAX_HEIGHT:
			printf("maximum height");
			break;
		case MAX_WIDTH:
			printf("maximum width");
			break;
		case MIN_HEIGHT:
			printf("minimum height");
			break;
		case MIN_WIDTH:
			printf("minimum width");
			break;
	}
	printf(" of screen.\r\n");
}

/** new
 * Create a new canvas of given size
 *
 * @param Page *page	The Page struct that holds the canvas
 * @param int x		The width of the canvas
 * @param int y		The height of the canvas
 */
void new(Page *page, int x, int y) {
	int i, j;
	i = 0;
	j = 0;

	page->x = x;
	page->y = y;
	page->canvas = (char**)malloc(page->y * sizeof(char*));
	
	for (i = 0; i < page->y; i++) {
		page->canvas[i] = (char*)malloc(page->x * sizeof(char));
		for (j = 0; j < page->x; j++) {
			page->canvas[i][j] = '.';
		}
	}
}
