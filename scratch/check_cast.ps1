$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$idx = $chars.IndexOf("K2Node_DynamicCast_AsBP_MIR_Skeleton_BASE")
if ($idx -ge 0) {
    $snippet = $chars.Substring($idx, [Math]::Min(1000, $chars.Length - $idx))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "Cast: $clean"
}
