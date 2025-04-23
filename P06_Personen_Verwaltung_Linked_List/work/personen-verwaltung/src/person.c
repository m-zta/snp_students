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
 * @brief Lab implementation: Person structure and functions
 */
#include "person.h"

#include <errno.h>  // fuer errno
#include <limits.h> // fuer ULONG_MAX
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // fuer strncmp, strlen, fgets, strchr

/**
 * @brief  Vergleicht zwei Personen nach Name, Vorname und Alter.
 */
int person_compare(const person_t* a, const person_t* b) {
    int cmp;

    // 1. Nach Name vergleichen
    cmp = strncmp(a->name, b->name, NAME_LEN);
    if (cmp != 0) {
        return cmp;
    }

    // 2. Nach Vorname vergleichen (wenn Nachnamen gleich sind)
    cmp = strncmp(a->first_name, b->first_name, NAME_LEN);
    if (cmp != 0) {
        return cmp;
    }

    // 3. Nach Alter vergleichen (wenn Namen gleich sind)
    // Sicherer Vergleich für unsigned int
    if (a->age < b->age) {
        return -1;
    } else if (a->age > b->age) {
        return 1;
    } else {
        return 0; // Alle Felder sind gleich
    }
}

// Hilfsfunktion zum sicheren Lesen einer Zeile und Entfernen des Newlines
// Gibt 0 bei Erfolg, -1 bei Fehler oder zu langer Eingabe zurück
static int read_line(char* buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        // Lesefehler oder EOF
        fprintf(stderr, "Error reading input.\n");
        return -1;
    }

    // Check ob Eingabe zu lang war (kein Newline im Puffer)
    char* newline_ptr = strchr(buffer, '\n');
    if (newline_ptr == NULL) {
        // Eingabe war zu lang, leere Rest des stdin-Puffers
        fprintf(stderr, "Error: Input too long (max %d characters allowed).\n",
                size - 2); // -1 für Nullterm, -1 für \n
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ; // Restliche Zeile verwerfen
        return -1;
    }

    // Ersetze Newline durch Nullterminator
    *newline_ptr = '\0';

    // Check ob Eingabe leer war (nur Enter gedrückt)
    if (strlen(buffer) == 0) {
        fprintf(stderr, "Error: Input cannot be empty.\n");
        return -1;
    }

    return 0; // Erfolg
}

/**
 * @brief Liest die Daten einer Person von der Standardeingabe ein.
 */
int person_read(person_t* person) {
    char          age_buffer[10]; // Puffer für Alter-Eingabe
    char*         endptr;
    unsigned long temp_age;

    printf("  Name: ");
    if (read_line(person->name, NAME_LEN) != 0) {
        return -1; // Fehler beim Lesen oder Name zu lang/leer
    }

    printf("  First name: ");
    if (read_line(person->first_name, NAME_LEN) != 0) {
        return -1; // Fehler beim Lesen oder Vorname zu lang/leer
    }

    printf("  Age: ");
    if (read_line(age_buffer, sizeof(age_buffer)) != 0) {
        return -1; // Fehler beim Lesen oder Alter zu lang/leer
    }

    // Alter konvertieren und validieren
    errno    = 0; // errno zurücksetzen vor strtoul
    temp_age = strtoul(age_buffer, &endptr, 10);

    // Prüfen auf Konvertierungsfehler
    if (errno != 0 || *endptr != '\0') {
        fprintf(stderr, "Error: Age must be a valid non-negative number.\n");
        return -1;
    }

    // Prüfen auf gültigen Bereich für unsigned int (sollte durch strtoul
    // passen, aber sicher ist sicher)
    if (temp_age > UINT_MAX) {
        fprintf(stderr, "Error: Age is too large.\n");
        return -1;
    }

    person->age = (unsigned int) temp_age;

    return 0; // Erfolg
}

/**
 * @brief Gibt die Daten einer Person auf der Standardausgabe aus.
 */
void person_print(const person_t* person) {
    if (person != NULL) {
        printf("  Name: %s\n", person->name);
        printf("  First name: %s\n", person->first_name);
        printf("  Age: %u\n", person->age);
    }
}