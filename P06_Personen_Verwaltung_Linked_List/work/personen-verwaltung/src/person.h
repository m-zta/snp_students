#ifndef PERSON_H
#define PERSON_H

// Konstante für die maximale Länge von Namen
#define NAME_LEN 20

/**
 * @brief Struktur zur Speicherung der Daten einer Person.
 */
typedef struct {
    char         name[NAME_LEN];       ///< Nachname der Person
    char         first_name[NAME_LEN]; ///< Vorname der Person
    unsigned int age;                  ///< Alter der Person
} person_t;

/**
 * @brief  Vergleicht zwei Personen nach Name, Vorname und Alter.
 * @param  a [IN] const Referenz auf die erste Person.
 * @param  b [IN] const Referenz auf die zweite Person.
 * @return =0 wenn alle Felder identisch sind.
 * >0 wenn a als "grösser" gilt (z.B. Name kommt später im Alphabet).
 * <0 wenn b als "grösser" gilt.
 */
int person_compare(const person_t* a, const person_t* b);

/**
 * @brief Liest die Daten einer Person von der Standardeingabe ein.
 * Fordert den Benutzer zur Eingabe von Name, Vorname und Alter auf.
 * Validiert die Eingaben (Länge, Alter ist Zahl).
 * @param person [OUT] Zeiger auf die person_t Struktur, die gefüllt werden
 * soll.
 * @return 0 bei Erfolg, -1 bei ungültiger Eingabe oder Lesefehler.
 */
int person_read(person_t* person);

/**
 * @brief Gibt die Daten einer Person auf der Standardausgabe aus.
 * @param person [IN] const Zeiger auf die auszugebende Person.
 */
void person_print(const person_t* person);

#endif // PERSON_H