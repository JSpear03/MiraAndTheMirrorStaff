$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
# Search for AnimStateTransitionNode
$idx = 0
while (($idx = $chars.IndexOf("AnimStateTransitionNode", $idx)) -ge 0) {
    $snippet = $chars.Substring($idx, [Math]::Min(300, $chars.Length - $idx))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "Transition at $idx : $clean"
    $idx += 23
}
