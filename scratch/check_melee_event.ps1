$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/Characters/Enemies/Skeleton/BP_MIR_SkeletonBASE.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$idx = $chars.IndexOf("OnMeleeAttackTriggered_Event")
while ($idx -ge 0) {
    $snippet = $chars.Substring([Math]::Max(0, $idx - 50), [Math]::Min(300, $chars.Length - $idx + 50))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "At $idx : $clean"
    $idx = $chars.IndexOf("OnMeleeAttackTriggered_Event", $idx + 28)
}
