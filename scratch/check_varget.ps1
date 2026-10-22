$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
# Search for MemberReference or VariableGet in ABP
$idx = 0
while (($idx = $chars.IndexOf("K2Node_VariableGet", $idx)) -ge 0) {
    $snippet = $chars.Substring($idx, [Math]::Min(200, $chars.Length - $idx))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "VariableGet at $idx : $clean"
    $idx += 18
}
