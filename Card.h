/*
 * $Header: Card.h,v 1.3 92/08/09 18:32:35 kirke Locked $
 *
 * xblackjack -- Card Interface
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
 *
 * $Log:	Card.h,v $
 * Revision 1.3  92/08/09  18:32:35  kirke
 * Replaced copyright notice (to include Silicon Graphics, Inc).
 * 
 * Revision 1.2  92/05/16  10:48:47  kirk
 * New copyright notice.
 * 
 * Revision 1.1  92/02/17  20:55:22  kirk
 * Inserted copyright notice.
 * 
 * Revision 1.0  92/01/30  22:01:20  kirk
 * Initial revision
 * 
 */

/*
 *  names
 */
#define ACE	0
#define TWO	1
#define THREE	2
#define FOUR	3
#define FIVE	4
#define SIX	5
#define SEVEN	6
#define EIGHT	7
#define NINE	8
#define TEN	9
#define JACK	10
#define QUEEN	11
#define KING	12

/*
 *  suits
 */
#define CLUBS		0
#define DIAMONDS	1
#define HEARTS		2
#define SPADES		3

typedef struct card {
        int index;	/* fresh deck position */
	int name;	/* ACE, TWO, ..., TEN, JACK, QUEEN, KING */
	int suit;	/* CLUBS, DIAMONDS, HEARTS, SPAPES */
} *cardPtr;

extern void init_cards();
extern void dump_cards();
extern void dump_card();
extern void shuffle();
extern struct card *next_card();
extern void shift_card();