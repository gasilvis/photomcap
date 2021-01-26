<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.File1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.SaveResultFile1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.Exit1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.Options1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.Labels1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.CSV1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.Help1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.Help2 = New System.Windows.Forms.ToolStripMenuItem()
        Me.About1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.DoitButton = New System.Windows.Forms.Button()
        Me.chartEdit = New System.Windows.Forms.TextBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.RedoButton = New System.Windows.Forms.Button()
        Me.KLabelEdit = New System.Windows.Forms.TextBox()
        Me.CLabelEdit = New System.Windows.Forms.TextBox()
        Me.Memo2 = New System.Windows.Forms.TextBox()
        Me.Button4 = New System.Windows.Forms.Button()
        Me.Button2 = New System.Windows.Forms.Button()
        Me.SaveFileDialog1 = New System.Windows.Forms.SaveFileDialog()
        Me.MenuStrip1.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.SuspendLayout()
        '
        'MenuStrip1
        '
        Me.MenuStrip1.ImageScalingSize = New System.Drawing.Size(20, 20)
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.File1, Me.Options1, Me.Help1})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Padding = New System.Windows.Forms.Padding(8, 3, 0, 3)
        Me.MenuStrip1.Size = New System.Drawing.Size(1211, 30)
        Me.MenuStrip1.TabIndex = 0
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'File1
        '
        Me.File1.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.SaveResultFile1, Me.Exit1})
        Me.File1.Name = "File1"
        Me.File1.Size = New System.Drawing.Size(44, 24)
        Me.File1.Text = "&File"
        '
        'SaveResultFile1
        '
        Me.SaveResultFile1.Name = "SaveResultFile1"
        Me.SaveResultFile1.Size = New System.Drawing.Size(216, 26)
        Me.SaveResultFile1.Text = "&Save result file"
        '
        'Exit1
        '
        Me.Exit1.Name = "Exit1"
        Me.Exit1.Size = New System.Drawing.Size(216, 26)
        Me.Exit1.Text = "E&xit"
        '
        'Options1
        '
        Me.Options1.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.Labels1, Me.CSV1})
        Me.Options1.Name = "Options1"
        Me.Options1.Size = New System.Drawing.Size(73, 24)
        Me.Options1.Text = "&Options"
        '
        'Labels1
        '
        Me.Labels1.CheckOnClick = True
        Me.Labels1.Name = "Labels1"
        Me.Labels1.Size = New System.Drawing.Size(216, 26)
        Me.Labels1.Text = "&Labels"
        Me.Labels1.ToolTipText = "Include star labels in the star identifier"
        '
        'CSV1
        '
        Me.CSV1.CheckOnClick = True
        Me.CSV1.Name = "CSV1"
        Me.CSV1.Size = New System.Drawing.Size(216, 26)
        Me.CSV1.Text = "&CSV"
        Me.CSV1.ToolTipText = "Output file in CSV format"
        '
        'Help1
        '
        Me.Help1.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.Help2, Me.About1})
        Me.Help1.Name = "Help1"
        Me.Help1.Size = New System.Drawing.Size(53, 24)
        Me.Help1.Text = "&Help"
        '
        'Help2
        '
        Me.Help2.Name = "Help2"
        Me.Help2.Size = New System.Drawing.Size(125, 26)
        Me.Help2.Text = "&Help"
        '
        'About1
        '
        Me.About1.Name = "About1"
        Me.About1.Size = New System.Drawing.Size(125, 26)
        Me.About1.Text = "&About"
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.DoitButton)
        Me.GroupBox1.Controls.Add(Me.chartEdit)
        Me.GroupBox1.Controls.Add(Me.Label1)
        Me.GroupBox1.Location = New System.Drawing.Point(80, 57)
        Me.GroupBox1.Margin = New System.Windows.Forms.Padding(2)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Padding = New System.Windows.Forms.Padding(2)
        Me.GroupBox1.Size = New System.Drawing.Size(571, 84)
        Me.GroupBox1.TabIndex = 1
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Source"
        '
        'DoitButton
        '
        Me.DoitButton.Location = New System.Drawing.Point(384, 31)
        Me.DoitButton.Name = "DoitButton"
        Me.DoitButton.Size = New System.Drawing.Size(111, 30)
        Me.DoitButton.TabIndex = 2
        Me.DoitButton.Text = "Do it!"
        Me.DoitButton.UseVisualStyleBackColor = True
        '
        'chartEdit
        '
        Me.chartEdit.Location = New System.Drawing.Point(210, 31)
        Me.chartEdit.Name = "chartEdit"
        Me.chartEdit.Size = New System.Drawing.Size(115, 30)
        Me.chartEdit.TabIndex = 1
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(26, 34)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(178, 23)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Enter chartID here:"
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.RedoButton)
        Me.GroupBox2.Controls.Add(Me.KLabelEdit)
        Me.GroupBox2.Controls.Add(Me.CLabelEdit)
        Me.GroupBox2.Location = New System.Drawing.Point(694, 57)
        Me.GroupBox2.Margin = New System.Windows.Forms.Padding(2)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Padding = New System.Windows.Forms.Padding(2)
        Me.GroupBox2.Size = New System.Drawing.Size(492, 84)
        Me.GroupBox2.TabIndex = 2
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Re Do "
        '
        'RedoButton
        '
        Me.RedoButton.Location = New System.Drawing.Point(358, 34)
        Me.RedoButton.Name = "RedoButton"
        Me.RedoButton.Size = New System.Drawing.Size(115, 30)
        Me.RedoButton.TabIndex = 3
        Me.RedoButton.Text = "Re Do it!"
        Me.RedoButton.UseVisualStyleBackColor = True
        '
        'KLabelEdit
        '
        Me.KLabelEdit.Location = New System.Drawing.Point(185, 32)
        Me.KLabelEdit.Name = "KLabelEdit"
        Me.KLabelEdit.Size = New System.Drawing.Size(120, 30)
        Me.KLabelEdit.TabIndex = 1
        Me.KLabelEdit.Text = "K label"
        '
        'CLabelEdit
        '
        Me.CLabelEdit.Location = New System.Drawing.Point(38, 32)
        Me.CLabelEdit.Name = "CLabelEdit"
        Me.CLabelEdit.Size = New System.Drawing.Size(120, 30)
        Me.CLabelEdit.TabIndex = 0
        Me.CLabelEdit.Text = "C label"
        '
        'Memo2
        '
        Me.Memo2.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Memo2.Location = New System.Drawing.Point(12, 203)
        Me.Memo2.Multiline = True
        Me.Memo2.Name = "Memo2"
        Me.Memo2.ScrollBars = System.Windows.Forms.ScrollBars.Both
        Me.Memo2.Size = New System.Drawing.Size(1187, 432)
        Me.Memo2.TabIndex = 3
        Me.Memo2.WordWrap = False
        '
        'Button4
        '
        Me.Button4.Location = New System.Drawing.Point(22, 146)
        Me.Button4.Name = "Button4"
        Me.Button4.Size = New System.Drawing.Size(97, 36)
        Me.Button4.TabIndex = 4
        Me.Button4.Text = "clear"
        Me.Button4.UseVisualStyleBackColor = True
        '
        'Button2
        '
        Me.Button2.Location = New System.Drawing.Point(140, 146)
        Me.Button2.Name = "Button2"
        Me.Button2.Size = New System.Drawing.Size(163, 36)
        Me.Button2.TabIndex = 5
        Me.Button2.Text = "to Clipboard"
        Me.Button2.UseVisualStyleBackColor = True
        '
        'SaveFileDialog1
        '
        Me.SaveFileDialog1.DefaultExt = "star"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(11.0!, 23.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1211, 647)
        Me.Controls.Add(Me.Button2)
        Me.Controls.Add(Me.Button4)
        Me.Controls.Add(Me.Memo2)
        Me.Controls.Add(Me.GroupBox2)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.MenuStrip1)
        Me.Font = New System.Drawing.Font("Arial", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MainMenuStrip = Me.MenuStrip1
        Me.Margin = New System.Windows.Forms.Padding(4)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents MenuStrip1 As MenuStrip
    Friend WithEvents File1 As ToolStripMenuItem
    Friend WithEvents SaveResultFile1 As ToolStripMenuItem
    Friend WithEvents Exit1 As ToolStripMenuItem
    Friend WithEvents Options1 As ToolStripMenuItem
    Friend WithEvents Labels1 As ToolStripMenuItem
    Friend WithEvents CSV1 As ToolStripMenuItem
    Friend WithEvents Help1 As ToolStripMenuItem
    Friend WithEvents Help2 As ToolStripMenuItem
    Friend WithEvents About1 As ToolStripMenuItem
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents DoitButton As Button
    Friend WithEvents chartEdit As TextBox
    Friend WithEvents Label1 As Label
    Friend WithEvents GroupBox2 As GroupBox
    Friend WithEvents RedoButton As Button
    Friend WithEvents KLabelEdit As TextBox
    Friend WithEvents CLabelEdit As TextBox
    Friend WithEvents Memo2 As TextBox
    Friend WithEvents Button4 As Button
    Friend WithEvents Button2 As Button
    Friend WithEvents SaveFileDialog1 As SaveFileDialog
End Class
