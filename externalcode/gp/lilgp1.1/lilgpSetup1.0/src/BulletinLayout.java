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
 * Lays out components as though they were pinned to
 * a bulletin board.<p>
 *
 * Components are simply reshaped to their location and their
 * preferred size.  BulletinLayout is preferrable to setting
 * a container's layout manager to null and explicitly positioning
 * and sizing components.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 */
public class BulletinLayout implements LayoutManager {
    public BulletinLayout() {
    }
    public void addLayoutComponent(String name, Component comp) {
    }
    public void removeLayoutComponent(Component comp) {
    }
    public Dimension preferredLayoutSize(Container target) {
        Insets    insets      = target.insets();
        Dimension dim         = new Dimension(0,0);
        int       ncomponents = target.countComponents();
        Component comp;
        Dimension d;
        Rectangle preferredBounds = new Rectangle(0,0);
        Rectangle compPreferredBounds;

        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                d = comp.preferredSize();
                compPreferredBounds =
                    new Rectangle(comp.location());
                compPreferredBounds.width  = d.width;
                compPreferredBounds.height = d.height;

                preferredBounds =
                    preferredBounds.union(compPreferredBounds);
            }
        }
        dim.width  += insets.left + insets.right;
        dim.height += insets.top + insets.bottom;

        return dim;
    }
    public Dimension minimumLayoutSize(Container target) {
        Insets    insets      = target.insets();
        Dimension dim         = new Dimension(0,0);
        int       ncomponents = target.countComponents();
        Component comp;
        Dimension d;
        Rectangle minimumBounds = new Rectangle(0,0);
        Rectangle compMinimumBounds;

        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                d = comp.minimumSize();
                compMinimumBounds =
                    new Rectangle(comp.location());
                compMinimumBounds.width  = d.width;
                compMinimumBounds.height = d.height;

                minimumBounds =
                    minimumBounds.union(compMinimumBounds);
            }
        }
        dim.width  += insets.left + insets.right;
        dim.height += insets.top + insets.bottom;

        return dim;
    }
    public void layoutContainer(Container target) {
        Insets    insets      = target.insets();
        int       ncomponents = target.countComponents();
        Component comp;
        Dimension ps;
        Point loc;

        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                ps  = comp.preferredSize();
                loc = comp.location();

                comp.reshape(insets.left + loc.x,
                             insets.top + loc.y,
                             ps.width, ps.height);
            }
        }
    }
}
