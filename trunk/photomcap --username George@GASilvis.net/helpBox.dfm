object Form2: TForm2
  Left = 104
  Top = 127
  Width = 979
  Height = 563
  Caption = 'PhotomCap Help'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object Button1: TButton
    Left = 40
    Top = 488
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 32
    Top = 32
    Width = 913
    Height = 441
    Lines.Strings = (
      'Introduction'
      ''
      
        'This PhotomCap program is a utility to "capture" the AAVSO photo' +
        'metry data from the comparison star data page of their chart pag' +
        'es. The data is then '
      
        'made available as a CSV file that can be imported to a spreadshe' +
        'et, or as a .STAR file compatible with the new Magnitude Measurm' +
        'ent Tool (MMT) '
      'feature of AIPWin.'
      ''
      'The process:'
      
        '1. Go to the AAVSO website and use VSP to select your star chart' +
        ': http://www.aavso.org/vsp/chart'
      
        '2. From the chart page click on "Photometry Table for this Chart' +
        '" to bring up the photometry data page.'
      ''
      
        '3. Click on the page and hit Ctrl-A to select all of the text da' +
        'ta.'
      '4. Hit Ctrl-C to copy that data.'
      
        '5. Now go to PhotomCap and click on the upper window. Hit Ctrl-V' +
        ' to copy in the raw data.'
      
        '6. Now click the "Do it!" button. The lower window will now hold' +
        ' either your CSV file or .STAR file, depending upon which option' +
        ' you choose.'
      
        '7. Click the "Save it" button and save the .STAR file in the AIP' +
        'Win/Data directory or the CSV file where ever.'
      
        '8. Now from Stars tab of the AIPWin MMT you can hit recall and o' +
        'pen this file.'
      ''
      'Voila! Comparison star data loaded, complete and accurate.'
      ''
      
        'Any questions or problems, please contact me, George Silvis, at ' +
        'George@GASilvis.net'
      ''
      
        'If you have a particular capture that fails, send me the PhotomC' +
        'ap version number and the chartid. I'#39'll sort it out. Thanks.'
      ''
      ''
      'Options:'
      
        '- Labels: The photometry tables have a column of Labels that app' +
        'ear on the chart. With these you match up the labled star to the' +
        ' table data. Clicking '
      
        'this option will include the label with the star ID in the STAR ' +
        'format. This works well in MMT.'
      ''
      
        '- CSV: Capture the data to a standard CSV format suitable for a ' +
        'spreadsheet'
      ''
      
        '- SegPlot: Input a txt file from the AAVSO SeqPlot program and o' +
        'utput a STAR file for MMT. These files are CSV with fields: #Lab' +
        'el, h, m, s, d, m, s, V, '
      
        'Verr, B-V, B-Verr, U-B, U-Berr, V-R,VRerr, R-I, R-Ierr, V-I, V-I' +
        'err, Source, #Comments'
      
        ' - Use the comment field for star identifier; if blank, the labe' +
        'l will be used'
      
        ' - The first line of the file will be the target, the others com' +
        'p stars. Remember, you can edit the input window. If the target ' +
        'is a field, dup the first line... '
      ''
      
        ' - Error computations: The error for the computed filters (B, U,' +
        ' R and I) is computed by adding its components in quadrature. Eg' +
        '. Berr= sqrt( B-Verr^2  + '
      
        'Verr^2) because we are computing B= B-V  + V. But this is conser' +
        'vative. '
      
        'An alternative is available if you check the Alternate Error Com' +
        'putation at the bottom of the form. If you assume, for example, ' +
        'that the B-V was computed '
      
        'from V and B in the source data, then you might reasonably assum' +
        'e that Berr= sqrt(B-Verr^2  - Verr^2). Ie, you back into the Ber' +
        'r on the assumption they '
      
        'computed B-Verr as sqrt(Berr^2  + Verr^2). In cases where B-Verr' +
        ' is greater than Verr, this assumption breaks down as this error' +
        ' computation generates '
      
        'an imaginary result. In such a case the Berr would be taken as e' +
        'qual to Verr.'
      ''
      '- Please Note this advisory from the AAVSO sequence team:'
      '-------------'
      
        'As a member of the AAVSO Sequence Team I would be very uncomfort' +
        'able with anyone using this feature and then reporting photometr' +
        'y data to the '
      'AAVSO Data Base:'
      
        '1) Different survey'#39's have differing points at which they satura' +
        'te and or error out on the faint end and this information is not' +
        ' available via SeqPlot.'
      ''
      
        '2) From time to time we detect data in SeqPlot that is not even ' +
        'suitable for a sequence and this might not be so obvious to any ' +
        'one using this new pgm '
      'feature.'
      ''
      
        '3) There is no easy way to match specific SeqPlot data, unless s' +
        'equenced by the team and available via the AAVSO VSP, to specifi' +
        'c Comp & Check '
      'Stars for reporting purposes.'
      ''
      'Tim Crawford, CTX'
      'AAVSO Sequence Team'
      ''
      '------------')
    ScrollBars = ssVertical
    TabOrder = 1
  end
end
