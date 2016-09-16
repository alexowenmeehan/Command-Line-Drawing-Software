/**
 * command.c
 * Functions for creating, pushing, deleting, printing and freeing linked list elements
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
#include "drawing.h"
#include "command.h"

/** deallocateElement
 * Frees all information in the element removed from the linked list
 *
 * @param Command *element	The element that has been recently removed from the linked list
 *
 */
void deallocateElement(Command *element) {

	free(element->command);
	free(element);
}

/** deallocateLinkedList
 * Free all the nodes of the linked list, including their data
 *
 * @param Command* root	The root node of the Linked List
 */
void deallocateLinkedList(Command *root) {

	int i = 0;
	Command * temp;

	while (root != NULL) {

		/* Sets the current node pointer to a temporary node and then frees the allocated memory through the temporary pointer */
		temp = root;
		root = root->next;

		/* Frees all allocated data in the temporary element, then frees the element */
		free(temp->command);
		free(temp);
	}
}

/** createElement
 * Creates an element (or node) for the most recently entered command
 *
 * @param command	The actual command entered by the user
 * @param param1		The first parameter entered with the command by the user
 * @param param2		The second parameter entered with the command by the user
 * @param param3		The third parameter entered with the command by the user
 * @param param4		The fourth parameter entered with the command by the user (only used by line and rect)
 *
 */
Command* createElement(char *command, int param1, int param2, int param3, int param4) {

	Command *pcommand;

	pcommand = (Command*)malloc(sizeof(Command));

	if (pcommand) {

		/* Sets the ID of the root node as 0 and uses root in the command character array in order to identify it as the root node*/
		if (strcmp(command, "root") == 0){
			pcommand->command = (char*)malloc((strlen("root") + 1) * sizeof(char));
			pcommand->ID = 0;
		}

		/* Identifies if the instruction being input is a circle command and if so, saves the relevant arguments for the command */
		if (strcmp(command, "circle") == 0) {
			pcommand->command = (char*)malloc((strlen(command) + 1) * sizeof(char));
			strcpy(pcommand->command, command);
			pcommand->param1 = param1;													/* Param1 is x co-ord for circle centre */
			pcommand->param2 = param2;													/* Param2 is y co-ord for circle centre */
			pcommand->param3 = param3;													/* Param3 is radius for circle */
			pcommand->param4 = 0;														/* Param4 is ignored and not used by the program */
		}

		/* Identifies if the instruction being input is a line or rectange command and if so, saves the relevant arguments for the command */
		if ((strcmp(command, "line") == 0) || (strcmp(command, "rect") == 0)) {
			pcommand->command = (char*)malloc((strlen(command) + 1) * sizeof(char));
			strcpy(pcommand->command, command);
			pcommand->param1 = param1;													/* Param1 is x1 co-ord, start of line */
			pcommand->param2 = param2;													/* Param2 is y1 co-ord, start of line */
			pcommand->param3 = param3;													/* Param3 is x2 co-ord, end of line */
			pcommand->param4 = param4;													/* Param4 is y2 co-ord, end of line */
		}
		pcommand->next = 0;	
	}

	return pcommand;
}

/** pushElement
 * Pushes the newly made elememt onto the end of the linked list
 *
 * @param Command *root		The first element in the linked list
 * @param Command *element	The new element to be added to the end of the linked list
 */
void pushElement(Command *root, Command *element) { 

	int i = 0;

	while (root != NULL) {
		i++;
		if (root->next != 0) {
			root = root->next;
		} else {
			root->next = element;
			element->ID = i;
			return;
		}
	}
}

/** deleteElement
 * Deletes an element in the linked list specified by the user
 *
 * @param Page *page					The page containing the canvas
 * @param Command *root		The first element in the linked list
 * @param int ID						The ID assigned to the element in the list that the user wishes to delete
 */
int deleteElement(Page *page, Command * root, int ID) {

	Command *prev_element;
	Command *temp = root;
	int id = 0;

	/* Goes through the linked list until it finds the element that needs to be deleted */
	for (prev_element = root; (prev_element->ID != ID) && (prev_element->next != NULL); prev_element = prev_element->next)
		temp = prev_element;

	/* When the element to be deleted is found, the next pointer in the element before the deleted element is given the value of the element after the deleted element and the deleted element is cut out of the list */
	if (prev_element->ID == ID) {
		temp->next = prev_element->next;
		id = temp->ID;

		while (temp->next != NULL) {
			temp = temp->next;
			temp->ID = ++id;
		}

		undraw(page, prev_element->command, prev_element->param1, prev_element->param2, prev_element->param3, prev_element->param4);
		deallocateElement(prev_element);

		return 1;
	} else {
		printf("Chosen command doesn't exist, please enter the number of a listed command\r\n");
		return 0;
	}
}

/** clearLinkedlist
 * Clears the linked list, removing all elements apart from the root node
 *
 * @param Page *page		The page containing the canvas
 * @param Command *root		The first element in the linked list
 */
void clearLinkedList(Page *page, Command *root) {
	int length = 0, i = 0;
	Command *next = root;
	while (next->next != NULL) {
		next = next->next;
		length++;
	}
	for (i = 0; i < length; i++) {
		deleteElement(page, root, 1);
	}
}

/** printlist
 * Prints the linked list, displaying all previously entered commands and their position in the list
 *
 * @param Command * root		The first element in the linked list
 */
void printlist(Command *root) {

	if (root->next == 0) {
		printf("No commands in history.\r\n");
		return;
	}
	
	/* Runs through the list of previously entered commands until there are no more previous command structures */
	while (root != NULL) {

		/* Sets the current node to the next one, if it is not already zero */
		if (root->next != 0) {
			root = root->next;

			/* Prints the number of the command */
			printf(" %d: ", root->ID);

			/* Prints the name of the command and the parameters associated with it */
			if (strcmp(root->command, "circle") == 0)
				printf("Circle, centre (%d, %d) and radius %d", root->param1, root->param2, root->param3);

			if(strcmp(root->command, "line") == 0) 
				printf("Line from (%d, %d) to (%d, %d)", root->param1, root->param2, root->param3, root->param4);

			if (strcmp(root->command, "rect") == 0)
				printf("Rectangle from (%d, %d) to (%d, %d)", root->param1, root->param2, root->param3, root->param4);
			
			printf("\r\n");
		}
		else {
			return;
		}
	}
}