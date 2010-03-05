//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "ElastFrm.hpp"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
   TMemo *Memo1;
   TMemo *Memo2;
   TButton *Button1;
   TCheckBox *DDCheckBox;
   TButton *Button2;
   TSaveDialog *SaveDialog1;
   TElasticForm *ElasticForm1;
   TMainMenu *MainMenu1;
   TMenuItem *Help1;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TGroupBox *GroupBox1;
   TMemo *Memo3;
   TButton *Button3;
   void __fastcall Button1Click(TObject *Sender);
   void __fastcall Button2Click(TObject *Sender);
   void __fastcall Button3Click(TObject *Sender);
   void __fastcall Help1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
