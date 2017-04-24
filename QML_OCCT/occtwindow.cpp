#include "occtwindow.h"


IMPLEMENT_STANDARD_RTTIEXT(occtWindow, Aspect_Window)

occtWindow::occtWindow(int theWidth, int theHeight, int theX1, int theX2, int theY1, int theY2)
    : myWidth (theWidth), myHeight(theHeight),
      myX1 (theX1), myX2 (theX2),
      myY1 (theY1), myY2 (theY2)
{
    if (myX1 == -1) myX1 = 0;
    if (myX2 == -1) myX2 = myWidth;

    if (myY1 == -1) myY1 = 0;
    if (myY2 == -1) myY2 = myHeight;
}

void occtWindow::Position (Standard_Integer& theX1,
                           Standard_Integer& theY1,
                           Standard_Integer& theX2,
                           Standard_Integer& theY2) const
{
  theX1 = myX1;
  theX2 = myX2;
  theY1 = myY1;
  theY2 = myY2;
}

void occtWindow::SetPosition (const Standard_Integer theX1,
                              const Standard_Integer theY1,
                              const Standard_Integer theX2,
                              const Standard_Integer theY2)
{
  myX1 = theX1;
  myX2 = theX2;
  myY1 = theY1;
  myY2 = theY2;
}

void occtWindow::Size (Standard_Integer& theWidth,
                       Standard_Integer& theHeight) const
{
  theWidth  = myWidth;
  theHeight = myHeight;
}

void occtWindow::SetSize (const Standard_Integer theWidth,
                          const Standard_Integer theHeight)
{
  myWidth  = theWidth;
  myHeight = theHeight;
}
