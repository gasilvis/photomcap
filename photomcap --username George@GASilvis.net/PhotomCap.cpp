//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("PhotomCap.res");
USEFORM("Unit1.cpp", Form1);
USEFORM("aboutbox.cpp", fAbout);
USEFORM("helpBox.cpp", Form2);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
       Application->Initialize();
       Application->Title = "PhotomCap";
       Application->HelpFile = "PHOTOMCAP.HLP";
       Application->CreateForm(__classid(TForm1), &Form1);
       Application->CreateForm(__classid(TfAbout), &fAbout);
       Application->CreateForm(__classid(TForm2), &Form2);
       Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------
