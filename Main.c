/*
 * $Header: Main.c,v 1.19 92/09/24 11:03:08 kirke Locked $
 *
 * xblackjack -- The game of Blackjack
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

/* $Log:	Main.c,v $
 * Revision 1.19  92/09/24  11:03:08  kirke
 * Reverted back to original main() arg declaration sequence.  Had attempted
 * to alleviate warning compiling under IRIX, but then failed on the Sun.
 * 
 * Revision 1.18  92/09/23  10:59:03  kirke
 * Fixed  CreateMotifWidgets() to Add okCallback (OkPayoffDialogCB) (pair
 * splitting was broken in versions 1.4 and 2.0).  This change was posted
 * to export as xblackjack-2.0.patch1.
 * 
 * The behavior of the scale widget under OLIT can be annoying, as Dev
 * Joneja (dj@shiva.gsb.columbia.edu found).  Under Motif, clicking on the
 * slider places the bet at its current value, whereas under OLIT the slider
 * can move.  To avoid this anomally, a "deal" pushbutton was introduced
 * for OLIT users to press rather than taking their chances with the slider.
 * Called xblackjack-2.0.patch2, this change was contributed by Guy Renard
 * (grenard@elrond.STG.TRW.COM).
 * 
 * Made OkPayoffDialogCB() declaration return void, matched use previous
 * to declaration in DoPayoffSplitDialog() which references MakeBet() too
 * (one or the other needed a forward declaration).
 * 
 * Fixes resulting from lint run.  Extraneous vars in deal_card(),
 * VersionCB(), OkPayoffDialogCB(), finish_hand(), YourBetScaleCB(),
 * DoubleCB(), CreateMotifWidgets(), and main().
 * 
 * Adjusted argument definitions for argc of main() to avoid warnings under
 * IRIX (Used Doug Young's XlibSpecificationRelease>=5 conditional).  We
 * want unsigned int up to X11R5, at which point we want int.
 * 
 * Alain Brossard (brossard@siisun.epfl.ch) caught an XtAddCallback in
 * AddCallbacks() that needed an additional NULL argument (client_data) to
 * compile using gcc on a sun4.
 * 
 * OLIT specific changes from Guy (grenard@elrond.STG.TRW.COM) to improve
 * bet scale behavior:   Now a person can see what they are about to bet
 * before releasing the select button.  I think the deal button is still
 * useful since There is still no way to simply click on the slider and
 * get the deafult bet.
 * 
 * Revision 1.17  92/08/19  20:54:04  kirke
 * Fixed  CreateMotifWidgets() to Add okCallback (OkPayoffDialogCB)
 * (pair splitting was broken in versions 1.4 and 2.0).
 * 
 * Revision 1.16  92/08/17  20:45:52  kirke
 * Removed rcsid header to compile on Sun cleanly.
 * 
 * Revision 1.15  92/08/15  16:59:26  kirke
 * Reverted back to "int" rather than "unsigned int" for argc.  The change
 * removed a warning under Irix, but Motif 1.2 and the new complrs will
 * expect int.
 * 
 * Revision 1.14  92/08/12  23:34:21  kirke
 * OLIT path adjustments.
 * 
 * Revision 1.13  92/08/09  23:00:37  kirke
 * Changes for OLIT support from GUX (Guy Renard) grenard@elrond.stg.trw.com
 * 
 * Revision 1.12  92/08/09  18:32:24  kirke
 * Replaced copyright notice (to include Silicon Graphics, Inc).
 * 
 * Revision 1.11  92/08/09  17:29:27  kirk
 * Made Msg button cycle through strategy tables for valid choices.
 * Removed all translations for MIDDLE and RIGHT mouse buttons; now relying
 * on LEFT button clicks on Msg pushbutton and Autoplay scale.
 * 
 * Fixed "clicking next to YourBetScale slider" runaway problem.
 * (worked around Motif scrollbar bug, by not setting insensitive in CB)
 * 
 * Revision 1.9  92/07/06  01:10:33  kirk
 * Setup separate what and rcstring for Main.c,
 * prefix from rcs strings.
 * 
 * Revision 1.8  92/07/06  00:33:37  kirk
 * Made Msg button cycle through strategy tables for valid choices.
 * Removed all translations for MIDDLE and RIGHT mouse buttons; now relying
 * on LEFT button clicks on Msg pushbutton and Autoplay scale.
 * 
 * Fixed "clicking next to YourBetScale slider" runaway problem.
 * (worked around Motif scrollbar bug, by not setting insensitive in CB)
 * 
 * Revision 1.7  92/07/05  12:32:15  kirk
 * Added dragCallback to autoplay slider.
 * Changed initial Msg.
 * 
 * Revision 1.6  92/07/04  21:27:10  kirk
 * Now unmanaging hint dialogs before managing a new one, so only one is
 * visible at a time (MIDDLE on HIT-SPLIT-HIT left HIT table below).
 * 
 * Removed Wcl code -- added CreateWidgets() to Main.c.
 * 
 * Removed XtVa calls (to avoid malloc/performance hit).
 * 
 * Removed most XtNameToWidget calls.
 * 
 * Added AutoplayScale, for control over speed.
 * 
 * Made StatusLabel into a PushButton (click for HintDialog).
 * 
 * Revision 1.5  92/05/19  07:01:48  kirk
 * Previous rev should have included:
 * Now unmanaging hint dialogs before managing a new one, so only one is
 * visible at a time (MIDDLE on HIT-SPLIT-HIT left HIT table below).
 * 
 * Removed xmpTableWidgetClass reference to build under Wcl 2.01.
 * 
 * Revision 1.4  92/05/16  10:48:04  kirk
 * New copyright notice.
 * 
 * Revision 1.3  92/02/17  21:03:00  kirk
 * Made YesInsuranceCB() payoff insurance bet rather than complicating pay_off()
 * with adjusting the bankroll for the insurance bet too.  Wasn't paying off
 * insurance in blackjacks push case.
 * 
 * deal_card() wasn't shifting the last card of each hand left on the table.
 * (After partial shuffle).
 * 
 * Eliminated (faceup, hole_shown) arguments to deal_card(), now using
 * hole_shown global (was HoleUnseen).
 * 
 * Handling partial shuffle differently;  no longer shifting (exchanging) cards.
 * Leaving cards on the table in same array positions (at the end of the deck).
 * After face_up() reaccounts for each of the unshuffled cards, unplayed_cards
 * has been decremented -- so we won't deal them again.
 * 
 * Removed logging facility.
 * 
 * Wasn't resetting cards[] after partial shuffle in autoplay.  Now picking
 * up cards in YourBetScaleCB() to cover this case.
 * 
 * Revampled VersionCB mechanism;  now bumping in Imakefile.
 * 
 * Revision 1.2  92/02/01  10:15:45  kirk
 * AutoplayTimeout() is no longer reseting timer when autoplay=False.
 * (After spending bankroll, autoplay wasn't stopping for MakeBet().)
 * 
 * Revision 1.1  92/01/30  21:50:00  kirk
 * Changes to compile and run under RISC/os 5.0 with RISCwindows4.10.
 * 
 * Fixed autoplay by setting 'busy' earlier in AutoplayTimeout() choice case
 * (under 5.0 busy was being cleared by StandCB() or HitCB() before being set).
 * 
 * Fixed MakeBet() to just place a buck bet when bankroll < 2.0 to avoid
 * setting scale min=max=1.
 * 
 * Broke the drawing area in two, and added label to show high-low index
 * calculation in card area rather than in msg label or strategy dialogs.
 * 
 * Made ManageHintACT() wait for autoplay to stop before managing dialog.
 * 
 * Fixed a bug in pay_off() to pay off insurance push (was loss).
 * 
 * Now picking up cards in ShuffleDialogCB (after dialog gets focus).
 * 
 * Revision 1.0  92/01/04  21:16:12  kirk
 * Initial revision
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include "Motif2OLIT.h"
#ifdef OLIT
#include <Xol/OpenLook.h>
#include <Xol/RubberTile.h>
#include <Xol/MenuButton.h>
#include <Xol/DrawArea.h>
#include <Xol/BulletinBo.h>
#include <Xol/StaticText.h>
#include <Xol/ControlAre.h>
#include <Xol/Notice.h>
#include <Xol/OblongButt.h>
#include <Xol/Slider.h>
#include <Xol/ScrolledWi.h>
#include <Xol/TextEdit.h>
#include <Xol/PopupWindo.h>
#else
#include <sys/types.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/DrawingA.h>
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/PanedW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/ScrolledW.h>
#include <Xm/SashP.h>
#include <Xm/Text.h>
#include "Table.h"
#endif /* OLIT */

#include "PatchLevel.h"
#include "Assert.h"
#include "Icon.h"
#include "Card.h"
#include "Choice.h"
#include "Strategy.h"

#define TIMEOUT		50	/* in milliseconds for autoplay */
#define SHUFFLE_POINT1	6	/* if this or fewer before the deal */
#define SHUFFLE_POINT2	0	/* shuffle with cards on the table */
#define MAX_HANDS	5	/* dealer, original and 3 possible splits */
#define MAX_CARDS	11	/* 4 Aces, 4 duces, and 3 threes! */
#define DEALER		0

#define BLACKJACK(h)    (((h == 0) || (h == 1)) \
			&& (cards[h] == 2) && (max_total(h) == 21))

#define MAKING_BET_STATE	1
#define MAKING_CHOICE_STATE	2
#define BUYING_INSURANCE_STATE  3
static int state;

static Arg args[10];
static int timeout = TIMEOUT;

Widget rootWidget;
#ifndef OLIT
static Widget PanedWindow;
#endif
static Widget MainWindow;
static Widget MainTable;
#ifdef OLIT
static Widget DrawAreas;
#endif
static Widget DealerDrawingArea;
static Widget PlayerDrawingArea;
static Widget HintMsgLabel;
static Widget StatusPushButton;
static Widget MenuBar;
static Widget FileCascadeButton;
static Widget FilePulldownMenu;
static Widget QuitCascadeButton;
static Widget HelpCascadeButton;
static Widget HelpPulldownMenu;
static Widget HelpGeneralDialog;
#ifndef OLIT
static Widget HelpGeneralDismissPushButton;
#endif
static Widget HelpGeneralPushButton;
static Widget HelpGeneralScrolledText;
static Widget HelpGeneralTable;
static Widget HelpRulesDialog;
#ifndef OLIT
static Widget HelpRulesDismissPushButton;
#endif
static Widget HelpRulesPushButton;
static Widget HelpRulesScrolledText;
static Widget HelpRulesTable;
static Widget HelpBasicStrategyDialog;
#ifndef OLIT
static Widget HelpBasicStrategyDismissPushButton;
#endif
static Widget HelpBasicStrategyPushButton;
static Widget HelpBasicStrategyScrolledText;
static Widget HelpBasicStrategyTable;
static Widget HelpCountingDialog;
#ifndef OLIT
static Widget HelpCountingDismissPushButton;
#endif
static Widget HelpCountingPushButton;
static Widget HelpCountingScrolledText;
static Widget HelpCountingTable;
static Widget HelpBestStrategyDialog;
#ifndef OLIT
static Widget HelpBestStrategyDismissPushButton;
#endif
static Widget HelpBestStrategyPushButton;
static Widget HelpBestStrategyScrolledText;
static Widget HelpBestStrategyTable;
static Widget HelpVersionPushButton;
static Widget InsuranceHelpDialog;
#ifndef OLIT
static Widget InsuranceHelpDismissPushButton;
#endif
static Widget InsuranceHelpScrolledText;
static Widget InsuranceHelpTable;
static Widget InsuranceQuestionDialog;
#ifdef OLIT
static Widget InsuranceQuestionDialogT;
static Widget InsuranceQuestionDialogY;
static Widget InsuranceQuestionDialogN;
static Widget InsuranceQuestionDialogH;
#endif
static Widget PlaceHolderLabel;
static Widget ControlTable;
static Widget WonLabel;
static Widget WonLabelValue;
static Widget WonLabelPercent;
static Widget LostLabel;
static Widget LostLabelValue;
static Widget LostLabelPercent;
static Widget PushedLabel;
static Widget PushedLabelValue;
static Widget PushedLabelPercent;
static Widget YourBetScale;
#ifdef OLIT
static Widget YourBetScaleValue;
static Widget YourBetScaleTitle;
static Widget DealPushButton;
#endif
static Widget AutoplayScale;
static Widget YouHaveLabel;
static Widget YouHaveLabelValue;
static Widget SplitPushButton;
static Widget DoublePushButton;
static Widget HitPushButton;
static Widget StandPushButton;
static Widget SplitStrategyDialog;
static Widget DoubleStrategyDialog;
static Widget HardStrategyDialog;
static Widget SoftStrategyDialog;
static Widget SplitStrategyTable;
static Widget DoubleStrategyTable;
static Widget HardStrategyTable;
static Widget SoftStrategyTable;
static Widget ShuffleDialog;
#ifdef OLIT
static Widget ShuffleDialogT;
static Widget ShuffleDialogOK;
#endif
static Widget PayoffSplitDialog;
#ifdef OLIT
static Widget PayoffSplitDialogT;
static Widget PayoffSplitDialogOK;
#endif

static XtAppContext app;
static int autoplay;

/* Add Bet unit and bankroll resources */
#define XtNbetUnit "betUnit"
#define XtCbetUnit "betUnit"
#define XtNadjustBetUnit "adjustBetUnit"
#define XtCadjustBetUnit "adjustBetUnit"
#define XtNbankroll "bankroll"
#define XtCbankroll "bankroll"
#define XtNdecks "decks"
#define XtCdecks "decks"

typedef struct {
  int bankroll;
  int bet_unit;
  Boolean adjust_bet_unit;
  int decks;
  } RValues, *RVptr;

RValues rv;

/* Default resource values */
XtResource resources[] = {
   { XtNbankroll, XtCbankroll, XtRInt, sizeof(int), 
     XtOffset(RVptr, bankroll), XtRImmediate, (XtPointer)100 },
   { XtNbetUnit, XtCbetUnit, XtRInt, sizeof(int), 
     XtOffset(RVptr, bet_unit), XtRImmediate, (XtPointer)1 },
   { XtNadjustBetUnit, XtCadjustBetUnit, XtRBoolean, sizeof(Boolean), 
     XtOffset(RVptr, adjust_bet_unit), XtRImmediate, (XtPointer)False },
   { XtNdecks, XtCdecks, XtRInt, sizeof(int), 
     XtOffset(RVptr, decks), XtRImmediate, (XtPointer)1 }
   };

/* card point values:	     	A  2  3  4  5  6  7  8  9  10   J   Q   K */
static int card_value[13] =  {  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };
static int count_value[13] = { -1, 1, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1 };

static char *hand_name[MAX_HANDS] = {
	"Dealer", "First", "Second", "Third", "Fourth"
};

static int total_cards;
static int total_fives;
static int total_tens;
static int total_others;
static double bankroll = 0;
static double max_bankroll = 0;
static double bet[MAX_HANDS];
static int cards[MAX_HANDS];			/* total cards in a hand */
static struct card *c[MAX_HANDS][MAX_CARDS];
static int simple_count;
static int highlow_index;
static int insured;
static int unplayed_cards;
static int unseen_cards;
static int unseen_fives;
static int unseen_tens;
static int unseen_others;
static unsigned long decks_played;
static unsigned long hands_played;
static unsigned long hands_won;
static unsigned long hands_lost;
static unsigned long hands_pushed;
static int percent_won;
static int percent_lost;
static int percent_pushed;

static int choices;		/* these 2 are set by MakeChoice() */
static int default_choice;	/* and used by ManageHintDialogCB() */
static int current_hand;
static int next_hand;		/* for pair splitting */
static Bool hole_seen;		/* is dealer's down card face up */
static XtIntervalId timer;	/* for auto play */
static Boolean busy = False;	/* ignore auto play timeouts */
static Boolean pickup_cards;	/* for ShuffleDialogCB() */

/*
    Kludge to work around the fact I can't find a function to tell if the
    OLIT popup windows are popped up or not.
*/
#ifdef OLIT
static Boolean SplitDialogIsUp=False;
static Boolean DoubleDialogIsUp=False;
static Boolean SoftDialogIsUp=False;
static Boolean HardDialogIsUp=False;
#endif

/*
    Kludge to keep Insurance Question dialog from popping down
    when help is requested.
*/
static Boolean DoNotPopdownInsurance=False;
static Boolean SliderFinished=False;

/*
    Kludge to workaround Motif 1.1.3 scale/scrollbar bug:
    If the ScrollBar is set insensitive from a valueChanged callback, the
    release method is not called and the continuous button press timer
    expires and is reset, generating new (bogus) events.

    Reset in MakeBet(), set in YourBetScaleCB() and AutoplayTimeout().
    We ignore valueChanged CBs after the first.
*/
static int betPlaced = 0;

void SetLabel(w, label)
	Widget w;
	String label;
{
	int i;
	XmString s;

	assert(w != NULL);
#ifdef OLIT
	s = label;
#else
	s = XmStringCreate(label, XmSTRING_DEFAULT_CHARSET);
#endif

	i = 0;
	XtSetArg(args[i], XmNlabelString, s);	i++;
	XtSetValues(w, args, i);
#ifndef OLIT
	XmStringFree(s);
#endif
}

/* Add this function because StaticText Widgets and buttons use different
   resources for setting their strings in OLIT */
#ifdef OLIT
void SetButtonLabel(w, label)
	Widget w;
	String label;
{
	int i;
	XmString s;

	assert(w != NULL);
	s = label;

	i = 0;
	XtSetArg(args[i], XtNlabel, s);	i++;
	XtSetValues(w, args, i);
}
#endif

void
Msg(str)
	String str;
{
#ifdef OLIT
	SetButtonLabel(StatusPushButton, str);
#else
	SetLabel(StatusPushButton, str);
#endif
}

void
MsgCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	Msg(client_data);
}

StopAutoplay()
{
	if (autoplay) {
		autoplay = False;
		XtRemoveTimeOut(timer);
	}
}

ShowHintMsg()
{
	char str[80];

        assert(highlow_index == (simple_count*100)/unseen_cards);
	sprintf(str, "(%d*100)/%d = %d", simple_count, unseen_cards,
				highlow_index);
	SetLabel(HintMsgLabel, str);
}

ClearHintMsg()
{
	SetLabel(HintMsgLabel, " ");
}

void
PopupCB(w, PopupIsUp, call_data)
	Widget w;
	Boolean *PopupIsUp;
	caddr_t call_data;
{
   *PopupIsUp = True;
}

void
PopdownCB(w, PopupIsUp, call_data)
	Widget w;
	Boolean *PopupIsUp;
	caddr_t call_data;
{
   *PopupIsUp = False;
}

Boolean
HintDialogIsManaged()
{
#ifdef OLIT
	return ((SplitDialogIsUp)
		|| (DoubleDialogIsUp)
		|| (SoftDialogIsUp)
		|| (HardDialogIsUp));
/*
	return (XtIsManaged(SplitStrategyDialog)
		|| XtIsManaged(DoubleStrategyDialog)
		|| XtIsManaged(SoftStrategyDialog)
		|| XtIsManaged(HardStrategyDialog));
	*/
#else
	return (XtIsManaged(SplitStrategyDialog)
		|| XtIsManaged(DoubleStrategyDialog)
		|| XtIsManaged(SoftStrategyDialog)
		|| XtIsManaged(HardStrategyDialog));
#endif
}

void
ManageHintDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	char str[80];
	int min, max, up;
	int choice;
	
	StopAutoplay();
	if (busy == True) {	/* wait for autoplay to stop */
		timer = XtAppAddTimeOut(app, timeout, ManageHintDialogCB, NULL);
		return;
	}
	ShowHintMsg();
	if (state == MAKING_BET_STATE) {
		if (highlow_index <= 2) {
			sprintf(str, "Only bet 1 when the index is less than 4",
				simple_count, unseen_cards, highlow_index);
		}
		else {
			sprintf(str, "Bet index/2 = %d/2 = %d units.",
				highlow_index, highlow_index/2);
		}
		Msg(str);
	}
	else if (state == MAKING_CHOICE_STATE) {
		min = min_total(current_hand);
		max = max_total(current_hand);
		up = c[DEALER][0]->name;

		/* cycle through tables, starting with best choice */
		choice = default_choice;
		if (HintDialogIsManaged()) {
		       if (
#ifdef OLIT
			   SplitDialogIsUp
#else
			   XtIsManaged(SplitStrategyDialog)
#endif
		           ) {
				if (choices & DOUBLE) {
					choice = DOUBLE;
				}
				else {
					choice = STAND;
				}
			}
		        else if (
#ifdef OLIT
				 DoubleDialogIsUp
#else
				 XtIsManaged(DoubleStrategyDialog)
#endif
			      ) {
				choice = STAND;
			}
			else {
				if (choices & SPLIT) {
					choice = SPLIT;
				}
				else if (choices & DOUBLE) {
					choice = DOUBLE;
				}
			}
		}

		if (choice & SPLIT) {
			UnmanageStrategyDialogs();
			ManageSplitDialog((min/2)-1, up, highlow_index);
		}
		else if (choice & DOUBLE) {
			UnmanageStrategyDialogs();
			ManageDoubleDialog(min, max, up, highlow_index);
		}
		else if (min != max) {
			UnmanageStrategyDialogs();
			ManageSoftDialog(max, up, highlow_index);
		}
		else {
			UnmanageStrategyDialogs();
			ManageHardDialog(max, up, highlow_index);
		}
	}
	else if (state == BUYING_INSURANCE_STATE) {
		if (highlow_index > 8) {
			sprintf(str, "Yes, since %d is greater than 8.",
				highlow_index);
		}
		else {
			sprintf(str, "No, since %d is not greater than 8.",
				highlow_index);
		}
		Msg(str);
	}
}

UnmanageStrategyDialogs()
{
#ifdef OLIT
	XtPopdown(SplitStrategyDialog);
	XtPopdown(DoubleStrategyDialog);
	XtPopdown(SoftStrategyDialog);
	XtPopdown(HardStrategyDialog);
#else
	XtUnmanageChild(SplitStrategyDialog);
	XtUnmanageChild(DoubleStrategyDialog);
	XtUnmanageChild(SoftStrategyDialog);
	XtUnmanageChild(HardStrategyDialog);
#endif
}

Boolean
IsMapped(w)
	Widget w;
{
	Boolean mapped = False;
	XWindowAttributes att;

	if (XtIsRealized(w)) {
		if (XGetWindowAttributes(XtDisplay(rootWidget),
					XtWindow(w), &att)) {
			mapped = !(att.map_state == IsUnmapped);
		}
	}
	return(mapped);
}

AdjustBankroll(delta)
	double delta;
{
	char str[10];

	bankroll += delta;
	sprintf(str, "$%g", bankroll);
	SetLabel(YouHaveLabelValue, str);
	if (bankroll > max_bankroll) {
		max_bankroll = bankroll;
	}
}

void
NoInsuranceCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	ClearHintMsg();
#ifdef OLIT
	XtPopdown(InsuranceHelpDialog);
#endif
	insured = False;
	if (BLACKJACK(DEALER) || BLACKJACK(current_hand)) {
		finish_hand();
	}
	else {
		if (!autoplay) {
			Msg("Make your choice.");
		}
		MakeChoice();
	}
}

void
YesInsuranceCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{ 
	ClearHintMsg();
#ifdef OLIT
	XtPopdown(InsuranceHelpDialog);
#endif
	insured = True;
	if (BLACKJACK(DEALER)) {
		AdjustBankroll(bet[current_hand]);	/* 2 to 1 on half */
#ifdef DEBUG
		fprintf(stderr, "YesInsuranceCB\tWon $%g on insurance bet.\n",
				bet[current_hand]);
#endif
		finish_hand();
	}
	else {
		AdjustBankroll(-bet[current_hand]/2);	/* lost half */
#ifdef DEBUG
		fprintf(stderr, "YesInsuranceCB\tLost $%g on insurance bet.\n",
				bet[current_hand]/2);
#endif
		Msg("You lost the insurance bet.");
		if (BLACKJACK(current_hand)) {
			finish_hand();
		}
		else {
			MakeChoice();
		}
	}
}

void
Action(action, w)
#ifdef OLIT
	OlVirtualName action;
#else
	String action;
#endif
	Widget w;
{
	XEvent event;
	assert(w != NULL);
#ifdef OLIT
	OlActivateWidget(w, action, (XtPointer) NULL);
#else
	XtCallActionProc(w, action, &event, (String *) NULL, 0);
#endif
}

#ifdef OLIT
void
select_button(w, e, p, np)
Widget w;
XEvent *e;
String *p;
Cardinal *np;
{
   XtCallCallbacks(w, XtNselect, NULL);
}
#endif

void
AutoplayTimeout()
{
	XmScaleCallbackStruct scale_data;

	if (busy == True) {	/* reset the timer */
		timer = XtAppAddTimeOut(app, timeout, AutoplayTimeout, NULL);
		return;
	}

	if (IsMapped(ShuffleDialog)) {
		Action("Return",
#ifdef OLIT
		       ShuffleDialogOK
#else
		       XtParent(XmMessageBoxGetChild(ShuffleDialog,
						     XmDIALOG_OK_BUTTON))
#endif
		    );
	}
	else if (IsMapped(InsuranceQuestionDialog)) {
		XtUnmanageChild(InsuranceQuestionDialog);
		if (should_insure(highlow_index)) {
			YesInsuranceCB(InsuranceQuestionDialog, NULL, NULL);
		}
		else {
			NoInsuranceCB(InsuranceQuestionDialog, NULL, NULL);
		}
	}
#if 0
	else if (XtIsSensitive(YourBetScale)) {
#else
	/* workaround Motif scrollbar bug */
        else if ((state == MAKING_BET_STATE) && !betPlaced) {
#endif

#ifdef OLIT
	   scale_data.new_location = should_bet(bankroll, highlow_index);
#else
		scale_data.reason = XmCR_VALUE_CHANGED;
		scale_data.event = (XEvent *) NULL;
		scale_data.value = should_bet(bankroll, highlow_index);
#endif
		XtCallCallbacks(YourBetScale, XmNvalueChangedCallback,
					(unsigned char *)&scale_data);
	}
	else if (XtIsSensitive(StandPushButton)) {	/* making choice */

		/* ignore timeouts till callback completes */
		busy = True;

		if (default_choice == SPLIT) {
			Action(BUTTON_PRESS, SplitPushButton);
		}
		else if (default_choice == DOUBLE) {
			Action(BUTTON_PRESS, DoublePushButton);
		}
		else if (default_choice == HIT) {
			Action(BUTTON_PRESS, HitPushButton);
		}
		else {
			assert (default_choice == STAND);
			Action(BUTTON_PRESS, StandPushButton);
		}
	}
	else {	/* this should never happen */
		fprintf(stderr, "AutoplayTimeout\tNOTHING SENSITIVE!\n");
	}

	/* if we're not waiting to stop, reset the timer */
	if (autoplay) {
		timer = XtAppAddTimeOut(app, timeout, AutoplayTimeout, NULL);
	}
}

void
VersionCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	char str[80];

	sprintf(str, "Version %s   PatchLevel %d", VERSION, PATCHLEVEL);
	Msg(str);
}

#ifdef DEBUG
dump_hands()
{
	int h,i;

	for (h = 0; h < MAX_HANDS; h++) {
		if (cards[h]) {
			fprintf(stderr, "dump_hands\t%s:\t", hand_name[h]);
			for (i = 0; i < cards[h]; i++) {
				dump_card(c[h][i]);
			}
			fprintf(stderr, "\n");
		}
	}
}

dump_stats()
{
	fprintf(stderr, "dump_stats\tcount=%+d index=%+d unplayed=%d\n",
		simple_count, highlow_index, unplayed_cards);
}
#endif

void
shuffle_up(first_in_play, doDialog)
	struct card *first_in_play;	/* NULL => shuffle all cards */
	Boolean doDialog;
{
	int h,i;

	/*
	 * We want ShuffleDialogCB() to pickup when we shuffle all the
	 * cards, so we wait for the dialog to get the focus.
	 */
	if (first_in_play) {
		DoShuffleDialog(first_in_play->index-1);
		pickup_cards = False;	/* for ShuffleDialogCB() */
	}
	else if (doDialog) {
		DoShuffleDialog(total_cards);
		pickup_cards = True;	/* for ShuffleDialogCB() */
	}

	shuffle(first_in_play);		/* first_in_play and on excluded */

	next_card();			/* bury the first card */
	unplayed_cards = total_cards - 1;
	unseen_cards = total_cards;
	unseen_fives = total_fives;
	unseen_tens = total_tens;
	unseen_others = total_others;
	simple_count = 0;
	highlow_index = 0;
	decks_played+=rv.decks;

	/* account for unshuffled cards on the table */
	/* should count all unshuffled (what about bust pick-ups?) */
	if (first_in_play) {
#ifdef DEBUG
		dump_stats();
#endif
		for (h = 0; h < MAX_HANDS; h++) {
			for (i = 1; i <= cards[h]; i++) {
				unplayed_cards--;
				/* hole card may get counted later */
				if (hole_seen || h != DEALER || i != 2) {
					face_up(h, i, False);
#ifdef DEBUG
					dump_stats();
#endif
				}
			}
		}
	}
}

/*
**  Deal a card to 'hand'.
*/
void
deal_card(hand)
	int hand;
{
	/* need to reshuffle during game */
	if (unplayed_cards <= SHUFFLE_POINT2) {
#ifdef DEBUG
		fprintf(stderr,"deal_card\tPartial shuffle\n");
		dump_hands();
#endif
		/* exclude first card dealt and on from the shuffle */
		shuffle_up(c[1][0], True);
	}

	c[hand][cards[hand]++] = next_card();
	unplayed_cards--;
	if ((hand == DEALER) && (cards[DEALER] == 2) && !hole_seen) {
		display_cards(DEALER);   /* force hole card under */
	}
	else {
		face_up(hand, cards[hand], True);
	}

}

#ifndef OLIT
TurnOffSashTraversal(pane)
	Widget pane;
{
	Widget *children;
	int num_children;
	int i;

	i = 0;
	XtSetArg(args[i], XmNchildren, &children);		i++;
	XtSetArg(args[i], XmNnumChildren, &num_children);	i++;
	XtGetValues(pane, args, i);
	while (num_children-- > 0) {
		if (XmIsSash(children[num_children])) {
			i = 0;
			XtSetArg(args[i], XmNtraversalOn, False); i++;
			XtSetValues(children[num_children], args, i);
		}
	}

}
#endif

MakeChoice()
{
	char str[80];

	state = MAKING_CHOICE_STATE;

	if (next_hand != 2) {
		strcpy(str,hand_name[current_hand]);
		strcat(str, " split hand.");
		Msg(str);
	}
	else if (!autoplay) {
		Msg("Make your choice.");
	}

	choices = STAND | HIT;
	if (cards[current_hand] == 2) {
		if (min_total(current_hand) == 10
		 || min_total(current_hand) == 11
		 || max_total(current_hand) == 10) {
			if (bankroll - bet[current_hand] >= 0) {
				choices |= DOUBLE;
			}
		}
		if (c[current_hand][0]->name == c[current_hand][1]->name) {
			if (bankroll - bet[current_hand] >= 0) {
				choices |= SPLIT;
			}
		}
	}

	default_choice = -1;
	if (choices & SPLIT) {
		if (should_split(c[current_hand][0]->name,
			c[DEALER][0]->name, highlow_index)) {
			default_choice = SPLIT;
		}
	}
	if ((default_choice == -1) && (choices & DOUBLE)) {
		if (should_double(min_total(current_hand),
			max_total(current_hand), c[DEALER][0]->name,
			highlow_index)) {
			default_choice = DOUBLE;
		}
	}
	if (default_choice == -1) {
		if (should_stand(min_total(current_hand),
			max_total(current_hand), c[DEALER][0]->name,
			highlow_index)) {
			default_choice = STAND;
		}
		else default_choice = HIT;
	}

	if (choices & SPLIT) {
		XtSetSensitive(SplitPushButton, True);
#ifndef OLIT
		if (default_choice == SPLIT) {
			XmProcessTraversal(SplitPushButton,
				XmTRAVERSE_NEXT_TAB_GROUP);
			XmProcessTraversal(SplitPushButton,
				XmTRAVERSE_CURRENT);
		}
#endif
	}
	else {
		XtSetSensitive(SplitPushButton, False);
	}

	if (choices & DOUBLE) {
		XtSetSensitive(DoublePushButton, True);
#ifndef OLIT
		if (default_choice == DOUBLE) {
			XmProcessTraversal(DoublePushButton,
				XmTRAVERSE_NEXT_TAB_GROUP);
			XmProcessTraversal(DoublePushButton,
				XmTRAVERSE_CURRENT);
		}
#endif
	}
        else {
		XtSetSensitive(DoublePushButton, False);
	}

	XtSetSensitive(HitPushButton, True);
#ifndef OLIT
	if (default_choice == HIT) {
		XmProcessTraversal(HitPushButton,
				XmTRAVERSE_NEXT_TAB_GROUP);
		XmProcessTraversal(HitPushButton,
				XmTRAVERSE_CURRENT);
	}
#endif

	XtSetSensitive(StandPushButton, True);
#ifndef OLIT
	if (default_choice == STAND) {
		XmProcessTraversal(StandPushButton,
				XmTRAVERSE_NEXT_TAB_GROUP);
		XmProcessTraversal(StandPushButton,
				XmTRAVERSE_CURRENT);
	}
#endif
	busy = False;	/* stop ignoring autoplay timeouts */
}

UpdateWonLabelValue()
{
	char str[80];

	sprintf(str, "%d", hands_won);
	SetLabel(WonLabelValue, str);
}

UpdateWonLabelPercent()
{
	char str[80];

	sprintf(str, "%d%%", percent_won);
	SetLabel(WonLabelPercent, str);
}

UpdateLostLabelValue()
{
	char str[80];

	sprintf(str, "%d", hands_lost);
	SetLabel(LostLabelValue, str);
}

UpdateLostLabelPercent()
{
	char str[80];

	sprintf(str, "%d%%", percent_lost);
	SetLabel(LostLabelPercent, str);
}

UpdatePushedLabelValue()
{
	char str[80];

	sprintf(str, "%d", hands_pushed);
	SetLabel(PushedLabelValue, str);
}

UpdatePushedLabelPercent()
{
	char str[80];

	sprintf(str, "%d%%", percent_pushed);
	SetLabel(PushedLabelPercent, str);
}

UpdateWonLostAndPushed()
{
	UpdateWonLabelValue();
	UpdateWonLabelPercent();
	UpdateLostLabelValue();
	UpdateLostLabelPercent();
	UpdatePushedLabelValue();
	UpdatePushedLabelPercent();
}

int
pay_off(hand)
	int hand;
{
	double delta;
	char str[80];
	char str1[30];
	int p;

	strcpy(str,"");
	if (BLACKJACK(DEALER)) {
		if (BLACKJACK(hand)) {
			delta = 0;
			strcat(str, "BLACKJACKS push.");
		}
		else {
			delta = -bet[hand];
			strcpy(str, "Dealer BLACKJACK!");
		}
	}
	else if (BLACKJACK(hand)) {
		delta = 1.5 * bet[hand];
		strcpy(str, "BLACKJACK!");
	}
	else if (min_total(hand) > 21) {
		delta = -bet[hand];
		strcpy(str, "You busted.");
	}
	else if (min_total(DEALER) > 21) {	
		delta = bet[hand];
		strcpy(str, "Dealer busted.");
	}
	else if (max_total(hand) == max_total(DEALER)) {
		delta = 0;
		sprintf(str, "Push %d's.", max_total(hand));
	}
	else {
		if (max_total(hand) > max_total(DEALER)) {
			delta = bet[hand];
		}
		else delta = -bet[hand];
		sprintf(str, "Dealer has %d, you have %d.",
			max_total(DEALER), max_total(hand));
	}
	
	if (delta > 0) {
		sprintf(str1, "  You won $%g", delta);
	}
	else if (delta < 0) {
		if (BLACKJACK(DEALER) && insured) {
			/* YesInsurance() paid off the insurance bet */
			assert (delta == -bet[hand]);
			sprintf(str1, "  Push, you were insured.");
		}
		else {
			sprintf(str1, "  You lost $%g", -delta);
		}
	}
	else {
		strcpy(str1, "");
	}

	strcat(str, str1);
	Msg(str);

	hands_played++;

	assert(hands_played != 0);
	if (delta > 0) {
		AdjustBankroll(bet[hand]+delta);
		hands_won++;
		UpdateWonLabelValue();
	}
	else if (delta < 0) {			/* house has the bet */
		hands_lost++;
		UpdateLostLabelValue();
	}
	else {
		AdjustBankroll(bet[hand]);	/* push, return the bet */
		hands_pushed++;
		UpdatePushedLabelValue();
	}

	p = ((hands_won*100)/hands_played) + 0.5;	/* round up */
	if (percent_won != p) {
		percent_won = p;
		UpdateWonLabelPercent();
	}
	p = ((hands_lost*100)/hands_played) + 0.5;
	if (percent_lost != p) {
		percent_lost = p;
		UpdateLostLabelPercent();
	}
	p = ((hands_pushed*100)/hands_played) + 0.5;
	if (percent_pushed != p) {
		percent_pushed = p;
		UpdatePushedLabelPercent();
	}

#ifdef DEBUG
	while (strlen(str) < 40) {
		strcat(str, " ");
	}
	if (cards[2] == 0) {	/* no splits */
		fprintf(stderr, "pay_off\t\t%s\t%8.2f\n", str, bankroll);
	}
	else {
		fprintf(stderr, "pay_off %s\t%s\t%8.2f\n",
					hand_name[hand], str, bankroll);
	}
#endif
}


void
deal_to_dealer()
{
	int hand;
	int player_waiting;
	int min = min_total(DEALER);
	int max = max_total(DEALER);
	
	face_up(DEALER, 2, True);

	/* If all player hands are blackjack or busted, quit */
	player_waiting = False;
	for (hand = 1; hand < MAX_HANDS; hand++) {
		if ((cards[hand] > 0) && (min_total(hand) <= 21)) {
			if ((max_total(hand) != 21) || (cards[hand] != 2)) {
				player_waiting = True;
				break;
			}
		}
	}
	if (player_waiting) {		/* hit soft 17 */
		while ((max < 17) || ((min != max) && (max == 17))) {
			deal_card(DEALER);
			min = min_total(DEALER);
			max = max_total(DEALER);
			if (min > 21) return;
		}
	}
}

RedrawDealerCards()
{
	XEvent event;

	/* drain Expose events */
	while (XCheckTypedWindowEvent(XtDisplay(rootWidget),
				XtWindow(DealerDrawingArea), Expose, &event));
	XClearWindow(XtDisplay(DealerDrawingArea), XtWindow(DealerDrawingArea));
	display_cards(DEALER);
}

RedrawPlayerCards()
{
	XEvent event;

	/* drain Expose events */
	while (XCheckTypedWindowEvent(XtDisplay(rootWidget),
				XtWindow(PlayerDrawingArea), Expose, &event));
	XClearWindow(XtDisplay(PlayerDrawingArea), XtWindow(PlayerDrawingArea));
	display_cards(current_hand);
}

RedrawCards()
{
	RedrawDealerCards();
	RedrawPlayerCards();
}

DoPayoffSplitDialog()
{
	int i;
	XmString s;
	char str[80];
	void OkPayoffDialogCB();
	
	pay_off(current_hand);

	if (autoplay) {
		OkPayoffDialogCB();
		return;
	}

	sprintf(str, "%s split hand", hand_name[current_hand]);
	i = 0;
#ifdef OLIT
	s = str;
	XtSetArg(args[i], XtNstring, s);	i++;
	XtSetValues(PayoffSplitDialogT, args, i);
	XtPopup(PayoffSplitDialog, XtGrabExclusive);
#else
	s = XmStringCreate(str, XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNmessageString, s);	i++;
	XtSetValues(PayoffSplitDialog, args, i);
	XmStringFree(s);
	XtUnmanageChild(XmMessageBoxGetChild(PayoffSplitDialog,
					XmDIALOG_CANCEL_BUTTON));
	XtUnmanageChild(XmMessageBoxGetChild(PayoffSplitDialog,
					     XmDIALOG_HELP_BUTTON));
	XtManageChild(PayoffSplitDialog);
#endif
}

DesensitizeChoiceButtons()
{
	XtSetSensitive(SplitPushButton, False);
	XtSetSensitive(DoublePushButton, False);
	XtSetSensitive(HitPushButton, False);
	XtSetSensitive(StandPushButton, False);
}

void
MakeBet()
{
	int i;
	extern unsigned long   blackpixel;
	XmScaleCallbackStruct scale_data;

	betPlaced = 0;			/* for working around Motif bug */
	state = MAKING_BET_STATE;

	if (bankroll < 1.0) {			/* bankroll spent */
		Msg("Thank-you for coming");
		bankroll = rv.bankroll;		/* resupply */
		AdjustBankroll(0.0);		/* update value displayed */
		StopAutoplay();
	}

	if (unplayed_cards <= SHUFFLE_POINT1) {	/* before the deal */
		shuffle_up(NULL, True);		/* all with dialog */
	}

	DesensitizeChoiceButtons();

	/* reset the scale, and make sensitive */
		if (rv.adjust_bet_unit)
		   rv.bet_unit = Set_Bet_Unit(bankroll);
	if (bankroll >= 2.0) {
		i = 0;
#ifdef OLIT
		{
		   char s[10];
		   sprintf(s,"$%d",should_bet(bankroll, highlow_index));
		   XtVaSetValues(YourBetScaleValue, XtNstring, s, NULL);
		   sprintf(s,"%d",(int)bankroll);
		   XtSetArg(args[i], XtNmaxLabel, s);	i++;
		}
#endif
		XtSetArg(args[i], XmNmaximum, (int)bankroll);	i++;
		XtSetArg(args[i], XmNvalue,
			should_bet(bankroll, highlow_index));  i++;
		XtSetValues(YourBetScale, args, i);
		XtSetSensitive(YourBetScale, True);
#ifndef OLIT
		XmProcessTraversal(YourBetScale, XmTRAVERSE_NEXT_TAB_GROUP);
		XmProcessTraversal(YourBetScale, XmTRAVERSE_CURRENT);
#endif
	}
	else {	/* only a buck to bet, scale requires min<max */
#ifdef OLIT
		scale_data.new_location = 1;
#else
		scale_data.reason = XmCR_VALUE_CHANGED;
		scale_data.event = (XEvent *) NULL;
		scale_data.value = 1;
#endif
		XtCallCallbacks(YourBetScale, XmNvalueChangedCallback,
					(unsigned char *)&scale_data);
	}
#ifdef OLIT
	XtSetSensitive(DealPushButton, True);
#endif
        busy = False;   /* stop ignoring autoplay timeouts */
}

void
OkPayoffDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	if (current_hand+1 < next_hand) {
		++current_hand;
		RedrawCards();
		DoPayoffSplitDialog();
	}
	else {
		MakeBet();
	}
}

finish_hand()
{
	if (current_hand == (next_hand-1)) {
		deal_to_dealer();
		if (next_hand > 2) {		/* pay splits with dialog */
			current_hand = 1;	/* starting with First */
			RedrawCards();
			DoPayoffSplitDialog();
		}
		else {
			pay_off(current_hand);
			MakeBet();
		}
	}
	else {	/* move on to next split hand */
		current_hand++;
		RedrawCards();
		MakeChoice();
	}
}

void
PickupCards()
{
	int i;

	for (i = 0; i < MAX_HANDS; i++) {
		cards[i] = 0;
	}
}

#ifdef OLIT
void
SetYourBetCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	XtPointer call_data;
{
   OlVirtualEvent vevent=(OlVirtualEvent)call_data;

   if (vevent->virtual_name == OL_SELECT) {
      switch (vevent->xevent->type) {
      case ButtonRelease:
	 SliderFinished=True;
	 break;
      case ButtonPress:
      case MotionNotify:
	   {
	      int bet;
	      char s[10];
	      XtVaGetValues(YourBetScale, XtNsliderValue, &bet, NULL);
	      sprintf(s,"$%d",bet);
	      XtVaSetValues(YourBetScaleValue, XtNstring, s, NULL);
	   }
	 SliderFinished=False;
	 break;
      default:
	 break;
      }
   }

}
#endif

void
YourBetScaleCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	XmScaleCallbackStruct *call_data;
{
	int i;
	char str[80];
	Widget defaultButton;

#if 0
	/* generates multiple CBs when left pressed next to slider */
	XtSetSensitive(w, False);
#else
	/* workaround by reseting when user changes later */
	if (betPlaced) {
		XtVaSetValues(w, XmNvalue, betPlaced, NULL);
		return;
	}
	else {
#ifdef OLIT
	   {
	      char s[10];
	      sprintf(s,"$%d",call_data->new_location);
	      XtVaSetValues(YourBetScaleValue, XtNstring, s, NULL);
	      if (!SliderFinished) {
		 return;
	      }
	   }

		betPlaced = call_data->new_location;
#else
		betPlaced = call_data->value;
#endif
		XtVaSetValues(w, XmNvalue, betPlaced, NULL);
	}
#endif

	PickupCards();			/* reset cards[] (for autoplay) */
	current_hand = 1;
	next_hand = 2;
#ifdef OLIT
	bet[current_hand] = call_data->new_location;
#else
	bet[current_hand] = call_data->value;
#endif
	AdjustBankroll(-bet[current_hand]);

        XClearWindow(XtDisplay(DealerDrawingArea), XtWindow(DealerDrawingArea));
#ifndef OLIT
	i = 0;
	XtSetArg(args[i], XmNtraversalOn, False);  i++;
	XtSetValues(w, args, i);
#endif

	ClearHintMsg();

        XClearWindow(XtDisplay(PlayerDrawingArea), XtWindow(PlayerDrawingArea));
#ifndef OLIT
	i = 0;
	XtSetArg(args[i], XmNtraversalOn, False);  i++;
	XtSetValues(PlayerDrawingArea, args, i);

	TurnOffSashTraversal(PanedWindow);
#endif

	insured = False;
	hole_seen = False;

	deal_card(current_hand);
	deal_card(DEALER);
	deal_card(current_hand);
	deal_card(DEALER);

	/* Handle INSURANCE dialog */
	if (c[DEALER][0]->name == ACE
	 && (bankroll - (double)bet[current_hand]/2) >= 0.0) {
		if (autoplay) {
			if (should_insure(highlow_index)) {
				YesInsuranceCB(w, NULL, NULL);
			}
			else{
				NoInsuranceCB(w, NULL, NULL);
			}
		}
		else {
			state = BUYING_INSURANCE_STATE;
			w = InsuranceQuestionDialog;
			if (should_insure(highlow_index)) {
#ifdef OLIT
			   XtVaSetValues(InsuranceQuestionDialogY,
				 XtNdefault, True,
					 NULL);
			   XtVaSetValues(InsuranceQuestionDialogN,
				 XtNdefault, False,
					 NULL);
#else
				defaultButton = XmMessageBoxGetChild(w,
					XmDIALOG_OK_BUTTON);
#endif
			}
			else {
#ifdef OLIT
			   XtVaSetValues(InsuranceQuestionDialogY,
				 XtNdefault, False,
					 NULL);
			   XtVaSetValues(InsuranceQuestionDialogN,
				 XtNdefault, True,
					 NULL);
#else
				defaultButton = XmMessageBoxGetChild(w,
					XmDIALOG_CANCEL_BUTTON);
#endif
			}
#ifdef OLIT
			XtPopup(w, XtGrabExclusive);
#else
			i = 0;
			XtSetArg(args[i], XmNdefaultButton, defaultButton); i++;
			XtSetValues(w, args, i);
			XtManageChild(w);
#endif
			sprintf(str, "Insurance costs $%g",
					(double)bet[current_hand]/2);
	 		Msg(str);
		}
	}
	else {
		if (BLACKJACK(current_hand) || BLACKJACK(DEALER)) {
			finish_hand();
		}
		else {
			MakeChoice();
		}
	}
}

void
AutoplayScaleCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	XmScaleCallbackStruct *call_data;
{
#ifdef OLIT
	timeout = call_data->new_location;
#else
	timeout = call_data->value;
#endif
        if (autoplay && (timeout == TIMEOUT)) {		/* stop */
		StopAutoplay();
	}
        else if (!autoplay) {				/* start */
		autoplay = True;
		timer = XtAppAddTimeOut(app, timeout, AutoplayTimeout, NULL);
	}
}

#ifdef OLIT
void
DealCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	XmScaleCallbackStruct scale_data;

	XtSetSensitive(DealPushButton, False);
	XtVaGetValues(YourBetScale,
		      XmNvalue, &scale_data.new_location,
		      NULL);
	XtCallCallbacks(YourBetScale, XmNvalueChangedCallback,
			(unsigned char *)&scale_data);
}
#endif

void
SplitCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	int split_hand;

	ClearHintMsg();
	DesensitizeChoiceButtons();
	UnmanageStrategyDialogs();
	split_hand = next_hand++;
	c[split_hand][0] = c[current_hand][1];
	cards[current_hand] = 1;
	cards[split_hand] = 1;
	bet[split_hand] = bet[current_hand];
	AdjustBankroll(-bet[current_hand]);
	deal_card(current_hand);
	deal_card(split_hand);
	if (c[current_hand][0]->name == ACE) {
		current_hand++;
		finish_hand();		/* Aces are split down and dirty */
	}
	else {
		RedrawCards();
		MakeChoice();
	}
}

void
DoubleCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	ClearHintMsg();
	DesensitizeChoiceButtons();
	UnmanageStrategyDialogs();
	AdjustBankroll(-bet[current_hand]);
	bet[current_hand] += bet[current_hand];

	deal_card(current_hand);
	finish_hand();
}


void
HitCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	ClearHintMsg();
	DesensitizeChoiceButtons();
	UnmanageStrategyDialogs();
	deal_card(current_hand);
	if (min_total(current_hand) >= 21) {
		finish_hand();
	}
	else {
		MakeChoice();
	}
}

void
StandCB(w, client_data, call_data)
	Widget w;
	XtPointer client_data;
	caddr_t call_data;
{
	ClearHintMsg();
	DesensitizeChoiceButtons();

	UnmanageStrategyDialogs();
	finish_hand();
}

void
DealerDrawingAreaCB(w, client_data, call_data)
	Widget w;
	XtPointer client_data;
	XmDrawingAreaCallbackStruct *call_data;
{
#ifdef OLIT
/* OLIT is not returning a valid window value for resize events so
   just always redraw */
	{
#else
	if (call_data->window) {
#endif
		RedrawDealerCards();
	}
	if (HintDialogIsManaged()) {
		ShowHintMsg();	/* recenter after possible resize */
	}
}

void
PlayerDrawingAreaCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	XmDrawingAreaCallbackStruct *call_data;
{
#ifdef OLIT
/* OLIT is not returning a valid window value for resize events so
   just always redraw */
	{
#else
	if (call_data->window) {
#endif
		RedrawPlayerCards();
	}
	if (HintDialogIsManaged()) {
		ShowHintMsg();
	}
}

void
ShuffleDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
	ClearHintMsg();
	if (pickup_cards) {
		PickupCards();
	}
	RedrawCards();
}

void
done(status)
	int status;
{
	exit(status);
}

initialize()
{
	autoplay = False;
        init_cards(rv.decks, &total_cards, &total_fives, &total_tens,
				&total_others);

	decks_played = -1;	/* displayed where we bump in shuffle_up() */
	bankroll = rv.bankroll;
	max_bankroll = rv.bankroll;
	hands_played = 0;
	hands_won = 0;
	hands_lost = 0;
	hands_pushed = 0;
	percent_won = 0;
	percent_lost = 0;
	percent_pushed = 0;
}

int
min_total(hand)
	int hand;
{
	int i;
	int total;
	
	total = 0;
	for (i = 0; i < cards[hand]; i++) {
		total += card_value[c[hand][i]->name];
	}
	return total;
}


int
max_total(hand)
	int hand;
{
	int i;
	int total;
	int aces;
	
	total = 0;
	aces = 0;
	for (i = 0; i < cards[hand]; i++) {
		if (c[hand][i]->name == ACE) {
			total += 11;
			aces++;
		}
		else total += card_value[c[hand][i]->name];
	}
	while ((total > 21) && (aces > 0)) {
		total -= 10;
		aces--;
	}
	return total;
}

DoShuffleDialog(total)
	int total;
{
	int i;
	XmString s;
	char str[80];
	
	if (autoplay) {		/* need to reset cards[] eventually */
		return;
	}

	sprintf(str, "Shuffling %d cards, will bury the first.", total);
	i = 0;
#ifdef OLIT
	s = str;
	XtSetArg(args[i], XtNstring, s);	i++;
	XtSetValues(ShuffleDialogT, args, i);
	XtPopup(ShuffleDialog, XtGrabExclusive);
#else
	s = XmStringCreate(str, XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNmessageString, s);	i++;
	XtSetValues(ShuffleDialog, args, i);
	XmStringFree(s);
	XtUnmanageChild(XmMessageBoxGetChild(ShuffleDialog,
					XmDIALOG_CANCEL_BUTTON));
	XtUnmanageChild(XmMessageBoxGetChild(ShuffleDialog,
					XmDIALOG_HELP_BUTTON));
	XtManageChild(ShuffleDialog);
#endif
}

/*
**  Display 'n'th card of 'hand' face up, update statistics.
*/
face_up(hand, n, display)
	int hand;
	int n;
	int display;
{
	int suit;
	int name;

	suit = c[hand][n-1]->suit;
	name = c[hand][n-1]->name;
	if (display) {
		display_card(hand, n, suit, name);
	}
	unseen_cards--;
	assert(unseen_cards > unplayed_cards);
	assert(unseen_cards != 0);
	if (name == FIVE) unseen_fives--;
	if (card_value[name] < 10) unseen_others--;
	else unseen_tens--;
	simple_count += count_value[name];
	highlow_index = (simple_count*100)/unseen_cards;
	if ((hand == DEALER) && (n == 2)) {
		hole_seen = True;
	}
}

display_cards(hand)
	int hand;
{
	int i;
	
	/* draw dealer's hole card underneath first */
	if ((hand == DEALER) && cards[DEALER] && !hole_seen) {
		face_down(hand, 2);
		display_card(hand, 1, c[hand][0]->suit, c[hand][0]->name);
	}
	else for (i = 0; i < cards[hand]; i++) {
		display_card(hand, i+1, c[hand][i]->suit, c[hand][i]->name);
	}
}

void
ManageHelpGeneralDialogCB(w, client_data, call_data)
	Widget w;
	Widget client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopup(HelpGeneralDialog, XtGrabNone);
#else
	XtManageChild(HelpGeneralDialog);
#endif
}

void
UnmanageHelpGeneralDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopdown(HelpGeneralDialog);
#else
	XtUnmanageChild(HelpGeneralDialog);
#endif
}

void
ManageHelpRulesDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopup(HelpRulesDialog, XtGrabNone);
#else
	XtManageChild(HelpRulesDialog);
#endif
}

void
UnmanageHelpRulesDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopdown(HelpRulesDialog);
#else
	XtUnmanageChild(HelpRulesDialog);
#endif
}

void
ManageHelpBasicStrategyDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopup(HelpBasicStrategyDialog, XtGrabNone);
#else
	XtManageChild(HelpBasicStrategyDialog);
#endif
}

void
UnmanageHelpBasicStrategyDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopdown(HelpBasicStrategyDialog);
#else
	XtUnmanageChild(HelpBasicStrategyDialog);
#endif
}

void
ManageHelpCountingDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopup(HelpCountingDialog, XtGrabNone);
#else
	XtManageChild(HelpCountingDialog);
#endif
}

void
UnmanageHelpCountingDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopdown(HelpCountingDialog);
#else
	XtUnmanageChild(HelpCountingDialog);
#endif
}

void
ManageHelpBestStrategyDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopup(HelpBestStrategyDialog, XtGrabNone);
#else
	XtManageChild(HelpBestStrategyDialog);
#endif
}

void
UnmanageHelpBestStrategyDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopdown(HelpBestStrategyDialog);
#else
	XtUnmanageChild(HelpBestStrategyDialog);
#endif
}

void
InsuranceQuestionPopdown(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	Boolean *call_data;
{
   if (DoNotPopdownInsurance) {
      DoNotPopdownInsurance = False;
      *call_data = False;
   }
}

void
ManageInsuranceHelpDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	DoNotPopdownInsurance = True;
	XtPopup(InsuranceHelpDialog, XtGrabNone);
#else
	XtManageChild(InsuranceHelpDialog);
#endif
}

void
UnmanageInsuranceHelpDialogCB(w, client_data, call_data)
	Widget w;
	caddr_t client_data;
	caddr_t call_data;
{
#ifdef OLIT
	XtPopdown(InsuranceHelpDialog);
#else
	XtUnmanageChild(InsuranceHelpDialog);
#endif
}

#ifdef OLIT
CreateOLITWidgets()
{
   int i;
   Widget w;
   char s[20];

   i=0;
   MainWindow = XtCreateManagedWidget("MainWindow",rubberTileWidgetClass,
					   rootWidget, args, i);

   i=0;
   XtSetArg(args[i], XtNweight, 0);		i++;
   MenuBar = XtCreateManagedWidget("MenuBar", controlAreaWidgetClass,
				   MainWindow, args, i);

   i=0;
   MainTable = XtCreateManagedWidget("MainTable",rubberTileWidgetClass,
				     MainWindow, args, i);

   i=0;
   XtSetArg(args[i], XtNweight, 0);		i++;
   StatusPushButton = XtCreateManagedWidget("StatusPushButton",
					    oblongButtonWidgetClass,
					    MainWindow,
					    args, i);

   i = 0;
   XtSetArg(args[i], XtNweight, 0);		i++;
   ControlTable = XtCreateManagedWidget("ControlTable",
					bulletinBoardWidgetClass,
					MainTable,args,i);

   i=0;
   DrawAreas = XtCreateManagedWidget("DrawAreas",
					     rubberTileWidgetClass,
					     MainTable, args, i);

   i = 0;
   sprintf(s,"%d",TIMEOUT);
   XtSetArg(args[i], XtNsliderMin, TIMEOUT);		i++;
   XtSetArg(args[i], XtNminLabel, s);			i++;
   XtSetArg(args[i], XtNsliderValue, TIMEOUT);		i++;
   XtSetArg(args[i], XtNweight, 0);		i++;
   AutoplayScale = XtCreateManagedWidget("AutoplayScale",sliderWidgetClass,
					 MainTable, args, i);
   XtSetSensitive(AutoplayScale, True);

   i=0;
   DealerDrawingArea = XtCreateManagedWidget("DealerDrawingArea",
					     drawAreaWidgetClass,
					     DrawAreas, args, i);

   i=0;
   XtSetArg(args[i], XtNweight, 0);		i++;
   HintMsgLabel = XtCreateManagedWidget("HintMsgLabel",staticTextWidgetClass,
					DrawAreas, args, i);

   i=0;
   PlayerDrawingArea = XtCreateManagedWidget("PlayerDrawingArea",
					     drawAreaWidgetClass,
					     DrawAreas, args, i);

   i = 0;
   sprintf(s,"%d",rv.bankroll);
   XtSetArg(args[i], XtNdragCBType, OL_CONTINUOUS);	i++;
   XtSetArg(args[i], XtNweight, 0);			i++;
   XtSetArg(args[i], XtNsliderMax, rv.bankroll);	i++;
   XtSetArg(args[i], XtNmaxLabel, s);			i++;
   YourBetScale = XtCreateManagedWidget("YourBetScale",sliderWidgetClass,
					 ControlTable, args, i);

   i = 0;
   YourBetScaleTitle = XtCreateManagedWidget("YourBetScaleTitle",
					     staticTextWidgetClass,
					     ControlTable,args,i);
   
   i = 0;
   YourBetScaleValue = XtCreateManagedWidget("YourBetScaleValue",
					     staticTextWidgetClass,
					     ControlTable,args,i);
   
   i = 0;
   YouHaveLabel = XtCreateManagedWidget("YouHaveLabel",
				    staticTextWidgetClass,
				    ControlTable,args,i);
/* still set from slider Max value */
   XtSetArg(args[i], XtNstring, s);			i++;
   YouHaveLabelValue = XtCreateManagedWidget("YouHaveLabelValue",
				    staticTextWidgetClass,
				    ControlTable,args,i);

   i = 0;
   WonLabel = XtCreateManagedWidget("WonLabel",
				    staticTextWidgetClass,
				    ControlTable,args,i);
   WonLabelValue = XtCreateManagedWidget("WonLabelValue",
				    staticTextWidgetClass,
				    ControlTable,args,i);
   WonLabelPercent = XtCreateManagedWidget("WonLabelPercent",
				    staticTextWidgetClass,
				    ControlTable,args,i);

   i = 0;
   LostLabel = XtCreateManagedWidget("LostLabel",
				    staticTextWidgetClass,
				    ControlTable,args,i);
   LostLabelValue = XtCreateManagedWidget("LostLabelValue",
				    staticTextWidgetClass,
				    ControlTable,args,i);
   LostLabelPercent = XtCreateManagedWidget("LostLabelPercent",
				    staticTextWidgetClass,
				    ControlTable,args,i);

   i = 0;
   PushedLabel = XtCreateManagedWidget("PushedLabel",
				    staticTextWidgetClass,
				    ControlTable,args,i);
   PushedLabelValue = XtCreateManagedWidget("PushedLabelValue",
				    staticTextWidgetClass,
				    ControlTable,args,i);
   PushedLabelPercent = XtCreateManagedWidget("PushedLabelPercent",
				    staticTextWidgetClass,
				    ControlTable,args,i);

   i = 0;
   PlaceHolderLabel = XtCreateManagedWidget("PlaceHolderLabel",
				    staticTextWidgetClass,
				    ControlTable,args,i);

   i=0;
   XtSetArg(args[i], XmNsensitive, False);			i++;
   DealPushButton = XtCreateManagedWidget("DealPushButton",
					    oblongButtonWidgetClass,
					    ControlTable,
					    args, i);

   i=0;
   XtSetArg(args[i], XmNsensitive, False);			i++;
   SplitPushButton = XtCreateManagedWidget("SplitPushButton",
					    oblongButtonWidgetClass,
					    ControlTable,
					    args, i);

   i=0;
   XtSetArg(args[i], XmNsensitive, False);			i++;
   DoublePushButton = XtCreateManagedWidget("DoublePushButton",
					    oblongButtonWidgetClass,
					    ControlTable,
					    args, i);

   i=0;
   XtSetArg(args[i], XmNsensitive, False);			i++;
   HitPushButton = XtCreateManagedWidget("HitPushButton",
					    oblongButtonWidgetClass,
					    ControlTable,
					    args, i);

   i=0;
   XtSetArg(args[i], XmNsensitive, False);			i++;
   StandPushButton = XtCreateManagedWidget("StandPushButton",
					    oblongButtonWidgetClass,
					    ControlTable,
					    args, i);

   i=0;
   FileCascadeButton = XtCreateManagedWidget("FileCascadeButton",
					    menuButtonWidgetClass,
					    MenuBar,
					    args, i);
   XtVaGetValues(FileCascadeButton,
		 XtNmenuPane, &w,
		 NULL);
   
   i=0;
   QuitCascadeButton = XtCreateManagedWidget("QuitCascadeButton",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpCascadeButton = XtCreateManagedWidget("HelpCascadeButton",
					    menuButtonWidgetClass,
					    MenuBar,
					    args, i);
   XtVaGetValues(HelpCascadeButton,
		 XtNmenuPane, &w,
		 NULL);
   
   i=0;
   HelpGeneralPushButton = XtCreateManagedWidget("HelpGeneralPushButton",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpRulesPushButton = XtCreateManagedWidget("HelpRulesPushButton",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpBasicStrategyPushButton =
      XtCreateManagedWidget("HelpBasicStrategyPushButton",
			    oblongButtonWidgetClass,
			    w,
			    args, i);

   i=0;
   HelpCountingPushButton = XtCreateManagedWidget("HelpCountingPushButton",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpBestStrategyPushButton =
      XtCreateManagedWidget("HelpBestStrategyPushButton",
			    oblongButtonWidgetClass,
			    w,
			    args, i);

   i=0;
   HelpVersionPushButton = XtCreateManagedWidget("HelpVersionPushButton",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   XtSetArg(args[i], XtNpushpin, OL_NONE);			i++;
   InsuranceQuestionDialog = XtCreatePopupShell("InsuranceQuestionDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(InsuranceQuestionDialog,
		 XtNupperControlArea, &w,
		 NULL);

   i=0;
   XtSetArg(args[i], XtNstring, "Want Insurance?");			i++;
   InsuranceQuestionDialogT = XtCreateManagedWidget("InsuranceQuestionDialogT",
						    staticTextWidgetClass,
						    InsuranceQuestionDialog,
						    args,i);

   XtVaGetValues(InsuranceQuestionDialog,
		 XtNlowerControlArea, &w,
		 NULL);

   i=0;
   InsuranceQuestionDialogY = XtCreateManagedWidget("InsuranceQuestionDialogY",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   InsuranceQuestionDialogN = XtCreateManagedWidget("InsuranceQuestionDialogN",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   InsuranceQuestionDialogH = XtCreateManagedWidget("InsuranceQuestionDialogH",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   XtSetArg(args[i], XtNemanateWidget, rootWidget);			i++;
   ShuffleDialog = XtCreatePopupShell("ShuffleDialog",
					    noticeShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(ShuffleDialog,
		 XtNtextArea, &ShuffleDialogT,
		 NULL);
   
   XtVaGetValues(ShuffleDialog,
		 XtNcontrolArea, &w,
		 NULL);

   i=0;
   ShuffleDialogOK = XtCreateManagedWidget("ShuffleDialogOK",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   XtSetArg(args[i], XtNemanateWidget, rootWidget);			i++;
   PayoffSplitDialog = XtCreatePopupShell("PayoffSplitDialog",
					    noticeShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(PayoffSplitDialog,
		 XtNtextArea, &PayoffSplitDialogT,
		 NULL);
   
   XtVaGetValues(PayoffSplitDialog,
		 XtNcontrolArea, &w,
		 NULL);

   i=0;
   PayoffSplitDialogOK = XtCreateManagedWidget("PayoffSplitDialogOK",
					    oblongButtonWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpGeneralDialog = XtCreatePopupShell("HelpGeneralDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(HelpGeneralDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   HelpGeneralTable = XtCreateManagedWidget("HelpGeneralTable",
					    scrolledWindowWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpGeneralScrolledText = XtCreateManagedWidget("HelpGeneralScrolledText",
					    textEditWidgetClass,
					    HelpGeneralTable,
					    args, i);

   i=0;
   HelpRulesDialog = XtCreatePopupShell("HelpRulesDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(HelpRulesDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   HelpRulesTable = XtCreateManagedWidget("HelpRulesTable",
					    scrolledWindowWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpRulesScrolledText = XtCreateManagedWidget("HelpRulesScrolledText",
					    textEditWidgetClass,
					    HelpRulesTable,
					    args, i);

   i=0;
   HelpBasicStrategyDialog = XtCreatePopupShell("HelpBasicStrategyDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(HelpBasicStrategyDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   HelpBasicStrategyTable = XtCreateManagedWidget("HelpBasicStrategyTable",
					    scrolledWindowWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpBasicStrategyScrolledText =
      XtCreateManagedWidget("HelpBasicStrategyScrolledText",
			    textEditWidgetClass,
			    HelpBasicStrategyTable,
			    args, i);

   i=0;
   HelpCountingDialog = XtCreatePopupShell("HelpCountingDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(HelpCountingDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   HelpCountingTable = XtCreateManagedWidget("HelpCountingTable",
					    scrolledWindowWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpCountingScrolledText = XtCreateManagedWidget("HelpCountingScrolledText",
					    textEditWidgetClass,
					    HelpCountingTable,
					    args, i);

   i=0;
   HelpBestStrategyDialog = XtCreatePopupShell("HelpBestStrategyDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(HelpBestStrategyDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   HelpBestStrategyTable = XtCreateManagedWidget("HelpBestStrategyTable",
					    scrolledWindowWidgetClass,
					    w,
					    args, i);

   i=0;
   HelpBestStrategyScrolledText =
      XtCreateManagedWidget("HelpBestStrategyScrolledText",
			    textEditWidgetClass,
			    HelpBestStrategyTable,
			    args, i);

   i=0;
   XtSetArg(args[i], XtNemanateWidget, InsuranceQuestionDialog);	i++;
   InsuranceHelpDialog = XtCreatePopupShell("InsuranceHelpDialog",
					    popupWindowShellWidgetClass,
					    InsuranceQuestionDialog,
					    args, i);

   XtVaGetValues(InsuranceHelpDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   InsuranceHelpTable = XtCreateManagedWidget("InsuranceHelpTable",
					    scrolledWindowWidgetClass,
					    w,
					    args, i);

   i=0;
   InsuranceHelpScrolledText = XtCreateManagedWidget("InsuranceHelpScrolledText",
					    textEditWidgetClass,
					    InsuranceHelpTable,
					    args, i);

   i=0;
   SplitStrategyDialog = XtCreatePopupShell("SplitStrategyDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(SplitStrategyDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   SplitStrategyTable = XtCreateManagedWidget("SplitStrategyTable",
					    bulletinBoardWidgetClass,
					    w,
					    args, i);

   i=0;
   DoubleStrategyDialog = XtCreatePopupShell("DoubleStrategyDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(DoubleStrategyDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   DoubleStrategyTable = XtCreateManagedWidget("DoubleStrategyTable",
					    bulletinBoardWidgetClass,
					    w,
					    args, i);

   i=0;
   SoftStrategyDialog = XtCreatePopupShell("SoftStrategyDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(SoftStrategyDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   SoftStrategyTable = XtCreateManagedWidget("SoftStrategyTable",
					    bulletinBoardWidgetClass,
					    w,
					    args, i);

   i=0;
   HardStrategyDialog = XtCreatePopupShell("HardStrategyDialog",
					    popupWindowShellWidgetClass,
					    rootWidget,
					    args, i);

   XtVaGetValues(HardStrategyDialog,
		 XtNupperControlArea, &w,
		 NULL);
   
   i=0;
   HardStrategyTable = XtCreateManagedWidget("HardStrategyTable",
					    bulletinBoardWidgetClass,
					    w,
					    args, i);

}

void
AddCallbacks()
{

   XtAddCallback(AutoplayScale, XmNvalueChangedCallback,
		 AutoplayScaleCB, NULL);

   XtAddCallback(YourBetScale, XmNvalueChangedCallback,
		 YourBetScaleCB, NULL);
   XtAddCallback(YourBetScale, XtNconsumeEvent,
		 SetYourBetCB, NULL);

   XtAddCallback(DealerDrawingArea, XmNexposeCallback,
		 DealerDrawingAreaCB, NULL);
   XtAddCallback(DealerDrawingArea, XmNresizeCallback,
		 DealerDrawingAreaCB, NULL);

   XtAddCallback(PlayerDrawingArea, XmNexposeCallback,
		 PlayerDrawingAreaCB, NULL);
   XtAddCallback(PlayerDrawingArea, XmNresizeCallback,
		 PlayerDrawingAreaCB, NULL);

   XtAddCallback(DealPushButton, XmNactivateCallback, DealCB, NULL);

   XtAddCallback(SplitPushButton, XmNactivateCallback, SplitCB, NULL);

   XtAddCallback(DoublePushButton, XmNactivateCallback, DoubleCB, NULL);

   XtAddCallback(HitPushButton, XmNactivateCallback, HitCB, NULL);

   XtAddCallback(StandPushButton, XmNactivateCallback, StandCB, NULL);

   XtAddCallback(StatusPushButton, XmNactivateCallback,
		 ManageHintDialogCB, NULL);

   XtAddCallback(QuitCascadeButton, XmNactivateCallback, exit, NULL);

   XtAddCallback(HelpGeneralPushButton, XmNactivateCallback,
		 ManageHelpGeneralDialogCB, NULL);

   XtAddCallback(HelpRulesPushButton, XmNactivateCallback,
		 ManageHelpRulesDialogCB, NULL);

   XtAddCallback(HelpBasicStrategyPushButton, XmNactivateCallback,
		 ManageHelpBasicStrategyDialogCB, NULL);

   XtAddCallback(HelpCountingPushButton, XmNactivateCallback,
		 ManageHelpCountingDialogCB, NULL);

   XtAddCallback(HelpBestStrategyPushButton, XmNactivateCallback,
		 ManageHelpBestStrategyDialogCB, NULL);

   XtAddCallback(HelpVersionPushButton, XmNactivateCallback,
		 VersionCB, NULL);

   XtAddCallback(InsuranceQuestionDialog, XtNverify,
		 InsuranceQuestionPopdown, NULL);
   XtAddCallback(InsuranceQuestionDialogY, XtNselect,
		 YesInsuranceCB, NULL);
   XtAddCallback(InsuranceQuestionDialogN, XtNselect,
		 NoInsuranceCB, NULL);
   XtAddCallback(InsuranceQuestionDialogH, XtNselect,
		 ManageInsuranceHelpDialogCB, NULL);

   XtAddCallback(ShuffleDialogOK, XtNselect, ShuffleDialogCB, NULL);

   XtAddCallback(PayoffSplitDialog, XtNpopdownCallback, OkPayoffDialogCB, NULL);

   XtAddCallback(SplitStrategyDialog, XtNpopupCallback, PopupCB,
		 &SplitDialogIsUp);
   XtAddCallback(SplitStrategyDialog, XtNpopdownCallback, PopdownCB,
		 &SplitDialogIsUp);
   XtAddCallback(DoubleStrategyDialog, XtNpopupCallback, PopupCB,
		 &DoubleDialogIsUp);
   XtAddCallback(DoubleStrategyDialog, XtNpopdownCallback, PopdownCB,
		 &DoubleDialogIsUp);
   XtAddCallback(SoftStrategyDialog, XtNpopupCallback, PopupCB,
		 &SoftDialogIsUp);
   XtAddCallback(SoftStrategyDialog, XtNpopdownCallback, PopdownCB,
		 &SoftDialogIsUp);
   XtAddCallback(HardStrategyDialog, XtNpopupCallback, PopupCB,
		 &HardDialogIsUp);
   XtAddCallback(HardStrategyDialog, XtNpopdownCallback, PopdownCB,
		 &HardDialogIsUp);

}

#else
CreateMotifWidgets()
{
	int i;
	char str[20];

	PanedWindow = XmCreatePanedWindow(rootWidget, "PanedWindow", NULL, 0);
	XtManageChild(PanedWindow);

	MainWindow = XmCreateMainWindow(PanedWindow, "MainWindow", NULL, 0);
	XtManageChild(MainWindow);

	MainTable = XtCreateManagedWidget("MainTable",
				tableWidgetClass, MainWindow, NULL, 0);

	ControlTable = XtCreateManagedWidget("ControlTable",
				tableWidgetClass, MainTable, NULL, 0);
					
	DealerDrawingArea = XmCreateDrawingArea(MainTable,
				"DealerDrawingArea", NULL, 0);
	XtManageChild(DealerDrawingArea);
	XtAddCallback(DealerDrawingArea, XmNexposeCallback,
				DealerDrawingAreaCB, NULL);
	XtAddCallback(DealerDrawingArea, XmNresizeCallback,
				DealerDrawingAreaCB, NULL);

	PlayerDrawingArea = XmCreateDrawingArea(MainTable,
				"PlayerDrawingArea", NULL, 0);
	XtManageChild(PlayerDrawingArea);
	XtAddCallback(PlayerDrawingArea, XmNexposeCallback,
				PlayerDrawingAreaCB, NULL);
	XtAddCallback(PlayerDrawingArea, XmNresizeCallback,
				PlayerDrawingAreaCB, NULL);

	HintMsgLabel = XmCreateLabel(MainTable, "HintMsgLabel", NULL, 0);
	XtManageChild(HintMsgLabel);

	AutoplayScale = XmCreateScale(MainTable, "AutoplayScale", NULL, 0);
	i = 0;
	XtSetArg(args[i], XmNtraversalOn, False);	i++;
	XtSetArg(args[i], XmNminimum, TIMEOUT);		i++;
	XtSetArg(args[i], XmNvalue, TIMEOUT);		i++;
	XtSetValues(AutoplayScale, args, i);
	XtManageChild(AutoplayScale);
	XtAddCallback(AutoplayScale, XmNvalueChangedCallback,
				AutoplayScaleCB, NULL);
	XtAddCallback(AutoplayScale, XmNdragCallback,
				AutoplayScaleCB, NULL);
	XtSetSensitive(AutoplayScale, True);

				/*	c, r,cs,rs, opts	*/
	XtTblConfig(ControlTable,	0, 0, 2, 3, TBL_SM_WIDTH);
	XtTblConfig(DealerDrawingArea,	2, 0, 1, 1, TBL_DEF_OPT);
	XtTblConfig(HintMsgLabel,	2, 1, 1, 1, TBL_SM_HEIGHT);
	XtTblConfig(PlayerDrawingArea,	2, 2, 1, 1, TBL_DEF_OPT);
	XtTblConfig(AutoplayScale,	3, 0, 1, 3, TBL_SM_WIDTH);

	i = 0;
	XtSetArg(args[i], XmNtraversalOn, False);		i++;
	XtSetArg(args[i], XmNskipAdjust, True);			i++;
	StatusPushButton = XmCreatePushButton(PanedWindow,
				"StatusPushButton", args, i);
	XtManageChild(StatusPushButton);
	XtAddCallback(StatusPushButton, XmNactivateCallback,
				ManageHintDialogCB, NULL);

	MenuBar = XmCreateMenuBar(MainWindow, "MenuBar", NULL, 0);

	FileCascadeButton = XmCreateCascadeButton(MenuBar,
				"FileCascadeButton", NULL, 0);
	FilePulldownMenu = XmCreatePulldownMenu(FileCascadeButton,
				"FilePulldownMenu", NULL, 0);
	i = 0;
	XtSetArg(args[i], XmNsubMenuId, FilePulldownMenu);	i++;
	XtSetValues(FileCascadeButton, args, i);
	XtManageChild(FileCascadeButton);

	QuitCascadeButton = XmCreateCascadeButton(FilePulldownMenu,
				"QuitCascadeButton", NULL, 0);
	XtManageChild(QuitCascadeButton);
	XtAddCallback(QuitCascadeButton, XmNactivateCallback, exit, NULL);

	HelpCascadeButton = XmCreateCascadeButton(MenuBar,
				"HelpCascadeButton", NULL, 0);

	HelpPulldownMenu = XmCreatePulldownMenu(FilePulldownMenu,
				"HelpPulldownMenu", NULL, 0);
	i = 0;
	XtSetArg(args[i], XmNsubMenuId, HelpPulldownMenu);	i++;
	XtSetValues(HelpCascadeButton, args, i);
	XtManageChild(HelpCascadeButton);

	i = 0;
	XtSetArg(args[i], XmNmenuHelpWidget, HelpCascadeButton); i++;
	XtSetValues(MenuBar, args, i);
	XtManageChild(MenuBar);

	HelpGeneralPushButton = XmCreatePushButton(HelpPulldownMenu,
				"HelpGeneralPushButton", NULL, 0);
	XtManageChild(HelpGeneralPushButton);
	XtAddCallback(HelpGeneralPushButton, XmNactivateCallback,
				ManageHelpGeneralDialogCB, NULL);

	HelpGeneralDialog =  XmCreateFormDialog(rootWidget,
				"HelpGeneralDialog", NULL, 0);

	HelpGeneralTable = XtCreateManagedWidget("HelpGeneralTable",
				tableWidgetClass, HelpGeneralDialog,
				NULL, 0);

	HelpGeneralScrolledText = XmCreateScrolledText(HelpGeneralTable,
				"HelpGeneralScrolledText",
				NULL, 0);
	XtManageChild(HelpGeneralScrolledText);

	HelpGeneralDismissPushButton = XmCreatePushButton(HelpGeneralTable,
				"HelpGeneralDismissPushButton",
				NULL, 0);
	XtManageChild(HelpGeneralDismissPushButton);
	XtAddCallback(HelpGeneralDismissPushButton, XmNactivateCallback,
				UnmanageHelpGeneralDialogCB, NULL);

	XtTblConfig(HelpGeneralDismissPushButton, 0, 1, 1, 1,
				TBL_SM_HEIGHT | TBL_LK_HEIGHT);
	XtTblConfig(XtParent(HelpGeneralScrolledText), 0, 0, 1, 1,
				TBL_DEF_OPT);

	HelpRulesPushButton = XmCreatePushButton(HelpPulldownMenu,
				"HelpRulesPushButton", NULL, 0);
	XtManageChild(HelpRulesPushButton);
	XtAddCallback(HelpRulesPushButton, XmNactivateCallback,
				ManageHelpRulesDialogCB, NULL);

	HelpRulesDialog =  XmCreateFormDialog(rootWidget,
				"HelpRulesDialog", NULL, 0);

	HelpRulesTable = XtCreateManagedWidget("HelpRulesTable",
				tableWidgetClass, HelpRulesDialog,
				NULL, 0);

	HelpRulesScrolledText = XmCreateScrolledText(HelpRulesTable,
				"HelpRulesScrolledText",
				NULL, 0);
	XtManageChild(HelpRulesScrolledText);

	HelpRulesDismissPushButton = XmCreatePushButton(HelpRulesTable,
				"HelpRulesDismissPushButton",
				NULL, 0);
	XtManageChild(HelpRulesDismissPushButton);
	XtAddCallback(HelpRulesDismissPushButton, XmNactivateCallback,
				UnmanageHelpRulesDialogCB, NULL);

	XtTblConfig(HelpRulesDismissPushButton, 0, 1, 1, 1,
				TBL_SM_HEIGHT | TBL_LK_HEIGHT);
	XtTblConfig(XtParent(HelpRulesScrolledText), 0, 0, 1, 1,
				TBL_DEF_OPT);

	HelpBasicStrategyPushButton = XmCreatePushButton(HelpPulldownMenu,
				"HelpBasicStrategyPushButton", NULL, 0);
	XtManageChild(HelpBasicStrategyPushButton);
	XtAddCallback(HelpBasicStrategyPushButton, XmNactivateCallback,
				ManageHelpBasicStrategyDialogCB, NULL);

	HelpBasicStrategyDialog =  XmCreateFormDialog(rootWidget,
				"HelpBasicStrategyDialog", NULL, 0);

	HelpBasicStrategyTable = XtCreateManagedWidget(
				"HelpBasicStrategyTable",
				tableWidgetClass, HelpBasicStrategyDialog,
				NULL, 0);

	HelpBasicStrategyScrolledText = XmCreateScrolledText(
				HelpBasicStrategyTable,
				"HelpBasicStrategyScrolledText",
				NULL, 0);
	XtManageChild(HelpBasicStrategyScrolledText);

	HelpBasicStrategyDismissPushButton = XmCreatePushButton(
				HelpBasicStrategyTable,
				"HelpBasicStrategyDismissPushButton",
				NULL, 0);
	XtManageChild(HelpBasicStrategyDismissPushButton);
	XtAddCallback(HelpBasicStrategyDismissPushButton, XmNactivateCallback,
				UnmanageHelpBasicStrategyDialogCB, NULL);

	XtTblConfig(HelpBasicStrategyDismissPushButton,	0, 1, 1, 1,
				TBL_SM_HEIGHT | TBL_LK_HEIGHT);
	XtTblConfig(XtParent(HelpBasicStrategyScrolledText), 0, 0, 1, 1,
				TBL_DEF_OPT);

	HelpCountingPushButton = XmCreatePushButton(HelpPulldownMenu,
				"HelpCountingPushButton", NULL, 0);
	XtManageChild(HelpCountingPushButton);
	XtAddCallback(HelpCountingPushButton, XmNactivateCallback,
				ManageHelpCountingDialogCB, NULL);

	HelpCountingDialog =  XmCreateFormDialog(rootWidget,
				"HelpCountingDialog", NULL, 0);

	HelpCountingTable = XtCreateManagedWidget("HelpCountingTable",
				tableWidgetClass, HelpCountingDialog,
				NULL, 0);

	HelpCountingScrolledText = XmCreateScrolledText(HelpCountingTable,
				"HelpCountingScrolledText",
				NULL, 0);
	XtManageChild(HelpCountingScrolledText);

	HelpCountingDismissPushButton = XmCreatePushButton( HelpCountingTable,
				"HelpCountingDismissPushButton",
				NULL, 0);
	XtManageChild(HelpCountingDismissPushButton);
	XtAddCallback(HelpCountingDismissPushButton, XmNactivateCallback,
				UnmanageHelpCountingDialogCB, NULL);

	XtTblConfig(HelpCountingDismissPushButton, 0, 1, 1, 1,
				TBL_SM_HEIGHT | TBL_LK_HEIGHT);
	XtTblConfig(XtParent(HelpCountingScrolledText), 0, 0, 1, 1,
				TBL_DEF_OPT);

	HelpBestStrategyPushButton = XmCreatePushButton(HelpPulldownMenu,
				"HelpBestStrategyPushButton", NULL, 0);
	XtManageChild(HelpBestStrategyPushButton);
	XtAddCallback(HelpBestStrategyPushButton, XmNactivateCallback,
				ManageHelpBestStrategyDialogCB, NULL);

	HelpBestStrategyDialog =  XmCreateFormDialog(rootWidget,
				"HelpBestStrategyDialog", NULL, 0);

	HelpBestStrategyTable = XtCreateManagedWidget(
				"HelpBestStrategyTable",
				tableWidgetClass, HelpBestStrategyDialog,
				NULL, 0);

	HelpBestStrategyScrolledText = XmCreateScrolledText(
				HelpBestStrategyTable,
				"HelpBestStrategyScrolledText",
				NULL, 0);
	XtManageChild(HelpBestStrategyScrolledText);

	HelpBestStrategyDismissPushButton = XmCreatePushButton(
				HelpBestStrategyTable,
				"HelpBestStrategyDismissPushButton",
				NULL, 0);
	XtManageChild(HelpBestStrategyDismissPushButton);
	XtAddCallback(HelpBestStrategyDismissPushButton, XmNactivateCallback,
				UnmanageHelpBestStrategyDialogCB, NULL);

	XtTblConfig(HelpBestStrategyDismissPushButton, 0, 1, 1, 1,
				TBL_SM_HEIGHT | TBL_LK_HEIGHT);
	XtTblConfig(XtParent(HelpBestStrategyScrolledText), 0, 0, 1, 1,
				TBL_DEF_OPT);


	HelpVersionPushButton = XmCreatePushButton(HelpPulldownMenu,
				"HelpVersionPushButton", NULL, 0);
	XtManageChild(HelpVersionPushButton);
	XtAddCallback(HelpVersionPushButton, XmNactivateCallback,
				VersionCB, NULL);

	WonLabel = XmCreateLabel(ControlTable, "WonLabel", NULL, 0);
	XtManageChild(WonLabel);
	WonLabelValue = XmCreateLabel(ControlTable, "WonLabelValue", NULL, 0);
	XtManageChild(WonLabelValue);
	WonLabelPercent = XmCreateLabel(ControlTable, "WonLabelPercent",
				NULL, 0);
	XtManageChild(WonLabelPercent);

	LostLabel = XmCreateLabel(ControlTable, "LostLabel", NULL, 0);
	XtManageChild(LostLabel);
	LostLabelValue = XmCreateLabel(ControlTable, "LostLabelValue", NULL, 0);
	XtManageChild(LostLabelValue);
	LostLabelPercent = XmCreateLabel(ControlTable, "LostLabelPercent",
				NULL, 0);
	XtManageChild(LostLabelPercent);

	PushedLabel = XmCreateLabel(ControlTable, "PushedLabel", NULL, 0);
	XtManageChild(PushedLabel);
	PushedLabelValue = XmCreateLabel(ControlTable, "PushedLabelValue",
				NULL, 0);
	XtManageChild(PushedLabelValue);
	PushedLabelPercent = XmCreateLabel(ControlTable, "PushedLabelPercent",
				NULL, 0);
	XtManageChild(PushedLabelPercent);

	YourBetScale = XmCreateScale(ControlTable, "YourBetScale", NULL, 0);
	i = 0;
	XtSetArg(args[i], XmNmaximum, bankroll);	i++;
	XtSetValues(YourBetScale, args, i);
	XtManageChild(YourBetScale);
	XtAddCallback(YourBetScale, XmNvalueChangedCallback,
				YourBetScaleCB, NULL);

	YouHaveLabel = XmCreateLabel(ControlTable, "YouHaveLabel", NULL, 0);
	XtManageChild(YouHaveLabel);
	i = 0;
	XtSetArg(args[i], XmNmaximum, bankroll);	i++;
	XtSetValues(YourBetScale, args, i);
	YouHaveLabelValue = XmCreateLabel(ControlTable,
				"YouHaveLabelValue", NULL, 0);
        sprintf(str, "$%g", bankroll);
        SetLabel(YouHaveLabelValue, str);
	XtManageChild(YouHaveLabelValue);
	i = 0;
	XtSetArg(args[i], XmNmaximum, bankroll);	i++;
	XtSetValues(YourBetScale, args, i);

	PlaceHolderLabel = XmCreateLabel(ControlTable, "PlaceHolderLabel",
				NULL, 0);
	XtManageChild(PlaceHolderLabel);

	i = 0;
	XtSetArg(args[i], XmNsensitive, False);			i++;
	SplitPushButton = XmCreatePushButton(ControlTable,
					"SplitPushButton", args, i);
	XtAddCallback(SplitPushButton, XmNactivateCallback, SplitCB, NULL);
	XtManageChild(SplitPushButton);

	i = 0;
	XtSetArg(args[i], XmNsensitive, False);			i++;
	DoublePushButton = XmCreatePushButton(ControlTable,
					"DoublePushButton", args, i);
	XtAddCallback(DoublePushButton, XmNactivateCallback, DoubleCB, NULL);
	XtManageChild(DoublePushButton);

	i = 0;
	XtSetArg(args[i], XmNsensitive, False);			i++;
	HitPushButton = XmCreatePushButton(ControlTable,
					"HitPushButton", args, i);
	XtAddCallback(HitPushButton, XmNactivateCallback, HitCB, NULL);
	XtManageChild(HitPushButton);

	i = 0;
	XtSetArg(args[i], XmNsensitive, False);			i++;
	StandPushButton = XmCreatePushButton(ControlTable,
					"StandPushButton", args, i);
	XtAddCallback(StandPushButton, XmNactivateCallback, StandCB, NULL);
	XtManageChild(StandPushButton);

				/*	c, r,cs,rs, opts	*/
	XtTblConfig(WonLabel,		0, 1, 1, 1,
		TBL_RIGHT | TBL_LK_HEIGHT | TBL_SM_HEIGHT | TBL_SM_WIDTH);
	XtTblConfig(WonLabelValue,	1, 1, 1, 1,
		TBL_LEFT | TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(WonLabelPercent,	2, 1, 1, 1,
		TBL_LEFT | TBL_LK_HEIGHT | TBL_SM_HEIGHT | TBL_LK_WIDTH);

	XtTblConfig(LostLabel,		0, 2, 1, 1,
		TBL_RIGHT | TBL_LK_HEIGHT | TBL_SM_HEIGHT | TBL_SM_WIDTH);
	XtTblConfig(LostLabelValue,	1, 2, 1, 1,
		TBL_LEFT | TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(LostLabelPercent,	2, 2, 1, 1,
		TBL_LEFT | TBL_LK_HEIGHT | TBL_SM_HEIGHT | TBL_LK_WIDTH);

	XtTblConfig(PushedLabel,	0, 3, 1, 1,
		TBL_RIGHT | TBL_LK_HEIGHT | TBL_SM_HEIGHT | TBL_SM_WIDTH);
	XtTblConfig(PushedLabelValue,	1, 3, 1, 1,
		TBL_LEFT | TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(PushedLabelPercent,	2, 3, 1, 1,
		TBL_LEFT | TBL_LK_HEIGHT | TBL_SM_HEIGHT | TBL_LK_WIDTH);

	XtTblConfig(SplitPushButton,	0, 5, 3, 1,
		TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(DoublePushButton,	0, 6, 3, 1,
		TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(HitPushButton,	0, 7, 3, 1,
		TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(StandPushButton,	0, 8, 3, 1,
		TBL_LK_HEIGHT | TBL_SM_HEIGHT);

	XtTblConfig(YourBetScale,	0, 10, 3, 1,
		TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(YouHaveLabel,	0, 11, 1, 1,
		TBL_RIGHT | TBL_LK_HEIGHT | TBL_SM_HEIGHT | TBL_SM_WIDTH);
	XtTblConfig(YouHaveLabelValue,	1, 11, 2, 1,
		TBL_RIGHT | TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(PlaceHolderLabel,	1, 14, 3, 1,
		TBL_RIGHT | TBL_LK_HEIGHT | TBL_SM_HEIGHT);

	InsuranceQuestionDialog = XmCreateQuestionDialog(rootWidget,
				"InsuranceQuestionDialog", NULL, 0);
	XtAddCallback(InsuranceQuestionDialog, XmNokCallback,
				YesInsuranceCB, NULL);
	XtAddCallback(InsuranceQuestionDialog, XmNcancelCallback,
				NoInsuranceCB, NULL);
	XtAddCallback(InsuranceQuestionDialog, XmNhelpCallback,
				ManageInsuranceHelpDialogCB, NULL);

	InsuranceHelpDialog = XmCreateFormDialog(rootWidget,
				"InsuranceHelpDialog", NULL, 0);

	InsuranceHelpTable = XtCreateManagedWidget("InsuranceHelpTable",
				tableWidgetClass, InsuranceHelpDialog,
				NULL, 0);

	InsuranceHelpDismissPushButton = XmCreatePushButton(InsuranceHelpTable,
				"InsuranceHelpDismissPushButton", NULL, 0);
	XtManageChild(InsuranceHelpDismissPushButton);
	XtAddCallback(InsuranceHelpDismissPushButton,XmNactivateCallback,
				UnmanageInsuranceHelpDialogCB, NULL);

	InsuranceHelpScrolledText = XmCreateScrolledText(InsuranceHelpTable,
				"InsuranceHelpScrolledText", NULL, 0);
	XtManageChild(InsuranceHelpScrolledText);

	XtTblConfig(InsuranceHelpDismissPushButton, 0, 1, 1, 1,
				TBL_LK_HEIGHT | TBL_SM_HEIGHT);
	XtTblConfig(XtParent(InsuranceHelpScrolledText), 0, 0, 1, 1,
				TBL_DEF_OPT);

	PayoffSplitDialog = XmCreateInformationDialog(rootWidget,
				"PayoffSplitDialog", NULL, 0);
	XtAddCallback(PayoffSplitDialog, XmNokCallback, OkPayoffDialogCB, NULL);

	ShuffleDialog = XmCreateInformationDialog(rootWidget,
			"ShuffleDialog", NULL, 0);
	XtAddCallback(ShuffleDialog, XmNfocusCallback, ShuffleDialogCB, NULL);

	HardStrategyDialog = XmCreateFormDialog(rootWidget,
			"HardStrategyDialog", NULL, 0);
	HardStrategyTable = XtCreateManagedWidget("HardStrategyTable",
				tableWidgetClass, HardStrategyDialog,
				NULL, 0);

	SoftStrategyDialog = XmCreateFormDialog(rootWidget,
			"SoftStrategyDialog", NULL, 0);
	SoftStrategyTable = XtCreateManagedWidget("SoftStrategyTable",
				tableWidgetClass, SoftStrategyDialog,
				NULL, 0);

	DoubleStrategyDialog = XmCreateFormDialog(rootWidget,
			"DoubleStrategyDialog", NULL, 0);
	DoubleStrategyTable = XtCreateManagedWidget("DoubleStrategyTable",
				tableWidgetClass, DoubleStrategyDialog,
				NULL, 0);

	SplitStrategyDialog = XmCreateFormDialog(rootWidget,
			"SplitStrategyDialog", NULL, 0);
	SplitStrategyTable = XtCreateManagedWidget("SplitStrategyTable",
				tableWidgetClass, SplitStrategyDialog,
				NULL, 0);
}
#endif /* OLIT */


SetIcon(dpy)


	Display *dpy;
{
	Pixmap IconPixmap;
	int i;

	IconPixmap = XCreateBitmapFromData(dpy,
				RootWindow(dpy, DefaultScreen(dpy)),
				icon_bits, icon_width, icon_height);
	i = 0;
	XtSetArg(args[i], XmNiconPixmap, IconPixmap);	i++;
	XtSetValues(rootWidget, args, i);
}

main (argc, argv)
	int argc;
	char *argv[];
{
	Display *dpy;
	int scr;

#ifdef OLIT
	rootWidget = OlInitialize(
#else
	rootWidget = XtInitialize(
#endif
		argv[0],	/* shell widget instance */
		"XBlackjack",	/* class name */
		NULL,		/* options (parm to XrmParseCommand) */
		0,		/* num_options */
		&argc,		/* number of command line parameters */
		argv);		/* command line */

	dpy = XtDisplay(rootWidget);
	scr = DefaultScreen(dpy);
	app = XtWidgetToApplicationContext(rootWidget);

/* get default resources */
	XtGetApplicationResources(rootWidget, (unsigned char *)&rv, resources,
					XtNumber(resources), NULL, 0);

/* Initialize after getting application resources so user can set initial
   bankroll */
	initialize();

#ifdef OLIT
	CreateOLITWidgets();
#else
	CreateMotifWidgets();
#endif /* OLIT */

	SetIcon(dpy);

	XtRealizeWidget(rootWidget);

#ifdef OLIT
	AddCallbacks();
#endif

#ifdef DEBUG
	XSynchronize(XtDisplay(rootWidget), True);
#endif
	draw_init(dpy, scr, DealerDrawingArea, HintMsgLabel,
			PlayerDrawingArea);

	strategy_init(dpy, SplitStrategyTable, SplitStrategyDialog, 
			DoubleStrategyTable, DoubleStrategyDialog, 
			SoftStrategyTable, SoftStrategyDialog, 
		        HardStrategyTable, HardStrategyDialog,
		        rv.bet_unit);
	UpdateWonLostAndPushed();
 	Msg("Click me for clues.  Scale on right controls autoplay.");
 	ClearHintMsg();
	shuffle_up(NULL, False);
	state = MAKING_BET_STATE;
	
	XtMainLoop();
}
