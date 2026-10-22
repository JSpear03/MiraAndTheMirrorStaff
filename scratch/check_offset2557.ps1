$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/Characters/Enemies/Skeleton/BP_MIR_SkeletonBASE.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$snippet = $chars.Substring([Math]::Max(0, 2557 - 200), 500)
$clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
Write-Output $clean
