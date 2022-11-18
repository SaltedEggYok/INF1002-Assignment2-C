/*
 * INF1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

 /*
  * Get the response to a question.
  *
  * Input:
  *   intent   - the question word
  *   entity   - the entity
  *   response - a buffer to receive the response
  *   n        - the maximum number of characters to write to the response buffer
  *
  * Returns:
  *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
  *   KB_NOTFOUND, if no response could be found
  *   KB_INVALID, if 'intent' is not a recognised question word
  */
int knowledge_get(const char* intent, const char* entity, char* response, int n)
{
	//setting ptr to list
	if (compare_token(intent, "what") == 0)
	{
		listIter = whatHead;
	}
	else if (compare_token(intent, "where") == 0)
	{
		listIter = whereHead;
	}
	else if (compare_token(intent, "who") == 0)
	{
		listIter = whoHead;
	}
	else
	{
		return KB_INVALID;
	}


	//searching for node that contains entity
	knowledgeNode* node = doesEntityExist(listIter, entity);

	if (NULL == node) { //if null means no node exists, return not found
		snprintf(response, n, "I don't know the answer to this question.");
		return KB_NOTFOUND;
	}

	//if enters here means node found, print response onto buffer
	snprintf(response, n, node->response);

	//all good
	return KB_OK;
}

/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_OK, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char* intent, const char* entity, const char* response)
{

	//if intent is what, where and who
	if (compare_token(intent, "what") == 0 ||
		compare_token(intent, "where") == 0 ||
		compare_token(intent, "who") == 0) {

		//constructing newNode, if can return KB_OK, KB_INVALID or KB_NOMEM
		return nodeConstructor(intent, entity, response);
	}
	else {
		//should not really enter here 
		return KB_INVALID;
	}

}

/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE* f)
{

	// error handling
	if (NULL == f)
	{
		printf("File was NULL (knowledge.c)\n");
		return -1;
	}

	int pairCount = 0;
	// token - used for strtok, string - used for each line in .ini file
	char* token, * buffer = malloc(MAX_INPUT * sizeof(char));
	char intent[MAX_INTENT], entity[MAX_ENTITY], response[MAX_RESPONSE];
	const char* delimiter = "=";
	const char delim = '=';

	// init intent, entity, response;
	strcpy(intent, "");
	strcpy(entity, "");
	strcpy(response, "");

	// file reading loop
	while (1)
	{
		// endOfFile to be used at the bottom of the while loop to break out if needed
		char* endOfFile = fgets(buffer, MAX_INPUT, f);

		// if the line contains '=', it is an entity-response pair, call knowledge_put();
		if (NULL != strchr(buffer, delim))
		{
			// if intent was not assigned anything, continue, if enters here means file was wrongly formatted
			if ("" == intent)
			{
				continue;
			}

			// 1st pass - entity, left of '='
			token = strtok(buffer, delimiter);
			strcpy(entity, token);

			// 2nd pass - responsed, right of '='
			token = strtok(NULL, delimiter);
			strcpy(response, token);

			//adding to knowledge base
			int result;
			result = knowledge_put(intent, entity, response);
			if (result == KB_NOMEM) {
				return result;
			}
			++pairCount;
		}
		else // if it does not contain '=', it is an intent line, set intent
		{

			if (strstr(buffer, "what")) // check if line is [WHAT], strcasestr returns a ptr, null if not found
			{
				strcpy(intent, "what");
			}
			else if (strstr(buffer, "who")) // check if line is [WHO]
			{
				strcpy(intent, "who");
			}
			else if (strstr(buffer, "where")) // check if line is [WHERE]
			{
				strcpy(intent, "where");
			}
			// blank line / irrelevant line
			// do nothing
		}

		if (NULL == endOfFile)
		{
			break;
		}
	}

	// clear memory
	if (buffer)
	{
		free(buffer);
		buffer = NULL; // for some reason nullptr doesn't work here
	}
	return pairCount;
}

/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset()
{

	if (whatHead)
	{
		listIter = whatHead;
		while (whatHead)
		{
			whatHead = whatHead->next;
			free(listIter);
			listIter = whatHead;
		}
	}

	if (whoHead)
	{
		listIter = whoHead;
		while (whoHead)
		{
			whoHead = whoHead->next;
			free(listIter);
			listIter = whoHead;
		}
	}

	if (whereHead)
	{
		listIter = whereHead;
		while (whereHead)
		{
			whereHead = whereHead->next;
			free(listIter);
			listIter = whereHead;
		}
	}
}

/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE* f)
{

	/* to be implemented */
	//Setting each linked-list
	//knowledgeNode* whoPtr = whoHead,
	//	* wherePtr = whereHead,
	//	* whatPtr = whatHead;
	//	//* temp = listIter;

	//WHO
	fprintf(f, "[who]\n");

	listIter = whoHead;
	while (listIter != NULL)
	{
		if (listIter->response[0] != '\0') {
			fprintf(f, "%s=%s\n", listIter->entity, listIter->response); //writing each data onto the file
		}
		listIter = listIter->next; //point to the next after each node
	}

	fprintf(f, "\n");

	//WHERE
	fprintf(f, "[where]\n");

	listIter = whereHead;
	while (listIter != NULL)
	{
		if (listIter->response[0] != '\0') {
			fprintf(f, "%s=%s\n", listIter->entity, listIter->response); //writing each data onto the file
		}
		listIter = listIter->next; //point to the next after each node
	}

	fprintf(f, "\n");

	//WHAT
	fprintf(f, "[what]\n");

	listIter = whatHead;
	while (listIter != NULL)
	{
		if (listIter->response[0] != '\0') {
			fprintf(f, "%s=%s\n", listIter->entity, listIter->response); //writing each data onto the file
		}
		listIter = listIter->next; //point to the next after each node
	}

	fprintf(f, "\n");
}
