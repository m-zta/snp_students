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
 * @brief Lab implementation: Personenverwaltung Hauptprogramm
 */
#include "list.h"
#include "person.h"

#include <ctype.h> // für toupper
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // für fgets, strcmp

// Helper Funktion zum Lesen der Benutzereingabe (Menüauswahl)
char get_menu_choice(void) {
    char buf[10]; // Puffer für Eingabe
    printf("\nI(nsert), R(emove), S(how), C(lear), E(nd): ");
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        // Nur das erste Zeichen nehmen und in Grossbuchstaben umwandeln
        return toupper(buf[0]);
    }
    return '\0'; // Fehler oder EOF
}

/**
 * @brief Main entry point.
 * @param[in] argc  The size of the argv array.
 * @param[in] argv  The command line arguments...
 * @returns Returns EXIT_SUCCESS (=0) on success, EXIT_FAILURE (=1) there is an
 * expression syntax error.
 */
int main(int argc, char* argv[]) {
    node_t anchor;      // Anker-Knoten für die Liste (statisch alloziiert)
    list_init(&anchor); // Liste initialisieren (Anker zeigt auf sich selbst)

    char choice;
    person_t
        current_person; // Variable für aktuell einzulesende/zu löschende Person

    printf("Personenverwaltung V1.0\n");

    while (1) { // Endlosschleife für das Menü
        choice = get_menu_choice();

        switch (choice) {
            case 'I': // Insert
                printf("--- Insert Person ---\n");
                if (person_read(&current_person) == 0) {
                    if (list_insert(&anchor, &current_person) == NULL) {
                        // Fehlerbehandlung: Speicherproblem oder Duplikat
                        fprintf(stderr, "Error: Could not insert person. "
                                        "Memory full or duplicate?\n");
                    } else {
                        printf("Person inserted successfully.\n");
                    }
                } else {
                    fprintf(stderr, "Error: Invalid person data entered. "
                                    "Operation cancelled.\n");
                    // Sicherstellen, dass keine ungültigen Daten in
                    // current_person bleiben (Obwohl person_read das
                    // idealerweise intern handhaben sollte)
                }
                break;

            case 'R': // Remove
                printf("--- Remove Person ---\n");
                printf("Enter data of person to remove:\n");
                if (person_read(&current_person) == 0) {
                    if (list_remove(&anchor, &current_person) == 0) {
                        printf("Person removed successfully.\n");
                    } else {
                        printf("Person not found in list.\n");
                    }
                } else {
                    fprintf(stderr, "Error: Invalid person data entered for "
                                    "removal. Operation cancelled.\n");
                }
                break;

            case 'S': // Show
                printf("--- Show Persons ---\n");
                list_show(&anchor);
                break;

            case 'C': // Clear
                printf("--- Clear List ---\n");
                list_clear(&anchor);
                printf("List cleared.\n");
                break;

            case 'E': // End
                printf("--- End Program ---\n");
                list_clear(&anchor); // Wichtig: Speicher freigeben vor Beenden
                printf("Goodbye!\n");
                return EXIT_SUCCESS; // Programm erfolgreich beenden

            case '\0': // Fehler beim Lesen (z.B. Ctrl+D)
                fprintf(stderr,
                        "\nError reading input or EOF detected. Exiting.\n");
                list_clear(&anchor); // Auch hier Speicher freigeben
                return EXIT_FAILURE;

            default: // Ungültige Auswahl
                fprintf(stderr,
                        "Error: Invalid choice '%c'. Please try again.\n",
                        choice);
                break;
        }
    }

    // Wird normalerweise nicht erreicht, aber zur Sicherheit:
    list_clear(&anchor);
    return EXIT_FAILURE; // Falls die Schleife unerwartet verlassen wird
}