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
#include "HttpProt.hpp"
#include "LibXmlComps.hpp"
#include "LibXmlParser.hpp"
//#include <HttpProt.h>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
   TMemo *Memo1;
   TMemo *Memo2;
   TButton *DoitButton;
   TCheckBox *DDCheckBox;
   TButton *Button2;
   TSaveDialog *SaveDialog1;
   TElasticForm *ElasticForm1;
   TMainMenu *MainMenu1;
   TMenuItem *Help1;
   TLabel *Label2;
   TLabel *Label3;
   TMenuItem *Options1;
   TMenuItem *Labels1;
   TMenuItem *CSV1;
   TMemo *Memo4;
   TButton *Button4;
   TMenuItem *SeqPlot1;
   TMenuItem *File1;
   TMenuItem *LoadSourceFile1;
   TMenuItem *SaveSourceFile1;
   TMenuItem *SaveResultFile1;
   TMenuItem *Exit1;
   TOpenDialog *OpenDialog1;
   TSaveDialog *SaveDialog2;
   TMenuItem *Help2;
   TMenuItem *About1;
   TCheckBox *SPaltError;
   TGroupBox *GroupBox1;
   TEdit *CLabelEdit;
   TEdit *KLabelEdit;
   TButton *ReDoButton;
   TLabel *versionLabel;
   THttpCli *HttpCli1;
   TEdit *chartEdit;
   TEasyXmlScanner *EasyXmlScanner1;
   TLabel *chartMsg;
   void __fastcall DoitButtonClick(TObject *Sender);
   void __fastcall Button2Click(TObject *Sender);
//   void __fastcall Button3Click(TObject *Sender);
   void __fastcall Help1Click(TObject *Sender);
   void __fastcall Labels1Click(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall CSV1Click(TObject *Sender);
   void __fastcall Button4Click(TObject *Sender);
   void __fastcall SeqPlot1Click(TObject *Sender);
   void __fastcall LoadSourceFile1Click(TObject *Sender);
   void __fastcall SaveSourceFile1Click(TObject *Sender);
   void __fastcall SaveResultFile1Click(TObject *Sender);
   void __fastcall Exit1Click(TObject *Sender);
   void __fastcall About1Click(TObject *Sender);
   void __fastcall SPaltErrorClick(TObject *Sender);
   void __fastcall ReDoButtonClick(TObject *Sender);
   void __fastcall HttpCli1DocBegin(TObject *Sender);
   void __fastcall HttpCli1DocEnd(TObject *Sender);
   void __fastcall versionLabelClick(TObject *Sender);
private:	// User declarations
   void __fastcall TForm1::PutIniData(TObject *Sender);
   void __fastcall TForm1::GetIniData(TObject *Sender);
   void __fastcall TForm1::DoAAVSO(TObject *Sender);
   void __fastcall TForm1::DoAAVSO2(TObject *Sender);
   void __fastcall TForm1::DoSeqPlot(TObject *Sender);
   float __fastcall TForm1::ErrorComp(float err1, float err2);
   void __fastcall TForm1::ReDo(TObject *Sender, unsigned int num, int* comps);
   AnsiString __fastcall TForm1::EncodeURIComponent(AnsiString ASrc);
   bool __fastcall TForm1::IsSafeChar(int ch);

public:		// User declarations
   __fastcall TForm1(TComponent* Owner);
   bool __fastcall TForm1::httpGet(AnsiString URL, char* buffer, int bufsize);
   void __fastcall TForm1::outputReport(TObject *Sender);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
