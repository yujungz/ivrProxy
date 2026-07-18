Set WshShell = CreateObject("WScript.Shell")

Dim  sessionId, pathPrj, titleCode, titleFlag, strClose, autoClose
sessionId = "c8b4135d-09a1-4543-a750-aaa8fd51f6b8"
pathPrj = "D:\work\wyprj\prj\ivrProxy\IwrApp\"

pwshExe = "D:\prgapp\PowerShell\7\pwsh.exe"
titleCode = " "
strClose  = " "

' 是否自动关闭窗口
autoClose = 1
' 改变窗口标题： 0-不改变，1-改变1次，2-循环改变
titleFlag = 2
' ----
' 从项目路径末尾提取目录名作为标题后缀（BillSumExt）
Dim p, pos
p = pathPrj
If Right(p, 1) = "\" Then p = Left(p, Len(p) - 1)
pos = InStrRev(p, "\")
If pos > 0 Then
    prjName = Mid(p, pos + 1)
Else
    prjName = p
End If
' 专用
prjName = "Billsum"

If     titleFlag = 1 Then
    titleCode = " Start-ThreadJob { Start-Sleep -Seconds 5; [System.Console]::Title = 'Claude code-" & prjName & "' } | Out-Null ;"
ElseIf titleFlag = 2 Then
    titleCode = " $null = Start-ThreadJob {   while($true){ [System.Console]::Title = 'Claude code-" & prjName & "'; Start-Sleep -Milliseconds 3000 } };"
End If
' ----

If autoClose = 0 Then
    strClose = "; Write-Host ''; Write-Host 'Claude exited. Press Enter to close.'; Read-Host "
End If

WshShell.Run """" & pwshExe & """ -NoLogo -NoProfile  -Command ""& {  Set-Location '" & pathPrj &  "';" & titleCode & " claude --resume " & sessionId & strClose & "}""" , 1

Set WshShell = Nothing