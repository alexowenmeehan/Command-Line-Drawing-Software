/**
* command.h
* Command history functions header file
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

/* Prevent possibly including command functions multiple times */
#ifndef COMMAND
#define COMMAND

typedef struct Command {
	char * command;
	int param1, param2, param3, param4, ID;
	struct Command * next;
} Command;

void deallocateElement(Command *element);
void deallocateLinkedList(Command *root);
Command* createElement(char *command, int param1, int param2, int param3, int param4);
void pushElement(Command * root, Command * element);
int deleteElement(Page *page, Command * root, int ID);
void clearLinkedList(Page *page, Command *root);
void printlist(Command * root);

#endif