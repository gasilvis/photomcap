object Form1: TForm1
  Left = 58
  Top = 158
  Width = 979
  Height = 563
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = 
    'PhotomCap : Capturing AAVSO photometry into AIPWin STAR format, ' +
    'and other formats too'
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
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 22
  object Label1: TLabel
    Left = 896
    Top = 0
    Width = 40
    Height = 22
    Caption = 'ver 17'
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
    ScrollBars = ssVertical
    TabOrder = 1
    WordWrap = False
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
    ScrollBars = ssVertical
    TabOrder = 0
    WordWrap = False
  end
  object Memo4: TMemo
    Left = 128
    Top = 256
    Width = 817
    Height = 233
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 5
  end
  object DoitButton: TButton
    Left = 24
    Top = 96
    Width = 75
    Height = 25
    Hint = 'Click to convert to STAR format'
    Caption = 'Do It!'
    TabOrder = 2
    OnClick = DoitButtonClick
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
    Left = 16
    Top = 416
    Width = 97
    Height = 25
    Hint = 'Copy output to clibboard'
    Caption = 'To Clipboard'
    TabOrder = 4
    OnClick = Button2Click
  end
  object Button4: TButton
    Left = 72
    Top = 200
    Width = 49
    Height = 25
    Caption = 'clear'
    TabOrder = 6
    OnClick = Button4Click
  end
  object SPaltError: TCheckBox
    Left = 208
    Top = 488
    Width = 305
    Height = 17
    Hint = 'See help file'
    Caption = 'Alternate error computation'
    TabOrder = 7
    Visible = False
    OnClick = SPaltErrorClick
  end
  object GroupBox1: TGroupBox
    Left = 16
    Top = 264
    Width = 97
    Height = 137
    Hint = 'Re do the output with just 2 comp stars'
    Caption = 'ReDo'
    TabOrder = 8
    object CLabelEdit: TEdit
      Left = 16
      Top = 24
      Width = 60
      Height = 30
      TabOrder = 0
      Text = 'C label'
    end
    object KLabelEdit: TEdit
      Left = 16
      Top = 64
      Width = 60
      Height = 30
      TabOrder = 1
      Text = 'K label'
    end
    object ReDoButton: TButton
      Left = 16
      Top = 104
      Width = 60
      Height = 25
      Caption = 'ReDo'
      TabOrder = 2
      OnClick = ReDoButtonClick
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 88
    Top = 448
  end
  object ElasticForm1: TElasticForm
    DesignScreenWidth = 1448
    DesignScreenHeight = 908
    DesignPixelsPerInch = 120
    DesignFormWidth = 979
    DesignFormHeight = 563
    DesignFormClientWidth = 971
    DesignFormClientHeight = 505
    DesignFormLeft = 58
    DesignFormTop = 158
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Arial Narrow'
    Font.Style = []
    Version = 700
    Left = 80
    Top = 16
  end
  object MainMenu1: TMainMenu
    Left = 32
    Top = 16
    object File1: TMenuItem
      Caption = 'File'
      object LoadSourceFile1: TMenuItem
        Caption = 'Load Source File'
        OnClick = LoadSourceFile1Click
      end
      object SaveSourceFile1: TMenuItem
        Caption = 'Save Source File'
        OnClick = SaveSourceFile1Click
      end
      object SaveResultFile1: TMenuItem
        Caption = '&Save Result File'
        OnClick = SaveResultFile1Click
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object Options1: TMenuItem
      Caption = 'Options'
      object Labels1: TMenuItem
        Caption = 'Labels'
        Hint = 'Add Labels to the StarID field'
        RadioItem = True
        OnClick = Labels1Click
      end
      object CSV1: TMenuItem
        Caption = 'CSV'
        RadioItem = True
        OnClick = CSV1Click
      end
      object SeqPlot1: TMenuItem
        Caption = 'SeqPlot'
        OnClick = SeqPlot1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object Help2: TMenuItem
        Caption = 'Help'
        OnClick = Help1Click
      end
      object About1: TMenuItem
        Caption = 'About'
        OnClick = About1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 8
    Top = 448
  end
  object SaveDialog2: TSaveDialog
    Left = 48
    Top = 448
  end
end
