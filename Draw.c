/*
 * $Header: Draw.c,v 1.11 92/09/23 10:58:28 kirke Locked $
 *
 * xblackjack -- Card drawing routines (from Spider)
 *
 * (c) Copyright 1992, Kirk Erickson and Silicon Graphics Inc.
 * (c) Copyright 1990, Heather Rose and Sun Microsystems, Inc.
 * (c) Copyright 1990, David Lemke and Network Computing Devices
 * (c) Copyright 1989, Donald R. Woods and Sun Microsystems, Inc.
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

/* $Log:	Draw.c,v $
 * Revision 1.11  92/09/23  10:58:28  kirke
 * Fixed lint errors.  Extraneous var defs in draw_int, card_x() and face_down().
 * Removed unused cliprects static.
 * 
 * Revision 1.10  92/08/17  20:45:47  kirke
 * Removed rcsid header to compile on Sun cleanly.
 * 
 * Revision 1.9  92/08/12  23:33:53  kirke
 * OLIT path adjustments.
 * 
 * Revision 1.8  92/08/09  22:59:05  kirke
 * Changes for OLIT support from GUX (Guy Renard) grenard@elrond.stg.trw.com
 * 
 * Revision 1.7  92/08/09  18:32:19  kirke
 * Replaced copyright notice (to include Silicon Graphics, Inc).
 * 
 * Revision 1.6  92/07/06  01:28:09  kirk
 * *** empty log message ***
 * 
 * Revision 1.5  92/07/06  01:10:30  kirk
 * Setup separate what and rcstring for Main.c,
 * prefix from from rcs strings.
 * 
 * Revision 1.4  92/07/04  21:26:40  kirk
 * Removed XtVa calls (to avoid malloc/performance hit).
 * 
 * Revision 1.3  92/05/16  10:47:50  kirk
 * New copyright notice.
 * 
 * Revision 1.2  92/02/17  20:58:49  kirk
 * Added hint message widget to draw_init parameter list.  Now allocating,
 * and setting fg/bg.  Using white on black when not color.
 * 
 * Revision 1.1  92/01/30  21:47:33  kirk
 * Passing 'win' Window argument to lower level routines for separate
 * dealer and player drawing areas.
 * 
 * Revision 1.0  92/01/04  21:16:10  kirk
 * Initial revision
 * 
 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xlib.h>
#include <X11/Xmu/Drawing.h>
#include "Motif2OLIT.h"
#ifdef OLIT
#include <Xol/OpenLook.h>
#else
#include <X11/Xutil.h>
#include <X11/Xm/Xm.h>
#endif
#include "Assert.h"
#include "Bitmaps.h"
#include "Draw.h"
#include "Card.h"

typedef enum	{Spade, Heart, Diamond, Club}	Suit;

typedef enum	{Ace, Deuce, Three, Four, Five, Six, Seven,
		 Eight, Nine, Ten, Jack, Queen, King}	Rank;

#define DEALER 0
#define PLAYER 1

#define	NUM_RANKS	13
#define	NUM_SUITS	4

#define	CARD_HEIGHT	123
#define	CARD_WIDTH	79

#define	FACECARD_WIDTH	47
#define	FACECARD_HEIGHT	92

#define	RANK_LOC_X	4
#define	RANK_LOC_Y	7

#define	SMALL_LOC_X	4
#define	SMALL_LOC_Y	(rank_height + RANK_LOC_Y + 3)

#define	MID_CARD_X	(CARD_WIDTH/2)
#define	MID_CARD_Y	(CARD_HEIGHT/2)

#define	CARD_COL1_X	(3 * CARD_WIDTH/10)
#define	CARD_COL2_X	(CARD_WIDTH/2)
#define	CARD_COL3_X	(7 * CARD_WIDTH/10)

/* 5 diff rows for the two main columns */
/* 1 and 5 are top and bottom, 3 is the middle */
/* 2 & 4 are for the 10 & 9 */
#define	CARD_ROW1_Y	(CARD_HEIGHT/5)
#define	CARD_ROW2_Y	(2 * CARD_HEIGHT/5)
#define	CARD_ROW3_Y	(CARD_HEIGHT/2)
#define	CARD_ROW4_Y	(CARD_HEIGHT - 2 * CARD_HEIGHT/5)
#define	CARD_ROW5_Y	(CARD_HEIGHT - CARD_HEIGHT/5)

/* between 1 & 3, 3 & 5 */
#define	CARD_SEVEN_Y	(7 * CARD_HEIGHT/20)
#define	CARD_EIGHT_Y	(CARD_HEIGHT - 7 * CARD_HEIGHT/20)

/* between rows 1 & 2, 4 & 5 */
#define	CARD_TEN_Y1	(3 * CARD_HEIGHT/10)
#define	CARD_TEN_Y2	(CARD_HEIGHT - 3 * CARD_HEIGHT/10)

/* card positioning */
#define	CARD_INSET_X	10
#define	CARD_INSET_Y	(CARD_HEIGHT/8)

/* pip info */
#define	PIP_WIDTH	10
#define	PIP_HEIGHT	10

Display	*dpy;
int screen;
Widget DealerDrawingArea;
Widget HintMsgLabel;
Widget PlayerDrawingArea;
Window DealerWindow;
Window PlayerWindow;

Pixmap	greenmap;
Pixmap	redmap;
Pixmap	logomap;

Pixel blackpixel;
Pixel whitepixel;
Pixel borderpixel;
Pixel greenpixel;
Pixel redpixel;

Bool	is_color;

/* function decls */
char	*rank_name();
char	*ank_name();
char	*suit_name();
char	*get_selection();
char	*remove_newlines();

extern char	*calloc();


#define	ROUND_W	7
#define	ROUND_H	7

Bool	round_cards = True;

static GC	cardgc;		/* gc in use when drawing cards */

static Pixmap	rank_map[NUM_RANKS],	rank_r_map[NUM_RANKS];
static Pixmap	rank_map_red[NUM_RANKS], rank_r_map_red[NUM_RANKS];
static Pixmap	suit_map[NUM_SUITS],	suit_r_map[NUM_SUITS];
static Pixmap	suit_sm_map[NUM_SUITS],	suit_sm_r_map[NUM_SUITS];
static Pixmap	suit_lg_map[NUM_SUITS];
static Pixmap	jack_map[NUM_SUITS], queen_map[NUM_SUITS], king_map[NUM_SUITS];

static GC	redgc;
static GC	blackgc;
static GC	whitegc;
static GC	backgc;

		/* how much to modify the TS origin by */
static int 	back_delta_x, back_delta_y;

static Bool	card_is_clipped;	/* optimizer for card drawing */

static	char	*rnk_names[] =	{
	"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
};

/*
 * shortened version for save files and info
 */
char	*
rnk_name(rank)
Rank	rank;
{
	assert(rank >= Ace && rank <= King);

	return (rnk_names[rank]);
}

static char	*rank_names[] =	{
	"Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven",
	"Eight", "Nine", "Ten", "Jack", "Queen", "King"
};

char	*
rank_name(rank)
Rank	rank;
{
	assert(rank >= Ace && rank <= King);

	return (rank_names[rank]);
}

static char	*suit_names[] = {
	"Spades", "Hearts", "Diamonds", "Clubs"
}; 

char	*
suit_name(suit)
Suit	suit;
{
	assert (suit >= Spade && suit <= Club);

	return (suit_names[suit]);
}


void
draw_init(d, scr, dw, hw, pw)
	Display	*d;
	int	scr;
	Widget  dw;	/* dealer's card drawing area */
	Widget  hw;	/* hint between drawing areas */
	Widget  pw;	/* player's card drawing area */
{
	XGCValues	gcv;
	long		gcflags;
	XColor		color;
	Colormap	cmap;
	Pixmap		tmpmap;
	GC		logogc;
	int		i;
	Arg		args[10];

	/* save these off */
	dpy = d;
	screen = scr;
	DealerDrawingArea = dw;
	HintMsgLabel = hw;
	PlayerDrawingArea = pw;
	DealerWindow = XtWindow(DealerDrawingArea);
	PlayerWindow = XtWindow(PlayerDrawingArea);
	
	if (DisplayCells(dpy, screen) > 2)	{
		is_color = True;
	}
	else {
		is_color = False;
	}

	blackpixel = BlackPixel(dpy, screen);
	whitepixel = WhitePixel(dpy, screen);

	/* make gc for white */
	gcv.foreground = WhitePixel(dpy, screen);
	gcv.background = BlackPixel(dpy, screen);
	gcv.graphics_exposures = False;
	gcflags = GCForeground | GCBackground | GCGraphicsExposures;

	whitegc = XCreateGC(dpy, RootWindow(dpy, screen), gcflags, &gcv);

	/* make gc for black */
	gcv.foreground = BlackPixel(dpy, screen);
	gcv.background = WhitePixel(dpy, screen);
	gcflags = GCForeground | GCBackground | GCGraphicsExposures;

	blackgc = XCreateGC(dpy, RootWindow(dpy, screen), gcflags, &gcv);

	tmpmap = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		logo_bits, logo_width, logo_height);

	logomap = XCreatePixmap(dpy, RootWindow(dpy, screen), logo_width,
		logo_height, DefaultDepth(dpy, screen));

	back_delta_x = (CARD_WIDTH - logo_width)/2;
	back_delta_y = (CARD_HEIGHT - logo_height)/2;

	if (is_color)	{
		cmap = DefaultColormap(dpy, screen);
		XAllocNamedColor(dpy, cmap, "Sea Green", &color, &color);
		gcv.foreground = color.pixel;
		gcv.background = WhitePixel(dpy, screen);
		gcflags = GCForeground | GCBackground;
		logogc = XCreateGC(dpy, RootWindow(dpy, screen), gcflags, &gcv);
		XCopyPlane(dpy, tmpmap, logomap, logogc, 0, 0, 
			logo_width, logo_height, 0, 0, 1);
		XFreeGC(dpy, logogc);

		/* allocate drawing area background color, set areas */
		XAllocNamedColor(dpy, cmap, "Forest Green", &color, &color);
		i = 0;
		XtSetArg(args[i], XmNbackground, color.pixel);   i++;
		XtSetArg(args[i], XmNforeground, whitepixel);   i++;
		XtSetValues(DealerDrawingArea, args, i);
		XtSetValues(HintMsgLabel, args, i);
		XtSetValues(PlayerDrawingArea, args, i);
		
	} else	{
		XCopyPlane(dpy, tmpmap, logomap, whitegc, 0, 0, 
			logo_width, logo_height, 0, 0, 1);
		i = 0;
		XtSetArg(args[i], XmNbackground, blackpixel);   i++;
		XtSetArg(args[i], XmNforeground, whitepixel);   i++;
		XtSetValues(DealerDrawingArea, args, i);
		XtSetValues(HintMsgLabel, args, i);
		XtSetValues(PlayerDrawingArea, args, i);
	}
	XFreePixmap(dpy, tmpmap);

	gcv.tile = logomap;
	gcv.fill_style = FillTiled;
	gcflags |= GCTile | GCFillStyle | GCGraphicsExposures;

	backgc = XCreateGC(dpy, RootWindow(dpy, screen), gcflags, &gcv);

	borderpixel = blackpixel;

	if (is_color)	{
		cmap = DefaultColormap(dpy, screen);

		color.flags = DoRed | DoGreen | DoBlue;

		/*
		 * color levels are the NeWS RGB values
		 */
		color.red = 13107;	/* 0.2 */
		color.green = 52428;	/* 0.8 */
		color.blue = 39321;	/* 0.6 */

		XAllocColor(dpy, cmap, &color);
		greenpixel = color.pixel;

		color.red = 52428;	/* 0.8 */
		color.green = color.blue = 0;
		XAllocColor(dpy, cmap, &color);
		redpixel = color.pixel;

		gcv.foreground = redpixel;
		gcv.background = WhitePixel(dpy, screen);
		gcflags = GCForeground | GCBackground | GCGraphicsExposures;

		redgc = XCreateGC(dpy, RootWindow(dpy, screen), gcflags, &gcv);

	} else	{
		greenmap = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
			gray1_bits, gray1_width, gray1_height);

		redmap = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
			gray1_bits, gray1_width, gray1_height);

		gcv.tile = redmap;
		gcv.fill_style = FillTiled;

		gcv.foreground = BlackPixel(dpy, screen);
		gcv.background = WhitePixel(dpy, screen);

		gcflags = GCTile | GCForeground | GCBackground |
			GCFillStyle | GCGraphicsExposures;

		redgc = XCreateGC(dpy, RootWindow(dpy, screen), gcflags, &gcv);
	}
	make_card_maps();
}


/*
 * make a 'red' pixmap by setting the clipmask to the desired shape and 
 * pushing 'red' through it
 */
static Pixmap
make_red_map(bits, width, height)
	char	*bits;
	int	width, height;
{
	Pixmap	tmpmap, newmap;
	static GC	cleargc = (GC) 0;
	XGCValues	xgcv;


	if (cleargc == (GC) 0)	{
		xgcv.function = GXclear;
		cleargc = XCreateGC(dpy, RootWindow(dpy, screen), GCFunction, 
								&xgcv);
	}

	tmpmap = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		bits, width, height);

	newmap = XCreatePixmap(dpy, RootWindow(dpy, screen), width, height, 1);

	/* clear pixmap */
	XFillRectangle(dpy, newmap, cleargc, 0, 0, width, height);

	XSetClipMask(dpy, redgc, tmpmap);
	XFillRectangle(dpy, newmap, redgc, 0, 0, width, height);
	XSetClipMask(dpy, redgc, None);
	XFreePixmap(dpy, tmpmap);
#ifdef OLIT
#define BIT_TYPE char *
#else
#define BIT_TYPE unsigned char *
#endif


	return (newmap);
}

make_card_maps()
{
	BIT_TYPE	new_bits;
	Rank	r;
	int	i;

	for (r = Ace; r <= King; r++)	{
		rank_map[(int)r] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			rank_bits[(int)r], rank_width, rank_height);

		new_bits = (BIT_TYPE) calloc(sizeof(rank_bits[(int)r]),
							1);
		rot_180((BIT_TYPE)rank_bits[(int)r], new_bits, 
			rank_width, rank_height);
		rank_r_map[(int)r] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			new_bits, rank_width, rank_height);
		free((char *)new_bits);
	}

	for (r = Ace; r <= King; r++)	{
		new_bits = (BIT_TYPE) calloc(sizeof(rank_bits[(int)r]),
							1);
		rot_180((BIT_TYPE)rank_bits[(int)r], new_bits, 
				rank_width, rank_height);
		if (is_color)	{
			rank_map_red[(int)r] = XCreateBitmapFromData(dpy, 
				RootWindow(dpy, screen),
				rank_bits[(int)r], rank_width, rank_height);

			rank_r_map_red[(int)r] = XCreateBitmapFromData(dpy, 
				RootWindow(dpy, screen),
				new_bits, rank_width, rank_height);
		} else	{
			rank_map_red[(int)r] = make_red_map(rank_bits[(int)r],
						rank_width, rank_height);

			rank_r_map_red[(int)r] = make_red_map((char *)new_bits, 
						rank_width, rank_height);
		}
		free((char *)new_bits);
	}

	i = (int)Spade;
	/* make all the card bitmaps */
	suit_map[i] = XCreateBitmapFromData(dpy, 
		RootWindow(dpy, screen),
		spade_bits, spade_width, spade_height);

	new_bits = (BIT_TYPE) calloc(sizeof(spade_bits), 1);
	flip_bits((BIT_TYPE)spade_bits, new_bits, spade_width, 
				spade_height);
	suit_r_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		new_bits, spade_width, spade_height);
	free((char *)new_bits);

	suit_sm_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		spade_sm_bits, spade_sm_width, spade_sm_height);

	new_bits = (BIT_TYPE) calloc(sizeof(spade_sm_bits), 1);
	flip_bits((BIT_TYPE)spade_sm_bits, new_bits, spade_sm_width,
			spade_sm_height);
	suit_sm_r_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		new_bits, spade_sm_width, spade_sm_height);
	free((char *)new_bits);

	suit_lg_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		spade_lg_bits, spade_lg_width, spade_lg_height);

	jack_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		jack_s_bits, jack_s_width, jack_s_height);

	queen_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		queen_s_bits, queen_s_width, queen_s_height);

	king_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		king_s_bits, king_s_width, king_s_height);

	i = (int)Heart;
	/* make all the card bitmaps */
	new_bits = (BIT_TYPE) calloc(sizeof(heart_bits), 1);
	flip_bits((BIT_TYPE)heart_bits, new_bits, heart_width, 
					heart_height);

	if (is_color)	{
		suit_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			heart_bits, heart_width, heart_height);
		suit_r_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			new_bits, heart_width, heart_height);
	} else	{
		suit_map[i] = make_red_map(heart_bits, heart_width, 
						heart_height);
		suit_r_map[i] = make_red_map((char *)new_bits, heart_width, 
						heart_height);
	}

	free((char *)new_bits);

	new_bits = (BIT_TYPE) calloc(sizeof(heart_sm_bits), 1);
	flip_bits((BIT_TYPE)heart_sm_bits, new_bits, heart_sm_width, 
		heart_sm_height);
	suit_sm_r_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		new_bits, heart_sm_width, heart_sm_height);

	if (is_color)	{
		suit_sm_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			heart_sm_bits, heart_sm_width, heart_sm_height);
		suit_sm_r_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			new_bits, heart_sm_width, heart_sm_height);
	} else	{
		suit_sm_map[i] = make_red_map(heart_sm_bits, heart_sm_width, 
						heart_height);
		suit_sm_r_map[i] = make_red_map((char *)new_bits, 
			heart_sm_width, heart_sm_height);
	}
	free((char *)new_bits);

	suit_lg_map[i] = suit_map[i];

	if (is_color)	{
		jack_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			jack_h_bits, jack_h_width, jack_h_height);

		queen_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			queen_h_bits, queen_h_width, queen_h_height);

		king_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			king_h_bits, king_h_width, king_h_height);
	} else	{
		jack_map[i] = make_red_map(jack_h_bits, jack_h_width, 
							jack_h_height);

		queen_map[i] = make_red_map(queen_h_bits, queen_h_width, 
							queen_h_height);

		king_map[i] = make_red_map(king_h_bits, king_h_width, 
							king_h_height);
	}


	i = (int)Diamond;
	/* make all the card bitmaps */
	new_bits = (BIT_TYPE) calloc(sizeof(diamond_bits), 1);
	flip_bits((BIT_TYPE)diamond_bits, new_bits, diamond_width, 
		diamond_height);

	if (is_color)	{
		suit_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			diamond_bits, diamond_width, diamond_height);
		suit_r_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			new_bits, diamond_width, diamond_height);
	} else	{
		suit_map[i] = make_red_map(diamond_bits, diamond_width, 
						diamond_height);
		suit_r_map[i] = make_red_map((char *)new_bits, diamond_width, 
						diamond_height);
	}

	free((char *)new_bits);

	new_bits = (BIT_TYPE) calloc(sizeof(diamond_sm_bits), 1);
	flip_bits((BIT_TYPE)diamond_sm_bits, new_bits, 
				diamond_sm_width, diamond_sm_height);
	suit_sm_r_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		new_bits, diamond_sm_width, diamond_sm_height);

	if (is_color)	{
		suit_sm_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			diamond_sm_bits, diamond_sm_width, diamond_sm_height);
		suit_sm_r_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			new_bits, diamond_sm_width, diamond_sm_height);
	} else	{
		suit_sm_map[i] = make_red_map(diamond_sm_bits, diamond_sm_width, 
						diamond_height);
		suit_sm_r_map[i] = make_red_map((char *)new_bits, 
				diamond_sm_width, diamond_sm_height);
	}
	free((char *)new_bits);

	suit_lg_map[i] = suit_map[i];

	if (is_color)	{
		jack_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			jack_d_bits, jack_d_width, jack_d_height);

		queen_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			queen_d_bits, queen_d_width, queen_d_height);

		king_map[i] = XCreateBitmapFromData(dpy, 
			RootWindow(dpy, screen),
			king_d_bits, king_d_width, king_d_height);
	} else	{
		jack_map[i] = make_red_map(jack_d_bits, jack_d_width, 
							jack_d_height);

		queen_map[i] = make_red_map(queen_d_bits, queen_d_width, 
							queen_d_height);

		king_map[i] = make_red_map(king_d_bits, king_d_width, 
							king_d_height);
	}

	i = (int)Club;
	/* make all the card bitmaps */
	suit_map[i] = XCreateBitmapFromData(dpy, 
		RootWindow(dpy, screen),
		club_bits, club_width, club_height);

	new_bits = (BIT_TYPE) calloc(sizeof(club_bits), 1);
	flip_bits((BIT_TYPE)club_bits, new_bits, club_width, 
		club_height);
	suit_r_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		new_bits, club_width, club_height);
	free((char *)new_bits);

	suit_sm_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		club_sm_bits, club_sm_width, club_sm_height);

	new_bits = (BIT_TYPE) calloc(sizeof(club_sm_bits), 1);
	flip_bits((BIT_TYPE)club_sm_bits, new_bits, club_sm_width, 
		club_sm_height);
	suit_sm_r_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		new_bits, club_sm_width, club_sm_height);
	free((char *)new_bits);

	suit_lg_map[i] = suit_map[i];


	jack_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		jack_c_bits, jack_c_width, jack_c_height);

	queen_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		queen_c_bits, queen_c_width, queen_c_height);

	king_map[i] = XCreateBitmapFromData(dpy, RootWindow(dpy, screen),
		king_c_bits, king_c_width, king_c_height);
}


void
draw_cardback(win, x, y)
	Window win;
	int	x, y;
{
	int	delta = CARD_HEIGHT;

#define	INSET	2
	/* change the origin so cards will have the same back anywhere
	 * on the Window
	 *
	 * there should be a tile centered in the card, with the
	 * surrounding tiles being partial
	 */
	XmuFillRoundedRectangle(dpy, win, blackgc, x, y, CARD_WIDTH, 
		(delta == CARD_HEIGHT) ? delta : delta + ROUND_H * 2,
		ROUND_W, ROUND_H);
	XSetTSOrigin(dpy, backgc, x + back_delta_x, y + back_delta_y);

	XmuFillRoundedRectangle(dpy, win, backgc, x + INSET, y + INSET, 
		CARD_WIDTH - 2*INSET, (CARD_HEIGHT - 2*INSET),
		ROUND_W, ROUND_H);
}

void
draw_card(win, x, y, rank, suit)
	int	x,y;
	Rank	rank;
	Suit	suit;
{
	if (suit == Spade || suit == Club)	{
		cardgc = blackgc;
	} else	{
		cardgc = redgc;
	}

	/* fill the background */
	XmuFillRoundedRectangle(dpy, win, whitegc, x, y, 
		CARD_WIDTH, CARD_HEIGHT,
		ROUND_W, ROUND_H);

	/* draw border on card */
	XmuDrawRoundedRectangle(dpy, win, blackgc, x, y, 
		CARD_WIDTH, CARD_HEIGHT,
		ROUND_W, ROUND_H);
		card_is_clipped = False;

	switch (rank)	{
	case	King:
		draw_king(win, suit, x, y);
		break;
	case	Queen:
		draw_queen(win, suit, x, y);
		break;
	case	Jack:
		draw_jack(win, suit, x, y);
		break;

	case	Ten:
		draw_pip(win, suit, MID_CARD_X + x, CARD_TEN_Y1 + y);
		draw_did(win, suit, MID_CARD_X + x, CARD_TEN_Y2 + y);
		draw_eight_pips(win, suit, x, y);
		break;

	case	Nine:
		draw_pip(win, suit, x + MID_CARD_X, y + MID_CARD_Y);
		draw_eight_pips(win, suit, x, y);
		break;

	case	Eight:
		draw_did(win, suit, x + MID_CARD_X, y + CARD_EIGHT_Y);
		/* fall thru */
	case	Seven:
		draw_pip(win, suit, MID_CARD_X + x, CARD_SEVEN_Y + y);
		/* fall thru */
	case	Six:
		draw_six_pips(win, suit, x, y);
		break;

	case	Five:
		draw_pip(win, suit, x + MID_CARD_X, y + MID_CARD_Y);
		/* fall thru */
	case	Four:
		draw_four_pips(win, suit, x, y);
		break;

	case	Three:
		draw_pip(win, suit, x + MID_CARD_X, y + MID_CARD_Y);
		/* fall thru */
	case	Deuce:
		draw_two_pips(win, suit, x, y);
		break;
	case	Ace:
		draw_center_pip(win, suit, x + MID_CARD_X, y + MID_CARD_Y);
		break;
	default:
		assert(0);
	}

	draw_rank(win, x, y, rank, suit);

	/* clear the clip mask */
	XSetClipMask(dpy, cardgc, None);
	if (round_cards)	{
	    XSetClipMask(dpy, whitegc, None);
	    if (cardgc != blackgc)
		XSetClipMask(dpy, blackgc, None);
	}
}

/*
 * NOTE -- for all the pip drawers except the one that actually plots the
 * bits, the location is the card's location.  the drawer's take the
 * pip's center as location.
 */

/*
 * draws right-side-up pip
 * location is for center of pip
 */
draw_pip(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x, y;
{
	int	w, h;

	switch(suit)	{
	case	Spade:
		w = spade_width;
		h = spade_height;
		break;
	case	Diamond:
		x++;
		w = diamond_width;
		h = diamond_height;
		break;
	case	Heart:
		y++;
		w = heart_width;
		h = heart_height;
		break;
	case	Club:
		y++;
		w = club_width;
		h = club_height;
		break;
	default:
		assert(0);
	}
	XCopyPlane(dpy, suit_map[suit], win, cardgc,
		0, 0, w, h,
		x - w/2, y - h/2, 1);
}

/*
 * draws upside-down pip
 *
 * location is for center of pip
 */
draw_did(win, suit, x, y)
	Suit	suit;
	int	x,y;
{
	int	w, h;

	if (card_is_clipped)	/* a clipped card never shows any did's */
		return;

	switch(suit)	{
	case	Spade:
		w = spade_width;
		h = spade_height;
		break;
	case	Diamond:
		x++;
		w = diamond_width;
		h = diamond_height;
		break;
	case	Heart:
		y++;
		w = heart_width;
		h = heart_height;
		break;
	case	Club:
		y++;
		w = club_width;
		h = club_height;
		break;
	default:
		assert(0);
	}
	XCopyPlane(dpy, suit_r_map[suit], win, cardgc, 
		0, 0, w, h,
		x - w/2, y - h/2, 1);
}

/*
 * draws big center pip
 */
draw_center_pip(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x,y;
{
	int	w, h;

	if (card_is_clipped)
		return;

	switch(suit)	{
	case	Spade:
		w = spade_lg_width;
		h = spade_lg_height;
		break;
	case	Diamond:
		w = diamond_width;
		h = diamond_height;
		break;
	case	Heart:
		w = heart_width;
		h = heart_height;
		break;
	case	Club:
		w = club_width;
		h = club_height;
		break;
	default:
		assert(0);
	}
	XCopyPlane(dpy, suit_lg_map[suit], win, cardgc, 
		0, 0, w, h,
		x - w/2, y - h/2, 1);
}

/* 
 * draw_two_pips
 */
draw_two_pips(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x,y;
{
	draw_pip(win, suit, x + MID_CARD_X, y + CARD_ROW1_Y);
	draw_did(win, suit, x + MID_CARD_X, y + CARD_ROW5_Y);
}

/*
 * draw_four_pips
 */
draw_four_pips(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x,y;
{
	draw_pip(win, suit, x + CARD_COL1_X, y + CARD_ROW1_Y);
	draw_did(win, suit, x + CARD_COL1_X, y + CARD_ROW5_Y);

	draw_pip(win, suit, x + CARD_COL3_X, y + CARD_ROW1_Y);
	draw_did(win, suit, x + CARD_COL3_X, y + CARD_ROW5_Y);
}

draw_six_pips(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x, y;
{
	draw_pip(win, suit, x + CARD_COL1_X, y + CARD_ROW1_Y);

	draw_pip(win, suit, x + CARD_COL3_X, y + CARD_ROW1_Y);

	if (card_is_clipped)
		return;

	/* these are only visible when its not clipped */
	draw_pip(win, suit, x + CARD_COL1_X, y + CARD_ROW3_Y);
	draw_did(win, suit, x + CARD_COL1_X, y + CARD_ROW5_Y);
	draw_pip(win, suit, x + CARD_COL1_X, y + CARD_ROW3_Y);

	draw_pip(win, suit, x + CARD_COL3_X, y + CARD_ROW3_Y);
	draw_did(win, suit, x + CARD_COL3_X, y + CARD_ROW5_Y);
}

draw_eight_pips(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x,y;
{
	draw_pip(win, suit, x + CARD_COL1_X, y + CARD_ROW1_Y);

	draw_pip(win, suit, x + CARD_COL3_X, y + CARD_ROW1_Y);

	if (card_is_clipped)
		return;

	/* these are only visible when its not clipped */
	draw_pip(win, suit, x + CARD_COL1_X, y + CARD_ROW2_Y);
	draw_did(win, suit, x + CARD_COL1_X, y + CARD_ROW4_Y);
        draw_did(win, suit, x + CARD_COL1_X, y + CARD_ROW5_Y);

	draw_pip(win, suit, x + CARD_COL3_X, y + CARD_ROW2_Y);
	draw_did(win, suit, x + CARD_COL3_X, y + CARD_ROW4_Y);
	draw_did(win, suit, x + CARD_COL3_X, y + CARD_ROW5_Y);
}

draw_jack(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x,y;
{
	XCopyPlane(dpy, jack_map[suit], win, cardgc, 
		0, 0, FACECARD_WIDTH, FACECARD_HEIGHT,
		x + (CARD_WIDTH - FACECARD_WIDTH)/2, 
		y + (CARD_HEIGHT - FACECARD_HEIGHT)/2, 1);

	XDrawRectangle(dpy, win, cardgc,
		x + (CARD_WIDTH - FACECARD_WIDTH)/2, 
		y + (CARD_HEIGHT - FACECARD_HEIGHT)/2,
		FACECARD_WIDTH, FACECARD_HEIGHT);
}

draw_queen(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x,y;
{
	XCopyPlane(dpy, queen_map[suit], win, cardgc,
		0, 0, FACECARD_WIDTH, FACECARD_HEIGHT,
		x + (CARD_WIDTH - FACECARD_WIDTH)/2, 
		y + (CARD_HEIGHT - FACECARD_HEIGHT)/2, 1);

	XDrawRectangle(dpy, win, cardgc,
		x + (CARD_WIDTH - FACECARD_WIDTH)/2, 
		y + (CARD_HEIGHT - FACECARD_HEIGHT)/2,
		FACECARD_WIDTH, FACECARD_HEIGHT);
}

draw_king(win, suit, x, y)
	Window	win;
	Suit	suit;
	int	x,y;
{
	XCopyPlane(dpy, king_map[suit], win, cardgc,
		0, 0, FACECARD_WIDTH, FACECARD_HEIGHT,
		x + (CARD_WIDTH - FACECARD_WIDTH)/2, 
		y + (CARD_HEIGHT - FACECARD_HEIGHT)/2, 1);

	XDrawRectangle(dpy, win, cardgc,
		x + (CARD_WIDTH - FACECARD_WIDTH)/2, 
		y + (CARD_HEIGHT - FACECARD_HEIGHT)/2,
		FACECARD_WIDTH, FACECARD_HEIGHT);
}

draw_rank(win, x, y, rank, suit)
	Window	win;
	int	x, y;
	Rank	rank;
	Suit	suit;
{
	int	w, h;

	if (suit == Heart || suit == Diamond)	{
		XCopyPlane(dpy, rank_map_red[rank], win, cardgc,
			0, 0, rank_width, rank_height,
			x + RANK_LOC_X, y + RANK_LOC_Y, 1);

		if (!card_is_clipped)
		    XCopyPlane(dpy, rank_r_map_red[rank], win, cardgc,
			0, 0, rank_width, rank_height,
			x + (CARD_WIDTH - rank_width - RANK_LOC_X), 
			y + (CARD_HEIGHT - rank_height - RANK_LOC_Y), 1);
	} else	{
		XCopyPlane(dpy, rank_map[rank], win, cardgc,
			0, 0, rank_width, rank_height,
			x + RANK_LOC_X, y + RANK_LOC_Y, 1);

		if (!card_is_clipped)
		    XCopyPlane(dpy, rank_r_map[rank], win, cardgc,
			0, 0, rank_width, rank_height,
			x + (CARD_WIDTH - rank_width - RANK_LOC_X), 
			y + (CARD_HEIGHT - rank_height - RANK_LOC_Y), 1);
	}

	switch (suit)	{
		case	Spade:
			w = spade_sm_width;
			h = spade_sm_height;
			break;
		case	Heart:
			w = heart_sm_width;
			h = heart_sm_height;
			break;
		case	Diamond:
			x++;	/* offset the smaller width */
			w = diamond_sm_width;
			h = diamond_sm_height;
			break;
		case	Club:
			w = club_sm_width;
			h = club_sm_height;
			break;
		default:
			assert(0);
	}
	XCopyPlane(dpy, suit_sm_map[suit], win, cardgc,
		0, 0, w, h,
		x + SMALL_LOC_X, y + SMALL_LOC_Y, 1);

	if (!card_is_clipped)
	    XCopyPlane(dpy, suit_sm_r_map[suit], win, cardgc,
		0, 0, w, h,
		x + (CARD_WIDTH - w - SMALL_LOC_X), 
		y + (CARD_HEIGHT - h - SMALL_LOC_Y), 1);
}


static unsigned char _reverse_byte[0x100] = {
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
	0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
	0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
	0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
	0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
	0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
	0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
	0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
	0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
	0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
	0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
	0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
	0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
	0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
	0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
	0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
	0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

#define S(x,y) src[(H-1-(y))*W+(x)]
#define D(x,y) dst[(H-1-(y))*W+(x)]

flip_bits(src, dst, W, H)
	unsigned char	*src, *dst;
	int	W, H;
{
	int	x, y;

	W = (W + 7)/8;
	for (y = 0; y < H; y++)	{
		for (x = 0; x < W; x++)	{
			D (x, y) = S (x, H - 1 - y);
		}
	}
}

rot_180(src, dst, W, H)
BIT_TYPE src;
BIT_TYPE dst;
int	W, H;
{
	int     x, y;
	int	width = W;
	BIT_TYPE new;
	int	bit;

	W = (W + 7)/8;
	for (y = 0; y < H; y++) {
		for (x = 0; x < W; x++) {
			D (x, y) = _reverse_byte[S (W - 1 - x, H - 1 - y)];
		}
	}

	/* shift it over */
	new = (BIT_TYPE)calloc((unsigned)W*H, (unsigned)1);
	for (y = 0; y < H; y++)	{
		for (x = 0; x < W*8; x++)	{
			bit = (*(dst + (x + (W*8 - width))/8 + y * W)
				& (1 << ((x + (W*8 - width)) % 8))) ? 1 : 0;
			*(new + x/8 + y*W) = (bit << (x%8)) | 
				(*(new + x/8 + y*W) & ~(1 << (x%8)));
		}
	}
	bcopy((char *)new, (char *)dst, W*H);
	free((char *)new);
}


int
card_x(n)
	int n;		/* card of hand */
{
	Dimension width;
	int i;
	Arg args[5];

	i = 0;
	XtSetArg(args[i], XtNwidth, &width);	i++;
	XtGetValues(DealerDrawingArea, args, i);
	
	/* spread them out if 7 will fit */
	if (width >= (10 + 7*(10+CARD_WIDTH))) {
		return(10 + (n-1)*(10+CARD_WIDTH));
	}
	else {	/* otherwise overlap, accomodating 7 */
#if 0
fprintf(stderr, "card_x\tReturning %d\n", 10+(n-1)*(width-20-CARD_WIDTH)/6);
#endif
		return(10+(n-1)*(width-20-CARD_WIDTH)/6);
	}
}

int
card_y(hand)
	int hand;
{
	Dimension height;
	int i;
	Arg args[5];

	i = 0;
	XtSetArg(args[i], XmNheight, &height);	i++;
	if (hand == DEALER) {
		XtGetValues(DealerDrawingArea, args, i);
	}
	else {
		XtGetValues(PlayerDrawingArea, args, i);
	}
	return ((height - CARD_HEIGHT)/2);
}

/*
** Draw the 'n'th card of 'hand' face up.
** 'hand' is 0 if DEALERs hand, otherwise 1 (PLAYERs)
*/  
void
display_card(hand, n, suit, name)
	int hand;
	int n;
	int suit;
	int name;
{
	switch(suit) {	/* map to drawing routine enum type */
	case CLUBS:	suit = Club;	break;
	case DIAMONDS:	suit = Diamond;	break;
	case HEARTS:	suit = Heart;	break;
	case SPADES:	suit = Spade;	break;
	}
	if (hand == DEALER) {
		draw_card(DealerWindow, card_x(n), card_y(hand), name, suit);
	}
	else {
		draw_card(PlayerWindow, card_x(n), card_y(hand), name, suit);
	}
}


/*
**  Draw the nth card of hand face down.
*/
void
face_down(hand, n)
	int hand;
	int n;
{	
	if (hand == DEALER) {
		draw_cardback(DealerWindow, card_x(n), card_y(hand));
	}
	else {
		draw_cardback(PlayerWindow, card_x(n), card_y(hand));
	}
	
}
