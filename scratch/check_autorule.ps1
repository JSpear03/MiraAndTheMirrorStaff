$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
# Search for bAutomaticRuleBasedOnSequencePlayerInState
$idx = $chars.IndexOf("bAutomaticRuleBasedOnSequencePlayerInState")
Write-Output "bAutomaticRuleBasedOnSequencePlayerInState at $idx"
if ($idx -ge 0) {
    $snippet = $chars.Substring([Math]::Max(0, $idx - 50), [Math]::Min(200, $chars.Length - $idx + 50))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "Snippet: $clean"
}
