/**
 * main.c
 * Main file
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "drawing.h"
#include "command.h"

int main(void) {

	Page *page;
	page = (Page*)malloc(sizeof(Page));
	Error err = NO_ERROR;
	Command *root = createElement("root", 0, 0, 0, 0);
	Command *element;

	int i = 0, j = 0, selection = 0, flag = 0, newflag = 0, param1 = 0, param2 = 0, param3 = 0, param4 = 0;
	char fullinput[16], command[7];

	/* Declare all of the possible commands in the dictionary */
	char *valid_commands[11] = {      
		"new",
		"r",
		"clear",
		"invert",
		"line",
		"rect",
		"circle",
		"fill",
		"list",
		"delete",
		"exit"
	};

	printf("Welcome to the drawing software\n");

	do {
		do {
			printf("> ");
			scanf("%s", command);

			/* If statements used to see how many command parameters are needed for evert specific command, then store those parameters */
			if (strcmp(command, valid_commands[9]) == 0)
				scanf("%d", &param1);
			if ((strcmp(command, valid_commands[0]) == 0) || (strcmp(command, valid_commands[7]) == 0))
				scanf("%d %d", &param1, &param2);
			if (strcmp(command, valid_commands[6]) == 0)
				scanf("%d %d %d", &param1, &param2, &param3);
			if ((strcmp(command, valid_commands[4]) == 0) || (strcmp(command, valid_commands[5]) == 0))
				scanf("%d %d %d %d", &param1, &param2, &param3, &param4);

			/* Checks to see if the command entered is a valid command */
			for (i = 0; i <= 10; i++) {
				if (strcmp(command, valid_commands[i]) == 0) {
					selection = i;                              /* Used for the switch case */
					flag = 1;
					break;
				}
			}

			if (flag == 0){
				printf("%s is an illegal command. Please ensure one of the eleven legal commands has been entered\n", command);
				/* Effectively clears the end of the scanf input buffer by directly going to the end of it */
				fseek(stdin, 0, SEEK_END);
			}
		} while (flag == 0);        /* Only progresses the program if a valid command has been entered */

		switch (selection) {
			case 0:
				/* Checks to ensure that the new command has only been entered once in the program run */
				if (newflag == 1) {
					printf("'New' cannot be executed more than once, please enter another command\n");
					break;
				}
				newflag = 1;
				new(page, param1, param2);
				break;
			case 1:
				r(page);
				break;
			case 2:
				clear(page);
				clearLinkedList(page, root);
				break;
			case 3:
				invert(page);
				break;
			case 4:
				err = drawLine(page, param1, param2, param3, param4, 0);
				if (err != NO_ERROR) {
					printError("line", err);
				} else {
					element = createElement(command, param1, param2, param3, param4);
					pushElement(root, element);
				}
				break;
			case 5:
				err = drawRect(page, param1, param2, param3, param4, 0);
				if (err != NO_ERROR) {
					printError("rectangle", err);
				} else {
					element = createElement(command, param1, param2, param3, param4);
					pushElement(root, element);
				}
				break;
			case 6:
				err = drawCircle(page, param1, param2, param3, 0);
				if (err != NO_ERROR) {
					printError("circle", err);
				} else {
					element = createElement(command, param1, param2, param3, 0);
					pushElement(root, element);
				}
				break;
			case 7:
				fill(page, param1, param2);
				break;
			case 8:
				printlist(root);
				break;
			case 9:
				deleteElement(page, root, param1);
				break;
			case 10:
				printf("Quitting Program\r\n");
				break;
		}

		flag = 0;

	} while (selection != 10);

	/* Frees all of the elements in the linked list for the previous commands */
	deallocateLinkedList(root);

	return 0;
}