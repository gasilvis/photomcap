/*
    Copyright 2010  George A Silvis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "aboutbox.h"
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ElastFrm"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

AnsiString SaveFile= "temp";
AnsiString SourceFile= "";

#define Maxlines 128

void __fastcall TForm1::DoAAVSO(TObject *Sender)
{
    AnsiString as;
    char  SName[64], SRA[32], SDec[32], ss[128], ss2[32], Filters[20], suffix[3], cs[256];
    int x, y, nextstate, state= 0, CDL;
    unsigned int BmVcol= 999, BmVcolTmp; // filter column labeled B-V; This needs to be skipped
    unsigned int i, j, Fs, Stars;

    // Clear output
    Memo2->Clear(); // AIP STAR fmt
    Memo4->Clear(); // CSV generic

    Memo1->SelectAll();
    char *M0= Memo1->Lines->GetText();
    if(0==strlen(M0))
       return;
    for(i= 0; i< strlen(M0); i++) if('\t'== M0[i]) M0[i]= ' '; // kill tabs

    char *T, *M1= M0;
    char* delim= " \r\n";
    char  Labels[Maxlines][8]; int LabelLine[Maxlines]; // to hold labels incase they need to
    char  AUIDs[Maxlines][24]; // capture the AUIDs
    // be modified because they are dups.   12 12 will be changed to 12a 12b

    Memo2->Lines->Append("FILETYPE=             STARDATA /Star Data file                                ");
    Memo2->Lines->Append("NUMTARGS=                    1 /Number of targets                             ");
    T= strtok(M1, delim);
    do {
       switch(state) {
          case 0: // find star name
             if(0==strcmp(T, "Photometry")) {
                T= strtok(NULL, delim);
                if(0==strcmp(T, "for")) {
                   SName[0]= 0; // init
                   state= 1;
                }
             }
             break;
          case 1: // get end of name (it may have embedded blanks
             if(0==strcmp(T, "From")) {
                strcpy(SName, &SName[1]); // trim leading blank
                for(i= 0; i<strlen(SName); i++) SName[i]= toupper(SName[i]);
                state= 2;
             } else {
                strcat(SName, " "); strcat(SName, T); // build name
             }
             break;
          case 2: // find RA of star
             if(0==strcmp(T, "RA:")) {
                state= 3;
             }
             break;
          case 3: // Get angle
             strcpy(SRA, T);
             state= 4;
             break;
          case 4: //
             if(DDCheckBox->Checked) {
                strcpy(SRA, T);
                strcpy(SRA, &SRA[1]); // kill lead (
                while(!isdigit(SRA[strlen(SRA)-1]))
                   SRA[strlen(SRA)-1]= 0;
             }
             state= 5;
             break;
          case 5: // find dec
             if(0==strcmp(T, "Decl.:")) {
                state= 6;
             }
             break;
          case 6: // Get DEC
             strcpy(SDec, T);
             state= 7;
             break;
          case 7: //
             if(DDCheckBox->Checked) {
                strcpy(SDec, T);
                strcpy(SDec, &SDec[1]); // kill lead (
                while(!isdigit(SDec[strlen(SDec)-1]))
                   SDec[strlen(SDec)-1]= 0;
             }
             state= 8;
             break;
          case 8: // get filters
             if(0==strcmp(T, "Dec.")) {  // look for "Dec.   Label"
                T= strtok(NULL, delim);
                if(0==strcmp(T, "Label")) {
                   state= 9;
                   Filters[0]= 0;
                }
             }
             break;
          case 9:
             if(strcmp(T, "Comments")) {
                if(strcmp(T, "B-V")) {
                   strncat(Filters, T, 1);  // grab first letter of filter name
                } else {
                   BmVcol= strlen(Filters);
                }
             } else {
//                strcpy(delim, " \r");
                Memo2->Lines->Append("NUMSTARS=                    x /Number of comp or field stars                 "); // plac  eholder
                sprintf(ss, "NUMFILTE=                   %2i /Number of filter bands                        ", strlen(Filters));
                Memo2->Lines->Append(ss);
                for(i= 1; i<= strlen(Filters); i++) {
                   sprintf(ss, "FILTER%02i=                    %c /Designation of filter band                    ", i, Filters[i-1]);
                   Memo2->Lines->Append(ss);
                }
                Memo2->Lines->Append("CHARTDES=              xxxxxxx /Chart designation                             "); // placeholder
                CDL= Memo2->Lines->Count- 1;
                Memo2->Lines->Append("--------=                      /                                              ");
                Memo2->Lines->Append("T001FN  =                    V /Target function                               ");

                sprintf(ss, "T001ID  =%21s /Target identifier                             ", SName);
                ss[78]= 0; // make sure its not too long
                Memo2->Lines->Append(ss);
                sprintf(ss, "T001RA  =%21s /Target RA                                     ", SRA);
                Memo2->Lines->Append(ss);
                sprintf(ss, "T001DC  =%21s /Target DEC                                    ", SDec);
                Memo2->Lines->Append(ss);

                sprintf(cs, "\"Starname\",\"%s\"", SName);
                Memo4->Lines->Append(cs);
                sprintf(cs, "\"Star RA\",\"%s\"", SRA);
                Memo4->Lines->Append(cs);
                sprintf(cs, "\"Star DEC\",\"%s\"", SDec);
                Memo4->Lines->Append(cs);
                Memo4->Lines->Append(cs); // placeholder for chartid
                sprintf(cs, "\"AUID\",\"RA\",\"Dec\",\"Label\"");
                for(i= 1; i<=strlen(Filters); i++) {
                   sprintf(ss2, ",\"%c\",", Filters[i-1]); // 2 cols: mag and err
                   strcat(cs, ss2);
                }
                Memo4->Lines->Append(cs);

                Stars= 0;
                state= 11; // next token is the start of the first line
             }
             break;
          case 10: // eat the comments
             if(0==strcmp(T, "Report")) {
                state= 19;
                break;
             }
             if(11!=strlen(T)) // AUID's are always 11 long....  nope
                break;
             else if(!('-'==T[3] && '-'==T[7])) // They have two dashes; still a hope and prayer
                break;
          case 11: // beginning of the data line
             Memo2->Lines->Append("--------=                      /                                              ");
             sprintf(ss, "S%03iFN  =                    C /Star function                                 ", ++Stars);
             Memo2->Lines->Append(ss);
             sprintf(ss, "S%03iID  =%21s /Star identifier                               ", Stars, T);
             Memo2->Lines->Append(ss);
             strcpy(AUIDs[Stars], T);
             sprintf(cs, "\"%s\"", T);
             state= 12;
             break;
          case 12: // get star RA
             strcpy(SRA, T);
             state= 13;
             break;
          case 13: //
             if(DDCheckBox->Checked) {
                strcpy(SRA, T);
                strcpy(SRA, &SRA[1]); // kill lead (
                while(!isdigit(SRA[strlen(SRA)-1]))
                   SRA[strlen(SRA)-1]= 0;
             }
             sprintf(ss, "S%03iRA  =%21s /RA                                            ", Stars, SRA);
             Memo2->Lines->Append(ss);
             sprintf(ss2, ",%s", SRA); strcat(cs, ss2);
             state= 14;
             break;
          case 14: // Get DEC
             strcpy(SDec, T);
             state= 15;
             break;
          case 15: //
             if(DDCheckBox->Checked) {
                strcpy(SDec, T);
                strcpy(SDec, &SDec[1]); // kill lead (
                while(!isdigit(SDec[strlen(SDec)-1]))
                   SDec[strlen(SDec)-1]= 0;
             }
             sprintf(ss, "S%03iDC  =%21s /DC                                            ", Stars, SDec);
             Memo2->Lines->Append(ss);
             sprintf(ss2, ",%s", SDec); strcat(cs, ss2);
             state= 16;
             break;
          case 16: // label
             sprintf(ss, "S%03iLab =%21s /Label                                         ", Stars, T);
             Memo2->Lines->Append(ss);
             strcpy(Labels[Stars], T);
             LabelLine[Stars]= Memo2->Lines->Count- 1;
             sprintf(ss2, ",%s", T); strcat(cs, ss2);
             state= 17;
             Fs= 0;
             BmVcolTmp= BmVcol;
             break;
          case 17:
             if(Fs==BmVcolTmp) { // skip the B-V column
                if(strcmp(T, "-")) {
                   T= strtok(NULL, delim); // kill the std dev token too
                }
                BmVcolTmp= 999;
                if(Fs== strlen(Filters)) { // we've finished the line
                   state= 10;
                }
                break;
             }
             Fs++;
             if(0==strcmp(T, "-")) { // filter with no data?
                sprintf(ss, "S%03iF%02iM=%21s /%c standard magnitude                          ", Stars, Fs, "0.000", Filters[Fs-1]);
                Memo2->Lines->Append(ss);
                sprintf(ss, "S%03iF%02iS=%21s /%c std dev                                     ", Stars, Fs, "0.000", Filters[Fs-1]);
                Memo2->Lines->Append(ss);
                strcat(cs, ",,");
                if(Fs== strlen(Filters)) { // we've finished the line
                   Memo4->Lines->Append(cs);
                   state= 10;
                }
             } else {
                sprintf(ss, "S%03iF%02iM=%21s /%c standard magnitude                            ", Stars, Fs, T, Filters[Fs-1]);
                Memo2->Lines->Append(ss);
                sprintf(ss2, ",%s", T); strcat(cs, ss2);
                state= 18;
             }
             break;
          case 18: // get std dev
             strcpy(T, &T[1]); // kill lead (
             x= strcspn(T, ")"); T[x]= 0; // kill trailing }
             if(0==strcmp("N/A", T)) strcpy(T, "0.000");
             sprintf(ss, "S%03iF%02iS=%21s /%c std dev                                       ", Stars, Fs, T, Filters[Fs-1]);
             Memo2->Lines->Append(ss);
             sprintf(ss2, ",%s", T); strcat(cs, ss2);
             if(Fs== strlen(Filters)) { // we've finished the line
                Memo4->Lines->Append(cs);
                state= 10;
             } else {
                state= 17;
             }
             break;
          case 19: // find chart
             if(0==strcmp(T, "as:")) {
                state= 20;
             }
             break;
          case 20:
             // look for duplicate Labels and modify with letter suffix
             for(i= 1; i<Stars; i++) { // don't need to do the last one
                strcpy(suffix, "a");
                for(j= i+1; j<=Stars; j++) {
                   if(!strcmp(Labels[i], Labels[j])) {
                      suffix[0]++;
                      strcat(Labels[j], suffix);
                      sprintf(ss, "S%03iLab =%21s /Label                                         ", j, Labels[j]);
                      Memo2->Lines->Strings[LabelLine[j]]= ss;
                   }
                }
                if(strcmp(suffix, "a")) { // go fix the first
                   strcat(Labels[i], "a");
                   sprintf(ss, "S%03iLab =%21s /Label                                         ", i, Labels[i]);
                   Memo2->Lines->Strings[LabelLine[i]]= ss;
                }
             }
             // Label option
             if(Labels1->Checked) {
                for(i= 1; i<=Stars; i++) {
                   sprintf(ss2, "%s %s", Labels[i], AUIDs[i]);
                   sprintf(ss, "S%03iID  =%21s /Star identifier                               ", i, ss2);
                   Memo2->Lines->Strings[LabelLine[i]- 3]= ss;
                }
             }

             sprintf(ss, "NUMSTARS=                  %3i /Number of comp or field stars                 ", Stars);
             Memo2->Lines->Strings[2]= ss;
             sprintf(ss, "CHARTDES=%21s /Chart designation                             ", T);
             Memo2->Lines->Strings[CDL]= ss;
             sprintf(cs, "\"ChartID\",\"%s\"", T);
             Memo4->Lines->Strings[3]= cs;
             as= SName;
             SaveFile= as + "+" + T;// + ".STAR";
             state= 21;
             break;
       }
       T= strtok(NULL, delim);
//       Memo2->Lines->Append(T);  // debug
    } while(T);
    StrDispose(M0);
    Memo2->Lines->Append("END                                                                           ");
    if(state!=21)
       ShowMessage("This did not finish properly; do not use this STAR file. If your input data is good and the problem repeats please send the chartid and the PhotomCap version to me: George@GASilvis.net");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::DoitButtonClick(TObject *Sender)
{
    if(SeqPlot1->Checked) {
       DoSeqPlot(Sender);
    } else {
       DoAAVSO(Sender);
    }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Button2Click(TObject *Sender)
{
   AnsiString s, as; int pos;
   if(CSV1->Checked) s= SaveFile+ ".CSV";
   else              s= SaveFile+ ".STAR";
   SaveDialog1->FileName= s;
   if(SaveDialog1->Execute()) {
      as= SaveDialog1->FileName;
      pos = as.LastDelimiter("\\");
      if(CSV1->Checked)
         Memo4->Lines->SaveToFile(SaveDialog1->FileName);
      else
         Memo2->Lines->SaveToFile(SaveDialog1->FileName);
      as.Delete(pos, 1000);
      SaveDialog1->InitialDir= as;
   }
   PutIniData(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Help1Click(TObject *Sender)
{
      //GroupBox1->BringToFront();
      //GroupBox1->Visible= true;
      Application->HelpJump("Introduction");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Labels1Click(TObject *Sender)
{
    Labels1->Checked= Labels1->Checked? false: true;
    PutIniData(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
   GetIniData(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PutIniData(TObject *Sender)
{
   TIniFile *ini;
   ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ) );

   ini->WriteString("Setup", "Dir", SaveDialog1->InitialDir);
   ini->WriteBool("Options", "Labels", Labels1->Checked);
   ini->WriteBool("Options", "CSV", CSV1->Checked);
   ini->WriteBool("Options", "SeqPlot", SeqPlot1->Checked);
   ini->WriteString("Setup", "SourceSaveDir", SaveDialog2->InitialDir);
   ini->WriteString("Setup", "SourceOpenDir", OpenDialog1->InitialDir);
   ini->WriteBool("Options", "SeqPlotAltError", SPaltError->Checked);
   delete ini;
   GetIniData(Sender); // to handle any setup related to the ini values
}


void __fastcall TForm1::GetIniData(TObject *Sender)
{
   TIniFile *ini;
   ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ) );

   SaveDialog1->InitialDir= ini->ReadString("Setup", "Dir", "");
   Labels1->Checked= ini->ReadBool("Options", "Labels", false);
   CSV1->Checked= ini->ReadBool("Options", "CSV", false);
   SeqPlot1->Checked= ini->ReadBool("Options", "SeqPlot", false);
   SaveDialog2->InitialDir= ini->ReadString("Setup", "SourceSaveDir", "");
   OpenDialog1->InitialDir= ini->ReadString("Setup", "SourceOpenDir", "");
   SPaltError->Checked= ini->ReadBool("Options", "SeqPlotAltError", false);
   delete ini;

   // setup related to the ini's
   if(CSV1->Checked) {
      Memo4->BringToFront();
      Label3->Caption= "Photometry data in csv format appears here ";
   } else {
      Memo4->SendToBack();
      Label3->Caption= "AIPWin MMT STAR file appears here ";
   }

   if(SeqPlot1->Checked) {
      Label2->Caption= "Paste SeqPlot data here";
      SPaltError->Visible= true;
   } else {
      Label2->Caption= "Paste AAVSO photometry data here";
      SPaltError->Visible= false;
   }

}


void __fastcall TForm1::CSV1Click(TObject *Sender)
{
    CSV1->Checked= CSV1->Checked? false: true;
    if(SeqPlot1->Checked)
        CSV1->Checked= false; // don't do csv output
    PutIniData(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SeqPlot1Click(TObject *Sender)
{
   SeqPlot1->Checked= SeqPlot1->Checked? false: true;
   if(SeqPlot1->Checked) {
       CSV1->Checked= false; // don't do csv output
       SPaltError->Visible= true;
   } else {
       SPaltError->Visible= false;
   }
   PutIniData(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
    Memo1->Clear(); // input window
}
//---------------------------------------------------------------------------
// #Label, h, m, s, d, m, s, V, Verr, B-V, B-Verr, U-B, U-Berr, V-R,VRerr, R-I, R-Ierr, V-I, V-Ierr, Source, # Comments
typedef struct sSigPlotData {
   char       *Label;
   int        rah;
   int        ram;
   float      ras;
   int        decd;
   int        decm;
   float      decs;
   float      V;
   float      Verr;
   float      B_V;
   float      B_Verr;
   float      U_B;
   float      U_Berr;
   float      V_R;
   float      V_Rerr;
   float      R_I;
   float      R_Ierr;
   float      V_I;
   float      V_Ierr;
   int        source;
   char       *comments;

   float      SRA;
   float      SDec;
   char       SName[128];
   float      B;
   float      Berr;
   float      U;
   float      Uerr;
   float      R;
   float      Rerr;
   float      I;
   float      Ierr;
} sigPlotData;

void __fastcall TForm1::DoSeqPlot(TObject *Sender)
{
    unsigned int i;
    char ss[128], sc[32];
    AnsiString as;
    char *T, *delim= ",";

    int lineCnt= Memo1->Lines->Count;
    if(lineCnt==0) {
       ShowMessage("No data in the first memo field.");
       return;
    }
    // Clear output area
    Memo2->Clear(); // AIP STAR fmt

    Memo2->Lines->Append("FILETYPE=             STARDATA /Star Data file                                ");
    Memo2->Lines->Append("NUMTARGS=                    1 /Number of targets                             ");
    Memo2->Lines->Append("NUMSTARS=                    x /Number of comp or field stars                 "); // place holder: [2]

    char Filters[]= "VBURI"; // this is fixed by code; don't change
    sprintf(ss, "NUMFILTE=                   %2i /Number of filter bands                        ", strlen(Filters));
    Memo2->Lines->Append(ss);
    for(i= 1; i<= strlen(Filters); i++) {
       sprintf(ss, "FILTER%02i=                    %c /Designation of filter band                    ", i, Filters[i-1]);
       Memo2->Lines->Append(ss);
    }
    Memo2->Lines->Append("CHARTDES=              SeqPlot /Chart designation                             ");

    int Stars= 0, Lines= 1, Fs;
    sigPlotData spd;
    do { // process the csv data
       as= Memo1->Lines->Strings[Lines-1];
       try {
          T= strtok(as.c_str(), delim);
          spd.Label= T;
          T= strtok(NULL, delim);
          spd.rah= atoi(T);
          T= strtok(NULL, delim);
          spd.ram= atoi(T);
          T= strtok(NULL, delim);
          spd.ras= atof(T);
          T= strtok(NULL, delim);
          spd.decd= atoi(T);
          T= strtok(NULL, delim);
          spd.decm= atoi(T);
          T= strtok(NULL, delim);
          spd.decs= atof(T);

          T= strtok(NULL, delim);
          spd.V= atof(T);
          T= strtok(NULL, delim);
          spd.Verr= atof(T);
          T= strtok(NULL, delim);
          spd.B_V= atof(T);
          T= strtok(NULL, delim);
          spd.B_Verr= atof(T);
          T= strtok(NULL, delim);
          spd.U_B= atof(T);
          T= strtok(NULL, delim);
          spd.U_Berr= atof(T);
          T= strtok(NULL, delim);
          spd.V_R= atof(T);
          T= strtok(NULL, delim);
          spd.V_Rerr= atof(T);
          T= strtok(NULL, delim);
          spd.R_I= atof(T);
          T= strtok(NULL, delim);
          spd.R_Ierr= atof(T);
          T= strtok(NULL, delim);
          spd.V_I= atof(T);
          T= strtok(NULL, delim);
          spd.V_Ierr= atof(T);

          T= strtok(NULL, delim);
          spd.source= atoi(T);
          T= strtok(NULL, delim);
          spd.comments= T;

          // derived data
          spd.SRA  = (spd.rah*15.0)+(spd.ram*15.0/60.0)+(spd.ras*15.0/3600.0);
          spd.SDec = (spd.decd< 0? -1: 1)*((abs(spd.decd))+(spd.decm/60.0)+(spd.decs/3600.0));
          //sprintf(ss, "%f, %f", spd.SRA, spd.SDec);      Memo2->Lines->Append(ss);
          strcpy(spd.SName, spd.comments);
          if('#'==spd.SName[0]) strcpy(spd.SName, &spd.SName[1]);
          //B=  V + B-V       Berr= sqrt( B-Verr^2 - Verr^2 )
          spd.B= (spd.V && spd.B_V) ? spd.V + spd.B_V : 0;
          spd.Berr= (spd.Verr && spd.B_Verr) ? ErrorComp(spd.B_Verr, spd.Verr) : 0;
          //U=  B + U-B       Uerr= sqrt( U-Berr^2 - Berr^2 )
          spd.U= (spd.B && spd.U_B) ? spd.B + spd.U_B : 0;
          spd.Uerr= (spd.Berr && spd.U_Berr) ? ErrorComp(spd.U_Berr, spd.Berr) : 0;
          //R=  V - V-R       Rerr= sqrt( V-Rerr^2 - Verr^2 )
          spd.R= (spd.V && spd.V_R) ? spd.V - spd.V_R : 0;
          spd.Rerr= (spd.Verr && spd.V_Rerr) ? ErrorComp(spd.V_Rerr, spd.Verr) : 0;
          //I=  V - V-I       Ierr= sqrt( V-Ierr^2 - Verr^2 )
          spd.I= (spd.V && spd.V_I) ? spd.V - spd.V_I : 0;
          spd.Ierr= (spd.Verr && spd.V_Ierr) ? ErrorComp(spd.V_Ierr, spd.Verr) : 0;
          //I=  R - R-I       Ierr= sqrt( R-Ierr^2 - Rerr^2 )
          if(!spd.I) { // if the first I formula didn't work, maybe this will
             spd.I= (spd.R && spd.R_I) ? spd.R - spd.R_I : 0;
             spd.Ierr= (spd.Rerr && spd.R_Ierr) ? ErrorComp(spd.R_Ierr, spd.Ierr) : 0;
          }

       } catch (...) {
          ShowMessage("bad record");
          break;
       }

       if(Lines==1) { // do the target
          Memo2->Lines->Append("--------=                      /                                              ");
          Memo2->Lines->Append("T001FN  =                    V /Target function                               ");
          sprintf(ss, "T001ID  =%21s /Target identifier                             ", spd.SName);
          ss[78]= 0; // make sure its not too long
          Memo2->Lines->Append(ss);
          sprintf(ss, "T001RA  =%21.5f /Target RA                                     ", spd.SRA);
          Memo2->Lines->Append(ss);
          sprintf(ss, "T001DC  =%21.5f /Target DEC                                    ", spd.SDec);
          Memo2->Lines->Append(ss);
        } else {  // all others are comp
          Memo2->Lines->Append("--------=                      /                                              ");
          sprintf(sc, "C%i", ++Stars);
          sprintf(ss, "S%03iFN  =%21s /Star function                                 ", Stars, sc);
          Memo2->Lines->Append(ss);
          sprintf(ss, "S%03iID  =%21s /Star identifier                               ", Stars, spd.SName);
          Memo2->Lines->Append(ss);
          sprintf(ss, "S%03iRA  =%21.5f /RA                                            ", Stars, spd.SRA);
          Memo2->Lines->Append(ss);
          sprintf(ss, "S%03iDC  =%21.5f /DC                                            ", Stars, spd.SDec);
          Memo2->Lines->Append(ss);
          sprintf(ss, "S%03iLab =%21s /Label                                         ", Stars, spd.Label);
          Memo2->Lines->Append(ss);
          for(Fs= 1; Fs<= (int)strlen(Filters); Fs++) {
             float mag, sd;
             switch(Filters[Fs-1]) {
                case 'V': mag= spd.V; sd= spd.Verr; break;
                case 'B': mag= spd.B; sd= spd.Berr; break;
                case 'U': mag= spd.U; sd= spd.Uerr; break;
                case 'R': mag= spd.R; sd= spd.Rerr; break;
                case 'I': mag= spd.I; sd= spd.Ierr; break;
             }
             sprintf(ss, "S%03iF%02iM=%21.3f /%c standard magnitude                          ", Stars, Fs, mag, Filters[Fs-1]);
             Memo2->Lines->Append(ss);
             sprintf(ss, "S%03iF%02iS=%21.3f /%c std dev                                     ", Stars, Fs, sd, Filters[Fs-1]);
             Memo2->Lines->Append(ss);
         }
      }
   } while(Lines++ < Memo1->Lines->Count);

   sprintf(ss, "NUMSTARS=                  %3i /Number of comp or field stars                 ", Stars);
   Memo2->Lines->Strings[2]= ss;

}
//---------------------------------------------------------------------------

float __fastcall TForm1::ErrorComp(float err1, float err2) {
      // err1 the combined error term
      float  err3;
      if(SPaltError->Checked) {
         if(err1 > err2) { // back into error implied
            err3= sqrt(err1*err1 - err2*err2);
         } else { // won't work; default
            err3= err2;
         }
      } else { // conservative, add in quadrature
         err3= sqrt(err1*err1 + err2*err2);
      }
      return err3;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSourceFile1Click(TObject *Sender)
{
   AnsiString as; int pos;
   OpenDialog1->FileName= SourceFile;
   if(OpenDialog1->Execute()) {
      as= OpenDialog1->FileName;
      pos = as.LastDelimiter("\\");
      SourceFile= as;
      SourceFile.Delete(1, pos);
      Memo1->Lines->LoadFromFile(OpenDialog1->FileName);
      as.Delete(pos, 1000);
      OpenDialog1->InitialDir= as;
   }
   PutIniData(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveSourceFile1Click(TObject *Sender)
{
   AnsiString as; int pos;
   SaveDialog2->FileName= SourceFile;
   if(SaveDialog2->Execute()) {
      as= SaveDialog2->FileName;
      pos = as.LastDelimiter("\\");
      SourceFile= as;
      SourceFile.Delete(1, pos);
      Memo1->Lines->SaveToFile(SaveDialog2->FileName);
      as.Delete(pos, 1000);
      SaveDialog2->InitialDir= as;
   }
   PutIniData(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveResultFile1Click(TObject *Sender)
{
   Button2Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::About1Click(TObject *Sender)
{
   fAbout->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SPaltErrorClick(TObject *Sender)
{
     PutIniData(Sender);
     //DoitButtonClick(Sender); // rerun xx Generates fault at start
     Memo4->Clear();
}
//---------------------------------------------------------------------------


