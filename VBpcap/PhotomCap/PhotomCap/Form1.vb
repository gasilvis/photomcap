Imports System.Net
Imports System.Text
Imports System.Xml
Imports System.Web

Public Class Form1

    Public version As String = "25"

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        ReDim pd.comp(MaxComps - 1)
        ReDim pd.fd(MaxComps - 1, MaxFilters - 1)
        pd.NumStars = 0
        Text = String.Format("PhotomCap : Capturing AAVSO photometry into AIPWin STAR format, and other formats too (version {0})", version)
        Labels1.Checked = My.Settings.Labels
        CSV1.Checked = My.Settings.CSV
    End Sub



    ' Menu items

    Private Sub SaveResultFile1_Click(sender As Object, e As EventArgs) Handles SaveResultFile1.Click
        SaveFileDialog1.Filter = "STAR Files (*.star*)|*.star"
        SaveFileDialog1.InitialDirectory = My.Settings.SaveDir
        If SaveFileDialog1.ShowDialog = Windows.Forms.DialogResult.OK Then
            My.Computer.FileSystem.WriteAllText(SaveFileDialog1.FileName, Memo2.Text, True)
            Dim sPath As String = SaveFileDialog1.FileName 'get the path
            sPath = StrReverse(sPath) 'reverse the string
            sPath = Mid(sPath, InStr(sPath, "\"), Len(sPath)) 'extract from the first slash
            sPath = StrReverse(sPath) 'reverse it again
            My.Settings.SaveDir = sPath
        End If
    End Sub

    Private Sub Exit1_Click(sender As Object, e As EventArgs) Handles Exit1.Click
        Close()
    End Sub

    Private Sub Help2_Click(sender As Object, e As EventArgs) Handles Help2.Click
        HelpForm.Show()
    End Sub

    Private Sub About1_Click(sender As Object, e As EventArgs) Handles About1.Click
        AboutBox1.Show()
    End Sub

    Private Sub DoitButton_Click(sender As Object, e As EventArgs) Handles DoitButton.Click
        DoAAVSO2()
    End Sub

    Private Sub RedoButton_Click(sender As Object, e As EventArgs) Handles RedoButton.Click
        ' check inputs
        If pd.NumStars = 0 Then
            MessageBox.Show("Need load data and DoIt first before you ReDoIt")
            Return
        End If
        Dim s As String = CLabelEdit.Text
        Dim i As Integer
        Dim iss(1) As Integer  ' just the C and K star
        For i = 0 To pd.NumStars - 1
            If 0 = String.Compare(s, pd.comp(i).Label) Then
                Exit For
            End If
        Next
        If i = pd.NumStars Then
            MessageBox.Show("C Label not found")
            Return
        Else
            iss(0) = i
        End If

        s = KLabelEdit.Text
        For i = 0 To pd.NumStars - 1
            If 0 = String.Compare(s, pd.comp(i).Label) Then
                Exit For
            End If
        Next
        If i = pd.NumStars Then
            MessageBox.Show("K Label not found")
            Return
        Else
            iss(1) = i
        End If

        ' then re do it
        outputReport(iss)
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        Memo2.Clear()
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        Clipboard.SetText(Memo2.Text)
    End Sub

    Const MaxFilters As Integer = 20
    Const MaxComps As Integer = 300

    Public Structure filterData
        Dim mag As String
        Dim sd As String
    End Structure
    Dim fDzero As New filterData With {.mag = "0.000", .sd = "0.000"}
    Dim fd As filterData

    Public Structure compData
        Dim AUID As String
        Dim RA As String
        Dim Dec As String
        Dim Label As String
        'Dim f() As filterData ' need to redim to MaxFilters
        'Dim mag() As String
        'Dim sd() As String
    End Structure
    Dim cd As compData

    Public Structure pdata
        Dim SName As String
        Dim Sauid As String
        Dim SRA As String
        Dim SDec As String
        Dim Filters As String ' MaxFilters, master list and order. Single letter designation
        Dim Chart As String
        Dim NumStars As Integer
        Dim comp() As compData ' MaxComps
        Dim fd(,) As filterData
    End Structure
    Dim pd As pdata


    Private Function hhmmss2degrees(ByVal z As String) As Double
        Dim h, m, s, r As Double
        Dim za() As String = z.Split(":")
        h = Double.Parse(za(0))
        m = Double.Parse(za(1))
        s = Double.Parse(za(2))
        r = 15.0 * ((s / 60.0 + m) / 60.0 + h) ' deg
        Return r
    End Function

    Private Function ddmmss2degrees(ByRef z As String) As Double
        Dim d, m, s, r As Double
        Dim zd As String = z.Trim()
        Dim neg As Boolean = False ' what if   -00:00:43
        If "-" = zd(0) Then
            neg = True
            zd = z.Substring(1)
        End If
        Dim za() As String = zd.Split(":")
        d = Double.Parse(za(0))
        m = Double.Parse(za(1))
        s = Double.Parse(za(2))
        r = ((s / 60.0 + m) / 60.0 + d) ' deg
        If neg Then r = -r
        Return r
    End Function

    Dim SaveFile As String = "temp"
    Dim SourceFile As String = ""


    Private Sub DoAAVSO2()
        '/* This parser Is designed to collect data for the AIPWin STAR file.
        '    - single letter filter names (use first letter offered. eg Ic Is I)
        '    - no filter data has mag==0, err==0
        Dim client As New WebClient
        Dim u As String

        pd.NumStars = 0
        pd.Filters = ""
        Memo2.Text = "" ' Clear() ' // AIP STAR fmt

        '    // get data from API by chartID
        '    // New API call
        u = "http://www.aavso.org/apps/vsp/api/chart/" & chartEdit.Text & "/?format=xml"
        'u = "http://www.aavso.org/apps/vsp/api/chart/2164EAF/?format=xml"
        'u = "http://www.aavso.org/apps/vsp/api/chart/X17787AU/?format=xml"

        'Try
        ParseAAVSOData2(u)
        'Catch ex As Exception
        ' Memo2.AppendText("Failed VXP API chart request: " & ex.Message)
        ' Return 'False
        ' End Try

        Dim iss() As Integer
        ReDim iss(pd.NumStars - 1)
        For i As Integer = 0 To pd.NumStars - 1
            iss(i) = i
        Next
        outputReport(iss)

    End Sub

    Private Sub ParseAAVSOData2(ByRef dataURL As String)

        Dim reader As New XmlTextReader(dataURL)
        'If  Then return
        Dim lastReaderName As String = "" ' because reader.Name not set when Text is offered
        Dim listLevel As Integer = 0
        Dim i, j As Integer
        ' What we are collecting
        Dim name As String = ""
        Dim auid As String = ""
        ' intermediates
        Dim band As String = ""
        Dim mag, Err As Double
        ' for call to add 
        'Dim mags As String
        Dim filterOK As Boolean
        Dim nbr As Integer = 0

        ' clear fd info from pd
        For i = 0 To MaxComps - 1
            For j = 0 To MaxFilters - 1
                pd.fd(i, j).mag = "0.000"
                pd.fd(i, j).sd = "0.000"
            Next
        Next
        Try
            reader.Read()
        Catch ex As Exception
            Return
        End Try
        While reader.Read()
            nbr += 1
            ' assumptions on XML structure:
            ' only list-items are in <photometry> and <photometry><bands>
            Select Case reader.NodeType
                Case XmlNodeType.Element
                    lastReaderName = reader.Name
                    If reader.Name = "list-item" Then
                        listLevel += 1
                        ' prep for collecting data from the list item
                        Select Case listLevel
                            Case 1 'photometry
                                'name = "" : auid = ""
                                'decl = 0 : ra = 0 : b = 0 : v = 0 : r = 0 : i = 0 : u = 0
                                'bErr = 0 : vErr = 0 : rErr = 0 : iErr = 0 : uErr = 0
                                filterOK = False
                                cd.AUID = ""
                                cd.RA = ""
                                cd.Dec = ""
                                cd.Label = ""
                                For ii As Integer = 0 To MaxFilters - 1
                                    'cd.f(ii) = fDzero
                                    'cd.mag(ii) = ""
                                    'cd.sd(ii) = ""
                                Next
                            Case 2 'band
                                band = "" : mag = 0 : Err = 0
                        End Select
                    End If

                Case XmlNodeType.Text
                    Select Case listLevel
                        Case 0 'root
                            Select Case lastReaderName
                                    ' available reader.Name's:
                                    '<comment/>
                                Case "star" ''<star>RR And</star>
                                    pd.SName = ToStr(reader.Value)
                                    '<maglimit>16.0</maglimit>
                                    '<special/>
                                Case "auid" '<auid>000-BBC-124</auid>
                                    pd.Sauid = ToStr(reader.Value)
                                    '<fov>30.0</fov>
                                Case "dec" '<dec>34.37689</dec>
                                    If -1 = reader.Value.IndexOf(":") Then
                                        pd.SDec = String.Format("{0:0.00000}", CDbl(Val(reader.Value)))
                                    Else
                                        pd.SDec = String.Format("{0:0.00000}", ddmmss2degrees((reader.Value)))
                                    End If
                                    '<title/>
                                    '<dss>False</dss>
                                Case "chartid" '<chartid>X15267EM</chartid>
                                    pd.Chart = ToStr(reader.Value)
                                    '<image_uri>https : //www.aavso.org/apps/vsp/chart/X15267EM.png</image_uri>
                                Case "ra"     '<ra>12.84717</ra>
                                    If -1 = reader.Value.IndexOf(":") Then
                                        pd.SRA = String.Format("{0:0.00000}", CDbl(Val(reader.Value)))
                                    Else
                                        pd.SRA = String.Format("{0:0.00000}", hhmmss2degrees((reader.Value)))
                                    End If
                                    '<resolution>100</resolution>
                                    '<photometry> list
                            End Select
                        Case 1 'photometry
                            Select Case lastReaderName
                                '<comments/>
                                Case "ra" '<ra>00:51:10.69</ra>
                                    cd.RA = String.Format("{0:0.00000}", hhmmss2degrees(reader.Value))
                                Case "dec" '<dec>34:16:27.3</dec>
                                    cd.Dec = String.Format("{0:0.00000}", ddmmss2degrees(reader.Value))
                                Case "label" '<label>94</label>
                                    cd.Label = ToStr(reader.Value)
                                Case "auid" '<auid>000-BBC-116</auid>
                                    cd.AUID = ToStr(reader.Value)
                                    '<bands> list
                            End Select
                        Case 2 'bands
                            Select Case lastReaderName
                                Case "band" '<band>V</band>
                                    band = reader.Value
                                Case "mag" '<mag>9.384</mag>
                                    mag = Convert.ToDouble(reader.Value)
                                Case "error" '<error>0.082</error>
                                    Err = Convert.ToDouble(reader.Value)
                            End Select
                    End Select

                Case XmlNodeType.EndElement 'ptEndTag
                    If reader.Name = "list-item" Then
                        Select Case listLevel
                            Case 0 'root
                            Case 1    ' photometry
                                pd.comp(pd.NumStars) = cd
                                pd.NumStars += 1
                            Case 2   ' band
                                j = InStr(pd.Filters, band(0)) ' // nb, just first letter Of filter name
                                If (j = 0) Then ' // New filter?
                                    pd.Filters = pd.Filters & band(0)
                                    j = Len(pd.Filters) - 1
                                Else
                                    j = j - 1 ' instr is 1 base
                                End If
                                pd.fd(pd.NumStars, j).mag = String.Format("{0:0.000}", mag)
                                pd.fd(pd.NumStars, j).sd = String.Format("{0:0.000}", Err)
                                'cd.f(j) = fd
                        End Select
                        listLevel -= 1
                    End If

                    Select Case reader.Name
                        Case "root"
                            ' exitroot
                            Exit While
                    End Select
                Case Else
                    ' unkown node
            End Select ' nodeType
        End While
        'chartID = chartID & nbr

        ' fixups
        ' look for duplicate Labels And modify with letter suffix
        Dim suffix As Char
        For i = 0 To pd.NumStars - 2 ' don't need to do the last one
            suffix = "a"
            For j = i + 1 To pd.NumStars - 1
                If (0 = String.Compare(pd.comp(i).Label, pd.comp(j).Label)) Then
                    suffix = Chr(Asc(suffix) + 1)
                    pd.comp(j).Label &= suffix
                End If
            Next
            If suffix <> "a" Then ' go fix the first
                pd.comp(i).Label &= "a"
            End If
        Next

        ' Label option
        If (Labels1.Checked) Then
            For i = 0 To pd.NumStars - 1
                '          //strcpy(ss, pd.comp[i].AUID);
                '          //sprintf(pd.comp[i].AUID, "C%s %s", pd.comp[i].Label, ss);
                pd.comp(i).AUID = "C" & pd.comp(i).Label
            Next
        End If

    End Sub

    Private Sub outputReport(iss() As Integer)
        If CSV1.Checked Then
            CSVreport(iss)
            Return
        End If

        ' output data in pd
        Memo2.Clear() ' clear output ; // AIP STAR fmt
        Dim Stars As Integer = 0
        Memo2.AppendText("FILETYPE=             STARDATA /Star Data file                                ")
        Memo2.AppendText(vbCrLf & "NUMTARGS=                    1 /Number of targets                             ")
        Memo2.AppendText(vbCrLf & String.Format("NUMSTARS=                  {0,3} /Number of comp or field stars                 ", pd.NumStars))
        Memo2.AppendText(vbCrLf & String.Format("NUMFILTE=                  {0,3} /Number of filter bands                        ", Len(pd.Filters)))
        For i As Integer = 0 To Len(pd.Filters) - 1
            Memo2.AppendText(vbCrLf & String.Format("FILTER{0:00}=                    {1:c} /Designation of filter band                    ", i + 1, pd.Filters(i)))
        Next
        Memo2.AppendText(vbCrLf & String.Format("CHARTDES={0,21} /Chart designation                             ", pd.Chart))
        Memo2.AppendText(vbCrLf & "--------=                      /                                              ")

        Memo2.AppendText(vbCrLf & "T001FN  =                    V /Target function                               ")
        Memo2.AppendText(vbCrLf & String.Format("T001ID  ={0,21} /Target identifier                             ", pd.SName))
        Memo2.AppendText(vbCrLf & String.Format("T001RA  ={0,21} /Target RA                                     ", pd.SRA))
        Memo2.AppendText(vbCrLf & String.Format("T001DC  ={0,21} /Target DEC                                    ", pd.SDec))

        For i As Integer = 0 To iss.Length - 1
            Stars = iss(i)
            Memo2.AppendText(vbCrLf & "--------=                      /                                              ")
            Memo2.AppendText(vbCrLf & String.Format("S{0:000}FN  =                    C /Star function                                 ", Stars + 1))
            Memo2.AppendText(vbCrLf & String.Format("S{0:000}ID  ={1,21} /Star identifier                               ", Stars + 1, pd.comp(Stars).AUID))
            Memo2.AppendText(vbCrLf & String.Format("S{0:000}RA  ={1,21} /RA                                            ", Stars + 1, pd.comp(Stars).RA))
            Memo2.AppendText(vbCrLf & String.Format("S{0:000}DC  ={1,21} /DC                                            ", Stars + 1, pd.comp(Stars).Dec))
            Memo2.AppendText(vbCrLf & String.Format("S{0:000}Lab ={1,21} /Label                                         ", Stars + 1, pd.comp(Stars).Label))
            For Fs As Integer = 0 To Len(pd.Filters) - 1
                Memo2.AppendText(vbCrLf & String.Format("S{0:000}F{1:00}M={2,21} /{3:c} standard magnitude                          ", Stars + 1, Fs + 1, pd.fd(Stars, Fs).mag, pd.Filters(Fs)))
                Memo2.AppendText(vbCrLf & String.Format("S{0:000}F{1:00}S={2,21} /{3:c} std dev                                     ", Stars + 1, Fs + 1, pd.fd(Stars, Fs).sd, pd.Filters(Fs)))
            Next
        Next
        Memo2.AppendText(vbCrLf & "END                                                                           ")
        Memo2.SelectionStart = 0
        Memo2.ScrollToCaret()
    End Sub

    Private Sub CSVreport(iss() As Integer)
        Memo2.Clear()
        'Dim Stars As Integer

        Memo2.AppendText(String.Format("""Starname"",""{0}""", pd.SName))
        Memo2.AppendText(vbCrLf & String.Format("""Star RA"",{0}", pd.SRA))
        Memo2.AppendText(vbCrLf & String.Format("""Star DEC"",{0}", pd.SDec))
        Memo2.AppendText(vbCrLf & String.Format("""ChartID"",""{0}""", pd.Chart))

        Memo2.AppendText(vbCrLf & String.Format("""AUID"",""RA"",""Dec"",""Label"""))
        For i As Integer = 0 To Len(pd.Filters) - 1
            Memo2.AppendText(String.Format(",""{0} mag"",""{0} err""", pd.Filters(i))) ' // 2 cols: mag And err
        Next

        Dim Stars As Integer
        For j As Integer = 0 To iss.Length - 1
            Stars = iss(j)
            Memo2.AppendText(vbCrLf & String.Format("{0}, {1}, {2}, {3}", pd.comp(Stars).AUID, pd.comp(Stars).RA, pd.comp(Stars).Dec, pd.comp(Stars).Label))
            For i As Integer = 0 To Len(pd.Filters) - 1
                Memo2.AppendText(String.Format(", {0}, {1}", pd.fd(Stars, i).mag, pd.fd(Stars, i).sd)) '; // 2 cols: mag And err
            Next
        Next
        Memo2.SelectionStart = 0
        Memo2.ScrollToCaret()
    End Sub

    Public Shared Function ToStr(ByVal pObject As Object) As String
        Try
            If IsDBNull(pObject) Then
                Return ""
            Else
                Return CStr(pObject)
            End If
        Catch
            Return ""
        End Try
    End Function

    Private Sub Labels1_Click(sender As Object, e As EventArgs) Handles Labels1.Click
        My.Settings.Labels = Labels1.Checked
    End Sub

    Private Sub CSV1_Click(sender As Object, e As EventArgs) Handles CSV1.Click
        My.Settings.CSV = CSV1.Checked
    End Sub
End Class
