/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 */
/**
 * @file
 * @brief Test suite for the given package.
 */
#include "test_utils.h"

#include <CUnit/Basic.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // für strcpy
#include <sys/wait.h>
#include <time.h>

// Include headers der zu testenden Module
#include "list.h"
#include "person.h"

#ifndef TARGET // must be given by the make file --> see test target
#error missing TARGET define
#endif

/// @brief alias for EXIT_SUCCESS
#define OK EXIT_SUCCESS
/// @brief alias for EXIT_FAILURE
#define FAIL EXIT_FAILURE

/// @brief The name of the STDOUT text file.
#define OUTFILE "stdout.txt"
/// @brief The name of the STDERR text file.
#define ERRFILE "stderr.txt"

#define TRACE_INDENT                                                           \
    "\n                " ///< allow for better stdout formatting in case of
                         ///< error

// setup & cleanup
static int setup(void) {
    remove_file_if_exists(OUTFILE);
    remove_file_if_exists(ERRFILE);
    return 0; // success
}

static int teardown(void) {
    // Nichts zu tun hier. Die Liste wird in den Tests selbst aufgeräumt.
    return 0; // success
}

// ==== Hilfsfunktion zum Erstellen von Personen für Tests ====
static person_t create_person(const char* name, const char* first_name,
                              unsigned int age) {
    person_t p;
    strncpy(p.name, name, NAME_LEN - 1);
    p.name[NAME_LEN - 1] = '\0'; // Sicherstellen, dass nullterminiert
    strncpy(p.first_name, first_name, NAME_LEN - 1);
    p.first_name[NAME_LEN - 1] = '\0';
    p.age                      = age;
    return p;
}

// ==== Tests ====

static void test_person_compare(void) {
    // arrange
    person_t p1 = create_person("Meier", "Hans", 30);
    person_t p2 = create_person("Meier", "Hans", 30);  // Identisch
    person_t p3 = create_person("Huber", "Anna", 25);  // Name kleiner
    person_t p4 = create_person("Schmidt", "Max", 40); // Name grösser
    person_t p5 = create_person("Meier", "Eva", 35);   // Vorname kleiner
    person_t p6 = create_person("Meier", "Peter", 28); // Vorname grösser
    person_t p7 = create_person("Meier", "Hans", 20);  // Alter kleiner
    person_t p8 = create_person("Meier", "Hans", 40);  // Alter grösser

    // act & assert
    CU_ASSERT_EQUAL(person_compare(&p1, &p2), 0); // p1 == p2
    CU_ASSERT_TRUE(person_compare(&p1, &p3) > 0); // p1 > p3 (wegen Name)
    CU_ASSERT_TRUE(person_compare(&p1, &p4) < 0); // p1 < p4 (wegen Name)
    CU_ASSERT_TRUE(person_compare(&p1, &p5) > 0); // p1 > p5 (wegen Vorname)
    CU_ASSERT_TRUE(person_compare(&p1, &p6) < 0); // p1 < p6 (wegen Vorname)
    CU_ASSERT_TRUE(person_compare(&p1, &p7) > 0); // p1 > p7 (wegen Alter)
    CU_ASSERT_TRUE(person_compare(&p1, &p8) < 0); // p1 < p8 (wegen Alter)

    // Test mit exakt gleichen Namen/Vornamen aber unterschiedlichem Alter
    CU_ASSERT_TRUE(person_compare(&p7, &p8) < 0);
    CU_ASSERT_TRUE(person_compare(&p8, &p7) > 0);
}

static void test_list_insert(void) {
    // arrange
    node_t anchor;
    list_init(&anchor);
    person_t p1 = create_person("Meier", "Hans", 30);
    person_t p2 = create_person("Huber", "Anna", 25);  // Sollte zuerst kommen
    person_t p3 = create_person("Schmidt", "Max", 40); // Sollte zuletzt kommen
    person_t p_dup = create_person("Meier", "Hans", 30); // Duplikat von p1

    node_t* inserted_node;

    // act & assert

    // 1. In leere Liste einfügen
    inserted_node = list_insert(&anchor, &p1);
    CU_ASSERT_PTR_NOT_NULL(inserted_node);
    CU_ASSERT_PTR_EQUAL(anchor.next, inserted_node);   // Anker zeigt auf p1
    CU_ASSERT_PTR_EQUAL(inserted_node->next, &anchor); // p1 zeigt auf Anker
    CU_ASSERT_EQUAL(person_compare(&inserted_node->content, &p1),
                    0); // Inhalt korrekt?

    // 2. Person einfügen, die an den Anfang gehört
    inserted_node = list_insert(&anchor, &p2);
    CU_ASSERT_PTR_NOT_NULL(inserted_node);
    CU_ASSERT_PTR_EQUAL(anchor.next,
                        inserted_node); // Anker zeigt auf p2 (Huber)
    CU_ASSERT_EQUAL(person_compare(&inserted_node->content, &p2), 0);
    CU_ASSERT_PTR_EQUAL(inserted_node->next,
                        anchor.next->next); // p2 zeigt auf p1
    CU_ASSERT_EQUAL(person_compare(&anchor.next->next->content, &p1),
                    0); // p1 ist zweites Element
    CU_ASSERT_PTR_EQUAL(anchor.next->next->next, &anchor); // p1 zeigt auf Anker

    // 3. Person einfügen, die ans Ende gehört
    inserted_node = list_insert(&anchor, &p3);
    CU_ASSERT_PTR_NOT_NULL(inserted_node);
    // Anker -> p2 -> p1 -> p3 -> Anker
    CU_ASSERT_PTR_EQUAL(anchor.next->next->next,
                        inserted_node); // p1 zeigt auf p3 (Schmidt)
    CU_ASSERT_EQUAL(person_compare(&inserted_node->content, &p3), 0);
    CU_ASSERT_PTR_EQUAL(inserted_node->next, &anchor); // p3 zeigt auf Anker

    // 4. Duplikat einfügen (sollte fehlschlagen)
    inserted_node = list_insert(&anchor, &p_dup);
    CU_ASSERT_PTR_NULL(inserted_node);
    // Liste sollte unverändert sein: Anker -> p2 -> p1 -> p3 -> Anker
    CU_ASSERT_EQUAL(person_compare(&anchor.next->content, &p2), 0);
    CU_ASSERT_EQUAL(person_compare(&anchor.next->next->content, &p1), 0);
    CU_ASSERT_EQUAL(person_compare(&anchor.next->next->next->content, &p3), 0);
    CU_ASSERT_PTR_EQUAL(anchor.next->next->next->next, &anchor);

    // 5. NULL-Pointer testen
    CU_ASSERT_PTR_NULL(list_insert(NULL, &p1));
    CU_ASSERT_PTR_NULL(list_insert(&anchor, NULL));

    // cleanup
    list_clear(&anchor); // Wichtig, um Speicher freizugeben
}

static void test_list_remove(void) {
    // arrange
    node_t anchor;
    list_init(&anchor);
    person_t p1         = create_person("Huber", "Anna", 25);
    person_t p2         = create_person("Meier", "Hans", 30);
    person_t p3         = create_person("Schmidt", "Max", 40);
    person_t p_nonexist = create_person("Weber", "Tim", 50);

    list_insert(&anchor, &p2); // Meier
    list_insert(&anchor, &p1); // Huber
    list_insert(&anchor, &p3); // Schmidt
    // Liste: Anker -> p1 (Huber) -> p2 (Meier) -> p3 (Schmidt) -> Anker

    int result;

    // act & assert

    // 1. Element in der Mitte entfernen (p2)
    result = list_remove(&anchor, &p2);
    CU_ASSERT_EQUAL(result, 0);
    // Liste: Anker -> p1 (Huber) -> p3 (Schmidt) -> Anker
    // CU_ASSERT_PTR_EQUAL(
    //     anchor.next, anchor.next->next->next); // p1->next sollte jetzt p3 sein
    CU_ASSERT_EQUAL(person_compare(&anchor.next->content, &p1), 0);
    CU_ASSERT_EQUAL(person_compare(&anchor.next->next->content, &p3), 0);

    // 2. Nicht existierendes Element entfernen
    result = list_remove(&anchor, &p_nonexist);
    CU_ASSERT_EQUAL(result, -1);
    // Liste sollte unverändert sein: Anker -> p1 -> p3 -> Anker
    CU_ASSERT_EQUAL(person_compare(&anchor.next->content, &p1), 0);
    CU_ASSERT_EQUAL(person_compare(&anchor.next->next->content, &p3), 0);
    CU_ASSERT_PTR_EQUAL(anchor.next->next->next, &anchor);

    // 3. Erstes Element entfernen (p1)
    result = list_remove(&anchor, &p1);
    CU_ASSERT_EQUAL(result, 0);
    // Liste: Anker -> p3 (Schmidt) -> Anker
    CU_ASSERT_EQUAL(person_compare(&anchor.next->content, &p3), 0);
    CU_ASSERT_PTR_EQUAL(anchor.next->next, &anchor);

    // 4. Letztes Element entfernen (p3)
    result = list_remove(&anchor, &p3);
    CU_ASSERT_EQUAL(result, 0);
    // Liste: Anker -> Anker (leer)
    CU_ASSERT_PTR_EQUAL(anchor.next, &anchor);

    // 5. Aus leerer Liste entfernen
    result = list_remove(&anchor, &p2); // p2 existiert nicht mehr
    CU_ASSERT_EQUAL(result, -1);
    CU_ASSERT_PTR_EQUAL(anchor.next, &anchor);

    // 6. NULL-Pointer testen
    // Das Verhalten bei NULL Pointern ist in list.c definiert (gibt -1 zurück)
    CU_ASSERT_EQUAL(list_remove(NULL, &p1), -1);
    CU_ASSERT_EQUAL(list_remove(&anchor, NULL), -1);

    // cleanup (Liste ist bereits leer)
    list_clear(&anchor);
}

static void test_list_clear(void) {
    // arrange
    node_t anchor;
    list_init(&anchor);
    person_t p1 = create_person("A", "A", 1);
    person_t p2 = create_person("B", "B", 2);
    person_t p3 = create_person("C", "C", 3);

    // act & assert

    // 1. Leere Liste löschen
    list_clear(&anchor);
    CU_ASSERT_PTR_EQUAL(anchor.next, &anchor); // Muss leer bleiben

    // 2. Liste mit Elementen füllen
    list_insert(&anchor, &p1);
    list_insert(&anchor, &p2);
    list_insert(&anchor, &p3);
    CU_ASSERT_PTR_NOT_EQUAL(anchor.next,
                            &anchor); // Liste sollte nicht leer sein

    // 3. Gefüllte Liste löschen
    list_clear(&anchor);
    CU_ASSERT_PTR_EQUAL(anchor.next, &anchor); // Muss jetzt leer sein

    // 4. NULL-Pointer testen (sollte nichts tun / nicht abstürzen)
    // list_clear prüft intern auf NULL
    list_clear(NULL); // Testet nur, ob es keinen Crash gibt

    // cleanup (Liste ist bereits leer)
}

/**
 * @brief Registers and runs the tests.
 * @returns success (0) or one of the CU_ErrorCode (>0)
 */
int main(void) {
    // setup, run, teardown
    TestMainBasic("Personenverwaltung Test Suite", setup, teardown,
                  test_person_compare, test_list_insert, test_list_remove,
                  test_list_clear);
}