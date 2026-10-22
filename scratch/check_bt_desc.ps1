$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/AI/AI_Skeleton/BT_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$idx = 0
while (($idx = $chars.IndexOf("CachedDescription", $idx)) -ge 0) {
    $snippet = $chars.Substring($idx, [Math]::Min(200, $chars.Length - $idx))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "Desc at $idx : $clean"
    $idx += 17
}
