object fAbout: TfAbout
  Left = 419
  Top = 203
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  ClientHeight = 301
  ClientWidth = 479
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 120
  TextHeight = 23
  object Panel1: TPanel
    Left = 16
    Top = 16
    Width = 449
    Height = 225
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelOuter = bvLowered
    TabOrder = 0
    object Image1: TImage
      Left = 24
      Top = 24
      Width = 40
      Height = 40
      AutoSize = True
      Picture.Data = {
        055449636F6E0000010001002020100000000000E80200001600000028000000
        2000000040000000010004000000000080020000000000000000000000000000
        0000000000000000000080000080000000808000800000008000800080800000
        80808000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000
        FFFFFF00CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCF8CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        F8CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCF8CCCCCCCCCF8CCCCCC8FFFCCCFFFF8C
        F8FF8CCCCCCF8CCCCC8FCC8FCEFDEF8CFFCEFDCCCCCF8CCCCCF8CCCFDCF8CF8C
        F8CCF8CCCCCFFFFFCCFDCCCCCCCFFF8CF8CCF8CCCCCF8CCF8EFDCCCCCCCCCF8C
        F8CCF8CCCCCF8CCFFCFDCCCCCCFDEFDCFFCEFDCCCCCF8CCFFCF8CCCFDC8FFFCC
        F8FF8CCCCCCF8CCF8C8FCC8FCCCCCCCCCCCCCCCCCCCFFFFFCCC8FFFCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCC00000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000}
      Stretch = True
    end
    object Label1: TLabel
      Left = 80
      Top = 24
      Width = 152
      Height = 32
      Caption = 'PhotomCap'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -30
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 80
      Top = 72
      Width = 345
      Height = 20
      Caption = 'Utility to capture AAVSO chart photometry '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 120
      Top = 96
      Width = 209
      Height = 20
      Caption = 'into AIPWin .STAR format'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 88
      Top = 152
      Width = 288
      Height = 16
      Alignment = taCenter
      Caption = 'Send complaints, suggestions and kudos '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label5: TLabel
      Left = 88
      Top = 176
      Width = 299
      Height = 16
      Alignment = taCenter
      Caption = 'to George Silvis: PhotomCap@GASilvis.net'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label6: TLabel
      Left = 48
      Top = 128
      Width = 360
      Height = 16
      Caption = 'http://www.gasilvis.com/PhotomCap/PhotomCap.exe'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object Button1: TButton
    Left = 174
    Top = 258
    Width = 124
    Height = 31
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Okay'
    TabOrder = 1
    OnClick = Button1Click
  end
end
