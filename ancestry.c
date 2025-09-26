#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ancestry.h"

struct person_t *make_person(char *first_name, char *last_name, int year_born) {
    //Heap allocation
    struct person_t *new = malloc(sizeof(struct person_t) );
    new->first_name = malloc( strlen(first_name) + 1);
    new->last_name = malloc( strlen(last_name) + 1);
    new->year_born = year_born;

    //copy strings:
    strcpy(new->first_name, first_name);
    strcpy(new->last_name , last_name);

    return new;
}

void free_person(struct person_t *person) {
    //free names
    free(person->first_name);
    person->first_name = NULL; //sikker C :)). Avoid use-after-free bug.
    free(person->last_name);
    person->last_name = NULL;

    //free struct
    free(person);
    person = NULL;
}

struct ancestry_node_t *make_tree(struct person_t *person) {
    struct ancestry_node_t *tree = malloc(sizeof (struct ancestry_node_t) );

    tree->person = person;
    tree->mom = NULL;
    tree->dad = NULL;

    return tree;
}

bool add_mom(struct ancestry_node_t *child_node, struct person_t *mom_person) {
    if (child_node->mom != NULL) {
        return false;
    }

    child_node->mom = make_tree(mom_person);
    return true;
}

bool add_dad(struct ancestry_node_t *child_node, struct person_t *dad_person) {
    if (child_node->dad != NULL) {
        return false;
    }

    child_node->dad = make_tree(dad_person);
    return true;
}

void free_tree(struct ancestry_node_t *node) {
    // TODO: Kald `free_person()` på personen i noden.
    // TODO: Kald `free_tree()` på begge forældre-stamtræer.
    // TODO: Sæt pointerne til NULL efter de er free()'et.
}

void print_tree_recursive(struct ancestry_node_t *node, char *prefix, bool is_last) {
    if (node == NULL) {
        return;
    }

    printf("%s", prefix);
    printf("%s", is_last ? "└── " : "├── ");
    printf("%s %s\n", node->person->first_name, node->person->last_name);

    char new_prefix[1000];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");

    if (node->mom != NULL && node->dad != NULL) {
        print_tree_recursive(node->mom, new_prefix, false);
        print_tree_recursive(node->dad, new_prefix, true);
    } else if (node->mom != NULL) {
        print_tree_recursive(node->mom, new_prefix, true);
    } else if (node->dad != NULL) {
        print_tree_recursive(node->dad, new_prefix, true);
    }
}

void print_tree(struct ancestry_node_t *node) {
    print_tree_recursive(node, "", true);
}
