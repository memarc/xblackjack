/*
 * $Header: /usr/people/kirke/src/xblackjack/RCS/Strategy.h,v 1.7 1993/06/18 17:15:05 kirke Exp kirke $
 *
 * xblackjack -- "Complete Point-Count System" strategy table interface
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
 * $Log: Strategy.h,v $
 * Revision 1.7  1993/06/18  17:15:05  kirke
 * *** empty log message ***
 *
 * Revision 1.6  92/08/09  23:00:56  kirke
 * Changes for OLIT support from GUX (Guy Renard) grenard@elrond.stg.trw.com
 * 
 * Revision 1.5  92/08/09  18:32:43  kirke
 * Replaced copyright notice (to include Silicon Graphics, Inc).
 * 
 * Revision 1.4  92/07/04  21:52:44  kirk
 * *** empty log message ***
 * 
 * Revision 1.3  92/07/04  21:28:08  kirk
 * 
 * Revision 1.2  92/05/16  10:48:51  kirk
 * New copyright notice.
 * 
 * Revision 1.1  92/02/17  20:55:32  kirk
 * Inserted copyright notice.
 * 
 * Revision 1.0  92/01/30  22:01:26  kirk
 * Initial revision
 * 
 */

extern void strategy_init();

extern int should_split();
extern int should_double();
extern int Set_Bet_Unit();
extern int should_hit();
extern int should_stand();
extern int should_insure();
extern int should_bet();

extern void ManageSplitDialog();
extern void ManageDoubleDialog();
extern void ManageSoftDialog();
extern void ManageHardDialog();
