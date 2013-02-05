/* SUN MICROSYSTEMS LICENSE AGREEMENT
 *
 * This is a legal agreement between the purchaser of this book/CD-ROM package
 * ("You") and Sun Microsystems, Inc.  By opening the sealed CD-ROM you are
 * agreeing to be bound by the terms of this agreement.  If you do not agree to the
 * terms of this agreement, promptly return the unopened book/CD-ROM package to the
 * place you obtained it for a full refund.
 *
 * SOFTWARE LICENSE
 *
 * 1.      Grant of License:       Sun Microsystems grants to you ("Licensee") a
 * non-exclusive, non-transferable license to use the Sun Microsystems software
 * programs included on the CD-ROM without fee. The software is in "use" on a
 * computer when it is loaded into the temporary memory (i.e. RAM) or installed
 * into the permanent memory (e.g. hard disk, CD-ROM, or other storage device).
 * You may network the software or otherwise use it on more than one computer or
 * computer terminal at the same time.
 *
 * 2.      Copyright:  The CD-ROM is copyrighted by Sun Microsystems, Inc. and is
 * protected by United States copyright laws and international treaty provisions.
 * Therefore, you must treat the CD-ROM like any other copyrighted material.
 * Individual software programs on the CD-ROM are copyrighted by their respective
 * owners and may require separate licensing. The Java Development Kit is
 * copyrighted by Sun Microsystems, Inc. and is covered by a separate license
 * agreement provided on the CD-ROM.
 *
 * 3.      Graphic Java Toolkit (GJT) and Sample Code:  Sun Microsystems, Inc.
 * grants you a royalty-free right to reproduce and distribute the GJT and sample
 * code provided that you: (a) distribute the GJT and sample code only in
 * conjunction with and as a part of your software application; (b) do not use Sun
 * Microsystems, Inc. or its authors' names, logos, or trademarks to market your
 * software product; and (c) agree to indemnify, hold harmless and defend Sun
 * Microsystems, Inc. and its authors and suppliers from and against any claims or
 * lawsuits, including attorneys fees, that arise or result from the use or
 * distribution of your software product.
 *
 * DISCLAIMER OF WARRANTY
 *
 * The SOFTWARE (including instructions for its use) is provided "AS IS" WITHOUT
 * WARRANTY OF ANY KIND.  SUN MICROSYSTEMS and any distributor of the SOFTWARE
 * FURTHER DISCLAIM ALL IMPLIED WARRANTIES INCLUDING WITHOUT LIMITATION ANY IMPLIED
 * WARRANTIES OF MERCHANTABILITY OR OF FITNESS FOR A PARTICULAR PURPOSE.  THE
 * ENTIRE RISK ARISING OUT OF THE USE OR PERFORMANCE OF THE SOFTWARE OR
 * DOCUMENTATION REMAINS WITH YOU.
 *
 * IN NO EVENT SHALL SUN MICROSYSTEMS, ITS AUTHORS, OR ANY ONE ELSE INVOLVED IN THE
 * CREATION, PRODUCTION, OR DELIVERY OF THE SOFTWARE BE LIABLE FOR ANY DAMAGES
 * WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 * BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS)
 * ARISING OUT OF THE USE OF OR INABILITY TO USE THE SOFTWARE OR DOCUMENTATION,
 * EVEN IF SUN MICROSYSTEMS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES,
 * BECAUSE SOME STATES/COUNTRIES DO NOT ALLOW THE EXCLUSION OF LIMITATION OF
 * LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES, THE ABOVE LIMITATION MAY NOT
 * APPLY TO YOU.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS
 *
 * The SOFTWARE and documentation are provided with RESTRICTED RIGHTS.  Use,
 * duplication, or disclosure is subject to restrictions as set forth in
 * subparagraph (c)(1)(ii) of The Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial
 * Computer Software -- Restricted Rights 48 CFR 52.227-19.
 *
 * Should you have any questions concerning this Agreement, or if you desire to
 * contact Sun Microsystems for any reason, please write:  Sun Microsystems, Inc.,
 * 2550 Garcia Avenue, Mountain View, CA  94043.
 *
 * Copyright: (c) 1997,  Sun Microsystems, Inc.
 */


import java.awt.*;

/**
 * Each Scroller contains a Panel (viewport) and two Scrollbars
 * (horizontal and vertical).  Works in conjunction with a
 * ScrollerLayout, that lays out the viewport and two
 * scrollbars.<p>
 *
 * Subclasses must override:<p>
 * <dl>
 * <dd> abstract public void      scrollTo(int x, int y)
 * <dd> abstract public Dimension getScrollAreaSize()
 * </dl>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ComponentScroller
 * @see     ImageScroller
 * @see     ScrollerLayout
 * @see     gjt.test.ComponentScrollerTest
 * @see     gjt.test.ImageScrollerTest
 */
public abstract class Scroller extends Panel {
    protected Panel     viewport;
    protected Scrollbar hbar, vbar;

    abstract public void      scrollTo(int x, int y);
    abstract public Dimension getScrollAreaSize();

    public Scroller() {
        setLayout(new ScrollerLayout(this));
        add("Scroll", viewport = new Panel());
        add("East", vbar = new Scrollbar(Scrollbar.VERTICAL));
        add("South",hbar = new Scrollbar(Scrollbar.HORIZONTAL));
    }
    public Scrollbar getHorizontalScrollbar() {return hbar;    }
    public Scrollbar getVerticalScrollbar  () {return vbar;    }
    public Panel     getViewport           () {return viewport;}

    public boolean handleEvent(Event event) {
        boolean handledEvent;

        switch(event.id) {
            case Event.SCROLL_LINE_UP:   scrollLineUp(event);
            break;
            case Event.SCROLL_LINE_DOWN: scrollLineDown(event);
            break;
            case Event.SCROLL_PAGE_UP:   scrollPageUp  (event);
            break;
            case Event.SCROLL_PAGE_DOWN: scrollPageDown(event);
            break;
            case Event.SCROLL_ABSOLUTE:  scrollAbsolute(event);
            break;
        }
        handledEvent = event.id == Event.SCROLL_LINE_UP   ||
                       event.id == Event.SCROLL_LINE_DOWN ||
                       event.id == Event.SCROLL_PAGE_UP   ||
                       event.id == Event.SCROLL_PAGE_DOWN ||
                       event.id == Event.SCROLL_ABSOLUTE;

        if(handledEvent) return true;
        else             return super.handleEvent(event);
    }
    public void paint (Graphics g) { scroll(); }
    public void update(Graphics g) { paint(g); }

    public void manageScrollbars() {
        manageHorizontalScrollbar();
        manageVerticalScrollbar  ();
    }
    protected void manageHorizontalScrollbar() {
        Dimension size           = size();
        Dimension scrollAreaSize = getScrollAreaSize();

        if(vbar.isVisible())
            size.width -= vbar.size().width;

        if(scrollAreaSize.width > size.width) {
            if( ! hbar.isVisible())
                hbar.show();
        }
        else if(hbar.isVisible()) {
            hbar.hide();
            hbar.setValue(0);
            repaint();
        }
    }
    protected void manageVerticalScrollbar() {
        Dimension size           = size();
        Dimension scrollAreaSize = getScrollAreaSize();

        if(hbar.isVisible())
            size.height -= hbar.size().height;

        if(scrollAreaSize.height > size.height) {
            if( ! vbar.isVisible())
                vbar.show();
        }
        else if(vbar.isVisible()) {
            vbar.hide();
            vbar.setValue(0);
            repaint();
        }
    }
    public void setScrollbarValues() {
        if(hbar.isVisible()) setHorizontalScrollbarValues();
        if(vbar.isVisible()) setVerticalScrollbarValues();
    }
    protected void setHorizontalScrollbarValues() {
        Dimension vsize          = viewport.size();
        Dimension scrollAreaSize = getScrollAreaSize();
        int max = scrollAreaSize.width - vsize.width;

        hbar.setValues(hbar.getValue(), // value
                       vsize.width,     // amt visible/page
                       0,               // minimum
                       max);            // maximum

        setHorizontalLineAndPageIncrements();
    }
    protected void setVerticalScrollbarValues() {
        Dimension vsize          = viewport.size();
        Dimension scrollAreaSize = getScrollAreaSize();
        int max = scrollAreaSize.height - vsize.height;

        vbar.setValues(vbar.getValue(), // value
                       vsize.height,    // amt visible/page
                       0,               // minimum
                       max);            // maximum

        setVerticalLineAndPageIncrements();
    }
    protected void scrollLineUp  (Event event) { scroll(); }
    protected void scrollLineDown(Event event) { scroll(); }
    protected void scrollPageUp  (Event event) { scroll(); }
    protected void scrollPageDown(Event event) { scroll(); }
    protected void scrollAbsolute(Event event) { scroll(); }

    protected void setHorizontalLineAndPageIncrements() {
        Dimension size = getScrollAreaSize();
        hbar.setLineIncrement(size.width/10);
        hbar.setPageIncrement(size.width/5);
    }
    protected void setVerticalLineAndPageIncrements() {
        Dimension size = getScrollAreaSize();
        vbar.setLineIncrement(size.height/10);
        vbar.setPageIncrement(size.height/5);
    }
    protected void scroll() {
        scrollTo(hbar.getValue(), vbar.getValue());
    }
}
