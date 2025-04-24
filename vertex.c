/**
 * @file  vertex.c
 * @author Alejandro Seguido & Pablo Calderón
 * @date 14 March 2025
 * @version 2.0
 * @brief Implements the ADT Vertex
 *
 * @details
 *
 * @see
 */

#include "vertex.h"

#include <string.h>
#include <stdlib.h>

struct _Vertex
{
  int id;               /*!< identification number, must be unique and equal or greater than 0 */
  char tag[TAG_LENGTH]; /*!< name of the vertex */
  Label state;          /*!< state of the vertex */
  int index;            /*!< position of the vertex in a graph */
};

/*----------------------------------------------------------------------------------------*/
/*
Private function:
*/

/**
 * @brief Modifies the id, tag or status of a given vertex.
 *
 * @details
 * The function takes a pointer to a vertex, a key and a value as arguments.
 * The key is a string that indicates which variable is going to be modified
 * and the value is the new value of the variable. The function will take the
 * correct value of the variable and update it.
 *
 * @param v Vertex to be printed
 * @param key Char that inidicates which variable is going to be modified
 * @param value New vertex id/tag/status
 *
 * @return Returns OK or ERROR in case of error.
 */
Status vertex_setField(Vertex *v, char *key, char *value)
{
  /* Check args: */
  if (!key || !value)
    return ERROR;

  if (strcmp(key, "id") == 0)
  {
    /* Modify the id of the vertex */
    return vertex_setId(v, atol(value));
  }
  else if (strcmp(key, "tag") == 0)
  {
    /* Modify the tag of the vertex */
    return vertex_setTag(v, value);
  }
  else if (strcmp(key, "state") == 0)
  {
    /* Modify the state of the vertex */
    return vertex_setState(v, (Label)atoi(value));
  }

  return ERROR;
}

/*----------------------------------------------------------------------------------------*/
Vertex *vertex_initFromString(char *descr)
{
  char buffer[1024];
  char *token;
  char *key;
  char *value;
  char *p;
  Vertex *v;

  /* Check args: */
  if (!descr)
    return NULL;

  /* Allocate memory for vertex: */
  v = vertex_init();
  if (!v)
    return NULL;

  /* Read and tokenize description: */
  sprintf(buffer, "%s", descr);
  token = strtok(buffer, " \t\n\r");
  while (token)
  {
    p = strchr(token, ':');
    if (!p)
      continue;

    *p = '\0';
    key = token;
    value = p + 1;

    if (vertex_setField(v, key, value) == ERROR)
    {
      vertex_free(v);
      return NULL;
    }

    token = strtok(NULL, " \t\n");
  }

  return v;
}

Vertex *vertex_init()
{
  Vertex *vertex = NULL;

  /* Allocate memory for a vertex */
  if (!(vertex = (Vertex *)malloc(sizeof(Vertex))))
  {
    return NULL;
  }

  /* Initialization of a vertex */
  if (vertex_setId(vertex, 0) == ERROR)
  {
    free(vertex);
    return NULL;
  }
  if (vertex_setTag(vertex, "") == ERROR)
  {
    free(vertex);
    return NULL;
  }
  if (vertex_setState(vertex, WHITE) == ERROR)
  {
    free(vertex);
    return NULL;
  }
  vertex->index = 0;

  return vertex;
}

void vertex_free(void *v)
{
  if (v)
    free(v);
}

int vertex_getId(const Vertex *v)
{
  if (!v)
    return ERROR_VALUE;

  return v->id;
}

const char *vertex_getTag(const Vertex *v)
{
  if (!v)
    return NULL;

  return v->tag;
}

Label vertex_getState(const Vertex *v)
{
  if (!v)
    return ERROR_VERTEX;

  return v->state;
}

int vertex_getIndex(const Vertex *v)
{
  if (!v)
    return ERROR_VALUE;

  return v->index;
}

Status vertex_setId(Vertex *v, const int id)
{
  /* Check args: */
  if (!v || id < 0)
    return ERROR;

  v->id = id;

  return OK;
}

Status vertex_setTag(Vertex *v, const char *tag)
{
  /* Check args: */
  if (!v || !tag)
    return ERROR;

  /* Error control */
  if (strlen(tag) >= TAG_LENGTH)
    return ERROR;

  if (!strcpy(v->tag, tag))
  {
    return ERROR;
  }

  return OK;
}

Status vertex_setState(Vertex *v, const Label state)
{
  /* Check args: */
  if (!v)
    return ERROR;

  /* Error control */
  if (state != WHITE && state != BLACK && state != ERROR_VERTEX)
    return ERROR;

  v->state = state;
  return OK;
}

Status vertex_setIndex(Vertex *v, const int ix)
{
  /* Check args: */
  if (!v || ix < 0)
  {
    return ERROR;
  }

  v->index = ix;
  return OK;
}

int vertex_cmp(const void *v1, const void *v2)
{
  const Vertex *vertex1;
  const Vertex *vertex2;

  /* Check args: */
  if (!v1 || !v2)
    return 0;

  /* Exchange void pointer to Vertex pointer */
  vertex1 = (const Vertex *)v1;
  vertex2 = (const Vertex *)v2;

  /* Compare id and return the diference */
  if (vertex1->id != vertex2->id)
  {
    return vertex1->id - vertex2->id;
  }

  /* In case the id are equal, return tag comparation */
  return strcmp(vertex1->tag, vertex2->tag);
}

void *vertex_copy(const void *src)
{
  Vertex *v = NULL;
  const Vertex *src_v = NULL;

  /* Check args: */
  if (!src)
    return NULL;

  /* Exchange void pointer to Vertex pointer */
  src_v = (Vertex *)src;

  /* Allocate memory for a vertex */
  if (!(v = vertex_init()))
  {
    return NULL;
  }

  /* Copy the data from src to v */
  if (vertex_setId(v, vertex_getId(src_v)) == ERROR)
  {
    vertex_free(v);
    return NULL;
  }
  if (vertex_setTag(v, vertex_getTag(src_v)) == ERROR)
  {
    vertex_free(v);
    return NULL;
  }
  if (vertex_setState(v, vertex_getState(src_v)) == ERROR)
  {
    vertex_free(v);
    return NULL;
  }

  return v;
}

int vertex_print(FILE *pf, const void *v)
{
  int cont;
  const Vertex *vertex = NULL;

  /* Check args: */
  if (!pf || !v)
    return ERROR_VALUE;

  /* Exchange void pointer to Vertex pointer */
  vertex = (Vertex *)v;

  /* Count the characters that have been written successfully */
  cont = 0;
  cont = fprintf(pf, "[%d, %s, %d, %d]", vertex_getId(vertex), vertex_getTag(vertex), vertex_getState(vertex), vertex_getIndex(vertex));

  return cont;
}
