object Form1: TForm1
  Left = 121
  Top = 167
  Width = 979
  Height = 563
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = 'PhotomCap : Capturing AAVSO photometry into AIPWin STAR format'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -17
  Font.Name = 'Arial Narrow'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Scaled = False
  ShowHint = True
  PixelsPerInch = 120
  TextHeight = 22
  object Label1: TLabel
    Left = 896
    Top = 0
    Width = 34
    Height = 22
    Caption = 'Ver 3'
  end
  object Label2: TLabel
    Left = 144
    Top = 8
    Width = 225
    Height = 22
    Caption = 'Paste AAVSO photometry data here'
  end
  object Label3: TLabel
    Left = 136
    Top = 232
    Width = 239
    Height = 22
    Caption = 'AIPWin MMT STAR file appears here '
  end
  object Memo1: TMemo
    Left = 128
    Top = 32
    Width = 817
    Height = 201
    Hint = 'Paste text fo AAVSO photometry report '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    WordWrap = False
  end
  object Memo2: TMemo
    Left = 128
    Top = 256
    Width = 817
    Height = 233
    Hint = 'Here will appear the .photometry data in STAR format'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    WordWrap = False
  end
  object Button1: TButton
    Left = 24
    Top = 120
    Width = 75
    Height = 25
    Hint = 'Click to convert to STAR format'
    Caption = 'Do It!'
    TabOrder = 2
    OnClick = Button1Click
  end
  object DDCheckBox: TCheckBox
    Left = 584
    Top = 488
    Width = 241
    Height = 17
    Caption = 'Decimal Degrees'
    Checked = True
    State = cbChecked
    TabOrder = 3
    Visible = False
  end
  object Button2: TButton
    Left = 24
    Top = 344
    Width = 75
    Height = 25
    Hint = 'Save in the AIPWin/Data directory'
    Caption = 'Save It!'
    TabOrder = 4
    OnClick = Button2Click
  end
  object GroupBox1: TGroupBox
    Left = 168
    Top = 8
    Width = 713
    Height = 473
    Caption = 'PhotomCap Help'
    TabOrder = 5
    Visible = False
    object Memo3: TMemo
      Left = 8
      Top = 24
      Width = 697
      Height = 409
      Lines.Strings = (
        
          'This PhotomCap program is a utility to "capture" the AAVSO photo' +
          'metry data from a web page and convert it '
        'into a AIPWin MMT Tool .STAR file.'
        ''
        'The process:'
        
          '1. Go to the AAVSO website and use VSP to select your star chart' +
          ': http://www.aavso.org/observing/charts/vsp/'
        
          '2. From the chart page click on "[Comparison Stars]" to bring up' +
          ' the photometry data page.'
        
          '3. Click on the page and hit Ctrl-A to select all of the text da' +
          'ta.'
        '4. Hit Ctrl-C to copy that data.'
        
          '5. Now go to PhotomCap and click on the upper window. Hit Ctrl-V' +
          ' to copy in the raw data.'
        
          '6. Now click the "Do it!" button. Your .STAR data will appear in' +
          ' the lower window.'
        
          '7. Click the "Save it" button and save the file in the AIPWin/Da' +
          'ta directory.'
        
          '8. Now from Stars tab of the AIPWin MMT you can hit recall and o' +
          'pen this file.'
        'Voila! Comparison star data loaded, complete and accurate.'
        ''
        
          'Any questions or problems, please contact me, George Silvis, at ' +
          'George@GASilvis.net'
        ''
        
          'If you have a particular capture that fails, send me the PhotomC' +
          'ap version number and the chartid. Thanks.'
        '')
      TabOrder = 0
    end
    object Button3: TButton
      Left = 296
      Top = 440
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 1
      OnClick = Button3Click
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 880
    Top = 248
  end
  object ElasticForm1: TElasticForm
    DesignScreenWidth = 1448
    DesignScreenHeight = 908
    DesignPixelsPerInch = 120
    DesignFormWidth = 979
    DesignFormHeight = 563
    DesignFormClientWidth = 971
    DesignFormClientHeight = 507
    DesignFormLeft = 121
    DesignFormTop = 167
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Arial Narrow'
    Font.Style = []
    Version = 700
    Left = 896
    Top = 192
  end
  object MainMenu1: TMainMenu
    Left = 64
    Top = 48
    object Help1: TMenuItem
      Caption = 'Help'
      OnClick = Help1Click
    end
  end
end
