$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$snippet = $chars.Substring(14000, 2000)
$clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
Write-Output $clean
