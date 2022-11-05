#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

knowledgeNode* nodeConstructor(char* intent, char* entity, char* response)
{	
	//checking intent, to see which list the node should be added to, using strstr as the intent is already being controlled
	if (strstr(intent, "who"))
	{
		listIter = whoHead;
	}
	else if (strstr(intent, "what"))
	{
		listIter = whatHead;
	}
	else if (strstr(intent, "where"))
	{
		listIter = whereHead;
	}
	else
	{
		printf("intent not properly specified, unable to construct node, should never enter here, as intent should have been controlled earlier (knowledgeNode.c)\n");
		return NULL;
	}

	//constructing newNode
	knowledgeNode* newNode = malloc(sizeof(knowledgeNode));
	strcpy(newNode->entity, entity);
	strcpy(newNode->response, response);
	newNode->next = NULL;

	//adding node to the back of its respective list
	listIter = getLastNode(listIter);
	if (NULL == listIter) //if list was found to be empty, make the currently constructed node the head
	{
		if (strstr(intent, "who"))
		{
			whoHead = newNode;
		}
		else if (strstr(intent, "what"))
		{
			whatHead = newNode;
		}
		else if (strstr(intent, "where"))
		{
			whereHead= newNode;
		}
	}
	else //else add it to the tail
	{
		listIter->next = newNode;
	}

	return newNode;
}

knowledgeNode* getLastNode(knowledgeNode* head)
{
	if (NULL == head)
	{
		//printf("Node passed in was null (knowledgeNode.c - getLastNode)\n");
		return NULL; // unlikely case
	}

	knowledgeNode* iter = head;
	//go through the list, till the end and return the tail node
	while (NULL != iter->next)
	{
		iter = iter->next;
	}
	return iter;
}

knowledgeNode* doesEntityExist(knowledgeNode* head, char* entity)
{
	
	if (NULL == head)
	{
		//printf("Node passed in was null (knowledgeNode.c - doesEntityExist)\n");
		return NULL; // unlikely case
	}

	knowledgeNode* iter = head;
	//iterate through the list, checking if entity exists 
	//SHOULD USE COMPARE TOKEN + THIS MAYBE SHOULD BE COMBINED WITH getLastNode?
	while (iter->entity != entity)
	{
		//if next is null break out of the list
		if (NULL == iter->next)
			break;

		//list iterating
		iter = iter->next;
	}
	return iter;

	
	//return nullptr;
}

//DEBUG FUNCTION
void printList(knowledgeNode* head)
{
	if (NULL == head)
	{
		return;
	}

	knowledgeNode* iter = head;
	while (1)
	{
		printf("%s = %s \n", iter->entity, iter->response);
		if (NULL == iter->next)
		{
			break;
		}
		iter = iter->next;
	} 
}

