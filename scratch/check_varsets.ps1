$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/Characters/Enemies/Skeleton/BP_MIR_SkeletonBASE.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
# Search for VariableSet
$idx = 0
while (($idx = $chars.IndexOf("K2Node_VariableSet", $idx)) -ge 0) {
    $snippet = $chars.Substring($idx, [Math]::Min(150, $chars.Length - $idx))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "VariableSet at $idx : $clean"
    $idx += 18
}
