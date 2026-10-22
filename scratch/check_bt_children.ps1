$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/AI/AI_Skeleton/BT_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
# In UE BT assets, Children are stored in BTComposite_Sequence / BTComposite_Selector.
# Let's inspect all strings and class names around Children
$idx = 0
while (($idx = $chars.IndexOf("Children", $idx)) -ge 0) {
    $snippet = $chars.Substring($idx, [Math]::Min(400, $chars.Length - $idx))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "Children at $idx : $clean"
    $idx += 8
}
