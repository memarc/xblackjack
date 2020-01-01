/*
 * $Header: /usr/people/kirke/src/xblackjack/RCS/Card.c,v 1.11 1993/06/18 17:15:05 kirke Exp kirke $
 *
 * xblackjack -- Card management routines.
 *
 * (c) Copyright 1992, Kirk Erickson and Silicon Graphics Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Kirk Erickson and
 * Silicon Graphics Inc. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  Kirk Erickson and Silicon Graphics Inc. make no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE ABOVE-NAMED DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE ABOVE-NAMED BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/* $Log: Card.c,v $
 * Revision 1.11  1993/06/18  17:15:05  kirke
 * *** empty log message ***
 *
 * Revision 1.10  92/09/23  10:57:01  kirke
 * Fixed lint errors.  Statics suit_char and name_char only needed under DEBUG.
 * 
 * Revision 1.9  92/08/17  20:45:05  kirke
 * Removed rcsid header to compile on Sun cleanly.
 * 
 * Revision 1.8  92/08/09  18:31:22  kirke
 * Replaced copyright notice (to include Silicon Graphics, Inc).
 * 
 * Revision 1.7  92/07/06  01:27:53  kirk
 * *** empty log message ***
 * 
 * Revision 1.6  92/07/06  01:09:21  kirk
 * Setup separate what and rcstring for Main.c,
 * removed prefix from rcs strings.
 * 
 * Revision 1.3  92/05/16  10:45:02  kirk
 * New copyright notice.
 * 
 * Revision 1.2  92/02/17  20:58:23  kirk
 * Made init_cards() and dump_cards() write to stderr.
 * 
 * Removed shift_card().  Main.c:shuffle_up() now relies on 'unplayed_cards'
 * to keep from dealing cards left on the table.
 * 
 * Added assert() to next_card().
 * 
 * Revision 1.1  92/01/30  21:42:35  kirk
 * Simplified technique for "fixing the deck" (special hands for debugging).
 * 
 * Revision 1.0  92/01/04  21:13:32  kirk
 * Initial revision
 * 
 */

#include <stdio.h>
#include "Card.h"
#include "Assert.h"

extern char *malloc();

static struct card *first_card;
static struct card *last_card;
static struct card *top_card;

#ifdef DEBUG
static char suit_char[4] = { 'C', 'D', 'H', 'S' };
static char name_char[13] = {
	'A', '2', '3', '4', '5', '6', '7', '8', '9', '1', 'J', 'Q', 'K'
};
#endif

/*
**  Allocate 'total' cards, and give them "fresh deck" values.
*/
void
init_cards(total_decks, total_cards, total_fives, total_tens, total_others)
	int total_decks;
	int *total_cards;
	int *total_fives;
	int *total_tens;
	int *total_others;
{
	int i;
	int total_bytes;
	struct card *c;

	/* seed is seconds elapsed since 00:00:00 GMT 1/1/70. */
	srand(time(NULL));  /* for shuffling */
	
	*total_cards = 52*total_decks;
	total_bytes = (*total_cards)*sizeof(struct card);
	first_card = (struct card *) malloc(total_bytes);
	if (first_card == NULL) {
		fprintf(stderr, "Unable to allocate cards (%d bytes)\n",
			total_bytes);
		return;
	}
	last_card = first_card + *total_cards - 1;
	top_card = first_card;

	*total_fives = 0;
	*total_tens = 0;
	*total_others = 0;

	/* initialize fields of each card structure */
	i = 0;
	for (c = first_card; c <= last_card; c++) {
		if (i % 52 < 13) c->suit = CLUBS;
		else if (i % 52 < 26) c->suit = DIAMONDS;
		else if (i % 52 < 39) c->suit = HEARTS;
		else c->suit = SPADES;
		c->name = (i % 52) - (c->suit * 13);
		c->index = i;
		i++;
		if (c->name == FIVE) (*total_fives)++;
		if (c->name >= TEN) (*total_tens)++;
		else (*total_others)++;
	}
}


#ifdef DEBUG
void
dump_cards()
{
	struct card *c;
	int i;

	i = 0;
	for (c = first_card; c <= last_card; c++) {
		if (i++ % 13 == 0) {
			if (i == 0) {
				fprintf(stderr, "\n");
			}
			fprintf(stderr, "\ndump_cards\t");
		}
		else if (i > 1) {
			fprintf(stderr, " ");
		}
		fprintf(stderr, "%c-%c", name_char[c->name],
			suit_char[c->suit]);
	}
	fprintf(stderr, "\n");
}

void
dump_card(c)
	struct card *c;
{
	fprintf(stderr, " %c-%c", name_char[c->name], suit_char[c->suit]);
}
#endif


/*
**  Comparison routine for quick sort.
*/
int
icmp(l, r)
	struct card *l;
	struct card *r;
{	
	int retval = (l->index - r->index);
	return retval;
}	


/*
**  Shuffle by assigning random numbers to each cards 'index' and sorting.
**  When shuffling during a game, 'first_in_play" will be non-NULL,
**  and only the cards up till this one are shuffled.
*/
void
shuffle(first_in_play)
	struct card *first_in_play;
{
	int i;
	struct card *c;
	struct card *end_card;
	
	if (first_in_play == NULL) {
		end_card = last_card;
	}
	else {
		end_card = --first_in_play;
	}
	
	/* assign each card a random position in deck */
	i = 0;
	for (c = first_card; c <= end_card; c++) {
		c->index = rand();
		i++;
	}

	/* sort the array into new order */
	qsort(first_card, i, sizeof(struct card), icmp);	

	/* normalize index values */
	i = 0;
	for (c = first_card; c <= end_card; c++) {
		c->index = ++i;
	}

	top_card = first_card;


/*
**  The following force special hands for debugging.
**  We will bury the top card later, so first player card is second.
*/
#if 0
/* Lots of cards for resize testing */
	(++top_card)->name = TWO;
	(++top_card)->name = TWO;
	(++top_card)->name = TWO;
	(++top_card)->name = TWO;
	(++top_card)->name = ACE;
	(++top_card)->name = ACE;
	(++top_card)->name = ACE;
	(++top_card)->name = ACE;
	(++top_card)->name = THREE;
	(++top_card)->name = THREE;
	(++top_card)->name = THREE;
	(++top_card)->name = THREE;
#endif

#if 0
/* A,9 soft double down */
	(++top_card)->name = ACE;
	(++top_card)->name = TWO;
	(++top_card)->name = NINE;
#endif

#if 0
/* Insuance checker:	Dealer BLACKJACK */
	(++top_card)->name = ACE;
	(++top_card)->name = ACE;
	(++top_card)->name = FOUR;
	(++top_card)->name = TEN;
#endif

#if 0
/* Insuance checker:	Player BLACKJACK */
	(++top_card)->name = ACE;
	(++top_card)->name = ACE;
	(++top_card)->name = TEN;
	(++top_card)->name = FOUR;
#endif

#if 0
/* Insuance checker:	Player wins */
	(++top_card)->name = TEN;
	(++top_card)->name = ACE;
	(++top_card)->name = TEN;
	(++top_card)->name = FOUR;
#endif

#if 0
/* Insurance checker:	BLACKJACKs push */
	(++top_card)->name = ACE;
	(++top_card)->name = ACE;
	(++top_card)->name = TEN;
	(++top_card)->name = TEN;
#endif

#if 0
/* (8,8) three times */
	(++top_card)->name = EIGHT;
	(++top_card)->name = NINE;
	(++top_card)->name = EIGHT;
	(++top_card)->name = TEN;
	(++top_card)->name = EIGHT;
	(++top_card)->name = TEN;
	(++top_card)->name = EIGHT;
	(++top_card)->name = TEN;
	(++top_card)->name = TEN;
	(++top_card)->name = TEN;
	(++top_card)->name = TEN;
#endif

#if 0
/* (A,A) three times */
	(++top_card)->name = ACE;
	(++top_card)->name = SIX;
	(++top_card)->name = ACE;
	(++top_card)->name = TEN;
	(++top_card)->name = ACE;
	(++top_card)->name = TWO;
	(++top_card)->name = ACE;
	(++top_card)->name = THREE;
	(++top_card)->name = FOUR;
	(++top_card)->name = FIVE;
	(++top_card)->name = SIX;
#endif

#if 0
/* (8,8) against 17 */
	(++top_card)->name = EIGHT;
	(++top_card)->name = TEN;
	(++top_card)->name = EIGHT;
	(++top_card)->name = SIX;
	(++top_card)->name = TWO;	/* double on 10 */
	(++top_card)->name = THREE;	/* or 11 too */
#endif

#if 0
/* (8,8) twice, dealer busts */
	(++top_card)->name = EIGHT;
	(++top_card)->name = TEN;
	(++top_card)->name = EIGHT;
	(++top_card)->name = SIX;
	(++top_card)->name = EIGHT;
	(++top_card)->name = SIX;	/* First */
	(++top_card)->name = SIX;	/* First */
	(++top_card)->name = SIX;	/* Second */
	(++top_card)->name = NINE;	/* Second */
	(++top_card)->name = SIX;	/* Third */
	(++top_card)->name = SIX;	/* Third */
	(++top_card)->name = SIX;	/* Dealer */

#endif

#if 0
/* (7,7) against 10 */
	(++top_card)->name = SEVEN;
	(++top_card)->name = TEN;
	(++top_card)->name = SEVEN;
#endif

#if 0
/* (3,3) against 8 */
	(++top_card)->name = THREE;
	(++top_card)->name = EIGHT;
	(++top_card)->name = THREE;
#endif

#if 0
/* (A,6) against 10 */
	(++top_card)->name = ACE;
	(++top_card)->name = QUEEN;
	(++top_card)->name = SIX;
#endif

#if 0
/* pair of fives (hard, split, and double tables) */
	(++top_card)->name = FIVE;
	(++top_card)->name = EIGHT;
	(++top_card)->name = FIVE;
#endif

#if 0
/* Dealer soft 17 */
	(++top_card)->name = TEN;
	(++top_card)->name = SIX;
	(++top_card)->name = TEN;
	(++top_card)->name = ACE;
#endif

	top_card = first_card;

#ifdef DEBUG
	dump_cards();
#endif
}

struct card *
next_card()
{	
	assert(top_card <= last_card);
	if (top_card > last_card) {
		return(NULL);
	}
	else {
		return(top_card++);
	}
}

