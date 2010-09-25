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
#include <ctype.h>
#include <stdio.h>
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

AnsiString SaveFile= "temp.txt";

#define Maxlines 128

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    AnsiString as;
    char  SName[64], SRA[32], SDec[32], ss[128], ss2[32], Filters[20], suffix[3], cs[256];
    int x, y, nextstate, state= 0, CDL;
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
             if(0==strcmp(T, "Label")) {
                state= 9;
                Filters[0]= 0;
             }
             break;
          case 9:
             if(strcmp(T, "Comments")) {
                strncat(Filters, T, 1);
             } else {
//                strcpy(delim, " \r");
                Memo2->Lines->Append("NUMSTARS=                    x /Number of comp or field stars                 "); // placeholder
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
             sprintf(ss2, ",%s", SRA); strcat(cs, ss2);
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
             break;
          case 17: // filter with no data?
             if(0==strcmp(T, "-")) {
                sprintf(ss, "S%03iF%02iM=%21s /standard magnitude                            ", Stars, ++Fs, "0.000");
                Memo2->Lines->Append(ss);
                sprintf(ss, "S%03iF%02iS=%21s /std dev                                       ", Stars, Fs, "0.000");
                Memo2->Lines->Append(ss);
                strcat(cs, ",,");
                if(Fs== strlen(Filters)) { // we've finished the line
                   Memo4->Lines->Append(cs);
                   state= 10;
                } else {
                   state= 17;
                }
             } else {
                sprintf(ss, "S%03iF%02iM=%21s /standard magnitude                            ", Stars, ++Fs, T);
                Memo2->Lines->Append(ss);
                sprintf(ss2, ",%s", T); strcat(cs, ss2);
                state= 18;
             }
             break;
          case 18: // get std dev
             strcpy(T, &T[1]); // kill lead (
             x= strcspn(T, ")"); T[x]= 0; // kill trailing }
             if(0==strcmp("N/A", T)) strcpy(T, "0.000");
             sprintf(ss, "S%03iF%02iS=%21s /std dev                                       ", Stars, Fs, T);
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
void __fastcall TForm1::Button2Click(TObject *Sender)
{
   AnsiString s;
   if(CSV1->Checked) s= SaveFile+ ".CSV";
   else              s= SaveFile+ ".STAR";
   SaveDialog1->FileName= s;
   if(SaveDialog1->Execute()) {
      if(CSV1->Checked)
         Memo4->Lines->SaveToFile(SaveDialog1->FileName);
      else
         Memo2->Lines->SaveToFile(SaveDialog1->FileName);
   }
   PutIniData(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
   GroupBox1->Visible= false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Help1Click(TObject *Sender)
{
      GroupBox1->BringToFront();
      GroupBox1->Visible= true;
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
   delete ini;

   // setup related to the ini's
   if(CSV1->Checked) {
      Memo4->BringToFront();
      Label3->Caption= "Photometry data in csv format appears here ";
   } else {
      Memo4->SendToBack();
      Label3->Caption= "AIPWin MMT STAR file appears here ";
   }
}


void __fastcall TForm1::CSV1Click(TObject *Sender)
{
    CSV1->Checked= CSV1->Checked? false: true;
    PutIniData(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button4Click(TObject *Sender)
{
    Memo1->Clear(); // input window
}
//---------------------------------------------------------------------------

