/*
 * INF1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include "chat1002.h"


 /*
  * Get the name of the chatbot.
  *
  * Returns: the name of the chatbot as a null-terminated string
  */
const char* chatbot_botname() {

	return "Chatbot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char* chatbot_username() {

	return "User";

}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char* inv[], char* response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char* intent) {

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char* inv[], char* response, int n) {

	snprintf(response, n, "Goodbye!");

	return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char* intent) {

	return (compare_token(intent, "load") == 0);

}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char* inv[], char* response, int n) {

	/* to be implemented */

	FILE* f = fopen(inv[1], "r");
	int lineCounter = 0;

	//Checking if file is empty
	if (f == NULL) {
		snprintf(response, n, "%s not found. You sure correct file anot?");
	}
	else {
		lineCounter = knowledge_read(f); //read the num of lines in the file
		if (lineCounter == KB_NOMEM) { //check if there is enough memory
			snprintf(response, n, "Not enough memory you dumbass");
		}
		else if (lineCounter == -1) //check if file have invalid input
		{
			snprintf(response, n, "Invalid file you dumbass");
		}
		else { //all inputs in file are valid
			snprintf(response, n, "Read %d response from %s already you dumbass", lineCounter, inv[1]);
		}
	}
	return 0;

}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char* intent) {

	//returns 1 if intent is what, where or who
	return (compare_token(intent, "what") == 0 ||
		compare_token(intent, "where") == 0 ||
		compare_token(intent, "who") == 0);

}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char* inv[], char* response, int n) {

	//int result = 100; //? delete
	//int index = 0; //delete
	char* entity = (char*)malloc(sizeof(char)*MAX_ENTITY);
	//entity = "";
	strcpy(entity, "");
	//knowledgeNode* nodePtr = doesEntityExist(inv, entity);
	//char* intent = inv[0];

	//set the list the program will iterate through depending on the intent
	if (compare_token(inv[0], "what") == 0) {
		listIter = whatHead;
	}
	else if (compare_token(inv[0], "who") == 0) {
		listIter = whoHead;
	}
	else if (compare_token(inv[0], "where") == 0) {
		listIter = whereHead;
	}
	else {
		//if intent is not properly set, reply invalid question (might not ever enter here)
		snprintf(response, n, "Invalid question!");
		return 0;
	}

	// Derive entity and question from inv[] here.
	int i;
	//if 2nd word is "is" or "are" skip it
	if (compare_token(inv[1], "is") == 0 ||
		compare_token(inv[1], "are") == 0) {
		i = 2;
	}
	else {
		i = 1;
	}
	// looping throgh the word ptr array until its reaches the end
	for (; inv[i] != '\0' || inv[i] != NULL; ++i) {
		//concatenate the rest of the string into local entity var to be passed to check
		strcat(entity, inv[i]);
	}
	strcat(entity, "\0");

	//checking and getting pointer consisting of entity
	knowledgeNode* nodeptr = doesEntityExist(listIter, entity);

	//if entity does not exist
	if (nodeptr == NULL)	{
		//print message and return 
		snprintf(response, n, "I don't know the answer to this question.");
		//return 0;
	}
	else { //if entity exists
		snprintf(response, n, "%s", nodeptr->response);
	}


	////if number of words in the qn is less than 3, return invalid?
	//if (inc < 3){
	//	snprintf(response, n, "Invalid question!");
	//	return 0;
	//}

	//delete?
	//if (nodePtr == NULL)
	//{
	//	return KB_NOMEM;
	//}

	// Create pointer to point to question. Must be not NULL.
	char* question = (char*)malloc(sizeof(char)* MAX_INPUT);
	if (question == NULL)
	{
		return KB_NOMEM;
	}
	strcpy(question, "");

	//int i = 0;
	//while (1)
	//{
	//	// Check for the end of entity which will be NULL.
	//	if (i > 1)
	//	{
	//		if (inv[i] == NULL)
	//		{
	//			nodePtr->entity[strlen(entity) - 1] = '\0';
	//			question[strlen(question) - 1] = '\0';
	//			break;
	//		}

	//		// If current inv[i] is not NULL, add it to entity.
	//		strcat(entity, inv[i]);
	//		strcat(entity, " ");
	//	}
	//	strcat(question, inv[i]);
	//	strcat(question, " ");
	//	i++;
	//}
	/*
	// Check if intent and entity is inside knowledgeNode.
	int status = knowledge_get(intent, entity, response, n);

	// If entity is not found, ask user to input response for new entity.
	if (status == KB_NOTFOUND)
	{
		char user_input[MAX_INPUT];
		strcat(question, "?");
		prompt_user(user_input, MAX_INPUT, question);

		// Display :-( if user input is empty.
		if (compare_token(user_input, "") == 0)
		{
			snprintf(response, n, "Please enter a question with an entity.");
		}
		else
		{
			// Put new question into knowledge of chatbot.Î
			status = knowledge_put(intent, entity, user_input);
			if (status == KB_OK)
			{
				snprintf(response, n, "Thank You.");
			}
			else
			{
				snprintf(response, n, "Something went wrong!");
			}
		}
	}
	// Entity is found, display the respective response.
	else if (status == KB_OK)
	{
		snprintf(response, n, "%s", response);
	}
	else if (status == KB_NOMEM)
		snprintf(response, n, "No memory currently!");
	else
		snprintf(response, n, "Something when wrong!");

	*/
	free(entity);
	free(question);
	return 0;
	
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char* intent) {

	return compare_token(intent, "reset") == 0;

}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char* inv[], char* response, int n) {

	/* to be implemented */

	knowledge_reset();

	snprintf(response, n, "Chatbot reset.");

	return 0;

}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char* intent) {

	return (compare_token(intent, "save") == 0);

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char* inv[], char* response, int n) {

	/* to be implemented */

	// first word,  indicates the intent. If the intent is not recognised, the chatbot should respond with "I do not understand [intent]." or similar, and ignore the rest of the input.
  // second word, for SAVE, it may be "as" or "to"
  // The word is otherwise ignored and may be omitted.
  // stored in the output buffer, and be no longer than n characters long (you can use snprintf() to do this). The contents of this buffer will be printed by the main loop.

	FILE* data;
	char file_name[MAX_INPUT];

	//get user input to open file in c
	//char s[MAX_INPUT];
	printf("Enter a filename:\n");
	fgets(file_name, MAX_INPUT, stdin);
	data = fopen(file_name, 'a');
	if (data == NULL)
		printf("I do not understand");
	else
		return 0;

	//Check if file exist, if not, create it.
	data = fopen(file_name, 'b');
	if (data != NULL) {
		//file exists and write into file 
		data = fopen(file_name, "f");
		knowledge_write(data);
		fclose(data);
		snprintf("Results saved from the knowledge base to [%s].\n", MAX_INPUT, file_name);
		// close file when you're done
		fclose(data);
	}
	else {
		//file doesn't exists or cannot be opened
		snprintf("File does not exist, can't open file [%s].\n", MAX_INPUT, file_name);
	}

	return 0;

}



