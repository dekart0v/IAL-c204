
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar. (enthesis)
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    // операторы удаленные из стэка записываестя до postfixExpression
    // Длинна выражения и указатель на первое свободное место хранит postfixExpressionLength
    // можно использовать одну char переменную
    char *top = malloc(sizeof(char));
    if (Stack_IsEmpty(stack) == 0) Stack_Top(stack, top);

    while (*top != '(') {
        postfixExpression[*postfixExpressionLength] = *top;
        postfixExpressionLength[0]++;
        Stack_Pop(stack);
        Stack_Top(stack, top);
    }
    Stack_Pop(stack);
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
  // обрабатывает оператор который хранится в char c после считывания знака на input-ном массиве знаков
  char *top = malloc(sizeof(char));
  if(Stack_IsEmpty(stack) == 0) {
    Stack_Top(stack, top);
  }
  if (c >= 65 || (c >= 48 && c <= 57)) {
    postfixExpression[*postfixExpressionLength] = c;
    postfixExpressionLength[0]++;
  }
  else {
    switch (c) {
      case '+':
      case '-':
        while (1) {
          if(Stack_IsEmpty(stack) || (*top != '+' && *top != '-' && *top != '*' && *top != '/' && *top != ')' && *top != '(') || *top == '(') {
            Stack_Push(stack, c);
            break;
          }
          else {
            postfixExpression[*postfixExpressionLength] = *top;
            postfixExpressionLength[0]++;
            Stack_Pop(stack);
          }
          if (Stack_IsEmpty(stack) == 0) Stack_Top(stack, top);
        }
        break;
      case '/':
      case '*':
        while (1) {
          if((Stack_IsEmpty(stack)) || *top == '(' || *top == '+' || *top == '-') {
            Stack_Push(stack, c);
            break;
          }
          else {
            postfixExpression[*postfixExpressionLength] = *top;
            postfixExpressionLength[0]++;
            Stack_Pop(stack);
          }
          if (Stack_IsEmpty(stack) == 0) Stack_Top(stack, top);
        }
        break;
      case '(':
        Stack_Push(stack, c);
        break;
      case ')':
        untilLeftPar(stack, postfixExpression, postfixExpressionLength);
        break;
      case '=':
        while (Stack_IsEmpty(stack) == 0) {
          postfixExpression[*postfixExpressionLength] = *top;
          postfixExpressionLength[0]++;
          Stack_Pop(stack);
          if (Stack_IsEmpty(stack) != 0) break;
          Stack_Top(stack, top);
        }
        postfixExpression[*postfixExpressionLength] = c;
        postfixExpressionLength[0]++;
        postfixExpression[*postfixExpressionLength] = '\0';
        break;
    }
  }

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    char *postfixExpression = malloc(sizeof(char) * MAX_LEN);

    Stack *stack = malloc(sizeof(Stack));
    unsigned* postfixExpressionLength = malloc(sizeof(unsigned) * 1);
    *postfixExpressionLength = 0;
    Stack_Init(stack);

    for (int i = 0; infixExpression[i] != '\0'; i++) {
        doOperation(stack, infixExpression[i], postfixExpression, postfixExpressionLength);
    }
    //solved = FALSE; /* V případě řešení smažte tento řádek! */
    return postfixExpression; /* V případě řešení můžete smazat tento řádek. */
}

/* Konec c204.c */
