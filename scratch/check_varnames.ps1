$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
# Search for VariableName
$idx = 0
while (($idx = $chars.IndexOf("VariableName", $idx)) -ge 0) {
    $snippet = $chars.Substring($idx, [Math]::Min(100, $chars.Length - $idx))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "VariableName at $idx : $clean"
    $idx += 12
}
