/**
* drawing.h
* Drawing functions header file
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
*/

/* Prevent possibly including drawing functions multiple times */
#ifndef DRAWING
#define DRAWING

/** Page
 * Page structure that holds the canvas and its boundaries
 */
typedef struct page {
	char **canvas;
	int x, y;
} Page;

typedef enum Error {
	NO_ERROR,
	MAX_HEIGHT,
	MAX_WIDTH,
	MIN_HEIGHT,
	MIN_WIDTH
} Error;

Error drawLine(Page *page, int x1, int y1, int x2, int y2, int delete);
Error drawRect(Page *page, int x1, int y1, int x2, int y2, int delete);
Error drawCircle(Page *page, int x, int y, int r, int delete);
void fill(Page *page, int x, int y);
void invert(Page *page);
void clear(Page *page);
void r(Page *page);
void printError(char* polygon, Error err);
void new(Page *page, int x, int y);

#endif