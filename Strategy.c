/*
 * $Header: /usr/people/kirke/src/xblackjack/RCS/Strategy.c,v 1.12 1993/06/18 17:15:05 kirke Exp kirke $
 *
 * xblackjack -- Strategy tables logic
 *
 * From "Beat the Dealer" by Edward O. Thorp, Ph.D., Vintage Books (1966).
 * We use his "Complete Point-Count System" to make default selections.
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

/* $Log: Strategy.c,v $
 * Revision 1.12  1993/06/18  17:15:05  kirke
 * *** empty log message ***
 *
 * Revision 1.11  92/09/23  11:00:11  kirke
 * Added 2 messages to ManageSoftDialog().  This change contributed by
 * Anthony Daniel <tony@sequent.com>.
 * 
 * Fixed lint warnings.  Variable 'b' unused in ManageSplitDialog() and
 * ManageSplitDialog().
 * 
 * Revision 1.10  92/08/17  20:45:54  kirke
 * Removed rcsid header to compile on Sun cleanly.
 * 
 * Revision 1.9  92/08/12  23:34:33  kirke
 * OLIT path adjustments.
 * 
 * Revision 1.8  92/08/09  23:00:46  kirke
 * Changes for OLIT support from GUX (Guy Renard) grenard@elrond.stg.trw.com
 * 
 * Revision 1.7  92/08/09  18:32:26  kirke
 * Replaced copyright notice (to include Silicon Graphics, Inc).
 * 
 * Revision 1.6  92/07/06  01:28:13  kirk
 * *** empty log message ***
 * 
 * Revision 1.5  92/07/06  01:10:35  kirk
 * Setup separate what and rcstring for Main.c,
 * prefix from rcs strings.
 * 
 * Revision 1.4  92/07/04  21:27:44  kirk
 * Removed reference to rootWidget global, now passing widgets used through
 * strategy_init().
 * 
 * Revision 1.3  92/05/16  10:48:06  kirk
 * New copyright notice.
 * 
 * Revision 1.2  92/02/17  21:05:00  kirk
 * ManageSplitDialog() was dereferencing table entry incorrectly in building
 * "Split, since" message.
 * 
 * ManageSoftDialog() was dereferencing hard table rather than soft in building
 * "Always hit soft" message.
 * 
 * Made ManageDoubleDialog() print A,9 and 10 distinctly in Never,Always msgs.
 * 
 * Revision 1.1  92/01/30  21:54:38  kirk
 * Fixed 'Aways' typo.
 * 
 * Revision 1.0  92/01/04  21:16:14  kirk
 * Initial revision
 * 
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Motif2OLIT.h"
#ifdef OLIT
#include <Xol/OpenLook.h>
#include <Xol/StaticText.h>
#else
#include <X11/Xm/Xm.h>
#include <X11/Xm/Label.h>
#include "Table.h"
#endif
#include "Assert.h"
#include "Card.h"
#include "Choice.h"

extern char *rank_name();	/* from Draw.c */

static Widget SplitStrategyDialog;
static Widget DoubleStrategyDialog;
static Widget HardStrategyDialog;
static Widget SoftStrategyDialog;
static Widget SplitStrategyTable;
static Widget DoubleStrategyTable;
static Widget HardStrategyTable;
static Widget SoftStrategyTable;

static char *footnote[5] = {
	"split if your index is greater",
	"@split (8,8) on 10 only if index < 24",
	"%split (3,3) on 8 when index < -2 or > 6",
	"double down if your index is greater",
	"stand if your index is greater"
};

static Bool is_color;
static Pixel yes_fg, yes_bg, no_fg, no_bg;

static int bet_unit=1;
static int min_unit=1;

static int split[10][10] = {
	/*UP:	 --A----2----3----4----5----6----7----8----9---10--*/
	/*A,A*/ { -17,-999,-999,-999,-999,-999, -33, -24, -22, -20 }, 
	/*2,2*/	{ 999,  -9, -15, -22, -30,-999,-999, 999, 999, 999 },
	/*3,3*/	{ 999, -21, -34,-999,-999,-999,-999,   6, 999, 999 },
	/*4,4*/	{ 999, 999,  18,   8,   0,   5, 999, 999, 999, 999 },
	/*5,5*/	{ 999, 999, 999, 999, 999, 999, 999, 999, 999, 999 },
	/*6,6*/	{ 999,   0,  -3,  -8, -13, -16,  -8, 999, 999, 999 },
	/*7,7*/	{ 999, -22, -29, -35,-999,-999,-999,-999, 999, 999 },
	/*8,8*/	{ -18,-999,-999,-999,-999,-999,-999,-999,-999,  24 },
	/*9,9*/	{  10,  -3,  -8, -10, -15, -14,   8, -16, -22, 999 },
	/*10*/	{ 999,  25,  17,  10,   6,   7,  19, 999, 999, 999 }
};


static int double_down[3][10] = {
	/*UP:   --A----2----3----4----5----6----7----8----9---10--*/
	/*10*/ {   6, -15, -17, -21, -24, -26, -17,  -9,  -3,   7 },
	/*11*/ {  -3, -23, -26, -29, -33, -35, -26, -16, -10,  -9 },
        /*A9*/ { 999, 999,  20,  12,   8,   8, 999, 999, 999, 999 }
};


static int soft[2][10] = {
	/*UP:   --A----2----3----4----5----6----7----8----9---10--*/
	/*17*/ { 999, 999, 999, 999, 999, 999,  29, 999, 999, 999 },
	/*18*/ {  -6,-999,-999,-999,-999,-999,-999,-999, 999,  12 },
};


static int hard[6][10] = {
	/*UP:   --A----2----3----4----5----6----7----8----9---10--*/
	/*12*/ { 999,  14,   6,   2,  -1,   0, 999, 999, 999, 999 },
	/*13*/ { 999,   1,  -2,  -5,  -9,  -8,  50, 999, 999, 999 },
	/*14*/ { 999,  -5,  -8, -13, -17, -17,  20,  38, 999, 999 },
	/*15*/ {  16, -12, -17, -21, -26, -28,  13,  15,  12,   8 },
	/*16*/ {  14, -21, -25, -30, -34, -35,  10,  11,   6,   2 },
	/*17*/ { -15,-999,-999,-999,-999,-999,-999,-999,-999,-999 }
};

/*
**  Return True if player should split pair of 'name' against dealer's
**  'up' card with high-low 'index'.
*/
int
should_split(name, up, index)
	int name;
	int up;
	int index;
{
	if (name > TEN) name = TEN;
	if (up > TEN) up = TEN;
	if ((name == EIGHT) && (up == TEN)) {
		return(index < 24);
	}
	if ((name == THREE) && (up == EIGHT)) {           
		return(index > 6) || (index < -2);
	}
	assert((name >= 0) && (name <= 9));
	assert((up >= 0) && (up <= 9));
	return(index > split[name][up]);
}


/*
 *  A table exists for (A,2) (A,3) (A,4) (A,5) (A,6) (A,7) and (A,8),
 *  but Reno only allows doubling down on totals of 10 or 11.
 */
int
should_double(min, max, up, index)
	int min;
	int max;
	int up;
	int index; 
{
	if (up > TEN) up = TEN;
	assert((up >= 0) && (up <= 9));
	if (min != max) {				/* soft */
		if ((min == 10) && (max == 20)) {	/* A,9 */
			return(index > double_down[2][up]);
		}
		else {					/* BLACKJACK */
			return(False);			
		}
	}
	assert((max == 10) || (max == 11));
	if (max == 10) {
		return(index > double_down[0][up]);	/* 10 */
	}
	return(index > double_down[1][up]);		/* 11 */
}


int
should_stand(min, max, up, index)
	int min;
	int max;
	int up;
	int index;
{
	if (up > TEN) up = TEN;
	if (min != max) {		/* soft */
		if (max <= 16)
			return(False);
		if (max == 17)
			return(index > soft[0][up]);
		if (max == 18)
			return(index > soft[1][up]);
		else return(True);
	}
	if (max >= 18) return(True);
	if (max <= 11) return(False);
	max -= 12;
	assert((max >= 0) && (max <= 5));
	assert((up >= 0) && (up <= 9));
	return(index > hard[max][up]);
}


int
should_bet(bankroll, index)
	double bankroll;
	int index;
{
	if (index <= 2) {
		if (bankroll < bet_unit) {
		   return(bankroll);
		} else {
		   return(bet_unit);
		}
	}
	else {
		if (bankroll < index/2*bet_unit) {
			return(bankroll);
		}
		else {
			return(index/2*bet_unit);
		}
	}
}


int
Set_Bet_Unit(bankroll)
	double bankroll;
{

   if (bet_unit<bankroll/100) bet_unit=bankroll/100;
   if (bankroll<100) bet_unit=min_unit;
   if (bet_unit < min_unit) bet_unit = min_unit;

   return bet_unit;
}


int
should_insure(index)
	int index;
{
	return(index > 8);
}


CreateLabel(table, c, r, cs, rs, label, fg, bg, sensitive)
	Widget table;
	Position c, r;
	Dimension cs, rs;
	String label;
	Pixel bg;
	Boolean sensitive;
{
	int i;
	Widget w;
	Arg args[10];
	XmString s;
	char n[80];
	char starn[80];

	assert(table != NULL);

	/* build name using table name, column, and row */
	if (table == SplitStrategyTable)
		sprintf(n, "%s-%d-%d", "split", c, r);
	else if (table == DoubleStrategyTable)
		sprintf(n, "%s-%d-%d", "double", c, r);
	if (table == SoftStrategyTable)
		sprintf(n, "%s-%d-%d", "soft", c, r);
	if (table == HardStrategyTable)
		sprintf(n, "%s-%d-%d", "hard", c, r);
	sprintf(starn, "*%s", n);

	w = XtNameToWidget(table, n);
	if (w == NULL) {  /* create widgets first time through */
		i = 0;
#ifdef OLIT
		s = label;
#else
		s = XmStringCreate(label, XmSTRING_DEFAULT_CHARSET);
#endif
		XtSetArg(args[i], XmNsensitive, sensitive);	i++;
		XtSetArg(args[i], XmNlabelString, s);		i++;
		if (fg != -1) {
			XtSetArg(args[i], XmNforeground, fg); i++;
#ifdef OLIT
			XtSetArg(args[i], XtNfontColor, fg); i++;
#endif
		}
		if (bg != -1) {
			XtSetArg(args[i], XmNbackground, bg); i++;
		}
		w = XtCreateManagedWidget(n, xmLabelWidgetClass, table,args,i);
#ifdef OLIT
#define COLUMN_WIDTH 50
#define ROW_HEIGHT 20
		XtVaSetValues(w,
			      XtNx,c*COLUMN_WIDTH,
			      XtNy,r*ROW_HEIGHT,
			      NULL);
#else
		XmStringFree(s);
		assert(w != NULL);
		XtTblConfig(w, c, r, cs, rs, TBL_DEF_OPT);
#endif
	}
	else {  /* already created, just set values */
		i = 0;
		XtSetArg(args[i], XmNsensitive, sensitive);	i++;
		if (fg != -1) {
			XtSetArg(args[i], XmNforeground, fg); i++;
#ifdef OLIT
			XtSetArg(args[i], XtNfontColor, fg); i++;
#endif
		}
		if (bg != -1) {
			XtSetArg(args[i], XmNbackground, bg); i++;
		}
		XtSetValues(w, args, i);
	}
}


/*
**  Fill in the legend of a strategy Table.
*/
legend(w, up, str1, str2)
	Widget w;
	int up;
	char *str1;
	char *str2;
{
	/*	       c  r  cs rs 	 fg  bg  sensitive */
	CreateLabel(w, 0, 0, 1, 1, str1, -1, -1, True);
	CreateLabel(w, 1, 0,11, 1, "Dealer Shows", -1, -1, True);
	CreateLabel(w, 0, 1, 1, 1, str2, -1, -1, True);
	CreateLabel(w, 1, 1, 1, 1,  "2", -1, -1, up == TWO);
	CreateLabel(w, 2, 1, 1, 1,  "3", -1, -1, up == THREE);
	CreateLabel(w, 3, 1, 1, 1,  "4", -1, -1, up == FOUR);
	CreateLabel(w, 4, 1, 1, 1,  "5", -1, -1, up == FIVE);
	CreateLabel(w, 5, 1, 1, 1,  "6", -1, -1, up == SIX);
	CreateLabel(w, 6, 1, 1, 1,  "7", -1, -1, up == SEVEN);
	CreateLabel(w, 7, 1, 1, 1,  "8", -1, -1, up == EIGHT);
	CreateLabel(w, 8, 1, 1, 1,  "9", -1, -1, up == NINE);
	CreateLabel(w, 9, 1, 1, 1, "10", -1, -1, up == TEN);
	CreateLabel(w,10, 1, 1, 1,  "A", -1, -1, up == ACE);
}


/*
**  Bring up the Pair Splitting table
*/
void
ManageSplitDialog(name, up, index)
	int name;
	int up;
	int index;
{
	Widget w;
	char str[7];
	int i, j;
	Pixel fg, bg;

	w = SplitStrategyTable;
	if (up > TEN) up = TEN;
	legend(w, up, "your", "pair");
        CreateLabel(w, 0, 2, 1, 1,  "A,A", -1, -1, name==ACE);
        CreateLabel(w, 0, 3, 1, 1, "10,10", -1, -1, name==TEN);
        CreateLabel(w, 0, 4, 1, 1,  "9,9", -1, -1, name==NINE);
        CreateLabel(w, 0, 5, 1, 1,  "8,8", -1, -1, name==EIGHT);
        CreateLabel(w, 0, 6, 1, 1,  "7,7", -1, -1, name==SEVEN);
        CreateLabel(w, 0, 7, 1, 1,  "6,6", -1, -1, name==SIX);
        CreateLabel(w, 0, 8, 1, 1,  "5,5", -1, -1, name==FIVE);
        CreateLabel(w, 0, 9, 1, 1,  "4,4", -1, -1, name==FOUR);
        CreateLabel(w, 0,10, 1, 1,  "3,3", -1, -1, name==THREE);
        CreateLabel(w, 0,11, 1, 1,  "2,2", -1, -1, name==TWO);

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if ((split[i][j] == 999) || (split[i][j] == -999)) {
				strcpy(str,"  ");
			}
			else sprintf(str, "%d", split[i][j]); 
			fg = (index > split[i][j]) ? yes_fg : no_fg;
			bg = (index > split[i][j]) ? yes_bg : no_bg;
			if ((i == 7) && (j == 9)) { /* (8,8) on 10 */
				strcat(str, "@");
				fg = (index<24) ? yes_fg : no_fg;
				bg = (index<24) ? yes_bg : no_bg;
			}
			if ((i == 2) && (j == 7)) { /* (3,3) on 8 */
				if  (index < -2) {
					strcpy(str, "-2%");
					fg = yes_fg;
					bg = yes_bg;
				}
				else {
					strcat(str, "%");
					fg = (index>6) ? yes_fg : no_fg;
					bg = (index>6) ? yes_bg : no_bg;
				}
			}
			CreateLabel(w, (j==0) ? 10 : j, (i==0) ? 2 : 12-i,
					1, 1, str, fg, bg, True);
		}
	}

        CreateLabel(w, 0, 12, 11, 2, footnote[0], -1, -1, True);
        CreateLabel(w, 0, 14, 11, 1, footnote[1], -1, -1,
			(name==EIGHT && up==TEN));
        CreateLabel(w, 0, 15, 11, 1, footnote[2], -1, -1,
			(name==THREE && up==EIGHT));

#ifdef OLIT
	XtPopup(SplitStrategyDialog, XtGrabNone);
#else
	XtManageChild(SplitStrategyDialog);
#endif

	assert((name >= 0) && (name <= 9));
	assert((up >= 0) && (up <= 9));

	if ((name == EIGHT) && (up == TEN)) {
		if (index < 24) {
		   sprintf(str, "Split, since %d is less than 24");
		}
		else {
		   sprintf(str, "Don't split, since %d is not less than 24");
		}
		Msg(str);
		return;
	}
	if ((name == THREE) && (up == EIGHT)) {           
		if (index > 6) {
		   sprintf(str, "Split, since %d is greater than 6");
		}
		else if (index < -2) {
		   sprintf(str, "Split, since %d is less than -2");
		}
		else {
		   sprintf(str, "Don't split, since %d is between -2 and 6");
		}
	}
	else if (split[name][up] == -999) {
		sprintf(str, "Always split %ss on %s",
			rank_name(name), rank_name(up));
	}
	else if (split[name][up] == 999) {
		sprintf(str, "Never split %ss on %s",
			rank_name(name), rank_name(up));
	}
	else if (index > split[name][up]) {
		sprintf(str, "Split, since %d is greater than %d",
			index, split[name][up]);
	}
	else {
		sprintf(str, "Don't split, since %d is not greater than %d",
			index, split[name][up]);
	}
	Msg(str);
}

/*
**  Draw the Doubling Down table  (handles both soft and hard hands).
*/
void
ManageDoubleDialog(min, max, up, index)
	int min;
	int max;
	int up;
	int index;
{
	Widget w;
	char str[80];
	char nstr[20];
	int i, j;
	int r;
	
	if (up > TEN) up = TEN;
	w = DoubleStrategyTable;
	legend(w, up, "you", "have");
        CreateLabel(w, 0, 2, 1, 1,  "11", -1, -1, max == 11);
        CreateLabel(w, 0, 3, 1, 1,  "10", -1, -1, max == 10);
        CreateLabel(w, 0, 4, 1, 1,  "A,9", -1, -1, min != max);
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 10; j++) {
			if ((double_down[i][j] == 999)
			 || (double_down[i][j] == -999)) {
				strcpy(str," ");
			}
			else {
				sprintf(str, "%d", double_down[i][j]); 
			}
			if (i == 1) r = 2;		/* 11 */
			else if (i == 0) r = 3;		/* 10 */
			else r = 4;			/* A,9 */
			CreateLabel(w, (j==0) ? 10 : j, r,  1, 1, str,
				(index > double_down[i][j]) ? yes_fg : no_fg,
				(index > double_down[i][j]) ? yes_bg : no_bg,
				True);
		}
	}
        CreateLabel(w, 0, 5, 11, 2, footnote[3], -1, -1, True);

#ifdef OLIT
	XtPopup(DoubleStrategyDialog, XtGrabNone);
#else
	XtManageChild(DoubleStrategyDialog);
#endif

	if (min != max) {					/* soft */
		if ((min == 10) && (max == 20)) {
			i = 2;					/* A,9 */
		}
		else {
			Msg("Never double down with BLACKJACK.");
			return;
		}
	}
	else {
		assert((max == 10) || (max == 11));
		i = max-10;					/* 10,11 */
	}

	switch (i) {
	case	0:	strcpy(nstr, "10");	break;
	case	1:	strcpy(nstr, "11");	break;
	case	2:	strcpy(nstr, "A,9");	break;
	}
	if (double_down[i][up] == -999) {
		sprintf(str, "Always double down %s on %s",
				nstr, rank_name(up));
	}
	else if (double_down[i][up] == 999) {
		sprintf(str, "Never double down %s on %s",
				nstr, rank_name(up));
	}
	else if (index > double_down[i][up]) {
		sprintf(str, "Double down, since %d is greater than %d",
				index, double_down[i][up]);
	}
	else {
		sprintf(str,
			"Don't Double down, since %d is not greater than %d",
			index, double_down[i][up]);
	}
	Msg(str);
}

/*
**  Draw the Hit or Stand with Soft Hands table
*/
void
ManageSoftDialog(total, up, index)
	int total;
	int up;
	int index;
{
	Widget w;
	char str[80];
	int i, j;
	
	if (up > TEN) up = TEN;
	w = SoftStrategyTable;
	legend(w, up, "you", "have");
        CreateLabel(w, 0, 2, 1, 1,  "19+", -1, -1, total >= 19);
        CreateLabel(w, 0, 3, 1, 1,  "18", -1, -1, total == 18);
        CreateLabel(w, 0, 4, 1, 1,  "17", -1, -1, total == 17);
        CreateLabel(w, 0, 5, 1, 1,  "16-", -1, -1, total <= 16);

        CreateLabel(w, 1, 2, 10, 1,  "  ", no_fg, no_bg, True);
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 10; j++) {
			if ((soft[i][j] == 999) || (soft[i][j] == -999)) {
				strcpy(str," ");
			}
			else {
				sprintf(str, "%d ", soft[i][j]); 
			}
			CreateLabel(w, (j==0) ? 10 : j, 4-i,  1, 1, str,
				(index > soft[i][j]) ? no_fg : yes_fg,
				(index > soft[i][j]) ? no_bg : yes_bg,
				True);
		}
	}
        CreateLabel(w, 1, 5, 10, 1,  "  ", yes_fg, yes_bg, True);
        CreateLabel(w, 0, 6, 11, 2,  footnote[4], -1, -1, True);

#ifdef OLIT
	XtPopup(SoftStrategyDialog, XtGrabNone);
#else
	XtManageChild(SoftStrategyDialog);
#endif

	if (total <= 16) {
		sprintf(str, "Always hit soft %d.\n", total);
		Msg(str);
	}
	else if ((total == 17) || (total == 18)) {
		i = total-17;
		if (soft[i][up] == -999) {
			sprintf(str, "Always stand at soft %d on %s.\n", total,
				rank_name(up));
		}
		else if (soft[i][up] == 999) {
			sprintf(str, "Always hit soft %d on %s.\n", total,
					rank_name(up));
		}
		else if (index > soft[i][up]) {
			sprintf(str, "Stand, since %d is greater than %d\n",
				index, soft[i][up]);
		}
		else {
			sprintf(str, "Hit, since %d is not greater than %d\n",
				index, soft[i][up]);
		}
		Msg(str);
	}
	else {
		Msg("Always stand at soft 19 or more.");
	}
}

/*
**  Draw the Hit or Stand with Hard Hands table
*/
void
ManageHardDialog(total, up, index)
	int total;
	int up;
	int index;
{
	char str[80];
	int i, j;
	Widget w;
	int tvalue;

	if (up > TEN) up = TEN;

	w = HardStrategyTable;

	legend(w, up, "you", "have");
        CreateLabel(w, 0, 2, 1, 1,  "18+", -1, -1, total >= 18);
        CreateLabel(w, 0, 3, 1, 1,  "17", -1, -1, total == 17);
        CreateLabel(w, 0, 4, 1, 1,  "16", -1, -1, total == 16);
        CreateLabel(w, 0, 5, 1, 1,  "15", -1, -1, total == 15);
        CreateLabel(w, 0, 6, 1, 1,  "14", -1, -1, total == 14);
        CreateLabel(w, 0, 7, 1, 1,  "13", -1, -1, total == 13);
        CreateLabel(w, 0, 8, 1, 1,  "12", -1, -1, total == 12);
        CreateLabel(w, 0, 9, 1, 1,  "11-", -1, -1, total <= 11);

        CreateLabel(w, 1, 2, 10, 1,  "  ", no_fg, no_bg, True);
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 10; j++) {
			if ((hard[i][j] == 999) || (hard[i][j] == -999)) {
				strcpy(str,"  ");
			}
			else sprintf(str, "%3d", hard[i][j]); 
			CreateLabel(w, (j==0) ? 10 : j, 8-i,  1, 1, str,
				(index > hard[i][j]) ? no_fg : yes_fg,
				(index > hard[i][j]) ? no_bg : yes_bg,
				True);
		}
	}
        CreateLabel(w, 1, 9, 10, 1,  "  ", yes_fg, yes_bg, True);
        CreateLabel(w, 0, 10, 11, 2,  footnote[4], -1, -1, True);

#ifdef OLIT
	XtPopup(HardStrategyDialog, XtGrabNone);
#else
	XtManageChild(HardStrategyDialog);
#endif

	i = total - 12;
	if (total >= 18) {
		Msg("Always stand with hard 18 or more.");
	}
	else if (total <= 11) {
		Msg("Always hit 11 or less.");
	}
	else if (hard[i][up] == -999) {
		sprintf(str, "Always stand at hard %d on %s.\n", total,
				rank_name(up));
		Msg(str);
	}
	else if (hard[i][up] == 999) {
		sprintf(str, "Always hit hard %d on %s.\n", total,
				rank_name(up));
		Msg(str);
	}
	else if (index > hard[i][up]) {
		sprintf(str, "Stand, since %d is greater than %d\n",
			index, hard[i][up]);
		Msg(str);
	}
	else {
		sprintf(str, "Hit, since %d is not greater than %d\n",
			index, hard[i][up]);
		Msg(str);
	}
}

/*
**  Initialize globals
*/
void
strategy_init(dpy, st, sd, dt, dd, ft, fd, ht, hd, bu)
	Display *dpy;
	Widget st, sd, dt, dd, ft, fd, ht, hd;
	int bu;
{
	extern unsigned long   greenpixel;	/* from Draw.c */
	extern unsigned long   redpixel;	/* from Draw.c */
	extern unsigned long   blackpixel;	/* from Draw.c */
	extern unsigned long   whitepixel;	/* from Draw.c */

	SplitStrategyTable = st;  SplitStrategyDialog = sd;
	DoubleStrategyTable = dt;  DoubleStrategyDialog = dd;
	SoftStrategyTable = ft;  SoftStrategyDialog = fd;
	HardStrategyTable = ht;  HardStrategyDialog = hd;

	bet_unit = bu;
	min_unit = bu;

	is_color = (DisplayCells(dpy, XDefaultScreen(dpy)) > 2);
	if (is_color) {
		yes_fg = blackpixel;
		yes_bg = greenpixel;

		no_fg = whitepixel;
		no_bg = redpixel;
	}
	else {
		yes_fg = whitepixel;
		yes_bg = blackpixel;

		no_fg = blackpixel;
		no_bg = whitepixel;
	}
}
