/**
 * @file
 * @brief Lab implementation: Linked list operations
 */
#include "list.h" // Enthält person.h

#include <stdio.h>
#include <stdlib.h> // fuer malloc, free
#include <string.h> // fuer Fehlerbehandlung (nicht direkt hier, aber oft nützlich)

/**
 * @brief Initialisiert einen Listenknoten als Anker einer leeren,
 * zyklisch verketteten Liste.
 */
void list_init(node_t* anchor) {
    if (anchor != NULL) {
        anchor->next = anchor;
        // Der Inhalt des Ankers wird nicht initialisiert, da er keine gültige
        // Person darstellt.
    }
}

/**
 * @brief Fügt eine Person sortiert in die Liste ein.
 */
node_t* list_insert(node_t* anchor, const person_t* person) {
    if (anchor == NULL || person == NULL) {
        return NULL; // Ungültige Argumente
    }

    // 1. Neuen Knoten erstellen und Speicher allozieren
    node_t* new_node = (node_t*) malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("Error allocating memory for new node");
        return NULL; // Speicherfehler
    }
    new_node->content = *person; // Kopiere die Personendaten

    // 2. Korrekte Einfügeposition finden und auf Duplikate prüfen
    node_t* current = anchor;
    int     compare_result;

    // Gehe durch die Liste, bis die Einfügeposition gefunden ist
    // Bedingung: nächster Knoten ist der Anker ODER nächster Knoten ist
    // "grösser"
    while (current->next != anchor) {
        compare_result = person_compare(person, &(current->next->content));
        if (compare_result == 0) {
            // Duplikat gefunden
            fprintf(stderr,
                    "Error: Person already exists in the list (duplicate).\n");
            free(new_node); // Allozierten Speicher wieder freigeben!
            return NULL;
        }
        if (compare_result < 0) {
            // Einfügeposition gefunden (vor current->next)
            break;
        }
        // Weiterschalten
        current = current->next;
    }
    // Nach der Schleife ist 'current' der Knoten, *nach* dem eingefügt werden
    // soll.

    // 3. Neuen Knoten einfügen
    new_node->next = current->next;
    current->next  = new_node;

    return new_node; // Erfolg, gib Zeiger auf neuen Knoten zurück
}

// TODO: Generell im code: Keine Zahlen "Magic Numbers", sondern oben ein
// #define

/**
 * @brief Entfernt eine Person aus der Liste.
 */
int list_remove(node_t* anchor, const person_t* person) {
    if (anchor == NULL || person == NULL || anchor->next == anchor) {
        return -1; // Ungültige Argumente oder leere Liste
    }

    node_t* current        = anchor;
    node_t* node_to_remove = NULL;

    // Suche nach dem Knoten *vor* dem zu löschenden Knoten
    while (current->next != anchor) {
        if (person_compare(person, &(current->next->content)) == 0) {
            // Person im nächsten Knoten gefunden
            node_to_remove = current->next;
            break; // Schleife beenden, Position gefunden
        }
        current = current->next;
    }

    // 4. Knoten entfernen, falls gefunden
    if (node_to_remove != NULL) {
        current->next = node_to_remove->next; // Knoten ausketten
        free(node_to_remove);                 // Speicher freigeben
        return 0;                             // Erfolg
    } else {
        return -1; // Person nicht gefunden
    }
}

/**
 * @brief Gibt alle Personen in der Liste in sortierter Reihenfolge aus.
 */
void list_show(const node_t* anchor) {
    if (anchor == NULL || anchor->next == anchor) {
        printf("List is empty.\n");
        return;
    }

    printf("--------------------\n");
    const node_t* current =
        anchor->next; // Starte beim ersten Element nach dem Anker
    int count = 1;
    while (current != anchor) {
        printf("Person %d:\n", count++);
        person_print(&(current->content));
        printf("--------------------\n");
        current = current->next; // Gehe zum nächsten Knoten
    }
}

// TODO: Grundsätzlich zum Fehler verhindern: Bei 'free' den pointer auf Null
// setzen

/**
 * @brief Entfernt alle Personenknoten aus der Liste und gibt deren Speicher
 * frei.
 */
void list_clear(node_t* anchor) {
    if (anchor == NULL) {
        return;
    }

    node_t* current        = anchor->next;
    node_t* node_to_delete = NULL;

    while (current != anchor) {
        node_to_delete = current;       // Knoten merken
        current        = current->next; // Zum nächsten Knoten gehen *bevor* der
                                        // aktuelle gelöscht wird
        free(node_to_delete);           // Gemerkten Knoten löschen
        node_to_delete = NULL;          // Besser es knallt im Fehlerfall
    }

    // Liste ist nun leer, Anker zurücksetzen
    anchor->next = anchor;
}