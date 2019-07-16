#ifndef OCCTWINDOW_H
#define OCCTWINDOW_H

#include <Aspect_Handle.hxx>
#include <Aspect_Window.hxx>

//Dummy window for viewport size functionality
class occtWindow : public Aspect_Window
{
public:
    occtWindow(int theWidth,    int theHeight,
               int theX1 = -1, int theX2 = -1,
               int theY1 = -1, int theY2 = -1);

    //! Set window handle
    void SetHandle(Aspect_Handle hndl) { myHandle = hndl;}

    //! Returns native Window handle
     virtual Aspect_Drawable NativeHandle() const override { return myHandle; }

     //! Returns parent of native Window handle.
     virtual Aspect_Drawable NativeParentHandle() const  override { return myHandle; }

    //! Returns native Window FB config (GLXFBConfig on Xlib)
    Standard_EXPORT virtual Aspect_FBConfig NativeFBConfig() const override { return 0; }

     virtual void Destroy() {}

     //! Opens the window <me>
     virtual void Map() const override {}

     //! Closes the window <me>
     virtual void Unmap() const override {}

     //! Applies the resizing to the window <me>
     virtual Aspect_TypeOfResize DoResize() const override { return Aspect_TOR_UNKNOWN; }

     //! Apply the mapping change to the window <me>
     virtual Standard_Boolean DoMapping() const override { return Standard_True; }

     //! Returns True if the window <me> is opened
     virtual Standard_Boolean IsMapped() const override { return Standard_True; }

     //! Returns The Window RATIO equal to the physical WIDTH/HEIGHT dimensions
     virtual Standard_Real Ratio() const override { return 1.0; }

     //! Returns The Window POSITION in PIXEL
     virtual void Position (Standard_Integer& theX1,
                            Standard_Integer& theY1,
                            Standard_Integer& theX2,
                            Standard_Integer& theY2) const override;

     //! Set The Window POSITION in PIXEL
     virtual void SetPosition (const Standard_Integer theX1,
                               const Standard_Integer theY1,
                               const Standard_Integer theX2,
                               const Standard_Integer theY2);

     //! Returns The Window SIZE in PIXEL
     virtual void Size (Standard_Integer& theWidth,
                        Standard_Integer& theHeight) const override;

     //! Set The Window SIZE in PIXEL
     virtual void SetSize (const Standard_Integer theWidth,
                           const Standard_Integer theHeight);

   private:

     int myWidth;
     int myHeight;

     int myX1;
     int myX2;
     int myY1;
     int myY2;

     Aspect_Handle myHandle;

   public:

DEFINE_STANDARD_RTTIEXT(occtWindow,Aspect_Window)
};

DEFINE_STANDARD_HANDLE(occtWindow, Aspect_Window)

#endif // OCCTWINDOW_H
