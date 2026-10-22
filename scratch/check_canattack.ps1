$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/Characters/Enemies/Skeleton/BP_MIR_SkeletonBASE.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$idx = 0
while (($idx = $chars.IndexOf("CanAttack", $idx)) -ge 0) {
    $snippet = $chars.Substring([Math]::Max(0, $idx - 50), [Math]::Min(200, $chars.Length - $idx + 50))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "CanAttack at $idx : $clean"
    $idx += 9
}
