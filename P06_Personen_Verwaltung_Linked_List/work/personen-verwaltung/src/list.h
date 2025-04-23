#ifndef LIST_H
#define LIST_H

#include "person.h" // Benötigt die Definition von person_t

/**
 * @brief Struktur für einen Knoten in der verketteten Liste.
 */
typedef struct node {
    person_t     content; ///< Die in diesem Knoten gespeicherte Person.
    struct node* next;    ///< Zeiger auf den nächsten Knoten in der Liste.
} node_t;

/**
 * @brief Initialisiert einen Listenknoten als Anker einer leeren,
 * zyklisch verketteten Liste.
 * Der Ankerknoten zeigt auf sich selbst. Der Inhalt ist undefiniert.
 * @param anchor [IN/OUT] Zeiger auf den zu initialisierenden Ankerknoten.
 */
void list_init(node_t* anchor);

/**
 * @brief Fügt eine Person sortiert in die Liste ein.
 * Es wird dynamisch Speicher für einen neuen Knoten alloziiert.
 * Die Liste bleibt nach dem Einfügen sortiert gemäss person_compare.
 * Duplikate (Personen, für die person_compare 0 zurückgibt) werden
 * nicht eingefügt.
 * @param anchor [IN/OUT] Zeiger auf den Ankerknoten der Liste.
 * @param person [IN] Zeiger auf die Person, die eingefügt werden soll.
 * @return Zeiger auf den neu eingefügten Knoten bei Erfolg,
 * NULL bei Speicherallozierungsfehler oder wenn die Person
 * bereits existiert (Duplikat).
 */
node_t* list_insert(node_t* anchor, const person_t* person);

/**
 * @brief Entfernt eine Person aus der Liste.
 * Sucht den Knoten, der die angegebene Person enthält (mittels person_compare),
 * löst ihn aus der Verkettung und gibt den Speicher frei.
 * @param anchor [IN/OUT] Zeiger auf den Ankerknoten der Liste.
 * @param person [IN] Zeiger auf die Person, die entfernt werden soll (nur die
 * Daten sind relevant für den Vergleich).
 * @return 0 wenn die Person gefunden und entfernt wurde,
 * -1 wenn die Person nicht in der Liste gefunden wurde.
 */
int list_remove(node_t* anchor, const person_t* person);

/**
 * @brief Gibt alle Personen in der Liste in sortierter Reihenfolge auf der
 * Standardausgabe aus.
 * @param anchor [IN] const Zeiger auf den Ankerknoten der Liste.
 */
void list_show(const node_t* anchor);

/**
 * @brief Entfernt alle Personenknoten aus der Liste und gibt deren Speicher
 * frei. Die Liste ist danach wieder leer (nur der Ankerknoten existiert noch).
 * @param anchor [IN/OUT] Zeiger auf den Ankerknoten der Liste.
 */
void list_clear(node_t* anchor);

#endif // LIST_H