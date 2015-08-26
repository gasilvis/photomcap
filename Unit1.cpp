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
#include "helpBox.h"
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ElastFrm"
#pragma link "HttpProt"
#pragma link "LibXmlComps"
#pragma link "LibXmlParser"
#pragma resource "*.dfm"
TForm1 *Form1;

#define Version 22
/* add to PClog.php
   22
   - use new VSP API for AAVSO data
   21
   - tidy up httpGet
*/
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
#define MaxFilters 20
#define MaxComps 300
typedef struct {
   char mag[16];
   char sd[16];
} filterData;
filterData fDzero= { "0.000", "0.000" };

typedef struct {
   char AUID[20];
   char RA[32];   // decimal string
   char Dec[32];  // decimal string
   char Label[16];
   filterData f[MaxFilters]; // in the order of pdata.Filters
} compData;

typedef struct {
   char SName[80];
   char SRA[32];
   char SDec[32];
   char Filters[MaxFilters]; // master list and order. Single letter designation
   char Chart[16];
   int  NumStars;  // ie comps
   compData comp[MaxComps];
} pdata;

pdata pd;

double __fastcall hhmmss2degrees(char *z) {
   double h, m, s, r;
   // it might already be in degrees
   //if(1== sscanf(z, "%lf", &r))
      //return r;
   if(3==sscanf(z, "%lf:%lf:%lf", &h, &m, &s))
      return  15.0 * ((s/60.0+ m)/60.0 + h);
   else
      return h;
}

double __fastcall ddmmss2degrees(char* z) {
   double d, m, s, r;
   bool neg;
   // it might already be in degrees
//   if(1== sscanf(z, "%lf", &r))
//      return r;
   neg= z[0]=='-';
   if(neg) z[0]= ' ';
   if(3==sscanf(z, "%lf:%lf:%lf", &d, &m, &s)) {
      r= ((s/60.0+ m)/60.0 + d);
      if(neg) r*= -1.0;
      return r;
   } else
      return d;
}


AnsiString SaveFile= "temp";
AnsiString SourceFile= "";

void __fastcall TForm1::DoAAVSO2(TObject *Sender)
{
    char cp[100000];
    char suffix[3], ss[32];
    float fx;
/* This parser is designed to collect data for the AIPWin STAR file.
    - single letter filter names (use first letter offered. eg Ic is I)
    - no filter data has mag==0, err==0

*/

    pd.NumStars= 0;
    pd.Filters[0]= 0;
    Memo2->Clear(); // AIP STAR fmt

    // get data from API by chartID
    // new API call
    AnsiString u= "http://www.aavso.org/apps/vsp/api/chart/"+ chartEdit->Text+ "/?format=xml";
    //if(Form1->UseStdField->Checked) u+= "&special=std_field";
    // eg   http://www.aavso.org/apps/vsp/api/chart/2164EAF/?format=xml
    if(!Form1->httpGet(u, cp, sizeof(cp))) {
       Memo2->Lines->Append(" failed VSP API chart request.");
       return;
    }
    // cp not big enough?
//          FILE * tf= fopen("testing.txt", "wt");
//          fwrite(cp, 1, strlen(cp), tf);
//          fclose(tf);
    EasyXmlScanner1->LoadFromBuffer(cp);
    EasyXmlScanner1->XmlParser->Normalize= true;
    EasyXmlScanner1->XmlParser->StartScan();
    int listLevel= 0, i, j, k;
    AnsiString band, mag, Err;
    compData cd;
    filterData fd;
    char f; char *cptr= NULL;
    AnsiString CurName, CurContent, ChartID;
    while (EasyXmlScanner1->XmlParser->Scan()) {
       CurName= EasyXmlScanner1->XmlParser->CurName;
       CurContent= EasyXmlScanner1->XmlParser->CurContent;
       switch(EasyXmlScanner1->XmlParser->CurPartType) {
          case ptStartTag:
             if(CurName == "list-item") {
                listLevel+= 1;
                // prep for collecting data from the list item
                switch(listLevel) {
                   case 1: // photometry. Clean the cd
                      cd.AUID[0]= 0;
                      cd.RA[0]= 0;
                      cd.Dec[0]= 0;
                      cd.Label[0]= 0;
                      for(int i= 0; i< MaxFilters; i++) cd.f[i]= fDzero;
                      break;
                   case 2: // band
                      band= ""; mag= "0.000"; Err= "0.000";
                      break;
                }
             }
             break;

          case ptContent:
             switch(listLevel) {
                case 0: // root
                   //<comment/>
                   if(CurName == "star") { //<star>RR And</star>
                      strcpy(pd.SName, CurContent.c_str());
                   }
                   //<maglimit>16.0</maglimit>
                   //<special/>
                   //<auid>000-BBC-124</auid>
                   //<fov>30.0</fov>
                   else if(CurName == "dec") { //<dec>24:59:55.9</dec> nb not RR And
                      sprintf(pd.SDec, "%.5f", ddmmss2degrees(CurContent.c_str()));
                   }
                   //<title/>
                   //<dss>False</dss>
                   else if(CurName == "chartid") { //<chartid>X15267EM</chartid>
                      strcpy(pd.Chart, CurContent.c_str());
                   }
                   //<image_uri>https://www.aavso.org/apps/vsp/chart/X15267EM.png</image_uri>
                   else if(CurName == "ra") { //<ra>21:44:39.68</ra> nb not RR And
                      sprintf(pd.SRA, "%.5f", hhmmss2degrees(CurContent.c_str()));
                   }
                   //<resolution>100</resolution>
                   //<photometry> list
                   break; // root
                case 1: // photometry
                   if(CurName == "auid") { //<auid>000-BBC-116</auid>
                      strcpy(cd.AUID, CurContent.c_str());
                   }
                   //<bands> list
                   //<comments/>
                   else if(CurName == "label") { //<label>94</label>
                      strcpy(cd.Label, CurContent.c_str());
                   }
                   else if(CurName == "ra") { //<ra>00:51:10.69</ra>
                      sprintf(cd.RA, "%.5f", hhmmss2degrees(CurContent.c_str()));
                   }
                   else if(CurName == "dec") { //<dec>34:16:27.3</dec>
                      sprintf(cd.Dec, "%.5f", ddmmss2degrees(CurContent.c_str()));
                   }
                   break;
                case 2: // bands
                   if(CurName == "band") band= CurContent; //<band>V</band>
                   else if(CurName == "mag") mag= CurContent; //<mag>9.384</mag>
                   else if(CurName == "error") Err= CurContent; //<error>0.082</error>
                   break;
             }
             break;

          case ptEndTag:
             if(CurName == "list-item") {
                // use the data collected the list item
                switch(listLevel) {
                   case 2: // band
                      cptr= strchr(pd.Filters, band.c_str()[0]); // nb, just first letter of filter name
                      if(cptr==NULL) { // new filter?
                         j= strlen(pd.Filters);
                         pd.Filters[j]= band.c_str()[0];
                         pd.Filters[j+1]= 0;
                      } else
                         j= cptr - pd.Filters;
                      //strcpy(fd.mag, mag.c_str());
                      sscanf(mag.c_str(), "%f", &fx);
                      sprintf(fd.mag, "%.3f", fx);
                      //strcpy(fd.sd, Err.c_str());
                      sscanf(Err.c_str(), "%f", &fx);
                      sprintf(fd.sd, "%.3f", fx);
                      cd.f[j]= fd;
                      break;
                   case 1: // photometry
                      pd.comp[pd.NumStars++]= cd;
                      break;
                }
                listLevel-= 1;
             }
             break;
       }
    } //end while

    // fixups
    // look for duplicate Labels and modify with letter suffix
    for(i= 0; i< pd.NumStars-1; i++) { // don't need to do the last one
       strcpy(suffix, "a");
       for(j= i+1; j< pd.NumStars; j++) {
          if(!strcmp(pd.comp[i].Label, pd.comp[j].Label)) {
             suffix[0]++;
             strcat(pd.comp[j].Label, suffix);
          }
       }
       if(strcmp(suffix, "a")) { // go fix the first
          strcat(pd.comp[i].Label, "a");
       }
    }
    // Label option
    if(Labels1->Checked) {
       for(i= 0; i<pd.NumStars; i++) {
          //strcpy(ss, pd.comp[i].AUID);
          //sprintf(pd.comp[i].AUID, "C%s %s", pd.comp[i].Label, ss);
          sprintf(pd.comp[i].AUID, "C%s", pd.comp[i].Label);
       }
    }

    outputReport(Sender);
}

void __fastcall TForm1::outputReport(TObject *Sender)
{   // output data in pd

    // Clear output
    AnsiString ss;
    Memo2->Clear(); // AIP STAR fmt
    Memo4->Clear(); // CSV generic
    int Stars= 0;

    Memo2->Lines->Append("FILETYPE=             STARDATA /Star Data file                                ");
    Memo2->Lines->Append("NUMTARGS=                    1 /Number of targets                             ");
    Memo2->Lines->Append(ss.sprintf("NUMSTARS=                  %3i /Number of comp or field stars                 ", pd.NumStars));
    Memo2->Lines->Append(ss.sprintf("NUMFILTE=                  %3i /Number of filter bands                        ", strlen(pd.Filters)));
    for(int i= 0; i< (signed)strlen(pd.Filters); i++) {
       Memo2->Lines->Append(ss.sprintf("FILTER%02i=                    %c /Designation of filter band                    ", i+1 , pd.Filters[i]));
    }
    Memo2->Lines->Append(ss.sprintf("CHARTDES=%21s /Chart designation                             ", pd.Chart));
    Memo2->Lines->Append("--------=                      /                                              ");

    Memo2->Lines->Append("T001FN  =                    V /Target function                               ");
    ss.sprintf("T001ID  =%21s /Target identifier                             ", pd.SName);
    ss[78]= 0; // make sure its not too long
    Memo2->Lines->Append(ss);
    Memo2->Lines->Append(ss.sprintf("T001RA  =%21s /Target RA                                     ", pd.SRA));
    Memo2->Lines->Append(ss.sprintf("T001DC  =%21s /Target DEC                                    ", pd.SDec));

    for(Stars= 0; Stars < pd.NumStars; Stars++) {
       Memo2->Lines->Append("--------=                      /                                              ");
       Memo2->Lines->Append(ss.sprintf("S%03iFN  =                    C /Star function                                 ", Stars+1));
       Memo2->Lines->Append(ss.sprintf("S%03iID  =%21s /Star identifier                               ", Stars+1, pd.comp[Stars].AUID));
       Memo2->Lines->Append(ss.sprintf("S%03iRA  =%21s /RA                                            ", Stars+1, pd.comp[Stars].RA));
       Memo2->Lines->Append(ss.sprintf("S%03iDC  =%21s /DC                                            ", Stars+1, pd.comp[Stars].Dec));
       Memo2->Lines->Append(ss.sprintf("S%03iLab =%21s /Label                                         ", Stars+1, pd.comp[Stars].Label));
       for(unsigned int Fs= 0; Fs<strlen(pd.Filters); Fs++) {
          Memo2->Lines->Append(ss.sprintf("S%03iF%02iM=%21s /%c standard magnitude                          ", Stars+1, Fs+1, pd.comp[Stars].f[Fs].mag, pd.Filters[Fs]));
          Memo2->Lines->Append(ss.sprintf("S%03iF%02iS=%21s /%c std dev                                     ", Stars+1, Fs+1, pd.comp[Stars].f[Fs].sd, pd.Filters[Fs]));
       }
    }
    Memo2->Lines->Append("END                                                                           ");

    // CSV report
    char cs[256], ss2[256];
    sprintf(cs, "\"Starname\",\"%s\"", pd.SName);
    Memo4->Lines->Append(cs);
    sprintf(cs, "\"Star RA\",\"%s\"", pd.SRA);
    Memo4->Lines->Append(cs);
    sprintf(cs, "\"Star DEC\",\"%s\"", pd.SDec);
    Memo4->Lines->Append(cs);
    sprintf(cs, "\"ChartID\",\"%s\"", pd.Chart);
    Memo4->Lines->Append(cs);
    sprintf(cs, "\"AUID\",\"RA\",\"Dec\",\"Label\"");
    for(unsigned int i= 0; i<strlen(pd.Filters); i++) {
       sprintf(ss2, ",\"%c\",", pd.Filters[i]); // 2 cols: mag and err
       strcat(cs, ss2);
    }
    Memo4->Lines->Append(cs);
    for(Stars= 0; Stars < pd.NumStars; Stars++) {
       sprintf(cs, "%s, %s, %s, %s", pd.comp[Stars].AUID, pd.comp[Stars].RA, pd.comp[Stars].Dec, pd.comp[Stars].Label);
       for(unsigned int i= 0; i<strlen(pd.Filters); i++) {
          sprintf(ss2, ", %s, %s", pd.comp[Stars].f[i].mag, pd.comp[Stars].f[i].sd ); // 2 cols: mag and err
          strcat(cs, ss2);
       }
       Memo4->Lines->Append(cs);
    }

}



void __fastcall TForm1::DoAAVSO(TObject *Sender)
{
    AnsiString as;
    char  ss[128], ss2[32], suffix[3], cs[256];//, SName[64], SRA[32], SDec[32], Filters[MaxFilters];
    int x, y, nextstate, state= 0, CDL;
    unsigned int BmVcol= 999, BmVcolTmp; // filter column labeled B-V; This needs to be skipped
    unsigned int i, j, Fs, Stars;


    Memo1->SelectAll();
    char *M0= Memo1->Lines->GetText();
    if(0==strlen(M0))
       return;
    for(i= 0; i< strlen(M0); i++) if('\t'== M0[i]) M0[i]= ' '; // kill tabs

    char *T, *M1= M0;
    char* delim= " \r\n";
    //char  Labels[MaxComps][8];
    int LabelLine[MaxComps]; // to hold labels incase they need to
    //char  AUIDs[MaxComps][24]; // capture the AUIDs
    // be modified because they are dups.   12 12 will be changed to 12a 12b

    Memo2->Lines->Append("FILETYPE=             STARDATA /Star Data file                                ");
    Memo2->Lines->Append("NUMTARGS=                    1 /Number of targets                             ");
    pd.Chart[0]= 0; // clear
    T= strtok(M1, delim);
    do {
       switch(state) {
          case 0: // find star name
             if(0==strcmp(T, "Photometry") || 0==strcmp(T, "photometry")) {
                T= strtok(NULL, delim); // next token
                if(0==strcmp(T, "for")){ // ok, the name starts here
                   pd.SName[0]= 0; // init
                   state= 1;
                } else if(0==strcmp(T, "From") || 0==strcmp(T, "from")){ // uhoh, there is no star name
                   strcpy(pd.SName, "unknown");
                   state= 2;
                }
             }
             break;
          case 1: // get end of name (it may have embedded blanks
             if(0==strcmp(T, "From") || 0==strcmp(T, "from")) {
                strcpy(pd.SName, &pd.SName[1]); // trim leading blank
                for(i= 0; i<strlen(pd.SName); i++) pd.SName[i]= toupper(pd.SName[i]);
                state= 2;
             } else {
                strcat(pd.SName, " "); strcat(pd.SName, T); // build name
             }
             break;
          case 2: // find RA of star
             if(0==strcmp(T, "RA:")) {
                state= 3;
             }
             break;
          case 3: // Get angle
             strcpy(pd.SRA, T);
             state= 4;
             break;
          case 4: //
             if(DDCheckBox->Checked) {
                strcpy(T, &T[1]); // kill lead (
                while(!isdigit(T[strlen(T)-1]))
                   T[strlen(T)-1]= 0;
                strcpy(pd.SRA, T);
             }
             state= 5;
             break;
          case 5: // find dec
             if(0==strcmp(T, "Decl.:") || 0==strcmp(T, "Dec:")) {
                state= 6;
             }
             break;
          case 6: // Get DEC
             strcpy(pd.SDec, T);
             state= 7;
             break;
          case 7: //
             if(DDCheckBox->Checked) {
                strcpy(T, &T[1]); // kill lead (
                while(!isdigit(T[strlen(T)-1]))
                   T[strlen(T)-1]= 0;
                strcpy(pd.SDec, T);
             }
             state= 8;
             break;
          case 8: // get filters
             // Beta  look for chart line
             if(0==strcmp(T, "as")) {
                T= strtok(NULL, delim); // next token
                strcpy(pd.Chart, T);
             }
             if(0==strcmp(T, "Dec.") || 0==strcmp(T, "Dec")) {  // look for "Dec.   Label"
                T= strtok(NULL, delim);
                if(0==strcmp(T, "Label")) {
                   state= 9;
                   pd.Filters[0]= 0;
                }
             }
             break;
          case 9:
             if(strcmp(T, "Comments")) {
                if(strcmp(T, "B-V")) {
                   strncat(pd.Filters, T, 1);  // grab first letter of filter name
                } else {
                   BmVcol= strlen(pd.Filters);
                }
             } else {
//                strcpy(delim, " \r");

                Memo2->Lines->Append("NUMSTARS=                    x /Number of comp or field stars                 "); // plac  eholder
                sprintf(ss, "NUMFILTE=                   %2i /Number of filter bands                        ", strlen(pd.Filters));
                Memo2->Lines->Append(ss);
                for(i= 1; i<= strlen(pd.Filters); i++) {
                   sprintf(ss, "FILTER%02i=                    %c /Designation of filter band                    ", i, pd.Filters[i-1]);
                   Memo2->Lines->Append(ss);
                }
                Memo2->Lines->Append("CHARTDES=              xxxxxxx /Chart designation                             "); // placeholder
                CDL= Memo2->Lines->Count- 1;
                Memo2->Lines->Append("--------=                      /                                              ");
                Memo2->Lines->Append("T001FN  =                    V /Target function                               ");

                sprintf(ss, "T001ID  =%21s /Target identifier                             ", pd.SName);
                ss[78]= 0; // make sure its not too long
                Memo2->Lines->Append(ss);
                sprintf(ss, "T001RA  =%21s /Target RA                                     ", pd.SRA);
                Memo2->Lines->Append(ss);
                sprintf(ss, "T001DC  =%21s /Target DEC                                    ", pd.SDec);
                Memo2->Lines->Append(ss);

                sprintf(cs, "\"Starname\",\"%s\"", pd.SName);
                Memo4->Lines->Append(cs);
                sprintf(cs, "\"Star RA\",\"%s\"", pd.SRA);
                Memo4->Lines->Append(cs);
                sprintf(cs, "\"Star DEC\",\"%s\"", pd.SDec);
                Memo4->Lines->Append(cs);
                Memo4->Lines->Append(cs); // placeholder for chartid
                sprintf(cs, "\"AUID\",\"RA\",\"Dec\",\"Label\"");
                for(i= 1; i<=strlen(pd.Filters); i++) {
                   sprintf(ss2, ",\"%c\",", pd.Filters[i-1]); // 2 cols: mag and err
                   strcat(cs, ss2);
                }
                Memo4->Lines->Append(cs);

                Stars= -1;
                state= 11; // next token is the start of the first line
             }
             break;
          case 10: // eat the comments
             if(0==strcmp(T, "Report") || 0==strcmp(T, "AUID")) {
                state= 19;
                break;
             }
             if(11!=strlen(T)) // AUID's are always 11 long....  nope
                break;
             else if(!('-'==T[3] && '-'==T[7])) // They have two dashes; still a hope and prayer
                break;
          case 11: // beginning of the data line
             Stars++; // base 0
             Memo2->Lines->Append("--------=                      /                                              ");
             sprintf(ss, "S%03iFN  =                    C /Star function                                 ", Stars+1);
             Memo2->Lines->Append(ss);
             strcpy(pd.comp[Stars].AUID, T);
             sprintf(ss, "S%03iID  =%21s /Star identifier                               ", Stars+1, pd.comp[Stars].AUID);
             Memo2->Lines->Append(ss);
             sprintf(cs, "\"%s\"", T);
             state= 12;
             break;
          case 12: // get star RA
             strcpy(pd.comp[Stars].RA, T);
             state= 13;
             break;
          case 13: //
             if(DDCheckBox->Checked) {
                strcpy(T, &T[1]); // kill lead (
                while(!isdigit(T[strlen(T)-1]))
                   T[strlen(T)-1]= 0;
                strcpy(pd.comp[Stars].RA, T);
             }
             sprintf(ss, "S%03iRA  =%21s /RA                                            ", Stars+1, pd.comp[Stars].RA);
             Memo2->Lines->Append(ss);
             sprintf(ss2, ",%s", pd.comp[Stars].RA); strcat(cs, ss2);
             state= 14;
             break;
          case 14: // Get DEC
             strcpy(pd.comp[Stars].Dec, T);
             state= 15;
             break;
          case 15: //
             if(DDCheckBox->Checked) {
                strcpy(T, &T[1]); // kill lead (
                while(!isdigit(T[strlen(T)-1]))
                   T[strlen(T)-1]= 0;
                strcpy(pd.comp[Stars].Dec, T);
             }
             sprintf(ss, "S%03iDC  =%21s /DC                                            ", Stars+1, pd.comp[Stars].Dec);
             Memo2->Lines->Append(ss);
             sprintf(ss2, ",%s", pd.comp[Stars].Dec); strcat(cs, ss2);
             state= 16;
             break;
          case 16: // label
             strcpy(pd.comp[Stars].Label, T);
             sprintf(ss, "S%03iLab =%21s /Label                                         ", Stars+1, pd.comp[Stars].Label);
             Memo2->Lines->Append(ss);
             LabelLine[Stars]= Memo2->Lines->Count- 1;
             sprintf(ss2, ",%s", T); strcat(cs, ss2);
             state= 17;
             Fs= -1;
             BmVcolTmp= BmVcol;
             break;
          case 17:
             if((Fs+1)==BmVcolTmp) { // skip the B-V column
                if(strcmp(T, "-") || 0==strcmp(T, "\x97")) {
                   T= strtok(NULL, delim); // kill the std dev token too
                }
                BmVcolTmp= 999;
                if(Fs== strlen(pd.Filters)) { // we've finished the line
                   state= 10;
                }
                break;
             }
             Fs++;
             if(0==strcmp(T, "-") || 0==strcmp(T, "\x97")) { // filter with no data?
                strcpy(pd.comp[Stars].f[Fs].mag, "0.000");
                sprintf(ss, "S%03iF%02iM=%21s /%c standard magnitude                          ", Stars+1, Fs+1, pd.comp[Stars].f[Fs].mag, pd.Filters[Fs]);
                Memo2->Lines->Append(ss);
                strcpy(pd.comp[Stars].f[Fs].sd, "0.000");
                sprintf(ss, "S%03iF%02iS=%21s /%c std dev                                     ", Stars+1, Fs+1, pd.comp[Stars].f[Fs].sd, pd.Filters[Fs]);
                Memo2->Lines->Append(ss);
                strcat(cs, ",,");
                if((Fs+1)== strlen(pd.Filters)) { // we've finished the line
                   Memo4->Lines->Append(cs);
                   state= 10;
                }
             } else {
                strcpy(pd.comp[Stars].f[Fs].mag, T);
                sprintf(ss, "S%03iF%02iM=%21s /%c standard magnitude                          ", Stars+1, Fs+1, pd.comp[Stars].f[Fs].mag, pd.Filters[Fs]);
                Memo2->Lines->Append(ss);
                sprintf(ss2, ",%s", pd.comp[Stars].f[Fs].mag); strcat(cs, ss2);
                state= 18;
             }
             break;
          case 18: // get std dev
             strcpy(T, &T[1]); // kill lead (
             x= strcspn(T, ")"); T[x]= 0; // kill trailing }
             if(0==strcmp("N/A", T)) strcpy(T, "0.000");
             strcpy(pd.comp[Stars].f[Fs].sd, T);
             sprintf(ss, "S%03iF%02iS=%21s /%c std dev                                     ", Stars+1, Fs+1, pd.comp[Stars].f[Fs].sd, pd.Filters[Fs]);
             Memo2->Lines->Append(ss);
             sprintf(ss2, ",%s", pd.comp[Stars].f[Fs].sd); strcat(cs, ss2);
             if((Fs+1)== strlen(pd.Filters)) { // we've finished the line
                Memo4->Lines->Append(cs);
                state= 10;
             } else {
                state= 17;
             }
             break;
          case 19: // find chart
             if(0==strcmp(T, "as:") || strlen(pd.Chart) ) {
                state= 20;
             }
             break;
          case 20:
             //  Stars is index of the last comp star
             // look for duplicate Labels and modify with letter suffix
             for(i= 0; i<Stars; i++) { // don't need to do the last one
                strcpy(suffix, "a");
                for(j= i+1; j<=Stars; j++) {
                   if(!strcmp(pd.comp[i].Label, pd.comp[j].Label)) {
                      suffix[0]++;
                      strcat(pd.comp[j].Label, suffix);
                      sprintf(ss, "S%03iLab =%21s /Label                                         ", j+1, pd.comp[j].Label);
                      Memo2->Lines->Strings[LabelLine[j]]= ss;
                   }
                }
                if(strcmp(suffix, "a")) { // go fix the first
                   strcat(pd.comp[i].Label, "a");
                   sprintf(ss, "S%03iLab =%21s /Label                                         ", i+1, pd.comp[i].Label);
                   Memo2->Lines->Strings[LabelLine[i]]= ss;
                }
             }
             // Label option
             if(Labels1->Checked) {
                for(i= 0; i<=Stars; i++) {
                   //sprintf(ss2, "%s %s", Labels[i], AUIDs[i]);
                   //sprintf(ss, "S%03iID  =%21s /Star identifier                               ", i, ss2);
                   //Memo2->Lines->Strings[LabelLine[i]- 3]= ss;
                   // add label to C identifier
                   sprintf(ss2, "C%s", pd.comp[i].Label);
                   sprintf(ss, "S%03iFN  =%21s /Star function                                 ", i+1, ss2);
                   Memo2->Lines->Strings[LabelLine[i]- 4]= ss;
                }
             }
             pd.NumStars= Stars+1;
             sprintf(ss, "NUMSTARS=                  %3i /Number of comp or field stars                 ", pd.NumStars);
             Memo2->Lines->Strings[2]= ss;
             if(0== strlen(pd.Chart)) strcpy(pd.Chart, T);
             sprintf(ss, "CHARTDES=%21s /Chart designation                             ", pd.Chart);
             Memo2->Lines->Strings[CDL]= ss;
             sprintf(cs, "\"ChartID\",\"%s\"", pd.Chart);
             Memo4->Lines->Strings[3]= cs;
             as= pd.SName;
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
void __fastcall TForm1::ReDoButtonClick(TObject *Sender)
{
   int i, is[20]; AnsiString s;
   // check inputs
   for(i= 0, s= CLabelEdit->Text; i< MaxComps; i++) {
      if(0==s.AnsiCompareIC(pd.comp[i].Label)) break;
   }
   if(i==MaxComps) {
      ShowMessage("C Label not found");
      //return;
   } else {
      is[0]= i;
   }
   for(i= 0, s= KLabelEdit->Text; i< MaxComps; i++) {
      if(0==s.AnsiCompareIC(pd.comp[i].Label)) break;
   }
   if(i==MaxComps) {
      ShowMessage("K Label not found");
      //return;
   } else {
      is[1]= i;
   }


   // then do it
   ReDo(Sender, 2, is);
   /* do all
   for(i= 0; i<pd.NumStars; i++) is[i]= i;
   ReDo(Sender, pd.NumStars, is);
   */
}


//---------------------------------------------------------------------------
// reprint the AAVSO reports
void __fastcall TForm1::ReDo(TObject *Sender, unsigned int num, int* comps)
{
    char  ss[128], ss2[32], cs[256];
    unsigned int i, j;
    // Clear output
    Memo2->Clear(); // AIP STAR fmt
    Memo4->Clear(); // CSV generic

    Memo2->Lines->Append("FILETYPE=             STARDATA /Star Data file                                ");
    Memo2->Lines->Append("NUMTARGS=                    1 /Number of targets                             ");
    sprintf(ss, "NUMSTARS=                  %3i /Number of comp or field stars                 ", num);
    Memo2->Lines->Append(ss);
    sprintf(ss, "NUMFILTE=                   %2i /Number of filter bands                        ", strlen(pd.Filters));
    Memo2->Lines->Append(ss);
    for(i= 0; i< strlen(pd.Filters); i++) {
        sprintf(ss, "FILTER%02i=                    %c /Designation of filter band                    ", i+1, pd.Filters[i]);
        Memo2->Lines->Append(ss);
    }
    sprintf(ss, "CHARTDES=%21s /Chart designation                             ", pd.Chart);
    Memo2->Lines->Append(ss);

    Memo2->Lines->Append("--------=                      /                                              ");
    Memo2->Lines->Append("T001FN  =                    V /Target function                               ");
    sprintf(ss, "T001ID  =%21s /Target identifier                             ", pd.SName);
    ss[78]= 0; // make sure its not too long
    Memo2->Lines->Append(ss);
    sprintf(ss, "T001RA  =%21s /Target RA                                     ", pd.SRA);
    Memo2->Lines->Append(ss);
    sprintf(ss, "T001DC  =%21s /Target DEC                                    ", pd.SDec);
    Memo2->Lines->Append(ss);

    sprintf(cs, "\"Starname\",\"%s\"", pd.SName);
    Memo4->Lines->Append(cs);
    sprintf(cs, "\"Star RA\",\"%s\"", pd.SRA);
    Memo4->Lines->Append(cs);
    sprintf(cs, "\"Star DEC\",\"%s\"", pd.SDec);
    Memo4->Lines->Append(cs);
    sprintf(cs, "\"ChartID\",\"%s\"", pd.Chart);
    Memo4->Lines->Append(cs);
    sprintf(cs, "\"AUID\",\"RA\",\"Dec\",\"Label\"");
    for(i= 0; i<strlen(pd.Filters); i++) {
       sprintf(ss2, ",\"%c\",", pd.Filters[i]); // 2 cols: mag and err
       strcat(cs, ss2);
    }
    Memo4->Lines->Append(cs);

    for(i= 0; i<num; i++) {
       Memo2->Lines->Append("--------=                      /                                              ");
       sprintf(ss, "S%03iFN  =                    C /Star function                                 ", i+1);
       if(Labels1->Checked) {
          sprintf(ss2, "C%s", pd.comp[comps[i]].Label);
          sprintf(ss, "S%03iFN  =%21s /Star function                                 ", i+1, ss2);
       }
       Memo2->Lines->Append(ss);
       sprintf(ss, "S%03iID  =%21s /Star identifier                               ", i+1, pd.comp[comps[i]].AUID);
       Memo2->Lines->Append(ss);
       sprintf(cs, "\"%s\"", pd.comp[comps[i]].AUID);
       sprintf(ss, "S%03iRA  =%21s /RA                                            ", i+1, pd.comp[comps[i]].RA);
       Memo2->Lines->Append(ss);
       sprintf(ss2, ",%s", pd.comp[comps[i]].RA); strcat(cs, ss2);
       sprintf(ss, "S%03iDC  =%21s /DC                                            ", i+1, pd.comp[comps[i]].Dec);
       Memo2->Lines->Append(ss);
       sprintf(ss2, ",%s", pd.comp[comps[i]].Dec); strcat(cs, ss2);
       sprintf(ss, "S%03iLab =%21s /Label                                         ", i+1, pd.comp[comps[i]].Label);
       Memo2->Lines->Append(ss);
       sprintf(ss2, ",%s", pd.comp[comps[i]].Label); strcat(cs, ss2);

       for(j= 0; j< strlen(pd.Filters); j++) {
          sprintf(ss, "S%03iF%02iM=%21s /%c standard magnitude                          ", i+1, j+1, pd.comp[comps[i]].f[j].mag, pd.Filters[j]);
          Memo2->Lines->Append(ss);
          sprintf(ss2, ",%s", pd.comp[comps[i]].f[j].mag); strcat(cs, ss2);
          sprintf(ss, "S%03iF%02iS=%21s /%c std dev                                     ", i+1, j+1, pd.comp[comps[i]].f[j].sd, pd.Filters[j]);
          Memo2->Lines->Append(ss);
          sprintf(ss2, ",%s", pd.comp[comps[i]].f[j].sd); strcat(cs, ss2);
       }
    }
    Memo2->Lines->Append("END                                                                           ");
}

void __fastcall TForm1::DoitButtonClick(TObject *Sender)
{
    if(SeqPlot1->Checked) {
       DoSeqPlot(Sender);
    } else {
       //DoAAVSO(Sender);
       DoAAVSO2(Sender);
    }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Button2Click(TObject *Sender)
{
   if(CSV1->Checked) {
      Memo4->SelectAll();
      Memo4->CopyToClipboard();
   } else {
      Memo2->SelectAll();
      Memo2->CopyToClipboard();
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Help1Click(TObject *Sender)
{
//      Application->HelpJump("Introduction");
        Form2->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Labels1Click(TObject *Sender)
{
    Labels1->Checked= Labels1->Checked? false: true;
/* from SDG (?)
   // collect INI file entries
   TIniFile *ini;
   if(0==INIfilename.Length()) { // first time
      INIfilename= ChangeFileExt( Application->ExeName, ".INI");
      ini= new TIniFile(INIfilename);
      INIfilename= ini->ReadString("Setup", "INI", INIfilename); // allows changing the INIfilename some day
      delete ini;
   }
   ini = new TIniFile(INIfilename);

   OpenDialog1->FilterIndex= ini->ReadInteger("Setup", "FilterIndex", 2); // default to all
   dataDir= ini->ReadString("Setup", "dataDir", "");
   observerEdit->Text= ini->ReadString("Setup", "Observer", "");
   Label4->Caption= reportDir= ini->ReadString("Setup", "reportDir", "");
   reportFile= ini->ReadString("Setup", "reportFile", "None");
   reportByMonth->Checked= ini->ReadBool("Setup", "reportByMonth", false);
   lastFile= ini->ReadString("Setup", "lastFile", "");
   lastFileLabel->Caption= lastFile;
   openReportFile(Sender, false);
   lt= ini->ReadInteger("Setup", "LogType", 0); // default to SuperSid
   LogTypes[lt]->Checked= true;
   freqEdit->Text= Frequency= ini->ReadString("Setup", "lastFrequency", 0);
   stationEdit->Text= StationID= ini->ReadString("Setup", "lastStation", "   ");
   delete ini;
*/
   PutIniData(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
   TStream *DataIn;
   char cp[10000];
   int cver;
   AnsiString s;
   Form1->Caption= s.sprintf("PhotomCap : Capturing AAVSO photometry into AIPWin STAR format, and other formats too (version %d)", Version);
   GetIniData(Sender);

   DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
   char szBuf[MAX_COMPUTERNAME_LENGTH + 1];
   szBuf[0] = '\0';
   //Retrieves the computer name
   GetComputerName(szBuf, &dwSize);

   // get current version information
//   if(httpGet(EncodeURIComponent(s.sprintf("http://www.gasilvis.com/PhotomCap/PClog.php?logentry=%s, ver=%d", szBuf, Version)), cp, sizeof(cp))) {
   if(httpGet(s.sprintf("http://www.gasilvis.com/PhotomCap/PClog.php?logentry=%s, ver=%d", szBuf, Version), cp, sizeof(cp))) {
      sscanf(cp, "%d", &cver);
      if(cver > Version) {
         versionLabel->Font->Color= clBlue;
         versionLabel->Caption= s.sprintf("Click here to download version %i", cver);
      } else {
         versionLabel->Caption= s.sprintf("%i is the latest version", cver);
      }
   }
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
      Label2->Visible= true;
      Memo1->Visible= true;
      SPaltError->Visible= true;
      chartMsg->Visible= false;
      chartEdit->Visible= false;
   } else {
      Label2->Caption= "Paste AAVSO photometry data here";
      SPaltError->Visible= false;
      Label2->Visible= false;
      Memo1->Visible= false;
      chartMsg->Visible= true;
      chartEdit->Visible= true;
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


void __fastcall TForm1::versionLabelClick(TObject *Sender)
{
    if(versionLabel->Tag) {
       ShellExecute(Handle,"open", "https://github.com/gasilvis/photomcap/raw/master/PhotomCap.exe",0,0,SW_SHOW);
//       ShellExecute(Handle,"open", "http://www.gasilvis.com/PhotomCap/PhotomCap.exe",0,0,SW_SHOW);
    }
}
//---------------------------------------------------------------------------



/* /  not needed
bool __fastcall TForm1::IsSafeChar(int ch)
{
   bool Result;
   if     (ch >= 48 && ch <= 57) Result= true;    // 0-9
   else if(ch >= 61 && ch <= 90) Result= true;  // =?>@A-Z
   else if(ch >= 97 && ch <= 122) Result= true;  // a-z
   else if(ch == 33) Result= true; // !
   else if(ch >= 39 && ch <= 42) Result= true; // '()*
   else if(ch >= 44 && ch <= 46) Result= true; // ,-.
   else if(ch == 95) Result= true; // _
   else if(ch == 126) Result= true; // ~
   else if(ch == 58) Result= true; // :
   else if(ch == 47) Result= true; // /
   else Result= false;
   return Result;
}

// the only thing needed to be encoded are spaces
// this code is pretty hacked up
AnsiString __fastcall TForm1::EncodeURIComponent(AnsiString ASrc)
{
   AnsiString UTF8String, HexMap= "0123456789ABCDEF", Result= "", ASrcUTF8;
   int I= 1, J= 1;

   ASrcUTF8= ASrc; //ASrcUTF8 := UTF8Encode(ASrc);
   // UTF8Encode call not strictly necessary but
   // prevents implicit conversion warning

   Result.SetLength(ASrcUTF8.Length() * 3); // space to %xx encode every byte
   while(I <= ASrcUTF8.Length()) {
      if(IsSafeChar(ASrcUTF8[I])) {
         Result[J]= ASrcUTF8[I];
         J++;
      }
//      else if(ASrcUTF8[I] == ' ') {
//         Result[J]= '+';
//         J++;
//      }
      else {
         Result[J]= '%';
         Result[J+1]= HexMap[(ASrcUTF8[I] >> 4) + 1];
         Result[J+2]= HexMap[(ASrcUTF8[I] & 0x0F) + 1];
         J+= 3;
      }
      I++;
   }
   Result.SetLength(J-1);
   return Result;
}
*/


// http Get stuff
bool __fastcall TForm1::httpGet(AnsiString URL, char* buffer, int bufsize)
{
   TStream *DataIn;
   // simple encoding: replace ' ' with '+'
   while(URL.Pos(" ")) URL[URL.Pos(" ")]= '+';
   HttpCli1->URL        = URL;
   HttpCli1->RcvdStream = NULL;
   char altbuffer[100];
   char* buf;
   if(buffer==NULL) { // return not expected
      buf= altbuffer;
      bufsize= sizeof(buffer);
   } else { buf= buffer; }
   try {
      HttpCli1->Get();
      DataIn = new TFileStream(Form1->HttpCli1->DocName, fmOpenRead);
      DataIn->ReadBuffer(buf, min(bufsize, DataIn->Size));
      delete DataIn;
      remove(HttpCli1->DocName.c_str());
      return true;
   } __except (TRUE) {
      //Form1->Memo4->Lines->Add("GET Failed !");
      //Form1->Memo4->Lines->Add("StatusCode   = " + IntToStr(Form1->HttpCli1->StatusCode));
      //Form1->Memo4->Lines->Add("ReasonPhrase = " + Form1->HttpCli1->ReasonPhrase);
      return false;
   }
}
void __fastcall TForm1::HttpCli1DocBegin(TObject *Sender)
{
    //Memo4->Lines->Add(HttpCli1->ContentType + " => " + HttpCli1->DocName);
    //Memo4->Lines->Add("Document = " + HttpCli1->DocName);
    HttpCli1->RcvdStream = new TFileStream(HttpCli1->DocName, fmCreate);
}

void __fastcall TForm1::HttpCli1DocEnd(TObject *Sender)
{
    if (HttpCli1->RcvdStream) {
        delete HttpCli1->RcvdStream;
        HttpCli1->RcvdStream = NULL;
    }
}
//---------------------------------------------------------------------------


/*
// http://www.experts-exchange.com/Programming/Languages/CPP/Q_20552424.html
 DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
   char szBuf[MAX_COMPUTERNAME_LENGTH + 1];
   szBuf[0] = '\0';

   //Retrieves the computer name
   GetComputerName(szBuf, &dwSize);

   //Adds the computer name to the memo
   Memo1->Lines->Add(AnsiString(szBuf));

//Windows Socket initialization stuff
   //Why the gethostbyname returned null
   WSAData wsaData;
   if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        ShowMessage("Error!");
    }

   //Gethostbyname
   //Better to use this funtion because you could use a dns name or a computer name
   struct hostent *phe;
   phe = gethostbyname(szBuf);

   //Converts the struct address to an AnsiString format
   AnsiString ipaddr;
   ipaddr = AnsiString(int(LOBYTE(LOWORD(*((DWORD*)phe->h_addr_list[0]))))) + ".";
   ipaddr+= AnsiString(int(HIBYTE(LOWORD(*((DWORD*)phe->h_addr_list[0]))))) + ".";
   ipaddr+= AnsiString(int(LOBYTE(HIWORD(*((DWORD*)phe->h_addr_list[0]))))) + ".";
   ipaddr+= AnsiString(int(HIBYTE(HIWORD(*((DWORD*)phe->h_addr_list[0])))));

   //Add the ip address to the memo
   Memo1->Lines->Add(ipaddr);

   //Windows socket clean up
   WSACleanup();
*/



